/*
  ==============================================================================
    SmearCard — Phase-randomisation effect ported from DtBlkFx SmearFx.
    Each FFT bin is multiplied by a linear interpolation between the original
    complex value (smear=0) and a unit-circle random complex number (smear=1),
    scaled by the amplitude multiplier.  Matches the original formula:
        *x = amp * (*x) * (rand_unit * smear  +  (1 - smear))
  ==============================================================================
*/

#pragma once

#include "SpectralCard.h"
#include "SpectralMaskModel.h"
#include <atomic>
#include <cstdint>

class SmearCard final : public SpectralCard
{
public:
    struct Settings
    {
        SpectralCardCommonSettings common;

        // Amplitude multiplier parameter (normalised 0..1 → -60..+40 dB)
        float ampParam     = 0.6f;

        // Smear amount 0..1 (0 = no phase change, 1 = full randomisation)
        float smearAmount  = 0.75f;

        FrequencyBandHz searchBandHz;
    };

    SmearCard() = default;

    void setSettings  (const Settings& s);
    Settings getSettingsSnapshot() const;

    void setProcessingContext (double sampleRate, int fftSize) override;
    void setNumChannels (int numChannels) override;
    void process (juce::dsp::Complex<float>* bins, int numBins) override;

private:
    // Thread-safe state
    std::atomic<float>    ampParam_    { 0.6f };
    std::atomic<float>    smearAmount_ { 0.75f };
    std::atomic<float>    frequencyAHz_{ 0.0f };
    std::atomic<float>    frequencyBHz_{ 40000.0f };

    double    currentSampleRate_ = 0.0;
    int       currentFftSize_    = 4096;

    // 32-bit PRBS state — advanced once per complete frame (all channels processed).
    // All channels within the same frame use the same frame-start seed so stereo
    // phase rotations are identical on L and R, preserving the stereo image.
    static constexpr uint32_t kPrbsSeed = 0xACE1u;
    mutable uint32_t prbs_           = kPrbsSeed;   // advances once per full frame
    mutable uint32_t frameStartPrbs_ = kPrbsSeed;   // seed used for current frame
    mutable int      channelCallCount_ = 0;          // resets after numChannels_ calls
    int              numChannels_      = 2;

    // Small look-up table of unit-circle complex values (sincos pairs).
    // Matches the original g_sincos_table approach.
    static constexpr int kLutSize = 4096;   // matches original DtBlkFx SinCosTable<12>
    static const std::array<juce::dsp::Complex<float>, kLutSize> kSinCosLut;

    static uint32_t prbs32 (uint32_t x) noexcept
    {
        // Galois LFSR – same XOR polynomial used in the original.
        // Cast the negation operand to signed to avoid MSVC C4146 on unsigned negation.
        const uint32_t mask = static_cast<uint32_t> (-static_cast<int32_t> (x & 1u));
        return (x >> 1) ^ (mask & 0xD0000001u);
    }
};
