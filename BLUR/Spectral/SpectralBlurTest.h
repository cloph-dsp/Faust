#pragma once

#include <array>
#include <vector>

#include "IPlug_include_in_plug_hdr.h"
#include "spectralblur.h"

const int kNumPresets = 0;

using namespace iplug;
using namespace iplug::igraphics;

enum EParams {
  kParamMix = 0,
  kParamSmearTime,
  kParamTone,
  kParamMotion,
  kParamTransientPreserve,
  kParamOutput,
  kParamQuality,
  kParamBandFocus,
  kParamStereoWidth,
  kNumParams
};

class SpectralBlurTest final : public Plugin {
public:
  explicit SpectralBlurTest(const InstanceInfo& info);

  void OnParamChange(int paramIdx) override;
  void OnReset() override;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;

private:
#if IPLUG_EDITOR
  void LayoutUI(IGraphics* pGraphics);
#endif

  void SyncParamsToProcessor();

  spectralblur::Processor mProcessor;
  std::array<std::vector<float>, 2> mInputBuffers;
  std::array<std::vector<float>, 2> mOutputBuffers;
  std::array<const float*, 2> mInputPtrs {{nullptr, nullptr}};
  std::array<float*, 2> mOutputPtrs {{nullptr, nullptr}};
};