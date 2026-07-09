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
#include <array>
#include <chrono>
#include <cmath>

namespace TunerAnalysis {

constexpr int kBufferSize       = 2048;
constexpr int kMinLag           = 32;
constexpr int kMaxLag           = 1024;
constexpr int kMedianSize       = 7;
constexpr int kAnalysisStride   = 1024;

// Result -- the audio->UI hand-off.  Write side: audio thread, every
// kAnalysisStride samples via Detector::RunAnalysis().  Read side: UI thread,
// every Draw() frame via Detector::GetResult().
//
// atomicity of double loads/stores is implementation-defined for tearing on
// platforms without 64-bit-atomic hardware (32-bit ARM pre-v8.1).  Torn reads
// on a single atomic<double> yield a value that is one ULP away from a valid
// past or future value -- harmless for visual display where the next frame
// re-reads.  std::atomic<double> is sufficient for the visual use case.
struct Result {
  std::atomic<double> pitchHz   { 0.0 };
  std::atomic<double> cents     { 0.0 };
  std::atomic<double> clarity   { 0.0 };
  std::atomic<double> level     { 0.0 };
  std::atomic<int>    noteIndex { -1 };
  std::atomic<int>    octave    { 4 };
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
  void SetSmoothing(double s)        { mSmooth = s; }
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
  double RefineLagNeville(const double* y, int bestLag) const;
  double MedianFilter(double candidate);
  double SmoothPitch(double candidate);
  double SmoothCents(double candidate);
  double NoteFromPitch(double hz, int& noteIdxOut, int& octaveOut) const;

  std::array<double, kBufferSize> mBuffer{};
  int    mWriteIdx     = 0;
  int    mSampleCount  = 0;
  int    mSamplesSince = 0;
  double mSampleRate   = 48000.0;
  double mSmooth       = 0.55;
  double mA4Ref        = 440.0;
  std::atomic<double> mA4RefTarget{440.0};

  std::array<double, kMedianSize> mMedianBuf{};
  int mMedianIdx = 0;

  double mLastPitch = 0.0;
  double mLastCents = 0.0;

  Result mResult;
  AnalysisProfiler mProfiler;
};

} // namespace TunerAnalysis

// Inline impl file -- everything below is part of the public header so the
// host-free smoke test (and the iPlugEffect example build, which never sees
// a separate TunerAnalysis.cpp) can link without a separate TU.
#include "TunerAnalysis_impl.h"
