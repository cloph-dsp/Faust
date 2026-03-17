#include "SpectralBlurTest.h"

#include <algorithm>

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

namespace {

constexpr int kDefaultQualityIndex = 1;

IVStyle MakeTestStyle()
{
  return DEFAULT_STYLE
    .WithLabelText(IText(12.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle))
    .WithValueText(IText(12.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle));
}

}  // namespace

SpectralBlurTest::SpectralBlurTest(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kParamMix)->InitDouble("Mix", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamSmearTime)->InitDouble("Smear Time", 55.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamTone)->InitDouble("Tone", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kParamMotion)->InitDouble("Motion", 30.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamTransientPreserve)->InitDouble("Transient Preserve", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamOutput)->InitDouble("Output", 0.0, -24.0, 24.0, 0.1, "dB");
  GetParam(kParamQuality)->InitEnum(
    "Quality", kDefaultQualityIndex, 3, "", IParam::kFlagsNone, "",
    "Eco", "Normal", "High");
  GetParam(kParamBandFocus)->InitDouble("Band Focus", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamStereoWidth)->InitDouble("Stereo Width", 100.0, 0.0, 200.0, 0.1, "%");

  SyncParamsToProcessor();

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) { LayoutUI(pGraphics); };
#endif
}

#if IPLUG_EDITOR
void SpectralBlurTest::LayoutUI(IGraphics* pGraphics)
{
  if (pGraphics->NControls()) {
    return;
  }

  pGraphics->EnableMouseOver(true);
  pGraphics->EnableTooltips(true);
  pGraphics->AttachPanelBackground(COLOR_GRAY);
  pGraphics->AttachTextEntryControl();
#ifndef OS_IOS
  pGraphics->AttachPopupMenuControl(DEFAULT_LABEL_TEXT);
#endif
  pGraphics->AttachBubbleControl();
  pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

  const IVStyle style = MakeTestStyle();
  const IRECT bounds = pGraphics->GetBounds();
  const IRECT padded = bounds.GetPadded(-16.f);
  const IRECT titleArea = padded.GetFromTop(54.f);
  const IRECT subtitleArea = padded.GetFromTop(82.f).GetReducedFromTop(42.f);
  const IRECT controlsArea = padded.GetReducedFromTop(96.f);

  pGraphics->AttachControl(new ITextControl(
    titleArea,
    "Spectral Blur Test",
    IText(24.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle)));

  pGraphics->AttachControl(new ITextControl(
    subtitleArea,
    "Modernized control model: musical macros on top, technical controls folded in",
    IText(14.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle)));

  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(0, 3, 3).GetPadded(-8.f), kParamMix, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(1, 3, 3).GetPadded(-8.f), kParamSmearTime, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(2, 3, 3).GetPadded(-8.f), kParamTone, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(3, 3, 3).GetPadded(-8.f), kParamMotion, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(4, 3, 3).GetPadded(-8.f), kParamTransientPreserve, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(5, 3, 3).GetPadded(-8.f), kParamOutput, "", style, true));

  auto* quality = new IVKnobControl(controlsArea.GetGridCell(6, 3, 3).GetPadded(-8.f), kParamQuality, "", style, true);
  quality->DisablePrompt(false);
  pGraphics->AttachControl(quality);

  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(7, 3, 3).GetPadded(-8.f), kParamBandFocus, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(8, 3, 3).GetPadded(-8.f), kParamStereoWidth, "", style, true));
}
#endif

void SpectralBlurTest::OnParamChange(int paramIdx)
{
  (void) paramIdx;
}

void SpectralBlurTest::OnReset()
{
  const int channelCount = std::max(1, std::min(2, MaxNChannels(ERoute::kOutput)));
  const double sampleRate = (GetSampleRate() > 0.0) ? GetSampleRate() : 44100.0;
  mProcessor.prepare(sampleRate, std::max(1, GetBlockSize()), channelCount);
  SyncParamsToProcessor();
}

void SpectralBlurTest::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // Sync once per block on the processing thread to avoid cross-thread DSP mutation.
  SyncParamsToProcessor();

  const int nIn = NInChansConnected();
  const int nOut = NOutChansConnected();
  const int processChannels = std::max(1, std::min(2, std::max(nIn, nOut)));

  if (nFrames <= 0) {
    return;
  }

  for (int channel = 0; channel < 2; ++channel) {
    if (static_cast<int>(mInputBuffers[channel].size()) < nFrames) {
      mInputBuffers[channel].resize(nFrames);
      mOutputBuffers[channel].resize(nFrames);
    }
  }

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex) {
    const bool hasInput0 = (inputs != nullptr && nIn > 0 && inputs[0] != nullptr);
    const bool hasInput1 = (inputs != nullptr && nIn > 1 && inputs[1] != nullptr);
    const float in0 = hasInput0 ? static_cast<float>(inputs[0][sampleIndex]) : 0.0f;
    const float in1 = hasInput1 ? static_cast<float>(inputs[1][sampleIndex]) : in0;
    mInputBuffers[0][sampleIndex] = in0;
    mInputBuffers[1][sampleIndex] = in1;
  }

  for (int channel = 0; channel < 2; ++channel) {
    mInputPtrs[channel] = mInputBuffers[channel].data();
    mOutputPtrs[channel] = mOutputBuffers[channel].data();
  }

  mProcessor.processBlock(mInputPtrs.data(), mOutputPtrs.data(), processChannels, nFrames);

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex) {
    if (outputs != nullptr && nOut > 0 && outputs[0] != nullptr) {
      outputs[0][sampleIndex] = static_cast<sample>(mOutputBuffers[0][sampleIndex]);
    }

    if (outputs != nullptr && nOut > 1 && outputs[1] != nullptr) {
      outputs[1][sampleIndex] = static_cast<sample>(mOutputBuffers[1][sampleIndex]);
    }

    for (int channel = 2; channel < nOut; ++channel) {
      if (outputs != nullptr && outputs[channel] != nullptr) {
        outputs[channel][sampleIndex] = 0.0;
      }
    }
  }
}

void SpectralBlurTest::SyncParamsToProcessor()
{
  spectralblur::Parameters parameters;
  parameters.mixPercent = static_cast<float>(GetParam(kParamMix)->Value());
  parameters.smearTimePercent = static_cast<float>(GetParam(kParamSmearTime)->Value());
  parameters.tonePercent = static_cast<float>(GetParam(kParamTone)->Value());
  parameters.motionPercent = static_cast<float>(GetParam(kParamMotion)->Value());
  parameters.transientPreservePercent = static_cast<float>(GetParam(kParamTransientPreserve)->Value());
  parameters.outputGainDb = static_cast<float>(GetParam(kParamOutput)->Value());
  parameters.qualityMode = GetParam(kParamQuality)->Int();
  parameters.bandFocusPercent = static_cast<float>(GetParam(kParamBandFocus)->Value());
  parameters.stereoWidthPercent = static_cast<float>(GetParam(kParamStereoWidth)->Value());
  mProcessor.setParameters(parameters);
}
