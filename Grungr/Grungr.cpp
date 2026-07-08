#include "Grungr.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "GrungrIPlug2UI.h"

#include <algorithm>
#include <cmath>
#include <xmmintrin.h>

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
bool Grungr::OnHostRequestingSupportedViewConfiguration(int width, int height)
{
  return ConstrainEditorResize(width, height);
}

void Grungr::OnHostSelectedViewConfiguration(int width, int height)
{
  if (GetUI()) {
    GetUI()->Resize(width, height, 1.f, true);
  }
}
#endif

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

  const int preallocFrames = std::max(16384, GetBlockSize());
  mSilentInput.assign(preallocFrames, 0.0);
  mScratchOutput.assign(preallocFrames, 0.0);

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

void Grungr::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (nFrames <= 0) {
    return;
  }

#ifdef _WIN32
  _mm_setcsr(_mm_getcsr() | 0x8040);
#endif

  const int nIn = NInChansConnected();
  const int nOut = NOutChansConnected();

  if (nOut <= 0 || outputs == nullptr) {
    return;
  }

  if (static_cast<int>(mSilentInput.size()) < nFrames) {
    nFrames = static_cast<int>(mSilentInput.size());
  }

  std::fill(mSilentInput.begin(), mSilentInput.begin() + nFrames, 0.0);

  sample* in0 = (nIn > 0 && inputs && inputs[0]) ? inputs[0] : mSilentInput.data();
  sample* in1 = (nIn > 1 && inputs && inputs[1]) ? inputs[1] : in0;

  sample* out0 = outputs[0];
  sample* out1 = (nOut > 1 && outputs[1]) ? outputs[1] : mScratchOutput.data();

  FAUSTFLOAT* faustInputs[2] = {in0, in1};
  FAUSTFLOAT* faustOutputs[2] = {out0, out1};

  mFaustDSP.compute(nFrames, faustInputs, faustOutputs);

  // ponytail: ADAA warm-up in OnReset() prevents NaN from zero-state
  // δ=0 singularity. This clamp remains as a safety net only.
  for (int s = 0; s < nFrames; ++s) {
    if (!std::isfinite(out0[s])) out0[s] = 0.0;
    if (nOut > 1 && !std::isfinite(out1[s])) out1[s] = 0.0;
  }

  if (nOut > 1) {
    for (int c = 2; c < nOut; ++c) {
      for (int s = 0; s < nFrames; ++s) {
        outputs[c][s] = out0[s];
      }
    }
  }
}
#endif
