// =============================================================================
// TunerAnalysis.cpp - implementation of the host-free TunerAnalysis namespace.
//
// These algorithms were originally inlined in Tuner.cpp inside the
// `namespace TunerAnalysis { ... }` block (lines 102..337 of the original
// pre-refactor Tuner.cpp).  Splitting them into a dedicated TU makes them
// linkable from a stand-alone smoke test (tests/detector_smoke.cpp) without
// pulling in iPlug2 / Win32 / Cocoa headers.
//
// All math here is DSP / signal analysis -- no UI, no host glue.  Methods
// are real-time safe on the audio thread (no allocation, bounded branches,
// NaN/Inf clamp at input boundary).
// =============================================================================

#include "TunerAnalysis.h"

namespace TunerAnalysis {

void Detector::Init(int sampleRate) {
  mSampleRate = sampleRate;
  Reset();
}

void Detector::Reset() {
  mBuffer.fill(0.0);
  mWriteIdx = 0;
  mSampleCount = 0;
  mSamplesSince = 0;
  mMedianBuf.fill(0.0);
  mMedianIdx = 0;
  mLastPitch = 0.0;
  mLastCents = 0.0;
  mResult.pitchHz.store(0.0);
  mResult.cents.store(0.0);
  mResult.clarity.store(0.0);
  mResult.level.store(0.0);
  mResult.noteIndex.store(-1);
  mResult.octave.store(4);
}

void Detector::PushSample(double mono) {
  if (!std::isfinite(mono)) mono = 0.0;  // RT-3: guard NaN/Inf before writing to ring buffer
  mBuffer[mWriteIdx] = mono;
  mWriteIdx = (mWriteIdx + 1) % kBufferSize;
  if (mSampleCount < kBufferSize) mSampleCount++;

  if (mSampleCount >= kBufferSize) {
    if (++mSamplesSince >= kAnalysisStride) {
      mSamplesSince = 0;
      RunAnalysis();
    }
  }
}

double Detector::DetectYIN(double& clarityOut) {
  const int N = kBufferSize;
  const int maxLag = std::min<int>(kMaxLag, N - 2);
  const int minLag = kMinLag;

  std::array<double, kMaxLag + 1> d{};
  for (int tau = 1; tau <= maxLag; ++tau) {
    double sum = 0.0;
    for (int j = 0; j < N - maxLag; ++j) {
      const double diff = mBuffer[(mWriteIdx + j) % N]
                        - mBuffer[(mWriteIdx + j + tau) % N];
      sum += diff * diff;
    }
    d[tau] = sum;
  }

  std::array<double, kMaxLag + 1> dN{};
  dN[0] = 1.0;
  double runningSum = 0.0;
  for (int tau = 1; tau <= maxLag; ++tau) {
    runningSum += d[tau];
    dN[tau] = (runningSum > 1e-12) ? (d[tau] * tau / runningSum) : 1.0;
  }

  double lvl = mResult.level.load();
  double threshold = 0.18 - 0.08 * std::min(1.0, std::max(0.0, lvl * 4.0));

  int bestTau = -1;
  for (int tau = minLag; tau <= maxLag; ++tau) {
    if (dN[tau] < threshold) {
      while (tau + 1 <= maxLag && dN[tau + 1] < dN[tau]) ++tau;
      bestTau = tau;
      break;
    }
  }
  if (bestTau < 0) { clarityOut = 0.0; return 0.0; }

  double refined = RefineLagNeville(dN.data(), bestTau);
  if (refined < minLag || refined > maxLag) return 0.0;

  clarityOut = std::min(1.0, std::max(0.0, 1.0 - dN[(int)std::round(refined)]));
  return mSampleRate / refined;
}

double Detector::RefineLagNeville(const double* y, int bestLag) const {
  if (bestLag < 1 || bestLag > kMaxLag - 2) return (double)bestLag;
  const double w0 = 1.0 / (1.0 + std::abs(y[bestLag - 1]));
  const double w1 = 1.0 / (1.0 + std::abs(y[bestLag]));
  const double w2 = 1.0 / (1.0 + std::abs(y[bestLag + 1]));
  const double wsum = w0 + w1 + w2;
  return ((-1.0) * w0 + 0.0 * w1 + 1.0 * w2) / wsum + (double)bestLag;
}

double Detector::DetectMPM(double& clarityOut) {
  const int N = kBufferSize;
  const int maxLag = std::min<int>(kMaxLag, N / 2);
  const int minLag = kMinLag;

  std::array<double, kMaxLag + 1> nsdf{};
  for (int tau = minLag; tau <= maxLag; ++tau) {
    double num = 0.0, den1 = 0.0, den2 = 0.0;
    for (int j = 0; j < N - maxLag; ++j) {
      const double a = mBuffer[(mWriteIdx + j) % N];
      const double b = mBuffer[(mWriteIdx + j + tau) % N];
      num  += a * b;
      den1 += a * a;
      den2 += b * b;
    }
    const double den = std::sqrt(den1 * den2);
    nsdf[tau] = (den > 1e-12) ? (num / den) : 0.0;
  }

  int bestTau = -1;
  double bestVal = 0.5;
  for (int tau = minLag; tau <= maxLag; ++tau) {
    if (nsdf[tau] > bestVal) {
      bestVal = nsdf[tau];
      bestTau = tau;
    }
  }
  if (bestTau < 0) { clarityOut = 0.0; return 0.0; }

  const int t = bestTau;
  if (t > minLag && t < maxLag) {
    const double y0 = nsdf[t - 1];
    const double y1 = nsdf[t];
    const double y2 = nsdf[t + 1];
    const double denom = (y0 - 2.0 * y1 + y2);
    if (std::abs(denom) > 1e-12) {
      const double offset = 0.5 * (y0 - y2) / denom;
      clarityOut = std::min(1.0, std::max(0.0, y1));
      return mSampleRate / ((double)t + offset);
    }
  }
  clarityOut = std::min(1.0, std::max(0.0, bestVal));
  return mSampleRate / (double)bestTau;
}

double Detector::MedianFilter(double candidate) {
  if (candidate <= 0.0) return 0.0;
  mMedianBuf[mMedianIdx] = candidate;
  mMedianIdx = (mMedianIdx + 1) % kMedianSize;
  // RT-2: replace std::nth_element with a deterministic partial selection
  // sort.  For n=7 we want the 4th smallest (index kMedianSize/2=3).
  // This performs (kMedianSize/2+1) * kMedianSize = 28 comparisons worst
  // case with zero recursion -- provably real-time safe on any platform,
  // no introselect fallback, no allocator interaction.
  std::array<double, kMedianSize> copy = mMedianBuf;
  for (int i = 0; i <= kMedianSize / 2; ++i) {
    int best = i;
    for (int j = i + 1; j < kMedianSize; ++j)
      if (copy[j] < copy[best]) best = j;
    std::swap(copy[i], copy[best]);
  }
  return copy[kMedianSize / 2];
}

double Detector::SmoothPitch(double candidate) {
  if (!std::isfinite(candidate) || candidate <= 0.0) { mLastPitch = 0.0; return 0.0; }
  if (mLastPitch <= 0.0) { mLastPitch = candidate; return candidate; }
  const double ratio = candidate / mLastPitch;
  const double semitones = std::abs(12.0 * std::log2(ratio));
  double alpha;
  if (semitones > 2.0)       alpha = 0.60;
  else if (semitones > 0.5)  alpha = 0.20;
  else                       alpha = 0.01 + 0.50 * (1.0 - mSmooth);
  mLastPitch = (1.0 - alpha) * mLastPitch + alpha * candidate;
  return mLastPitch;
}

double Detector::SmoothCents(double candidate) {
  if (!std::isfinite(candidate)) { mLastCents = 0.0; return 0.0; }
  const double absCents = std::abs(candidate);
  const double alpha = 1.0 / (1.0 + 0.05 * absCents) * (1.0 - 0.95 * mSmooth);
  mLastCents = (1.0 - alpha) * mLastCents + alpha * candidate;
  return mLastCents;
}

double Detector::NoteFromPitch(double hz, int& noteIdxOut, int& octaveOut) const {
  if (hz <= 0.0) { noteIdxOut = -1; octaveOut = 4; return 0.0; }
  const double midi = 69.0 + 12.0 * std::log2(hz / mA4Ref);
  const int midiRound = (int)std::round(midi);
  noteIdxOut = ((midiRound % 12) + 12) % 12;
  octaveOut  = (midiRound / 12) - 1;
  return (midi - midiRound) * 100.0;
}

void Detector::RunAnalysis() {
  mProfiler.Begin();
  double yClarity = 0.0, mClarity = 0.0;
  const double yinHz  = DetectYIN(yClarity);
  const double mpmHz  = DetectMPM(mClarity);

  double rawPitch = 0.0;
  if (yinHz > 0.0 && mpmHz > 0.0) {
    const double ratio = yinHz / mpmHz;
    if (ratio > 0.97 && ratio < 1.03) rawPitch = yinHz;
    else rawPitch = (yClarity >= mClarity) ? yinHz : mpmHz;
  } else if (yinHz > 0.0) {
    rawPitch = yinHz;
  } else if (mpmHz > 0.0) {
    rawPitch = mpmHz;
  }

  const double lvl = mResult.level.load();
  if (lvl < 0.005 || !std::isfinite(rawPitch)) rawPitch = 0.0;

  // Glide A4 reference toward target to avoid abrupt pitch/note jump when user
  // changes A4 mid-note.  Step is 30% of remaining diff per analysis pass (~47 Hz);
  // 95% convergence takes ~7 passes ~= 150ms -- fast enough for responsiveness,
  // slow enough to avoid audible click.
  {
    const double a4Target = mA4RefTarget.load(std::memory_order_relaxed);
    const double a4Diff = a4Target - mA4Ref;
    if (std::abs(a4Diff) > 0.01)
      mA4Ref += a4Diff * 0.30;
    else
      mA4Ref = a4Target;
  }

  const double medianPitch  = MedianFilter(rawPitch);
  const double smoothedPitch = SmoothPitch(medianPitch);

  int noteIdx = -1, octave = 4;
  const double rawCents = NoteFromPitch(smoothedPitch, noteIdx, octave);
  const double smoothedCents = SmoothCents(rawCents);

  const double clarity = std::max(yClarity, mClarity);

  mResult.pitchHz.store(smoothedPitch);
  mResult.cents.store(smoothedCents);
  mResult.clarity.store(clarity);
  mResult.noteIndex.store(noteIdx);
  mResult.octave.store(octave);
  mProfiler.End();
}

} // namespace TunerAnalysis
