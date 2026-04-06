#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "fft.h"

#include <array>
#include <vector>

static constexpr int kNumPresets = 1;

enum EParams
{
  kAmount = 0,
  kTarget,
  kSmoothing,
  kFFTSize,
  kCharacter,
  kTransient,
  kNumParams
};

enum ETargetCurve
{
  kTargetWhite = 0,
  kTargetPink,
  kTargetRusset,
  kTargetBrown,
  kTargetEqualLoudness,
  kNumTargetCurves
};

enum EFFTSizeSelector
{
  kFFTSize256 = 0,
  kFFTSize512,
  kFFTSize1024,
  kFFTSize2048,
  kFFTSize4096,
  kFFTSize8192,
  kFFTSize16384,
  kNumFFTSizes
};

using namespace iplug;
using namespace igraphics;

class RussetNoise final : public Plugin
{
public:
  RussetNoise(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
#endif

private:
#if IPLUG_DSP
  // Maximum FFT size (16384) - we allocate for max but use selected size at runtime
  static constexpr int kMaxFFTSize = 16384;
  static constexpr int kMaxHopSize = kMaxFFTSize / 2;
  static constexpr int kMaxChannels = 2;
  static constexpr int kNumBands = 64;  // Increased bands for higher resolution
  static constexpr int kOutputFifoSize = kMaxFFTSize * 2;

  // Current runtime FFT size
  int mFFTSize = 2048;
  int mHopSize = 1024;

  void ResetDspState();
  void UpdateBandLayout(double sampleRate);
  void ProcessHop(int channelCount);
  void PushOutputSample(int channelIndex, float value);
  float PopOutputSample(int channelIndex);
  float EvaluateTargetDb(float frequencyHz, int targetMode) const;
  float EvaluateEqualLoudnessDb(float frequencyHz) const;
  float LinearToDb(float linearValue) const;
  float DbToLinear(float dbValue) const;
  float ClampCorrectionDb(float frequencyHz, float correctionDb, float aggression) const;
  void SmoothBandCorrections(const std::array<float, kNumBands>& input,
                             std::array<float, kNumBands>& output,
                             int radius) const;
  float ComputeHopRms(int channelIndex, const float* buffer, int length) const;
  void UpdateMakeupGain(float inputRms, float outputRms);

  // Use vectors for runtime-sized buffers
  std::vector<float> mWindow;
  std::vector<int> mPermutation;
  std::array<float, kNumBands + 1> mBandEdgesHz {};
  std::array<float, kNumBands> mBandCentersHz {};
  std::array<int, kNumBands> mBandStartBins {};
  std::array<int, kNumBands> mBandEndBins {};
  std::array<int, (kMaxFFTSize / 2) + 1> mBinLowerBand {};
  std::array<int, (kMaxFFTSize / 2) + 1> mBinUpperBand {};
  std::array<float, (kMaxFFTSize / 2) + 1> mBinBandBlend {};

  std::array<std::vector<float>, kMaxChannels> mAnalysisBuffer;
  std::array<std::vector<float>, kMaxChannels> mOverlapAddBuffer;
  std::array<std::vector<float>, kMaxChannels> mHopBuffer;
  std::array<std::vector<float>, kMaxChannels> mOutputFifo;
  std::array<std::vector<WDL_FFT_COMPLEX>, kMaxChannels> mFftBuffer;
  // Dry signal delay buffer - matches FFT latency for phase-coherent wet/dry mixing
  std::array<std::vector<float>, kMaxChannels> mDryDelayBuffer;
  std::array<int, kMaxChannels> mDryDelayIndex {};
  std::array<float, kNumBands> mAverageSpectrumDb {};

  std::array<int, kMaxChannels> mOutputReadIndex {};
  std::array<int, kMaxChannels> mOutputWriteIndex {};
  std::array<int, kMaxChannels> mOutputCount {};

  int mHopFill = 0;
  double mSampleRate = 44100.0;
  bool mHasSpectrum = false;

  // Auto makeup gain state - professional RMS-based gain compensation
  float mAutoMakeupGainDb = 0.f;          // Current smoothed makeup gain in dB
  float mInputRmsDb = -60.f;             // Measured input RMS per hop (dB)
  float mOutputRmsDb = -60.f;            // Measured output RMS per hop (dB)
  float mTruePeakLevel = 0.f;            // Running true peak for limiter
  static constexpr float kMakeupGainSmoothAlpha = 0.15f;  // Time constant for gain smoothing
  static constexpr float kTruePeakCeiling = -0.1f;        // True peak ceiling in dBFS
  static constexpr float kMaxMakeupGainDb = 24.f;         // Maximum makeup gain
  static constexpr float kMakeupGainToleranceDb = 0.1f;   // Target accuracy tolerance

  // Wet/dry mix state for smooth transitions
  float mCurrentWetMix = 1.f;            // Current wet/dry blend (smoothed)
  float mPreviousWetMix = 1.f;           // Previous frame's wet mix for interpolation
  static constexpr float kWetMixSmoothing = 0.05f; // Smooth wet/dry transitions

  // Dither state per channel - ensures bit-accurate output
  float mDitherState[kMaxChannels] = {};         // Last quantization error for noise shaping
#endif
};