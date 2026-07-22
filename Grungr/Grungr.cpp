#include "Grungr.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "GrungrIPlug2UI.h"

#include <algorithm>
#include <cmath>
#if ((_MSC_VER && (_M_IX86 || _M_X64)) || defined(__SSE__))
#define GRUNGR_HAS_SSE_DENORMALS 1
#include <xmmintrin.h>
#else
#define GRUNGR_HAS_SSE_DENORMALS 0
#endif

Grungr::Grungr(const InstanceInfo& info)
: iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGrunge)->InitDouble("Grunge", 0.70, 0.0, 1.0, 0.01);
  GetParam(kParamGrunge)->SetDisplayFunc([](double val, WDL_String& str) {
    int v = static_cast<int>(std::round(val * 100.0));
    if (v >= 10) {
      char buf[16];
      snprintf(buf, sizeof(buf), "%d", v);
      std::string s;
      for (char* p = buf; *p; ++p) {
        if (p != buf) s += ' ';
        s += *p;
      }
      s += " %";
      str.Set(s.c_str());
    } else {
      str.SetFormatted(32, "%d%%", v);
    }
  });
  GetParam(kParamButt)->InitDouble("Butt", 0.62, 0.0, 1.0, 0.01);
  GetParam(kParamButt)->SetDisplayFunc([](double val, WDL_String& str) {
    int v = static_cast<int>(std::round(val * 100.0));
    if (v >= 10) {
      char buf[16];
      snprintf(buf, sizeof(buf), "%d", v);
      std::string s;
      for (char* p = buf; *p; ++p) {
        if (p != buf) s += ' ';
        s += *p;
      }
      s += " %";
      str.Set(s.c_str());
    } else {
      str.SetFormatted(32, "%d%%", v);
    }
  });
  GetParam(kParamFace)->InitDouble("Face", 0.56, 0.0, 1.0, 0.01);
  GetParam(kParamFace)->SetDisplayFunc([](double val, WDL_String& str) {
    int v = static_cast<int>(std::round(val * 100.0));
    if (v >= 10) {
      char buf[16];
      snprintf(buf, sizeof(buf), "%d", v);
      std::string s;
      for (char* p = buf; *p; ++p) {
        if (p != buf) s += ' ';
        s += *p;
      }
      s += " %";
      str.Set(s.c_str());
    } else {
      str.SetFormatted(32, "%d%%", v);
    }
  });
  GetParam(kParamLoud)->InitDouble("Loud", 0.72, 0.0, 1.0, 0.01);
  GetParam(kParamLoud)->SetDisplayFunc([](double val, WDL_String& str) {
    if (val <= 0.01) str.Set("-inf dB");
    else str.SetFormatted(32, "%.1f dB", val * 66.0 - 60.0);
  });
  GetParam(kParamRawVoicing)->InitEnum("RAW Voicing", 0, {"RAW", "MOD", "BASS"});
  GetParam(kParamBypass)->InitBool("Bypass", false);
  
  // Factory presets: order matches EParams enum (Grunge, Butt, Face, Loud, RawVoicing, Bypass)
  MakePreset("Clean-ish", 0.30, 0.50, 0.40, 0.70, 0., 0.);
  MakePreset("Crunch", 0.60, 0.60, 0.50, 0.72, 1., 0.);
  MakePreset("Full Grunge", 0.95, 0.80, 0.80, 0.65, 1., 0.);
  MakePreset("Bass Boost", 0.50, 1.00, 0.30, 0.68, 0., 0.);
  MakePreset("Bright", 0.40, 0.30, 1.00, 0.70, 0., 0.);
  MakePreset("Bass Grunge", 0.75, 0.85, 0.50, 0.70, 2., 0.);

#if IPLUG_DSP
  mFaustDSP.buildUserInterface(&mFaustUI);
  // Hosts can legally submit larger buffers after instantiation. Reserve a
  // bounded working area here, never from the audio callback.
  mSilentInput.assign(kMaxBlockFrames, 0.0);
  mScratchOutput.assign(kMaxBlockFrames, 0.0);
#endif

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    if (pGraphics->NControls() == 0) {
      pGraphics->EnableMouseOver(true);
      pGraphics->EnableTooltips(true);
      pGraphics->EnableMultiTouch(true);
      pGraphics->SetKeyHandlerFunc([this](const IKeyPress& key, bool isUp) {
        return grungr::ui::HandleGlobalKey(GetUI(), key, isUp);
      });
    }

    grungr::ui::Params params;
    params.grunge = kParamGrunge;
    params.butt = kParamButt;
    params.face = kParamFace;
    params.loud = kParamLoud;
    params.bypass = kParamBypass;
    params.rawVoicing = kParamRawVoicing;

    grungr::ui::BuildOrRelayout(pGraphics, params);
  };
#endif

#if IPLUG_DSP
  OnReset();
#endif
}

#if IPLUG_EDITOR
// Scale-mode host resize, mirroring Freeze95 (which resolved the same Reaper
// crop). The UI is laid out at the fixed logical size PLUG_WIDTH x PLUG_HEIGHT
// and the whole vector scene is uniformly scaled to fit whatever window the
// host gives, so nothing is ever cropped. The previous Size-mode path
// (Resize(w, h, 1.f, layoutOnResize=true) via the OnHost*ViewConfiguration
// overrides) let Reaper open the editor at a size the plugin didn't dictate
// and clipped the fixed-size scene. SetScaleConstraints() in BuildOrRelayout()
// now bounds the scale, so ConstrainEditorResize() and the view-configuration
// overrides are unnecessary -- iPlug2's defaults honour the scale constraints.
void Grungr::OnParentWindowResize(int width, int height)
{
  IGraphics* pGraphics = GetUI();
  if (!pGraphics) {
    return;
  }

  // Divide out the screen (DPI) scale: GetTotalScale() already folds in
  // mScreenScale, so this keeps `scale` to just the host/user resize factor.
  const float screenScale = pGraphics->GetScreenScale();
  const float scaleX = static_cast<float>(width) / static_cast<float>(PLUG_WIDTH) / screenScale;
  const float scaleY = static_cast<float>(height) / static_cast<float>(PLUG_HEIGHT) / screenScale;
  // Bounds match the config min/max (360/480 = 0.75 .. 960/480 = 2.0) and the
  // SetScaleConstraints() call in grungr::ui::BuildOrRelayout().
  const float scale = std::clamp(std::min(scaleX, scaleY), 0.75f, 2.0f);

  // Rebuild at the new scale so SVGs re-rasterise crisply and every control
  // keeps its proportions (matches Freeze95). RemoveAllControls() makes
  // BuildOrRelayout() take its full-build path.
  grungr::ui::Params params;
  params.grunge = kParamGrunge;
  params.butt = kParamButt;
  params.face = kParamFace;
  params.loud = kParamLoud;
  params.bypass = kParamBypass;
  params.rawVoicing = kParamRawVoicing;

  pGraphics->RemoveAllControls();
  pGraphics->Resize(PLUG_WIDTH, PLUG_HEIGHT, scale, false);
  grungr::ui::BuildOrRelayout(pGraphics, params);
}
#endif

void Grungr::OnRestoreState()
{
  iplug::Plugin::OnRestoreState();
#if IPLUG_DSP
  // Some hosts restore a state without a subsequent reset callback.
  OnReset();
#endif
}

#if IPLUG_DSP
void Grungr::SyncParamToFaust(int paramIdx)
{
  switch (paramIdx)
  {
    case kParamGrunge:
      mFaustUI.setParamValue("Grunge", static_cast<FAUSTFLOAT>(GetParam(kParamGrunge)->Value()));
      break;
    case kParamButt:
      mFaustUI.setParamValue("Butt", static_cast<FAUSTFLOAT>(GetParam(kParamButt)->Value()));
      break;
    case kParamFace:
      mFaustUI.setParamValue("Face", static_cast<FAUSTFLOAT>(GetParam(kParamFace)->Value()));
      break;
    case kParamLoud:
      mFaustUI.setParamValue("Loud", static_cast<FAUSTFLOAT>(GetParam(kParamLoud)->Value()));
      break;
    case kParamRawVoicing:
      // Int() returns enum index 0=RAW, 1=MOD, 2=BASS — Faust expects these raw values.
      mFaustUI.setParamValue("RAW Voicing", static_cast<FAUSTFLOAT>(GetParam(kParamRawVoicing)->Int()));
      break;
    case kParamBypass:
      mFaustUI.setParamValue("Bypass", GetParam(kParamBypass)->Bool() ? FAUSTFLOAT(1.0) : FAUSTFLOAT(0.0));
      break;
    default:
      break;
  }
}

void Grungr::OnReset()
{
  mFaustDSP.init(std::max(1, static_cast<int>(GetSampleRate())));
  mInputPeakL.store(0.f, std::memory_order_relaxed);
  mInputPeakR.store(0.f, std::memory_order_relaxed);
  mOutputPeakL.store(0.f, std::memory_order_relaxed);
  mOutputPeakR.store(0.f, std::memory_order_relaxed);

  for (int paramIdx = 0; paramIdx < kNumParams; ++paramIdx)
  {
    SyncParamToFaust(paramIdx);
  }

  // ADAA warm-up: run a few samples through the DSP to initialise filter
  // states and prevent NaN from the divided-difference singularity (delta=0).
  {
    FAUSTFLOAT warmIn[2][8] = {};
    FAUSTFLOAT warmOut[2][8] = {};
    for (int i = 0; i < 8; ++i) {
      const FAUSTFLOAT v = static_cast<FAUSTFLOAT>(1e-6 * (i + 1));
      warmIn[0][i] = v;
      warmIn[1][i] = v;
    }
    FAUSTFLOAT* warmInPtr[2] = {warmIn[0], warmIn[1]};
    FAUSTFLOAT* warmOutPtr[2] = {warmOut[0], warmOut[1]};
    mFaustDSP.compute(8, warmInPtr, warmOutPtr);
  }

}

void Grungr::OnParamChange(int paramIdx)
{
  SyncParamToFaust(paramIdx);
}

void Grungr::OnIdle()
{
  if (auto* pGraphics = GetUI()) {
    grungr::ui::UpdateMeterLevels(pGraphics,
                                  mInputPeakL.load(std::memory_order_relaxed),
                                  mInputPeakR.load(std::memory_order_relaxed),
                                  mOutputPeakL.load(std::memory_order_relaxed),
                                  mOutputPeakR.load(std::memory_order_relaxed));
  }
}

void Grungr::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (nFrames <= 0) {
    return;
  }

#if GRUNGR_HAS_SSE_DENORMALS
  _mm_setcsr(_mm_getcsr() | 0x8040);
#endif

  const int nIn = NInChansConnected();
  const int nOut = NOutChansConnected();

  if (nOut <= 0 || outputs == nullptr || outputs[0] == nullptr) {
    return;
  }

  float inPeakL = 0.f, inPeakR = 0.f;
  float outPeakL = 0.f, outPeakR = 0.f;

  for (int offset = 0; offset < nFrames; offset += kMaxBlockFrames) {
    const int frames = std::min(kMaxBlockFrames, nFrames - offset);
    std::fill_n(mSilentInput.data(), frames, 0.0);

    sample* in0 = (nIn > 0 && inputs && inputs[0]) ? inputs[0] + offset : mSilentInput.data();
    sample* in1 = (nIn > 1 && inputs && inputs[1]) ? inputs[1] + offset : in0;
    sample* out0 = outputs[0] + offset;
    sample* out1 = (nOut > 1 && outputs[1]) ? outputs[1] + offset : mScratchOutput.data();
    FAUSTFLOAT* faustInputs[2] = {in0, in1};
    FAUSTFLOAT* faustOutputs[2] = {out0, out1};

    mFaustDSP.compute(frames, faustInputs, faustOutputs);

    for (int s = 0; s < frames; ++s) {
      if (!std::isfinite(out0[s])) out0[s] = 0.0;
      if (nOut > 1 && !std::isfinite(out1[s])) out1[s] = 0.0;

      const float inL = std::fabs(static_cast<float>(in0[s]));
      const float inR = std::fabs(static_cast<float>(in1[s]));
      const float oL = std::fabs(static_cast<float>(out0[s]));
      const float oR = (nOut > 1) ? std::fabs(static_cast<float>(out1[s])) : oL;
      inPeakL = std::max(inPeakL, inL);
      inPeakR = std::max(inPeakR, inR);
      outPeakL = std::max(outPeakL, oL);
      outPeakR = std::max(outPeakR, oR);
    }

    for (int c = 2; c < nOut; ++c) {
      if (outputs[c]) {
        std::copy_n(out0, frames, outputs[c] + offset);
      }
    }
  }

  mInputPeakL.store(inPeakL, std::memory_order_relaxed);
  mInputPeakR.store(inPeakR, std::memory_order_relaxed);
  mOutputPeakL.store(outPeakL, std::memory_order_relaxed);
  mOutputPeakR.store(outPeakR, std::memory_order_relaxed);
}
#endif
