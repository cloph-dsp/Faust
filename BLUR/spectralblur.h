#pragma once

#include <complex>
#include <random>
#include <vector>

namespace spectralblur {

enum class VarianceType : int {
    Fixed = 0,           // Static blur, no modulation
    Track = 1,           // Blur depth follows signal amplitude
    Ghost = 2,           // Blur depth inverts with amplitude — smears decays
};

struct Parameters {
    float blurAmountPercent = 50.0f;
    int varianceType = static_cast<int>(VarianceType::Fixed);
    float blurVariancePercent = 0.5f;
    float loBinCutoffPercent = 0.0f;
    float hiBinCutoffPercent = 100.0f;
    bool randomizePhases = true;
    int fftSizeIndex = 5;
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

        uint32_t prbsState;
        bool hasAnalysisHistory = false;
        float hopPeak = 0.0f;
    };

    void rebuildBuffers(int numChannels);
    void resetSmoothers();
    void updateAnalysisSmoothers();
    void updateGainSmoother();
    void updateTargetFromParameters();
    void analyseFrame(
        ChannelState& state,
        int loBin,
        int hiBin,
        float magnitudeAlpha,
        float phaseAlpha,
        float phaseRandomizeMix,
        float blurDepth);
    float computeFramePeakAmplitude();
    float computeEffectiveBlur(float framePeakAmplitude);
    void rotatePhaseTable(ChannelState& state, int loBin, int hiBin);
    void pushInputSample(ChannelState& state, float sample);
    float pullWetSample(ChannelState& state);

    void fft(std::vector<std::complex<float>>& values, bool inverse);
    static float wrapPhase(float radians);
    static float clamp(float x, float lo, float hi);
    static float blurShape(float normalizedBlur);
    static float dbToLinear(float db);
    static float sanitizeSample(float x);
    static int fftSizeFromIndex(int fftSizeIndex);
    static uint32_t prbs32(uint32_t x) noexcept;

    Parameters parameters_;
    SmoothedRuntime smoothed_;
    SmoothedRuntime target_;
    float smoothingCoeff_ = 0.0f;
    float smoothingBlend_ = 0.0f;     // per-sample blend (for gain smoother)
    float hopSmoothingBlend_ = 0.0f;  // per-hop  blend (for analysis smoothers)
    float amplitudeAttackCoeff_ = 0.0f;
    float amplitudeReleaseCoeff_ = 0.0f;
    float fftSizeNorm_ = 0.0f;
    float expectedAdvancePerBin_ = 0.0f;
    float binToNorm_ = 0.0f;
    bool smoothersInitialized_ = false;

    double sampleRate_ = 44100.0;
    int maximumBlockSize_ = 0;
    int preparedChannels_ = 0;
    int fftSize_ = 2048;
    int hopSize_ = 512;
    int inputHistoryMask_ = 0;
    int overlapAddMask_ = 0;
    int positiveBinCount_ = 0;
    int nyquistBin_ = 0;

    std::vector<float> analysisWindow_;
    std::vector<float> synthesisWindow_;
    std::vector<int> fftBitReversal_;
    std::vector<std::complex<float>> fftStageTwiddleIncrements_;
    std::vector<ChannelState> states_;

    uint32_t prbs_ = 0xACE1u;
    uint32_t frameStartPrbs_ = 0xACE1u;
    int channelCallCount_ = 0;

    int totalSamplesSeen_ = 0;
    int samplesSinceLastFrame_ = 0;
    float amplitudeFollower_ = 0.0f;
};

}  // namespace spectralblur