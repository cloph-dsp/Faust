/*
  ==============================================================================
    SmearCard.cpp
  ==============================================================================
*/

#include "SmearCard.h"

#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
// Unit-circle LUT: kSinCosLut[i] = exp(i * i*2*pi/kLutSize) as complex<float>
// Used to pick a random unit-complex multiplier given a PRBS index.
// ─────────────────────────────────────────────────────────────────────────────
const std::array<juce::dsp::Complex<float>, SmearCard::kLutSize> SmearCard::kSinCosLut = []
{
    std::array<juce::dsp::Complex<float>, kLutSize> lut {};
    for (int i = 0; i < kLutSize; ++i)
    {
        const float angle = juce::MathConstants<float>::twoPi
                            * static_cast<float> (i) / static_cast<float> (kLutSize);
        lut[static_cast<size_t> (i)] = { std::cos (angle), std::sin (angle) };
    }
    return lut;
}();

// ─────────────────────────────────────────────────────────────────────────────
namespace
{
// Amplitude multiplier from normalised 0..1 parameter: maps to -60..+40 dB.
float effectAmpMultiplier (float ampParam) noexcept
{
    const auto p = juce::jlimit (0.0f, 1.0f, ampParam);
    if (p <= 0.0f)
        return 0.0f;

    const auto db = juce::jmap (p, -60.0f, 40.0f);
    return std::pow (10.0f, db * 0.05f);
}
}

// ─────────────────────────────────────────────────────────────────────────────
void SmearCard::setSettings (const Settings& s)
{
    setCommonSettings (s.common);
    ampParam_.store    (juce::jlimit (0.0f, 1.0f, s.ampParam),    std::memory_order_relaxed);
    smearAmount_.store (juce::jlimit (0.0f, 1.0f, s.smearAmount), std::memory_order_relaxed);
    frequencyAHz_.store (juce::jmax (0.0f, s.searchBandHz.minHz), std::memory_order_relaxed);
    frequencyBHz_.store (juce::jmax (0.0f, s.searchBandHz.maxHz), std::memory_order_relaxed);
}

SmearCard::Settings SmearCard::getSettingsSnapshot() const
{
    Settings snap;
    snap.common        = getCommonSettingsSnapshot();
    snap.ampParam      = ampParam_.load    (std::memory_order_relaxed);
    snap.smearAmount   = smearAmount_.load (std::memory_order_relaxed);
    snap.searchBandHz.minHz = frequencyAHz_.load (std::memory_order_relaxed);
    snap.searchBandHz.maxHz = frequencyBHz_.load (std::memory_order_relaxed);
    return snap;
}

void SmearCard::setProcessingContext (double sampleRate, int fftSize)
{
    if (sampleRate > 0.0) currentSampleRate_ = sampleRate;
    if (fftSize > 0)      currentFftSize_    = fftSize;
}

void SmearCard::setNumChannels (int numChannels)
{
    numChannels_       = juce::jmax (1, numChannels);
    channelCallCount_  = 0;   // reset on layout change
}

// ─────────────────────────────────────────────────────────────────────────────
// process():  for each in-band bin:
//   bin *= amp * ( randUnit * smear + (1-smear) )
// At smear=0  → bin *= amp  (pure gain, no phase change)
// At smear=1  → bin *= amp * randUnit  (full phase scramble)
// ─────────────────────────────────────────────────────────────────────────────
void SmearCard::process (juce::dsp::Complex<float>* bins, int numBins)
{
    if (bins == nullptr || numBins <= 1)
        return;

    const auto snap = getSettingsSnapshot();
    if (snap.common.isBypassed)
        return;

    const auto amp   = effectAmpMultiplier (snap.ampParam);
    if (amp <= 0.0f)
        return;

    const auto smear = snap.smearAmount;

    const auto hzPerBin = static_cast<float> (currentSampleRate_
                          / static_cast<double> (currentFftSize_));
    if (hzPerBin <= 0.0f)
        return;

    // Build in-band mask based on frequency range
    std::vector<uint8_t> inBandMask;
    SpectralMaskModel::buildInBandMask (inBandMask, numBins, hzPerBin,
                                        snap.searchBandHz, /*minActiveBin=*/ 1);

    // All channels in the same frame must apply identical PRBS rotations so that
    // the stereo image is preserved.  Snapshot the frame seed on the first channel
    // call; subsequent channels reuse the same snapshot.
    if (channelCallCount_ == 0)
        frameStartPrbs_ = prbs_;

    uint32_t randState = frameStartPrbs_;

    for (int bin = 1; bin < numBins; ++bin)
    {
        if (inBandMask[static_cast<size_t> (bin)] == 0)
            continue;

        // Draw next entry from PRBS and index into LUT for random unit complex
        randState = prbs32 (randState);
        const auto lutIdx = static_cast<int> (randState & static_cast<uint32_t> (kLutSize - 1));
        const juce::dsp::Complex<float> randUnit = kSinCosLut[static_cast<size_t> (lutIdx)];

        // Blend between identity (smear=0) and random unit multiplier (smear=1)
        const juce::dsp::Complex<float> blended
        {
            randUnit.real() * smear + (1.0f - smear),
            randUnit.imag() * smear
        };

        bins[bin] = amp * bins[bin] * blended;
    }

    // Advance per-frame seed only after all channels have been processed.
    ++channelCallCount_;
    if (channelCallCount_ >= numChannels_)
    {
        channelCallCount_ = 0;
        prbs_ = prbs32 (prbs_);   // one step per complete frame for temporal evolution
    }
}
