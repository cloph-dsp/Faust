#pragma once

#include "SpectralCard.h"
#include <atomic>

class ContrastCard final : public SpectralCard
{
public:
    struct Settings
    {
        SpectralCardCommonSettings common;
        float amount = 0.0f;
        SpectralHarmonicType harmonicType = SpectralHarmonicType::Both;
        FrequencyBandHz searchBandHz;
    };

    ContrastCard() = default;

    void setSettings (const Settings& newSettings);
    Settings getSettingsSnapshot() const;

    void setProcessingContext (double sampleRate, int fftSize) override;
    void process (juce::dsp::Complex<float>* bins, int numBins) override;

private:
    std::atomic<float> amount { 0.0f };
    std::atomic<int> harmonicType { static_cast<int> (SpectralHarmonicType::Both) };
    std::atomic<float> frequencyAHz { 0.0f };
    std::atomic<float> frequencyBHz { 40000.0f };

    double currentSampleRate = 0.0;
    int currentFftSize = 4096;
};
