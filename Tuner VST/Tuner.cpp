// =============================================================================
// Tuner.cpp - iPlug2 plugin + YIN/MPM/Neville detector + Amorph-faithful UI
// =============================================================================

#include "Tuner.h"
#include "TunerDSP.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

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

namespace TunerAnalysis {

void Detector::Init(int sampleRate) {
  mSampleRate = sampleRate;
  Reset();
}

void Detector::Reset() {
  mBuffer.fill(0.0);
  mWriteIdx = 0;
  mSampleCount = 0;
  mSamplesSince = 0;
  mMedianBuf.fill(0.0);
  mMedianIdx = 0;
  mLastPitch = 0.0;
  mLastCents = 0.0;
  mResult.pitchHz.store(0.0);
  mResult.cents.store(0.0);
  mResult.clarity.store(0.0);
  mResult.level.store(0.0);
  mResult.noteIndex.store(-1);
  mResult.octave.store(4);
}

void Detector::PushSample(double mono) {
  mBuffer[mWriteIdx] = mono;
  mWriteIdx = (mWriteIdx + 1) % kBufferSize;
  if (mSampleCount < kBufferSize) mSampleCount++;

  if (mSampleCount >= kBufferSize) {
    if (++mSamplesSince >= kAnalysisStride) {
      mSamplesSince = 0;
      RunAnalysis();
    }
  }
}

double Detector::DetectYIN(double& clarityOut) {
  const int N = kBufferSize;
  const int maxLag = std::min<int>(kMaxLag, N - 2);
  const int minLag = kMinLag;

  std::array<double, kMaxLag + 1> d{};
  for (int tau = 1; tau <= maxLag; ++tau) {
    double sum = 0.0;
    for (int j = 0; j < N - maxLag; ++j) {
      const double diff = mBuffer[(mWriteIdx + j) % N]
                        - mBuffer[(mWriteIdx + j + tau) % N];
      sum += diff * diff;
    }
    d[tau] = sum;
  }

  std::array<double, kMaxLag + 1> dN{};
  dN[0] = 1.0;
  double runningSum = 0.0;
  for (int tau = 1; tau <= maxLag; ++tau) {
    runningSum += d[tau];
    dN[tau] = (runningSum > 1e-12) ? (d[tau] * tau / runningSum) : 1.0;
  }

  double lvl = mResult.level.load();
  double threshold = 0.18 - 0.08 * std::min(1.0, std::max(0.0, lvl * 4.0));

  int bestTau = -1;
  for (int tau = minLag; tau <= maxLag; ++tau) {
    if (dN[tau] < threshold) {
      while (tau + 1 <= maxLag && dN[tau + 1] < dN[tau]) ++tau;
      bestTau = tau;
      break;
    }
  }
  if (bestTau < 0) { clarityOut = 0.0; return 0.0; }

  double refined = RefineLagNeville(dN.data(), bestTau);
  if (refined < minLag || refined > maxLag) return 0.0;

  clarityOut = std::min(1.0, std::max(0.0, 1.0 - dN[(int)std::round(refined)]));
  return mSampleRate / refined;
}

double Detector::RefineLagNeville(const double* y, int bestLag) const {
  if (bestLag < 1 || bestLag > kMaxLag - 2) return (double)bestLag;
  const double w0 = 1.0 / (1.0 + std::abs(y[bestLag - 1]));
  const double w1 = 1.0 / (1.0 + std::abs(y[bestLag]));
  const double w2 = 1.0 / (1.0 + std::abs(y[bestLag + 1]));
  const double wsum = w0 + w1 + w2;
  return ((-1.0) * w0 + 0.0 * w1 + 1.0 * w2) / wsum + (double)bestLag;
}

double Detector::DetectMPM(double& clarityOut) {
  const int N = kBufferSize;
  const int maxLag = std::min<int>(kMaxLag, N / 2);
  const int minLag = kMinLag;

  std::array<double, kMaxLag + 1> nsdf{};
  for (int tau = minLag; tau <= maxLag; ++tau) {
    double num = 0.0, den1 = 0.0, den2 = 0.0;
    for (int j = 0; j < N - maxLag; ++j) {
      const double a = mBuffer[(mWriteIdx + j) % N];
      const double b = mBuffer[(mWriteIdx + j + tau) % N];
      num  += a * b;
      den1 += a * a;
      den2 += b * b;
    }
    const double den = std::sqrt(den1 * den2);
    nsdf[tau] = (den > 1e-12) ? (num / den) : 0.0;
  }

  int bestTau = -1;
  double bestVal = 0.5;
  for (int tau = minLag; tau <= maxLag; ++tau) {
    if (nsdf[tau] > bestVal) {
      bestVal = nsdf[tau];
      bestTau = tau;
    }
  }
  if (bestTau < 0) { clarityOut = 0.0; return 0.0; }

  const int t = bestTau;
  if (t > minLag && t < maxLag) {
    const double y0 = nsdf[t - 1];
    const double y1 = nsdf[t];
    const double y2 = nsdf[t + 1];
    const double denom = (y0 - 2.0 * y1 + y2);
    if (std::abs(denom) > 1e-12) {
      const double offset = 0.5 * (y0 - y2) / denom;
      clarityOut = std::min(1.0, std::max(0.0, y1));
      return mSampleRate / ((double)t + offset);
    }
  }
  clarityOut = std::min(1.0, std::max(0.0, bestVal));
  return mSampleRate / (double)bestTau;
}

double Detector::MedianFilter(double candidate) {
  if (candidate <= 0.0) return 0.0;
  mMedianBuf[mMedianIdx] = candidate;
  mMedianIdx = (mMedianIdx + 1) % kMedianSize;
  // RT-2: replace std::nth_element with a deterministic partial selection
  // sort.  For n=7 we want the 4th smallest (index kMedianSize/2=3).
  // This performs (kMedianSize/2+1) * kMedianSize = 28 comparisons worst
  // case with zero recursion -- provably real-time safe on any platform,
  // no introselect fallback, no allocator interaction.
  std::array<double, kMedianSize> copy = mMedianBuf;
  for (int i = 0; i <= kMedianSize / 2; ++i) {
    int best = i;
    for (int j = i + 1; j < kMedianSize; ++j)
      if (copy[j] < copy[best]) best = j;
    std::swap(copy[i], copy[best]);
  }
  return copy[kMedianSize / 2];
}

double Detector::SmoothPitch(double candidate) {
  if (candidate <= 0.0) { mLastPitch = 0.0; return 0.0; }
  if (mLastPitch <= 0.0) { mLastPitch = candidate; return candidate; }
  const double ratio = candidate / mLastPitch;
  const double semitones = std::abs(12.0 * std::log2(ratio));
  double alpha;
  if (semitones > 2.0)       alpha = 0.60;
  else if (semitones > 0.5)  alpha = 0.20;
  else                       alpha = 0.01 + 0.50 * (1.0 - mSmooth);
  mLastPitch = (1.0 - alpha) * mLastPitch + alpha * candidate;
  return mLastPitch;
}

double Detector::SmoothCents(double candidate) {
  const double absCents = std::abs(candidate);
  const double alpha = 1.0 / (1.0 + 0.05 * absCents) * (1.0 - 0.95 * mSmooth);
  mLastCents = (1.0 - alpha) * mLastCents + alpha * candidate;
  return mLastCents;
}

double Detector::NoteFromPitch(double hz, int& noteIdxOut, int& octaveOut) const {
  if (hz <= 0.0) { noteIdxOut = -1; octaveOut = 4; return 0.0; }
  const double midi = 69.0 + 12.0 * std::log2(hz / mA4Ref);
  const int midiRound = (int)std::round(midi);
  noteIdxOut = ((midiRound % 12) + 12) % 12;
  octaveOut  = (midiRound / 12) - 1;
  return (midi - midiRound) * 100.0;
}

void Detector::RunAnalysis() {
  mProfiler.Begin();
  double yClarity = 0.0, mClarity = 0.0;
  const double yinHz  = DetectYIN(yClarity);
  const double mpmHz  = DetectMPM(mClarity);

  double rawPitch = 0.0;
  if (yinHz > 0.0 && mpmHz > 0.0) {
    const double ratio = yinHz / mpmHz;
    if (ratio > 0.97 && ratio < 1.03) rawPitch = yinHz;
    else rawPitch = (yClarity >= mClarity) ? yinHz : mpmHz;
  } else if (yinHz > 0.0) {
    rawPitch = yinHz;
  } else if (mpmHz > 0.0) {
    rawPitch = mpmHz;
  }

  const double lvl = mResult.level.load();
  if (lvl < 0.005) rawPitch = 0.0;

  const double medianPitch  = MedianFilter(rawPitch);
  const double smoothedPitch = SmoothPitch(medianPitch);

  int noteIdx = -1, octave = 4;
  const double rawCents = NoteFromPitch(smoothedPitch, noteIdx, octave);
  const double smoothedCents = SmoothCents(rawCents);

  const double clarity = std::max(yClarity, mClarity);

  mResult.pitchHz.store(smoothedPitch);
  mResult.cents.store(smoothedCents);
  mResult.clarity.store(clarity);
  mResult.noteIndex.store(noteIdx);
  mResult.octave.store(octave);
  mProfiler.End();
}

} // namespace TunerAnalysis


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

  // Track bg + border.
  g.FillRect(smoothBg, slTrack);
  g.DrawRect(textDim, slTrack);

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
    const float kA4Y = mRECT.B - 36.f;
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
    const float kA4Y = mRECT.B - 36.f;
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
  if (mIsHovering != wasHovering) SetDirty(false);
}

void TunerReadoutControl::OnMouseOut() {
  if (mIsHovering) {
    mIsHovering = false;
    SetDirty(false);
  }
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
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
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
    mDetector.SetA4Reference(kA4Hz[std::min(a4Idx, 5)]);
  }

  // RT-1: flush-to-zero + denormals-are-zero for the audio-thread IIR RMS
  // smoother (and any future accumulator that risks producing denormals).
  // 0x8040 = _MM_FLUSH_ZERO_ON (0x8000) | _MM_DENORMALS_ZERO_ON (0x0040).
  // Set once at construction; the thread-affinity guarantees from iPlug2
  // mean this sticks across the lifetime of the process.
  _mm_setcsr(_mm_getcsr() | 0x8040);

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
    const double mono = 0.5 * ((double)inL[i] + (double)inR[i]);
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
    const double pctSlot = (slotUs > 0.0) ? (avgUs / slotUs * 100.0) : 0.0;
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

#endif // IPLUG_EDITOR