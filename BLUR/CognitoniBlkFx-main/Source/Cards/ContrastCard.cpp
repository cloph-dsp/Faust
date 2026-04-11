#include "ContrastCard.h"
#include "HarmonicMaskModel.h"
#include "SpectralMaskModel.h"

#include <cmath>
#include <limits>
#include <vector>

namespace
{
float effectAmpMultiplier (float ampParam, bool mixMode)
{
    const auto p = juce::jlimit (0.0f, 1.0f, ampParam);
    constexpr auto zeroDbParam = 0.6f;

    if (mixMode && p < zeroDbParam)
        return p * (1.0f / zeroDbParam);

    const auto db = juce::jmap (p, -60.0f, 40.0f);
    return (p <= 0.0f) ? 0.0f : std::pow (10.0f, db * 0.05f);
}

float matchPower (float amplitude, double targetPower, double currentPower)
{
    double scale = amplitude;

    if (targetPower <= 0.0)
        return 0.0f;

    if (currentPower > 0.0)
        scale *= std::sqrt (targetPower / currentPower);

    if (scale > 1.0e30)
        return 1.0f;

    if (scale < 1.0e-37)
        return 0.0f;

    return static_cast<float> (scale);
}
}

void ContrastCard::setSettings (const Settings& newSettings)
{
    setCommonSettings (newSettings.common);
    amount.store (juce::jlimit (0.0f, 1.0f, newSettings.amount), std::memory_order_relaxed);
    harmonicType.store (juce::jlimit (0, 3, static_cast<int> (newSettings.harmonicType)), std::memory_order_relaxed);
    frequencyAHz.store (juce::jmax (0.0f, newSettings.searchBandHz.minHz), std::memory_order_relaxed);
    frequencyBHz.store (juce::jmax (0.0f, newSettings.searchBandHz.maxHz), std::memory_order_relaxed);
}

ContrastCard::Settings ContrastCard::getSettingsSnapshot() const
{
    Settings snapshot;
    snapshot.common = getCommonSettingsSnapshot();
    snapshot.amount = amount.load (std::memory_order_relaxed);
    snapshot.harmonicType = static_cast<SpectralHarmonicType> (juce::jlimit (0, 3, harmonicType.load (std::memory_order_relaxed)));
    snapshot.searchBandHz.minHz = frequencyAHz.load (std::memory_order_relaxed);
    snapshot.searchBandHz.maxHz = frequencyBHz.load (std::memory_order_relaxed);
    return snapshot;
}

void ContrastCard::setProcessingContext (double sampleRate, int fftSize)
{
    if (sampleRate > 0.0)
        currentSampleRate = sampleRate;

    if (fftSize > 0)
        currentFftSize = fftSize;
}

void ContrastCard::process (juce::dsp::Complex<float>* bins, int numBins)
{
    if (bins == nullptr || numBins <= 0)
        return;

    const auto snapshot = getSettingsSnapshot();
    if (snapshot.common.isBypassed)
        return;

    const auto amp = effectAmpMultiplier (snapshot.common.wetDry, false);
    if (amp <= 0.0f)
        return;

    // Copied/ported from DtBlkFx contrast mapping behavior.
    // Original author: Darrell Tam. Public refactor lineage reference: skullzy.
    // bipolar amount maps to asymmetric spectral exponent shaping.
    auto raise = (snapshot.amount - 0.5f) * 2.0f;
    if (raise >= 0.0f)
        raise = juce::jmap (raise * raise, 0.0f, 1.0f, 0.0f, 4.0f);
    else
        raise = juce::jmap (raise * raise, 0.0f, 1.0f, 0.0f, -0.5f);

    float minValue = 1.0e-30f;
    float maxValue = 1.0e30f;
    if (raise > 0.0f)
    {
        minValue = std::exp (-80.0f / ((2.0f * raise) + 1.0f));
        maxValue = std::exp (80.0f / ((2.0f * raise) + 1.0f));
    }

    const auto binHz = (currentSampleRate > 0.0 && currentFftSize > 0)
        ? static_cast<float> (currentSampleRate / static_cast<double> (currentFftSize))
        : 0.0f;

    std::vector<uint8_t> inBandMask;
    SpectralMaskModel::buildInBandMask (inBandMask,
                                        numBins,
                                        binHz,
                                        snapshot.searchBandHz,
                                        1);

    // Contrast applies to all bins within the frequency band (no harmonic
    // filtering). Matches DtBlkFx ContrastFx which uses SplitMaskProcess,
    // gating only by freqA/freqB (DtBlkFx FxRun1_0.cpp, ContrastFx).
    // Original algorithm: Darrell Tam.

    std::vector<juce::dsp::Complex<float>> processed (static_cast<size_t> (numBins));

    int selectedBins = 0;
    double inputPower = 0.0;
    for (int bin = 1; bin < numBins - 1; ++bin)
    {
        if (inBandMask[static_cast<size_t> (bin)] == 0)
            continue;

        inputPower += std::norm (bins[bin]);
        ++selectedBins;
    }

    if (selectedBins <= 0 || inputPower <= 0.0)
        return;

    const auto normaliseIn = matchPower (1.0f, static_cast<double> (selectedBins), inputPower);
    double outputPower = 0.0;

    for (int bin = 1; bin < numBins - 1; ++bin)
    {
        if (inBandMask[static_cast<size_t> (bin)] == 0)
            continue;

        auto transformed = bins[bin] * normaliseIn;
        const auto power = std::norm (transformed);

        if (power < minValue)
            transformed = {};
        else if (power < maxValue)
        {
            const auto boundedPower = juce::jmax (power, 1.0e-30f);
            const auto scale = std::pow (boundedPower, raise);
            if (std::isfinite (scale))
                transformed *= scale;
            else
                transformed = {};
        }

        if (! std::isfinite (transformed.real()) || ! std::isfinite (transformed.imag()))
            transformed = {};

        outputPower += std::norm (transformed);
        processed[static_cast<size_t> (bin)] = transformed;
    }

    const auto normaliseOut = matchPower (amp, inputPower, outputPower);

    for (int bin = 1; bin < numBins - 1; ++bin)
    {
        if (inBandMask[static_cast<size_t> (bin)] != 0)
            bins[bin] = processed[static_cast<size_t> (bin)] * normaliseOut;
    }
}
