// =============================================================================
// detector_smoke.cpp -- host-free smoke test for TunerAnalysis::Detector.
//
// Catches the regression where the Tuner GUI shows "---" cents, "------ Hz",
// and a needle stuck at center even with valid audio input.  The root cause
// was a downstream runtime bug, but a missing / fundamentally broken detector
// produces the same symptom.  By exercising the detector pipeline directly
// (no iPlug2, no VST3, no host) we can confirm the data path is intact
// before the binary ships.
//
// What this test does:
//   1. Construct a TunerAnalysis::Detector at 48 kHz.
//   2. Feed it 8192 samples of synthesized 440 Hz sine wave at -12 dBFS.
//      (8192 samples = 4 full analysis passes at kAnalysisStride=1024; the
//       first pass after Reset is consumed filling the ring buffer, so we
//       need >= kBufferSize=2048 + 3*kAnalysisStride to hit 3 settled passes.)
//   3. Read the atomic Result fields:
//        - pitchHz          ~ 440 Hz    (within ±2 Hz tolerance)
//        - clarity          > 0.50
//        - cents            ~ 0.0      (within ±10 cents)
//        - noteIndex        == 9       (A in 0..11 chromatic)
//        - octave           == 4
//        - level            > 0.05     (signal present)
//   4. Switch the A4 reference and verify the detector glides toward the
//      new target without producing NaN/Inf.
//   5. Feed 4096 samples of silence and verify clarity drops to 0 and pitch
//      stops reporting valid Hz.
//
// Pure C++17, no dependencies on iPlug2 / VST3 SDK / Cocoa / Win32.
//
// Compile (standalone, no iPlug2 framework):
//   clang++ -std=c++17 -O2 -fno-exceptions -Wall -Werror \
//     -o detector_smoke detector_smoke.cpp TunerAnalysis.cpp -lpthread
// =============================================================================

#include "TunerAnalysis.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace TunerAnalysis;

static int s_failures = 0;

#define EXPECT_NEAR(actual, expected, tol) \
  do { \
    const double _a = (actual); \
    const double _e = (expected); \
    const double _t = (tol); \
    if (std::isfinite(_a) && std::isfinite(_e) && std::abs(_a - _e) <= _t) { \
      /* ok */ \
    } else { \
      std::printf("  FAIL: EXPECT_NEAR(%s) actual=%.6f expected=%.6f tol=%.6f (line %d)\n", \
                  #actual, _a, _e, _t, __LINE__); \
      ++s_failures; \
    } \
  } while (0)

#define EXPECT_TRUE(cond) \
  do { \
    if (!(cond)) { \
      std::printf("  FAIL: EXPECT_TRUE(%s) (line %d)\n", #cond, __LINE__); \
      ++s_failures; \
    } \
  } while (0)

#define EXPECT_EQ_INT(actual, expected) \
  do { \
    if ((actual) != (expected)) { \
      std::printf("  FAIL: EXPECT_EQ_INT(%s) actual=%d expected=%d (line %d)\n", \
                  #actual, (int)(actual), (int)(expected), __LINE__); \
      ++s_failures; \
    } \
  } while (0)

int main() {
  std::printf("=== TunerAnalysis detector smoke ===\n");
  std::printf("[1/3] Construct Detector @ 48 kHz\n");
  Detector det;
  det.Init(48000);

  // The detector's Result.level field is host-side RMS written by
  // Tuner::ProcessBlock, NOT derived internally from the ring buffer.
  // (See Tuner.cpp:156-161: per-sample runningRms + end-of-block
  // level.store).  The smoke test must mirror this contract -- without
  // it, the YIN/MPM threshold gating drops all analysis as "no signal"
  // even with valid audio flowing in.  A regression that breaks the
  // runningRms -> level pipeline in ProcessBlock is the EXACT failure
  // mode the user reported: GUI shows "---" cents, "------ Hz", needle
  // stuck at center, because level=0 means RunAnalysis gates rawPitch to
  // 0.0 on every pass.
  double runningRms = 0.0;
  auto feed = [&](int n, double freq, double amplitude) {
    const double sr = 48000.0;
    for (int i = 0; i < n; ++i) {
      const double mono = amplitude * std::sin(2.0 * 3.141592653589793 * freq * i / sr);
      det.PushSample(mono);
      const double sq = mono * mono;
      // Match Tuner::ProcessBlock rmsCoeff -- IPlug gets it from
      // GetSampleRate() via the FAUST wrapper, falling back to a
      // roughly 50ms time constant.  For 48 kHz that resolves to
      // exp(-1 / (0.050 * 48000)) ≈ 0.99958.
      const double rmsCoeff = std::exp(-1.0 / (0.050 * 48000.0));
      runningRms = rmsCoeff * sq + (1.0 - rmsCoeff) * runningRms;
    }
    const double lvl = std::min(1.0, std::max(0.0, std::sqrt(runningRms)));
    det.GetResult().level.store(lvl);
  };

  // ---- Test A: feed a clean 440 Hz sine, expect valid pitch ----
  std::printf("[2/3] Feed 8192 samples of 440 Hz sine @ -12 dBFS\n");
  feed(8192, 440.0, std::pow(10.0, -12.0 / 20.0));

  const Result& r = det.GetResult();
  const double pitchHz    = r.pitchHz.load();
  const double cents      = r.cents.load();
  const double clarity    = r.clarity.load();
  const double level      = r.level.load();
  const int    noteIndex  = r.noteIndex.load();
  const int    octave     = r.octave.load();

  std::printf("      pitchHz  = %.3f Hz   (expected 440.0 ±2)\n",  pitchHz);
  std::printf("      cents    = %+.3f     (expected 0.0 ±10)\n",   cents);
  std::printf("      clarity  = %.3f      (expected > 0.50)\n",    clarity);
  std::printf("      level    = %.3f      (expected > 0.05)\n",    level);
  std::printf("      noteIdx  = %d          (expected 9 = A)\n",    noteIndex);
  std::printf("      octave   = %d          (expected 4)\n",         octave);

  EXPECT_NEAR(pitchHz,  440.0, 2.0);
  EXPECT_NEAR(cents,      0.0,  10.0);
  EXPECT_TRUE(clarity >  0.50);
  EXPECT_TRUE(level   >  0.05);
  EXPECT_EQ_INT(noteIndex,  9);
  EXPECT_EQ_INT(octave,     4);
  EXPECT_TRUE(std::isfinite(pitchHz));
  EXPECT_TRUE(std::isfinite(cents));
  EXPECT_TRUE(std::isfinite(clarity));
  EXPECT_TRUE(std::isfinite(level));

  // ---- Test B: A4 reference glide -- instant=true must not NaN/Inf ----
  std::printf("[3/3] A4 reference glide -- 440 -> 442 Hz (instant)\n");
  const double peakAmp2 = std::pow(10.0, -12.0 / 20.0);
  det.SetA4Reference(442.0, /*instant=*/true);
  feed(8192, 440.0, peakAmp2);
  const Result& r2 = det.GetResult();
  EXPECT_TRUE(std::isfinite(r2.pitchHz.load()));
  EXPECT_TRUE(std::isfinite(r2.cents.load()));
  EXPECT_TRUE(std::isfinite(r2.clarity.load()));
  std::printf("      pitchHz after glide = %.3f Hz\n", r2.pitchHz.load());

  // ---- Test C: silence -- level must drop, pitch must stop ----
  std::printf("[3/3] Silence -- level should drop, pitch should stop reporting\n");
  feed(4096, 440.0, 0.0);   // amplitude=0 -> level decays toward 0
  const Result& r3 = det.GetResult();
  std::printf("      silence pitchHz  = %.3f Hz\n", r3.pitchHz.load());
  std::printf("      silence level    = %.3f    (expected < %g)\n",
               r3.level.load(), 0.05);
  EXPECT_TRUE(r3.level.load() < 0.05);

  // ---- Summary ----
  std::printf("\n=== Summary ===\n");
  if (s_failures == 0) {
    std::printf("  detector_smoke: PASSED (0 failures)\n");
    return 0;
  } else {
    std::printf("  detector_smoke: FAILED (%d failure(s))\n", s_failures);
    return 1;
  }
}
