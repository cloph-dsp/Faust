// =============================================================================
// TunerAnalysis_impl.h - inline implementations for the host-free detector.
//
// Included at the bottom of TunerAnalysis.h so the entire algorithm is
// header-only -- the build system never has to compile a separate .cpp.
// All Detector methods are declared inline to satisfy ODR when the header
// is included in multiple translation units (e.g. the standalone smoke
// test + the iPlugEffect example build).
//
// These algorithms were originally inlined in Tuner.cpp inside the
// `namespace TunerAnalysis { ... }` block.  Splitting them out makes
// them includable from a stand-alone unit test without dragging in
// iPlug2 / Win32 / Cocoa headers.
//
// All math is DSP / signal analysis -- no UI, no host glue.  Methods are
// real-time safe on the audio thread (no allocation, bounded branches,
// NaN/Inf clamp at input boundary).
// =============================================================================

namespace TunerAnalysis {

inline void Detector::Init(int sampleRate) {
  mSampleRate = std::max(1.0, static_cast<double>(sampleRate) / kDecimationFactor);
  Reset();
}

inline void Detector::Reset() {
  mBuffer.fill(0.0);
  mWriteIdx = 0;
  mSampleCount = 0;
  mSamplesSince = 0;
  mMedianBuf.fill(0.0);
  mMedianIdx = 0;
  mLastPitch = 0.0;
  mCommittedPitch = 0.0;
  mPendingPitch = 0.0;
  mLastStableClarity = 0.0;
  mPendingFrames = 0;
  mNoPitchFrames = 0;
  mDecimationAccumulator = 0.0;
  mDecimationCount = 0;
  mResult.Publish(0.0, 0.0, 0.0, -1, 4);
  mResult.level.store(0.0);
}

inline void Detector::PushSample(double mono) {
  if (!std::isfinite(mono)) mono = 0.0;  // RT-3: guard NaN/Inf before writing to ring buffer
  // A two-sample box average is a cheap, deterministic anti-alias filter for
  // 2:1 decimation. Pitch fundamentals below 1.5 kHz remain far below the
  // 12 kHz decimated Nyquist limit, while bass notes gain one octave of lag
  // history without increasing the O(N*lag) detector cost.
  mDecimationAccumulator += mono;
  if (++mDecimationCount < kDecimationFactor) return;
  const double analysisSample = mDecimationAccumulator / kDecimationFactor;
  mDecimationAccumulator = 0.0;
  mDecimationCount = 0;

  mBuffer[mWriteIdx] = analysisSample;
  mWriteIdx = (mWriteIdx + 1) % kBufferSize;
  if (mSampleCount < kBufferSize) mSampleCount++;

  if (mSampleCount >= kBufferSize) {
    if (++mSamplesSince >= kAnalysisStride) {
      mSamplesSince = 0;
      RunAnalysis();
    }
  }
}

inline double Detector::DetectYIN(double& clarityOut) {
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

  double refined = RefineLagParabolic(dN.data(), bestTau);
  if (refined < minLag || refined > maxLag) return 0.0;

  clarityOut = std::min(1.0, std::max(0.0, 1.0 - dN[(int)std::round(refined)]));
  return mSampleRate / refined;
}

inline double Detector::RefineLagParabolic(const double* y, int bestLag) const {
  if (bestLag < 1 || bestLag > kMaxLag - 2) return (double)bestLag;
  const double y0 = y[bestLag - 1];
  const double y1 = y[bestLag];
  const double y2 = y[bestLag + 1];
  const double denom = y0 - 2.0 * y1 + y2;
  if (std::abs(denom) < 1e-12) return (double)bestLag;
  // Vertex of the local CMNDF minimum. Unlike the previous inverse-weight
  // average, this is unbiased for a locally parabolic peak/minimum.
  const double offset = std::clamp(0.5 * (y0 - y2) / denom, -0.5, 0.5);
  return (double)bestLag + offset;
}

inline double Detector::DetectMPM(double& clarityOut) {
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

  // MPM's global maximum can be a later multiple of the period (or a strong
  // harmonic). Prefer the first credible local maximum after an NSDF trough:
  // this tracks the fundamental and makes the MPM vote agree with YIN more
  // often on plucked/string-rich material.
  int bestTau = -1;
  double bestVal = 0.55;
  bool sawNegative = false;
  for (int tau = minLag + 1; tau < maxLag; ++tau) {
    if (nsdf[tau] <= 0.0) sawNegative = true;
    if (sawNegative && nsdf[tau] >= bestVal &&
        nsdf[tau] > nsdf[tau - 1] && nsdf[tau] >= nsdf[tau + 1]) {
      bestTau = tau;
      bestVal = nsdf[tau];
      break;
    }
  }
  // Some nearly sinusoidal windows never cross below zero in the searched
  // range. Keep a bounded best-peak fallback rather than reporting no pitch.
  if (bestTau < 0) {
    for (int tau = minLag; tau <= maxLag; ++tau) {
      if (nsdf[tau] > bestVal) {
        bestVal = nsdf[tau];
        bestTau = tau;
      }
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

inline double Detector::MedianFilter(double candidate) {
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

inline double Detector::SmoothPitch(double candidate) {
  if (!std::isfinite(candidate) || candidate <= 0.0) { mLastPitch = 0.0; return 0.0; }
  if (mLastPitch <= 0.0) { mLastPitch = candidate; return candidate; }
  const double semitones = std::abs(12.0 * std::log2(candidate / mLastPitch));
  const double smooth = std::clamp(mSmooth.load(std::memory_order_relaxed), 0.0, 1.0);
  // Express smoothing in seconds, not arbitrary per-analysis coefficients.
  // Log-frequency interpolation keeps a constant-cent glide across registers.
  const double dt = static_cast<double>(kAnalysisStride) / mSampleRate;
  const double tau = (semitones > 2.0) ? 0.030 : (0.010 + 0.350 * smooth * smooth);
  const double alpha = (smooth < 0.001) ? 1.0 : 1.0 - std::exp(-dt / tau);
  const double logPitch = std::log2(mLastPitch) + alpha * std::log2(candidate / mLastPitch);
  mLastPitch = std::exp2(logPitch);
  return mLastPitch;
}

inline double Detector::ChooseCandidate(double yinHz, double yinClarity,
                                        double mpmHz, double mpmClarity) const {
  if (yinHz <= 0.0) return mpmHz;
  if (mpmHz <= 0.0) return yinHz;

  const double disagreement = std::abs(12.0 * std::log2(yinHz / mpmHz));
  if (disagreement < 0.35) return (yinClarity >= mpmClarity) ? yinHz : mpmHz;

  // During disagreement, continuity is more reliable than a one-frame
  // clarity contest. This specifically rejects one-off octave/harmonic votes.
  if (mCommittedPitch > 0.0) {
    const double yinDistance = std::abs(12.0 * std::log2(yinHz / mCommittedPitch));
    const double mpmDistance = std::abs(12.0 * std::log2(mpmHz / mCommittedPitch));
    if (std::abs(yinDistance - mpmDistance) > 0.15)
      return (yinDistance < mpmDistance) ? yinHz : mpmHz;
  }
  return (yinClarity >= mpmClarity) ? yinHz : mpmHz;
}

inline double Detector::StabilizeCandidate(double candidate, double& clarityInOut) {
  constexpr int kConfirmFrames = 2;
  constexpr int kHoldFrames = 5; // ~107 ms at 48 kHz (analysis pass ~=21 ms)

  if (!std::isfinite(candidate) || candidate <= 0.0) {
    if (mCommittedPitch > 0.0 && ++mNoPitchFrames <= kHoldFrames) {
      clarityInOut = std::max(clarityInOut, mLastStableClarity * 0.70);
      return mCommittedPitch;
    }
    mCommittedPitch = 0.0;
    mPendingPitch = 0.0;
    mPendingFrames = 0;
    mNoPitchFrames = 0;
    mLastStableClarity = 0.0;
    return 0.0;
  }

  mNoPitchFrames = 0;
  if (mCommittedPitch <= 0.0) {
    mCommittedPitch = candidate;
    mLastStableClarity = clarityInOut;
    return candidate;
  }

  const double delta = std::abs(12.0 * std::log2(candidate / mCommittedPitch));
  if (delta <= 0.45) {
    mCommittedPitch = candidate;
    mPendingPitch = 0.0;
    mPendingFrames = 0;
    mLastStableClarity = clarityInOut;
    return candidate;
  }

  const bool matchesPending = mPendingPitch > 0.0 &&
    std::abs(12.0 * std::log2(candidate / mPendingPitch)) <= 0.35;
  mPendingPitch = matchesPending ? 0.5 * (mPendingPitch + candidate) : candidate;
  mPendingFrames = matchesPending ? mPendingFrames + 1 : 1;
  if (mPendingFrames >= kConfirmFrames) {
    mCommittedPitch = mPendingPitch;
    mPendingPitch = 0.0;
    mPendingFrames = 0;
    mLastStableClarity = clarityInOut;
  } else {
    clarityInOut = std::max(clarityInOut, mLastStableClarity * 0.85);
  }
  return mCommittedPitch;
}

inline double Detector::NoteFromPitch(double hz, int& noteIdxOut, int& octaveOut) const {
  if (hz <= 0.0) { noteIdxOut = -1; octaveOut = 4; return 0.0; }
  const double midi = 69.0 + 12.0 * std::log2(hz / mA4Ref);
  const int midiRound = (int)std::round(midi);
  noteIdxOut = ((midiRound % 12) + 12) % 12;
  octaveOut  = (midiRound / 12) - 1;
  return (midi - midiRound) * 100.0;
}

inline void Detector::RunAnalysis() {
  mProfiler.Begin();
  double yClarity = 0.0, mClarity = 0.0;
  const double yinHz  = DetectYIN(yClarity);
  const double mpmHz  = DetectMPM(mClarity);

  double rawPitch = ChooseCandidate(yinHz, yClarity, mpmHz, mClarity);

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

  double clarity = std::max(yClarity, mClarity);
  const double stablePitch = StabilizeCandidate(rawPitch, clarity);
  const double medianPitch  = MedianFilter(stablePitch);
  const double smoothedPitch = SmoothPitch(medianPitch);

  int noteIdx = -1, octave = 4;
  const double cents = NoteFromPitch(smoothedPitch, noteIdx, octave);
  mResult.Publish(smoothedPitch, cents, clarity, noteIdx, octave);
  mProfiler.End();
}

} // namespace TunerAnalysis
