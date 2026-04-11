#pragma once

#include <JuceHeader.h>
#include "../Cards/SpectralCard.h"

class FFTProcessor
{
public:
    // Compile-time defaults — used as fallback and initial values.
    static constexpr int defaultFftOrder = 12;
    static constexpr int defaultFftSize  = 1 << defaultFftOrder;  // 4096

    struct Settings
    {
        float overlapAmount = 0.499f;
        int   fftOrder      = defaultFftOrder;  // 8–16 (256–65536)
    };

    FFTProcessor();

    void prepare (double sampleRate, int numChannels);
    void reset();
    void setSettings (const Settings& newSettings);
    void processBlock (juce::AudioBuffer<float>& buffer,
                       const std::vector<std::unique_ptr<SpectralCard>>& cards);

    int getFftOrder() const noexcept { return currentFftOrder; }
    int getFftSize()  const noexcept { return currentFftSize; }
    // The hop size is the number of samples between successive FFT frames.
    // The true output latency of the OLA engine is  2*fftSize - hopSize.
    int getHopSize()  const noexcept { return currentHopSize; }

private:
    // -------------------------------------------------------------------------
    struct ChannelState
    {
        std::vector<float> inputRing;
        std::vector<float> outputRing;
        int inputWritePos  = 0;
        int outputReadPos  = 0;
        int outputWritePos = 0;
        int hopCounter     = 0;
        float powerScale   = 1.0f;
    };

    void rebuildFft (int newOrder);
    void processFrameForChannel (ChannelState& channelState,
                                 const std::vector<std::unique_ptr<SpectralCard>>& cards);

    double currentSampleRate    = 0.0;
    int    currentFftOrder      = defaultFftOrder;
    int    currentFftSize       = defaultFftSize;
    int    currentOutputRingSize = 3 * defaultFftSize;
    int    currentHopSize       = defaultFftSize / 2;

    std::unique_ptr<juce::dsp::FFT>         fft;
    std::vector<float>                      fftTimeDomain;
    std::vector<float>                      fftPacked;
    std::vector<juce::dsp::Complex<float>>  bins;

    std::vector<ChannelState> channels;
};
