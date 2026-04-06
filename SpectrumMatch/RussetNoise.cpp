#include "RussetNoise.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

namespace
{
constexpr const char* kUIFontID = "RussetNoiseUIFont";
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

class TargetSelectorControl final : public IControl, public IVectorBase {
public:
  TargetSelectorControl(const IRECT& bounds, int parameterIndex, const char* label, const IVStyle& style)
    : IControl(bounds, parameterIndex), IVectorBase(style, false, true) {
    AttachIControl(this, label);
    DisablePrompt(false);
    SetPromptShowsParamLabel(false);
  }
  void Draw(IGraphics& graphics) override {
    const bool isHot = mMouseIsOver && !IsDisabled();
    DrawBackground(graphics, mRECT);
    DrawPressableRectangle(graphics, mWidgetBounds, false, isHot, IsDisabled());
    DrawLabel(graphics);
    const IColor frameColor = GetColor(kFR).WithOpacity(isHot ? 0.95f : 0.72f);
    graphics.DrawRoundRect(frameColor, mWidgetBounds.GetPadded(-0.75f), 3.0f, &mBlend, 1.2f);
    const IRECT textBounds = mValueBounds.GetPadded(-4.f).GetFromLeft(mValueBounds.W() - 18.f);
    graphics.DrawText(mStyle.valueText, mValueStr.Get(), textBounds, &mBlend);
    const IRECT arrowBounds = IRECT(mValueBounds.R - 18.f, mValueBounds.T, mValueBounds.R - 4.f, mValueBounds.B).GetMidVPadded(8.f);
    graphics.FillTriangle(GetColor(kX1).WithOpacity(0.9f),
      arrowBounds.L, arrowBounds.T + 1.f,
      arrowBounds.R, arrowBounds.T + 1.f,
      arrowBounds.MW(), arrowBounds.B - 1.f,
      &mBlend);
  }
  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    if (IsDisabled() || !GetParam()) return;
    PromptUserInput(mWidgetBounds);
  }
  void OnResize() override { SetTargetRECT(MakeRects(mRECT)); }
  void OnInit() override {
    if (const IParam* parameter = GetParam()) parameter->GetDisplay(mValueStr);
  }
  void SetDirty(bool triggerAction = true, int valueIndex = kNoValIdx) override {
    IControl::SetDirty(triggerAction, valueIndex);
    if (const IParam* parameter = GetParam()) parameter->GetDisplay(mValueStr);
  }
};
// Spectrum visualizer control for before/after/reference
class SpectrumVisualizerControl : public IControl {
public:
  SpectrumVisualizerControl(const IRECT& bounds)
    : IControl(bounds) {
    mBefore.fill(0.f);
    mAfter.fill(0.f);
    mReference.fill(0.f);
    mFreqs.fill(0.f);
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
    mMaxDb = std::min(mMaxDb, 40.f);
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
      mMaxDb = std::min(mMaxDb, 40.f);
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

    // Clean dark background
    g.FillRoundRect(IColor(255, 18, 22, 32), r, 6.f, &mBlend);
    g.DrawRoundRect(IColor(255, 80, 80, 90), r, 6.f, &mBlend, 1.0f);

    if (mBands < 2)
    {
      g.DrawText(IText(14.f, IColor(255, 140, 145, 150), nullptr, EAlign::Center, EVAlign::Middle),
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
    char dbStr[8];
    for (float db = std::floor(minDb / dbStep) * dbStep; db <= maxDb; db += dbStep) {
      if (db < minDb || db > maxDb) continue;
      const float y = r.B - ((db - minDb) / (maxDb - minDb)) * h;
      if (y < r.T || y > r.B) continue;
      g.DrawLine(IColor(255, 60, 65, 70), r.L, y, r.R, y, &mBlend, 1.0f);
      // Tick mark on axis - draw inside the rect
      g.DrawLine(IColor(255, 150, 150, 150), r.L, y, r.L + 4.f, y, &mBlend, 1.5f);
      // Label inside the plot area
      IRECT labelRect(r.L + 6.f, y - 8.f, r.L + 38.f, y + 8.f);
      std::snprintf(dbStr, sizeof(dbStr), "%.0f", db);
      g.DrawText(IText(9.f, IColor(255, 150, 155, 160), nullptr, EAlign::Near, EVAlign::Middle), 
                  dbStr, labelRect, &mBlend);
    }

    // Frequency scale on X axis
    constexpr std::array<float, 6> kLabelFreqs = {20.f, 50.f, 100.f, 500.f, 1000.f, 10000.f};
    constexpr std::array<const char*, 6> kLabelTexts = {"20", "50", "100", "500", "1k", "10k"};
    for (size_t i = 0; i < kLabelFreqs.size(); ++i) {
      const float x = r.L + (w * std::log10(kLabelFreqs[i] / 20.f) / std::log10(20000.f / 20.f));
      if (x < r.L || x > r.R) continue;
      g.DrawLine(IColor(255, 60, 65, 70), x, r.T, x, r.B, &mBlend, 1.0f);
      // Tick mark on axis - draw inside
      g.DrawLine(IColor(255, 150, 150, 150), x, r.B - 4.f, x, r.B, &mBlend, 1.5f);
      // Label below axis, inside
      IRECT labelRect(x - 18.f, r.B - 22.f, x + 18.f, r.B - 6.f);
      g.DrawText(IText(9.f, IColor(255, 150, 155, 160), nullptr, EAlign::Center, EVAlign::Middle), 
                  kLabelTexts[i], labelRect, &mBlend);
    }

    // Target curve (gold)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = r.B - ((mReference[i - 1] - minDb) / (maxDb - minDb)) * h;
      const float y1 = r.B - ((mReference[i] - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 255, 200, 80), x0, y0, x1, y1, &mBlend, 2.0f);
    }

    // Input curve (magenta)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = r.B - ((mBefore[i - 1] - minDb) / (maxDb - minDb)) * h;
      const float y1 = r.B - ((mBefore[i] - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 220, 80, 200), x0, y0, x1, y1, &mBlend, 1.5f);
    }

    // Output curve (cyan)
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = r.B - ((mAfter[i - 1] - minDb) / (maxDb - minDb)) * h;
      const float y1 = r.B - ((mAfter[i] - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 80, 220, 255), x0, y0, x1, y1, &mBlend, 2.5f);
    }

    // Legend
    g.DrawText(IText(10.f, IColor(255, 200, 200, 200), nullptr, EAlign::Near, EVAlign::Middle),
               "In", IRECT(r.L + 4.f, r.T + 4.f, r.L + 24.f, r.T + 16.f), &mBlend);
    g.DrawText(IText(10.f, IColor(255, 255, 200, 80), nullptr, EAlign::Near, EVAlign::Middle),
               "Tgt", IRECT(r.L + 28.f, r.T + 4.f, r.L + 56.f, r.T + 16.f), &mBlend);
    g.DrawText(IText(10.f, IColor(255, 80, 220, 255), nullptr, EAlign::Near, EVAlign::Middle),
               "Out", IRECT(r.L + 62.f, r.T + 4.f, r.L + 90.f, r.T + 16.f), &mBlend);
  }
private:
  std::array<float, 64> mBefore;
  std::array<float, 64> mAfter;
  std::array<float, 64> mReference;
  std::array<float, 64> mFreqs;
  int mBands = 0;
  float mMinDb = -80.f;
  float mMaxDb = 20.f;
  bool mDragging = false;
  float mDragStartY = 0.f;
  float mDragStartMinDb = 0.f;
  float mDragStartMaxDb = 0.f;
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

 RussetNoise::RussetNoise(const InstanceInfo& info)
 : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kAmount)->InitPercentage("Amount", 100.0);
  GetParam(kTarget)->InitEnum("Target", kTargetRusset, {"White", "Pink", "Russet", "Brown", "Equal-loudness"});
  GetParam(kSmoothing)->InitDouble("Smoothing", 0.55, 0.0, 1.0, 0.001);
  GetParam(kFFTSize)->InitEnum("FFT Size", kFFTSize2048, {"256", "512", "1024", "2048", "4096", "8192", "16384"});
  GetParam(kCharacter)->InitDouble("Character", 0.0, -1.0, 1.0, 0.01);
  GetParam(kTransient)->InitDouble("Transient", 0.5, 0.0, 1.0, 0.01);

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
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };
  mLayoutFunc = [&](IGraphics* graphics) {
    const IColor backgroundColor(255, 17, 19, 24);
    const IColor panelColor(255, 28, 31, 38);
    const IColor frameColor(255, 75, 79, 88);
    const IColor accentColor(255, 187, 101, 67);
    const IColor primaryTextColor(255, 239, 234, 226);
    const IColor secondaryTextColor(255, 156, 161, 169);

    const char* uiFont = nullptr;
    if (graphics->LoadFont(kUIFontID, "Segoe UI", ETextStyle::Normal)
        || graphics->LoadFont(kUIFontID, "Arial", ETextStyle::Normal)
        || graphics->LoadFont(kUIFontID, "Verdana", ETextStyle::Normal))
    {
      uiFont = kUIFontID;
    }

    graphics->AttachPanelBackground(backgroundColor);
    graphics->AttachPopupMenuControl(DEFAULT_TEXT.WithFont(uiFont).WithFGColor(primaryTextColor));
    const IVStyle controlStyle = DEFAULT_STYLE
      .WithColor(kBG, panelColor)
      .WithColor(kFG, IColor(255, 88, 94, 104))
      .WithColor(kPR, accentColor)
      .WithColor(kFR, frameColor)
      .WithColor(kHL, IColor(255, 255, 240, 224))
      .WithColor(kX1, primaryTextColor)
      .WithLabelText(IText(15.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Top))
      .WithValueText(IText(18.f, primaryTextColor, uiFont, EAlign::Center, EVAlign::Bottom))
      .WithRoundness(0.22f)
      .WithDrawShadows(false);
    const IVStyle selectorStyle = controlStyle.WithShowValue(false);
    const IRECT bounds = graphics->GetBounds().GetPadded(-24.f);
    const IRECT titleRect(bounds.L, bounds.T + 4.f, bounds.R, bounds.T + 52.f);
    const IRECT subtitleRect(bounds.L, titleRect.B - 4.f, bounds.R, titleRect.B + 24.f);
    graphics->AttachControl(new ITextControl(titleRect,
      "RUSSET NOISE",
      IText(34.f, accentColor, uiFont, EAlign::Near, EVAlign::Middle)));
    graphics->AttachControl(new ITextControl(subtitleRect,
      "Adaptive spectral targeting.",
      IText(14.f, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle)));
    const float controlTop = subtitleRect.B + 18.f;
    // Spectrum visualizer on TOP - takes most of the canvas
    const float visTop = controlTop;
    const float visBottom = bounds.B - 90.f;
    const IRECT visRect(bounds.L + 8.f, visTop, bounds.R - 8.f, visBottom);
    graphics->AttachControl(new SpectrumVisualizerControl(visRect));
    // All knobs in ONE row at bottom
    const float knobY = bounds.B - 70.f;
    const float knobHeight = 56.f;
    const float knobWidth = 80.f;
    const float gap = 12.f;
    const IRECT amountRect(bounds.L + 4.f, knobY, bounds.L + 4.f + knobWidth, knobY + knobHeight);
    const IRECT smoothingRect(amountRect.R + gap, knobY, amountRect.R + gap + knobWidth, knobY + knobHeight);
    const IRECT charRect(smoothingRect.R + gap, knobY, smoothingRect.R + gap + knobWidth, knobY + knobHeight);
    const IRECT transRect(charRect.R + gap, knobY, charRect.R + gap + knobWidth, knobY + knobHeight);
    const IRECT selectorRect(transRect.R + gap + 4.f, knobY - 8.f, transRect.R + gap + 130.f, knobY + knobHeight);
    const IRECT fftRect(selectorRect.R + gap - 4.f, knobY - 8.f, selectorRect.R + gap + 60.f, knobY + knobHeight);
    graphics->AttachControl(new IVKnobControl(amountRect, kAmount, "Amount", controlStyle));
    graphics->AttachControl(new IVKnobControl(smoothingRect, kSmoothing, "Smoothing", controlStyle));
    graphics->AttachControl(new IVKnobControl(charRect, kCharacter, "Character", controlStyle));
    graphics->AttachControl(new IVKnobControl(transRect, kTransient, "Transient", controlStyle));
    graphics->AttachControl(new TargetSelectorControl(selectorRect, kTarget, "Target", selectorStyle));
    graphics->AttachControl(new TargetSelectorControl(fftRect, kFFTSize, "FFT", selectorStyle));
    const IRECT footerRect(bounds.L, bounds.B - 14.f, bounds.R, bounds.B);
    graphics->AttachControl(new ITextControl(footerRect,
      "White, Pink, Red, Orange, Russet, Olive, Brown, Blue, Violet, Equal-loudness",
      IText(11.f, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle)));
  };
#endif
}

#if IPLUG_DSP
void RussetNoise::OnReset()
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

  for (int index = 0; index < mFFTSize; ++index)
    mPermutation[index] = WDL_fft_permute(mFFTSize, index);

  UpdateBandLayout(mSampleRate);
  ResetDspState();
}

void RussetNoise::OnParamChange(int paramIdx)
{
  if (paramIdx == kFFTSize)
  {
    // When FFT size changes, trigger a reset
    OnReset();
  }
}

void RussetNoise::ResetDspState()
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

void RussetNoise::UpdateBandLayout(double sampleRate)
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

void RussetNoise::PushOutputSample(int channelIndex, float value)
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

float RussetNoise::PopOutputSample(int channelIndex)
{
  if (mOutputCount[channelIndex] <= 0)
    return 0.f;

  float* outputBuffer = mOutputFifo[channelIndex].data();
  const float value = outputBuffer[mOutputReadIndex[channelIndex]];
  mOutputReadIndex[channelIndex] = (mOutputReadIndex[channelIndex] + 1) % kOutputFifoSize;
  --mOutputCount[channelIndex];
  return value;
}

float RussetNoise::EvaluateTargetDb(float frequencyHz, int targetMode) const
{
  const float clippedFrequencyHz = std::max(kMinTargetHz, frequencyHz);

  if (targetMode == kTargetEqualLoudness)
    return EvaluateEqualLoudnessDb(clippedFrequencyHz);

  float slopeDbPerOctave = 0.f;

  switch (targetMode)
  {
    case kTargetPink: slopeDbPerOctave = -3.f; break;
    case kTargetRusset: slopeDbPerOctave = -4.5f; break;
    case kTargetBrown: slopeDbPerOctave = -6.f; break;
    default: slopeDbPerOctave = 0.f; break;
  }

  return slopeDbPerOctave * std::log2(clippedFrequencyHz / 1000.f);
}

float RussetNoise::EvaluateEqualLoudnessDb(float frequencyHz) const
{
  return InterpolateLogTable(frequencyHz, kEqualLoudnessFrequenciesHz, kEqualLoudnessInverseDb);
}

float RussetNoise::LinearToDb(float linearValue) const
{
  return 20.f * std::log10(std::max(1.0e-12f, linearValue));
}

float RussetNoise::DbToLinear(float dbValue) const
{
  return std::pow(10.f, dbValue / 20.f);
}

// Compute RMS of a float buffer segment in linear scale
float RussetNoise::ComputeHopRms(int /*channelIndex*/, const float* buffer, int length) const
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
void RussetNoise::UpdateMakeupGain(float inputRms, float outputRms)
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

float RussetNoise::ClampCorrectionDb(float frequencyHz, float correctionDb, float aggression) const
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

void RussetNoise::SmoothBandCorrections(const std::array<float, kNumBands>& input,
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

void RussetNoise::ProcessHop(int channelCount)
{
  const int activeChannels = Clip(channelCount, 1, kMaxChannels);
  const float amount = static_cast<float>(GetParam(kAmount)->Value() * 0.01);
  const float amountDrive = MapAmountDrive(amount);
  const float aggression = MapAggression(amount);
  const float smoothing = static_cast<float>(GetParam(kSmoothing)->Value());
  const int targetMode = static_cast<int>(GetParam(kTarget)->Value());
  const float character = static_cast<float>(GetParam(kCharacter)->Value());
  const float liveSpectrumBlend = Lerp(0.10f, 0.58f, aggression);
  const float temporalAlpha = Clip((0.16f + (0.18f * amountDrive)) - (0.12f * smoothing), 0.05f, 0.34f);
  const int smoothingRadius = static_cast<int>(std::lround(std::pow(smoothing, 0.85f) * 6.f));

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

  for (int bandIndex = 0; bandIndex < visBands; ++bandIndex)
  {
    beforeSpec[bandIndex] = displaySpectrumDb[bandIndex];
    afterSpec[bandIndex] = displaySpectrumDb[bandIndex] + smoothedCorrectionDb[bandIndex];
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

  // Update spectrum visualizer control
  if (IGraphics* g = GetUI()) {
    for (int i = 0; i < g->NControls(); ++i) {
      if (auto* vis = dynamic_cast<SpectrumVisualizerControl*>(g->GetControl(i))) {
        vis->SetData(beforeSpec, afterSpec, referenceSpec, freqSpec, visBands);
        break;
      }
    }
  }
}

void RussetNoise::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int activeInputChannels = NInChansConnected();
  const int activeOutputChannels = NOutChansConnected();
  const int processedChannels = Clip(std::max(activeInputChannels, activeOutputChannels), 1, kMaxChannels);

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex)
  {
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
    }
  }
}
#endif
