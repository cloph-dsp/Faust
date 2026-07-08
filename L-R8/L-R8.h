#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <atomic>

#include "IPlug_include_in_plug_hdr.h"
#include "FaustCompat.h"
#include "DSP/L8FaustDSP.h"

const int kNumPresets = 1;
const int kStateVersion = 1;

using namespace iplug;
using namespace igraphics;

enum EParams {
  kParamBypass = 0,
  kParamSync,
  kParamRate,
  kParamBpm,
  kParamFreeRate,
  kParamCrossfade,
  kParamHighPass,
  kParamLowPass,
  kNumParams
};

class LR8 final : public iplug::Plugin {
public:
  explicit LR8(const iplug::InstanceInfo& info);

#if IPLUG_EDITOR
  void OnParentWindowResize(int width, int height) override;
#endif

  void OnParamChange(int paramIdx) override;
  void OnReset() override;
  void OnUIClose() override;
  void ProcessBlock(iplug::sample** inputs, iplug::sample** outputs, int nFrames) override;

  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;

private:
  void LayoutUI(iplug::igraphics::IGraphics* pGraphics);
  void SyncParamsToDSP();
  void SetFaustParam(const char* label, float value);

  std::unique_ptr<L8FaustDSP> mDSP;
  std::unordered_map<std::string, FAUSTFLOAT*> mFaustZones;

  static constexpr int kMaxBlockFrames = 16384;
  static constexpr int kVisBufferSize = 1024;

  std::vector<float> mInL;
  std::vector<float> mInR;
  std::vector<float> mOutL;
  std::vector<float> mOutR;

  FAUSTFLOAT* mInPtrs[2] = {nullptr, nullptr};
  FAUSTFLOAT* mOutPtrs[2] = {nullptr, nullptr};

  int mBufferSize = 0;

  // Visualizer ring buffers (stores recent sample data for display)
  std::vector<float> mVisBufferL;
  std::vector<float> mVisBufferR;
  int mVisWritePos = 0;

  // Track modulation state for haptic feedback

  // Smoothed bypass gain for click-free toggling
  float mBypassGain = 1.0f;
  float mTargetBypassGain = 1.0f;

  // Opaque cached pointer to the StereoVisualizerControl (set during LayoutUI)
  // Atomic for safe read from audio thread, reset in OnUIClose()
  std::atomic<void*> mVisControlPtr{nullptr};
  
  // Busy flag: audio thread sets this while using the scope, OnUIClose spins until clear
  std::atomic<int> mVisBusy{0};

  bool mTransportWasRunning = false;
};
