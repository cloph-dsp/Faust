#pragma once

#include <atomic>
#include <deque>
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

#if IPLUG_EDITOR
public:
  bool OnHostRequestingSupportedViewConfiguration(int width, int height) override;
  void OnHostSelectedViewConfiguration(int width, int height) override;
#endif

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;

private:
  void SyncParamToFaust(int paramIdx);

  GrungrFaustDSP mFaustDSP;
  MapUI mFaustUI;
  std::vector<sample> mSilentInput;
  std::vector<sample> mScratchOutput;

  // I/O meter peaks (audio thread writes, GUI thread reads via OnIdle).
  // Input peaks are time-shifted by PLUG_LATENCY samples via the delay
  // deques below so the input meter shows what is currently emerging at
  // the output meter (avoids visual misalignment under plugin latency).
  std::atomic<float> mInputPeakDelayedL {0.f};
  std::atomic<float> mInputPeakDelayedR {0.f};
  std::atomic<float> mOutputPeakL {0.f};
  std::atomic<float> mOutputPeakR {0.f};
  std::deque<float> mInputPeakHistoryL;
  std::deque<float> mInputPeakHistoryR;
  int mInputDelayBlocks = 0;
#endif
};
