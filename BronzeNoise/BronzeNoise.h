#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "fft.h"

#include <array>
#include <atomic>
#include <vector>

static constexpr int kNumPresets = 8;

enum EParams
{
  kAmount = 0,
  kTarget,
  kSmoothing,
  kQ,
  kFFTSize,
  kCharacter,
  kTransient,
  kBypass,
  kNumParams
};

enum ETargetCurve
{
  kTargetLoud = 0,
  kTargetViolet,
  kTargetWhite,
  kTargetRed,
  kTargetOrange,
  kTargetPink,
  kTargetBronze,
  kTargetBrown,
  kTargetOlive,
  kTargetBlue,
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

// Forward declaration — defined in BronzeNoise.cpp
class IOMeterControl;

using namespace iplug;
using namespace igraphics;

class BronzeNoise final : public Plugin
{
public:
  BronzeNoise(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
#endif

#if IPLUG_EDITOR
  void LayoutUI(IGraphics* pGraphics);
  void OnParentWindowResize(int width, int height) override;
  void OnActivate(bool active) override;
  void OnIdle() override;

  // I/O meter control pointers (UI thread, OnIdle polls atomics and updates them).
  IOMeterControl* mInputMeter = nullptr;
  IOMeterControl* mOutputMeter = nullptr;
  // Latency readout label (updated dynamically in OnReset when FFT size changes).
  // NB: ITextControl is not available in headers (IGraphics_include_in_plug_src.h is .cpp-only), so store as base IControl*.
  IControl* mLatencyLabel = nullptr;
  // Spectrum visualizer (cached pointer so the audio->UI publish path skips the
  // dynamic_cast scan). Nulled in OnParentWindowResize before RemoveAllControls().
  IControl* mVisControl = nullptr;
#endif

private:
#if IPLUG_DSP
  // Maximum FFT size (16384) - we allocate for max but use selected size at runtime
  static constexpr int kMaxFFTSize = 16384;
  static constexpr int kMaxHopSize = kMaxFFTSize / 2;
  static constexpr int kMaxChannels = 2;
  static constexpr int kNumBands = 64;  // Increased bands for higher resolution
  static constexpr int kOutputFifoSize = kMaxFFTSize * 2;

  // Current runtime FFT size - 4096 for better HF resolution (~10.8 Hz/bin vs ~21.5 Hz/bin at 2048)
  int mFFTSize = 4096;
  int mHopSize = 2048;

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
  int mVisUpdateCounter = 0;
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

  // Bypass state
  bool mBypassed = false;
  int mPrevFFTSize = 4096;                       // Track FFT size across state loads

  // VST3 GUI parameter sync (Freeze95 pattern: setComponentState is a no-op in non-distributed VST3)
  bool mSendUpdate = false;

  // Smoothed parameter values (one-pole filtering for zipper-free automation)
  float mSmoothedAmount = 1.f;
  float mSmoothedSmoothing = 0.55f;
  float mSmoothedQ = 0.5f;
  float mSmoothedCharacter = 0.f;
  float mSmoothedTransient = 0.5f;

  // I/O peak tracking (atomic so audio thread can write, UI thread can read).
  std::atomic<float> mInputPeakL {0.f};
  std::atomic<float> mInputPeakR {0.f};
  std::atomic<float> mOutputPeakL {0.f};
  std::atomic<float> mOutputPeakR {0.f};
#endif
};