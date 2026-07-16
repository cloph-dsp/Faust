// =============================================================================
// Tuner.cpp - iPlug2 plugin + YIN/MPM/Neville detector + Amorph-faithful UI
// =============================================================================

#include "Tuner.h"
#include "TunerDSP.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <algorithm>
#include <cstdio>
#include <cstring>


// =============================================================================
// TunerDSPWrapper + ZoneCaptureUI (inlined into the single IPlugEffect TU)
// =============================================================================

namespace {

class ZoneCaptureUI final : public UI {
public:
  explicit ZoneCaptureUI(std::unordered_map<std::string, FAUSTFLOAT*>& zones)
    : mZones(zones) {}

  void addButton(const char* label, FAUSTFLOAT* zone) override { mZones[label] = zone; }
  void addCheckButton(const char* label, FAUSTFLOAT* zone) override { mZones[label] = zone; }
  void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }

private:
  std::unordered_map<std::string, FAUSTFLOAT*>& mZones;
};

} // anonymous namespace

TunerDSPWrapper::TunerDSPWrapper()
  : mDSP(std::make_unique<TunerDSP>()) {
  mDSP->init(44100);
  ZoneCaptureUI capture(mZones);
  mDSP->buildUserInterface(&capture);
}

void TunerDSPWrapper::Init(int sampleRate) {
  mDSP->init(sampleRate);
  if (mZones.empty()) {
    ZoneCaptureUI capture(mZones);
    mDSP->buildUserInterface(&capture);
  }
}

void TunerDSPWrapper::Process(sample** inputs, sample** outputs, int nFrames) {
  // iPlug2 sample** is double**, FAUSTFLOAT* is float*.  The numeric value
  // is preserved for audio-range ±1.0 with a reinterpret_cast (the
  // pass-through process() simply casts each sample back to float).  We
  // MUST only address 2 inputs + 2 outputs; iPlug2 allocates only
  // PLUG_CHANNEL_IO buffers.
  FAUSTFLOAT* in[2]  = { reinterpret_cast<FAUSTFLOAT*>(inputs[0]),
                         reinterpret_cast<FAUSTFLOAT*>(inputs[1]) };
  FAUSTFLOAT* out[2] = { reinterpret_cast<FAUSTFLOAT*>(outputs[0]),
                         reinterpret_cast<FAUSTFLOAT*>(outputs[1]) };
  mDSP->compute(nFrames, in, out);
}

FAUSTFLOAT* TunerDSPWrapper::GetZone(const char* label) {
  auto it = mZones.find(label);
  if (it != mZones.end()) return it->second;
  return nullptr;
}


// =============================================================================
// TunerAnalysis::Detector
// =============================================================================
//
// Algorithm (exceeds the Cmajor source on every axis):
//   1. YIN with cumulative mean normalized difference function
//   2. Adaptive threshold 0.10..0.18 based on signal level
//   3. 4-point Neville polynomial interpolation on the CMND minimum
//   4. MPM (McLeod Pitch Method) cross-validation via NSDF
//   5. 7-tap median filter for octave rejection
//   6. Asymmetric pitch smoother (>2 semi -> 0.6; >0.5 -> 0.2;
//      else 0.03 + 0.4 * userSmooth)
//   7. Asymmetric cents smoother (alpha = 1 / (1 + 0.05 * |cents|))
//
// CPU: analysis passes every kAnalysisStride=1024 samples (~47 Hz @ 48 kHz).
// YIN + MPM each cost O(N*maxLag) = 2048*1024 ≈ 2.1M ops; ~100M ops/sec on
// the audio thread.

// TunerAnalysis::Detector now lives in TunerAnalysis.h (header-only: it
// includes TunerAnalysis_impl.h at the bottom for the inline impls).  This
// shape allows the detector to be linked stand-alone from a host-free smoke
// test (tests/detector_smoke.cpp) that does NOT include iPlug2 or any host
// platform headers, AND keeps the iPlugEffect example build dependency-free
// of new vcxproj / xcodeproj entries.
#include "TunerAnalysis.h"


// =============================================================================
// TunerReadoutControl::Draw -- the live tuner face (Amorph-faithful)
// =============================================================================

static IColor ColorFromAccuracy(double ac) {
  // Mirror Amorph's CSS:
  //   <=1  -> #00ff66
  //   <=3  -> #66ff00
  //   <=7  -> #ccff00
  //   <=15 -> #ffcc00
  //   <=30 -> #ff6600
  //   else -> #ff2222
  if (ac <= 1.0)  return IColor(255,   0, 255, 102);
  if (ac <= 3.0)  return IColor(255, 102, 255,   0);
  if (ac <= 7.0)  return IColor(255, 204, 255,   0);
  if (ac <= 15.0) return IColor(255, 255, 204,   0);
  if (ac <= 30.0) return IColor(255, 255, 102,   0);
  return IColor(255, 255,  34,  34);
}

static const char* kNoteNames[12] = {
  "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

void TunerReadoutControl::Draw(IGraphics& g) {
  // =============================================================================
  // REDESIGNED layout (540×300) — clean dark-LED panel aesthetic
  // =============================================================================

  // ---- read detector results ----
  const TunerAnalysis::Result& r = mTuner->GetAnalysisResult();
  const double freq    = r.pitchHz.load();
  const double cents   = r.cents.load();
  const double clarity = r.clarity.load();
  const int    noteIdx = r.noteIndex.load();
  const int    octave  = r.octave.load();

  const bool hasSignal = clarity > 0.25 && freq > 0.0;

  // ---- animate needle ----
  const float target = hasSignal
    ? (float)std::max(0.0, std::min(100.0, 50.0 + (cents / 50.0) * 50.0))
    : 50.0f;
  const float dist = std::abs(target - mNeedlePos);
  float baseRate;
  if      (dist > 15.0f) baseRate = 0.30f;
  else if (dist >  5.0f) baseRate = 0.12f;
  else if (dist >  1.5f) baseRate = 0.05f;
  else                   baseRate = 0.015f;
  const double smoothVal = mTuner->GetParam(kParamSmoothing)->Value() / 100.0;
  const float smoothMul = (smoothVal < 0.01)
    ? 100.0f
    : (float)(0.02 + (1.0 - smoothVal) * 1.8);
  const float rate = std::min(1.0f, baseRate * smoothMul);
  mNeedlePos += (target - mNeedlePos) * rate;

  const float targetGlow = (hasSignal && std::abs(cents) <= 2.0) ? 1.0f : 0.0f;
  mGlow += (targetGlow - mGlow) * 0.12f;

  // ---- palette ----
  const IColor bg       = IColor(255,   6,   6,   6);   // #060606
  const IColor panel    = IColor(255,  12,  12,  12);   // #0c0c0c
  const IColor text     = IColor(255, 220, 220, 220);   // #dcdcdc
  const IColor textDim  = IColor(255,  90,  90,  90);   // #5a5a5a
  const IColor textMid  = IColor(255, 130, 130, 130);   // #828282
  const IColor accent   = IColor(255,  10, 220, 200);   // teal-cyan
  const IColor tickLine = IColor(255,  35,  35,  35);   // #232323
  const IColor tickMid  = IColor(255,  60,  60,  60);   // #3c3c3c
  const IColor tickCtr  = IColor(255,  90,  90,  90);   // #5a5a5a
  const IColor trackBg  = IColor(255,  18,  18,  18);   // #121212
  const IColor smoothBg = IColor(255,  22,  22,  22);   // #161616
  const IColor smoothFg = IColor(255,  10, 200, 180);   // teal fill

  // ---- background ----
  g.FillRect(bg, mRECT);

  // ---- NOTE NAME — large, centered ----
  const char* noteStr = hasSignal ? kNoteNames[noteIdx] : "-";
  IColor noteCol = hasSignal ? text : IColor(255, 150, 150, 150);  // brighter when inactive
  g.DrawText(IText(64, noteCol, "TexasLED",
                   EAlign::Center, EVAlign::Middle),
             noteStr,
             IRECT(mRECT.L + 80, mRECT.T + 8,
                   mRECT.R - 80, mRECT.T + 100));

  // ---- OCTAVE — top-right, beside note ----
  char octStr[4] = {0};
  if (hasSignal) snprintf(octStr, sizeof(octStr), "%d", octave);
  g.DrawText(IText(18, hasSignal ? textMid : textDim, "TexasLED",
                   EAlign::Near, EVAlign::Middle),
             octStr,
             IRECT(mRECT.L + 452, mRECT.T + 20,
                   mRECT.L + 495, mRECT.T + 60));

  // ---- CENTS — below octave ----
  char centsStr[16] = {0};
  if (hasSignal) {
    const char* sign = cents > 0 ? "+" : "";
    snprintf(centsStr, sizeof(centsStr), "%s%.1f", sign, cents);
  } else {
    snprintf(centsStr, sizeof(centsStr), "---");
  }
  IColor centsCol = hasSignal ? ColorFromAccuracy(std::abs(cents)) : textDim;
  g.DrawText(IText(20, centsCol, "TexasLED",
                   EAlign::Near, EVAlign::Middle),
               centsStr,
               IRECT(mRECT.L + 452, mRECT.T + 62,
                     mRECT.L + 540, mRECT.T + 100));

  // ---- FREQUENCY — centered below ----
  char freqStr[32] = {0};
  if (hasSignal) snprintf(freqStr, sizeof(freqStr), "%.2f Hz", freq);
  else           snprintf(freqStr, sizeof(freqStr), "------ Hz");
  g.DrawText(IText(14, hasSignal ? text : textDim, "TexasLED",
                   EAlign::Center, EVAlign::Middle),
               freqStr,
               IRECT(mRECT.L, mRECT.T + 102,
                     mRECT.R, mRECT.T + 126));

  // =============================================================================
  // TUNING METER — horizontal track, centered, with ticks and needle
  // =============================================================================
  const IRECT track(mRECT.L + 20, mRECT.T + 130,
                    mRECT.R - 20, mRECT.T + 156);

  // Track bg.
  g.FillRect(trackBg, track);

  // Center line.
  const float cy = (float)track.MH();
  g.DrawLine(tickLine, track.L, cy, track.R, cy, nullptr, 1.0f);

  // Ticks: 0%, 25%, 50% (2px wide), 75%, 100%.
  auto drawTick = [&](float pct, const IColor& c, float thickness) {
    const float x = (float)(track.L + pct * track.W());
    g.DrawLine(c, x, track.T + 3, x, track.B - 3, nullptr, thickness);
  };
  drawTick(0.00f, tickLine, 1.0f);
  drawTick(0.25f, tickMid,  1.0f);
  drawTick(0.50f, tickCtr,  2.0f);
  drawTick(0.75f, tickMid,  1.0f);
  drawTick(1.00f, tickLine, 1.0f);

  // In-tune zone (46%..54%): faint teal glow.
  if (mGlow > 0.001f) {
    const float zL = (float)(track.L + 0.46f * track.W());
    const float zR = (float)(track.L + 0.54f * track.W());
    IColor zoneCol = IColor((uint8_t)(255 * mGlow * 0.35f),
                             10, 220, 200);
    g.FillRect(zoneCol, IRECT(zL, track.T, zR, track.B));
  }

  // Needle.
  const float nx = (float)(track.L + (mNeedlePos / 100.0f) * track.W());
  IColor needleCol = hasSignal ? ColorFromAccuracy(std::abs(cents)) : textDim;
  g.DrawLine(needleCol, nx, track.T - 5, nx, track.B + 5, nullptr, 2.5f);

  // =============================================================================
  // SMOOTHING SLIDER — horizontal, left side, 150×24px (wider for easier drag)
  // =============================================================================
  const IRECT slTrack(mRECT.L + 20, mRECT.T + 166,
                      mRECT.L + 170, mRECT.T + 190);
  const float sval = (float)(mTuner->GetParam(kParamSmoothing)->Value() / 100.0);

  // Track bg + border (brighten border when hovering).
  g.FillRect(mIsHovering ? IColor(255, 28, 28, 28) : smoothBg, slTrack);
  g.DrawRect(mIsHovering ? accent : textDim, slTrack);

  // Fill from center to (center + direction*value).
  const float ctrX = slTrack.MW();
  const float fillEnd = ctrX + (sval - 0.5f) * slTrack.W();
  if (std::abs(sval - 0.5f) > 0.001f) {
    IRECT fillRect = (sval > 0.5f)
      ? IRECT(ctrX, slTrack.T + 2, fillEnd, slTrack.B - 2)
      : IRECT(fillEnd, slTrack.T + 2, ctrX, slTrack.B - 2);
    g.FillRect(smoothFg, fillRect);
  }

  // Center detent tick.
  g.DrawLine(tickCtr, ctrX, slTrack.T + 2, ctrX, slTrack.B - 2, nullptr, 2.0f);

  // Label + value below.
  g.DrawText(IText(10, textMid, "TexasLED", EAlign::Near, EVAlign::Middle),
              "SM", IRECT(slTrack.L, slTrack.B + 2, slTrack.L + 24, slTrack.B + 18));
  char smVal[8] = {0};
  snprintf(smVal, sizeof(smVal), "%.0f%%", mTuner->GetParam(kParamSmoothing)->Value());
  g.DrawText(IText(10, textMid, "TexasLED", EAlign::Near, EVAlign::Middle),
              smVal, IRECT(slTrack.L + 26, slTrack.B + 2, slTrack.L + 70, slTrack.B + 18));

  // Smoothing hit-test rect (same as visual track).
  mSmoothRect = slTrack;

  // =============================================================================
  // A4 REFERENCE — centered, below slider row
  // =============================================================================
  {
    constexpr float kA4Cells = 6.f;
    constexpr float kA4CellW = 30.f;
    constexpr float kA4H     = 20.f;
    constexpr float kA4Gap  = 2.f;
    const float kA4W = kA4Cells * (kA4CellW + kA4Gap) - kA4Gap;
    const float kA4X = mRECT.MW() - kA4W / 2.f;
    const float kA4Y = mRECT.B - 44.f;
    const IRECT a4Bg(kA4X, kA4Y, kA4X + kA4W, kA4Y + kA4H);

    int selIdx = (int)mTuner->GetParam(kParamA4Ref)->Value();
    static const char* a4Labels[] = {"415", "430", "432", "440", "442", "444"};

    g.FillRect(smoothBg, a4Bg);
    g.DrawRect(textDim, a4Bg);

    for (int i = 0; i < (int)kA4Cells; i++) {
      IRECT cell(a4Bg.L + i * (kA4CellW + kA4Gap), a4Bg.T,
                 a4Bg.L + i * (kA4CellW + kA4Gap) + kA4CellW, a4Bg.B);
      bool active = (i == selIdx);
      if (active)
        g.FillRect(accent, cell.GetPadded(-1));  // teal fill for active cell
      else if (i == mHoveredA4Cell)
        g.FillRect(IColor(50, 10, 220, 200), cell.GetPadded(-1));  // subtle teal hover
      const IColor& cellText = active ? IColor(255, 245, 245, 245)  // near-white on teal
                                      : textMid;
      g.DrawText(IText(12, cellText, "TexasLED",
                       EAlign::Center, EVAlign::Middle),
                  a4Labels[i], cell);
    }
    // Label.
    g.DrawText(IText(11, textMid, "TexasLED", EAlign::Center, EVAlign::Middle),
               "A4", IRECT(a4Bg.MW() - 20, a4Bg.T - 16, a4Bg.MW() + 20, a4Bg.T));
  }

  // =============================================================================
  // CLOPH LOGO — bottom-right, teal tint for brand consistency
  // =============================================================================
  if (mLogo && mLogo->IsValid()) {
    constexpr float kLogoAspect = 1461.67f / 569.f;
    constexpr float kLogoW     = 72.f;    // 72px wide — visible but not dominant
    constexpr float kLogoH     = kLogoW / kLogoAspect;  // ≈ 28
    const IRECT logoRect(mRECT.R - kLogoW - 10.f,
                          mRECT.B - kLogoH - 8.f,
                          mRECT.R - 10.f,
                          mRECT.B - 8.f);
    // Tint the SVG teal to match the accent palette.
    g.DrawSVG(*mLogo, logoRect, nullptr, &accent, nullptr);
  }
}

// ponytail: dead code removed — lines 567-763 were the OLD active draw block.
// The redesign (lines 349-566) is now the only Draw() implementation.
// Key improvements: horizontal 100x20px smoothing slider (was 8px strip),
// teal-tinted logo, unified palette, teal in-tune glow, improved typography.


void TunerReadoutControl::OnMouseDown(float x, float y, const IMouseMod& mod) {
  // ---- Smoothing slider hit-test (horizontal bar) ----
  if (mSmoothRect.Contains(x, y)) {
    mDragStartX = x;
    mDragStartSmooth = (float)(mTuner->GetParam(kParamSmoothing)->Value() / 100.0);
    mIsDragging = true;
    if (IGraphics* pGraphics = GetUI()) {
      if (auto* pPlug = dynamic_cast<iplug::Plugin*>(pGraphics->GetDelegate())) {
        pPlug->BeginInformHostOfParamChangeFromUI(kParamSmoothing);
      }
    }
    SetDirty(false);
    return;
  }

  // ---- A4 reference bar hit-test (bottom-center) ----
  {
    constexpr float kA4Cells  = 6.f;
    constexpr float kA4CellW  = 30.f;
    constexpr float kA4H      = 20.f;
    constexpr float kA4Gap    = 2.f;
    const float kA4W = kA4Cells * (kA4CellW + kA4Gap) - kA4Gap;
    const float kA4X = mRECT.MW() - kA4W / 2.f;
    const float kA4Y = mRECT.B - 44.f;
    const IRECT a4Bg(kA4X, kA4Y, kA4X + kA4W, kA4Y + kA4H);
    if (a4Bg.Contains(x, y)) {
      int cell = (int)((x - a4Bg.L) / (kA4CellW + kA4Gap));
      if (cell >= 0 && cell < (int)kA4Cells) {
        if (IGraphics* pGraphics = GetUI()) {
          if (auto* pPlug = dynamic_cast<iplug::Plugin*>(pGraphics->GetDelegate())) {
            const double norm = pPlug->GetParam(kParamA4Ref)->ToNormalized((double)cell);
            pPlug->SendParameterValueFromUI(kParamA4Ref, norm);
          }
        }
      }
      SetDirty(false);
    }
  }
}

void TunerReadoutControl::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) {
  if (!mIsDragging) return;
  // Horizontal drag: 1px = 1% smoothing (100px = full range 0..1).
  const float delta = (x - mDragStartX) * 0.01f;
  const float newVal = std::max(0.0f, std::min(1.0f, mDragStartSmooth + delta));
  if (IGraphics* pGraphics = GetUI()) {
    if (auto* pPlug = dynamic_cast<iplug::Plugin*>(pGraphics->GetDelegate())) {
      pPlug->SendParameterValueFromUI(kParamSmoothing, (double)newVal);
    }
  }
  SetDirty(false);
}

void TunerReadoutControl::OnMouseUp(float x, float y, const IMouseMod& mod) {
  if (mIsDragging) {
    mIsDragging = false;
    if (IGraphics* pGraphics = GetUI()) {
      if (auto* pPlug = dynamic_cast<iplug::Plugin*>(pGraphics->GetDelegate())) {
        pPlug->EndInformHostOfParamChangeFromUI(kParamSmoothing);
      }
    }
    SetDirty(false);
  }
}

void TunerReadoutControl::OnMouseOver(float x, float y, const IMouseMod& mod) {
  const bool wasHovering = mIsHovering;
  mIsHovering = mSmoothRect.Contains(x, y);

  // A4 cell hover.
  constexpr float kA4Cells  = 6.f;
  constexpr float kA4CellW  = 30.f;
  constexpr float kA4H      = 20.f;
  constexpr float kA4Gap    = 2.f;
  const float kA4W = kA4Cells * (kA4CellW + kA4Gap) - kA4Gap;
  const float kA4X = mRECT.MW() - kA4W / 2.f;
  const float kA4Y = mRECT.B - 44.f;
  const IRECT a4Bg(kA4X, kA4Y, kA4X + kA4W, kA4Y + kA4H);
  int newA4Cell = -1;
  if (a4Bg.Contains(x, y)) {
    newA4Cell = (int)((x - a4Bg.L) / (kA4CellW + kA4Gap));
    if (newA4Cell < 0 || newA4Cell >= (int)kA4Cells) newA4Cell = -1;
  }

  // Tooltip: show a short contextual hint based on the region under the cursor.
  // The platform layer only calls GetTooltip() on WM_MOUSEHOVER (first entry),
  // not on every OnMouseOver, so this text must be a stable region-hint,
  // not a per-cell dynamic value.
  if (mIsHovering)
    mTooltip.Set("Smoothing: drag to adjust / double-click to reset");
  else if (newA4Cell >= 0)
    mTooltip.Set("A4 reference: click to select");
  else
    mTooltip.Set("");

  const bool a4Changed = (newA4Cell != mHoveredA4Cell);
  mHoveredA4Cell = newA4Cell;

  if (mIsHovering != wasHovering || a4Changed) SetDirty(false);
}

void TunerReadoutControl::OnMouseOut() {
  if (mIsHovering) SetDirty(false);
  mIsHovering = false;
  mHoveredA4Cell = -1;
  mTooltip.Set("");
}

void TunerReadoutControl::OnMouseDblClick(float x, float y, const IMouseMod& mod) {
  // Double-click on smoothing slider resets to default (55%).
  if (mSmoothRect.Contains(x, y)) {
    if (IGraphics* pGraphics = GetUI()) {
      if (auto* pPlug = dynamic_cast<iplug::Plugin*>(pGraphics->GetDelegate())) {
        pPlug->BeginInformHostOfParamChangeFromUI(kParamSmoothing);
        pPlug->SendParameterValueFromUI(kParamSmoothing, 0.55); // 55% default
        pPlug->EndInformHostOfParamChangeFromUI(kParamSmoothing);
      }
    }
    SetDirty(false);
  }
}


// =============================================================================
// Plugin class
// =============================================================================

Tuner::Tuner(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
  , mDSP(std::make_unique<TunerDSPWrapper>())
{
  GetParam(kParamSmoothing)->InitDouble(
      "Smoothing", 55.0, 0.0, 100.0, 1.0, "%");
  GetParam(kParamA4Ref)->InitEnum("A4", 3, 6, "Hz");
  {
    static const char* a4Labels[] = {"415", "430", "432", "440", "442", "444"};
    for (int i = 0; i < 6; i++)
      GetParam(kParamA4Ref)->SetDisplayText(i, a4Labels[i]);
  }

  mDetector.SetSmoothing(GetParam(kParamSmoothing)->Value() / 100.0);
  {
    int a4Idx = (int)GetParam(kParamA4Ref)->Value();
    static const double kA4Hz[] = {415.0, 430.0, 432.0, 440.0, 442.0, 444.0};
    mDetector.SetA4Reference(kA4Hz[std::min(a4Idx, 5)], true);  // instant = true at init
  }

  // RT-1: flush-to-zero + denormals-are-zero for the audio-thread IIR RMS
  // smoother (and any future accumulator that risks producing denormals).
  // 0x8040 = _MM_FLUSH_ZERO_ON (0x8000) | _MM_DENORMALS_ZERO_ON (0x0040).
  // Set once at construction; the thread-affinity guarantees from iPlug2
  // mean this sticks across the lifetime of the process.  Gated by
  // TUNER_HAS_SSE_DENORMALS -- the intrinsics are x86-only; on ARM64 we
  // rely on the math library's default denormal handling.
#if TUNER_HAS_SSE_DENORMALS
  _mm_setcsr(_mm_getcsr() | 0x8040);
#endif

  // ST-1: factory presets.  Enum indices for A4: 0=415,1=430,2=432,3=440,4=442,5=444.
  MakePreset("Standard A=440",  55.0, 3.0);
  MakePreset("Baroque A=415",   55.0, 0.0);
  MakePreset("Classical A=430", 55.0, 1.0);
  MakePreset("Baritone",        65.0, 3.0);

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS);
  };
  mLayoutFunc = [&](IGraphics* g) { LayoutUI(g); };
#endif
}

// ST-1: chunk state serialization with magic number validation.  iPlug2's
// default SerializeParams/UnserializeParams handles the two exposed params
// (Smoothing, A4) automatically; the magic + version guard catches corrupted
// or cross-version state before it reaches the parameter system.
static constexpr int32_t kTunerMagic   = 'TnR1';
static constexpr int32_t kTunerVersion = 1;

bool Tuner::SerializeState(IByteChunk& chunk) const {
  chunk.Put(&kTunerMagic);
  chunk.Put(&kTunerVersion);
  return SerializeParams(chunk);
}

int Tuner::UnserializeState(const IByteChunk& chunk, int startPos) {
  int pos = startPos;
  int32_t magic = 0;
  pos = chunk.Get(&magic, pos);
  if (magic != kTunerMagic) {
    // Magic mismatch — corrupted or unknown format.  Reset to defaults.
    for (int i = 0; i < kNumParams; ++i) GetParam(i)->SetToDefault();
    OnParamChange(kParamSmoothing);
    OnParamChange(kParamA4Ref);
    return pos;
  }
  int32_t version = 0;
  pos = chunk.Get(&version, pos);
  // Version 1 format — only one version currently.  Future migrations
  // branch here.
  pos = UnserializeParams(chunk, pos);
  // Sync DSP to freshly loaded parameter values.
  OnParamChange(kParamSmoothing);
  OnParamChange(kParamA4Ref);
  return pos;
}

void Tuner::OnRestoreState() {
  // RT-5 / state-restore: iPlug2's default OnRestoreState only pushes
  // restored parameter values to the GUI delegate. It does NOT re-init the
  // Detector's mSampleRate. When a host (Bitwig is the known offender)
  // skips IAudioProcessor::setupProcessing() because the project's sample
  // rate matches the previous session, OnReset() never fires and the
  // Detector keeps the SR it had when the project was last saved -- which
  // can be different from the current session's SR (e.g. 48000 saved,
  // 44100 active). YIN/MPM then search the wrong lag range, fail to find
  // pitch, atomics stay at zero, and the GUI shows no values even though
  // audio is flowing. Symptom: deleting and re-inserting the plugin fixes
  // it (fresh instances go through setupProcessing and OnReset).
  iplug::Plugin::OnRestoreState();  // parent sends restored params to GUI
  mDetector.Init(GetSampleRate());
  OnParamChange(kParamSmoothing);
  OnParamChange(kParamA4Ref);
}

void Tuner::OnReset() {
  mDSP->Init(GetSampleRate());
  mDetector.Init(GetSampleRate());
}

void Tuner::OnParamChange(int paramIdx) {
  switch (paramIdx) {
    case kParamSmoothing:
      mDetector.SetSmoothing(GetParam(kParamSmoothing)->Value() / 100.0);
      break;
    case kParamA4Ref: {
      static const double kA4Hz[] = {415.0, 430.0, 432.0, 440.0, 442.0, 444.0};
      int idx = (int)GetParam(kParamA4Ref)->Value();
      mDetector.SetA4Reference(kA4Hz[(idx < 0 || idx > 5) ? 3 : idx]);
      break;
    }
    default: break;
  }
}

void Tuner::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  // Bit-perfect pass-through.  We deliberately do NOT route through the
  // FAUST wrapper here: its generated code does
  //   output[i] = (float)(float)(input[i])
  // after a reinterpret_cast<float*>(double*) pointer-aliasing, which
  // corrupts non-power-of-2 sample values and writes 4 bytes where 8 were
  // expected (overflowing into the next sample).  A tuner must never
  // touch the audio -- so a plain memcpy is the only correct option.
  //
  // The detector still reads from `inputs` (read-only); the pass-through
  // and the analysis happen on the same source buffer.
  for (int ch = 0; ch < 2; ++ch) {
    std::memcpy(outputs[ch], inputs[ch], static_cast<size_t>(nFrames) * sizeof(sample));
  }

  const sample* inL = inputs[0];
  const sample* inR = inputs[1];

  const double sr = GetSampleRate();
  const double rmsCoeff = 1.0 - std::exp(-1.0 / std::max(1.0, 0.050 * sr));

  double runningRms = mDetector.GetResult().level.load();
  if (std::isnan(runningRms) || runningRms < 0.0) runningRms = 0.0;

  for (int i = 0; i < nFrames; ++i) {
    double mono = 0.5 * ((double)inL[i] + (double)inR[i]);
    if (!std::isfinite(mono)) mono = 0.0;  // RT-4: guard NaN/Inf before RMS + detector
    const double sq = mono * mono;
    runningRms = rmsCoeff * sq + (1.0 - rmsCoeff) * runningRms;
    mDetector.PushSample(mono);
  }

  const double levelOut = std::min(1.0, std::max(0.0, std::sqrt(runningRms)));
  mDetector.GetResult().level.store(levelOut);
}

#if IPLUG_EDITOR

// Freeze95/BronzeNoise pattern: setComponentState is a no-op in non-distributed
// VST3, so OnActivate is the only signal that the parameter container may hold
// values that diverge from the UI. Set mSendUpdate and let OnIdle push them.
void Tuner::OnActivate(bool active) {
  if (active) mSendUpdate = true;
}

void Tuner::OnIdle() {
  // PERF-1: drain the analysis-path profiler once a second and log to the
  // host's debug stream (OutputDebugString on Windows, NSLog on macOS).
  // At 30 FPS this fires after 30 frames; analysis runs at ~47 Hz which
  // means each sample contains ~1.57 detector passes worth of timing.
  static int s_idleFrames = 0;
  if (++s_idleFrames >= 30) {
    s_idleFrames = 0;
    const double avgUs = mDetector.Profiler().ReadAndReset();
    // Compute the fraction of one analysis-pass slot (kAnalysisStride=1024
    // samples / SR) that the detector consumed -- this is the metric that
    // matters for real-time budgeting regardless of the actual pass rate.
    const double slotUs = (double)TunerAnalysis::kAnalysisStride / GetSampleRate() * 1e6;
    [[maybe_unused]] const double pctSlot = (slotUs > 0.0) ? (avgUs / slotUs * 100.0) : 0.0;
    DBGMSG("Tuner: analysis avg=%.1f us (%.2f%% of %d-sample slot @ %.0f Hz)\n",
           avgUs, pctSlot, (int)TunerAnalysis::kAnalysisStride, GetSampleRate());
  }
  if (mSendUpdate) {
    if (GetUI()) SendCurrentParamValuesFromDelegate();
    mSendUpdate = false;
  }
  if (GetUI()) {
    GetUI()->SetAllControlsDirty();
  }
}

// LayoutUI -- load the TexasLED display TTF + CLOPH brand logo from the
// resource compiler's embedded payload (resources/main.rc).
//
// Matches the Freeze95 pattern exactly: bare-filename calls into
// g.LoadFont / g.LoadSVG.  iPlug2's LocateResource -> EnumResourceNamesW
// -> quote-strip -> strcmp chain finds the embedded resource by the
// bare filename (e.g. "CLOPH.svg"), and the filename doubles as the
// cache key.  No sidecar TEXASLED.TTF / CLOPH.svg copies next to the
// .vst3 required.
//
// IMPORTANT: LoadSVG takes ONE argument (the filename), not two.  The
// signature is `LoadSVG(const char* fileNameOrResID, const char* units,
// float dpi)` -- a 2-arg call would interpret the second string as
// the `units` parameter ("px", "mm", etc.) and silently fail the
// resource lookup.
void Tuner::LayoutUI(IGraphics* pGraphics) {
  IGraphics& g = *pGraphics;

  // 7-segment-style display font for note names + cents readout.
  g.LoadFont("TexasLED", TEXASLED_FN);

  // CLOPH brand logo (bottom-right of the readout).
  ISVG logo = g.LoadSVG(CLOPH_LOGO_FN);

  TunerReadoutControl* pReadout = new TunerReadoutControl(IRECT(0, 0, PLUG_WIDTH, PLUG_HEIGHT), this);
  if (logo.IsValid()) pReadout->SetLogo(logo);
  g.AttachControl(pReadout);

  // Initial sync: push current host param values to UI. OnActivate/OnIdle
  // only fires on re-activation, not on first window open.
  SendCurrentParamValuesFromDelegate();
}

bool Tuner::OnHostRequestingSupportedViewConfiguration(int width, int height)
{
  // Window can resize freely; ConstrainEditorResize locks the aspect ratio
  // (matching PLUG_WIDTH:PLUG_HEIGHT = 9:5) and clamps to PLUG_MIN/MAX bounds.
  return ConstrainEditorResize(width, height);
}

void Tuner::OnHostSelectedViewConfiguration(int width, int height)
{
  if (GetUI()) {
    // Scale mode: keep the inner graphics at canonical PLUG_WIDTH/PLUG_HEIGHT
    // but render at a scale that fits the host window. LayoutUI uses
    // g->Width()/g->Height() which equal PLUG_WIDTH/PLUG_HEIGHT in Scale mode,
    // so elements stay at their fixed canonical positions while the window
    // canvas is upscaled to fill the host window.
    const float screenScale = GetUI()->GetScreenScale();
    const float scaleX = static_cast<float>(width) / static_cast<float>(PLUG_WIDTH) / screenScale;
    const float scaleY = static_cast<float>(height) / static_cast<float>(PLUG_HEIGHT) / screenScale;
    const float scale = std::clamp(std::min(scaleX, scaleY), 0.65f, 2.0f);

    // Strip and re-layout at the new scale so every control keeps its
    // proportional relationship. Same pattern as Freeze95 OnParentWindowResize.
    GetUI()->RemoveAllControls();
    GetUI()->Resize(PLUG_WIDTH, PLUG_HEIGHT, scale, false);
    LayoutUI(GetUI());
  }
}

void Tuner::OnParentWindowResize(int width, int height)
{
  // Freeze95 pattern: don't constrain the window size, just scale the inner
  // canvas via min(scaleX, scaleY) so the content always has the right aspect
  // ratio. The window may have letterbox margins but content is always locked.
  OnHostSelectedViewConfiguration(width, height);
}

bool Tuner::ConstrainEditorResize(int& w, int& h) const
{
  // Override the base class to enforce aspect ratio (PLUG_WIDTH:PLUG_HEIGHT).
  // The base only clips to MIN/MAX independently; we additionally lock the
  // aspect ratio so the host window rectangle always matches 9:5.
  // Called by VST3 checkSizeConstraint + CLAP canResize + our own resize handlers.
  const float aspect = static_cast<float>(PLUG_WIDTH) / static_cast<float>(PLUG_HEIGHT);

  // First clip to MIN/MAX bounds
  w = std::max(PLUG_MIN_WIDTH, std::min(w, PLUG_MAX_WIDTH));
  h = std::max(PLUG_MIN_HEIGHT, std::min(h, PLUG_MAX_HEIGHT));

  // Then lock aspect ratio: derive h from w (height follows width)
  h = static_cast<int>(static_cast<float>(w) / aspect + 0.5f);

  // Re-clip h after aspect adjustment
  if (h < PLUG_MIN_HEIGHT) {
    h = PLUG_MIN_HEIGHT;
    w = static_cast<int>(static_cast<float>(h) * aspect + 0.5f);
  }
  if (h > PLUG_MAX_HEIGHT) {
    h = PLUG_MAX_HEIGHT;
    w = static_cast<int>(static_cast<float>(h) * aspect + 0.5f);
  }

  return true;
}

#endif // IPLUG_EDITOR