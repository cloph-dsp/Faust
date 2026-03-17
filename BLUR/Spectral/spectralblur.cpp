#include "spectralblur.h"

#include <algorithm>
#include <cmath>

namespace spectralblur {

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 6.28318530717958647692f;

float dbToLinear(float db)
{
    return std::pow(10.0f, db * 0.05f);
}

float fftSizeToNorm(int fftSize)
{
    const float minLog2 = 10.0f;
    const float maxLog2 = 12.0f;
    const float value = std::log2(static_cast<float>(fftSize));
    return std::min(1.0f, std::max(0.0f, (value - minLog2) / (maxLog2 - minLog2)));
}

float onePoleCoeff(double sampleRate, double timeSeconds)
{
    const double safeSampleRate = std::max(1.0, sampleRate);
    const double safeTime = std::max(1.0e-4, timeSeconds);
    return static_cast<float>(std::exp(-1.0 / (safeSampleRate * safeTime)));
}

}  // namespace

Processor::Processor()
{
    target_.mix = clamp(parameters_.mixPercent * 0.01f, 0.0f, 1.0f);
    target_.smear = clamp(parameters_.smearTimePercent * 0.01f, 0.0f, 1.0f);
    target_.tone = clamp(parameters_.tonePercent * 0.01f, -1.0f, 1.0f);
    target_.motion = clamp(parameters_.motionPercent * 0.01f, 0.0f, 1.0f);
    target_.transientPreserve = clamp(parameters_.transientPreservePercent * 0.01f, 0.0f, 1.0f);
    target_.bandFocus = clamp(parameters_.bandFocusPercent * 0.01f, 0.0f, 1.0f);
    target_.stereoWidth = clamp(parameters_.stereoWidthPercent * 0.01f, 0.0f, 2.0f);
    target_.gainLinear = dbToLinear(parameters_.outputGainDb);

    smoothed_ = target_;
    smoothersInitialized_ = true;
}

void Processor::prepare(double sampleRate, int maximumBlockSize, int numChannels)
{
    // Be permissive during host/plugin scan when rates can be temporarily invalid.
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
    sanitized.mixPercent = clamp(sanitized.mixPercent, 0.0f, 100.0f);
    sanitized.smearTimePercent = clamp(sanitized.smearTimePercent, 0.0f, 100.0f);
    sanitized.tonePercent = clamp(sanitized.tonePercent, -100.0f, 100.0f);
    sanitized.motionPercent = clamp(sanitized.motionPercent, 0.0f, 100.0f);
    sanitized.transientPreservePercent = clamp(sanitized.transientPreservePercent, 0.0f, 100.0f);
    sanitized.outputGainDb = clamp(sanitized.outputGainDb, -40.0f, 40.0f);
    sanitized.bandFocusPercent = clamp(sanitized.bandFocusPercent, 0.0f, 100.0f);
    sanitized.stereoWidthPercent = clamp(sanitized.stereoWidthPercent, 0.0f, 200.0f);

    const int minQuality = static_cast<int>(QualityMode::Eco);
    const int maxQuality = static_cast<int>(QualityMode::High);
    sanitized.qualityMode = std::max(minQuality, std::min(maxQuality, sanitized.qualityMode));

    const int desiredFftSize = qualityToFftSize(sanitized.qualityMode);
    const bool fftSizeChanged = desiredFftSize != fftSize_;

    parameters_ = sanitized;

    target_.mix = clamp(parameters_.mixPercent * 0.01f, 0.0f, 1.0f);
    target_.smear = clamp(parameters_.smearTimePercent * 0.01f, 0.0f, 1.0f);
    target_.tone = clamp(parameters_.tonePercent * 0.01f, -1.0f, 1.0f);
    target_.motion = clamp(parameters_.motionPercent * 0.01f, 0.0f, 1.0f);
    target_.transientPreserve = clamp(parameters_.transientPreservePercent * 0.01f, 0.0f, 1.0f);
    target_.bandFocus = clamp(parameters_.bandFocusPercent * 0.01f, 0.0f, 1.0f);
    target_.stereoWidth = clamp(parameters_.stereoWidthPercent * 0.01f, 0.0f, 2.0f);
    target_.gainLinear = dbToLinear(parameters_.outputGainDb);

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
        updateSmoothedRuntime();

        const float wetMix = clamp(smoothed_.mix, 0.0f, 1.0f);
        const float dryGain = std::cos(0.5f * kPi * wetMix);
        const float wetGain = std::sin(0.5f * kPi * wetMix);
        const float outputGain = smoothed_.gainLinear;

        float wetSamples[2] = {0.0f, 0.0f};
        float drySamples[2] = {0.0f, 0.0f};

        for (int channel = 0; channel < activeChannels; ++channel) {
            const float inputSample = (inputs != nullptr && inputs[channel] != nullptr)
                ? sanitizeSample(inputs[channel][sampleIndex])
                : 0.0f;

            ChannelState& state = states_[channel];
            wetSamples[channel] = pullWetSample(state);
            drySamples[channel] = delayDrySample(state, inputSample);
            pushInputSample(state, inputSample);
        }

        if (activeChannels > 1) {
            const float width = clamp(smoothed_.stereoWidth, 0.0f, 2.0f);
            const float mid = 0.5f * (wetSamples[0] + wetSamples[1]);
            const float side = 0.5f * (wetSamples[0] - wetSamples[1]) * width;
            wetSamples[0] = mid + side;
            wetSamples[1] = mid - side;
        }

        for (int channel = 0; channel < activeChannels; ++channel) {
            if (outputs != nullptr && outputs[channel] != nullptr) {
                outputs[channel][sampleIndex] = sanitizeSample((drySamples[channel] * dryGain + wetSamples[channel] * wetGain) * outputGain);
            }
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

            if (totalSamplesSeen_ >= fftSize_) {
                const float frameEnvelope = computeFrameEnvelope();
                const float blurDepth = computeBlurDepth(
                    frameEnvelope,
                    smoothed_.smear,
                    smoothed_.motion,
                    smoothed_.transientPreserve);
                const float fftNorm = fftSizeToNorm(fftSize_);
                const float magnitudeTau = 0.01f + (0.06f + 10.0f * fftNorm) * std::pow(std::max(0.001f, blurDepth), 2.0f);
                const float phaseTau = std::max(0.0035f, magnitudeTau * 0.35f);
                const float magnitudeAlpha = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * magnitudeTau));
                const float phaseAlpha = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * phaseTau));
                const float phaseJitterAmount = blurDepth * smoothed_.motion * (0.02f + 0.22f * fftNorm) * kPi;

                const int positiveBinCount = fftSize_ / 2 + 1;
                const float focus = clamp(smoothed_.bandFocus, 0.0f, 1.0f);
                const float bandWidth = 1.0f - 0.85f * focus;
                const float loPercent = clamp(0.5f - 0.5f * bandWidth, 0.0f, 1.0f);
                const float hiPercent = clamp(0.5f + 0.5f * bandWidth, 0.0f, 1.0f);
                const int loBin = static_cast<int>(std::floor(loPercent * static_cast<float>(positiveBinCount - 1)));
                const int hiBin = static_cast<int>(std::ceil(hiPercent * static_cast<float>(positiveBinCount - 1)));

                for (int channel = 0; channel < activeChannels; ++channel) {
                    analyseFrame(states_[channel], loBin, hiBin, magnitudeAlpha, phaseAlpha, phaseJitterAmount, smoothed_.tone);
                }
            }
        }
    }
}

void Processor::rebuildBuffers(int numChannels)
{
    fftSize_ = qualityToFftSize(parameters_.qualityMode);
    hopSize_ = fftSize_ / 4;

    analysisWindow_.assign(fftSize_, 0.0f);
    for (int i = 0; i < fftSize_; ++i) {
        const float phase = kTwoPi * static_cast<float>(i) / static_cast<float>(fftSize_);
        const float hann = 0.5f - 0.5f * std::cos(phase);
        analysisWindow_[i] = std::sqrt(std::max(0.0f, hann));
    }

    float overlapSum = 0.0f;
    for (int i = 0; i < fftSize_; i += hopSize_) {
        overlapSum += analysisWindow_[i] * analysisWindow_[i];
    }
    outputScale_ = (overlapSum > 0.0f) ? (1.0f / overlapSum) : 1.0f;

    states_.assign(numChannels, ChannelState{});
    const int positiveBinCount = fftSize_ / 2 + 1;
    const int overlapAddSize = fftSize_ * 8;
    const int dryDelaySize = std::max(1, fftSize_);

    for (int channel = 0; channel < numChannels; ++channel) {
        ChannelState& state = states_[channel];
        state.inputHistory.assign(fftSize_, 0.0f);
        state.inputWriteIndex = 0;
        state.overlapAddBuffer.assign(overlapAddSize, 0.0f);
        state.overlapAddReadIndex = 0;
        state.dryDelayBuffer.assign(dryDelaySize, 0.0f);
        state.dryDelayIndex = 0;
        state.fftBuffer.assign(fftSize_, std::complex<float>(0.0f, 0.0f));
        state.prevAnalysisPhase.assign(positiveBinCount, 0.0f);
        state.synthPhase.assign(positiveBinCount, 0.0f);
        state.smoothedMagnitude.assign(positiveBinCount, 0.0f);
        state.smoothedPhaseAdvance.assign(positiveBinCount, 0.0f);
        state.phaseJitter.assign(positiveBinCount, 0.0f);
        state.randomGenerator.seed(0x12345u + static_cast<unsigned>(channel * 7919) + static_cast<unsigned>(fftSize_));
        state.hasAnalysisHistory = false;
    }

    totalSamplesSeen_ = 0;
    samplesSinceLastFrame_ = 0;
    lfoPhase_ = 0.0f;
    randomModValue_ = 0.5f;
    envelopeFollower_ = 0.0f;
    modulationRandom_.seed(0x4d595df4u + static_cast<unsigned>(fftSize_));
}

void Processor::analyseFrame(
    ChannelState& state,
    int loBin,
    int hiBin,
    float magnitudeAlpha,
    float phaseAlpha,
    float phaseJitterAmount,
    float tone)
{
    int historyIndex = state.inputWriteIndex;
    for (int i = 0; i < fftSize_; ++i) {
        state.fftBuffer[i] = std::complex<float>(state.inputHistory[historyIndex] * analysisWindow_[i], 0.0f);
        historyIndex = (historyIndex + 1) % fftSize_;
    }

    fft(state.fftBuffer, false);

    const int nyquistBin = fftSize_ / 2;
    const bool initializeHistory = !state.hasAnalysisHistory;
    const float clampedTone = clamp(tone, -1.0f, 1.0f);
    std::uniform_real_distribution<float> randomPhaseDistribution(-1.0f, 1.0f);

    for (int bin = 0; bin <= nyquistBin; ++bin) {
        const std::complex<float> inputBin = state.fftBuffer[bin];
        const float magnitude = std::abs(inputBin);
        const float phase = std::atan2(inputBin.imag(), inputBin.real());
        const float expectedAdvance = kTwoPi * static_cast<float>(bin * hopSize_) / static_cast<float>(fftSize_);

        if (initializeHistory) {
            state.prevAnalysisPhase[bin] = phase;
            state.synthPhase[bin] = phase;
            state.smoothedMagnitude[bin] = magnitude;
            state.smoothedPhaseAdvance[bin] = expectedAdvance;
            state.phaseJitter[bin] = 0.0f;
            continue;
        }

        const float wrappedDelta = wrapPhase(phase - state.prevAnalysisPhase[bin] - expectedAdvance);
        const float measuredAdvance = expectedAdvance + wrappedDelta;
        state.prevAnalysisPhase[bin] = phase;

        const bool blurThisBin = (bin >= loBin && bin <= hiBin && bin > 0 && bin < nyquistBin);
        if (!blurThisBin) {
            state.synthPhase[bin] = phase;
            state.smoothedMagnitude[bin] = magnitude;
            state.smoothedPhaseAdvance[bin] = measuredAdvance;
            state.phaseJitter[bin] = 0.0f;
            state.fftBuffer[bin] = inputBin;
            continue;
        }

        const float binNorm = (nyquistBin > 0) ? static_cast<float>(bin) / static_cast<float>(nyquistBin) : 0.0f;
        const float toneTilt = clamp(1.0f + clampedTone * (2.0f * binNorm - 1.0f), 0.35f, 1.85f);
        const float localMagnitudeAlpha = clamp(std::pow(magnitudeAlpha, 1.0f / toneTilt), 0.0f, 0.999995f);
        const float localPhaseAlpha = clamp(std::pow(phaseAlpha, 1.0f / toneTilt), 0.0f, 0.999995f);

        state.smoothedMagnitude[bin] = localMagnitudeAlpha * state.smoothedMagnitude[bin] + (1.0f - localMagnitudeAlpha) * magnitude;
        state.smoothedPhaseAdvance[bin] = localPhaseAlpha * state.smoothedPhaseAdvance[bin] + (1.0f - localPhaseAlpha) * measuredAdvance;
        state.synthPhase[bin] = wrapPhase(state.synthPhase[bin] + state.smoothedPhaseAdvance[bin]);

        if (phaseJitterAmount > 0.0f) {
            const float binSpread = 0.5f + 0.5f * binNorm;
            const float targetJitter = randomPhaseDistribution(state.randomGenerator) * phaseJitterAmount * binSpread;
            state.phaseJitter[bin] = 0.96f * state.phaseJitter[bin] + 0.04f * targetJitter;
        } else {
            state.phaseJitter[bin] = 0.0f;
        }

        const float synthPhase = state.synthPhase[bin] + state.phaseJitter[bin];
        state.fftBuffer[bin] = std::polar(state.smoothedMagnitude[bin], synthPhase);
    }

    if (initializeHistory) {
        state.hasAnalysisHistory = true;
    }

    for (int bin = 1; bin < nyquistBin; ++bin) {
        state.fftBuffer[fftSize_ - bin] = std::conj(state.fftBuffer[bin]);
    }

    fft(state.fftBuffer, true);

    const int frameWriteIndex = state.overlapAddReadIndex;
    const int overlapAddSize = static_cast<int>(state.overlapAddBuffer.size());
    for (int i = 0; i < fftSize_; ++i) {
        const float sample = state.fftBuffer[i].real() * analysisWindow_[i] * outputScale_;
        const int outputIndex = (frameWriteIndex + i) % overlapAddSize;
        state.overlapAddBuffer[outputIndex] = sanitizeSample(state.overlapAddBuffer[outputIndex] + sample);
    }
}

float Processor::computeFrameEnvelope()
{
    if (states_.empty() || totalSamplesSeen_ < fftSize_) {
        return 0.0f;
    }

    double sumSquares = 0.0;
    for (const ChannelState& state : states_) {
        int historyIndex = state.inputWriteIndex;
        for (int i = 0; i < fftSize_; ++i) {
            const float sample = state.inputHistory[historyIndex];
            sumSquares += static_cast<double>(sample) * static_cast<double>(sample);
            historyIndex = (historyIndex + 1) % fftSize_;
        }
    }

    const double meanSquare = sumSquares / static_cast<double>(states_.size() * fftSize_);
    const float rms = static_cast<float>(std::sqrt(meanSquare));
    const float targetEnvelope = clamp(rms * 1.8f, 0.0f, 1.0f);

    const float attackCoeff = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * 0.01));
    const float releaseCoeff = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * 0.2));
    const float coefficient = (targetEnvelope > envelopeFollower_) ? attackCoeff : releaseCoeff;
    envelopeFollower_ = coefficient * envelopeFollower_ + (1.0f - coefficient) * targetEnvelope;

    return envelopeFollower_;
}

float Processor::computeBlurDepth(float frameEnvelope, float smear, float motion, float transientPreserve)
{
    const float smearAmount = clamp(smear, 0.0f, 1.0f);
    const float motionAmount = clamp(motion, 0.0f, 1.0f);
    const float preserveAmount = clamp(transientPreserve, 0.0f, 1.0f);

    const float lfoRateHz = 0.05f + 7.5f * motionAmount * motionAmount;
    const float lfo = 0.5f + 0.5f * std::sin(lfoPhase_);
    lfoPhase_ = wrapPhase(lfoPhase_ + kTwoPi * lfoRateHz * static_cast<float>(hopSize_) / static_cast<float>(sampleRate_));

    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    const float randomTarget = distribution(modulationRandom_);
    const float randomTau = std::max(0.03f, 0.8f / (0.2f + motionAmount));
    const float randomAlpha = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * randomTau));
    randomModValue_ = randomAlpha * randomModValue_ + (1.0f - randomAlpha) * randomTarget;

    const float motionSource = 0.55f * lfo + 0.45f * randomModValue_;
    const float transientReduction = 1.0f - preserveAmount * frameEnvelope;
    const float motionDepth = 0.35f * motionAmount;

    return clamp(smearAmount * transientReduction + (2.0f * motionSource - 1.0f) * motionDepth, 0.0f, 1.0f);
}

void Processor::pushInputSample(ChannelState& state, float sample)
{
    state.inputHistory[state.inputWriteIndex] = sanitizeSample(sample);
    state.inputWriteIndex = (state.inputWriteIndex + 1) % fftSize_;
}

float Processor::pullWetSample(ChannelState& state)
{
    const float sample = sanitizeSample(state.overlapAddBuffer[state.overlapAddReadIndex]);
    state.overlapAddBuffer[state.overlapAddReadIndex] = 0.0f;
    state.overlapAddReadIndex = (state.overlapAddReadIndex + 1) % static_cast<int>(state.overlapAddBuffer.size());
    return sample;
}

float Processor::delayDrySample(ChannelState& state, float sample)
{
    const float delayed = sanitizeSample(state.dryDelayBuffer[state.dryDelayIndex]);
    state.dryDelayBuffer[state.dryDelayIndex] = sanitizeSample(sample);
    state.dryDelayIndex = (state.dryDelayIndex + 1) % static_cast<int>(state.dryDelayBuffer.size());
    return delayed;
}

void Processor::resetSmoothers()
{
    smoothed_ = target_;
    smoothersInitialized_ = true;
}

void Processor::updateSmoothedRuntime()
{
    if (!smoothersInitialized_) {
        resetSmoothers();
    }

    const auto smoothTowards = [this](float current, float target) {
        return target + (current - target) * smoothingCoeff_;
    };

    smoothed_.mix = clamp(smoothTowards(smoothed_.mix, target_.mix), 0.0f, 1.0f);
    smoothed_.smear = clamp(smoothTowards(smoothed_.smear, target_.smear), 0.0f, 1.0f);
    smoothed_.tone = clamp(smoothTowards(smoothed_.tone, target_.tone), -1.0f, 1.0f);
    smoothed_.motion = clamp(smoothTowards(smoothed_.motion, target_.motion), 0.0f, 1.0f);
    smoothed_.transientPreserve = clamp(smoothTowards(smoothed_.transientPreserve, target_.transientPreserve), 0.0f, 1.0f);
    smoothed_.bandFocus = clamp(smoothTowards(smoothed_.bandFocus, target_.bandFocus), 0.0f, 1.0f);
    smoothed_.stereoWidth = clamp(smoothTowards(smoothed_.stereoWidth, target_.stereoWidth), 0.0f, 2.0f);
    smoothed_.gainLinear = std::max(0.0f, smoothTowards(smoothed_.gainLinear, target_.gainLinear));
}

void Processor::fft(std::vector<std::complex<float>>& values, bool inverse)
{
    const int size = static_cast<int>(values.size());

    for (int i = 1, j = 0; i < size; ++i) {
        int bit = size >> 1;
        for (; (j & bit) != 0; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(values[i], values[j]);
        }
    }

    for (int length = 2; length <= size; length <<= 1) {
        const float angle = (inverse ? 1.0f : -1.0f) * kTwoPi / static_cast<float>(length);
        const std::complex<float> twiddleIncrement(std::cos(angle), std::sin(angle));

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

    if (inverse) {
        const float inverseSize = 1.0f / static_cast<float>(size);
        for (std::complex<float>& value : values) {
            value *= inverseSize;
        }
    }
}

float Processor::wrapPhase(float radians)
{
    while (radians > kPi) {
        radians -= kTwoPi;
    }
    while (radians < -kPi) {
        radians += kTwoPi;
    }
    return radians;
}

float Processor::clamp(float x, float lo, float hi)
{
    return std::min(hi, std::max(lo, x));
}

float Processor::sanitizeSample(float x)
{
    if (!std::isfinite(x) || std::fabs(x) < 1.0e-24f) {
        return 0.0f;
    }
    return x;
}

int Processor::qualityToFftSize(int qualityMode)
{
    if (qualityMode <= static_cast<int>(QualityMode::Eco)) {
        return 1024;
    }
    if (qualityMode >= static_cast<int>(QualityMode::High)) {
        return 4096;
    }
    return 2048;
}

}  // namespace spectralblur
