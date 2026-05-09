#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "ISender.h"
#include <array>
#include <cmath>
#include <cstdint>

const int kNumPresets = 1;
static constexpr int kMaxVoices = 200;
static constexpr int kMaxChannels = 2;
static constexpr int kDisplaySlots = kMaxVoices * kMaxChannels + 2;

enum EParams
{
    // === MACRO UI PARAMETERS (Visible in simplified interface) ===
    // These map to the 12-knob + selector + toggle design
    
    // Primary 12 knobs
    kMix = 0,           // Dry/wet balance (0-100%)
    kVoices,            // Number of voices (1-200, log)
    kMinDelay,          // Minimum delay time
    kDelaySpread,       // Delay range between voices
    kFeedback,          // Regeneration (-100% to +100%)
    kScatter,           // Voice attenuation at edges (0-10)
    kStereo,            // Stereo rotation + width
    kSpeed,             // Modulation rate
    kRandom,            // Drift/random amount
    kTone,              // Tilt EQ style brightness
    kMod,               // Combined LFO + Env modulation
    
    // Selector (LFO Shape)
    kLFOShape,
    
    // Global SYNC toggle
    kSync,
    
    // === INTERNAL PROCESSING PARAMETERS ===
    // These drive the DSP but aren't directly exposed in macro UI
    
    // Legacy compatible indexes (for preset loading)
    kPostGain = 100,
    kDelayRange,        // Maps to kDelaySpread
    kLegacyDamper,      // Maps to kScatter
    kLowpass,           // Used for TONE calculation
    kRateRange,         // Used for SPEED calculation
    kRateUpdate,        // Used for RANDOM calculation
    
    // Stereo processing
    kStereoMode,
    kRotation,          // Used for kStereo calculation
    kWidth,             // Used for kStereo calculation
    
    // Rate/timing
    kMinRate,
    kUpdateUnit,
    kUpdateQuantize,
    kUpdateOnCollision,
    kManualBPM,
    
    // Filter processing
    kHighpass,
    kLPRes,
    kHPRes,
    
    // Modulation
    kEnvLP,
    kEnvHP,
    kEnvDelay,
    kLFORate,
    kLFOLP,
    kLFOHP,
    kLFODelay,
    
    // Dynamics
    kPregain,
    kFollowLevel,
    
    // Meta
    kLFOUnit,
    kLFOQuantize,
    kLFOMode,
    
    // Deprecated (kept for state compatibility)
    kSignalSyncSource,
    kDelaySyncNote,
    kLFOSyncNote,
    kMotionScale,
    
    kNumParams
};

enum ECtrlTags
{
  kCtrlTagDisplay = 0,
  kCtrlTagTabSwitch,
  kCtrlTagTempoModeHint,
  kCtrlTagManualBPMKnob,
  kCtrlTagSyncToggle,
  kCtrlTagLFOShape,
};

enum EStereoMode
{
  kStereoFree = 0,
  kStereoSlave,
  kStereoAntiSlave,
  kStereoHalf,
  kNumStereoModes
};

enum ETimeUnit
{
  kTimeMilliseconds = 0,
  kTimeSeconds,
  kTimeNoteSyncHost,
  kTimeNoteSyncManualBPM,
  kNumTimeUnits
};

enum EFollowLevel
{
  kFollowOff = 0,
  kFollowNormal,
  kFollowFastDecay,
  kFollowSlowDecay,
  kNumFollowLevels
};

enum EMotionScale
{
  kMotionScaleChromatic = 0,
  kMotionScaleMajor,
  kMotionScaleMinor,
  kMotionScaleDorian,
  kMotionScaleMixolydian,
  kMotionScalePentatonic,
  kMotionScaleWholeTone,
  kMotionScaleFifths,
  kNumMotionScales
};

enum ELFOShape
{
  kLFOSine = 0,
  kLFOTriangle,
  kLFOSquare,
  kLFORampUp,
  kLFORampDown,
  kLFOSH,           // Sample & Hold
  kLFOSmoothSH,     // Smooth Sample & Hold
  kLFOSawtooth,
  kLFOReverseSaw,
  kLFOBitcrush,
  kNumLFOShapes
};

enum ESyncState
{
  kSyncOff = 0,
  kSyncOn,
  kNumSyncStates
};

using namespace iplug;
using namespace igraphics;

class SoreThroat final : public Plugin
{
public:
  SoreThroat(const InstanceInfo& info);

#if IPLUG_EDITOR
  void OnParamChangeUI(int paramIdx, EParamSource source) override;
#endif

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnIdle() override;
  void OnReset() override;
#endif

private:
#if IPLUG_DSP
  struct VoiceState
  {
    float delayL = 0.f;
    float delayR = 0.f;
    float rateL = 0.f;
    float rateR = 0.f;
    float gainL = 0.f;
    float gainR = 0.f;
  };

  struct SVFState
  {
    double ic1eq = 0.;
    double ic2eq = 0.;
  };

  WDL_TypedBuf<float> mBuffer;
  uint32_t mBufferSize = 0;
  uint32_t mBufferMask = 0;
  uint32_t mWriteAddress = 0;

  std::array<VoiceState, kMaxVoices> mVoiceStates;
  std::array<SVFState, kMaxChannels> mLPState;
  std::array<SVFState, kMaxChannels> mHPState;
  std::array<float, kMaxChannels> mFeedbackTap = {0.f, 0.f};

  std::array<uint8_t, kDisplaySlots> mDots{};
  ISender<1, 3, std::array<uint8_t, kDisplaySlots> > mDisplaySender;

  // Smoothed state
  double mSmMix = 1.0;
  double mSmPregain = 1.0;
  double mSmPostgain = 1.0;
  double mSmFeedback = 0.0;
  double mSmRotationRad = 0.0;
  double mSmLowpassHz = 12000.0;
  double mSmHighpassHz = 40.0;
  double mSmLPQ = 0.707;
  double mSmHPQ = 0.707;
  double mSmEnvLP = 0.0;
  double mSmEnvHP = 0.0;
  double mSmEnvDelay = 0.0;
  double mSmLFOLP = 0.0;
  double mSmLFOHP = 0.0;
  double mSmLFODelay = 0.0;

  double mParamAlpha = 0.0;
  double mFilterAlpha = 0.0;

  // Dynamics / timing state
  double mInputEnv = 0.0;
  double mOutputEnv = 0.0;
  double mFollowGain = 1.0;
  double mEnvAttack = 0.0;
  double mEnvRelease = 0.0;
  double mOutputEnvRelease = 0.0;
  double mUpdateSamplesRemaining = 0.0;
  double mLFOPhase = 0.0;
  uint32_t mRngState = 0x86E39A3Bu;
  bool mUseFastDelayInterp = false;

  int mLastVoiceCount = 0;
  int mLastStereoMode = -1;

  void ResetDSPState();
  void ResizeDelayBuffer();
  void RandomizeVoice(int voiceIdx, int stereoMode, int nChans, float delayRangeSamples, float minRateMsPerSec, float rateRangeMsPerSec, float legacyDamper);
  void RandomizeRates(int voiceCount, int stereoMode, int nChans, float minRateMsPerSec, float rateRangeMsPerSec);
  void EnsureVoiceState(int voiceCount, int stereoMode, int nChans, float delayRangeSamples, float minRateMsPerSec, float rateRangeMsPerSec, float legacyDamper);

  float Random01();
  float RandomRateSamplesPerSample(float minRateMsPerSec, float rateRangeMsPerSec);

  double ResolveTempoBPM(int unit, double manualBPM) const;
  double TimeFromUnitMs(double normalized, int unit, bool quantize, double bpm) const;
  double NoteDivisionQN(double normalized, bool quantize) const;
  double LFOValue(int shape) const;

  float UpdateVoiceDelay(float& delay, float& rate, float delayRangeSamples, bool updateOnCollision, float minRateMsPerSec, float rateRangeMsPerSec);
  float ReadDelayHermite(int channel, float delaySamples) const;
  float ProcessSVF(SVFState& state, float input, double cutoffHz, double q, bool highPass, double sr);

  static uint32_t NextPow2(uint32_t v);
  static inline double SmoothTo(double current, double target, double alpha)
  {
    return current + (target - current) * alpha;
  }

  static inline float SoftClip(float x)
  {
    return static_cast<float>(std::tanh(static_cast<double>(x)));
  }
#endif
} WDL_FIXALIGN;
