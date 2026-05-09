#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IPlug_include_in_plug_hdr.h"
#include "FaustCompat.h"
#include "Freeze95DSP.h"

const int kNumPresets = 0;

using namespace iplug;
using namespace igraphics;

enum EParams {
  kParamPower = 0,
  kParamChaos,
  kParamLoFi,
  kParamBpm,
  kParamSync,
  kNumParams
};

class Freeze95 final : public iplug::Plugin {
public:
  explicit Freeze95(const iplug::InstanceInfo& info);

#if IPLUG_EDITOR
  void OnParentWindowResize(int width, int height) override;
#endif

  void OnParamChange(int paramIdx) override;
  void OnReset() override;
  void ProcessBlock(iplug::sample** inputs, iplug::sample** outputs, int nFrames) override;

  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;

private:
  void LayoutUI(iplug::igraphics::IGraphics* pGraphics);
  void SyncParamsToDSP();
  void SetFaustParam(const char* label, float value);

  std::unique_ptr<Freeze95DSP> mDSP;
  std::unordered_map<std::string, FAUSTFLOAT*> mFaustZones;

  std::vector<float> mInL;
  std::vector<float> mInR;
  std::vector<float> mOutL;
  std::vector<float> mOutR;

  FAUSTFLOAT* mInPtrs[2] = {nullptr, nullptr};
  FAUSTFLOAT* mOutPtrs[2] = {nullptr, nullptr};

  bool mHasTransportState = false;
  bool mLastTransportRunning = false;
};
