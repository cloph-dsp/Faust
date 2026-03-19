#pragma once

#include <complex>
#include <random>
#include <vector>

namespace spectralblur {

enum class VarianceType : int {
    None = 0,
    Random = 1,
    LFO = 2,
    LinkAmplitude = 3,
    LinkInvertedAmplitude = 4,
};

struct Parameters {
    float blurAmountPercent = 50.0f;
    int varianceType = static_cast<int>(VarianceType::None);
    float blurVariancePercent = 0.5f;
    float lfoRateHz = 1.0f;
    float loBinCutoffPercent = 0.0f;
    float hiBinCutoffPercent = 100.0f;
    bool randomizePhases = true;
    int fftSizeIndex = 2;
    float outputGainDb = 0.0f;
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
        float blurAmountPercent = 50.0f;
        float blurVariancePercent = 0.5f;
        float lfoRateHz = 1.0f;
        float loBinCutoffPercent = 0.0f;
        float hiBinCutoffPercent = 100.0f;
        float randomizePhaseBlend = 1.0f;
        float gainLinear = 1.0f;
    };

    struct ChannelState {
        std::vector<float> inputHistory;
        int inputWriteIndex = 0;

        std::vector<float> overlapAddBuffer;
        int overlapAddReadIndex = 0;

        std::vector<std::complex<float>> fftBuffer;
        std::vector<float> prevAnalysisPhase;
        std::vector<float> smoothedMagnitude;
        std::vector<float> smoothedPhaseAdvance;
        std::vector<float> synthPhase;
        std::vector<float> phaseRotationAngles;

        std::minstd_rand randomGenerator{0x12345u};
        bool hasAnalysisHistory = false;
    };

    void rebuildBuffers(int numChannels);
    void resetSmoothers();
    void updateSmoothedRuntime();
    void updateTargetFromParameters();
    void analyseFrame(
        ChannelState& state,
        int loBin,
        int hiBin,
        float magnitudeAlpha,
        float phaseAlpha,
        float phaseRandomizeMix);
    float computeFramePeakAmplitude();
    float computeEffectiveBlur(float framePeakAmplitude);
    void rotatePhaseTable(ChannelState& state, int loBin, int hiBin);
    void pushInputSample(ChannelState& state, float sample);
    float pullWetSample(ChannelState& state);

    static void fft(std::vector<std::complex<float>>& values, bool inverse);
    static float wrapPhase(float radians);
    static float clamp(float x, float lo, float hi);
    static float blurShape(float normalizedBlur);
    static float dbToLinear(float db);
    static float sanitizeSample(float x);
    static int fftSizeFromIndex(int fftSizeIndex);

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
    std::vector<float> synthesisWindow_;
    std::vector<ChannelState> states_;

    int totalSamplesSeen_ = 0;
    int samplesSinceLastFrame_ = 0;
    float globalLfoPhase_ = 0.0f;
    float randomBlurValue_ = 0.5f;
    float amplitudeFollower_ = 0.0f;
    std::minstd_rand modulationRandom_{0x4d595df4u};
};

}  // namespace spectralblur