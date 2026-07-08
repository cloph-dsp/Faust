/* ------------------------------------------------------------
 * TunerDSP.h - C++ wrapper around the Faust-generated TunerDSP_gen.h
 *
 * The Faust .dsp file owns only the signal-conditioning chain (pre-emphasis,
 * DC blocker, RMS meter).  The pitch detection (YIN + MPM + Neville + median
 * filter + smoothing) lives in C++ inside Tuner.cpp; this wrapper exists to
 * keep the iPlug2 plugin class decoupled from Faust's generated code.
 *
 * Generation command (kept here for reproducibility):
 *   faust -lang cpp -i -light -nvi -ec -ct 1 -cn TunerDSP \
 *         -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2 \
 *         TunerDSP.dsp -o TunerDSP_gen.h
 * ------------------------------------------------------------ */

#ifndef __TUNER_DSP_H__
#define __TUNER_DSP_H__

#include "FaustCompat.h"
#include "TunerDSP_gen.h"

// Thin facade around the generated Faust class.  We don't expose
// buildUserInterface() directly -- Tuner.cpp uses its own ZoneCaptureUI.
class TunerDSPWrapper {
public:
  TunerDSPWrapper();
  ~TunerDSPWrapper() = default;

  void Init(int sampleRate);
  void Process(sample** inputs, sample** outputs, int nFrames);

  // FAUSTFLOAT* zone lookup.  Keys are the labels the .dsp uses in
  // addHorizontalBargraph / addHorizontalSlider.  Returns nullptr if not
  // found -- callers must check.
  FAUSTFLOAT* GetZone(const char* label);

private:
  std::unique_ptr<TunerDSP> mDSP;
  std::unordered_map<std::string, FAUSTFLOAT*> mZones;
};

#endif // __TUNER_DSP_H__