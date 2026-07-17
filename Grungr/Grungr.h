#pragma once

#include <atomic>
#include <vector>

#include "IPlug_include_in_plug_hdr.h"

#if IPLUG_DSP
#ifndef FAUSTFLOAT
#define FAUSTFLOAT double
#endif

#include "GrungrFaustCompat.h"

#include "DSP/GrungrFaustDSP.h"
#endif

const int kNumPresets = 6;

enum EParams
{
  kParamGrunge = 0,
  kParamButt,
  kParamFace,
  kParamLoud,
  kParamRawVoicing,
  kParamBypass,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class Grungr final : public Plugin
{
public:
  Grungr(const InstanceInfo& info);
  void OnRestoreState() override;

#if IPLUG_EDITOR
public:
  bool OnHostRequestingSupportedViewConfiguration(int width, int height) override;
  void OnHostSelectedViewConfiguration(int width, int height) override;
  void OnParentWindowResize(int width, int height) override;
  bool ConstrainEditorResize(int& width, int& height) const override;
#endif

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;

private:
  static constexpr int kMaxBlockFrames = 65536;
  void SyncParamToFaust(int paramIdx);

  GrungrFaustDSP mFaustDSP;
  MapUI mFaustUI;
  std::vector<sample> mSilentInput;
  std::vector<sample> mScratchOutput;

  // I/O meter peaks (audio thread writes, GUI thread reads via OnIdle).
  std::atomic<float> mInputPeakL {0.f};
  std::atomic<float> mInputPeakR {0.f};
  std::atomic<float> mOutputPeakL {0.f};
  std::atomic<float> mOutputPeakR {0.f};
#endif
};
