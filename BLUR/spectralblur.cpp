#include "spectralblur.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace spectralblur {

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 6.28318530717958647692f;
constexpr int kLutSize = 4096;
const std::array<std::complex<float>, kLutSize> kSinCosLut = []
{
    std::array<std::complex<float>, kLutSize> lut {};
    for (int i = 0; i < kLutSize; ++i)
    {
        const float angle = kTwoPi * static_cast<float>(i) / static_cast<float>(kLutSize);
        lut[static_cast<size_t>(i)] = std::complex<float>(std::cos(angle), std::sin(angle));
    }
    return lut;
}();

float onePoleCoeff(double sampleRate, double timeSeconds)
{
    const double safeSampleRate = std::max(1.0, sampleRate);
    const double safeTime = std::max(1.0e-4, timeSeconds);
    return static_cast<float>(std::exp(-1.0 / (safeSampleRate * safeTime)));
}

}  // namespace

Processor::Processor()
{
    updateTargetFromParameters();
    smoothed_ = target_;
    smoothersInitialized_ = true;
}

void Processor::prepare(double sampleRate, int maximumBlockSize, int numChannels)
{
    if (sampleRate <= 0.0) {
        sampleRate = 44100.0;
    }
    if (maximumBlockSize <= 0) {
        maximumBlockSize = 64;
    }

    sampleRate_ = sampleRate;
    maximumBlockSize_ = maximumBlockSize;
    preparedChannels_ = std::max(1, numChannels);
    smoothingCoeff_ = onePoleCoeff(sampleRate_, 0.02);
    smoothingBlend_ = 1.0f - smoothingCoeff_;
    resetSmoothers();
    rebuildBuffers(preparedChannels_);
}

void Processor::reset()
{
    if (preparedChannels_ > 0) {
        resetSmoothers();
        rebuildBuffers(preparedChannels_);
    }
}

void Processor::setParameters(const Parameters& parameters)
{
    Parameters sanitized = parameters;
    sanitized.blurAmountPercent = clamp(sanitized.blurAmountPercent, 0.0f, 100.0f);
    sanitized.varianceType = std::max(
        static_cast<int>(VarianceType::Fixed),
        std::min(static_cast<int>(VarianceType::Ghost), sanitized.varianceType));
    sanitized.blurVariancePercent = clamp(sanitized.blurVariancePercent, 0.0f, 100.0f);
    sanitized.loBinCutoffPercent = clamp(sanitized.loBinCutoffPercent, 0.0f, 100.0f);
    sanitized.hiBinCutoffPercent = clamp(sanitized.hiBinCutoffPercent, sanitized.loBinCutoffPercent, 100.0f);
    sanitized.outputGainDb = clamp(sanitized.outputGainDb, -40.0f, 40.0f);
    sanitized.fftSizeIndex = 5;

    const int desiredFftSize = fftSizeFromIndex(sanitized.fftSizeIndex);
    const bool fftSizeChanged = desiredFftSize != fftSize_;

    parameters_ = sanitized;
    updateTargetFromParameters();

    if (!smoothersInitialized_) {
        resetSmoothers();
    }

    if (preparedChannels_ > 0 && fftSizeChanged) {
        rebuildBuffers(preparedChannels_);
    }
}

const Parameters& Processor::getParameters() const noexcept
{
    return parameters_;
}

int Processor::getLatencySamples() const noexcept
{
    return fftSize_;
}

int Processor::getFftSize() const noexcept
{
    return fftSize_;
}

int Processor::getHopSize() const noexcept
{
    return hopSize_;
}

void Processor::processBlock(const float* const* inputs, float* const* outputs, int numChannels, int numSamples)
{
    if (preparedChannels_ <= 0 || numSamples <= 0) {
        return;
    }

    const int activeChannels = std::min(std::max(numChannels, 0), preparedChannels_);
    if (activeChannels <= 0) {
        return;
    }

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
        // Only the output gain smoother runs per-sample; analysis
        // parameter smoothers are deferred to hop rate below.
        updateGainSmoother();

        const float outputGain = smoothed_.gainLinear;

        for (int channel = 0; channel < activeChannels; ++channel) {
            const float inputSample = (inputs != nullptr && inputs[channel] != nullptr)
                ? sanitizeSample(inputs[channel][sampleIndex])
                : 0.0f;

            ChannelState& state = states_[channel];
            const float wetSample = pullWetSample(state);

            if (outputs != nullptr && outputs[channel] != nullptr) {
                outputs[channel][sampleIndex] = sanitizeSample(wetSample * outputGain);
            }

            pushInputSample(state, inputSample);
        }

        for (int channel = activeChannels; channel < numChannels; ++channel) {
            if (outputs != nullptr && outputs[channel] != nullptr) {
                outputs[channel][sampleIndex] = 0.0f;
            }
        }

        ++totalSamplesSeen_;
        ++samplesSinceLastFrame_;

        if (samplesSinceLastFrame_ >= hopSize_) {
            samplesSinceLastFrame_ = 0;
            channelCallCount_ = 0;
            frameStartPrbs_ = prbs_;
            // Advance all analysis-domain smoothers once per hop.
            updateAnalysisSmoothers();

            if (totalSamplesSeen_ >= fftSize_) {
                const float framePeakAmplitude = computeFramePeakAmplitude();
                const float blurDepth = blurShape(computeEffectiveBlur(framePeakAmplitude));
                // VOICING PASS: "Frozen in time" — spectral evolution nearly stops at max blur.
                // Increased multipliers (32→72 magnitude, 22→48 phase) keep spectral frames
                // held much longer, creating a crystallized, massively washed-out freeze effect.
                const float magnitudeFrames = 1.0f + blurDepth * 72.0f;
                const float phaseFrames = 1.0f + blurDepth * 48.0f;
                const float magnitudeAlpha = clamp(1.0f - 1.0f / magnitudeFrames, 0.0f, 0.998f);
                const float phaseAlpha = clamp(1.0f - 1.0f / phaseFrames, 0.0f, 0.995f);
                // Reduce phase randomization at high blur to lock in phase coherence for crystalline feel
                const float phaseRandomizeMix = smoothed_.randomizePhaseBlend * std::pow(blurDepth, 1.5f);

                // Cubic taper on Lo: logarithmic-feel mapping spreads the musically dense bass/midrange
                // (0–2 kHz) across the first ~45% of knob travel (was ~30% with quadratic, ~23% linear).
                // Sweep calibration at 44100/16384 FFT:
                //   Knob 30% → (0.3)^3 = 2.7%  bins → ~595 Hz   (vs 1985 Hz with quadratic)
                //   Knob 50% → (0.5)^3 = 12.5% bins → ~2.75 kHz
                //   Knob 70% → (0.7)^3 = 34.3% bins → ~7.5 kHz
                //   Knob 100%→ 100%              → Nyquist
                // Hi remains linear — its useful range sits naturally at the top of the scale.
                const float loLinear = clamp(smoothed_.loBinCutoffPercent * 0.01f, 0.0f, 1.0f);
                const float loPercent = loLinear * loLinear * loLinear;
                const float hiPercent = clamp(smoothed_.hiBinCutoffPercent * 0.01f, 0.0f, 1.0f);
                const int loBin = static_cast<int>(std::floor(loPercent * static_cast<float>(nyquistBin_)));
                const int hiBin = std::max(loBin, static_cast<int>(std::ceil(hiPercent * static_cast<float>(nyquistBin_))));

                for (int channel = 0; channel < activeChannels; ++channel) {
                    analyseFrame(states_[channel], loBin, hiBin, magnitudeAlpha, phaseAlpha, phaseRandomizeMix, blurDepth);
                }

                prbs_ = prbs32(prbs_);
            }
        }
    }
}

void Processor::rebuildBuffers(int numChannels)
{
    fftSize_ = fftSizeFromIndex(5);
    hopSize_ = fftSize_ / 4;
    // Per-hop smoothing blend: equivalent of applying per-sample smoothingBlend_
    // exactly hopSize_ times.  smoothingCoeff_ is already set by prepare() before
    // rebuildBuffers() is called.
    hopSmoothingBlend_ = 1.0f - std::pow(smoothingCoeff_, static_cast<float>(hopSize_));
    inputHistoryMask_ = fftSize_ - 1;
    overlapAddMask_ = (fftSize_ * 8) - 1;
    positiveBinCount_ = fftSize_ / 2 + 1;
    nyquistBin_ = fftSize_ / 2;
    fftSizeNorm_ = 5.0f / 7.0f;

    const float safeSampleRate = static_cast<float>(std::max(1.0, sampleRate_));
    const float hopSamples = static_cast<float>(hopSize_);
    amplitudeAttackCoeff_ = std::exp(-hopSamples / (safeSampleRate * 0.01f));
    amplitudeReleaseCoeff_ = std::exp(-hopSamples / (safeSampleRate * 0.12f));
    expectedAdvancePerBin_ = kTwoPi * hopSamples / static_cast<float>(fftSize_);
    binToNorm_ = (nyquistBin_ > 0) ? 1.0f / static_cast<float>(nyquistBin_) : 0.0f;

    analysisWindow_.assign(fftSize_, 0.0f);
    synthesisWindow_.assign(fftSize_, 0.0f);
    for (int i = 0; i < fftSize_; ++i) {
        const float phase = kTwoPi * static_cast<float>(i) / static_cast<float>(fftSize_);
        const float hann = 0.5f - 0.5f * std::cos(phase);
        analysisWindow_[i] = hann;
        // Pre-scale by 1/fftSize_ to fold the IFFT normalisation into the
        // synthesis window — eliminates the 16384-multiply 1/N loop in fft().
        synthesisWindow_[i] = hann * (2.0f / 3.0f) / static_cast<float>(fftSize_);
    }

    fftBitReversal_.assign(fftSize_, 0);
    for (int i = 0, j = 0; i < fftSize_; ++i) {
        fftBitReversal_[static_cast<size_t>(i)] = j;
        int bit = fftSize_ >> 1;
        for (; (j & bit) != 0; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
    }

    fftStageTwiddleIncrements_.clear();
    for (int length = 2; length <= fftSize_; length <<= 1) {
        const float angle = -kTwoPi / static_cast<float>(length);
        fftStageTwiddleIncrements_.emplace_back(std::cos(angle), std::sin(angle));
    }

    states_.assign(numChannels, ChannelState{});
    const int overlapAddSize = fftSize_ * 8;

    for (int channel = 0; channel < numChannels; ++channel) {
        ChannelState& state = states_[channel];

        state.inputHistory.assign(fftSize_, 0.0f);
        state.inputWriteIndex = 0;
        state.overlapAddBuffer.assign(overlapAddSize, 0.0f);
        state.overlapAddReadIndex = 0;
        state.fftBuffer.assign(fftSize_, std::complex<float>(0.0f, 0.0f));
        state.prevAnalysisPhase.assign(positiveBinCount_, 0.0f);
        state.smoothedMagnitude.assign(positiveBinCount_, 0.0f);
        state.smoothedPhaseAdvance.assign(positiveBinCount_, 0.0f);
        state.synthPhase.assign(positiveBinCount_, 0.0f);
        state.phaseRotationAngles.assign(positiveBinCount_, 0.0f);
        state.prbsState = 0xACE1u + static_cast<unsigned>(channel * 7919);

        // We don't pre-populate phaseRotationAngles with random values anymore.
        // It's just used as per-bin persistent phase offsets if needed.
        // But actually the PRBS does that per-bin in analyseFrame.
        // For backwards compatibility of the struct memory we leave it zeroed.

        state.hasAnalysisHistory = false;
    }

    totalSamplesSeen_ = 0;
    samplesSinceLastFrame_ = 0;
    amplitudeFollower_ = 0.0f;
}

void Processor::analyseFrame(
    ChannelState& state,
    int loBin,
    int hiBin,
    float magnitudeAlpha,
    float phaseAlpha,
    float phaseRandomizeMix,
    float blurDepth)
{
    int historyIndex = state.inputWriteIndex;
    for (int i = 0; i < fftSize_; ++i) {
        state.fftBuffer[i] = std::complex<float>(state.inputHistory[historyIndex] * analysisWindow_[i], 0.0f);
        historyIndex = (historyIndex + 1) & inputHistoryMask_;
    }

    fft(state.fftBuffer, false);

    const int nyquistBin = nyquistBin_;
    const bool initializeHistory = !state.hasAnalysisHistory;

    uint32_t binPrbsState = frameStartPrbs_;

    for (int bin = 0; bin <= nyquistBin; ++bin) {
        const std::complex<float> inputBin = state.fftBuffer[bin];
        const float magnitude = std::abs(inputBin);
        const float phase = std::atan2(inputBin.imag(), inputBin.real());
        const float expectedAdvance = expectedAdvancePerBin_ * static_cast<float>(bin);
        const bool insideBlurBand = (bin >= loBin && bin <= hiBin);
        const float binNorm = static_cast<float>(bin) * binToNorm_;

        if (initializeHistory) {
            state.prevAnalysisPhase[bin] = phase;
            state.smoothedMagnitude[bin] = magnitude;
            state.smoothedPhaseAdvance[bin] = expectedAdvance;
            state.synthPhase[bin] = phase;

            if (!insideBlurBand) {
                state.fftBuffer[bin] = std::complex<float>(0.0f, 0.0f);
                continue;
            }

            if (phaseRandomizeMix > 0.0f && bin > 0 && bin < nyquistBin) {
                const float phaseGlassyWeight = 0.35f + 0.65f * binNorm;
                
                binPrbsState = prbs32(binPrbsState);
                const int lutIdx = static_cast<int>(binPrbsState & static_cast<uint32_t>(kLutSize - 1));
                const std::complex<float> randUnit = kSinCosLut[static_cast<size_t>(lutIdx)];

                // phaseGlassyWeight acts to scale down the randomization, we can blend identity vs randUnit
                const float blendWeight = phaseRandomizeMix * phaseGlassyWeight;
                const std::complex<float> blended {
                    randUnit.real() * blendWeight + (1.0f - blendWeight),
                    randUnit.imag() * blendWeight
                };

                state.fftBuffer[bin] = inputBin * blended;
            } else {
                state.fftBuffer[bin] = inputBin;
            }

            continue;
        }

        const float wrappedDelta = wrapPhase(phase - state.prevAnalysisPhase[bin] - expectedAdvance);
        const float measuredAdvance = expectedAdvance + wrappedDelta;
        state.prevAnalysisPhase[bin] = phase;

        if (!insideBlurBand) {
            state.smoothedMagnitude[bin] = magnitude;
            state.smoothedPhaseAdvance[bin] = measuredAdvance;
            state.synthPhase[bin] = phase;
            state.fftBuffer[bin] = std::complex<float>(0.0f, 0.0f);
            continue;
        }

        // Keep the low end steadier and reduce top-end agitation so the blur
        // feels more like suspended glass than rough motion.
        // VOICING: Increased low-end tightening (0.90→0.95) locks bass frequencies
        // into an even more frozen state at high blur.
        const float lowEndTightening = 0.95f + 0.05f * binNorm;
        const float localMagnitudeAlpha = clamp(magnitudeAlpha * lowEndTightening, 0.0f, 0.995f);
        const float localPhaseAlpha = clamp(phaseAlpha * lowEndTightening, 0.0f, 0.9925f);

        // Asymmetric attack/release: accumulate magnitude slowly (the blur smear effect)
        // but release more conservatively when the signal envelope falls at high blur.
        // VOICING: Increased release damping (blurDepth → blurDepth^1.2) makes blurred
        // magnitudes stick around longer, freezing decays and transient tails.
        const float releaseBlurFactor = std::pow(blurDepth, 1.2f);
        const float localReleaseAlpha = localMagnitudeAlpha * releaseBlurFactor;
        const float effectiveAlpha    = (magnitude >= state.smoothedMagnitude[bin])
                                          ? localMagnitudeAlpha
                                          : clamp(localReleaseAlpha, 0.0f, 0.995f);
        // std::max with 1e-30f prevents denormal accumulation after
        // extended silence (~25 min at 44.1kHz / 4096-sample hop).
        state.smoothedMagnitude[bin] = std::max(
            effectiveAlpha * state.smoothedMagnitude[bin] + (1.0f - effectiveAlpha) * magnitude,
            1.0e-30f);
        state.smoothedPhaseAdvance[bin] = localPhaseAlpha * state.smoothedPhaseAdvance[bin] + (1.0f - localPhaseAlpha) * measuredAdvance;
        state.synthPhase[bin] = wrapPhase(state.synthPhase[bin] + state.smoothedPhaseAdvance[bin]);

        // Steer synthPhase back toward the true analysis phase when blur is low.
        // At blurDepth=0 this locks synthPhase to 'phase' every hop, eliminating
        // all PV drift at zero blur. At blurDepth=1 the coefficient is 0 (PV runs
        // free). Prevents flanging/phasing when blur is raised from near-zero.
        if (blurDepth < 1.0f) {
            const float phaseDelta = wrapPhase(phase - state.synthPhase[bin]);
            state.synthPhase[bin] = wrapPhase(state.synthPhase[bin] + phaseDelta * (1.0f - blurDepth));
        }

        std::complex<float> blurredBin = std::polar(state.smoothedMagnitude[bin], state.synthPhase[bin]);

        if (phaseRandomizeMix > 0.0f && bin > 0 && bin < nyquistBin) {
            const float phaseGlassyWeight = 0.35f + 0.65f * binNorm;
            
            binPrbsState = prbs32(binPrbsState);
            const int lutIdx = static_cast<int>(binPrbsState & static_cast<uint32_t>(kLutSize - 1));
            const std::complex<float> randUnit = kSinCosLut[static_cast<size_t>(lutIdx)];

            const float blendWeight = phaseRandomizeMix * phaseGlassyWeight;
            const std::complex<float> blended {
                randUnit.real() * blendWeight + (1.0f - blendWeight),
                randUnit.imag() * blendWeight
            };
            
            blurredBin *= blended;
        }

        // Blend between the unmodified analysis bin and the PV output.
        // At blurDepth=0: exact pass-through regardless of any residual drift.
        // At blurDepth=1: full PV smearing.
        state.fftBuffer[bin] = inputBin + (blurredBin - inputBin) * blurDepth;
    }

    if (initializeHistory) {
        state.hasAnalysisHistory = true;
    }

    // rotatePhaseTable is removed

    state.fftBuffer[0] = std::complex<float>(state.fftBuffer[0].real(), 0.0f);
    state.fftBuffer[nyquistBin] = std::complex<float>(state.fftBuffer[nyquistBin].real(), 0.0f);

    for (int bin = 1; bin < nyquistBin; ++bin) {
        state.fftBuffer[fftSize_ - bin] = std::conj(state.fftBuffer[bin]);
    }

    fft(state.fftBuffer, true);

    const int frameWriteIndex = state.overlapAddReadIndex;
    for (int i = 0; i < fftSize_; ++i) {
        const float sample = state.fftBuffer[i].real() * synthesisWindow_[i];
        const int outputIndex = (frameWriteIndex + i) & overlapAddMask_;
        // No sanitizeSample here — the IFFT output is finite if the input is
        // finite; sanitizeSample on pullWetSample catches any issues at
        // DAW boundary without burning 16384 isfinite+fabs calls per hop.
        state.overlapAddBuffer[outputIndex] += sample;
    }
}

float Processor::computeFramePeakAmplitude()
{
    if (states_.empty() || totalSamplesSeen_ < fftSize_) {
        return 0.0f;
    }

    // hopPeak was accumulated sample-by-sample in pushInputSample;
    // reading it here is O(channels) instead of O(fftSize * channels).
    float peak = 0.0f;
    for (ChannelState& state : states_) {
        peak = std::max(peak, state.hopPeak);
        state.hopPeak = 0.0f;
    }

    const float coefficient = (peak > amplitudeFollower_) ? amplitudeAttackCoeff_ : amplitudeReleaseCoeff_;
    amplitudeFollower_ = coefficient * amplitudeFollower_ + (1.0f - coefficient) * peak;

    return clamp(amplitudeFollower_, 0.0f, 1.0f);
}

float Processor::computeEffectiveBlur(float framePeakAmplitude)
{
    const float baseBlur = clamp(smoothed_.blurAmountPercent * 0.01f, 0.0f, 1.0f);
    const float varianceDepth = clamp(smoothed_.blurVariancePercent * 0.01f, 0.0f, 1.0f);
    const float clampedPeak = clamp(framePeakAmplitude, 0.0f, 1.0f);

    float effectiveBlur = baseBlur;

    switch (static_cast<VarianceType>(parameters_.varianceType)) {
    case VarianceType::Fixed:
        break;
    case VarianceType::Track:
        // Bipolar: quiet signal pulls blur BELOW base; loud peaks push it above.
        // With the amplitude follower's 120ms release, blur blooms on each hit
        // and clears during decay — a pump that flips in sync with dynamics.
        // At Hold=100% and Blur=50%: silence → 0% blur, peak → 100% blur.
        effectiveBlur = baseBlur + (2.0f * clampedPeak - 1.0f) * varianceDepth;
        break;
    case VarianceType::Ghost:
        // Quadratic decay-fill: (1 - peak²) stays near 1.0 through moderate
        // amplitudes, only relieving at the loudest transients. Material lives
        // in a persistent spectral fog; attacks briefly cut through then the
        // cloud closes. 1.5× boost ensures the effect is fully saturated at
        // modest Hold settings without needing the knob maxed out.
        effectiveBlur = baseBlur + (1.0f - clampedPeak * clampedPeak) * varianceDepth * 1.5f;
        break;
    }

    return clamp(effectiveBlur, 0.0f, 1.0f);
}

void Processor::pushInputSample(ChannelState& state, float sample)
{
    state.inputHistory[state.inputWriteIndex] = sample;
    state.inputWriteIndex = (state.inputWriteIndex + 1) & inputHistoryMask_;
    // Track hop-window peak for computeFramePeakAmplitude — avoids an
    // O(fftSize) history scan every hop.
    const float absSample = std::fabs(sample);
    if (absSample > state.hopPeak) {
        state.hopPeak = absSample;
    }
}

float Processor::pullWetSample(ChannelState& state)
{
    const float sample = state.overlapAddBuffer[state.overlapAddReadIndex];
    state.overlapAddBuffer[state.overlapAddReadIndex] = 0.0f;
    state.overlapAddReadIndex = (state.overlapAddReadIndex + 1) & overlapAddMask_;
    return sample;
}

void Processor::resetSmoothers()
{
    smoothed_ = target_;
    smoothersInitialized_ = true;
}

// Analysis-domain smoothers — called once per hop (~10.77 fps at 44.1kHz/4096).
void Processor::updateAnalysisSmoothers()
{
    if (!smoothersInitialized_) {
        resetSmoothers();
        return;
    }

    smoothed_.blurAmountPercent = clamp(
        smoothed_.blurAmountPercent + (target_.blurAmountPercent - smoothed_.blurAmountPercent) * hopSmoothingBlend_,
        0.0f,
        100.0f);
    smoothed_.blurVariancePercent = clamp(
        smoothed_.blurVariancePercent + (target_.blurVariancePercent - smoothed_.blurVariancePercent) * hopSmoothingBlend_,
        0.0f,
        100.0f);
    smoothed_.loBinCutoffPercent = clamp(
        smoothed_.loBinCutoffPercent + (target_.loBinCutoffPercent - smoothed_.loBinCutoffPercent) * hopSmoothingBlend_,
        0.0f,
        100.0f);
    smoothed_.hiBinCutoffPercent = clamp(
        smoothed_.hiBinCutoffPercent + (target_.hiBinCutoffPercent - smoothed_.hiBinCutoffPercent) * hopSmoothingBlend_,
        smoothed_.loBinCutoffPercent,
        100.0f);
    smoothed_.randomizePhaseBlend = clamp(
        smoothed_.randomizePhaseBlend + (target_.randomizePhaseBlend - smoothed_.randomizePhaseBlend) * hopSmoothingBlend_,
        0.0f,
        1.0f);
}

// Gain smoother only — called per-sample to avoid output zipper noise.
void Processor::updateGainSmoother()
{
    if (!smoothersInitialized_) {
        resetSmoothers();
        return;
    }
    smoothed_.gainLinear = std::max(0.0f, smoothed_.gainLinear + (target_.gainLinear - smoothed_.gainLinear) * smoothingBlend_);
}

void Processor::updateTargetFromParameters()
{
    target_.blurAmountPercent = clamp(parameters_.blurAmountPercent, 0.0f, 100.0f);
    target_.blurVariancePercent = clamp(parameters_.blurVariancePercent, 0.0f, 100.0f);
    target_.loBinCutoffPercent = clamp(parameters_.loBinCutoffPercent, 0.0f, 100.0f);
    target_.hiBinCutoffPercent = clamp(parameters_.hiBinCutoffPercent, target_.loBinCutoffPercent, 100.0f);
    target_.randomizePhaseBlend = parameters_.randomizePhases ? 1.0f : 0.0f;
    target_.gainLinear = dbToLinear(parameters_.outputGainDb);
}

void Processor::rotatePhaseTable(ChannelState& state, int loBin, int hiBin)
{
    // Function obsolete; left stub for compatibility if declared
}

void Processor::fft(std::vector<std::complex<float>>& values, bool inverse)
{
    const int size = static_cast<int>(values.size());

    for (int i = 0; i < size; ++i) {
        const int j = fftBitReversal_[static_cast<size_t>(i)];
        if (i < j) {
            std::swap(values[static_cast<size_t>(i)], values[static_cast<size_t>(j)]);
        }
    }

    for (int stage = 0, length = 2; length <= size; ++stage, length <<= 1) {
        const std::complex<float> twiddleIncrement = inverse
            ? std::conj(fftStageTwiddleIncrements_[static_cast<size_t>(stage)])
            : fftStageTwiddleIncrements_[static_cast<size_t>(stage)];

        for (int blockStart = 0; blockStart < size; blockStart += length) {
            std::complex<float> twiddle(1.0f, 0.0f);
            const int halfLength = length / 2;
            for (int offset = 0; offset < halfLength; ++offset) {
                const std::complex<float> even = values[blockStart + offset];
                const std::complex<float> odd = values[blockStart + offset + halfLength] * twiddle;
                values[blockStart + offset] = even + odd;
                values[blockStart + offset + halfLength] = even - odd;
                twiddle *= twiddleIncrement;
            }
        }
    }

    // 1/N normalisation has been folded into the synthesis window at init
    // time (synthesisWindow_ *= 1/fftSize_), so the IFFT loop is
    // unconditionally omitted here — saves 16384 float multiplies per hop.
}

float Processor::wrapPhase(float radians)
{
    // std::remainder is O(1) and handles large values (e.g. high-bin synth phase
    // accumulations) without iterating; semantically identical to wrap-to-[-pi,pi].
    return std::remainder(radians, kTwoPi);
}

float Processor::clamp(float x, float lo, float hi)
{
    return std::min(hi, std::max(lo, x));
}

float Processor::blurShape(float normalizedBlur)
{
    // VOICING PASS: Modified curve to intensify frozen effect at high blur.
    // Smoothstep (3x²-2x³) is smooth but gentle upper-end punch.
    // Raised curve with ^1.15 power gives more aggressive transition to full
    // crystallization at max blur, while remaining smooth in lower regions.
    // Result: 50% knob → ~44% effect, 100% knob → ~100% effect (vs smoothstep's
    // more balanced progression).
    const float x = clamp(normalizedBlur, 0.0f, 1.0f);
    const float smoothed = x * x * (3.0f - 2.0f * x);
    return std::pow(smoothed, 1.15f);
}

float Processor::dbToLinear(float db)
{
    return std::pow(10.0f, db * 0.05f);
}

float Processor::sanitizeSample(float x)
{
    if (!std::isfinite(x) || std::fabs(x) < 1.0e-24f) {
        return 0.0f;
    }
    return x;
}

int Processor::fftSizeFromIndex(int fftSizeIndex)
{
    (void) fftSizeIndex;
    return kFftSizeMenu[static_cast<size_t>(kFixedFftSizeIndex)];
}

uint32_t Processor::prbs32(uint32_t x) noexcept
{
    const uint32_t mask = static_cast<uint32_t>(-static_cast<int32_t>(x & 1u));
    return (x >> 1) ^ (mask & 0xD0000001u);
}

}  // namespace spectralblur
