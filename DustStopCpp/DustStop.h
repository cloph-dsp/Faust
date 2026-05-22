#pragma once

#include "IPlug_include_in_plug_hdr.h"

#include <array>
#include <cstdint>
#include <vector>
#include <cmath>

static constexpr int kNumPresets = 1;

enum EParams
{
  kTrigger = 0,
  kTimeDiv,
  kDust,
  kDamping,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class DustStop final : public Plugin
{
public:
  explicit DustStop(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
#endif

private:
#if IPLUG_DSP
  static constexpr int kMaxChannels = 2;
  static constexpr uint32_t kBufSize = 262144;
  static constexpr uint32_t kBufMask = kBufSize - 1;

  // Harmonic comb bank — 5 combs forming a D major chord
  static constexpr int kNumCombs = 5;
  static constexpr int kCombBufSize = 256;
  static constexpr int kCombMask = kCombBufSize - 1;
  // Delays: D4(294Hz), A4(441Hz), D5(588Hz), G5(787Hz), D6(1192Hz) at 44.1kHz
  static constexpr int kCombDelays[kNumCombs] = {150, 100, 75, 56, 37};
  // Base feedback per comb — higher = more "shards"
  static constexpr float kCombFBBase[kNumCombs] = {0.80f, 0.85f, 0.92f, 0.88f, 0.95f};

  // Reboot crossfade duration equals tape stop duration (time_div)
  int mBootDurSamps = 0;

  enum class Phase : uint8_t { kIdle, kTapeStop, kDustTail, kDone, kReboot };

  Phase mPhase = Phase::kIdle;
  int mElapsed = 0;
  int mDurSamps = 0;
  int mTailSamps = 0;
  int mTotalSamps = 0;
  bool mLastTrigger = false;

  // Circular delay buffer for tape stop
  std::array<std::vector<float>, kMaxChannels> mDelayBuf;
  uint32_t mWritePos = 0;

  // 5 parallel comb filters, each with own circular buffer per channel
  float mCombs[kNumCombs][kMaxChannels][kCombBufSize]{};
  int mCombIOTAs[kNumCombs]{};

  // Reboot timer
  int mBootElapsed = 0;

  // DC blocker + sub oscillator
  float mHpL = 0.f, mHpPrevL = 0.f;
  float mHpR = 0.f, mHpPrevR = 0.f;
  float mSubPhase = 0.f;

  // Random generators
  uint32_t mRng1 = 111;
  uint32_t mRng2 = 222;
  uint32_t mRng3 = 333;

  // One-pole filter states for noise + parameter smoothing
  float mNoiseSmL = 0.f;
  float mNoiseSmR = 0.f;
  float mFastNoiseL = 0.f;
  float mFastNoiseR = 0.f;
  float mSmDust = 0.5f;
  float mSmDamping = 0.5f;
  float mDustSt = 0.f;
  float mDampingSt = 0.f;

  double mSampleRate = 44100.0;

  static constexpr double kTimeVals[6] = { 0.25, 0.5, 1.0, 2.0, 4.0, 8.0 };

  uint32_t XorShift32(uint32_t& state);
  float FastNoiseLP(uint32_t& rng, float& state, float alpha);
  float SlowNoiseLP(uint32_t& rng, float& state, float alpha);
  float OnePole(float in, float& st, float alpha);
#endif
};
