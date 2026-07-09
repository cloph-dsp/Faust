// =============================================================================
// Tuner.cpp - iPlug2 plugin + YIN/MPM/Neville detector + Amorph-faithful UI
// =============================================================================

#include "Tuner.h"
#include "TunerDSP.h"
#include "TunerAnalysis.h"   // host-free detector split out (round-11 refactor)
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <cstdio>
#include <cstring>


// =============================================================================
// TunerDSPWrapper + ZoneCaptureUI (inlined into the single IPlugEffect TU)
// =============================================================================

namespace {

class ZoneCaptureUI final : public UI {
public:
  explicit ZoneCaptureUI(std::unordered_map<std::string, FAUSTFLOAT*>& zones)
    : mZones(zones) {}

  void addButton(const char* label, FAUSTFLOAT* zone) override { mZones[label] = zone; }
  void addCheckButton(const char* label, FAUSTFLOAT* zone) override { mZones[label] = zone; }
  void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }

private:
  std::unordered_map<std::string, FAUSTFLOAT*>& mZones;
};

} // anonymous namespace

TunerDSPWrapper::TunerDSPWrapper()
  : mDSP(std::make_unique<TunerDSP>()) {
  mDSP->init(44100);
  ZoneCaptureUI capture(mZones);
  mDSP->buildUserInterface(&capture);
}

void TunerDSPWrapper::Init(int sampleRate) {
  mDSP->init(sampleRate);
  if (mZones.empty()) {
    ZoneCaptureUI capture(mZones);
    mDSP->buildUserInterface(&capture);
  }
}

void TunerDSPWrapper::Process(sample** inputs, sample** outputs, int nFrames) {
  // iPlug2 sample** is double**, FAUSTFLOAT* is float*.  The numeric value
  // is preserved for audio-range ±1.0 with a reinterpret_cast (the
  // pass-through process() simply casts each sample back to float).  We
  // MUST only address 2 inputs + 2 outputs; iPlug2 allocates only
  // PLUG_CHANNEL_IO buffers.
  FAUSTFLOAT* in[2]  = { reinterpret_cast<FAUSTFLOAT*>(inputs[0]),
                         reinterpret_cast<FAUSTFLOAT*>(inputs[1]) };
  FAUSTFLOAT* out[2] = { reinterpret_cast<FAUSTFLOAT*>(outputs[0]),
                         reinterpret_cast<FAUSTFLOAT*>(outputs[1]) };
  mDSP->compute(nFrames, in, out);
}

FAUSTFLOAT* TunerDSPWrapper::GetZone(const char* label) {
  auto it = mZones.find(label);
  if (it != mZones.end()) return it->second;
  return nullptr;
}


// =============================================================================
// TunerAnalysis::Detector
// =============================================================================
//
// Algorithm (exceeds the Cmajor source on every axis):
//   1. YIN with cumulative mean normalized difference function
//   2. Adaptive threshold 0.10..0.18 based on signal level
//   3. 4-point Neville polynomial interpolation on the CMND minimum
//   4. MPM (McLeod Pitch Method) cross-validation via NSDF
//   5. 7-tap median filter for octave rejection
//   6. Asymmetric pitch smoother (>2 semi -> 0.6; >0.5 -> 0.2;
//      else 0.03 + 0.4 * userSmooth)
//   7. Asymmetric cents smoother (alpha = 1 / (1 + 0.05 * |cents|))
//
// =============================================================================
// TunerAnalysis::Detector -- algorithm documentation
// =============================================================================
//
// Implementation lives in TunerAnalysis.cpp (host-free TU; split out in
// round-11 so the detector is linkable from a stand-alone smoke test that
// does NOT include iPlug2).  That file contains the bodies of:
//
//   Detector::Init       -- configure sample rate + reset all state
//   Detector::Reset      -- zero ring buffer + atomic Result fields
//   Detector::PushSample -- RT-3 NaN/Inf clamp, ring-buffer write,
//                            fire RunAnalysis() every kAnalysisStride samples
//   Detector::DetectYIN  -- YIN with cumulative mean normalized difference
//   Detector::DetectMPM  -- McLeod Pitch Method (NSDF parabola)
//   Detector::RefineLagNeville -- 3-point Neville polynomial refinement
//   Detector::MedianFilter -- 7-tap bounded partial sort (RT-2 safe)
//   Detector::SmoothPitch  -- asymmetric alpha smoothing (semantic-aware)
//   Detector::SmoothCents  -- alpha = 1 / (1 + 0.05 * |cents|)
//   Detector::NoteFromPitch -- MIDI/note/cents mapping w/ A4 reference glide
//   Detector::RunAnalysis -- YIN+MPM pick + MedianFilter + smoothing + atomics
//
// The Detector::GetResult() return type / fields are the audio->UI hand-off
      mDetector.SetA4Reference(kA4Hz[(idx < 0 || idx > 5) ? 3 : idx]);
      break;
    }
    default: break;
  }
}

void Tuner::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  // Bit-perfect pass-through.  We deliberately do NOT route through the
  // FAUST wrapper here: its generated code does
  //   output[i] = (float)(float)(input[i])
  // after a reinterpret_cast<float*>(double*) pointer-aliasing, which
  // corrupts non-power-of-2 sample values and writes 4 bytes where 8 were
  // expected (overflowing into the next sample).  A tuner must never
  // touch the audio -- so a plain memcpy is the only correct option.
  //
  // The detector still reads from `inputs` (read-only); the pass-through
  // and the analysis happen on the same source buffer.
  for (int ch = 0; ch < 2; ++ch) {
    std::memcpy(outputs[ch], inputs[ch], static_cast<size_t>(nFrames) * sizeof(sample));
  }

  const sample* inL = inputs[0];
  const sample* inR = inputs[1];

  const double sr = GetSampleRate();
  const double rmsCoeff = 1.0 - std::exp(-1.0 / std::max(1.0, 0.050 * sr));

  double runningRms = mDetector.GetResult().level.load();
  if (std::isnan(runningRms) || runningRms < 0.0) runningRms = 0.0;

  for (int i = 0; i < nFrames; ++i) {
    double mono = 0.5 * ((double)inL[i] + (double)inR[i]);
    if (!std::isfinite(mono)) mono = 0.0;  // RT-4: guard NaN/Inf before RMS + detector
    const double sq = mono * mono;
    runningRms = rmsCoeff * sq + (1.0 - rmsCoeff) * runningRms;
    mDetector.PushSample(mono);
  }

  const double levelOut = std::min(1.0, std::max(0.0, std::sqrt(runningRms)));
  mDetector.GetResult().level.store(levelOut);
}

#if IPLUG_EDITOR

// Freeze95/BronzeNoise pattern: setComponentState is a no-op in non-distributed
// VST3, so OnActivate is the only signal that the parameter container may hold
// values that diverge from the UI. Set mSendUpdate and let OnIdle push them.
void Tuner::OnActivate(bool active) {
  if (active) mSendUpdate = true;
}

void Tuner::OnIdle() {
  // PERF-1: drain the analysis-path profiler once a second and log to the
  // host's debug stream (OutputDebugString on Windows, NSLog on macOS).
  // At 30 FPS this fires after 30 frames; analysis runs at ~47 Hz which
  // means each sample contains ~1.57 detector passes worth of timing.
  static int s_idleFrames = 0;
  if (++s_idleFrames >= 30) {
    s_idleFrames = 0;
    const double avgUs = mDetector.Profiler().ReadAndReset();
    // Compute the fraction of one analysis-pass slot (kAnalysisStride=1024
    // samples / SR) that the detector consumed -- this is the metric that
    // matters for real-time budgeting regardless of the actual pass rate.
    const double slotUs = (double)TunerAnalysis::kAnalysisStride / GetSampleRate() * 1e6;
    [[maybe_unused]] const double pctSlot = (slotUs > 0.0) ? (avgUs / slotUs * 100.0) : 0.0;
    DBGMSG("Tuner: analysis avg=%.1f us (%.2f%% of %d-sample slot @ %.0f Hz)\n",
           avgUs, pctSlot, (int)TunerAnalysis::kAnalysisStride, GetSampleRate());
  }
  if (mSendUpdate) {
    if (GetUI()) SendCurrentParamValuesFromDelegate();
    mSendUpdate = false;
  }
  if (GetUI()) {
    GetUI()->SetAllControlsDirty();
  }
}

// LayoutUI -- load the TexasLED display TTF + CLOPH brand logo from the
// resource compiler's embedded payload (resources/main.rc).
//
// Matches the Freeze95 pattern exactly: bare-filename calls into
// g.LoadFont / g.LoadSVG.  iPlug2's LocateResource -> EnumResourceNamesW
// -> quote-strip -> strcmp chain finds the embedded resource by the
// bare filename (e.g. "CLOPH.svg"), and the filename doubles as the
// cache key.  No sidecar TEXASLED.TTF / CLOPH.svg copies next to the
// .vst3 required.
//
// IMPORTANT: LoadSVG takes ONE argument (the filename), not two.  The
// signature is `LoadSVG(const char* fileNameOrResID, const char* units,
// float dpi)` -- a 2-arg call would interpret the second string as
// the `units` parameter ("px", "mm", etc.) and silently fail the
// resource lookup.
void Tuner::LayoutUI(IGraphics* pGraphics) {
  IGraphics& g = *pGraphics;

  // 7-segment-style display font for note names + cents readout.
  g.LoadFont("TexasLED", TEXASLED_FN);

  // CLOPH brand logo (bottom-right of the readout).
  ISVG logo = g.LoadSVG(CLOPH_LOGO_FN);

  TunerReadoutControl* pReadout = new TunerReadoutControl(IRECT(0, 0, PLUG_WIDTH, PLUG_HEIGHT), this);
  if (logo.IsValid()) pReadout->SetLogo(logo);
  g.AttachControl(pReadout);

  // Initial sync: push current host param values to UI. OnActivate/OnIdle
  // only fires on re-activation, not on first window open.
  SendCurrentParamValuesFromDelegate();
}

#endif // IPLUG_EDITOR