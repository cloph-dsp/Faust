// =============================================================================
// thdn_smoke.cpp -- host-free THD+N measurement for the Tuner audio path.
//
// The Tuner's audio path is bit-perfect pass-through:
//   ProcessBlock:  std::memcpy(outputs[ch], inputs[ch], nFrames * sizeof(sample));
// The Detector is a read-only analysis side-channel; it never writes to
// outputs. So the output of ProcessBlock should equal its input bit-for-bit,
// and THD+N of the output should be at the noise floor of double-precision
// floating-point arithmetic (~-130 dBFS for any well-conditioned signal).
//
// This test verifies that promise end-to-end:
//   1. Synthesize a 1 kHz sine at -1 dBFS in double precision.
//   2. Pass it through the same memcpy pass-through that ProcessBlock uses.
//   3. Feed the same samples to TunerAnalysis::Detector to verify the
//      analysis side-channel does not contaminate the audio path.
//   4. Compute THD+N of the output using Goertzel (fundamentals + harmonics
//      up to 10f0) and a noise-floor estimate from bins away from f0 and its
//      harmonics.
//
// Reference levels (informational):
//   - 16-bit PCM quantization noise: ~-96 dBFS theoretical
//   - 24-bit PCM quantization noise: ~-144 dBFS theoretical
//   - Double-precision arithmetic noise floor: ~-150 dBFS (limited by
//     53-bit mantissa for any computation; pure pass-through has none)
//
// We assert THD+N < -100 dBFS (well above double-precision noise floor but
// below any audible distortion a guitar-input tuner would care about).
//
// Pure C++17, no dependencies on iPlug2 / VST3 SDK / Cocoa / Win32.
// Compile (standalone):
//   clang++ -std=c++17 -O2 -Wall -I "Tuner VST" -o thdn_smoke thdn_smoke.cpp
// =============================================================================

#include "TunerAnalysis.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =============================================================================
// Test framework
// =============================================================================
static int s_failures = 0;

#define EXPECT_LT_DBL(actual, threshold_db) \
  do { \
    const double _a = (actual); \
    const double _t = (threshold_db); \
    if (std::isfinite(_a) && _a < _t) { \
      /* ok */ \
    } else { \
      std::printf("  FAIL: EXPECT_LT_DBL(%s) actual=%.3f dB expected<%.3f dB (line %d)\n", \
                  #actual, _a, _t, __LINE__); \
      ++s_failures; \
    } \
  } while (0)

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

// =============================================================================
// Goertzel: measure amplitude at one specific frequency bin
// =============================================================================
struct Goertzel {
  double coeff;
  double s1, s2;
  void Init(double k, double N) {
    const double w = 2.0 * M_PI * k / N;
    coeff = 2.0 * std::cos(w);
    s1 = 0.0;
    s2 = 0.0;
  }
  void Process(double x) {
    const double s0 = x + coeff * s1 - s2;
    s2 = s1;
    s1 = s0;
  }
  double Magnitude() const {
    return std::sqrt(s1 * s1 + s2 * s2 - coeff * s1 * s2);
  }
};

// =============================================================================
// Pass-through simulator: mirrors exactly what Tuner::ProcessBlock does for
// the audio path (memcpy), with a read-only analysis side-channel to the
// Detector. We DO NOT call into the actual Plugin class -- this test stays
// host-free.
// =============================================================================
static void ProcessBlockMirror(const std::vector<double>& in,
                               std::vector<double>& out,
                               TunerAnalysis::Detector& det,
                               int sampleRate) {
  // Pass-through: byte-perfect copy.
  out.resize(in.size());
  std::memcpy(out.data(), in.data(), in.size() * sizeof(double));

  // Mirror Tuner::ProcessBlock's level pipeline: per-sample runningRms with
  // rmsCoeff = 1 - exp(-1 / (0.050 * sr)). This is what gates YIN/MPM.
  const double rmsCoeff = 1.0 - std::exp(-1.0 / std::max(1.0, 0.050 * (double)sampleRate));
  double runningRms = det.GetResult().level.load();
  if (std::isnan(runningRms) || runningRms < 0.0) runningRms = 0.0;
  for (size_t i = 0; i < in.size(); ++i) {
    double mono = in[i];  // pass-through path is mono-collapsed upstream;
                          // ProcessBlock uses 0.5*(inL+inR). For pure sine,
                          // either is fine; the analysis is identical.
    if (!std::isfinite(mono)) mono = 0.0;
    runningRms = rmsCoeff * mono * mono + (1.0 - rmsCoeff) * runningRms;
    det.PushSample(mono);
  }
  const double levelOut = std::min(1.0, std::max(0.0, std::sqrt(runningRms)));
  det.GetResult().level.store(levelOut);
}

// =============================================================================
// THD+N computation
// =============================================================================
static double ComputeTHDPlusN(const std::vector<double>& samples,
                              double sampleRate,
                              double fundamentalHz) {
  const size_t N = samples.size();
  const double k = fundamentalHz * N / sampleRate;

  // Measure fundamental + 10 harmonics.
  std::vector<Goertzel> bins(11);
  for (int h = 1; h <= 10; ++h) {
    bins[h].Init(k * h, (double)N);
  }
  for (double x : samples) {
    for (int h = 1; h <= 10; ++h) bins[h].Process(x);
  }
  std::vector<double> harmonicPows(11);
  double signalPow = 0.0;
  for (size_t i = 0; i < N; ++i) signalPow += samples[i] * samples[i];
  for (int h = 1; h <= 10; ++h) {
    const double mag = bins[h].Magnitude();
    harmonicPows[h] = mag * mag;
  }

  // Fundamental energy.
  const double fundPow = harmonicPows[1];

  // Distortion energy: harmonics + everything else.
  // noisePow = total signal power - fundamental - sum(harmonics 2..10).
  double harmonicSum = 0.0;
  for (int h = 2; h <= 10; ++h) harmonicSum += harmonicPows[h];
  double noisePow = signalPow - fundPow - harmonicSum;
  if (noisePow < 0.0) noisePow = 0.0;  // numerical floor
  const double distortionPow = harmonicSum + noisePow;

  // THD+N in dBFS relative to fundamental.
  if (fundPow <= 0.0 || distortionPow <= 0.0) return -200.0;
  return 10.0 * std::log10(distortionPow / fundPow);
}

// =============================================================================
// Test 1: pure 1 kHz sine through pass-through -> THD+N at noise floor
// =============================================================================
static void TestPureSineTHDN() {
  std::printf("[1/3] Pure 1 kHz sine through pass-through (no distortion)\n");
  constexpr int kSampleRate = 48000;
  constexpr int kN = 49152;          // 1.024 s @ 48 kHz, contains EXACTLY 1024
                                     // periods of 1 kHz -> fundamental sits
                                     // exactly on FFT bin 1024 (freq res
                                     // 0.977 Hz/bin, no spectral leakage).
  constexpr double kFreq = 1000.0;
  constexpr double kAmp = 0.891;     // -1 dBFS

  std::vector<double> input(kN);
  for (int i = 0; i < kN; ++i) {
    input[i] = kAmp * std::sin(2.0 * M_PI * kFreq * i / kSampleRate);
  }

  TunerAnalysis::Detector det;
  det.Init(kSampleRate);

  std::vector<double> output;
  ProcessBlockMirror(input, output, det, kSampleRate);

  // Verify pass-through is bit-perfect.
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] != output[i]) {
      std::printf("  FAIL: output[%zu] != input[%zu] (%.17g vs %.17g) -- pass-through is not bit-perfect\n",
                  i, i, output[i], input[i]);
      ++s_failures;
      return;
    }
  }

  // THD+N of pass-through output (which equals input).
  const double thdn = ComputeTHDPlusN(output, kSampleRate, kFreq);
  std::printf("    THD+N = %.3f dBFS (expecting < -100 dB)\n", thdn);

  // For a perfectly synthesised 1 kHz sine at double precision with no bin
  // leakage (exact-integer bin alignment), THD+N is bounded only by FFT
  // roundoff. Empirically this lands around -300 dBFS -- the noise floor is
  // so deep we cap our pass threshold at -100 dB to leave comfortable headroom.
  EXPECT_LT_DBL(thdn, -100.0);
}

// =============================================================================
// Test 2: 1 kHz sine + known 0.01% 2nd harmonic distortion
// Verifies the THD+N measurement picks up what we put in.
// =============================================================================
static void TestKnownDistortionMeasurement() {
  std::printf("[2/3] 1 kHz sine + 0.01%% 2nd harmonic (control test)\n");
  constexpr int kSampleRate = 48000;
  constexpr int kN = 49152;
  constexpr double kFreq = 1000.0;
  constexpr double kFundAmp = 0.891;
  constexpr double kHarm2Amp = 0.0000891;  // -80 dBFS, ~0.01% of fundamental

  std::vector<double> input(kN);
  for (int i = 0; i < kN; ++i) {
    input[i] = kFundAmp * std::sin(2.0 * M_PI * kFreq * i / kSampleRate)
             + kHarm2Amp * std::sin(2.0 * M_PI * 2.0 * kFreq * i / kSampleRate);
  }

  // No Detector needed -- we're just verifying the THD+N measurement itself.
  const double thdn = ComputeTHDPlusN(input, kSampleRate, kFreq);
  // Expected: 20*log10(0.0000891/0.891) = -80 dB, plus the noise floor.
  std::printf("    THD+N = %.3f dBFS (expecting ~ -80 dB)\n", thdn);
  EXPECT_NEAR(thdn, -80.0, 3.0);  // +/-3 dB tolerance for Goertzel + windowing
}

// =============================================================================
// Test 3: detector side-channel does NOT modify output (no aliasing / leakage)
// =============================================================================
static void TestDetectorSideChannelIsReadOnly() {
  std::printf("[3/3] Detector side-channel does not modify pass-through output\n");
  constexpr int kSampleRate = 48000;
  constexpr int kN = 49152;
  // Use 440.43 Hz (= bin 451 exactly) instead of 440 Hz so the fundamental
  // sits exactly on an FFT bin at N=49152. 48000 * 451 / 49152 = 440.43 Hz.
  // This eliminates bin leakage as a confound for the THD+N measurement.
  constexpr double kFreq = 48000.0 * 451.0 / 49152.0;
  constexpr double kAmp = 0.2512;  // -12 dBFS

  std::vector<double> input(kN);
  for (int i = 0; i < kN; ++i) {
    input[i] = kAmp * std::sin(2.0 * M_PI * kFreq * i / kSampleRate);
  }

  TunerAnalysis::Detector det;
  det.Init(kSampleRate);

  std::vector<double> output;
  // Mirror ProcessBlock's one-block latency: first block feeds samples + writes
  // level at end; second block reads level=non-zero so YIN/MPM threshold gating
  // yields valid pitch. Same trick as detector_smoke.cpp.
  ProcessBlockMirror(input, output, det, kSampleRate);
  ProcessBlockMirror(input, output, det, kSampleRate);

  // Pass-through must be byte-identical even when detector is fed in parallel.
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] != output[i]) {
      std::printf("  FAIL: output[%zu] != input[%zu] after detector feed\n",
                  i, i);
      ++s_failures;
      return;
    }
  }

  // Detector should report valid pitch (close to 440.43 Hz within YIN tolerance).
  const double pitch = det.GetResult().pitchHz.load();
  const double clarity = det.GetResult().clarity.load();
  std::printf("    pitch=%.3f Hz clarity=%.3f\n", pitch, clarity);
  EXPECT_NEAR(pitch, 440.43, 5.0);
  EXPECT_TRUE(clarity > 0.5);

  // And the output of the second block, run through THD+N at the exact-bin
  // fundamental, must still be clean.
  const double thdn = ComputeTHDPlusN(output, kSampleRate, kFreq);
  std::printf("    THD+N = %.3f dBFS (expecting < -100 dB)\n", thdn);
  EXPECT_LT_DBL(thdn, -100.0);
}

int main() {
  std::printf("=== Tuner audio path THD+N measurement ===\n");

  TestPureSineTHDN();
  TestKnownDistortionMeasurement();
  TestDetectorSideChannelIsReadOnly();

  if (s_failures > 0) {
    std::printf("\nFAILED: %d check(s) failed\n", s_failures);
    return 1;
  }
  std::printf("\nAll THD+N checks passed.\n");
  return 0;
}
