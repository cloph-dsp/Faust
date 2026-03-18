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

float Lerp(float start, float end, float blend)
{
  return start + ((end - start) * Clip(blend, 0.f, 1.f));
}

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
// Spectrum visualizer control for before/after
class SpectrumVisualizerControl : public IControl {
public:
  SpectrumVisualizerControl(const IRECT& bounds)
    : IControl(bounds) {
    mBefore.fill(0.f);
    mAfter.fill(0.f);
    mFreqs.fill(0.f);
  }
  void SetData(const float* before, const float* after, const float* freqs, int bands) {
    for (int i = 0; i < bands && i < 64; ++i) {
      mBefore[i] = before[i];
      mAfter[i] = after[i];
      mFreqs[i] = freqs[i];
    }
    mBands = bands;
    SetDirty();
  }
  void Draw(IGraphics& g) override {
    const IRECT r = mRECT.GetPadded(-4.f);
    const float w = r.W();
    const float h = r.H();
    const float minDb = -72.f;
    const float maxDb = 12.f;

    g.FillRoundRect(IColor(255, 21, 24, 30), r, 8.f, &mBlend);
    g.DrawRoundRect(IColor(255, 68, 74, 82), r, 8.f, &mBlend, 1.2f);

    if (mBands < 2)
    {
      g.DrawText(IText(14.f, IColor(255, 156, 161, 169), nullptr, EAlign::Center, EVAlign::Middle),
                 "Waiting for signalâ€¦",
                 r,
                 &mBlend);
      return;
    }

    // Draw grid
    for (int db = -60; db <= 12; db += 12) {
      const float y = r.B - ((db - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 60, 60, 60), r.L, y, r.R, y, &mBlend, 1.0f);
      IRECT labelRect = r.GetFromLeft(48.f);
      labelRect.T = y - 8.f;
      labelRect.B = y + 8.f;
      const std::string dbLabel = std::to_string(db) + " dB";
      g.DrawText(IText(12.f, IColor(255, 120, 120, 120), nullptr, EAlign::Near, EVAlign::Middle), dbLabel.c_str(), labelRect, &mBlend);
    }

    constexpr std::array<float, 4> kLabelFreqs = {20.f, 100.f, 1000.f, 10000.f};
    constexpr std::array<const char*, 4> kLabelTexts = {"20", "100", "1k", "10k"};

    for (size_t labelIndex = 0; labelIndex < kLabelFreqs.size(); ++labelIndex)
    {
      const float x = r.L + (w * std::log10(kLabelFreqs[labelIndex] / 20.f) / std::log10(20000.f / 20.f));
      g.DrawLine(IColor(255, 52, 56, 62), x, r.T, x, r.B, &mBlend, 1.0f);

      IRECT labelRect(x - 18.f, r.B - 20.f, x + 18.f, r.B - 4.f);
      g.DrawText(IText(12.f, IColor(255, 120, 120, 120), nullptr, EAlign::Center, EVAlign::Middle), kLabelTexts[labelIndex], labelRect, &mBlend);
    }

    // Draw before spectrum
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = r.B - ((mBefore[i - 1] - minDb) / (maxDb - minDb)) * h;
      const float y1 = r.B - ((mBefore[i] - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 180, 180, 180), x0, y0, x1, y1, &mBlend, 2.0f);
    }
    // Draw after spectrum
    for (int i = 1; i < mBands; ++i) {
      const float x0 = r.L + (w * std::log10(std::max(20.f, mFreqs[i - 1]) / 20.f) / std::log10(20000.f / 20.f));
      const float x1 = r.L + (w * std::log10(std::max(20.f, mFreqs[i]) / 20.f) / std::log10(20000.f / 20.f));
      const float y0 = r.B - ((mAfter[i - 1] - minDb) / (maxDb - minDb)) * h;
      const float y1 = r.B - ((mAfter[i] - minDb) / (maxDb - minDb)) * h;
      g.DrawLine(IColor(255, 67, 187, 101), x0, y0, x1, y1, &mBlend, 2.5f);
    }

    g.DrawText(IText(14.f, IColor(255, 239, 234, 226), nullptr, EAlign::Near, EVAlign::Middle),
               "Spectrum: Before/After",
               r.GetFromTop(20.f).GetPadded(-8.f),
               &mBlend);
  }
private:
  std::array<float, 64> mBefore;
  std::array<float, 64> mAfter;
  std::array<float, 64> mFreqs;
  int mBands = 0;
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

#if IPLUG_DSP
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
      "Adaptive spectral targeting with simplified test UI.",
      IText(14.f, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle)));
    const float controlTop = subtitleRect.B + 18.f;
    const float controlHeight = 128.f;
    const float controlGap = 24.f;
    const float knobWidth = 200.f;
    const float selectorWidth = bounds.W() - (2.f * knobWidth) - (2.f * controlGap);
    const IRECT amountRect(bounds.L, controlTop, bounds.L + knobWidth, controlTop + controlHeight);
    const IRECT selectorRect(amountRect.R + controlGap, controlTop + 18.f, amountRect.R + controlGap + selectorWidth, controlTop + 84.f);
    const IRECT smoothingRect(bounds.R - knobWidth, controlTop, bounds.R, controlTop + controlHeight);
    graphics->AttachControl(new IVKnobControl(amountRect, kAmount, "Amount", controlStyle));
    graphics->AttachControl(new TargetSelectorControl(selectorRect, kTarget, "Target Curve", selectorStyle));
    graphics->AttachControl(new IVKnobControl(smoothingRect, kSmoothing, "Smoothing", controlStyle));
    // Spectrum visualizer
    const IRECT visRect(bounds.L + 8.f, controlTop + controlHeight + 14.f, bounds.R - 8.f, bounds.B - 34.f);
    graphics->AttachControl(new SpectrumVisualizerControl(visRect));
    const IRECT footerRect(bounds.L, bounds.B - 28.f, bounds.R, bounds.B);
    graphics->AttachControl(new ITextControl(footerRect,
      "White, Pink, Russet, Brown and perceptual equal-loudness targeting.",
      IText(13.f, secondaryTextColor, uiFont, EAlign::Far, EVAlign::Middle)));
  };
#endif
}

#if IPLUG_DSP
void RussetNoise::OnReset()
{
  mSampleRate = GetSampleRate();

  for (int index = 0; index < kFFTSize; ++index)
    mPermutation[index] = WDL_fft_permute(kFFTSize, index);

  UpdateBandLayout(mSampleRate);
  ResetDspState();
}

void RussetNoise::ResetDspState()
{
  for (auto& channelBuffer : mAnalysisBuffer)
    channelBuffer.fill(0.f);

  for (auto& channelBuffer : mOverlapAddBuffer)
    channelBuffer.fill(0.f);

  for (auto& channelBuffer : mHopBuffer)
    channelBuffer.fill(0.f);

  for (auto& channelBuffer : mOutputFifo)
    channelBuffer.fill(0.f);

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

  for (int sampleIndex = 0; sampleIndex < kFFTSize; ++sampleIndex)
  {
    const double phase = (kTwoPi * static_cast<double>(sampleIndex)) / static_cast<double>(kFFTSize);
    const double hann = 0.5 - (0.5 * std::cos(phase));
    mWindow[sampleIndex] = static_cast<float>(std::sqrt(std::max(0.0, hann)));
  }
}

void RussetNoise::UpdateBandLayout(double sampleRate)
{
  const float nyquistHz = std::max(1000.f, static_cast<float>(sampleRate * 0.5));
  const float binWidthHz = static_cast<float>(sampleRate / static_cast<double>(kFFTSize));
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
    int endBin = std::min((kFFTSize / 2) - 1, static_cast<int>(std::ceil(highHz / binWidthHz)));

    if (endBin < startBin)
    {
      const int centerBin = Clip(static_cast<int>(std::lround(centerHz / binWidthHz)), 1, (kFFTSize / 2) - 1);
      startBin = centerBin;
      endBin = centerBin;
    }

    mBandCentersHz[bandIndex] = centerHz;
    mBandStartBins[bandIndex] = startBin;
    mBandEndBins[bandIndex] = endBin;
  }

  int upperBand = 1;

  for (int binIndex = 0; binIndex <= kFFTSize / 2; ++binIndex)
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
  const float liveSpectrumBlend = Lerp(0.10f, 0.58f, aggression);
  const float temporalAlpha = Clip((0.16f + (0.18f * amountDrive)) - (0.12f * smoothing), 0.05f, 0.34f);
  const int smoothingRadius = static_cast<int>(std::lround(std::pow(smoothing, 0.85f) * 6.f));

  std::array<float, kNumBands> currentSpectrumDb {};
  std::array<float, kNumBands> targetSpectrumDb {};
  std::array<float, kNumBands> rawCorrectionDb {};
  std::array<float, kNumBands> smoothedCorrectionDb {};

  float beforeSpec[64] = {};
  float afterSpec[64] = {};
  float freqSpec[64] = {};
  const int visBands = std::min(kNumBands, 64);

  for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
  {
    std::memmove(mAnalysisBuffer[channelIndex].data(),
                 mAnalysisBuffer[channelIndex].data() + kHopSize,
                 static_cast<size_t>(kFFTSize - kHopSize) * sizeof(float));
    std::memcpy(mAnalysisBuffer[channelIndex].data() + (kFFTSize - kHopSize),
                mHopBuffer[channelIndex].data(),
                static_cast<size_t>(kHopSize) * sizeof(float));

    for (int sampleIndex = 0; sampleIndex < kFFTSize; ++sampleIndex)
    {
      mFftBuffer[channelIndex][sampleIndex].re = mAnalysisBuffer[channelIndex][sampleIndex] * mWindow[sampleIndex];
      mFftBuffer[channelIndex][sampleIndex].im = 0.f;
    }

    WDL_fft(mFftBuffer[channelIndex].data(), kFFTSize, 0);
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

  // Variables needed for target blend (declared outside the if block)
  float targetMeanDb = 0.f;
  const auto& displaySpectrumDb = signalIsUsable ? currentSpectrumDb : mAverageSpectrumDb;

  if (amountDrive > 1.0e-4f && signalIsUsable && mHasSpectrum)
  {
    float analysisMeanDb = 0.f;

    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      analysisMeanDb += mAverageSpectrumDb[bandIndex];
      targetSpectrumDb[bandIndex] = EvaluateTargetDb(mBandCentersHz[bandIndex], targetMode);
      targetMeanDb += targetSpectrumDb[bandIndex];
    }

    analysisMeanDb /= static_cast<float>(kNumBands);
    targetMeanDb /= static_cast<float>(kNumBands);

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
  }

  // Fix: Enhanced blend toward pure target spectrum at high Amount
  // Start blending at 50% Amount for more powerful effect
  const float targetBlend = Clip((amountDrive - 0.5f) / (1.0f - 0.5f), 0.f, 1.f);
  
  // Add extra amplification at high amounts for more pronounced effect
  const float extraGain = Lerp(1.f, 1.35f, amountDrive * amountDrive);

  if (targetBlend > 0.f && signalIsUsable && mHasSpectrum)
  {
    for (int bandIndex = 0; bandIndex < kNumBands; ++bandIndex)
    {
      const float targetOnlyShape = (targetSpectrumDb[bandIndex] - targetMeanDb) * extraGain;
      smoothedCorrectionDb[bandIndex] = Lerp(smoothedCorrectionDb[bandIndex], targetOnlyShape, targetBlend);
    }
  }

  for (int bandIndex = 0; bandIndex < visBands; ++bandIndex)
  {
    beforeSpec[bandIndex] = displaySpectrumDb[bandIndex];
    afterSpec[bandIndex] = displaySpectrumDb[bandIndex] + smoothedCorrectionDb[bandIndex];
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

  for (int binIndex = 0; binIndex <= kFFTSize / 2; ++binIndex)
  {
    float linearGain = 1.f;

    if (binIndex > 0)
    {
      const int lowerBand = mBinLowerBand[binIndex];
      const int upperBand = mBinUpperBand[binIndex];
      const float blend = mBinBandBlend[binIndex];
      const float lowerDb = smoothedCorrectionDb[lowerBand];
      const float upperDb = smoothedCorrectionDb[upperBand];
      const float correctionDb = lowerDb + ((upperDb - lowerDb) * blend);
      linearGain = DbToLinear(correctionDb) * gainCompensation;
    }

    const int fftIndex = mPermutation[binIndex];

    for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
    {
      mFftBuffer[channelIndex][fftIndex].re *= linearGain;
      mFftBuffer[channelIndex][fftIndex].im *= linearGain;
    }

    if (binIndex > 0 && binIndex < kFFTSize / 2)
    {
      const int mirrorIndex = mPermutation[kFFTSize - binIndex];

      for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
      {
        mFftBuffer[channelIndex][mirrorIndex].re *= linearGain;
        mFftBuffer[channelIndex][mirrorIndex].im *= linearGain;
      }
    }
  }

  const float inverseScale = 1.f / static_cast<float>(kFFTSize);

  for (int channelIndex = 0; channelIndex < activeChannels; ++channelIndex)
  {
    for (WDL_FFT_COMPLEX& fftBin : mFftBuffer[channelIndex])
    {
      fftBin.re *= inverseScale;
      fftBin.im *= inverseScale;
    }

    WDL_fft(mFftBuffer[channelIndex].data(), kFFTSize, 1);

    for (int sampleIndex = 0; sampleIndex < kFFTSize; ++sampleIndex)
    {
      mOverlapAddBuffer[channelIndex][sampleIndex] += mFftBuffer[channelIndex][sampleIndex].re * mWindow[sampleIndex];
    }

    for (int sampleIndex = 0; sampleIndex < kHopSize; ++sampleIndex)
      PushOutputSample(channelIndex, mOverlapAddBuffer[channelIndex][sampleIndex]);

    std::memmove(mOverlapAddBuffer[channelIndex].data(),
                 mOverlapAddBuffer[channelIndex].data() + kHopSize,
                 static_cast<size_t>(kFFTSize - kHopSize) * sizeof(float));
    std::fill(mOverlapAddBuffer[channelIndex].begin() + (kFFTSize - kHopSize), mOverlapAddBuffer[channelIndex].end(), 0.f);
  }

  // Update spectrum visualizer control
  if (IGraphics* g = GetUI()) {
    for (int i = 0; i < g->NControls(); ++i) {
      if (auto* vis = dynamic_cast<SpectrumVisualizerControl*>(g->GetControl(i))) {
        vis->SetData(beforeSpec, afterSpec, freqSpec, visBands);
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

    if (mHopFill >= kHopSize)
    {
      ProcessHop(processedChannels);
      mHopFill = 0;
    }

    // Apply auto makeup gain and soft clip
    for (int channelIndex = 0; channelIndex < activeOutputChannels; ++channelIndex) 
    { 
      float output = PopOutputSample(channelIndex); 
      float makeupGain = ComputeMakeupGain(GetParam(kAmount)->Value() * 0.01); 
      output = output * makeupGain; 
      output = SoftClip(output); 
      outputs[channelIndex][sampleIndex] = output; 
    }
  }
}
#endif
