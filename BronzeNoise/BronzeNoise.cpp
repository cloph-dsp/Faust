#include "BronzeNoise.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

// SSE intrinsics are x86/x64-only. On Apple Silicon (arm64) Clang defines
// __SSE__=0 and rejects these headers with "This header is only meant to be
// used on x86 and x64 architecture" errors. macOS math library handles
// denormal flushing at the OS level on arm64 (no SSE FTZ needed), so
// gating is safe — the runtime _mm_setcsr calls become no-ops there.
#ifdef __SSE__
  #include <xmmintrin.h>
  #include <pmmintrin.h>
#endif

// ponytail: Two-channel peak meter with peak-hold indicator. 0 dBFS at top, 60 dB at bottom.
// Linear peak tracks the audio thread atomics (via SetLevels from OnIdle); peak hold sticks for ~1.5s then releases.
// Must live at file scope (not in anonymous namespace) because BronzeNoise.h forward-declares it as a file-scope type.
class IOMeterControl final : public IControl
{
public:
  IOMeterControl(const IRECT& bounds, const char* label, const IText& labelText, const char* fontID = nullptr)
    : IControl(bounds, -1), mLabel(label), mLabelText(labelText), mFontID(fontID) {}

  void SetLevels(float peakL, float peakR) {
    // Smoothed level: instant attack on rising, slow release on falling (classic peak-meter ballistics).
    mCurrentL = std::max(peakL, mCurrentL * 0.985f);
    mCurrentR = std::max(peakR, mCurrentR * 0.985f);
    // Peak hold: stick at highest value, decay only after hold frames elapse.
    if (peakL > mPeakL) { mPeakL = peakL; mHoldL = mHoldFrames; }
    else if (mHoldL > 0) --mHoldL;
    else mPeakL = std::max(0.f, mPeakL * 0.996f);  // very slow release after hold
    if (peakR > mPeakR) { mPeakR = peakR; mHoldR = mHoldFrames; }
    else if (mHoldR > 0) --mHoldR;
    else mPeakR = std::max(0.f, mPeakR * 0.996f);
    SetDirty(false);
  }

  void Draw(IGraphics& g) override
  {
    const IColor background(255, 18, 22, 32);
    const IColor border(255, 70, 74, 82);
    const IColor channelColor(255, 205, 127, 50);
    const IColor channelDim(255, 95, 100, 110);
    g.FillRoundRect(background, mRECT, 4.f, &mBlend);
    g.DrawRoundRect(border, mRECT, 4.f, &mBlend, 1.f);

    // Two vertical bars, narrow gap between them.
    const IRECT inner = mRECT.GetPadded(6.f, 18.f, 6.f, 18.f);
    const float barW = (inner.W() - 2.f) * 0.5f;
    const IRECT barL(inner.L, inner.T, inner.L + barW, inner.B);
    const IRECT barR(inner.R - barW, inner.T, inner.R, inner.B);

    auto drawBar = [&](const IRECT& r, float peak) {
      g.FillRoundRect(background.WithOpacity(0.6f), r, 1.f, &mBlend);
      // 0 dBFS = top, -60 dBFS = bottom. Linear amplitude: linear == 1.0 → 0 dB → bar full height.
      const float db = (peak > 0.f) ? 20.f * std::log10(peak) : -60.f;
      const float norm = Clip((db + 60.f) / 60.f, 0.f, 1.f);
      const float fillH = norm * r.H();
      const IRECT fill(r.L, r.B - fillH, r.R, r.B);
      // Muted palette: bronze main, dim amber for warning, soft rose only for genuine clip.
      IColor col = channelColor;
      if (norm > 0.98f) col = IColor(255, 180, 90, 90);       // soft rose — only at/near 0 dBFS
      else if (norm > 0.78f) col = IColor(255, 200, 150, 85); // dim amber — caution zone
      g.FillRoundRect(col, fill, 1.f, &mBlend);
    };
    drawBar(barL, mCurrentL);
    drawBar(barR, mCurrentR);

    // Peak hold indicators (muted horizontal tick at peak position).
    auto drawPeakHold = [&](const IRECT& r, float peak) {
      if (peak <= 0.001f) return;
      const float db = 20.f * std::log10(peak);
      const float norm = Clip((db + 60.f) / 60.f, 0.f, 1.f);
      const float y = r.B - norm * r.H();
      g.DrawLine(IColor(180, 200, 195, 180), r.L, y, r.R, y, &mBlend, 1.2f);
    };
    drawPeakHold(barL, mPeakL);
    drawPeakHold(barR, mPeakR);

    // Label at top
    g.DrawText(mLabelText, mLabel, IRECT(mRECT.L, mRECT.T + 2.f, mRECT.R, mRECT.T + 16.f), &mBlend);
  }

private:
  static constexpr int mHoldFrames = 240;  // ~4s at 60fps — chill, not twitchy
  const char* mLabel;
  IText mLabelText;
  const char* mFontID;
  float mCurrentL = 0.f;
  float mCurrentR = 0.f;
  float mPeakL = 0.f;
  float mPeakR = 0.f;
  int mHoldL = 0;
  int mHoldR = 0;
};

namespace
{
constexpr const char* kUIFontID = "BronzeNoiseUIFont";
constexpr double kTwoPi = 6.28318530717958647692;
constexpr float kMinTargetHz = 20.f;
constexpr float kSilenceGateDb = -72.f;
constexpr float kMaxCorrectionBoostDb = 18.f;
constexpr float kMaxCorrectionCutDb = 21.f;
constexpr float kLowBandProtectionDb = 6.f;

// Fast denormal protection - flushes subnormal floats to zero (prevents DSP stalls)
inline float FlushDenorm(float x) { return (std::abs(x) < 1.0e-15f) ? 0.f : x; }

// Simple deterministic LCG dither for audio - 16-bit equivalent dither
inline float Dither16()
{
  static uint32_t state = 0x7FED0456;
  state = state * 1103515245 + 12345;
  return (static_cast<int>(state) & 0xFFFF) / 65536.f - 0.5f;
}

// Noise-shaped dither: triangular PDF + simple 1st-order noise shaping
inline float DitherAndNoiseShape(float& lastShapedError, float lastSample)
{
  const float dither = Dither16() + Dither16(); // TPDF
  const float sum = lastSample + dither * 0.5f;
  const float error = sum - std::round(sum); // quantization error
  const float shaped = sum + (error - lastShapedError) * 0.5f; // simple noise shaping
  lastShapedError = error;
  return shaped;
}

float Lerp(float start, float end, float blend)
{
  return start + ((end - start) * Clip(blend, 0.f, 1.f));
}

// NaN/Inf safety guard - returns safe value if input is invalid
inline float SafeValue(float x) { return (std::isfinite(x) ? x : 0.f); }

// Bound check helper - ensures value stays within AudioManifesto "sterile" range
inline float Sanitize(float x) { return Clip(x, -10.f, 10.f); }

// Maps Amount (0-1) to drive factor for spectral processing intensity
// 0 -> 0.0 (no processing), 1.0 -> 1.0 (full effect)
float MapAmountDrive(float amount)
{
  return std::pow(Clip(amount, 0.f, 1.f), 0.82f);
}

float LinearToDb(float linearValue) 
{ 
  return 20.f * std::log10(std::max(1.0e-6f, linearValue)); 
} 

float DbToLinearFunc(float dbValue) 
{ 
  return std::pow(10.f, dbValue / 20.f); 
} 

float SmoothStepFunc(float edge0, float edge1, float x) 
{ 
  float t = Clip((x - edge0) / (edge1 - edge0), 0.f, 1.f); 
  return t * t * (3.f - 2.f * t); 
} 

float MapAggression(float amount) 
{ 
  float base = std::pow(Clip(amount, 0.f, 1.f), 1.55f); 
  float extraBoost = SmoothStepFunc(0.85f, 1.0f, amount) * 0.35f; 
  return base + extraBoost; 
} 

float ComputeMakeupGain(float amount) 
{ 
  float linearAmount = Clip(amount, 0.f, 1.f); 
  // Increased makeup gain from 6dB to 12dB for more power at high amounts
  float makeupDb = std::pow(linearAmount, 1.2f) * 12.0f; 
  return DbToLinearFunc(makeupDb); 
} 

float SoftClip(float x) 
{ 
  return 1.5f * std::tanh(x / 1.5f); 
}

float ComputeDifferenceAccent(float differenceDb, float aggression)
{
  const float magnitudeDb = std::abs(differenceDb);
  return 1.f + (0.85f * aggression * (magnitudeDb / (6.f + magnitudeDb)));
}

float ComputeBandFocus(float frequencyHz, float aggression)
{
  const float octaveDistance = std::abs(std::log2(std::max(25.f, frequencyHz) / 1000.f));
  return 1.f + (0.30f * aggression * Clip(octaveDistance / 3.5f, 0.f, 1.f));
}

// Presence shelf: Gaussian-shaped boost/cut around 10kHz
float ComputePresenceShelf(float frequencyHz, float character)
{
  if (std::abs(character) < 0.01f)
    return 0.f;

  constexpr float kPresenceCenterHz = 10000.f;
  constexpr float kPresenceWidthOctaves = 1.5f;
  
  const float centerLog = std::log(kPresenceCenterHz);
  const float widthLog = kPresenceWidthOctaves * 0.693147f; // ln(2)
  const float freqLog = std::log(std::max(frequencyHz, 20.f));

  const float dist = (freqLog - centerLog) / widthLog;
  const float envelope = std::exp(-dist * dist * 2.f);

  constexpr float maxBoostDb = 6.f;
  return character * maxBoostDb * envelope;
}

constexpr std::array<float, 31> kEqualLoudnessFrequenciesHz = {
  20.f, 25.f, 31.5f, 40.f, 50.f, 63.f, 80.f, 100.f, 125.f, 160.f,
  200.f, 250.f, 315.f, 400.f, 500.f, 630.f, 800.f, 1000.f, 1250.f, 1600.f,
  2000.f, 2500.f, 3150.f, 4000.f, 5000.f, 6300.f, 8000.f, 10000.f, 12500.f,
  16000.f, 20000.f
};

constexpr std::array<float, 31> kEqualLoudnessInverseDb = {
  18.0f, 14.2f, 11.0f, 8.4f, 6.4f, 5.0f, 3.8f, 2.8f, 2.0f, 1.3f,
  0.8f, 0.4f, 0.1f, -0.2f, -0.4f, -0.6f, -0.4f, 0.0f, 0.3f, 0.3f,
  0.0f, -0.8f, -1.4f, -1.0f, -0.3f, 0.7f, 1.8f, 3.2f, 5.0f,
  7.4f, 10.0f
};

// ponytail: ISVGKnobControl doesn't inherit IVectorBase, so IVStyle-driven label/value never render.
// Subclass adds label above + value below the SVG cap. Lets parent do the rotation (replicates mStartAngle/mEndAngle math internally).
class KnobWithTextControl final : public ISVGKnobControl
{
public:
  KnobWithTextControl(const IRECT& bounds, const ISVG& svg, int paramIdx,
                      const char* label, const IText& labelText, const IText& valueText)
    : ISVGKnobControl(bounds, svg, paramIdx)
    , mKnobSVG(svg), mLabel(label), mLabelText(labelText), mValueText(valueText)
  {
    // No AttachIControl — ISVGKnobControl doesn't inherit IVectorBase.
  }

  void Draw(IGraphics& g) override
  {
    // Static body — render SVG without rotation. ISVGKnobControl::Draw would rotate it; we want body fixed.
    g.DrawSVG(mKnobSVG, mRECT, &mBlend);

    // Rotating pointer: triangle from center pointing outward, angle = -135° (min) to +135° (max).
    if (const IParam* p = GetParam()) {
      const float normalized = static_cast<float>(p->ToNormalized(p->Value()));
      const float angleDeg = -135.f + normalized * 270.f;
      const float angleRad = angleDeg * 3.14159265f / 180.f;
      const float cx = mRECT.MW();
      const float cy = mRECT.MH();
      const float radius = mRECT.W() * 0.42f;
      const float tipX = cx + radius * std::sin(angleRad);
      const float tipY = cy - radius * std::cos(angleRad);
      const float baseHalfW = mRECT.W() * 0.05f;
      const float baseX1 = cx + baseHalfW * std::sin(angleRad + 1.5708f);
      const float baseY1 = cy - baseHalfW * std::cos(angleRad + 1.5708f);
      const float baseX2 = cx - baseHalfW * std::sin(angleRad + 1.5708f);
      const float baseY2 = cy + baseHalfW * std::cos(angleRad + 1.5708f);
      const IColor pointerColor(mIsActive ? 255 : (mMouseIsOver ? 240 : 220), 255, 240, 200);
      g.FillTriangle(pointerColor, tipX, tipY, baseX1, baseY1, baseX2, baseY2, &mBlend);
      // Hub dot at center
      g.FillCircle(IColor(255, 28, 30, 38), cx, cy, mRECT.W() * 0.05f, &mBlend);
    }

    // Detect value change -> kick pulse (decays via Animate below).
    if (const IParam* p = GetParam()) {
      const double v = p->Value();
      if (mLastValueSet && std::abs(v - mLastValue) > 0.001) {
        mPulse = 1.f;
        SetAnimation([this](IControl* c) { mPulse = std::max(0.f, mPulse - 0.06f); }, 16);
      }
      mLastValue = v;
      mLastValueSet = true;
    }

    const float labelH = mLabelText.mSize + 4.f;
    const float valueH = mValueText.mSize + 4.f + mPulse * 3.f;
    // Pill alpha lifts on hover + on press + on pulse so the knob reacts.
    const int pillAlpha = static_cast<int>(Clip(180.f + mPulse * 50.f + (mMouseIsOver ? 35.f : 0.f) + (mIsActive ? 45.f : 0.f), 0.f, 255.f));
    const IColor pillBg(pillAlpha, 14, 16, 20);
    g.FillRect(pillBg, IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + labelH));
    g.FillRect(pillBg, IRECT(mRECT.L, mRECT.B - valueH, mRECT.R, mRECT.B));
    g.DrawText(mLabelText, mLabel, IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + labelH), &mBlend);
    if (const IParam* p = GetParam()) {
      WDL_String valStr;
      p->GetDisplay(valStr);
      g.DrawText(mValueText, valStr.Get(), IRECT(mRECT.L, mRECT.B - valueH, mRECT.R, mRECT.B), &mBlend);
    }
    // Active state ring: bronze stroke around the SVG cap when dragging.
    if (mIsActive) {
      g.DrawRoundRect(IColor(255, 205, 127, 50), mRECT.GetPadded(-2.f), 6.f, &mBlend, 2.f);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    if (mod.R) { PromptUserInput(mRECT); return; }
    mIsActive = true;
    ISVGKnobControl::OnMouseDown(x, y, mod);
  }
  void OnMouseUp(float x, float y, const IMouseMod& mod) override { mIsActive = false; ISVGKnobControl::OnMouseUp(x, y, mod); SetDirty(false); }
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
    if (const IParam* p = GetParam()) {
      const_cast<IParam*>(p)->SetToDefault();
      // Match single-click value-change behaviour: redraw + notify host via parent.
      SetValueFromDelegate(p->ToNormalized(p->GetDefault()));
      const_cast<KnobWithTextControl*>(this)->SetDirty();
    }
  }

  // F-04 (P3): circular hit area so corners aren't clickable deadzone.
  bool IsHit(float x, float y) const override {
    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    const float dx = x - cx;
    const float dy = y - cy;
    return (dx * dx + dy * dy) <= (mRECT.W() * 0.5f) * (mRECT.W() * 0.5f);
  }

  void OnEndAnimation() override { if (mPulse <= 0.01f) { mPulse = 0.f; SetAnimation(nullptr); } }

private:
  ISVG mKnobSVG;
  const char* mLabel;
  IText mLabelText;
  IText mValueText;
  double mLastValue = 0.0;
  bool mLastValueSet = false;
  float mPulse = 0.f;
  bool mIsActive = false;
};

class TargetSelectorControl final : public IControl, public IVectorBase {
public:
  TargetSelectorControl(const IRECT& bounds, int parameterIndex, const char* label, const IVStyle& style)
    : IControl(bounds, parameterIndex), IVectorBase(style, false, true) {
    AttachIControl(this, label);
  }
  // ponytail: SegmentedSelector — draw N click-target cells, each one an enum choice. Click a cell = set that value. Visible list always, no popup.
  void Draw(IGraphics& graphics) override {
    IParam* parameter = const_cast<IParam*>(GetParam());
    if (!parameter) return;
    // ponytail: last known local mouse coords captured by OnMouseOver — used to highlight the cell under the cursor.
    const int n = static_cast<int>(parameter->GetRange()) + 1;
    const int current = static_cast<int>(parameter->Value());
    const IColor panel(255, 24, 27, 34);
    const IColor accent(255, 205, 127, 50);
    const IColor accentDim(255, 170, 100, 35);
    const IColor text(255, 239, 234, 226);
    const IColor textDim(255, 130, 135, 142);
    // Reserve a small label strip at the top
    const float labelH = 14.f;
    const IRECT labelRect = mRECT.GetFromTop(labelH);
    DrawBackground(graphics, mRECT);
    graphics.DrawText(IText(13.f, textDim, mStyle.valueText.mFont, EAlign::Center, EVAlign::Middle),
                      mLabelStr.Get(), labelRect, &mBlend);
    const IRECT cellsRect = mRECT.GetReducedFromTop(labelH).GetPadded(-2.f);
    const float rows = 1.f;
    const float cols = static_cast<float>(n);
    const float cellW = cellsRect.W() / cols;
    const float cellH = cellsRect.H() / rows;
    for (int i = 0; i < n; ++i) {
      const int col = i % static_cast<int>(cols);
      const int row = i / static_cast<int>(cols);
      const IRECT cell(cellsRect.L + col * cellW, cellsRect.T + row * cellH,
                       cellsRect.L + (col + 1) * cellW, cellsRect.T + (row + 1) * cellH);
      const bool isCurrent = (i == current);
      const bool isHot = cell.Contains(mLastMouseX, mLastMouseY) && !IsDisabled();
      // Cell background
      graphics.FillRoundRect(panel.WithOpacity(isCurrent ? 1.f : 0.6f), cell.GetPadded(-1.f), 2.f, &mBlend);
      // Cell border (bronze if current, dim if hover, none otherwise)
      IColor border;
      if (isCurrent) border = accent;
      else if (isHot) border = accentDim;
      else            border = IColor(255, 70, 74, 82);
      graphics.DrawRoundRect(border.WithOpacity(isCurrent ? 1.f : (isHot ? 0.95f : 0.55f)),
                             cell.GetPadded(-1.f), 2.f, &mBlend, isCurrent ? 1.4f : 1.0f);
      // Cell text — fetch the enum's display string for this index. withDisplayText=true returns the named label ("White", "4096", etc).
      WDL_String cellText;
      parameter->GetDisplay(static_cast<double>(i), false, cellText, true);
      // F-05 (P3): shrink text to fit narrow cells (especially at minimum window width).
      IText cellIText(mStyle.valueText.mSize * 0.85f, isCurrent ? text : textDim, mStyle.valueText.mFont,
                      EAlign::Center, EVAlign::Middle);
      IRECT textBounds = cell.GetPadded(-2.f);
      const float maxW = textBounds.W();
      IRECT measureBounds = textBounds;
      graphics.MeasureText(cellIText, cellText.Get(), measureBounds);
      while (cellIText.mSize > 7.f && measureBounds.W() > maxW) {
        cellIText.mSize -= 0.5f;
        measureBounds = textBounds;
        graphics.MeasureText(cellIText, cellText.Get(), measureBounds);
      }
      graphics.DrawText(cellIText, cellText.Get(), textBounds, &mBlend);
    }
  }
  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    IParam* parameter = const_cast<IParam*>(GetParam());
    if (!parameter) return;
    const int n = static_cast<int>(parameter->GetRange()) + 1;
    const float labelH = 14.f;
    const IRECT cellsRect = mRECT.GetReducedFromTop(labelH).GetPadded(-2.f);
    const float cellW = cellsRect.W() / static_cast<float>(n);
    if (cellW <= 0.f) return;
    const int col = Clip(static_cast<int>((x - cellsRect.L) / cellW), 0, n - 1);
    parameter->Set(static_cast<double>(col));
  }
  void OnMouseOver(float x, float y, const IMouseMod& mod) override {
    mLastMouseX = x; mLastMouseY = y;
    IParam* parameter = const_cast<IParam*>(GetParam());
    if (parameter) {
      const int n = static_cast<int>(parameter->GetRange()) + 1;
      const float labelH = 14.f;
      const IRECT cellsRect = mRECT.GetReducedFromTop(labelH).GetPadded(-2.f);
      const float cellW = cellsRect.W() / static_cast<float>(n > 0 ? n : 1);
      if (cellW > 0.f) {
        const int col = Clip(static_cast<int>((x - cellsRect.L) / cellW), 0, n - 1);
        WDL_String cellText;
        parameter->GetDisplay(static_cast<double>(col), false, cellText, true);
        WDL_String tip;
        tip.Set(mLabelStr.Get());
        tip.Append(" — ");
        tip.Append(cellText.Get());
        SetTooltip(tip.Get());
      }
    }
    SetDirty(false);
  }
  void OnMouseOut() override { SetDirty(false); }
  void OnResize() override { SetTargetRECT(MakeRects(mRECT)); }
  void OnInit() override {
    if (const IParam* parameter = GetParam()) parameter->GetDisplay(mValueStr);
  }
  void SetDirty(bool triggerAction = true, int valueIndex = kNoValIdx) override {
    IControl::SetDirty(triggerAction, valueIndex);
    if (const IParam* parameter = GetParam()) parameter->GetDisplay(mValueStr);
  }
private:
  float mLastMouseX = -1.f;
  float mLastMouseY = -1.f;
};
// Spectrum visualizer control for before/after/reference
class SpectrumVisualizerControl : public IControl {
public:
  SpectrumVisualizerControl(const IRECT& bounds, const char* fontID = nullptr)
    : IControl(bounds, -1), mFontID(fontID) {  // -1 means not linked to parameter, but still receives mouse events
    mBefore.fill(0.f);
    mAfter.fill(0.f);
    mReference.fill(0.f);
    mFreqs.fill(0.f);
  }
  void OnInit() override {
    // Enable mouse interaction for this control
  }
  void SetData(const float* before, const float* after, const float* reference, const float* freqs, int bands) {
    for (int i = 0; i < bands && i < 64; ++i) {
      mBefore[i] = before[i];
      mAfter[i] = after[i];
      mReference[i] = reference[i];
      mFreqs[i] = freqs[i];
    }
    mBands = bands;
    SetDirty();
  }
  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override {
    float zoom = d * 3.f;
    mMinDb -= zoom;
    mMaxDb += zoom;
    mMinDb = std::max(mMinDb, -100.f);
    mMaxDb = std::min(mMaxDb, 48.f);
    SetDirty();
  }
  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    mDragging = true;
    mDragStartY = y;
    mDragStartMinDb = mMinDb;
    mDragStartMaxDb = mMaxDb;
    SetDirty();
  }
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override {
    if (mDragging) {
      float range = mDragStartMaxDb - mDragStartMinDb;
      float deltaDb = (dY / mRECT.H()) * range;
      mMinDb = mDragStartMinDb + deltaDb;
      mMaxDb = mDragStartMaxDb + deltaDb;
      mMinDb = std::max(mMinDb, -100.f);
      mMaxDb = std::min(mMaxDb, 48.f);
      SetDirty();
    }
  }
  void OnMouseUp(float x, float y, const IMouseMod& mod) override {
    mDragging = false;
  }
  void Draw(IGraphics& g) override {
    const IRECT r = mRECT.GetPadded(-6.f);
    const float w = r.W();
    const float h = r.H();
    const float minDb = mMinDb;
    const float maxDb = mMaxDb;

    // Dark background with bronze-tinted border
    g.FillRoundRect(IColor(255, 18, 22, 32), r, 6.f, &mBlend);
    g.DrawRoundRect(IColor(180, 205, 127, 50), r, 6.f, &mBlend, 1.0f);

    if (mBands < 2)
    {
      g.DrawText(IText(16.f, IColor(255, 140, 145, 150), mFontID, EAlign::Center, EVAlign::Middle),
                 "No signal",
                 r,
                 &mBlend);
      return;
    }

    // Draw axes
    g.DrawLine(IColor(255, 120, 120, 120), r.L, r.T, r.L, r.B, &mBlend, 1.5f);
    g.DrawLine(IColor(255, 120, 120, 120), r.L, r.B, r.R, r.B, &mBlend, 1.5f);

    // Dynamic dB scale based on current view
    float dbStep = 12.f;
    if (maxDb - minDb > 60.f) dbStep = 24.f;
    if (maxDb - minDb < 30.f) dbStep = 6.f;
    char dbStr[16];
    for (float db = std::floor(minDb / dbStep) * dbStep; db <= maxDb; db += dbStep) {
      if (db < minDb || db > maxDb) continue;
      const float y = r.B - ((db - minDb) / (maxDb - minDb)) * h;
      if (y < r.T + 10.f || y > r.B - 10.f) continue;
      g.DrawLine(IColor(255, 60, 65, 70), r.L, y, r.R, y, &mBlend, 1.0f);
      g.DrawLine(IColor(255, 200, 200, 200), r.L, y, r.L + 6.f, y, &mBlend, 2.0f);
      IRECT labelRect(r.L + 8.f, y - 10.f, r.L + 50.f, y + 10.f);
      std::snprintf(dbStr, sizeof(dbStr), "%.0f dB", db);
      g.DrawText(IText(13.f, IColor(255, 220, 220, 220), mFontID, EAlign::Near, EVAlign::Middle),
                  dbStr, labelRect, &mBlend);
    }

    // Frequency scale on X axis
    constexpr std::array<float, 6> kLabelFreqs = {20.f, 50.f, 100.f, 500.f, 1000.f, 10000.f};
    constexpr std::array<const char*, 6> kLabelTexts = {"20 Hz", "50 Hz", "100 Hz", "500 Hz", "1 kHz", "10 kHz"};
    for (size_t i = 0; i < kLabelFreqs.size(); ++i) {
      const float x = r.L + (w * std::log10(kLabelFreqs[i] / 20.f) / std::log10(20000.f / 20.f));
      if (x < r.L + 20.f || x > r.R - 20.f) continue;
      g.DrawLine(IColor(255, 60, 65, 70), x, r.T, x, r.B, &mBlend, 1.0f);
      g.DrawLine(IColor(255, 200, 200, 200), x, r.B - 6.f, x, r.B, &mBlend, 2.0f);
      IRECT labelRect(x - 30.f, r.B - 28.f, x + 30.f, r.B - 4.f);
      g.DrawText(IText(13.f, IColor(255, 220, 220, 220), mFontID, EAlign::Center, EVAlign::Middle),
                  kLabelTexts[i], labelRect, &mBlend);
    }

    // Target curve (bronze - dashed)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = Clip(r.B - ((mReference[i - 1] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      const float y1 = Clip(r.B - ((mReference[i] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      g.DrawLine(IColor(255, 205, 127, 50), x0, y0, x1, y1, &mBlend, 2.0f);
    }

    // Input curve (cool gray - subtle, what we're matching FROM)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = Clip(r.B - ((mBefore[i - 1] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      const float y1 = Clip(r.B - ((mBefore[i] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      g.DrawLine(IColor(255, 160, 165, 175), x0, y0, x1, y1, &mBlend, 1.5f);
    }

    // Output curve (bronze-orange - bold, the result)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = Clip(r.B - ((mAfter[i - 1] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      const float y1 = Clip(r.B - ((mAfter[i] - minDb) / (maxDb - minDb)) * h, r.T, r.B);
      g.DrawLine(IColor(255, 255, 170, 60), x0, y0, x1, y1, &mBlend, 2.5f);
    }

    // Legend
    const float legendY = r.T + 4.f;
    g.DrawText(IText(10.f, IColor(255, 160, 165, 175), mFontID, EAlign::Near, EVAlign::Middle),
               "In", IRECT(r.L + 4.f, legendY, r.L + 24.f, legendY + 12.f), &mBlend);
    g.DrawText(IText(10.f, IColor(255, 205, 127, 50), mFontID, EAlign::Near, EVAlign::Middle),
               "Tgt", IRECT(r.L + 28.f, legendY, r.L + 56.f, legendY + 12.f), &mBlend);
    g.DrawText(IText(10.f, IColor(255, 255, 170, 60), mFontID, EAlign::Near, EVAlign::Middle),
               "Out", IRECT(r.L + 62.f, legendY, r.L + 90.f, legendY + 12.f), &mBlend);
  }
private:
  std::array<float, 64> mBefore;
  std::array<float, 64> mAfter;
  std::array<float, 64> mReference;
  std::array<float, 64> mFreqs;
  int mBands = 0;
  float mMinDb = -100.f;
  float mMaxDb = 48.f;
  bool mDragging = false;
  float mDragStartY = 0.f;
  float mDragStartMinDb = 0.f;
  float mDragStartMaxDb = 0.f;
  const char* mFontID = nullptr;
};

// Simple bypass overlay - draws a semi-transparent full-ui overlay with "BYPASSED" text
class BypassOverlayControl : public IControl
{
public:
  BypassOverlayControl(const IRECT& bounds) : IControl(bounds, -1) { SetIgnoreMouse(true); }
  
  void Draw(IGraphics& g) override
  {
    // Check bypass param via plugin delegate
    IParam* bypassParam = GetUI()->GetDelegate()->GetParam(kBypass);
    if (!bypassParam || bypassParam->Int() == 0)
      return;
    
    // Semi-transparent dark overlay
    g.FillRect(IColor(160, 0, 0, 0), mRECT, &mBlend);
    
    // "BYPASSED" badge
    const IRECT badge = mRECT.GetCentredInside(200.f, 40.f);
    g.FillRoundRect(IColor(200, 205, 127, 50), badge, 6.f, &mBlend);
    g.DrawText(IText(18.f, IColor(255, 255, 255, 255), nullptr, EAlign::Center, EVAlign::Middle),
               "BYPASSED", badge, &mBlend);
  }
};

// ponytail: Tiny accent dot in the title bar that breathes via SetAnimation lambda.
// Subtle idle motion = "this plugin is alive" without being distracting.
class TitleBarLEDPulseControl final : public IControl
{
public:
  TitleBarLEDPulseControl(const IRECT& bounds) : IControl(bounds, -1) {
    SetIgnoreMouse(true);
    SetAnimation([this](IControl* c) {
      mBreath += 0.04f;
      if (mBreath > 1.f) mBreath -= 1.f;
      SetDirty(false);
    }, 30);
  }
  void Draw(IGraphics& g) override {
    const float t = 0.5f - 0.5f * std::cos(mBreath * 6.2831853f);  // 0..1 sinusoidal
    const int alpha = static_cast<int>(140 + t * 100);
    const IColor col(alpha, 205, 127, 50);
    g.FillCircle(col, mRECT.MW(), mRECT.MH(), mRECT.W() * 0.5f, &mBlend);
    // Inner hot spot
    g.FillCircle(IColor(static_cast<int>(t * 200), 255, 220, 180), mRECT.MW(), mRECT.MH(), mRECT.W() * 0.18f, &mBlend);
  }
private:
  float mBreath = 0.f;
};

float InterpolateLogTable(float frequencyHz,
                          const std::array<float, kEqualLoudnessFrequenciesHz.size()>& frequencies,
                          const std::array<float, kEqualLoudnessInverseDb.size()>& values)
{
  if (frequencyHz <= frequencies.front())
    return values.front();

  if (frequencyHz >= frequencies.back())
    return values.back();

  const float logFrequency = std::log(frequencyHz);

  for (size_t index = 1; index < frequencies.size(); ++index)
  {
    if (frequencyHz <= frequencies[index])
    {
      const float lowLog = std::log(frequencies[index - 1]);
      const float highLog = std::log(frequencies[index]);
      const float blend = (logFrequency - lowLog) / std::max(1.0e-6f, highLog - lowLog);
      return values[index - 1] + ((values[index] - values[index - 1]) * blend);
    }
  }

  return values.back();
}
}

 BronzeNoise::BronzeNoise(const InstanceInfo& info)
 : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kAmount)->InitPercentage("Amount", 100.0);
  GetParam(kTarget)->InitEnum("Target", kTargetBronze, {"Loud", "Violet", "White", "Red", "Orange", "Pink", "Bronze", "Brown", "Olive", "Blue"});
  GetParam(kSmoothing)->InitDouble("Smoothing", 0.55, 0.0, 1.0, 0.001);
  GetParam(kQ)->InitDouble("Bandwidth", 0.5, 0.01, 4.0, 0.01);
  GetParam(kFFTSize)->InitEnum("FFT Size", kFFTSize4096, {"256", "512", "1024", "2048", "4096", "8192", "16384"});
  GetParam(kCharacter)->InitDouble("Character", 0.0, -1.0, 1.0, 0.01);
  GetParam(kTransient)->InitDouble("Transient", 0.5, 0.0, 1.0, 0.01);
  GetParam(kBypass)->InitInt("Bypass", 0, 0, 1, "Bypass");

  // Factory presets — arg order matches EParams: Amount, Target, Smoothing, Q, FFTSize, Character, Transient, Bypass.
  // Target / FFTSize are enum indices, not display strings.
  MakePreset("Bronze (Default)", 100.0, kTargetBronze,  0.55, 0.50, kFFTSize4096, 0.0,  0.50, 0);
  MakePreset("Vocal Warmth",     70.0,  kTargetRed,     0.40, 0.70, kFFTSize2048, 0.2,  0.30, 0);
  MakePreset("Podcast Clarity",  80.0,  kTargetLoud,    0.60, 0.50, kFFTSize1024, 0.1,  0.20, 0);
  MakePreset("Master Glue",      35.0,  kTargetWhite,   0.75, 0.80, kFFTSize8192, 0.0,  0.60, 0);
  MakePreset("Tame Harsh",       75.0,  kTargetOlive,   0.50, 1.50, kFFTSize4096, -0.3, 0.20, 0);
  MakePreset("Lo-Fi Texture",    50.0,  kTargetPink,    0.30, 0.60, kFFTSize512,  -0.5, 0.10, 0);
  MakePreset("Bright Air",       50.0,  kTargetViolet,  0.40, 0.70, kFFTSize2048, 0.4,  0.40, 0);
  MakePreset("Subtle Match",     25.0,  kTargetBronze,  0.80, 1.00, kFFTSize4096, 0.0,  0.50, 0);

#if IPLUG_DSP
  // Initialize vector buffers with maximum size
  mWindow.resize(kMaxFFTSize);
  mPermutation.resize(kMaxFFTSize);
  for (int ch = 0; ch < kMaxChannels; ++ch) {
    mAnalysisBuffer[ch].resize(kMaxFFTSize);
    mOverlapAddBuffer[ch].resize(kMaxFFTSize);
    mHopBuffer[ch].resize(kMaxHopSize);
    mOutputFifo[ch].resize(kOutputFifoSize);
    mFftBuffer[ch].resize(kMaxFFTSize);
    // Dry delay buffer for phase-coherent wet/dry mixing (size = max latency)
    mDryDelayBuffer[ch].resize(kMaxFFTSize);
    mDryDelayIndex[ch] = 0;
  }
  
  WDL_fft_init();
  ResetDspState();
#endif

  // UI setup with visualizer
#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS);
  };
  mLayoutFunc = [&](IGraphics* pGraphics) { LayoutUI(pGraphics); };
#endif
}

#if IPLUG_EDITOR
void BronzeNoise::LayoutUI(IGraphics* pGraphics)
{
  // Bronze-themed palette
  const IColor backgroundColor(255, 14, 16, 20);
  const IColor panelColor(255, 24, 27, 34);
  const IColor frameColor(255, 70, 74, 82);
  const IColor accentColor(255, 205, 127, 50);       // #CD7F32 bronze
  const IColor accentDim(255, 170, 100, 35);          // darker bronze for inactive
  const IColor primaryTextColor(255, 239, 234, 226);
  const IColor secondaryTextColor(255, 156, 161, 169);
  const IColor visBackground(255, 18, 22, 32);
  const IColor visBorder(255, 70, 74, 82);

  // Two fonts: titleFont (IronSans) for plugin title, uiFont (Kenyan Coffee) for everything else.
  // 2-arg LoadFont looks up the TTF as a Windows resource by name (matches main.rc macro).
  // 3-arg overload takes a system font FACE name, not a resource id — that's why system fallback chain keeps its 3-arg form.
  const char* kTitleFontID = "BronzeNoiseTitleFont";
  const char* titleFont = nullptr;
  if (pGraphics->LoadFont(kTitleFontID, IRON_SANS_FN)
      || pGraphics->LoadFont(kTitleFontID, "Segoe UI", ETextStyle::Normal))
  {
    titleFont = kTitleFontID;
  }
  const char* uiFont = nullptr;
  if (pGraphics->LoadFont(kUIFontID, ROBOTO_FN)
      || pGraphics->LoadFont(kUIFontID, "Segoe UI", ETextStyle::Normal)
      || pGraphics->LoadFont(kUIFontID, "Arial", ETextStyle::Normal)
      || pGraphics->LoadFont(kUIFontID, "Verdana", ETextStyle::Normal))
  {
    uiFont = kUIFontID;
  }

  // Load knob SVG once — KNOB_SVG_FN macro expands to "knob.svg" which is the resource name compiled into the DLL by main.rc.
  ISVG knobSVG = pGraphics->LoadSVG(KNOB_SVG_FN);
  const bool knobSVGOk = knobSVG.IsValid();

  // iPlug2 logo SVG — drawn opposite the title (right side of title bar).
  ISVG clophSVG = pGraphics->LoadSVG(CLOPH_LOGO_FN);
  const bool clophSVGOk = clophSVG.IsValid();

  pGraphics->AttachPanelBackground(backgroundColor);
  pGraphics->AttachPopupMenuControl(DEFAULT_TEXT.WithFont(uiFont).WithFGColor(primaryTextColor));
  pGraphics->EnableMouseOver(true);
  pGraphics->EnableTooltips(true);
  pGraphics->AttachBubbleControl();
  pGraphics->AssignParamNameToolTips();
  const IVStyle controlStyle = DEFAULT_STYLE
    .WithColor(kBG, panelColor)
    .WithColor(kFG, IColor(255, 88, 94, 104))
    .WithColor(kPR, accentColor)
    .WithColor(kFR, frameColor)
    .WithColor(kHL, IColor(255, 255, 240, 224))
    .WithColor(kX1, primaryTextColor)
    .WithLabelText(IText(16.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Top))
    .WithValueText(IText(18.f, primaryTextColor, uiFont, EAlign::Center, EVAlign::Bottom))
    .WithRoundness(0.22f)
    .WithDrawShadows(false);
  const IVStyle selectorStyle = controlStyle.WithShowValue(false);
  const IRECT bounds = pGraphics->GetBounds().GetPadded(-16.f);

  // ponytail: title-only strip (no subtitle). Bumped height for larger text. IronSans for the title.
  const float titleH = 72.f;
  const IRECT titleRect(bounds.L, bounds.T, bounds.L + 320.f, bounds.T + titleH);
  pGraphics->AttachControl(new ITextControl(titleRect,
    "BRONZE NOISE",
    IText(36.f, accentColor, titleFont ? titleFont : uiFont, EAlign::Near, EVAlign::Middle)));

  // ponytail: CLOPH logo, right side of title bar, vertically centered. ViewBox 1462×569 → aspect 2.57:1.
  if (clophSVGOk) {
    const float logoH = 54.f;
    const float logoW = logoH * (1462.f / 569.f); // ~154px wide
    const float logoRightPad = 20.f;
    const float logoY = bounds.T + (titleH - logoH) * 0.5f;
    const IRECT logoRect(bounds.R - logoW - logoRightPad, logoY, bounds.R - logoRightPad, logoY + logoH);
    pGraphics->AttachControl(new ISVGControl(logoRect, clophSVG));
  }

  // ponytail: brand mark + version in title bar (small text under or beside the title, and a breathing accent LED that signals "alive").
  const IRECT brandRect(bounds.L + 200.f, bounds.T + 4.f, bounds.L + 380.f, bounds.T + 18.f);
  pGraphics->AttachControl(new ITextControl(brandRect,
    "CLOPH \xC2\xB7 v1.0.0",  // UTF-8 "·"
    IText(12.f, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle)));
  // Latency readout — initially empty; updated in OnReset when block size / sample rate change.
  const IRECT latencyRect(bounds.L + 385.f, bounds.T + 4.f, bounds.L + 540.f, bounds.T + 18.f);
  mLatencyLabel = new ITextControl(latencyRect, "",
    IText(12.f, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle));
  mLatencyLabel->SetTooltip("Current processing latency (varies with FFT size)");
  pGraphics->AttachControl(mLatencyLabel);
  // Breathing LED, sits just left of the segmented bars at the bottom-left of the title strip.
  const IRECT ledRect(bounds.L + 4.f, bounds.T + titleH - 16.f, bounds.L + 14.f, bounds.T + titleH - 6.f);
  pGraphics->AttachControl(new TitleBarLEDPulseControl(ledRect));

  // ponytail: knobs ride at top of spectrum area (below title), bars sit at bottom — visualizer fills the band between them.
  const float barsH = 44.f;
  const float barsTop = bounds.B - barsH - 8.f;
  const float knobsTop = bounds.T + titleH + 8.f;

  // ponytail: knobs + bars reflow with window width. Default 1000 → 72px / 320px FFT; min 600 → 60px / 240px; max 1600 → 92px / 380px.
  const float knobSize = Clip(bounds.W() / 12.f, 60.f, 92.f);
  const float gap = Clip(knobSize * 0.18f, 10.f, 18.f);
  const float knobRowW = knobSize * 5 + gap * 4;
  const float knobStartX = bounds.L + (bounds.W() - knobRowW) * 0.5f;
  const float knobY = knobsTop;

  // ponytail: I/O meters flank the visualizer. ~36px wide each, leave a 10px gap. Visualizer shrinks horizontally to make room.
  const float meterW = 36.f;
  const float meterGap = 10.f;
  const IRECT inputMeterRect(bounds.L, knobY + knobSize + 12.f, bounds.L + meterW, barsTop - 12.f);
  const IRECT outputMeterRect(bounds.R - meterW, knobY + knobSize + 12.f, bounds.R, barsTop - 12.f);
  mInputMeter = new IOMeterControl(inputMeterRect, "IN",
    IText(11.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle), uiFont);
  mInputMeter->SetTooltip("Input level (L/R) with peak hold");
  pGraphics->AttachControl(mInputMeter);
  mOutputMeter = new IOMeterControl(outputMeterRect, "OUT",
    IText(11.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle), uiFont);
  mOutputMeter->SetTooltip("Output level (L/R) with peak hold");
  pGraphics->AttachControl(mOutputMeter);

  // Visualizer fills the band between knobs (top) and bars (bottom), shrunk to leave meter channels.
  const IRECT visRect(bounds.L + meterW + meterGap, knobY + knobSize + 12.f,
                      bounds.R - meterW - meterGap, barsTop - 12.f);
  pGraphics->AttachControl(new SpectrumVisualizerControl(visRect, uiFont));

  auto attachKnob = [&](const IRECT& r, int paramIdx, const char* label, const char* tip) {
    IControl* knob = nullptr;
    if (knobSVGOk) {
      // ponytail: KnobWithTextControl wraps ISVGKnobControl and overlays label/value text since ISVGKnobControl doesn't inherit IVectorBase.
      knob = new KnobWithTextControl(r, knobSVG, paramIdx, label,
        IText(15.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle),
        IText(17.f, accentColor, uiFont, EAlign::Center, EVAlign::Middle));
    } else {
      knob = new IVKnobControl(r, paramIdx, label, controlStyle);
    }
    knob->SetTooltip(tip);
    pGraphics->AttachControl(knob);
    return knob;
  };

  for (int i = 0; i < 5; ++i) {
    const float x = knobStartX + i * (knobSize + gap);
    const IRECT kr(x, knobY, x + knobSize, knobY + knobSize);
    switch (i) {
      case 0: attachKnob(kr, kAmount,    "Amount",
              "Mix level of target noise spectrum (0-100%)\nShift+drag = fine adjust | Double-click = reset"); break;
      case 1: attachKnob(kr, kSmoothing, "Smoothing",
              "Spectrum smoothing time constant - higher = smoother but slower response\nShift+drag = fine adjust | Double-click = reset"); break;
      case 2: attachKnob(kr, kQ,         "Bandwidth",
              "EQ band bandwidth - lower = narrower, higher = wider bands\nShift+drag = fine adjust | Double-click = reset"); break;
      case 3: attachKnob(kr, kCharacter, "Character",
              "Spectral tilt: negative = bassier, positive = brighter\nShift+drag = fine adjust | Double-click = reset"); break;
      case 4: attachKnob(kr, kTransient, "Transient",
              "Transient/sustain balance: lower preserves transients, higher boosts body\nShift+drag = fine adjust | Double-click = reset"); break;
    }
  }

  // ponytail: full-width segmented bars — Target (10 options, ~5x2 cells) on the left, FFT (7 options, single row) on the right.
  const float barGap = 16.f;
  const float fftW = Clip(bounds.W() * 0.32f, 240.f, 400.f);
  const float targetW = bounds.W() - fftW - barGap;
  const IRECT targetBar(bounds.L, barsTop, bounds.L + targetW, barsTop + barsH);
  const IRECT fftBar(targetBar.R + barGap, barsTop, targetBar.R + barGap + fftW, barsTop + barsH);

  auto* targetControl = new TargetSelectorControl(targetBar, kTarget, "Target", selectorStyle);
  targetControl->SetTooltip("Target noise spectrum curve");
  pGraphics->AttachControl(targetControl);

  auto* fftControl = new TargetSelectorControl(fftBar, kFFTSize, "FFT", selectorStyle);
  fftControl->SetTooltip("FFT analysis size - higher = more freq resolution but more latency");
  pGraphics->AttachControl(fftControl);

  // Bypass overlay (full bounds, drawn on top only when bypass is engaged)
  auto* bypassOverlay = new BypassOverlayControl(bounds);
  bypassOverlay->SetIgnoreMouse(true);
  pGraphics->AttachControl(bypassOverlay);

  // Initial sync: push current parameter values to all UI controls. OnActivate/OnIdle handles re-activation but the very first UI build needs this.
  SendCurrentParamValuesFromDelegate();
}
void BronzeNoise::OnParentWindowResize(int width, int height)
{
  if (!GetUI())
    return;

  const float screenScale = GetUI()->GetScreenScale();
  const float scaleX = static_cast<float>(width) / static_cast<float>(PLUG_WIDTH) / screenScale;
  const float scaleY = static_cast<float>(height) / static_cast<float>(PLUG_HEIGHT) / screenScale;
  const float scale = Clip(std::min(scaleX, scaleY), 0.6f, 1.6f);

  GetUI()->RemoveAllControls();
  GetUI()->Resize(PLUG_WIDTH, PLUG_HEIGHT, scale, false);
  LayoutUI(GetUI());
}

// Freeze95 pattern: setComponentState is a no-op in non-distributed VST3.
// On activate, the parameter container may hold stale values; sync + ask OnIdle to push them to the UI.
void BronzeNoise::OnActivate(bool active)
{
  if (active) {
#if defined IPLUG_VST3
    UpdateParams(this, GetBypassed() ? 1 : 0);
#endif
    mSendUpdate = true;
  }
}

void BronzeNoise::OnIdle()
{
#if IPLUG_EDITOR
  if (mSendUpdate) {
    if (GetUI())
      SendCurrentParamValuesFromDelegate();
    mSendUpdate = false;
  }
  // ponytail: poll audio-thread atomics and push to I/O meters (UI thread, ~60Hz).
  // GetUI() guard needed: OnIdle can fire after GUI destruction, before mInputMeter/mOutputMeter are nulled.
  if (GetUI() && mInputMeter && mOutputMeter) {
    mInputMeter->SetLevels(
      mInputPeakL.load(std::memory_order_relaxed),
      mInputPeakR.load(std::memory_order_relaxed));
    mOutputMeter->SetLevels(
      mOutputPeakL.load(std::memory_order_relaxed),
      mOutputPeakR.load(std::memory_order_relaxed));
  }
#endif
}
#endif // IPLUG_EDITOR (outer—LayoutUI, OnParentWindowResize, OnActivate, OnIdle)

#if IPLUG_DSP
void BronzeNoise::OnReset()
{
  mSampleRate = GetSampleRate();
  
  // Get FFT size from parameter
  const int fftSizeSelector = static_cast<int>(GetParam(kFFTSize)->Value());
  static constexpr int kFFTSizes[kNumFFTSizes] = {256, 512, 1024, 2048, 4096, 8192, 16384};
  mFFTSize = kFFTSizes[fftSizeSelector];
  mHopSize = mFFTSize / 2;

  // Report actual latency to host.
  // With 50% overlap (hop = FFTSize/2), the algorithmic latency is one hop.
  SetLatency(mHopSize);

  // Update latency readout in title bar (ITextControl not available in headers, cast from base).
  // GetUI() guard: label pointer can outlive GUI if accessed after destruction.
  if (GetUI() && mLatencyLabel) {
    const double ms = static_cast<double>(mHopSize) / mSampleRate * 1000.0;
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%d smp \xC2\xB7 %.1fms @ %.0fHz", mHopSize, ms, mSampleRate);
    static_cast<ITextControl*>(mLatencyLabel)->SetStr(buf);
    mLatencyLabel->SetDirty(false);
  }

  for (int index = 0; index < mFFTSize; ++index)
    mPermutation[index] = WDL_fft_permute(mFFTSize, index);

  UpdateBandLayout(mSampleRate);
  ResetDspState();
}

void BronzeNoise::OnParamChange(int paramIdx)
{
  if (paramIdx == kFFTSize)
  {
    OnReset();
  }
  else if (paramIdx == kBypass)
  {
    mBypassed = (GetParam(kBypass)->Int() != 0);
  }
  else
  {
    // Update smoothed targets immediately so next ProcessBlock can ramp smoothly
    switch (paramIdx)
    {
      case kAmount:   mSmoothedAmount = static_cast<float>(GetParam(kAmount)->Value()); break;
      case kSmoothing: mSmoothedSmoothing = static_cast<float>(GetParam(kSmoothing)->Value()); break;
      case kQ:        mSmoothedQ = static_cast<float>(GetParam(kQ)->Value()); break;
      case kCharacter: mSmoothedCharacter = static_cast<float>(GetParam(kCharacter)->Value()); break;
      case kTransient: mSmoothedTransient = static_cast<float>(GetParam(kTransient)->Value()); break;
    }
  }
}

bool BronzeNoise::SerializeState(IByteChunk& chunk) const
{
  return SerializeParams(chunk);
}

int BronzeNoise::UnserializeState(const IByteChunk& chunk, int startPos)
{
  const int result = UnserializeParams(chunk, startPos);
  // Trigger reset to update DSP state with any changed parameters (FFT size, etc.)
  OnReset();
  return result;
}

void BronzeNoise::ResetDspState()
{
  for (auto& channelBuffer : mAnalysisBuffer)
    std::fill(channelBuffer.begin(), channelBuffer.end(), 0.f);

  for (auto& channelBuffer : mOverlapAddBuffer)
    std::fill(channelBuffer.begin(), channelBuffer.end(), 0.f);

  for (auto& channelBuffer : mHopBuffer)
    std::fill(channelBuffer.begin(), channelBuffer.end(), 0.f);

  for (auto& channelBuffer : mOutputFifo)
    std::fill(channelBuffer.begin(), channelBuffer.end(), 0.f);

  for (auto& channelBuffer : mFftBuffer)
  {
    for (WDL_FFT_COMPLEX& fftBin : channelBuffer)
    {
      fftBin.re = 0.f;
      fftBin.im = 0.f;
    }
  }

  mAverageSpectrumDb.fill(0.f);
  mOutputReadIndex.fill(0);
  mOutputWriteIndex.fill(0);
  mOutputCount.fill(0);
  mHopFill = 0;
  mHasSpectrum = false;
  mAutoMakeupGainDb = 0.f;
  mInputRmsDb = -60.f;
  mOutputRmsDb = -60.f;
  mTruePeakLevel = 0.f;

  for (int sampleIndex = 0; sampleIndex < mFFTSize; ++sampleIndex)
  {
    const double phase = (kTwoPi * static_cast<double>(sampleIndex)) / static_cast<double>(mFFTSize);
    const double hann = 0.5 - (0.5 * std::cos(phase));
    mWindow[sampleIndex] = static_cast<float>(std::sqrt(std::max(0.0, hann)));
  }
}

void BronzeNoise::UpdateBandLayout(double sampleRate)
{
  const float nyquistHz = std::max(1000.f, static_cast<float>(sampleRate * 0.5));
  const float binWidthHz = static_cast<float>(sampleRate / static_cast<double>(mFFTSize));
  const float ratio = std::pow(nyquistHz / kMinTargetHz, 1.0f / static_cast<float>(kNumBands));

  mBandEdgesHz[0] = kMinTargetHz;

  for (int bandIndex = 1; bandIndex <= kNumBands; ++bandIndex)
    mBandEdgesHz[bandIndex] = std::min(nyquistHz, mBandEdgesHz[bandIndex - 1] * ratio);

  mBandEdgesHz[kNumBands] = nyquistHz;

  for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
  {
    const float lowHz = mBandEdgesHz[bandIndex];
    const float highHz = mBandEdgesHz[bandIndex + 1];
    const float centerHz = std::sqrt(lowHz * highHz);
    int startBin = std::max(1, static_cast<int>(std::floor(lowHz / binWidthHz)));
    int endBin = std::min((mFFTSize / 2) - 1, static_cast<int>(std::ceil(highHz / binWidthHz)));

    if (endBin < startBin)
    {
      const int centerBin = Clip(static_cast<int>(std::lround(centerHz / binWidthHz)), 1, (mFFTSize / 2) - 1);
      startBin = centerBin;
      endBin = centerBin;
    }

    mBandCentersHz[bandIndex] = centerHz;
    mBandStartBins[bandIndex] = startBin;
    mBandEndBins[bandIndex] = endBin;
  }

  int upperBand = 1;

  for (int binIndex = 0; binIndex <= mFFTSize / 2; ++binIndex)
  {
    const float frequencyHz = std::max(binWidthHz, static_cast<float>(binIndex) * binWidthHz);

    if (frequencyHz <= mBandCentersHz.front())
    {
      mBinLowerBand[binIndex] = 0;
      mBinUpperBand[binIndex] = 0;
      mBinBandBlend[binIndex] = 0.f;
      continue;
    }

    if (frequencyHz >= mBandCentersHz.back())
    {
      mBinLowerBand[binIndex] = kNumBands - 1;
      mBinUpperBand[binIndex] = kNumBands - 1;
      mBinBandBlend[binIndex] = 0.f;
      continue;
    }

    while (upperBand < kNumBands - 1 && frequencyHz > mBandCentersHz[upperBand])
      ++upperBand;

    const int lowerBand = std::max(0, upperBand - 1);
    const float lowerLog = std::log(mBandCentersHz[lowerBand]);
    const float upperLog = std::log(mBandCentersHz[upperBand]);
    const float blend = (std::log(frequencyHz) - lowerLog) / std::max(1.0e-6f, upperLog - lowerLog);

    mBinLowerBand[binIndex] = lowerBand;
    mBinUpperBand[binIndex] = upperBand;
    mBinBandBlend[binIndex] = Clip(blend, 0.f, 1.f);
  }
}

void BronzeNoise::PushOutputSample(int channelIndex, float value)
{
  float* outputBuffer = mOutputFifo[channelIndex].data();
  outputBuffer[mOutputWriteIndex[channelIndex]] = value;
  mOutputWriteIndex[channelIndex] = (mOutputWriteIndex[channelIndex] + 1) % kOutputFifoSize;

  if (mOutputCount[channelIndex] < kOutputFifoSize)
  {
    ++mOutputCount[channelIndex];
  }
  else
  {
    mOutputReadIndex[channelIndex] = (mOutputReadIndex[channelIndex] + 1) % kOutputFifoSize;
  }
}

float BronzeNoise::PopOutputSample(int channelIndex)
{
  if (mOutputCount[channelIndex] <= 0)
    return 0.f;

  float* outputBuffer = mOutputFifo[channelIndex].data();
  const float value = outputBuffer[mOutputReadIndex[channelIndex]];
  mOutputReadIndex[channelIndex] = (mOutputReadIndex[channelIndex] + 1) % kOutputFifoSize;
  --mOutputCount[channelIndex];
  return value;
}

float BronzeNoise::EvaluateTargetDb(float frequencyHz, int targetMode) const
{
  const float clippedFrequencyHz = std::max(kMinTargetHz, frequencyHz);

  if (targetMode == kTargetLoud)
    return EvaluateEqualLoudnessDb(clippedFrequencyHz);

  float slopeDbPerOctave = 0.f;

  switch (targetMode)
  {
    case kTargetWhite:   slopeDbPerOctave = 0.f;   break;
    case kTargetRed:    slopeDbPerOctave = -1.5f; break;
    case kTargetOrange: slopeDbPerOctave = -2.f;  break;
    case kTargetPink:   slopeDbPerOctave = -3.f;  break;
    case kTargetBronze: slopeDbPerOctave = -4.5f; break;
    case kTargetBrown:  slopeDbPerOctave = -6.f;  break;
    case kTargetOlive:  slopeDbPerOctave = -7.5f; break;
    case kTargetBlue:   slopeDbPerOctave = -9.f;  break;
    case kTargetViolet: slopeDbPerOctave = +3.f;  break;
    default:            slopeDbPerOctave = 0.f;   break;
  }

  return slopeDbPerOctave * std::log2(clippedFrequencyHz / 1000.f);
}

float BronzeNoise::EvaluateEqualLoudnessDb(float frequencyHz) const
{
  return InterpolateLogTable(frequencyHz, kEqualLoudnessFrequenciesHz, kEqualLoudnessInverseDb);
}

float BronzeNoise::LinearToDb(float linearValue) const
{
  return 20.f * std::log10(std::max(1.0e-12f, linearValue));
}

float BronzeNoise::DbToLinear(float dbValue) const
{
  return std::pow(10.f, dbValue / 20.f);
}

// Compute RMS of a float buffer segment in linear scale
float BronzeNoise::ComputeHopRms(int /*channelIndex*/, const float* buffer, int length) const
{
  if (length <= 0)
    return 0.f;
  double sumSq = 0.0;
  for (int i = 0; i < length; ++i)
    sumSq += static_cast<double>(buffer[i]) * buffer[i];
  return static_cast<float>(std::sqrt(sumSq / static_cast<double>(length)));
}

// Update smoothed auto makeup gain based on measured input vs output RMS.
// Target: output RMS == input RMS within kMakeupGainToleranceDb.
void BronzeNoise::UpdateMakeupGain(float inputRms, float outputRms)
{
  const float inputDb  = 20.f * std::log10(std::max(1.0e-9f, inputRms));
  const float outputDb = 20.f * std::log10(std::max(1.0e-9f, outputRms));
  // Required compensation = level removed by spectral reshaping
  const float requiredCompensationDb = inputDb - outputDb;
  // Never cut signal; cap at maximum makeup gain
  const float targetMakeupDb = Clip(requiredCompensationDb, 0.f, kMaxMakeupGainDb);
  // Exponential smoothing to prevent gain-pumping artefacts
  mAutoMakeupGainDb += (targetMakeupDb - mAutoMakeupGainDb) * kMakeupGainSmoothAlpha;
}

float BronzeNoise::ClampCorrectionDb(float frequencyHz, float correctionDb, float aggression) const
{
  const float negativeLimitDb = Lerp(8.f, kMaxCorrectionCutDb, aggression);
  float positiveLimitDb = Lerp(7.f, kMaxCorrectionBoostDb, aggression);

  if (frequencyHz < 55.f)
    positiveLimitDb = Lerp(kLowBandProtectionDb * 0.75f, kLowBandProtectionDb + 3.f, aggression);
  else if (frequencyHz < 90.f)
    positiveLimitDb = Lerp(7.f, 10.5f, aggression);
  else if (frequencyHz > 16000.f)
    positiveLimitDb = Lerp(6.f, 10.f, aggression);
  else if (frequencyHz > 12000.f)
    positiveLimitDb = Lerp(7.f, 11.f, aggression);

  return Clip(correctionDb, -negativeLimitDb, positiveLimitDb);
}

void BronzeNoise::SmoothBandCorrections(const std::array<float, kNumBands>& input,
                                        std::array<float, kNumBands>& output,
                                        int radius) const
{
  if (radius <= 0)
  {
    output = input;
    return;
  }

  for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
  {
    double weightedSum = 0.0;
    double weightTotal = 0.0;

    for (int neighbourIndex = std::max(0, bandIndex - radius);
         neighbourIndex <= std::min(kNumBands - 1, bandIndex + radius);
         ++neighbourIndex)
    {
      const double weight = static_cast<double>(radius + 1 - std::abs(neighbourIndex - bandIndex));
      weightedSum += static_cast<double>(input[neighbourIndex]) * weight;
      weightTotal += weight;
    }

    output[bandIndex] = static_cast<float>(weightedSum / std::max(1.0, weightTotal));
  }
}

void BronzeNoise::ProcessHop(int channelCount)
{
  const int activeChannels = Clip(channelCount, 1, kMaxChannels);
  const float amount = mSmoothedAmount * 0.01f;
  const float amountDrive = MapAmountDrive(amount);
  const float aggression = MapAggression(amount);
  const float smoothing = mSmoothedSmoothing;
  const float qValue = mSmoothedQ;
  const int targetMode = static_cast<int>(GetParam(kTarget)->Value());  // enum, no smoothing needed
  const float character = mSmoothedCharacter;
  const float liveSpectrumBlend = Lerp(0.10f, 0.58f, aggression);
  const float temporalAlpha = Clip((0.16f + (0.18f * amountDrive)) - (0.12f * smoothing), 0.05f, 0.34f);
  // Bandwidth affects how many neighboring bands are averaged together
  const int smoothingRadius = static_cast<int>(std::lround(std::pow(smoothing, 0.85f) * 6.f + qValue * 4.f));

  std::array<float, kNumBands> currentSpectrumDb {};
  std::array<float, kNumBands> targetSpectrumDb {};
  std::array<float, kNumBands> rawCorrectionDb {};
  std::array<float, kNumBands> smoothedCorrectionDb {};

  float beforeSpec[64] = {};
  float afterSpec[64] = {};
  float referenceSpec[64] = {};
  float freqSpec[64] = {};
  const int visBands = std::min(kNumBands, 64);

  // Measure input RMS across all channels for makeup gain calculation
  double inputPowerSum = 0.0;
  int inputSampleCount = 0;
  for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
  {
    for (int i = 0; i < mHopSize; ++i)
    {
      const double s = static_cast<double>(mHopBuffer[channelIndex][i]);
      inputPowerSum += s * s;
    }
    inputSampleCount += mHopSize;
  }
  const float hopInputRms = (inputSampleCount > 0)
    ? static_cast<float>(std::sqrt(inputPowerSum / static_cast<double>(inputSampleCount)))
    : 0.f;

  for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
  {
    std::memmove(mAnalysisBuffer[channelIndex].data(),
                 mAnalysisBuffer[channelIndex].data() + mHopSize,
                 static_cast<size_t>(mFFTSize - mHopSize) * sizeof(float));
    std::memcpy(mAnalysisBuffer[channelIndex].data() + (mFFTSize - mHopSize),
                mHopBuffer[channelIndex].data(),
                static_cast<size_t>(mHopSize) * sizeof(float));

    for (int sampleIndex = 0; sampleIndex < mFFTSize; ++sampleIndex)
    {
      mFftBuffer[channelIndex][sampleIndex].re = mAnalysisBuffer[channelIndex][sampleIndex] * mWindow[sampleIndex];
      mFftBuffer[channelIndex][sampleIndex].im = 0.f;
    }

    WDL_fft(mFftBuffer[channelIndex].data(), mFFTSize, 0);
  }

  double totalBandPower = 0.0;
  int totalBandBinCount = 0;

  for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
  {
    double bandPower = 0.0;
    int bandBinCount = 0;

    for (int binIndex = mBandStartBins[bandIndex]; binIndex <= mBandEndBins[bandIndex]; ++binIndex)
    {
      const int fftIndex = mPermutation[binIndex];
      double channelPower = 0.0;

      for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
      {
        const WDL_FFT_COMPLEX& fftBin = mFftBuffer[channelIndex][fftIndex];
        channelPower += (static_cast<double>(fftBin.re) * fftBin.re) + (static_cast<double>(fftBin.im) * fftBin.im);
      }

      channelPower /= static_cast<double>(activeChannels);
      bandPower += channelPower;
      totalBandPower += channelPower;
      ++bandBinCount;
      ++totalBandBinCount;
    }

    currentSpectrumDb[bandIndex] = LinearToDb(static_cast<float>(std::sqrt(bandPower / std::max(1, bandBinCount))));
  }

  float currentMeanDb = 0.f;

  for (float bandDb : currentSpectrumDb)
    currentMeanDb += bandDb;

  currentMeanDb /= static_cast<float>(kNumBands);

  const float frameRmsDb = LinearToDb(static_cast<float>(std::sqrt(totalBandPower / std::max(1, totalBandBinCount))));
  const bool signalIsUsable = frameRmsDb > kSilenceGateDb;

  if (signalIsUsable)
  {
    if (!mHasSpectrum)
    {
      mAverageSpectrumDb = currentSpectrumDb;
      mHasSpectrum = true;
    }
    else
    {
      for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
      {
        mAverageSpectrumDb[bandIndex] += (currentSpectrumDb[bandIndex] - mAverageSpectrumDb[bandIndex]) * temporalAlpha;
      }
    }
  }

  smoothedCorrectionDb.fill(0.f);
  targetSpectrumDb.fill(0.f);

  // Always compute target spectrum for visualization and processing
  float targetMeanDb = 0.f;
  for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
  {
    targetSpectrumDb[bandIndex] = EvaluateTargetDb(mBandCentersHz[bandIndex], targetMode);
    targetMeanDb += targetSpectrumDb[bandIndex];
  }
  targetMeanDb /= static_cast<float>(kNumBands);

  const auto& displaySpectrumDb = signalIsUsable ? currentSpectrumDb : mAverageSpectrumDb;

  if (amountDrive > 1.0e-4f && signalIsUsable && mHasSpectrum)
  {
    float analysisMeanDb = 0.f;

    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      analysisMeanDb += mAverageSpectrumDb[bandIndex];
    }

    analysisMeanDb /= static_cast<float>(kNumBands);

    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      const float stableShapeDb = mAverageSpectrumDb[bandIndex] - analysisMeanDb;
      const float liveShapeDb = currentSpectrumDb[bandIndex] - currentMeanDb;
      const float analysisShapeDb = stableShapeDb + ((liveShapeDb - stableShapeDb) * liveSpectrumBlend);
      const float targetShapeDb = targetSpectrumDb[bandIndex] - targetMeanDb;
      const float differenceDb = targetShapeDb - analysisShapeDb;
      const float accent = ComputeDifferenceAccent(differenceDb, aggression);
      const float bandFocus = ComputeBandFocus(mBandCentersHz[bandIndex], aggression);
      rawCorrectionDb[bandIndex] = amountDrive * differenceDb * accent * bandFocus;
    }

    SmoothBandCorrections(rawCorrectionDb, smoothedCorrectionDb, smoothingRadius);

    float correctionMeanDb = 0.f;

    for (float correctionDb : smoothedCorrectionDb)
      correctionMeanDb += correctionDb;

    correctionMeanDb /= static_cast<float>(kNumBands);

    const float meanTrim = Lerp(1.f, 0.45f, aggression);

    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      smoothedCorrectionDb[bandIndex] = ClampCorrectionDb(mBandCentersHz[bandIndex],
                                                          smoothedCorrectionDb[bandIndex] - (correctionMeanDb * meanTrim),
                                                          aggression);
    }

    // Enhanced spectral transformation at high amounts
    // At 100%, replace input spectrum with target spectrum (noise generation mode)
    const float noiseModeBlend = SmoothStepFunc(0.85f, 1.0f, amountDrive); // Pure noise mode above 85%
    
    if (noiseModeBlend > 0.05f)
    {
      // In noise mode: discard input characteristics and synthesize target spectrum
      for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
      {
        // Get current input level for this band
        const float inputDb = currentSpectrumDb[bandIndex];
        // Get target level (absolute, not centered)
        const float targetDb = targetSpectrumDb[bandIndex];
        
        // Calculate gain needed to transform input to target
        // Limit extreme gains to prevent overflow (especially with brown noise lows)
        float gainDb = targetDb - inputDb;
        gainDb = Clip(gainDb, -30.f, 24.f); // Hard limits: -30dB cut to +24dB boost
        
        // Blend between correction and pure noise synthesis
        const float noiseGain = gainDb * noiseModeBlend;
        const float correctionGain = smoothedCorrectionDb[bandIndex] * (1.0f - noiseModeBlend);
        
        smoothedCorrectionDb[bandIndex] = correctionGain + noiseGain;
      }
    }
  }

  // Apply presence shelf from Character parameter
  if (std::abs(character) > 0.01f)
  {
    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      const float presence = ComputePresenceShelf(mBandCentersHz[bandIndex], character);
      smoothedCorrectionDb[bandIndex] += presence;
    }
  }

  // Include makeup gain in visualizer output (global gain applied to all bands)
  const float makeupGainDb = mAutoMakeupGainDb;

  for (int bandIndex = 0; bandIndex < visBands; ++bandIndex)
  {
    beforeSpec[bandIndex] = displaySpectrumDb[bandIndex];
    // Output = input + correction + makeup gain (makeup is applied in ProcessBlock)
    afterSpec[bandIndex] = displaySpectrumDb[bandIndex] + smoothedCorrectionDb[bandIndex] + makeupGainDb;
    // Reference is the target spectral shape centered around mean
    referenceSpec[bandIndex] = targetSpectrumDb[bandIndex] - targetMeanDb;
    freqSpec[bandIndex] = mBandCentersHz[bandIndex];
  }

  double gainPowerSum = 0.0;

  for (float correctionDb : smoothedCorrectionDb)
  {
    const double linearGain = DbToLinear(correctionDb);
    gainPowerSum += linearGain * linearGain;
  }

  const float fullGainCompensation = static_cast<float>(1.0 / std::sqrt(std::max(1.0, gainPowerSum / static_cast<double>(kNumBands))));
  const float gainCompensation = std::pow(std::max(1.0e-4f, fullGainCompensation), Lerp(0.90f, 0.42f, aggression));

  for (int binIndex = 0; binIndex <= mFFTSize / 2; ++binIndex)
  {
    float linearGain = 1.f;

    if (binIndex > 0)
    {
      const int lowerBand = Clip(mBinLowerBand[binIndex], 0, kNumBands - 1);
      const int upperBand = Clip(mBinUpperBand[binIndex], 0, kNumBands - 1);
      const float blend = mBinBandBlend[binIndex];
      const float lowerDb = smoothedCorrectionDb[lowerBand];
      const float upperDb = smoothedCorrectionDb[upperBand];
      // faust-optimize: Sanitize correction before conversion
      const float correctionDb = Sanitize(lowerDb + ((upperDb - lowerDb) * blend));
      linearGain = DbToLinear(correctionDb) * gainCompensation;
    }

    const int fftIndex = mPermutation[binIndex];

    for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
    {
      mFftBuffer[channelIndex][fftIndex].re *= linearGain;
      mFftBuffer[channelIndex][fftIndex].im *= linearGain;
    }

    if (binIndex > 0 && binIndex < mFFTSize / 2)
    {
      const int mirrorIndex = mPermutation[mFFTSize - binIndex];

      for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
      {
        mFftBuffer[channelIndex][mirrorIndex].re *= linearGain;
        mFftBuffer[channelIndex][mirrorIndex].im *= linearGain;
      }
    }
  }

  const float inverseScale = 1.f / static_cast<float>(mFFTSize);

  for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
  {
    for (WDL_FFT_COMPLEX& fftBin : mFftBuffer[channelIndex])
    {
      fftBin.re *= inverseScale;
      fftBin.im *= inverseScale;
    }

    WDL_fft(mFftBuffer[channelIndex].data(), mFFTSize, 1);

    for (int sampleIndex = 0; sampleIndex < mFFTSize; ++sampleIndex)
    {
      mOverlapAddBuffer[channelIndex][sampleIndex] += mFftBuffer[channelIndex][sampleIndex].re * mWindow[sampleIndex];
    }

    for (int sampleIndex = 0; sampleIndex < mHopSize; ++sampleIndex)
      PushOutputSample(channelIndex, mOverlapAddBuffer[channelIndex][sampleIndex]);

    std::memmove(mOverlapAddBuffer[channelIndex].data(),
                 mOverlapAddBuffer[channelIndex].data() + mHopSize,
                 static_cast<size_t>(mFFTSize - mHopSize) * sizeof(float));
    std::fill(mOverlapAddBuffer[channelIndex].begin() + (mFFTSize - mHopSize), mOverlapAddBuffer[channelIndex].end(), 0.f);
  }

  // --- Professional auto makeup gain: measure output RMS of this hop then update gain ---
  {
    double outputPowerSum = 0.0;
    int outputSampleCount = 0;
    // Use the output FIFO tail (most recently pushed mHopSize samples per channel)
    for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
    {
      const float* fifo = mOutputFifo[channelIndex].data();
      // Walk back mHopSize samples from write pointer
      const int writeIdx = mOutputWriteIndex[channelIndex];
      for (int i = 0; i < mHopSize; ++i)
      {
        const int idx = (writeIdx - mHopSize + i + kOutputFifoSize) % kOutputFifoSize;
        const double s = static_cast<double>(fifo[idx]);
        outputPowerSum += s * s;
      }
      outputSampleCount += mHopSize;
    }
    const float hopOutputRms = (outputSampleCount > 0)
      ? static_cast<float>(std::sqrt(outputPowerSum / static_cast<double>(outputSampleCount)))
      : 0.f;

    if (signalIsUsable)
      UpdateMakeupGain(hopInputRms, hopOutputRms);
  }

  // Update spectrum visualizer at fixed ~30 FPS to keep GUI responsive and smooth
  const int hopsPerSecond = static_cast<int>(GetSampleRate() / mHopSize);
  const int targetFps = 30;
  const int skipCount = std::max(1, hopsPerSecond / targetFps);
  mVisUpdateCounter++;

  if (mVisUpdateCounter >= skipCount) {
    mVisUpdateCounter = 0;

    if (IGraphics* g = GetUI()) {
      for (int i = 0; i < g->NControls(); ++i) {
        if (auto* vis = dynamic_cast<SpectrumVisualizerControl*>(g->GetControl(i))) {
          vis->SetData(beforeSpec, afterSpec, referenceSpec, freqSpec, visBands);
          break;
        }
      }
    }
  }
}

void BronzeNoise::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // CPU-level denormal protection: flush denormals to zero (FTZ) and set denormals are zero (DAZ).
  // SSE intrinsics are x86/x64-only; on arm64 macOS math library handles this at OS level.
#ifdef __SSE__
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

  const int activeInputChannels = NInChansConnected();
  const int activeOutputChannels = NOutChansConnected();
  const int processedChannels = Clip(std::max(activeInputChannels, activeOutputChannels), 1, kMaxChannels);

  // Track per-block peak for I/O meters. We sample once per block (the last sample's magnitude) — UI polls at OnIdle rate, so any per-block peak is fine. Cheaper than per-sample max.
  float inPeakL = 0.f, inPeakR = 0.f, outPeakL = 0.f, outPeakR = 0.f;

  // One-pole smoothing coefficient for zipper-free parameter automation (~30ms @ 44.1kHz)
  constexpr float kSmoothCoeff = 0.0007f;

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex)
  {
    // Per-sample parameter smoothing: ramp smoothed values toward targets
    mSmoothedAmount += (static_cast<float>(GetParam(kAmount)->Value()) - mSmoothedAmount) * kSmoothCoeff;
    mSmoothedSmoothing += (static_cast<float>(GetParam(kSmoothing)->Value()) - mSmoothedSmoothing) * kSmoothCoeff;
    mSmoothedQ += (static_cast<float>(GetParam(kQ)->Value()) - mSmoothedQ) * kSmoothCoeff;
    mSmoothedCharacter += (static_cast<float>(GetParam(kCharacter)->Value()) - mSmoothedCharacter) * kSmoothCoeff;
    mSmoothedTransient += (static_cast<float>(GetParam(kTransient)->Value()) - mSmoothedTransient) * kSmoothCoeff;

    if (mBypassed)
    {
      // Bypass: dry signal pass-through, skip all DSP processing
      for (int channelIndex = 0; channelIndex < activeOutputChannels; ++channelIndex)
      {
        const float inputSample = channelIndex < activeInputChannels ? static_cast<float>(inputs[channelIndex][sampleIndex]) : 0.f;
        outputs[channelIndex][sampleIndex] = static_cast<double>(inputSample);
        // Track peaks even in bypass so the user can see signal flow.
        if (channelIndex == 0) { const float a = std::abs(inputSample); if (a > inPeakL) inPeakL = a; }
        else if (channelIndex == 1) { const float a = std::abs(inputSample); if (a > inPeakR) inPeakR = a; }
        const float outA = std::abs(inputSample);
        if (channelIndex == 0) { if (outA > outPeakL) outPeakL = outA; }
        else if (channelIndex == 1) { if (outA > outPeakR) outPeakR = outA; }
      }
      continue;
    }

    for (int channelIndex = 0; channelIndex < processedChannels; ++channelIndex)
    {
      const float inputSample = channelIndex < activeInputChannels ? static_cast<float>(inputs[channelIndex][sampleIndex]) : 0.f;
      mHopBuffer[channelIndex][mHopFill] = inputSample;
    }

    ++mHopFill;

    if (mHopFill >= mHopSize)
    {
      ProcessHop(processedChannels);
      mHopFill = 0;
    }

    // --- Professional auto makeup gain + true-peak limiting ---
    // Apply the smoothed RMS-matched makeup gain computed in ProcessHop,
    // then apply a true-peak brickwall limiter to guarantee no clipping.
    const float makeupLinear = std::pow(10.f, mAutoMakeupGainDb / 20.f);
    const float truePeakCeilingLinear = std::pow(10.f, kTruePeakCeiling / 20.f); // ~0.9886

    for (int channelIndex = 0; channelIndex < activeOutputChannels; ++channelIndex)
    {
      float output = PopOutputSample(channelIndex);

      // Apply makeup gain
      output *= makeupLinear;

      // True-peak envelope follower: track absolute peak with fast attack, slow release
      const float absSample = std::abs(output);
      if (absSample > mTruePeakLevel)
        mTruePeakLevel = absSample;                             // instant attack
      else
        mTruePeakLevel *= 0.9999f;                             // ~4.3 ms release @ 44.1 kHz

      // Compute limiting gain: if peak exceeds ceiling, attenuate proportionally
      float limiterGain = 1.f;
      if (mTruePeakLevel > truePeakCeilingLinear)
        limiterGain = truePeakCeilingLinear / mTruePeakLevel;

      output *= limiterGain;

      // faust-optimize: Final sanitization - prevent any stray NaN/Inf/denorm from reaching output
      output = SafeValue(output);
      output = FlushDenorm(output);

      // Final safety soft-clip (should never trigger with working limiter)
      output = SoftClip(output);

      // Final safety clamp
      output = Clip(output, -1.f, 1.f);

      outputs[channelIndex][sampleIndex] = output;

      // Track per-channel peak for I/O meters (audio → UI via atomic).
      const float outA = std::abs(output);
      if (channelIndex == 0) { if (outA > outPeakL) outPeakL = outA; }
      else if (channelIndex == 1) { if (outA > outPeakR) outPeakR = outA; }
    }

    // Track input peaks after the per-channel loop (input is read once per frame).
    for (int channelIndex = 0; channelIndex < activeInputChannels; ++channelIndex)
    {
      const float a = static_cast<float>(std::abs(inputs[channelIndex][sampleIndex]));
      if (channelIndex == 0) { if (a > inPeakL) inPeakL = a; }
      else if (channelIndex == 1) { if (a > inPeakR) inPeakR = a; }
    }
  }

  // Publish per-block peaks to UI thread (relaxed ordering is fine — meters are advisory).
  mInputPeakL.store(inPeakL, std::memory_order_relaxed);
  mInputPeakR.store(inPeakR, std::memory_order_relaxed);
  mOutputPeakL.store(outPeakL, std::memory_order_relaxed);
  mOutputPeakR.store(outPeakR, std::memory_order_relaxed);
}
#endif
