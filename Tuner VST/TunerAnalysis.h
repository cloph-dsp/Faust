#pragma once

// =============================================================================
// TunerAnalysis.h - host-free detector + analysis primitives for the Tuner VST.
//
// Design intent: every algorithm that does pure DSP / signal analysis lives
// here, with NO iPlug2 / VST3 / Win32 / Cocoa headers.  This file is
// includable from a stand-alone unit test (Tuner VST/tests/detector_smoke),
// without dragging in the iPlug2 framework.  It is header-only: the impl
// lives in TunerAnalysis_impl.h, which is included at the bottom of this
// file so consumers see one logical translation unit.  The header-only
// shape sidesteps the vcxproj / xcodeproj edit that would otherwise be
// required to add a new .cpp file to the iPlugEffect example build.
//
// The atomic Result struct is the audio->UI hand-off: the audio thread
// populates pitchHz / cents / clarity / level / noteIndex / octave atomically
// in Detector::RunAnalysis(); the UI thread reads them in Draw() via
// GetAnalysisResult().
//
// All algorithm mathematics are real-time safe:
//   - no heap allocation in push/analysis paths
//   - no std::nth_element (replaced with bounded partial sort in
//     Detector::MedianFilter)
//   - all branches are bounded
//   - NaN/Inf are clamped at the input boundary (Detector::PushSample)
// =============================================================================

#include <atomic>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>

namespace TunerAnalysis {

// Analysis is decimated by two with a two-sample box low-pass. This keeps the
// existing CPU budget while doubling the history to ~85 ms: at 48 kHz the
// usable fundamental range becomes roughly 23 Hz..1.5 kHz (B0 through the
// upper range of acoustic instruments), instead of ~47 Hz..1.5 kHz.
constexpr int kDecimationFactor = 2;
constexpr int kBufferSize       = 2048;
constexpr int kMinLag           = 16;
constexpr int kMaxLag           = 1024;
constexpr int kMedianSize       = 7;
constexpr int kAnalysisStride   = 512;

// Result -- the audio->UI hand-off.  Write side: audio thread, every
// kAnalysisStride decimated samples via Detector::RunAnalysis(). Read side: UI thread,
// every Draw() frame via Detector::GetResult().
//
// The fields are atomic for the audio/UI hand-off. ReadSnapshot() additionally
// prevents a UI frame from combining note/cents from different analyses.
struct Result {
  struct Snapshot {
    double pitchHz = 0.0;
    double cents = 0.0;
    double clarity = 0.0;
    double level = 0.0;
    int noteIndex = -1;
    int octave = 4;
  };

  // The individual atomics remain public for the host-side level writer and
  // backwards-compatible tests. The sequence counter lets the GUI read the
  // pitch/note/cents tuple from one analysis pass rather than a mixed frame.
  std::atomic<uint32_t> sequence { 0 };
  std::atomic<double> pitchHz   { 0.0 };
  std::atomic<double> cents     { 0.0 };
  std::atomic<double> clarity   { 0.0 };
  std::atomic<double> level     { 0.0 };
  std::atomic<int>    noteIndex { -1 };
  std::atomic<int>    octave    { 4 };

  void Publish(double newPitchHz, double newCents, double newClarity,
               int newNoteIndex, int newOctave) {
    sequence.fetch_add(1, std::memory_order_release); // writer active (odd)
    pitchHz.store(newPitchHz, std::memory_order_relaxed);
    cents.store(newCents, std::memory_order_relaxed);
    clarity.store(newClarity, std::memory_order_relaxed);
    noteIndex.store(newNoteIndex, std::memory_order_relaxed);
    octave.store(newOctave, std::memory_order_relaxed);
    sequence.fetch_add(1, std::memory_order_release); // stable snapshot (even)
  }

  Snapshot ReadSnapshot() const {
    for (int attempt = 0; attempt < 8; ++attempt) {
      const uint32_t before = sequence.load(std::memory_order_acquire);
      if (before & 1U) continue;
      Snapshot snapshot;
      snapshot.pitchHz = pitchHz.load(std::memory_order_relaxed);
      snapshot.cents = cents.load(std::memory_order_relaxed);
      snapshot.clarity = clarity.load(std::memory_order_relaxed);
      snapshot.level = level.load(std::memory_order_relaxed);
      snapshot.noteIndex = noteIndex.load(std::memory_order_relaxed);
      snapshot.octave = octave.load(std::memory_order_relaxed);
      if (before == sequence.load(std::memory_order_acquire)) return snapshot;
    }

    // A writer only runs ~47 times/sec, so this fallback is exceptionally
    // unlikely. Returning a fresh atomic read is still safe for drawing.
    return { pitchHz.load(), cents.load(), clarity.load(), level.load(),
             noteIndex.load(), octave.load() };
  }
};

// Lightweight timer for the analysis path.  Accumulates microseconds per
// RunAnalysis() pass; the UI thread (OnIdle) drains the average via
// ReadAndReset().  Not thread-safe for concurrent Begin/End + Read (Begin/End
// always run on the audio thread, ReadAndReset always on the UI thread --
// atomicity of double reads is not required because the value is only used
// for developer diagnostics and a single torn read is harmless).
struct AnalysisProfiler {
  void Begin() { mStart = std::chrono::steady_clock::now(); }
  void End() {
    auto end = std::chrono::steady_clock::now();
    double us = std::chrono::duration<double, std::micro>(end - mStart).count();
    mTotalUs += us;
    mCount++;
  }
  double ReadAndReset() {
    double avg = (mCount > 0) ? (mTotalUs / mCount) : 0.0;
    mTotalUs = 0.0;
    mCount = 0;
    return avg;
  }
private:
  std::chrono::steady_clock::time_point mStart;
  double mTotalUs = 0.0;
  int mCount = 0;
};

class Detector {
public:
  void Init(int sampleRate);
  void Reset();
  void PushSample(double mono);
  Result& GetResult() { return mResult; }
  const Result& GetResult() const { return mResult; }
  void SetSmoothing(double s)        { mSmooth.store(s, std::memory_order_relaxed); }
  void SetA4Reference(double hz, bool instant = false) {
    mA4RefTarget.store(hz, std::memory_order_relaxed);
    if (instant) mA4Ref = hz;
  }

  // Read by the plugin (UI thread) to drain the analysis-path profiler.
  AnalysisProfiler& Profiler() { return mProfiler; }

private:
  void RunAnalysis();
  double DetectYIN(double& clarityOut);
  double DetectMPM(double& clarityOut);
  double RefineLagParabolic(const double* y, int bestLag) const;
  double MedianFilter(double candidate);
  double SmoothPitch(double candidate);
  double ChooseCandidate(double yinHz, double yinClarity,
                         double mpmHz, double mpmClarity) const;
  double StabilizeCandidate(double candidate, double& clarityInOut);
  double NoteFromPitch(double hz, int& noteIdxOut, int& octaveOut) const;

  std::array<double, kBufferSize> mBuffer{};
  int    mWriteIdx     = 0;
  int    mSampleCount  = 0;
  int    mSamplesSince = 0;
  // This is the post-decimation rate used by lag-to-Hz conversion.
  double mSampleRate   = 24000.0;
  std::atomic<double> mSmooth { 0.55 };
  double mA4Ref        = 440.0;
  std::atomic<double> mA4RefTarget{440.0};

  double mDecimationAccumulator = 0.0;
  int mDecimationCount = 0;

  std::array<double, kMedianSize> mMedianBuf{};
  int mMedianIdx = 0;

  double mLastPitch = 0.0;
  double mCommittedPitch = 0.0;
  double mPendingPitch = 0.0;
  double mLastStableClarity = 0.0;
  int mPendingFrames = 0;
  int mNoPitchFrames = 0;

  Result mResult;
  AnalysisProfiler mProfiler;
};

} // namespace TunerAnalysis

// Inline impl file -- everything below is part of the public header so the
// host-free smoke test (and the iPlugEffect example build, which never sees
// a separate TunerAnalysis.cpp) can link without a separate TU.
#include "TunerAnalysis_impl.h"
