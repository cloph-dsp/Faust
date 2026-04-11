#pragma once

#include <JuceHeader.h>
#include <array>

namespace CardSchema
{
    // ── Card type enum (slot content) ────────────────────────────────────────
    enum class CardType : int
    {
        empty    = 0,   // slot is inactive / unassigned
        autoHarm = 1,
        contrast = 2,
        saws     = 3,
        smear    = 4
    };

    static constexpr int numSlots    = 3;
    static constexpr int numCardTypes = 5;   // 0..4

    // ── Per-slot parameter key names (suffix appended to "slot{N}_") ─────────
    inline juce::String slotParam (int slot, const char* key)
    {
        return "slot" + juce::String (slot) + "_" + key;
    }

    inline juce::String cardTypeParam (int slot) { return slotParam (slot, "cardType"); }
    inline juce::String amountParam   (int slot) { return slotParam (slot, "amount");   }
    inline juce::String harmTypeParam (int slot) { return slotParam (slot, "harmType"); }
    inline juce::String freqAParam    (int slot) { return slotParam (slot, "freqA");    }
    inline juce::String freqBParam    (int slot) { return slotParam (slot, "freqB");    }
    inline juce::String bypassParam   (int slot) { return slotParam (slot, "bypass");   }
    inline juce::String wetDryParam   (int slot) { return slotParam (slot, "wetDry");   }

    // ── CardType metadata ─────────────────────────────────────────────────────
    inline const char* cardTypeName (CardType t) noexcept
    {
        switch (t)
        {
            case CardType::autoHarm: return "AutoHarm";
            case CardType::contrast: return "Contrast";
            case CardType::saws:     return "Saws";
            case CardType::smear:    return "Smear";
            default:                 return "Empty";
        }
    }

    inline juce::Colour accentColour (CardType t) noexcept
    {
        switch (t)
        {
            case CardType::autoHarm: return juce::Colour::fromRGB (255, 178, 100);  // peach
            case CardType::contrast: return juce::Colour::fromRGB (178, 145, 235);  // lavender
            case CardType::saws:     return juce::Colour::fromRGB (100, 215, 178);  // mint
            case CardType::smear:    return juce::Colour::fromRGB (115, 185, 245);  // sky-blue
            default:                 return juce::Colour::fromRGB (185, 180, 172);  // neutral
        }
    }

    // Whether the given card type uses the harmonic-type selector knob
    // AutoHarm uses a single continuous wetDry knob that sweeps 0%Both→100%Between
    inline bool hasHarmonicSelector (CardType /*t*/) noexcept
    {
        return false;
    }

    // Display label for the "Value" knob per card type
    inline const char* wetDryKnobLabel (CardType t) noexcept
    {
        switch (t)
        {
            case CardType::autoHarm: return "Type";
            case CardType::smear:    return "Smear";
            default:                 return "Value";
        }
    }

    // ── Legacy compatibility: old-style struct kept for preset migration ───────
    // These are no longer used for parameter IDs but kept to avoid break in
    // serialisation helper until full migration is done.
    enum class CardId : int
    {
        autoHarm = 0,
        contrast = 1,
        saws = 2
    };

    struct ParamIds
    {
        const char* amount;
        const char* type;
        const char* freqA;
        const char* freqB;
        const char* bypass;
        const char* wetDry;
    };

    enum class Key : int
    {
        amount = 0,
        type,
        freqA,
        freqB,
        bypass,
        wetDry
    };

    constexpr std::array<ParamIds, 3> all
    {
        ParamIds {
            "autoHarmTargetIntensity",
            "autoHarmType",
            "autoHarmMinFreqHz",
            "autoHarmMaxFreqHz",
            "autoHarmBypass",
            "autoHarmWetDry"
        },
        ParamIds {
            "contrastAmount",
            "contrastType",
            "contrastMinFreqHz",
            "contrastMaxFreqHz",
            "contrastBypass",
            "contrastWetDry"
        },
        ParamIds {
            "sawsAmount",
            "sawsType",
            "sawsMinFreqHz",
            "sawsMaxFreqHz",
            "sawsBypass",
            "sawsWetDry"
        }
    };

    constexpr const ParamIds& paramsFor (CardId id)
    {
        return all[static_cast<int> (id)];
    }

    constexpr const char* cardName (CardId id)
    {
        switch (id)
        {
            case CardId::autoHarm: return "AutoHarm";
            case CardId::contrast: return "Contrast";
            case CardId::saws: return "Saws";
            default: return "Card";
        }
    }

    constexpr const char* keyName (Key key)
    {
        switch (key)
        {
            case Key::amount: return "amount";
            case Key::type: return "type";
            case Key::freqA: return "freqA";
            case Key::freqB: return "freqB";
            case Key::bypass: return "bypass";
            case Key::wetDry: return "wetDry";
            default: return "unknown";
        }
    }

    inline Key keyFromName (const juce::String& key)
    {
        if (key == "amount") return Key::amount;
        if (key == "type") return Key::type;
        if (key == "freqA") return Key::freqA;
        if (key == "freqB") return Key::freqB;
        if (key == "bypass") return Key::bypass;
        return Key::wetDry;
    }

    constexpr const char* paramIdFor (CardId id, Key key)
    {
        const auto& p = paramsFor (id);
        switch (key)
        {
            case Key::amount: return p.amount;
            case Key::type: return p.type;
            case Key::freqA: return p.freqA;
            case Key::freqB: return p.freqB;
            case Key::bypass: return p.bypass;
            case Key::wetDry: return p.wetDry;
            default: return p.amount;
        }
    }
}
