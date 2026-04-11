#pragma once

#include "SpectralCard.h"

#include <cmath>
#include <cstdint>
#include <vector>

namespace SpectralMaskModel
{
inline bool isInBandHz (float frequencyHz, const FrequencyBandHz& band)
{
    const auto minHz = juce::jmax (0.0f, band.minHz);
    const auto maxHz = juce::jmax (0.0f, band.maxHz);
    const auto normalRange = (minHz <= maxHz);
    return normalRange
        ? (frequencyHz >= minHz && frequencyHz <= maxHz)
        : (frequencyHz <= maxHz || frequencyHz >= minHz);
}

inline void buildInBandMask (std::vector<uint8_t>& mask,
                             int numBins,
                             float hzPerBin,
                             const FrequencyBandHz& band,
                             int firstBin = 1)
{
    mask.assign (static_cast<size_t> (juce::jmax (0, numBins)), 0);

    if (numBins <= 0)
        return;

    if (hzPerBin <= 0.0f)
    {
        for (int bin = juce::jmax (0, firstBin); bin < numBins; ++bin)
            mask[static_cast<size_t> (bin)] = 1;
        return;
    }

    for (int bin = juce::jmax (0, firstBin); bin < numBins; ++bin)
    {
        const auto frequencyHz = static_cast<float> (bin) * hzPerBin;
        if (isInBandHz (frequencyHz, band))
            mask[static_cast<size_t> (bin)] = 1;
    }
}

inline bool matchesHarmonicType (int bin,
                                 float fundamentalBin,
                                 SpectralHarmonicType type,
                                 float betweenWidth = 0.22f)
{
    if (bin <= 0 || fundamentalBin <= 1.0f)
        return (type == SpectralHarmonicType::Both);

    const auto harmonicPosition = static_cast<float> (bin) / fundamentalBin;
    if (harmonicPosition < 0.5f)
        return false;

    const auto nearestHarmonic = juce::jmax (1, static_cast<int> (std::lround (harmonicPosition)));

    switch (type)
    {
        case SpectralHarmonicType::Odd:
            return (nearestHarmonic % 2) == 1;

        case SpectralHarmonicType::Even:
            return (nearestHarmonic % 2) == 0;

        case SpectralHarmonicType::Between:
        {
            const auto lower = std::floor (harmonicPosition);
            const auto nearestHalf = lower + 0.5f;
            return std::abs (harmonicPosition - nearestHalf) <= betweenWidth;
        }

        case SpectralHarmonicType::Both:
        default:
            return true;
    }
}
}
