#include "spectralblur.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace spectralblur {

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 6.28318530717958647692f;
constexpr std::array<int, 8> kFftSizeMenu = {512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};

float fftSizeToNorm(int fftSize)
{
    const float minLog2 = 9.0f;
    const float maxLog2 = 16.0f;
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
        static_cast<int>(VarianceType::None),
        std::min(static_cast<int>(VarianceType::LinkInvertedAmplitude), sanitized.varianceType));
    sanitized.blurVariancePercent = clamp(sanitized.blurVariancePercent, 0.0f, 100.0f);
    sanitized.lfoRateHz = clamp(sanitized.lfoRateHz, 0.0f, 50.0f);
    sanitized.loBinCutoffPercent = clamp(sanitized.loBinCutoffPercent, 0.0f, 100.0f);
    sanitized.hiBinCutoffPercent = clamp(sanitized.hiBinCutoffPercent, sanitized.loBinCutoffPercent, 100.0f);
    sanitized.outputGainDb = clamp(sanitized.outputGainDb, -40.0f, 40.0f);
    sanitized.fftSizeIndex = std::max(0, std::min(static_cast<int>(kFftSizeMenu.size()) - 1, sanitized.fftSizeIndex));

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
        updateSmoothedRuntime();

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

            if (totalSamplesSeen_ >= fftSize_) {
                const float framePeakAmplitude = computeFramePeakAmplitude();
                const float blurDepth = blurShape(computeEffectiveBlur(framePeakAmplitude));
                const float fftNorm = fftSizeToNorm(fftSize_);
                const float magnitudeFrames = 1.0f + blurDepth * (3.0f + 18.0f * fftNorm);
                const float phaseFrames = 1.0f + blurDepth * (1.5f + 9.0f * fftNorm);
                const float magnitudeAlpha = clamp(1.0f - 1.0f / magnitudeFrames, 0.0f, 0.995f);
                const float phaseAlpha = clamp(1.0f - 1.0f / phaseFrames, 0.0f, 0.9925f);
                const float phaseRandomizeMix = smoothed_.randomizePhaseBlend * std::sqrt(std::max(0.0f, blurDepth));

                const int positiveBinCount = fftSize_ / 2 + 1;
                const float loPercent = clamp(smoothed_.loBinCutoffPercent * 0.01f, 0.0f, 1.0f);
                const float hiPercent = clamp(smoothed_.hiBinCutoffPercent * 0.01f, loPercent, 1.0f);
                const int loBin = static_cast<int>(std::floor(loPercent * static_cast<float>(positiveBinCount - 1)));
                const int hiBin = static_cast<int>(std::ceil(hiPercent * static_cast<float>(positiveBinCount - 1)));

                for (int channel = 0; channel < activeChannels; ++channel) {
                    analyseFrame(states_[channel], loBin, hiBin, magnitudeAlpha, phaseAlpha, phaseRandomizeMix);
                }
            }
        }
    }
}

void Processor::rebuildBuffers(int numChannels)
{
    fftSize_ = fftSizeFromIndex(parameters_.fftSizeIndex);
    hopSize_ = fftSize_ / 4;

    analysisWindow_.assign(fftSize_, 0.0f);
    synthesisWindow_.assign(fftSize_, 0.0f);
    for (int i = 0; i < fftSize_; ++i) {
        const float phase = kTwoPi * static_cast<float>(i) / static_cast<float>(fftSize_);
        const float hann = 0.5f - 0.5f * std::cos(phase);
        analysisWindow_[i] = hann;
        synthesisWindow_[i] = hann * (2.0f / 3.0f);
    }

    states_.assign(numChannels, ChannelState{});
    const int positiveBinCount = fftSize_ / 2 + 1;
    const int overlapAddSize = fftSize_ * 8;

    for (int channel = 0; channel < numChannels; ++channel) {
        ChannelState& state = states_[channel];
        std::uniform_real_distribution<float> phaseDistribution(-kPi, kPi);

        state.inputHistory.assign(fftSize_, 0.0f);
        state.inputWriteIndex = 0;
        state.overlapAddBuffer.assign(overlapAddSize, 0.0f);
        state.overlapAddReadIndex = 0;
        state.fftBuffer.assign(fftSize_, std::complex<float>(0.0f, 0.0f));
        state.prevAnalysisPhase.assign(positiveBinCount, 0.0f);
        state.smoothedMagnitude.assign(positiveBinCount, 0.0f);
        state.smoothedPhaseAdvance.assign(positiveBinCount, 0.0f);
        state.synthPhase.assign(positiveBinCount, 0.0f);
        state.phaseRotationAngles.assign(positiveBinCount, 0.0f);
        state.randomGenerator.seed(0x12345u + static_cast<unsigned>(channel * 7919) + static_cast<unsigned>(fftSize_));

        for (int bin = 1; bin < positiveBinCount - 1; ++bin) {
            state.phaseRotationAngles[bin] = phaseDistribution(state.randomGenerator);
        }

        state.hasAnalysisHistory = false;
    }

    totalSamplesSeen_ = 0;
    samplesSinceLastFrame_ = 0;
    globalLfoPhase_ = 0.0f;
    randomBlurValue_ = 0.5f;
    amplitudeFollower_ = 0.0f;
    modulationRandom_.seed(0x4d595df4u + static_cast<unsigned>(fftSize_));
}

void Processor::analyseFrame(
    ChannelState& state,
    int loBin,
    int hiBin,
    float magnitudeAlpha,
    float phaseAlpha,
    float phaseRandomizeMix)
{
    int historyIndex = state.inputWriteIndex;
    for (int i = 0; i < fftSize_; ++i) {
        state.fftBuffer[i] = std::complex<float>(state.inputHistory[historyIndex] * analysisWindow_[i], 0.0f);
        historyIndex = (historyIndex + 1) % fftSize_;
    }

    fft(state.fftBuffer, false);

    const int nyquistBin = fftSize_ / 2;
    const bool initializeHistory = !state.hasAnalysisHistory;

    for (int bin = 0; bin <= nyquistBin; ++bin) {
        const std::complex<float> inputBin = state.fftBuffer[bin];
        const float magnitude = std::abs(inputBin);
        const float phase = std::atan2(inputBin.imag(), inputBin.real());
        const float expectedAdvance = kTwoPi * static_cast<float>(bin * hopSize_) / static_cast<float>(fftSize_);
        const bool insideBlurBand = (bin >= loBin && bin <= hiBin);

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
                const float phaseOffset = state.phaseRotationAngles[bin] * phaseRandomizeMix;
                state.fftBuffer[bin] = inputBin * std::polar(1.0f, phaseOffset);
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

        const float binNorm = (nyquistBin > 0) ? static_cast<float>(bin) / static_cast<float>(nyquistBin) : 0.0f;
        const float lowEndTightening = 0.82f + 0.18f * std::sqrt(binNorm);
        const float localMagnitudeAlpha = clamp(magnitudeAlpha * lowEndTightening, 0.0f, 0.995f);
        const float localPhaseAlpha = clamp(phaseAlpha * (0.78f + 0.22f * binNorm), 0.0f, 0.9925f);

        state.smoothedMagnitude[bin] = localMagnitudeAlpha * state.smoothedMagnitude[bin] + (1.0f - localMagnitudeAlpha) * magnitude;
        state.smoothedPhaseAdvance[bin] = localPhaseAlpha * state.smoothedPhaseAdvance[bin] + (1.0f - localPhaseAlpha) * measuredAdvance;
        state.synthPhase[bin] = wrapPhase(state.synthPhase[bin] + state.smoothedPhaseAdvance[bin]);

        std::complex<float> blurredBin = std::polar(state.smoothedMagnitude[bin], state.synthPhase[bin]);

        if (phaseRandomizeMix > 0.0f && bin > 0 && bin < nyquistBin) {
            const float phaseOffset = state.phaseRotationAngles[bin] * phaseRandomizeMix;
            blurredBin *= std::polar(1.0f, phaseOffset);
        }

        state.fftBuffer[bin] = blurredBin;
    }

    if (initializeHistory) {
        state.hasAnalysisHistory = true;
    }

    if (phaseRandomizeMix > 0.0f) {
        rotatePhaseTable(state, loBin, hiBin);
    }

    state.fftBuffer[0] = std::complex<float>(state.fftBuffer[0].real(), 0.0f);
    state.fftBuffer[nyquistBin] = std::complex<float>(state.fftBuffer[nyquistBin].real(), 0.0f);

    for (int bin = 1; bin < nyquistBin; ++bin) {
        state.fftBuffer[fftSize_ - bin] = std::conj(state.fftBuffer[bin]);
    }

    fft(state.fftBuffer, true);

    const int frameWriteIndex = state.overlapAddReadIndex;
    const int overlapAddSize = static_cast<int>(state.overlapAddBuffer.size());
    for (int i = 0; i < fftSize_; ++i) {
        const float sample = state.fftBuffer[i].real() * synthesisWindow_[i];
        const int outputIndex = (frameWriteIndex + i) % overlapAddSize;
        state.overlapAddBuffer[outputIndex] = sanitizeSample(state.overlapAddBuffer[outputIndex] + sample);
    }
}

float Processor::computeFramePeakAmplitude()
{
    if (states_.empty() || totalSamplesSeen_ < fftSize_) {
        return 0.0f;
    }

    float peak = 0.0f;
    for (const ChannelState& state : states_) {
        int historyIndex = state.inputWriteIndex;
        for (int i = 0; i < fftSize_; ++i) {
            peak = std::max(peak, std::fabs(state.inputHistory[historyIndex]));
            historyIndex = (historyIndex + 1) % fftSize_;
        }
    }

    const float attackCoeff = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * 0.01));
    const float releaseCoeff = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * 0.12));
    const float coefficient = (peak > amplitudeFollower_) ? attackCoeff : releaseCoeff;
    amplitudeFollower_ = coefficient * amplitudeFollower_ + (1.0f - coefficient) * peak;

    return clamp(amplitudeFollower_, 0.0f, 1.0f);
}

float Processor::computeEffectiveBlur(float framePeakAmplitude)
{
    const float baseBlur = clamp(smoothed_.blurAmountPercent * 0.01f, 0.0f, 1.0f);
    const float varianceDepth = clamp(smoothed_.blurVariancePercent * 0.01f, 0.0f, 1.0f);
    const float clampedPeak = clamp(framePeakAmplitude, 0.0f, 1.0f);
    const float lfoRateHz = clamp(smoothed_.lfoRateHz, 0.0f, 50.0f);
    const float lfoValue = std::sin(globalLfoPhase_);

    globalLfoPhase_ = wrapPhase(
        globalLfoPhase_ + kTwoPi * lfoRateHz * static_cast<float>(hopSize_) / static_cast<float>(sampleRate_));

    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    const float randomTarget = distribution(modulationRandom_);
    const float randomTau = 0.14f;
    const float randomAlpha = std::exp(-static_cast<float>(hopSize_) / static_cast<float>(sampleRate_ * randomTau));
    randomBlurValue_ = randomAlpha * randomBlurValue_ + (1.0f - randomAlpha) * randomTarget;

    float effectiveBlur = baseBlur;

    switch (static_cast<VarianceType>(parameters_.varianceType)) {
    case VarianceType::None:
        break;
    case VarianceType::Random:
        effectiveBlur = baseBlur + (2.0f * randomBlurValue_ - 1.0f) * varianceDepth;
        break;
    case VarianceType::LFO:
        effectiveBlur = baseBlur + lfoValue * varianceDepth;
        break;
    case VarianceType::LinkAmplitude:
        effectiveBlur = baseBlur + 2.0f * clampedPeak * varianceDepth;
        break;
    case VarianceType::LinkInvertedAmplitude:
        effectiveBlur = baseBlur + 2.0f * (1.0f - clampedPeak) * varianceDepth;
        break;
    }

    return clamp(effectiveBlur, 0.0f, 1.0f);
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

    smoothed_.blurAmountPercent = clamp(smoothTowards(smoothed_.blurAmountPercent, target_.blurAmountPercent), 0.0f, 100.0f);
    smoothed_.blurVariancePercent = clamp(smoothTowards(smoothed_.blurVariancePercent, target_.blurVariancePercent), 0.0f, 100.0f);
    smoothed_.lfoRateHz = clamp(smoothTowards(smoothed_.lfoRateHz, target_.lfoRateHz), 0.0f, 50.0f);
    smoothed_.loBinCutoffPercent = clamp(smoothTowards(smoothed_.loBinCutoffPercent, target_.loBinCutoffPercent), 0.0f, 100.0f);
    smoothed_.hiBinCutoffPercent = clamp(
        smoothTowards(smoothed_.hiBinCutoffPercent, target_.hiBinCutoffPercent),
        smoothed_.loBinCutoffPercent,
        100.0f);
    smoothed_.randomizePhaseBlend = clamp(smoothTowards(smoothed_.randomizePhaseBlend, target_.randomizePhaseBlend), 0.0f, 1.0f);
    smoothed_.gainLinear = std::max(0.0f, smoothTowards(smoothed_.gainLinear, target_.gainLinear));
}

void Processor::updateTargetFromParameters()
{
    target_.blurAmountPercent = clamp(parameters_.blurAmountPercent, 0.0f, 100.0f);
    target_.blurVariancePercent = clamp(parameters_.blurVariancePercent, 0.0f, 100.0f);
    target_.lfoRateHz = clamp(parameters_.lfoRateHz, 0.0f, 50.0f);
    target_.loBinCutoffPercent = clamp(parameters_.loBinCutoffPercent, 0.0f, 100.0f);
    target_.hiBinCutoffPercent = clamp(parameters_.hiBinCutoffPercent, target_.loBinCutoffPercent, 100.0f);
    target_.randomizePhaseBlend = parameters_.randomizePhases ? 1.0f : 0.0f;
    target_.gainLinear = dbToLinear(parameters_.outputGainDb);
}

void Processor::rotatePhaseTable(ChannelState& state, int loBin, int hiBin)
{
    const int startBin = std::max(1, loBin);
    const int endBin = std::min(hiBin, fftSize_ / 2 - 1);
    const int span = endBin - startBin + 1;

    if (span <= 1) {
        return;
    }

    std::uniform_int_distribution<int> prefixDistribution(1, span - 1);
    const int prefixLength = prefixDistribution(state.randomGenerator);
    auto begin = state.phaseRotationAngles.begin() + startBin;
    std::rotate(begin, begin + prefixLength, begin + span);
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

float Processor::blurShape(float normalizedBlur)
{
    const float clampedBlur = clamp(normalizedBlur, 0.0f, 1.0f);
    return 1.0f - std::pow(1.0f - clampedBlur, 3.0f);
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
    const int clampedIndex = std::max(0, std::min(static_cast<int>(kFftSizeMenu.size()) - 1, fftSizeIndex));
    return kFftSizeMenu[static_cast<size_t>(clampedIndex)];
}

}  // namespace spectralblur
