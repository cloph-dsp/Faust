#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "fft.h"

#include <array>
#include <atomic>
#include <cstdint>
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
  kBnMix,         // 0=dry, 1=wet (default 1, full wet)
  kBnStereoMode,  // 0=Linked, 1=Mid only, 2=Side only, 3=Independent
  kBnAutoGain,    // 0=off, 1=on (default on)
  kBnVisualMode,  // 0=All 3 curves, 1=After only, 2=Delta fill, 3=Before+Target overlay
  kBnReset,       // momentary button (0=idle, 1=triggered)
  kBnABCompare,   // momentary button (0=idle, 1=capture, 2=compare)
  kBypass,
  kNumParams
};

// Visual modes (used as enum display for kVisualMode)
enum EVisualMode
{
  kVisualAllCurves = 0,
  kVisualAfterOnly,
  kVisualDeltaFill,
  kVisualBeforeTarget,
  kNumVisualModes
};

enum EStereoMode
{
  kStereoLinked = 0,
  kStereoMidOnly,
  kStereoSideOnly,
  kStereoIndependent,
  kNumStereoModes
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

class BronzeNoise final : public iplug::Plugin
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
  bool ConstrainEditorResize(int& width, int& height) const override;
  void OnParentWindowResize(int width, int height) override;
  void OnActivate(bool active) override;
  void OnIdle() override;
  // Crash guard: when the editor closes, iPlug2 calls IGraphics::~IGraphics ->
  // RemoveAllControls() which deletes every control. We null our cached
  // member pointers BEFORE OnIdle can dereference a freed IControl*.
  void OnUIClose() override;

  // I/O meter control pointers (UI thread, OnIdle polls atomics and updates them).
  IOMeterControl* mInputMeter = nullptr;
  IOMeterControl* mOutputMeter = nullptr;
  // Latency readout label (updated dynamically in OnReset when FFT size changes).
  // NB: ITextControl is not available in headers (IGraphics_include_in_plug_src.h is .cpp-only), so store as base IControl*.
  IControl* mLatencyLabel = nullptr;
  // Spectrum visualizer (cached pointer so the audio->UI publish path skips the
  // dynamic_cast scan). Nulled in OnParentWindowResize before RemoveAllControls().
  IControl* mVisControl = nullptr;
  int mLastLatencySamplesUI = -1;
  double mLastLatencySampleRateUI = 0.0;
  // Last shown status-line telemetry (quantized to 0.1 dB so the label only
  // redraws when the displayed text would actually change).
  int mLastMakeupTenthsUI = INT32_MIN;
  int mLastLimiterTenthsUI = INT32_MIN;
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
  int mLatencySamples = 4095;

  // State-chunk header: magic + version guard against silent parameter
  // misassignment when the layout changes between releases. Legacy (v1.0.0)
  // chunks have no header and are detected by the missing magic.
  static constexpr int kStateMagic = 0x325A4E42;  // 'BNZ2' little-endian
  static constexpr int kStateVersion = 2;

  void ApplyFFTSizeSelector(int selector);
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
  std::array<std::array<float, kNumBands>, kMaxChannels> mChannelAverageSpectrumDb {};
  // Hop-to-hop smoothing of the APPLIED correction curve. The analysis average
  // is already smoothed, but up to 58% of the live frame leaks into each
  // correction (liveSpectrumBlend), which produces audible frame-rate gain
  // jitter ("musical noise") on small FFT sizes. One-pole per band, time
  // constant normalized to real time so behaviour is FFT-size independent.
  std::array<std::array<float, kNumBands>, kMaxChannels> mPrevCorrectionDb {};
  bool mHasPrevCorrection = false;

  std::array<int, kMaxChannels> mOutputReadIndex {};
  std::array<int, kMaxChannels> mOutputWriteIndex {};
  std::array<int, kMaxChannels> mOutputCount {};

  int mHopFill = 0;
  int mVisUpdateCounter = 0;
  double mSampleRate = 44100.0;
  bool mHasSpectrum = false;
  std::array<bool, kMaxChannels> mChannelHasSpectrum {};
  float mPreviousHopInputDb = -120.f;
  float mAnalyzerMagnitudeOffsetDb = 0.f;

  // Auto makeup gain state - professional RMS-based gain compensation
  float mAutoMakeupGainDb = 0.f;          // Current smoothed makeup gain in dB
  float mLimiterEnvelope = 0.f;          // Linked output safety-limiter envelope
  static constexpr float kOutputCeilingDb = -0.3f;        // Sample-peak safety ceiling in dBFS
  static constexpr float kMaxMakeupGainDb = 24.f;         // Maximum makeup gain

  // Wet/dry mix state for smooth transitions
  float mCurrentWetMix = 1.f;            // Current latency-aligned wet/dry blend

  // A/B compare snapshot (capture button → toggle)
  static constexpr int kNumABParams = kBnAutoGain + 1;
  std::array<double, kNumABParams> mABSnapshotA {};
  std::array<double, kNumABParams> mABSnapshotB {};
  bool mABHasSnapshotA = false;
  bool mABHasSnapshotB = false;
  int mABPreviousState = 0;

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

  // Audio -> UI spectrum publication. Atomics keep the audio thread completely
  // detached from IControl/IGraphics lifetimes.
  std::array<std::atomic<float>, kNumBands> mVisBefore {};
  std::array<std::atomic<float>, kNumBands> mVisAfter {};
  std::array<std::atomic<float>, kNumBands> mVisReference {};
  std::array<std::atomic<float>, kNumBands> mVisFrequencies {};
  std::atomic<int> mVisBandsReady {0};
  std::atomic<uint32_t> mVisGeneration {0};
  uint32_t mLastVisGeneration = 0;
  std::atomic<int> mLatencySamplesForUI {PLUG_LATENCY};
  // Thread-safe FFT-size switching: OnParamChange (UI thread) publishes the
  // selector here; ProcessBlock (audio thread) consumes it and rebuilds the
  // window/permutation/band tables on its own thread. This removes the data
  // race the previous design had (OnParamChange -> OnReset mutating mWindow /
  // mPermutation / mFFTSize while ProcessHop was reading them).
  std::atomic<int> mPendingFFTSelector {-1};
  // Audio -> UI status telemetry (makeup gain + safety-limiter gain reduction).
  std::atomic<float> mMakeupGainDbForUI {0.f};
  std::atomic<float> mLimiterGrDbForUI {0.f};
#endif
};
