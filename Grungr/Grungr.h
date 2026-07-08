#pragma once

#include <vector>

#include "IPlug_include_in_plug_hdr.h"

#if IPLUG_DSP
#ifndef FAUSTFLOAT
#define FAUSTFLOAT double
#endif

#include "faust/dsp/dsp.h"
#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"

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

private:
  void SyncParamToFaust(int paramIdx);

  GrungrFaustDSP mFaustDSP;
  MapUI mFaustUI;
  std::vector<sample> mSilentInput;
  std::vector<sample> mScratchOutput;
#endif
};
