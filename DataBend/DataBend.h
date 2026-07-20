#pragma once

#include "IPlug_include_in_plug_hdr.h"

#include <array>
#include <atomic>
#include <cstdint>
#include <vector>

static constexpr int kNumPresets = 1;

enum EParams
{
  kMode = 0,
  kIntensity,
  kDensity,
  kWindowMs,
  kRewindMs,
  kBits,
  kRateReduce,
  kJitter,
  kTone,
  kMix,
  kOutputTrim,
  kNumParams
};

enum EMode
{
  kModeRepeat = 0,
  kModeDropout,
  kModeRewind,
  kModeHybrid,
  kNumModes
};

enum EBitDepthSelector
{
  kBits4 = 0,
  kBits6,
  kBits8,
  kBits10,
  kBits12,
  kBits14,
  kBits16,
  kNumBitDepthSelectors
};

enum ERateReduceSelector
{
  kRate1 = 0,
  kRate2,
  kRate4,
  kRate8,
  kRate12,
  kRate16,
  kRate24,
  kRate32,
  kNumRateReduceSelectors
};

using namespace iplug;
using namespace igraphics;

class DataBend final : public Plugin
{
public:
  explicit DataBend(const InstanceInfo& info);

  std::atomic<float> mInputPeak { 0.f };
  std::atomic<float> mOutputPeak { 0.f };

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
#endif

private:
#if IPLUG_DSP
  static constexpr int kMaxChannels = 2;

  enum class EventType : uint8_t
  {
    None = 0,
    Repeat,
    Dropout,
    Rewind
  };

  struct EventState
  {
    bool active = false;
    EventType type = EventType::None;
    uint32_t sourceStartIndex = 0;
    int windowSamples = 0;
    int remainingSamples = 0;
    int playbackPos = 0;
    std::array<float, kMaxChannels> latchedSamples {};
  };

  static uint32_t NextPow2(uint32_t value);

  void ResetState();
  void ResizeHistoryBuffer();
  void ScheduleNextTrigger(float densityNorm, float jitterNorm);
  void TriggerEvent(int modeSelector,
                    float intensityNorm,
                    float windowMs,
                    float rewindMs,
                    float jitterNorm);
  float ProcessEventSample(int channel, float drySample, float intensityNorm) const;
  float ReadHistory(int channel, uint32_t index) const;
  float ApplyTone(int channel, float input, float lpAlpha);
  float DcBlock(int channel, float input);
  float QuantizeToBits(float input, int bits) const;
  float Random01();
  float RandomSigned();
  int MillisecondsToSamples(float milliseconds) const;

  std::array<std::vector<float>, kMaxChannels> mHistory;
  std::array<float, kMaxChannels> mToneState {};
  std::array<float, kMaxChannels> mRateHold {};
  std::array<float, kMaxChannels> mLastGlitchSample {};
  std::array<float, kMaxChannels> mDcState {};
  std::array<float, kMaxChannels> mAaState {};
  uint32_t mHistorySize = 0;
  uint32_t mHistoryMask = 0;
  uint32_t mWriteIndex = 0;
  uint32_t mHistoryFill = 0;
  uint32_t mRngState = 0xA341316Cu;
  int mRateCountdown = 0;
  int mNextTriggerSamples = 0;
  double mSampleRate = 44100.0;
  float mEventBlend = 0.f;
  float mParamAlphaFast = 0.f;
  float mParamAlphaSlow = 0.f;
  float mBlendAlpha = 0.f;
  float mLpAlpha = 0.f;
  float mDcAlpha = 0.f;
  float mAaAlpha = 0.f;

  float mSmIntensity = 0.55f;
  float mSmDensity = 0.40f;
  float mSmWindowMs = 60.f;
  float mSmRewindMs = 120.f;
  float mSmJitter = 0.25f;
  float mSmTone = 0.58f;
  float mSmMix = 1.f;
  float mSmOutputDb = 0.f;

  EventState mEvent;
#endif
};