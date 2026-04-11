#pragma once

#include <JuceHeader.h>

#include <cmath>
#include <cstdint>

namespace HarmonicMaskModel
{
struct PackedHarmonicValue
{
    int typePart = 0;
    float widthFraction = 0.0f;
};

struct HarmonicMaskGeometry
{
    float offset = 1.0f;
    float spacing = 1.0f;
    int halfWidth = 0;
    int width = 0;
    int minBin = 1;
    int maxBin = 1;

    bool isValid() const
    {
        return width > 0 && maxBin >= minBin;
    }
};

inline PackedHarmonicValue decodePackedValue (float value)
{
    PackedHarmonicValue decoded;
    const auto scaled = juce::jlimit (0.0f, 1.0f, value) * 4.0f;
    decoded.typePart = juce::jlimit (0, 3, static_cast<int> (std::floor (scaled)));
    decoded.widthFraction = juce::jlimit (0.0f, 0.9999f, scaled - static_cast<float> (decoded.typePart));
    return decoded;
}

inline HarmonicMaskGeometry buildGeometry (float packedValue,
                                           float fundamentalBin,
                                           int numBins,
                                           float multiplier = 1.0f,
                                           int maxHarmonicCount = 200)
{
    HarmonicMaskGeometry geometry;

    const auto scaledFundamental = fundamentalBin * multiplier;
    if (scaledFundamental < 1.0f || numBins <= 1)
        return geometry;

    const auto packed = decodePackedValue (packedValue);

    geometry.offset = scaledFundamental;
    geometry.spacing = scaledFundamental;

    switch (packed.typePart)
    {
        case 1:
            geometry.offset = scaledFundamental;
            geometry.spacing = scaledFundamental * 2.0f;
            break;

        case 2:
            geometry.offset = scaledFundamental * 2.0f;
            geometry.spacing = scaledFundamental * 2.0f;
            break;

        case 3:
            geometry.offset = scaledFundamental * 0.5f;
            geometry.spacing = scaledFundamental;
            break;

        case 0:
        default:
            geometry.offset = scaledFundamental;
            geometry.spacing = scaledFundamental;
            break;
    }

    geometry.spacing = juce::jmax (4.0f, geometry.spacing);

    const auto widthFloat = juce::jlimit (0.0f, geometry.spacing, geometry.spacing * packed.widthFraction);
    geometry.halfWidth = static_cast<int> (widthFloat) / 2;
    geometry.width = geometry.halfWidth * 2;

    geometry.maxBin = juce::jlimit (1, numBins - 1, static_cast<int> (geometry.spacing * static_cast<float> (maxHarmonicCount)));
    geometry.minBin = juce::jmax (1, static_cast<int> (geometry.offset) - geometry.halfWidth);

    return geometry;
}

template <typename Fn>
inline void forEachMaskedRange (const HarmonicMaskGeometry& geometry,
                                int lowerBin,
                                int upperBin,
                                Fn&& processRange)
{
    if (! geometry.isValid())
        return;

    const auto rangeMin = juce::jmax (lowerBin, geometry.minBin);
    const auto rangeMax = juce::jmin (upperBin, geometry.maxBin);
    if (rangeMax < rangeMin)
        return;

    auto currHarm = std::floor ((static_cast<float> (rangeMin) - geometry.offset) / geometry.spacing);
    auto currCentre = geometry.offset + (geometry.spacing * currHarm);

    auto nextUnprocessed = rangeMin;
    while (nextUnprocessed <= rangeMax)
    {
        auto v0 = static_cast<int> (std::lround (currCentre)) - geometry.halfWidth;
        auto v1 = v0 + geometry.width;

        if (v0 < nextUnprocessed)
            v0 = nextUnprocessed;
        if (v1 > rangeMax)
            v1 = rangeMax;

        if (v1 >= v0)
        {
            processRange (v0, v1);
            nextUnprocessed = v1 + 1;
        }

        currCentre += geometry.spacing;
        currHarm += 1.0f;

        if (currCentre > static_cast<float> (rangeMax + geometry.halfWidth) && nextUnprocessed <= rangeMax)
            break;
    }
}

template <typename ComplexType>
inline float estimateFractionalPeakBin (const ComplexType* bins, int centreBin)
{
    // Port of DtBlkFx EstFftBin (fftw_support.cpp). Original algorithm: Darrell Tam.

    const auto t_prev = (centreBin > 0)
                            ? bins[centreBin - 1]
                            : std::conj (bins[centreBin + 1]); // reflect at DC edge
    const auto t_cent = bins[centreBin];
    const auto t_next = bins[centreBin + 1];

    const auto d0 = t_cent - t_prev; // centre - previous
    const auto d1 = t_next - t_cent; // next - centre

    // weights: abs of outer bin * 0.4  plus  abs of difference
    const auto p0 = std::abs (t_prev) * 0.4f + std::abs (d0);
    const auto p1 = std::abs (t_next) * 0.4f + std::abs (d1);

    const auto pSum = p0 + p1;
    if (pSum <= 0.0f)
        return static_cast<float> (centreBin);

    auto s = p0 / pSum;
    s = 0.5f * (1.0f + juce::jlimit (-1.0f, 1.0f, (s - 0.5f) * 5.0f));

    const auto d0Abs = std::abs (d0);
    const auto d1Abs = std::abs (d1);

    float f0 = 0.0f;
    float f1 = 0.0f;

    // f0: uses abs(PREVIOUS bin) / abs(d0)   (original: abs(t0) where t0=fft[centre-1])
    if (d0Abs > 0.0f)
        f0 = (std::abs (t_prev) / d0Abs) * std::cos (std::arg (t_prev) - std::arg (d0));

    // f1: uses abs(CENTRE bin) / abs(d1)   (original: abs(t1) where t1=fft[centre])
    if (d1Abs > 0.0f)
        f1 = 1.0f + (std::abs (t_cent) / d1Abs) * std::cos (std::arg (t_cent) - std::arg (d1));

    return static_cast<float> (centreBin) + juce::jlimit (-0.5f, 0.5f, f0 * s + f1 * (1.0f - s));
}

template <typename ComplexType>
inline float findPeakOrFundamentalBin (const ComplexType* bins,
                                       int numBins,
                                       int minBin,
                                       int maxBin,
                                       float estimateFundamental = 1.0f)
{
    // Port of DtBlkFx PeakFindFft (fftw_support.cpp). Original algorithm: Darrell Tam.
    if (bins == nullptr || numBins <= 2)
        return static_cast<float> (juce::jmax (1, minBin));

    const auto b0 = juce::jlimit (1, numBins - 2, minBin);
    const auto b1 = juce::jlimit (b0, numBins - 2, maxBin);

    auto maxPower = -1.0f;
    auto maxBinInt = b0;

    ComplexType previous {};
    auto previousPower = 0.0f;
    const ComplexType halfA { 0.0f, 0.79f };
    const ComplexType halfB { 0.0f, -0.79f };

    for (int bin = b0; bin <= b1; ++bin)
    {
        const auto current = bins[bin];
        const auto currentPower = std::norm (current);
        const auto halfPower = std::norm ((current * halfA) + (previous * halfB));

        if (halfPower > maxPower)
        {
            maxPower = halfPower;
            maxBinInt = (previousPower > currentPower) ? (bin - 1) : bin;
        }

        if (currentPower > maxPower)
        {
            maxPower = currentPower;
            maxBinInt = bin;
        }

        previous = current;
        previousPower = currentPower;
    }

    maxBinInt = juce::jlimit (b0, b1, maxBinInt);
    auto maxBinEstimate = estimateFractionalPeakBin (bins, maxBinInt);

    auto harmonic = 1.0f;
    const auto threshold = 0.1f * maxPower;
    const auto harmonicMin = juce::jmax (b0, 3);

    for (auto harmonicIndex = 2.0f; harmonicIndex <= estimateFundamental; harmonicIndex += 1.0f)
    {
        const auto candidate = static_cast<int> (std::floor (maxBinEstimate / harmonicIndex));
        if (candidate < harmonicMin)
            break;

        const auto c0 = juce::jlimit (1, numBins - 1, candidate);
        const auto c1 = juce::jlimit (1, numBins - 1, candidate + 1);
        const auto p0 = std::norm (bins[c0]);
        const auto p1 = std::norm (bins[c1]);

        if (p0 > threshold || p1 > threshold)
            harmonic = harmonicIndex;
    }

    return juce::jlimit (1.0f, static_cast<float> (numBins - 1), maxBinEstimate / harmonic);
}
}
