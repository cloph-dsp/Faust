#include "SpectralBlurTest.h"

#include <algorithm>

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

namespace {

constexpr int kDefaultFftSizeIndex = 2;

IVStyle MakeTestStyle()
{
  return DEFAULT_STYLE
    .WithLabelText(IText(12.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle))
    .WithValueText(IText(12.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle));
}

}  // namespace

SpectralBlurTest::SpectralBlurTest(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kParamBlurAmount)->InitDouble("Blur amount", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamVarianceType)->InitEnum("Variance type", 0, {"None", "Random", "LFO", "Link amplitude", "Link inverted amp"});
  GetParam(kParamBlurVariance)->InitDouble("Blur variance", 0.5, 0.0, 100.0, 0.1, "%");
  GetParam(kParamLFORate)->InitDouble("LFO rate", 1.0, 0.0, 50.0, 0.01, "Hz");
  GetParam(kParamLoBinCutoff)->InitDouble("Lo bin cutoff", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamHiBinCutoff)->InitDouble("Hi bin cutoff", 100.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamRandomizePhases)->InitBool("Randomize phases", true, "", IParam::kFlagsNone, "", "Off", "On");
  GetParam(kParamFFTSize)->InitEnum("FFT Size", kDefaultFftSizeIndex, {"512", "1024", "2048", "4096", "8192", "16384", "32768", "65536"});
  GetParam(kParamOutputGain)->InitDouble("Gain", 0.0, -40.0, 40.0, 0.1, "dB");

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
    "Recovered AU control surface, modernized spectral voicing under the hood",
    IText(14.f, COLOR_BLACK, "Roboto-Regular", EAlign::Center, EVAlign::Middle)));

  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(0, 3, 3).GetPadded(-8.f), kParamBlurAmount, "", style, true));

  auto* varianceType = new IVKnobControl(controlsArea.GetGridCell(1, 3, 3).GetPadded(-8.f), kParamVarianceType, "", style, true);
  varianceType->DisablePrompt(false);
  pGraphics->AttachControl(varianceType);

  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(2, 3, 3).GetPadded(-8.f), kParamBlurVariance, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(3, 3, 3).GetPadded(-8.f), kParamLFORate, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(4, 3, 3).GetPadded(-8.f), kParamLoBinCutoff, "", style, true));
  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(5, 3, 3).GetPadded(-8.f), kParamHiBinCutoff, "", style, true));
  pGraphics->AttachControl(new IVToggleControl(controlsArea.GetGridCell(6, 3, 3).GetPadded(-8.f), kParamRandomizePhases, "", style, "Off", "On"));

  auto* fftSize = new IVKnobControl(controlsArea.GetGridCell(7, 3, 3).GetPadded(-8.f), kParamFFTSize, "", style, true);
  fftSize->DisablePrompt(false);
  pGraphics->AttachControl(fftSize);

  pGraphics->AttachControl(new IVKnobControl(controlsArea.GetGridCell(8, 3, 3).GetPadded(-8.f), kParamOutputGain, "", style, true));
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
  parameters.blurAmountPercent = static_cast<float>(GetParam(kParamBlurAmount)->Value());
  parameters.varianceType = GetParam(kParamVarianceType)->Int();
  parameters.blurVariancePercent = static_cast<float>(GetParam(kParamBlurVariance)->Value());
  parameters.lfoRateHz = static_cast<float>(GetParam(kParamLFORate)->Value());
  parameters.loBinCutoffPercent = static_cast<float>(GetParam(kParamLoBinCutoff)->Value());
  parameters.hiBinCutoffPercent = static_cast<float>(GetParam(kParamHiBinCutoff)->Value());
  parameters.randomizePhases = GetParam(kParamRandomizePhases)->Bool();
  parameters.fftSizeIndex = GetParam(kParamFFTSize)->Int();
  parameters.outputGainDb = static_cast<float>(GetParam(kParamOutputGain)->Value());
  mProcessor.setParameters(parameters);
}
