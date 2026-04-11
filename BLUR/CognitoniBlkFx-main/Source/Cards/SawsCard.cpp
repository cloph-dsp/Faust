#include "SawsCard.h"
#include "HarmonicMaskModel.h"
#include "SawsCoefficients.h"
#include "SpectralMaskModel.h"

#include <algorithm>
#include <cmath>
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

float lookupSawsCoefficient (float tablePosition, float harmonicPosition)
{
    const auto tableClamped = juce::jlimit (0.0f,
                                            static_cast<float> (SawsCoefficients::numTables - 1),
                                            tablePosition);
    const auto harmonicClamped = juce::jlimit (1.0f,
                                               static_cast<float> (SawsCoefficients::numHarmonics),
                                               harmonicPosition);

    const auto tableA = static_cast<int> (std::floor (tableClamped));
    const auto tableB = juce::jmin (SawsCoefficients::numTables - 1, tableA + 1);
    const auto tableFrac = tableClamped - static_cast<float> (tableA);

    const auto harmonicA = static_cast<int> (std::floor (harmonicClamped));
    const auto harmonicB = juce::jmin (SawsCoefficients::numHarmonics, harmonicA + 1);
    const auto harmonicFrac = harmonicClamped - static_cast<float> (harmonicA);

    const auto index = [] (int table, int harmonic)
    {
        return static_cast<size_t> ((table * SawsCoefficients::numHarmonics) + (juce::jlimit (1, SawsCoefficients::numHarmonics, harmonic) - 1));
    };

    const auto c00 = SawsCoefficients::data[index (tableA, harmonicA)];
    const auto c01 = SawsCoefficients::data[index (tableA, harmonicB)];
    const auto c10 = SawsCoefficients::data[index (tableB, harmonicA)];
    const auto c11 = SawsCoefficients::data[index (tableB, harmonicB)];

    const auto c0 = juce::jmap (harmonicFrac, c00, c01);
    const auto c1 = juce::jmap (harmonicFrac, c10, c11);
    return juce::jmap (tableFrac, c0, c1);
}

float matchPwrScale (float scale, double targetPower, double currentPower)
{
    if (targetPower <= 0.0)
        return 0.0f;

    auto s = static_cast<double> (scale);
    if (currentPower > 0.0)
        s *= std::sqrt (targetPower / currentPower);

    if (s > 1.0e30)
        return 1.0f;
    if (s < 1.0e-37)
        return 0.0f;

    return static_cast<float> (s);
}
}

void SawsCard::setSettings (const Settings& newSettings)
{
    setCommonSettings (newSettings.common);
    amount.store (juce::jlimit (0.0f, 1.0f, newSettings.amount), std::memory_order_relaxed);
    harmonicType.store (juce::jlimit (0, 3, static_cast<int> (newSettings.harmonicType)), std::memory_order_relaxed);
    frequencyAHz.store (juce::jmax (0.0f, newSettings.searchBandHz.minHz), std::memory_order_relaxed);
    frequencyBHz.store (juce::jmax (0.0f, newSettings.searchBandHz.maxHz), std::memory_order_relaxed);
}

SawsCard::Settings SawsCard::getSettingsSnapshot() const
{
    Settings snapshot;
    snapshot.common = getCommonSettingsSnapshot();
    snapshot.amount = amount.load (std::memory_order_relaxed);
    snapshot.harmonicType = static_cast<SpectralHarmonicType> (juce::jlimit (0, 3, harmonicType.load (std::memory_order_relaxed)));
    snapshot.searchBandHz.minHz = frequencyAHz.load (std::memory_order_relaxed);
    snapshot.searchBandHz.maxHz = frequencyBHz.load (std::memory_order_relaxed);
    return snapshot;
}

void SawsCard::setProcessingContext (double sampleRate, int fftSize)
{
    if (sampleRate > 0.0)
        currentSampleRate = sampleRate;

    if (fftSize > 0)
        currentFftSize = fftSize;
}

void SawsCard::process (juce::dsp::Complex<float>* bins, int numBins)
{
    if (bins == nullptr || numBins <= 2)
        return;

    const auto snapshot = getSettingsSnapshot();
    if (snapshot.common.isBypassed)
        return;

    const auto amp = effectAmpMultiplier (snapshot.common.wetDry, true);
    if (amp <= 0.0f)
        return;

    const auto amountValue = juce::jlimit (0.0f, 1.0f, snapshot.amount);
    const auto copyMode = amountValue >= 0.5f;
    const auto modeAmount = copyMode
        ? juce::jlimit (0.0f, 1.0f, (amountValue - 0.5f) * 2.0f)
        : juce::jlimit (0.0f, 1.0f, amountValue * 2.0f);

    // modeAmount=0 is valid — table[0] applies at scale/copy 0% (original: f_part=0 is processed).
    if (modeAmount < 0.0f)
        return;

    const auto hzPerBin = (currentSampleRate > 0.0 && currentFftSize > 0)
        ? static_cast<float> (currentSampleRate / static_cast<double> (currentFftSize))
        : 0.0f;

    // Default peak search range matches DtBlkFx AutoHarmMaskRun:
    // b0=1, b1=freq_fft_n/8 where freq_fft_n=fftSize=2*(numBins-1),
    // so b1 = (numBins-1)/4 (DtBlkFx FxRun1_0.cpp, AutoHarmMaskRun).
    // Original algorithm: Darrell Tam.
    const auto fundamental = HarmonicMaskModel::findPeakOrFundamentalBin (bins,
                                                                           numBins,
                                                                           1,
                                                                           juce::jlimit (1, numBins - 2, (numBins - 1) / 4),
                                                                           5.0f);

    // Copied/ported from DtBlkFx HarmMatchFx / HarmMatchProcess (Darrell Tam):
    // interpolate harmonic power table by value and scale masked harmonic
    // regions toward those target powers.
    const auto tablePosition = modeAmount * static_cast<float> (SawsCoefficients::numTables - 1);

    const auto maskGeometry = HarmonicMaskModel::buildGeometry (0.2499f,
                                                                fundamental,
                                                                numBins,
                                                                1.0f,
                                                                SawsCoefficients::numHarmonics + 2);
    if (! maskGeometry.isValid())
        return;

    std::vector<uint8_t> inBandMask;
    SpectralMaskModel::buildInBandMask (inBandMask,
                                        numBins,
                                        hzPerBin,
                                        snapshot.searchBandHz,
                                        1);

    std::vector<juce::dsp::Complex<float>> original (static_cast<size_t> (numBins));
    for (int bin = 0; bin < numBins; ++bin)
        original[static_cast<size_t> (bin)] = bins[bin];

    const auto harm0Power = juce::jmax (1.0e-12f, lookupSawsCoefficient (tablePosition, 1.0f));

    const auto fundamentalCentre = juce::jlimit (1, numBins - 1, static_cast<int> (std::lround (fundamental)));
    const auto fundamentalStart = juce::jlimit (1, numBins - 1, fundamentalCentre - maskGeometry.halfWidth);
    const auto fundamentalEnd = juce::jlimit (fundamentalStart, numBins - 1, fundamentalStart + maskGeometry.width);

    double fundamentalPwr = 0.0;
    for (int bin = fundamentalStart; bin <= fundamentalEnd; ++bin)
    {
        if (inBandMask[static_cast<size_t> (bin)] != 0)
            fundamentalPwr += std::norm (original[static_cast<size_t> (bin)]);
    }

    auto pwrScale = static_cast<float> (fundamentalPwr / static_cast<double> (harm0Power));
    if (copyMode)
        pwrScale *= (amp * amp);
    const auto origAmp = juce::jmax (0.0f, 1.0f - amp);

    auto harmonicIndexFloat = std::floor ((1.0f - maskGeometry.offset) / maskGeometry.spacing);
    auto centre = maskGeometry.offset + (maskGeometry.spacing * harmonicIndexFloat);
    auto nextBin = 1;

    while (nextBin < numBins)
    {
        auto v0 = static_cast<int> (std::lround (centre)) - maskGeometry.halfWidth;
        auto v1 = v0 + maskGeometry.width;
        if (v1 < nextBin)
        {
            centre += maskGeometry.spacing;
            harmonicIndexFloat += 1.0f;
            continue;
        }

        v0 = juce::jmax (nextBin, v0);
        v1 = juce::jmin (numBins - 1, v1);
        if (v1 < v0)
            break;

        // Port of DtBlkFx HarmMatchProcess::run: curr_harm = (long)_curr_harm (C truncation,
        // toward zero). If curr_harm < 0, run() returns immediately — window is left unchanged.
        // Use truncation here to match exactly; do NOT clamp negative indices to 1.
        const auto harmIndexInt = static_cast<int> (harmonicIndexFloat); // truncate toward zero
        if (harmIndexInt < 0)
        {
            nextBin = v1 + 1;
            centre += maskGeometry.spacing;
            harmonicIndexFloat += 1.0f;
            continue;
        }

        const auto harmonicNumber = harmIndexInt + 1; // 1-indexed: harmIndexInt=0 → harm[0]
        const auto harmonicPosition = static_cast<float> (harmonicNumber);
        // Port of DtBlkFx: target_pwr = harm_pwr * _pwr_scale — no division by harm0 here;
        // harm0 is already factored into pwrScale = fundamentalPwr / harm0Power.
        const auto coefficient = lookupSawsCoefficient (tablePosition, harmonicPosition);
        const auto targetPower = static_cast<double> (juce::jmax (0.0f, coefficient) * pwrScale);

        double segmentPower = 0.0;
        for (int bin = v0; bin <= v1; ++bin)
        {
            if (inBandMask[static_cast<size_t> (bin)] == 0)
                continue;

            segmentPower += std::norm (original[static_cast<size_t> (bin)]);
        }

        if (copyMode)
        {
            // Matches DtBlkFx: cent_offs = RndToInt(freq - curr_cent) — round the difference,
            // not the difference of the rounds (avoids ±1 bin offset when fractions cancel).
            const auto centOffset = static_cast<int> (std::lround (fundamental - centre));

            auto src0 = v0 + centOffset;
            auto src1 = v1 + centOffset;

            if (src1 >= numBins)
            {
                const auto delta = src1 - (numBins - 1);
                src1 -= delta;
                v1 -= delta;
            }
            if (src0 < 1)
            {
                const auto delta = 1 - src0;
                src0 += delta;
                v0 += delta;
            }

            if (src0 <= src1 && v0 <= v1)
            {
                // Port of DtBlkFx HarmMatchProcess copy mode:
                // scale = sqrt(harm_pwr * pwr_scale) where pwr_scale already includes
                // amp² (applied in prepare). No per-window source power normalisation.
                const auto copyScale = std::sqrt (targetPower);

                if (std::isfinite (copyScale))
                {
                    auto srcBin = src0;
                    for (int bin = v0; bin <= v1; ++bin, ++srcBin)
                    {
                        if (inBandMask[static_cast<size_t> (bin)] == 0)
                            continue;

                        const auto dry = original[static_cast<size_t> (bin)];
                        const auto src = original[static_cast<size_t> (juce::jlimit (1, numBins - 1, srcBin))];
                        bins[bin] = (dry * origAmp) + (src * static_cast<float> (copyScale));
                    }
                }
            }
        }
        else if (segmentPower > 0.0)
        {
            const auto segmentScale = origAmp + matchPwrScale (amp, targetPower, segmentPower);

            for (int bin = v0; bin <= v1; ++bin)
            {
                if (inBandMask[static_cast<size_t> (bin)] == 0)
                    continue;

                const auto dry = original[static_cast<size_t> (bin)];
                bins[bin] = dry * segmentScale;
            }
        }

        nextBin = v1 + 1;
        centre += maskGeometry.spacing;
        harmonicIndexFloat += 1.0f;
    }

}
