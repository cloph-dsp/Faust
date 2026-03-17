#pragma once

#include <complex>
#include <cstdint>
#include <random>
#include <vector>

namespace spectralblur {

enum class QualityMode : int {
    Eco = 0,
    Normal = 1,
    High = 2,
};

struct Parameters {
    float mixPercent = 50.0f;
    float smearTimePercent = 55.0f;
    float tonePercent = 0.0f;
    float motionPercent = 30.0f;
    float transientPreservePercent = 50.0f;
    float outputGainDb = 0.0f;
    int qualityMode = static_cast<int>(QualityMode::Normal);
    float bandFocusPercent = 0.0f;
    float stereoWidthPercent = 100.0f;
};

class Processor {
public:
    Processor();

    void prepare(double sampleRate, int maximumBlockSize, int numChannels);
    void reset();
    void setParameters(const Parameters& parameters);

    const Parameters& getParameters() const noexcept;
    int getLatencySamples() const noexcept;
    int getFftSize() const noexcept;
    int getHopSize() const noexcept;

    void processBlock(const float* const* inputs, float* const* outputs, int numChannels, int numSamples);

private:
    struct SmoothedRuntime {
        float mix = 0.5f;
        float smear = 0.55f;
        float tone = 0.0f;
        float motion = 0.3f;
        float transientPreserve = 0.5f;
        float bandFocus = 0.0f;
        float stereoWidth = 1.0f;
        float gainLinear = 1.0f;
    };

    struct ChannelState {
        std::vector<float> inputHistory;
        int inputWriteIndex = 0;

        std::vector<float> overlapAddBuffer;
        int overlapAddReadIndex = 0;

        std::vector<float> dryDelayBuffer;
        int dryDelayIndex = 0;

        std::vector<std::complex<float>> fftBuffer;
        std::vector<float> prevAnalysisPhase;
        std::vector<float> synthPhase;
        std::vector<float> smoothedMagnitude;
        std::vector<float> smoothedPhaseAdvance;
        std::vector<float> phaseJitter;

        std::minstd_rand randomGenerator{0x12345u};
        bool hasAnalysisHistory = false;
    };

    void rebuildBuffers(int numChannels);
    void resetSmoothers();
    void updateSmoothedRuntime();
    void analyseFrame(
        ChannelState& state,
        int loBin,
        int hiBin,
        float magnitudeAlpha,
        float phaseAlpha,
        float phaseJitterAmount,
        float tone);
    float computeFrameEnvelope();
    float computeBlurDepth(float frameEnvelope, float smear, float motion, float transientPreserve);
    void pushInputSample(ChannelState& state, float sample);
    float pullWetSample(ChannelState& state);
    float delayDrySample(ChannelState& state, float sample);

    static void fft(std::vector<std::complex<float>>& values, bool inverse);
    static float wrapPhase(float radians);
    static float clamp(float x, float lo, float hi);
    static float sanitizeSample(float x);
    static int qualityToFftSize(int qualityMode);

    Parameters parameters_;
    SmoothedRuntime smoothed_;
    SmoothedRuntime target_;
    float smoothingCoeff_ = 0.0f;
    bool smoothersInitialized_ = false;

    double sampleRate_ = 44100.0;
    int maximumBlockSize_ = 0;
    int preparedChannels_ = 0;
    int fftSize_ = 2048;
    int hopSize_ = 512;

    std::vector<float> analysisWindow_;
    float outputScale_ = 1.0f;
    std::vector<ChannelState> states_;

    int totalSamplesSeen_ = 0;
    int samplesSinceLastFrame_ = 0;
    float lfoPhase_ = 0.0f;
    float randomModValue_ = 0.5f;
    float envelopeFollower_ = 0.0f;
    std::minstd_rand modulationRandom_{0x4d595df4u};
};

}  // namespace spectralblur