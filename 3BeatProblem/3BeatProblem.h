#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "sequencer_engine.h"

#include <cstdio>

using namespace iplug;
using namespace igraphics;

class ThreeBeatProblem final : public Plugin {
public:
  explicit ThreeBeatProblem(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;

  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
#endif

private:
  threebeat::SequencerEngine engine_;

  // Cached parameter values for ProcessBlock (RT safe)
  int cachedVelocity_ = 100;
  int cachedVelRandom_ = 0;
  int cachedProbability_ = 100;
  int cachedNoteDuration_ = 50;

  // Rate mapping: parameter index 0-12 to float rate value
  static float RateIdxToFloat(int idx) {
    static const float kRateMap[] = {
      1.0f / 64.0f,   // 0: 1/64
      1.0f / 32.0f,   // 1: 1/32
      1.0f / 16.0f,   // 2: 1/16
      1.0f / 8.0f,    // 3: 1/8
      1.0f / 4.0f,    // 4: 1/4
      1.0f / 2.0f,    // 5: 1/2
      1.0f,           // 6: 1x
      2.0f,            // 7: 2x
      4.0f,            // 8: 4x
      8.0f,            // 9: 8x
      16.0f,           // 10: 16x
      32.0f,           // 11: 32x
      64.0f            // 12: 64x
    };
    if (idx < 0) return kRateMap[0];
    if (idx > 12) return kRateMap[12];
    return kRateMap[idx];
  }

  // Helper methods for parameter handling
  void HandleMelodySeqParam(int seqIdx, int localParam, double value);
  void HandleDrumSeqParam(int seqIdx, int localParam, double value);
  void SyncAllParamsFromEngine();

  // VST3 parameter enums
  enum EParams {
    // Global (0-7)
    kParamMode = 0,
    kParamBPM,
    kParamSync,
    kParamSwing,
    kParamNoteDuration,
    kParamVelocity,
    kParamVelRandom,
    kParamProbability,

    // Melody mode global (8-11)
    kParamScaleType,
    kParamScaleRoot,
    kParamOctaveLow,
    kParamOctaveHigh,

    // S1 melody params (12-31)
    kParamS1Rate = 12,
    kParamS1Steps,
    kParamS1Mood,
    kParamS1PatternBase,  // 16 pattern steps: base+0 to base+15

    // S2 melody params (32-51)
    kParamS2Rate = 32,
    kParamS2Steps,
    kParamS2Mood,
    kParamS2PatternBase,

    // S3 melody params (52-71)
    kParamS3Rate = 52,
    kParamS3Steps,
    kParamS3Mood,
    kParamS3PatternBase,

    // Drum mode S1 (72-91)
    kParamDrumS1Rate = 72,
    kParamDrumS1Steps,
    kParamDrumS1Note,
    kParamDrumS1Mute,
    kParamDrumS1Solo,
    kParamDrumS1PatternBase,  // 16 pattern steps

    // Drum mode S2 (94-115)
    kParamDrumS2Rate = 94,
    kParamDrumS2Steps,
    kParamDrumS2Note,
    kParamDrumS2Mute,
    kParamDrumS2Solo,
    kParamDrumS2PatternBase,

    // Drum mode S3 (116-137)
    kParamDrumS3Rate = 116,
    kParamDrumS3Steps,
    kParamDrumS3Note,
    kParamDrumS3Mute,
    kParamDrumS3Solo,
    kParamDrumS3PatternBase,

    kNumParams = 138
  };

  static constexpr int kNumPresets = 1;
};