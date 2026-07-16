#include "DataBend.h"
#include "DataBendIPlug2UI.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <string>

// Cross-platform denormal protection (FTZ + DAZ) on x86/x64. No-op on arm64 (OS handles).
#if (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) || defined(__SSE__) || defined(__SSE2__)
  #define DATABEND_HAS_SSE 1
  #include <intrin.h>
#else
  #define DATABEND_HAS_SSE 0
#endif

namespace
{
constexpr const char* kUIFontID = "DataBendUIFont";
constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 6.28318530717958647692f;
constexpr float kHistorySeconds = 2.5f;

constexpr std::array<int, kNumBitDepthSelectors> kBitDepthValues = {4, 6, 8, 10, 12, 14, 16};
constexpr std::array<int, kNumRateReduceSelectors> kRateReduceValues = {1, 2, 4, 8, 12, 16, 24, 32};

#if DATABEND_HAS_SSE
void EnableSseDenormals()
{
  _mm_setcsr(_mm_getcsr() | 0x8040); // FTZ (bit 15) + DAZ (bit 6)
}
#endif

template <typename T>
T ClampValue(T value, T lo, T hi)
{
  return std::min(hi, std::max(lo, value));
}

float Lerp(float start, float end, float blend)
{
  return start + ((end - start) * ClampValue(blend, 0.f, 1.f));
}

float LogFrequency(float norm, float minHz, float maxHz)
{
  return minHz * std::pow(maxHz / minHz, ClampValue(norm, 0.f, 1.f));
}

float FlushDenorm(float value)
{
  return (std::abs(value) < 1.0e-18f) ? 1.0e-18f : value;
}

float SoftClip(float value)
{
  // Asymmetric soft-clipper for more analog character
  if (value >= 0.0f)
    return std::tanh(value);
  else
    return 0.95f * std::tanh(value * 1.05f);
}

float DbToLinear(float db)
{
  return std::pow(10.f, db / 20.f);
}

float CoeffFromTimeMs(double sampleRate, double timeMs)
{
  const double safeTimeMs = std::max(0.1, timeMs);
  return static_cast<float>(1.0 - std::exp(-1.0 / (0.001 * safeTimeMs * sampleRate)));
}

} // ponytail: anonymous namespace close (was on line ~140 pre-extraction, lost when EnumSelectorControl moved out)

DataBend::DataBend(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kMode)->InitEnum("Mode", kModeHybrid, {"Repeat", "Dropout", "Rewind", "Hybrid"});
  GetParam(kIntensity)->InitPercentage("Intensity", 56.0);
  GetParam(kDensity)->InitPercentage("Density", 42.0);
  GetParam(kWindowMs)->InitDouble("Window", 72.0, 5.0, 250.0, 0.1, "ms");
  GetParam(kRewindMs)->InitDouble("Rewind", 145.0, 0.0, 500.0, 0.1, "ms");
  GetParam(kBits)->InitEnum("Bits", kBits8, {"4", "6", "8", "10", "12", "14", "16"});
  GetParam(kRateReduce)->InitEnum("Rate", kRate8, {"1x", "2x", "4x", "8x", "12x", "16x", "24x", "32x"});
  GetParam(kJitter)->InitPercentage("Jitter", 28.0);
  GetParam(kTone)->InitPercentage("Tone", 58.0);
  GetParam(kMix)->InitPercentage("Mix", 100.0);
  GetParam(kOutputTrim)->InitDouble("Output", 0.0, -18.0, 6.0, 0.1, "dB");

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() -> IGraphics* {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* graphics) {
    databend::ui::BuildLayout(graphics, this);
  };
#endif

#if IPLUG_DSP
  OnReset();
#endif
}

#if IPLUG_DSP
void DataBend::OnReset()
{
  mSampleRate = std::max(1.0, GetSampleRate());
#if DATABEND_HAS_SSE
  EnableSseDenormals();
#endif
  SetLatency(0);

  ResizeHistoryBuffer();

  mParamAlphaFast = CoeffFromTimeMs(mSampleRate, 5.0);
  mParamAlphaSlow = CoeffFromTimeMs(mSampleRate, 12.0);
  mBlendAlpha = CoeffFromTimeMs(mSampleRate, 2.5);

  ResetState();
}

uint32_t DataBend::NextPow2(uint32_t value)
{
  if (value < 2U)
    return 2U;

  value--;
  value |= value >> 1U;
  value |= value >> 2U;
  value |= value >> 4U;
  value |= value >> 8U;
  value |= value >> 16U;
  return ++value;
}

void DataBend::ResizeHistoryBuffer()
{
  const uint32_t target = NextPow2(static_cast<uint32_t>(std::ceil(mSampleRate * kHistorySeconds)) + 64U);

  if (target == mHistorySize)
    return;

  mHistorySize = target;
  mHistoryMask = mHistorySize - 1U;

  for (auto& buffer : mHistory)
    buffer.assign(mHistorySize, 0.f);
}

void DataBend::ResetState()
{
  for (auto& buffer : mHistory)
    std::fill(buffer.begin(), buffer.end(), 0.f);

  mToneState.fill(0.f);
  mRateHold.fill(0.f);
  mLastGlitchSample.fill(0.f);
  mEvent = {};
  mDcState.fill(0.f);
  mAaState.fill(0.f);
  mLpAlpha = 0.f;
  mDcAlpha = 0.f;
  mAaAlpha = 0.f;
  mEventBlend = 0.f;
  mWriteIndex = 0;
  mHistoryFill = 0;
  mRateCountdown = 0;
  mRngState = 0xA341316Cu;

  mSmIntensity = static_cast<float>(GetParam(kIntensity)->Value() * 0.01);
  mSmDensity = static_cast<float>(GetParam(kDensity)->Value() * 0.01);
  mSmWindowMs = static_cast<float>(GetParam(kWindowMs)->Value());
  mSmRewindMs = static_cast<float>(GetParam(kRewindMs)->Value());
  mSmJitter = static_cast<float>(GetParam(kJitter)->Value() * 0.01);
  mSmTone = static_cast<float>(GetParam(kTone)->Value() * 0.01);
  mSmMix = static_cast<float>(GetParam(kMix)->Value() * 0.01);
  mSmOutputDb = static_cast<float>(GetParam(kOutputTrim)->Value());

  ScheduleNextTrigger(mSmDensity, mSmJitter);
}

void DataBend::ScheduleNextTrigger(float densityNorm, float jitterNorm)
{
  const float shapedDensity = std::pow(ClampValue(densityNorm, 0.f, 1.f), 1.35f);
  const float baseMs = Lerp(2500.f, 60.f, shapedDensity);
  const float randomScale = 0.55f + (Random01() * (0.85f + 0.60f * ClampValue(jitterNorm, 0.f, 1.f)));
  mNextTriggerSamples = std::max(1, MillisecondsToSamples(baseMs * randomScale));
}

void DataBend::TriggerEvent(int modeSelector,
                            float intensityNorm,
                            float windowMs,
                            float rewindMs,
                            float jitterNorm)
{
  if (mHistoryFill < 64U)
    return;

  EventType type = EventType::Repeat;

  switch (ClampValue(modeSelector, 0, kNumModes - 1))
  {
    case kModeRepeat:
      type = EventType::Repeat;
      break;
    case kModeDropout:
      type = EventType::Dropout;
      break;
    case kModeRewind:
      type = EventType::Rewind;
      break;
    default:
    {
      const float chooser = Random01();
      if (chooser < (0.46f + 0.16f * intensityNorm))
        type = EventType::Repeat;
      else if (chooser < 0.80f)
        type = EventType::Rewind;
      else
        type = EventType::Dropout;
      break;
    }
  }

  const float jitterScale = 1.f + (RandomSigned() * jitterNorm * 1.55f); // Increased jitter range for more chaos
  const int maxWindowSamples = std::max(16, static_cast<int>(mHistorySize / 4U));
  // Window duration is heavily modulated by intensity for more aggressive changes
  int windowSamples = MillisecondsToSamples(windowMs * jitterScale * std::pow(0.50f + 0.50f * intensityNorm, 1.25f));
  windowSamples = ClampValue(windowSamples, 4, maxWindowSamples);

  // Rewind depth gets much deeper with jitter
  int rewindSamples = MillisecondsToSamples(rewindMs * (1.1f + (RandomSigned() * jitterNorm * 2.25f)));
  rewindSamples = ClampValue(rewindSamples, 0, std::max(0, static_cast<int>(mHistoryFill) - 8));

  const uint32_t latestIndex = (mWriteIndex + mHistoryMask) & mHistoryMask;
  const int availableSamples = static_cast<int>(mHistoryFill);
  const int safeLookback = std::max(0, availableSamples - windowSamples - 4);

  mEvent = {};
  mEvent.active = true;
  mEvent.type = type;
  mEvent.windowSamples = windowSamples;
  mEvent.remainingSamples = windowSamples;
  mEvent.playbackPos = 0;

  for (int channel = 0; channel < kMaxChannels; ++channel)
    mEvent.latchedSamples[static_cast<size_t>(channel)] = ReadHistory(channel, latestIndex);

  if (type == EventType::Repeat)
  {
    // Mode-specific voicing: Repeat can sometimes "drift" slightly back in history
    const int repeatLookback = ClampValue(static_cast<int>(rewindSamples * (0.15f + (0.85f * Random01() * jitterNorm))), 0, safeLookback);
    const uint32_t distance = static_cast<uint32_t>((windowSamples - 1) + repeatLookback);
    mEvent.sourceStartIndex = (latestIndex + mHistorySize - distance) & mHistoryMask;
  }
  else if (type == EventType::Rewind)
  {
    // Rewind is more aggressive with its jump selection
    const int jumpBack = ClampValue(std::max(windowSamples, rewindSamples), windowSamples, std::max(windowSamples, safeLookback));
    const uint32_t distance = static_cast<uint32_t>(jumpBack - 1);
    mEvent.sourceStartIndex = (latestIndex + mHistorySize - distance) & mHistoryMask;
  }
  else
  {
    mEvent.sourceStartIndex = latestIndex;
  }
}

float DataBend::ProcessEventSample(int channel, float drySample, float intensityNorm) const
{
  if (!mEvent.active)
    return drySample;

  switch (mEvent.type)
  {
    case EventType::Repeat:
    {
      const int phase = (mEvent.windowSamples > 0) ? (mEvent.playbackPos % mEvent.windowSamples) : 0;
      const uint32_t index = (mEvent.sourceStartIndex + static_cast<uint32_t>(phase)) & mHistoryMask;
      const float sample = ReadHistory(channel, index);
      
      // Tighter fade windows for "clickier" digital repeat signatures
      const int fadeSamples = std::min(12, std::max(2, mEvent.windowSamples / 8));
      float edgeShape = 1.0f;

      if (phase < fadeSamples)
        edgeShape = std::pow(static_cast<float>(phase) / static_cast<float>(fadeSamples), 0.5f);
      else if ((mEvent.windowSamples - phase) < fadeSamples)
        edgeShape = std::pow(static_cast<float>(mEvent.windowSamples - phase) / static_cast<float>(fadeSamples), 0.75f);

      return sample * edgeShape;
    }

    case EventType::Rewind:
    {
      const int phase = ClampValue(mEvent.playbackPos, 0, std::max(0, mEvent.windowSamples - 1));
      const uint32_t index = (mEvent.sourceStartIndex + static_cast<uint32_t>(phase)) & mHistoryMask;
      return ReadHistory(channel, index);
    }

    case EventType::Dropout:
    {
      const float held = mEvent.latchedSamples[static_cast<size_t>(channel)];
      const float keepAmount = 0.35f * (1.f - (0.85f * intensityNorm));
      return held * keepAmount;
    }

    default:
      return drySample;
  }
}

float DataBend::ReadHistory(int channel, uint32_t index) const
{
  const int safeChannel = ClampValue(channel, 0, kMaxChannels - 1);
  return mHistory[static_cast<size_t>(safeChannel)][index & mHistoryMask];
}

float DataBend::ApplyTone(int channel, float input, float lpAlpha)
{
  const int safeChannel = ClampValue(channel, 0, kMaxChannels - 1);
  const float lp = ClampValue(lpAlpha, 0.001f, 1.0f);
  const float next = mToneState[static_cast<size_t>(safeChannel)] + (lp * (input - mToneState[static_cast<size_t>(safeChannel)]));
  mToneState[static_cast<size_t>(safeChannel)] = FlushDenorm(next);
  return mToneState[static_cast<size_t>(safeChannel)];
}

float DataBend::DcBlock(int channel, float input)
{
  const int safeChannel = ClampValue(channel, 0, kMaxChannels - 1);
  const float hp = input - mDcState[static_cast<size_t>(safeChannel)];
  mDcState[static_cast<size_t>(safeChannel)] += mDcAlpha * hp;
  return FlushDenorm(hp);
}

float DataBend::QuantizeToBits(float input, int bits) const
{
  const int safeBits = ClampValue(bits, 2, 24);
  const float clipped = ClampValue(input, -1.f, 1.0f);
  
  // Use bit-manipulation for power-of-two bits if possible for perf
  // For arbitrary bits, this is standard
  const float scale = static_cast<float>((1 << (safeBits - 1)) - 1);
  if (scale <= 0.f) return 0.f;
  
  return std::round(clipped * scale) / scale;
}

float DataBend::Random01()
{
  mRngState ^= mRngState << 13;
  mRngState ^= mRngState >> 17;
  mRngState ^= mRngState << 5;
  return static_cast<float>(mRngState & 0x00FFFFFFu) / 16777215.0f;
}

float DataBend::RandomSigned()
{
  return (Random01() * 2.f) - 1.f;
}

int DataBend::MillisecondsToSamples(float milliseconds) const
{
  return static_cast<int>(std::round(milliseconds * 0.001 * mSampleRate));
}

void DataBend::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (nFrames <= 0 || !outputs)
    return;

  const int activeInputChannels = std::max(0, NInChansConnected());
  const int activeOutputChannels = std::max(0, NOutChansConnected());
  const int channelCount = ClampValue(std::max(std::max(activeInputChannels, activeOutputChannels), 1), 1, kMaxChannels);

  // --- Control Rate Pre-calculations ---
  // Fetch parameters once per block
  const int modeSelector = ClampValue(static_cast<int>(GetParam(kMode)->Value()), 0, kNumModes - 1);
  const int bits = kBitDepthValues[ClampValue(static_cast<int>(GetParam(kBits)->Value()), 0, kNumBitDepthSelectors - 1)];
  const int rateFactor = kRateReduceValues[ClampValue(static_cast<int>(GetParam(kRateReduce)->Value()), 0, kNumRateReduceSelectors - 1)];

  const float intensityTarget = static_cast<float>(GetParam(kIntensity)->Value() * 0.01);
  const float densityTarget = static_cast<float>(GetParam(kDensity)->Value() * 0.01);
  const float windowTarget = static_cast<float>(GetParam(kWindowMs)->Value());
  const float rewindTarget = static_cast<float>(GetParam(kRewindMs)->Value());
  const float jitterTarget = static_cast<float>(GetParam(kJitter)->Value() * 0.01);
  const float toneTarget = static_cast<float>(GetParam(kTone)->Value() * 0.01);
  const float mixTarget = static_cast<float>(GetParam(kMix)->Value() * 0.01);
  const float outputDbTarget = static_cast<float>(GetParam(kOutputTrim)->Value());

  // Precompute 1-pole filter alphas once per block (was per-sample inside ApplyTone).
  // Per-block constant values
  const float driveBase = 1.0f;
  const float driveScale = 1.8f;
  const float toneFreqBase = 450.f;
  const float toneFreqScale = 40.f;
  const float toneCutoffHz = toneFreqBase * std::pow(toneFreqScale, ClampValue(toneTarget, 0.f, 1.f));
  mLpAlpha = ClampValue(static_cast<float>(1.0 - std::exp(-kTwoPi * toneCutoffHz / mSampleRate)), 0.001f, 1.0f);
  mDcAlpha = ClampValue(static_cast<float>(1.0 - std::exp(-kTwoPi * 10.0 / mSampleRate)), 0.0001f, 1.0f);
  const double aaCutoffHz = mSampleRate / (2.0 * std::max(1, rateFactor));
  mAaAlpha = ClampValue(static_cast<float>(1.0 - std::exp(-kTwoPi * aaCutoffHz / mSampleRate)), 0.0001f, 1.0f);

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex)
  {
    // Smooth parameters
    mSmIntensity += (intensityTarget - mSmIntensity) * mParamAlphaFast;
    mSmDensity += (densityTarget - mSmDensity) * mParamAlphaFast;
    mSmWindowMs += (windowTarget - mSmWindowMs) * mParamAlphaSlow;
    mSmRewindMs += (rewindTarget - mSmRewindMs) * mParamAlphaSlow;
    mSmJitter += (jitterTarget - mSmJitter) * mParamAlphaSlow;
    mSmTone += (toneTarget - mSmTone) * mParamAlphaSlow;
    mSmMix += (mixTarget - mSmMix) * mParamAlphaFast;
    mSmOutputDb += (outputDbTarget - mSmOutputDb) * mParamAlphaSlow;

    const float intensityNorm = ClampValue(mSmIntensity, 0.f, 1.f);
    const float densityNorm = ClampValue(mSmDensity, 0.f, 1.f);
    const float jitterNorm = ClampValue(mSmJitter, 0.f, 1.f);

    float dry[kMaxChannels] = {0.f, 0.f};

    // --- History Write ---
    for (int channel = 0; channel < channelCount; ++channel)
    {
      const int sourceChannel = (channel < activeInputChannels) ? channel : 0;
      const sample* source = (inputs && sourceChannel < activeInputChannels) ? inputs[sourceChannel] : nullptr;
      float value = (source && std::isfinite(source[sampleIndex])) ? static_cast<float>(source[sampleIndex]) : 0.f;
      dry[channel] = ClampValue(value, -1.f, 1.f);
      mHistory[static_cast<size_t>(channel)][mWriteIndex] = dry[channel];
    }

    // ponytail: per-channel loop is unavoidable for peak capture, channels=2 max
    for (int channel = 0; channel < channelCount; ++channel)
      mInputPeak.store(std::max(mInputPeak.load(), std::abs(dry[channel])));

    if (channelCount == 1)
      mHistory[1][mWriteIndex] = dry[0];

    mWriteIndex = (mWriteIndex + 1U) & mHistoryMask;
    if (mHistoryFill < mHistorySize)
      ++mHistoryFill;

    // --- Event Scheduling ---
    if (!mEvent.active)
    {
      --mNextTriggerSamples;

      if (mHistoryFill > 64U && mNextTriggerSamples <= 0)
      {
        if (Random01() <= densityNorm)
          TriggerEvent(modeSelector, intensityNorm, mSmWindowMs, mSmRewindMs, jitterNorm);

        ScheduleNextTrigger(densityNorm, jitterNorm);
      }
    }

    // --- Glitch Processing ---
    float rawGlitch[kMaxChannels] = {0.f, 0.f};

    for (int channel = 0; channel < channelCount; ++channel)
    {
      if (mEvent.active)
      {
        rawGlitch[channel] = ProcessEventSample(channel, dry[channel], intensityNorm);
        mLastGlitchSample[static_cast<size_t>(channel)] = rawGlitch[channel];
      }
      else
      {
        rawGlitch[channel] = mLastGlitchSample[static_cast<size_t>(channel)];
      }
    }

    const float glitchDepth = 0.85f;
    const float blendTarget = mEvent.active ? 1.f : 0.f;
    // Smoother crossfades between events for less clicky, more "fluid" glitching
    const float currentBlendAlpha = mEvent.active ? mBlendAlpha : mBlendAlpha * 0.45f;
    mEventBlend += (blendTarget - mEventBlend) * currentBlendAlpha;
    mEventBlend = ClampValue(mEventBlend, 0.f, 1.f);

    float timeStage[kMaxChannels] = {0.f, 0.f};
    // Higher drive for more aggressive "databend" crunch
    const float glitchDrive = 1.0f + (5.5f * intensityNorm);
    const float glitchNormalization = 1.0f / (1.0f + 0.95f * intensityNorm);

    for (int channel = 0; channel < channelCount; ++channel)
    {
      const float drivenGlitch = SoftClip(rawGlitch[channel] * glitchDrive);
      const float normalizedGlitch = drivenGlitch * glitchNormalization;
      
      timeStage[channel] = dry[channel] + ((normalizedGlitch - dry[channel]) * glitchDepth * mEventBlend);
    }

    // --- Corruption & Bitcrush ---
    if (mRateCountdown <= 0)
    {
      const float crushDrive = driveBase + (4.0f * intensityNorm);
      const float crushNormalization = 1.0f / (1.0f + 0.5f * intensityNorm);

      for (int channel = 0; channel < channelCount; ++channel)
      {
        const float driven = SoftClip(timeStage[channel] * crushDrive);
        const float normalized = driven * crushNormalization;
        mAaState[static_cast<size_t>(channel)] += mAaAlpha * (normalized - mAaState[static_cast<size_t>(channel)]);
        mRateHold[static_cast<size_t>(channel)] = QuantizeToBits(mAaState[static_cast<size_t>(channel)], bits);
      }

      const int jitteredRate = rateFactor + static_cast<int>(RandomSigned() * jitterNorm * rateFactor * 0.5f);
      mRateCountdown = std::max(1, jitteredRate);
    }

    --mRateCountdown;

    // --- Tone & Output ---
    const float wetMix = ClampValue(mSmMix, 0.f, 1.f);
    const float outputGain = DbToLinear(mSmOutputDb);

    for (int channel = 0; channel < activeOutputChannels && channel < kMaxChannels; ++channel)
    {
      const int inputChannel = std::min(channel, channelCount - 1);
      const float crushedBlend = 0.25f + (0.75f * mEventBlend);
      const float crushed = dry[inputChannel] + ((mRateHold[static_cast<size_t>(inputChannel)] - dry[inputChannel]) * crushedBlend);
      const float wetDifference = crushed - dry[inputChannel];
      const float dcBlock = DcBlock(inputChannel, wetDifference);
      const float tonedDifference = ApplyTone(inputChannel, dcBlock, mLpAlpha);
      const float wetSample = dry[inputChannel] + tonedDifference;
      float outputSample = dry[inputChannel] + ((wetSample - dry[inputChannel]) * wetMix);
      outputSample *= outputGain;
      
      // Safety clip
      outputSample = ClampValue(outputSample, -1.0f, 1.0f);
      outputs[channel][sampleIndex] = static_cast<sample>(FlushDenorm(outputSample));

      // Track output peak per-sample
      mOutputPeak.store(std::max(mOutputPeak.load(), std::abs(outputSample)));
    }

    if (mEvent.active)
    {
      ++mEvent.playbackPos;
      --mEvent.remainingSamples;

      if (mEvent.remainingSamples <= 0)
        mEvent.active = false;
    }
  }
}
#endif