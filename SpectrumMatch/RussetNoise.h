#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "fft.h"

#include <array>

static constexpr int kNumPresets = 1;

enum EParams
{
  kAmount = 0,
  kTarget,
  kSmoothing,
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

using namespace iplug;
using namespace igraphics;

class RussetNoise final : public Plugin
{
public:
  RussetNoise(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
#endif

private:
#if IPLUG_DSP
  static constexpr int kFFTSize = 2048;
  static constexpr int kHopSize = kFFTSize / 2;
  static constexpr int kMaxChannels = 2;
  static constexpr int kNumBands = 48;
  static constexpr int kOutputFifoSize = kFFTSize * 2;

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

  std::array<float, kFFTSize> mWindow {};
  std::array<int, kFFTSize> mPermutation {};
  std::array<float, kNumBands + 1> mBandEdgesHz {};
  std::array<float, kNumBands> mBandCentersHz {};
  std::array<int, kNumBands> mBandStartBins {};
  std::array<int, kNumBands> mBandEndBins {};
  std::array<int, (kFFTSize / 2) + 1> mBinLowerBand {};
  std::array<int, (kFFTSize / 2) + 1> mBinUpperBand {};
  std::array<float, (kFFTSize / 2) + 1> mBinBandBlend {};

  std::array<std::array<float, kFFTSize>, kMaxChannels> mAnalysisBuffer {};
  std::array<std::array<float, kFFTSize>, kMaxChannels> mOverlapAddBuffer {};
  std::array<std::array<float, kHopSize>, kMaxChannels> mHopBuffer {};
  std::array<std::array<float, kOutputFifoSize>, kMaxChannels> mOutputFifo {};
  std::array<std::array<WDL_FFT_COMPLEX, kFFTSize>, kMaxChannels> mFftBuffer {};
  std::array<float, kNumBands> mAverageSpectrumDb {};

  std::array<int, kMaxChannels> mOutputReadIndex {};
  std::array<int, kMaxChannels> mOutputWriteIndex {};
  std::array<int, kMaxChannels> mOutputCount {};

  int mHopFill = 0;
  double mSampleRate = 44100.0;
  bool mHasSpectrum = false;
#endif
};