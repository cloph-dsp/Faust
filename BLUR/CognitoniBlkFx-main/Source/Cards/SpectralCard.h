#pragma once

#include <JuceHeader.h>

#include <atomic>

struct FrequencyBandHz
{
    float minHz = 0.0f;
    float maxHz = 40000.0f;
};

enum class SpectralHarmonicType
{
    Odd = 0,
    Even,
    Both,
    Between
};

struct SpectralCardCommonSettings
{
    bool isBypassed = false;
    float wetDry = 1.0f;
};

class SpectralCard
{
public:
    virtual ~SpectralCard() = default;

    void setCommonSettings (const SpectralCardCommonSettings& settings)
    {
        bypassed.store (settings.isBypassed, std::memory_order_relaxed);
        wetDry.store (juce::jlimit (0.0f, 1.0f, settings.wetDry), std::memory_order_relaxed);
    }

    SpectralCardCommonSettings getCommonSettingsSnapshot() const
    {
        SpectralCardCommonSettings settings;
        settings.isBypassed = bypassed.load (std::memory_order_relaxed);
        settings.wetDry = wetDry.load (std::memory_order_relaxed);
        return settings;
    }

    virtual void setProcessingContext (double sampleRate, int fftSize)
    {
        juce::ignoreUnused (sampleRate, fftSize);
    }

    /** Called once per prepareToPlay to let cards know the stereo width. */
    virtual void setNumChannels (int numChannels)
    {
        juce::ignoreUnused (numChannels);
    }

    virtual void process (juce::dsp::Complex<float>* bins, int numBins) = 0;

protected:
    float getWetDryMix() const
    {
        return wetDry.load (std::memory_order_relaxed);
    }

    bool isBypassed() const
    {
        return bypassed.load (std::memory_order_relaxed);
    }

private:
    std::atomic<bool> bypassed { false };
    std::atomic<float> wetDry { 1.0f };
};
