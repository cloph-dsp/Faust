/*
  ==============================================================================

    CognitoniBlkFx  PluginProcessor implementation.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <array>
#include <cmath>

namespace
{
constexpr auto paramMasterWetDry = "masterWetDry";
constexpr auto paramInputGain    = "inputGain";
constexpr auto paramOutputGain   = "outputGain";
constexpr auto paramBlackLens    = "blackLens";

constexpr float dtBlkC0Hz    = 16.3516f;
constexpr float dtBlkNoteSpan = 255.0f * 0.5f;

float loadParamOr (const std::atomic<float>* p, float fallback) noexcept
{
    return (p != nullptr) ? p->load (std::memory_order_relaxed) : fallback;
}

float normalisedToDtBlkHz (float normalised, float nyquistHz) noexcept
{
    const auto v = juce::jlimit (0.0f, 1.0f, normalised);
    if (v <= 0.0f)
        return 0.0f;
    const auto hz = dtBlkC0Hz * std::pow (2.0f, v * dtBlkNoteSpan / 12.0f);
    return juce::jlimit (0.0f, juce::jmax (20.0f, nyquistHz), hz);
}

CardSchema::CardType cardTypeFromFloat (float v) noexcept
{
    const auto i = juce::jlimit (0, CardSchema::numCardTypes - 1, static_cast<int> (v + 0.5f));
    return static_cast<CardSchema::CardType> (i);
}
} // namespace

//==============================================================================
//  Preset helpers
//==============================================================================
std::vector<CognitoniBlkFxAudioProcessor::PresetDefinition>
CognitoniBlkFxAudioProcessor::createDefaultPresetDefinitions()
{
    auto makeSlot = [](int slotIndex,
                       float cardType,
                       float amount,
                       float harmType,
                       float freqA,
                       float freqB,
                       float bypass,
                       float wetDry) -> SlotPresetValues
    {
        SlotPresetValues sv;
        sv.slotIndex = slotIndex;
        auto add = [&](const juce::String& id, float val)
        {
            sv.values.push_back ({ id, val });
        };
        add (CardSchema::cardTypeParam (slotIndex), cardType);
        add (CardSchema::amountParam   (slotIndex), amount);
        add (CardSchema::harmTypeParam (slotIndex), harmType);
        add (CardSchema::freqAParam    (slotIndex), freqA);
        add (CardSchema::freqBParam    (slotIndex), freqB);
        add (CardSchema::bypassParam   (slotIndex), bypass);
        add (CardSchema::wetDryParam   (slotIndex), wetDry);
        return sv;
    };

    const float kAutoHarm = 1.0f;
    const float kContrast = 2.0f;
    const float kSmear    = 4.0f;

    std::vector<PresetDefinition> defaults
    {
        // -- Empty preset
        {
            "Empty",
            false,
            {},   // author: empty — label is hidden for Empty
            { { paramMasterWetDry, 1.0f }, { paramBlackLens, 12.0f } },
            {
                makeSlot (0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
                makeSlot (1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
                makeSlot (2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)
            }
        },

        // -- AutoHarm preset (original DtBlkFx classic: 40dB, 30% Odd, full freq range)
        {
            "Auto Harm",
            false,
            "DtBlkFx",
            { { paramMasterWetDry, 1.0f }, { paramBlackLens, 12.0f } },
            {
                // Slot 0: AutoHarm — 30% Odd packed value (0.325), amplitude=1.0 (max), freqA=0.245 (≈99Hz), freqB=1.0
                makeSlot (0, kAutoHarm, 0.325f, 0.0f, 0.245f, 1.0f, 0.0f, 1.0f),
                // Slot 1: Contrast — contrast=0.3005 (≈-40%), amp=0.598 (≈0dB), full range
                makeSlot (1, kContrast, 0.3005f, 0.0f, 0.0f, 1.0f, 0.0f, 0.598f),
                makeSlot (2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)
            }
        },

        // -- Glitchy Harmonics preset
        {
            "Glitchy Harmonics",
            false,
            "Cognitoni",
            { { paramMasterWetDry, 1.0f }, { paramBlackLens, 12.0f } },
            {
                // Slot 0: AutoHarm, Amount 0.539, FreqA 0.339, FreqB 1.0, WetDry 0.768
                makeSlot (0, kAutoHarm, 0.539f, 0.0f, 0.339f, 1.0f, 0.0f, 0.768f),
                // Slot 1: Contrast, Amount 0.407, FreqA 0.0, FreqB 1.0, WetDry 1.0
                makeSlot (1, kContrast, 0.407f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                // Slot 2: Smear, Amount 0.3, FreqA 0.0, FreqB 1.0, WetDry 0.399
                makeSlot (2, kSmear, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.399f)
            }
        },

        // -- SuperSoft preset (original DtBlkFx: Smear 0dB/100%, Contrast 50%/1dB, mid range)
        {
            "Super Soft",
            false,
            "DtBlkFx",
            { { paramMasterWetDry, 1.0f }, { paramBlackLens, 16.0f } },
            {
                // Slot 0: Smear — amp=0.6 (0dB), smear=1.0 (100%), freqB=0.94886
                makeSlot (0, kSmear, 0.6f, 0.0f, 0.0f, 0.94886f, 0.0f, 1.0f),
                // Slot 1: Contrast — contrast=0.75 (+50%), amp=0.61 (≈1dB), midrange freqA=0.38636, freqB=0.63068
                makeSlot (1, kContrast, 0.75f, 0.0f, 0.38636f, 0.63068f, 0.0f, 0.61f),
                makeSlot (2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)
            }
        },

        
    };

    return defaults;
}

const std::vector<CognitoniBlkFxAudioProcessor::PresetDefinition>&
CognitoniBlkFxAudioProcessor::getPresetDefinitions() const
{
    return presetDefinitions;
}

//==============================================================================
//  Constructor / Destructor
//==============================================================================
CognitoniBlkFxAudioProcessor::CognitoniBlkFxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
#else
        : apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    bindRuntimeParameters();
    masterWetDryParam = apvts.getRawParameterValue (paramMasterWetDry);
    inputGainParam    = apvts.getRawParameterValue (paramInputGain);
    outputGainParam   = apvts.getRawParameterValue (paramOutputGain);
    blackLensParam    = apvts.getRawParameterValue (paramBlackLens);

    presetDefinitions = createDefaultPresetDefinitions();
    loadPresetsFromJson();

    rebuildCardRack();
    applyPresetByIndex (0);
}

CognitoniBlkFxAudioProcessor::~CognitoniBlkFxAudioProcessor() {}

//==============================================================================
//  Parameter layout
//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout
CognitoniBlkFxAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve (26);

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { paramMasterWetDry, 1 }, "Master Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { paramInputGain, 1 }, "Input Gain",
        juce::NormalisableRange<float> (-18.0f, 18.0f, 0.1f), 0.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { paramOutputGain, 1 }, "Output Gain",
        juce::NormalisableRange<float> (-18.0f, 18.0f, 0.1f), 0.0f));

    // BlackLens: FFT window size stored as FFT order (8–16, integer steps).
    // Order N = 2^N samples.  Stored directly so the host parameter has no
    // threshold ambiguity and the knob snaps to exactly 9 discrete values.
    // Default order 12 ≈ 92.9 ms @ 44.1 kHz.  Max order 16 = 65536 samples
    // ≈ 1486 ms — nearest power-of-2 to the original DtBlkFx ceiling (80640
    // samples = 1829 ms).  Non-power-of-2 sizes are not supported by
    // juce::dsp::FFT, so intermediate snaps cannot be added without switching
    // the FFT library.
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { paramBlackLens, 1 }, "BlackLens",
        juce::NormalisableRange<float> (8.0f, 16.0f, 1.0f), 12.0f));

    for (int s = 0; s < CardSchema::numSlots; ++s)
    {
        const auto pre = "Slot " + juce::String (s) + " ";

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::cardTypeParam (s), 1 },
            pre + "Card Type",
            juce::NormalisableRange<float> (0.0f, static_cast<float> (CardSchema::numCardTypes - 1), 1.0f),
            0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::amountParam (s), 1 },
            pre + "Amount",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.3f));

        // Float param (0..3, step 1) so a rotary Slider can attach to it
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::harmTypeParam (s), 1 },
            pre + "Harmonic Type",
            juce::NormalisableRange<float> (0.0f, 3.0f, 1.0f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::freqAParam (s), 1 },
            pre + "Freq Low",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::freqBParam (s), 1 },
            pre + "Freq High",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));

        params.push_back (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID { CardSchema::bypassParam (s), 1 },
            pre + "Bypass", false));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { CardSchema::wetDryParam (s), 1 },
            pre + "Wet/Dry",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));
    }

    return { params.begin(), params.end() };
}

//==============================================================================
//  Runtime parameter binding
//==============================================================================

// BlackLens parameter stores the FFT order directly; just clamp and round.
static int fftOrderFromParam (float storedOrder) noexcept
{
    return juce::jlimit (8, 16, juce::roundToInt (storedOrder));
}

void CognitoniBlkFxAudioProcessor::bindRuntimeParameters()
{
    for (int s = 0; s < CardSchema::numSlots; ++s)
    {
        auto& sp = slotRuntimeParams[static_cast<size_t> (s)];
        sp.cardType = apvts.getRawParameterValue (CardSchema::cardTypeParam (s));
        sp.amount   = apvts.getRawParameterValue (CardSchema::amountParam   (s));
        sp.harmType = apvts.getRawParameterValue (CardSchema::harmTypeParam (s));
        sp.freqA    = apvts.getRawParameterValue (CardSchema::freqAParam    (s));
        sp.freqB    = apvts.getRawParameterValue (CardSchema::freqBParam    (s));
        sp.bypass   = apvts.getRawParameterValue (CardSchema::bypassParam   (s));
        sp.wetDry   = apvts.getRawParameterValue (CardSchema::wetDryParam   (s));
    }
}

//==============================================================================
//  Card rack management 
//==============================================================================
void CognitoniBlkFxAudioProcessor::rebuildCardRack()
{
    std::array<int, CardSchema::numSlots> currentTypes;
    for (int s = 0; s < CardSchema::numSlots; ++s)
        currentTypes[static_cast<size_t> (s)] =
            static_cast<int> (loadParamOr (slotRuntimeParams[static_cast<size_t> (s)].cardType, 0.0f) + 0.5f);

    bool changed = false;
    for (int s = 0; s < CardSchema::numSlots; ++s)
        if (currentTypes[static_cast<size_t> (s)] != lastBuiltSlotTypes[static_cast<size_t> (s)])
        { changed = true; break; }

    if (! changed)
        return;

    cardRack.clear();
    for (int s = 0; s < CardSchema::numSlots; ++s)
    {
        const auto t = static_cast<CardSchema::CardType> (currentTypes[static_cast<size_t> (s)]);
        lastBuiltSlotTypes[static_cast<size_t> (s)] = static_cast<int> (t);

        switch (t)
        {
            case CardSchema::CardType::autoHarm:
            {
                auto card = std::make_unique<AutoHarmCard>();
                card->setSettings (AutoHarmCard::PresetManager::getProfile (
                    AutoHarmCard::Profile::DtBlkFxClassicAutoHarm));
                cardRack.push_back (std::move (card));
                break;
            }
            case CardSchema::CardType::contrast:
                cardRack.push_back (std::make_unique<ContrastCard>());
                break;
            case CardSchema::CardType::saws:
                cardRack.push_back (std::make_unique<SawsCard>());
                break;
            case CardSchema::CardType::smear:
                cardRack.push_back (std::make_unique<SmearCard>());
                break;
            case CardSchema::CardType::empty:
            default:
                break;
        }
    }

    const auto sr = getSampleRate();
    const int numCh = juce::jmax (1, juce::jmax (getTotalNumInputChannels(), getTotalNumOutputChannels()));
    if (sr > 0.0)
        for (auto& card : cardRack)
        {
            card->setProcessingContext (sr, fftProcessor.getFftSize());
            card->setNumChannels (numCh);
        }
}

//==============================================================================
//  Push parameters -> card settings
//==============================================================================
void CognitoniBlkFxAudioProcessor::pushParameterSnapshotToCards()
{
    rebuildCardRack();

    const auto nyquist = juce::jmax (20.0f, static_cast<float> (getSampleRate() * 0.5));

    int rackIndex = 0;
    for (int s = 0; s < CardSchema::numSlots && rackIndex < (int)cardRack.size(); ++s)
    {
        const auto& sp = slotRuntimeParams[static_cast<size_t> (s)];
        const auto  t  = cardTypeFromFloat (loadParamOr (sp.cardType, 0.0f));
        if (t == CardSchema::CardType::empty)
            continue;

        auto& card = *cardRack[static_cast<size_t> (rackIndex++)];

        const bool  bypassed   = loadParamOr (sp.bypass,   0.0f) > 0.5f;
        const float wetDry     = loadParamOr (sp.wetDry,   1.0f);
        const float amount     = juce::jlimit (0.0f, 1.0f, loadParamOr (sp.amount, 0.0f));
        const int   harmInt    = juce::jlimit (0, 3, static_cast<int> (loadParamOr (sp.harmType, 0.0f)));
        const float freqAHz    = normalisedToDtBlkHz (juce::jlimit (0.0f, 1.0f, loadParamOr (sp.freqA, 0.0f)), nyquist);
        const float freqBHz    = normalisedToDtBlkHz (juce::jlimit (0.0f, 1.0f, loadParamOr (sp.freqB, 1.0f)), nyquist);

        if (auto* autoHarmC = dynamic_cast<AutoHarmCard*> (&card))
        {
            auto settings = AutoHarmCard::PresetManager::getProfile (AutoHarmCard::Profile::DtBlkFxClassicAutoHarm);
            settings.common.isBypassed  = bypassed;
            settings.common.wetDry      = wetDry;
            settings.targetIntensity    = amount;
            settings.harmonicType       = static_cast<SpectralHarmonicType> (harmInt);
            settings.searchBandHz.minHz = freqAHz;
            settings.searchBandHz.maxHz = freqBHz;
            autoHarmC->setSettings (settings);
        }
        else if (auto* contrastC = dynamic_cast<ContrastCard*> (&card))
        {
            ContrastCard::Settings settings;
            settings.common.isBypassed  = bypassed;
            settings.common.wetDry      = wetDry;
            settings.amount             = amount;
            settings.harmonicType       = static_cast<SpectralHarmonicType> (harmInt);
            settings.searchBandHz.minHz = freqAHz;
            settings.searchBandHz.maxHz = freqBHz;
            contrastC->setSettings (settings);
        }
        else if (auto* sawsC = dynamic_cast<SawsCard*> (&card))
        {
            SawsCard::Settings settings;
            settings.common.isBypassed  = bypassed;
            settings.common.wetDry      = wetDry;
            settings.amount             = amount;
            settings.harmonicType       = static_cast<SpectralHarmonicType> (harmInt);
            settings.searchBandHz.minHz = freqAHz;
            settings.searchBandHz.maxHz = freqBHz;
            sawsC->setSettings (settings);
        }
        else if (auto* smearC = dynamic_cast<SmearCard*> (&card))
        {
            SmearCard::Settings settings;
            settings.common.isBypassed  = bypassed;
            settings.common.wetDry      = 1.0f;   // SmearCard doesn't use an extra wet/dry mix
            settings.ampParam           = amount;  // dB knob 0..1 → amp multiplier
            settings.smearAmount        = wetDry;  // Value knob 0..1 → phase scramble intensity
            settings.searchBandHz.minHz = freqAHz;
            settings.searchBandHz.maxHz = freqBHz;
            smearC->setSettings (settings);
        }
    }
}

//==============================================================================
//  Boilerplate
//==============================================================================
const juce::String CognitoniBlkFxAudioProcessor::getName() const { return JucePlugin_Name; }

bool CognitoniBlkFxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CognitoniBlkFxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CognitoniBlkFxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CognitoniBlkFxAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int  CognitoniBlkFxAudioProcessor::getNumPrograms() { return juce::jmax (1, static_cast<int> (getPresetDefinitions().size())); }
int  CognitoniBlkFxAudioProcessor::getCurrentProgram() { return currentPresetIndex.load (std::memory_order_relaxed); }
void CognitoniBlkFxAudioProcessor::setCurrentProgram (int index) { applyPresetByIndex (index); }

const juce::String CognitoniBlkFxAudioProcessor::getProgramName (int index)
{
    const auto& presets = getPresetDefinitions();
    if (index >= 0 && index < (int)presets.size())
        return presets[(size_t)index].name;
    return "Preset";
}

void CognitoniBlkFxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    if (newName.isEmpty() || index < 0 || index >= (int)presetDefinitions.size()) return;
    presetDefinitions[(size_t)index].name = newName;
    savePresetsToJson();
}

juce::StringArray CognitoniBlkFxAudioProcessor::getPresetNames() const
{
    juce::StringArray n;
    for (const auto& p : getPresetDefinitions()) n.add (p.name);
    return n;
}

float CognitoniBlkFxAudioProcessor::getCurrentNyquistHz() const noexcept
{
    const auto sr = getSampleRate();
    return (sr > 0.0) ? static_cast<float> (sr * 0.5) : 22050.0f;
}

float CognitoniBlkFxAudioProcessor::getLastInputRms()         const noexcept { return lastInputRms.load (std::memory_order_relaxed); }
float CognitoniBlkFxAudioProcessor::getLastOutputRms()        const noexcept { return lastOutputRms.load (std::memory_order_relaxed); }
int   CognitoniBlkFxAudioProcessor::getLastSanitisedSamples() const noexcept { return lastSanitisedSamples.load (std::memory_order_relaxed); }
int   CognitoniBlkFxAudioProcessor::getLastInputChannels()    const noexcept { return lastInputChannels.load (std::memory_order_relaxed); }
int   CognitoniBlkFxAudioProcessor::getLastOutputChannels()   const noexcept { return lastOutputChannels.load (std::memory_order_relaxed); }

bool CognitoniBlkFxAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* CognitoniBlkFxAudioProcessor::createEditor()
{
    return new CognitoniBlkFxAudioProcessorEditor (*this);
}

//==============================================================================
//  prepareToPlay / releaseResources
//==============================================================================
void CognitoniBlkFxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    const int prepareCh = juce::jmax (1, juce::jmax (getTotalNumInputChannels(), getTotalNumOutputChannels()));
    fftProcessor.prepare (sampleRate, prepareCh);

    FFTProcessor::Settings fftSettings;
    fftSettings.overlapAmount = 0.499f;
    fftSettings.fftOrder      = (blackLensParam != nullptr)
                                    ? fftOrderFromParam (blackLensParam->load (std::memory_order_relaxed))
                                    : FFTProcessor::defaultFftOrder;
    fftProcessor.setSettings (fftSettings);

    kDryDelaySize = 2 * fftProcessor.getFftSize() - fftProcessor.getHopSize();
    setLatencySamples (kDryDelaySize);

    // Initialise dry-delay ring buffers
    dryDelayBuffers.assign ((size_t)prepareCh, std::vector<float> ((size_t)kDryDelaySize, 0.0f));
    dryDelayWritePos.assign ((size_t)prepareCh, 0);

    for (auto& card : cardRack)
    {
        card->setProcessingContext (sampleRate, kDryDelaySize);
        card->setNumChannels (prepareCh);
    }

    pushParameterSnapshotToCards();

    // Mark the processor as fully initialised so applyPresetByIndex() uses the
    // queue path instead of direct setValueNotifyingHost() calls.
    processorInitialized = true;
}

void CognitoniBlkFxAudioProcessor::releaseResources() { fftProcessor.reset(); }

#ifndef JucePlugin_PreferredChannelConfigurations
bool CognitoniBlkFxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
    return true;
  #endif
}
#endif

//==============================================================================
//  processBlock
//==============================================================================
void CognitoniBlkFxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                  juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

    const int totalIn  = getTotalNumInputChannels();
    const int totalOut = getTotalNumOutputChannels();
    const int bufCh    = buffer.getNumChannels();
    lastInputChannels.store  (totalIn,  std::memory_order_relaxed);
    lastOutputChannels.store (totalOut, std::memory_order_relaxed);

    if (totalIn > 0 && totalOut > totalIn)
    {
        const int from = juce::jlimit (0, bufCh, totalIn);
        const int to   = juce::jlimit (from, bufCh, totalOut);
        for (int ch = from; ch < to; ++ch)
            buffer.clear (ch, 0, buffer.getNumSamples());
    }

    // Drain pending parameter changes (enqueued on the message thread, applied
    // here on the audio thread so the VST3 wrapper's paramChanged() takes the
    // non-message-thread path and skips performEdit() — meaning the host records
    // this entire batch as one undoable event, not N individual ones).
    if (hasPendingParams.load (std::memory_order_acquire))
    {
        std::vector<PendingParamChange> localChanges;
        bool doForceRebuild = false;
        int  newPresetIdx   = -1;
        {
            const juce::ScopedLock sl (pendingParamsLock);
            std::swap (localChanges, pendingParams);
            doForceRebuild = forceCardRebuildWhenDrained.exchange (false, std::memory_order_relaxed);
            newPresetIdx   = pendingPresetIndexToStore .exchange (-1,    std::memory_order_relaxed);
            hasPendingParams.store (false, std::memory_order_release);
        }
        for (auto& c : localChanges)
            if (auto* p = apvts.getParameter (c.paramId))
                p->setValueNotifyingHost (juce::jlimit (0.0f, 1.0f, c.normValue));
        if (doForceRebuild)
            lastBuiltSlotTypes = { -1, -1, -1 };
        if (newPresetIdx >= 0)
            currentPresetIndex.store (newPresetIdx, std::memory_order_relaxed);
    }

    // Input gain
    const float igLin = juce::Decibels::decibelsToGain (loadParamOr (inputGainParam, 0.0f), -60.0f);
    if (std::abs (igLin - 1.0f) > 1.0e-5f)
        for (int ch = 0; ch < bufCh; ++ch)
            buffer.applyGain (ch, 0, buffer.getNumSamples(), igLin);

    // Capture pre-FFT dry copy
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf (buffer, true);

    // Check whether any slot is active
    bool hasActiveSlot = false;
    for (int s = 0; s < CardSchema::numSlots; ++s)
    {
        const auto& sp = slotRuntimeParams[(size_t)s];
        if (cardTypeFromFloat (loadParamOr (sp.cardType, 0.0f)) == CardSchema::CardType::empty) continue;
        if (loadParamOr (sp.bypass, 0.0f) > 0.5f) continue;
        if (loadParamOr (sp.wetDry, 1.0f) > 1.0e-4f) { hasActiveSlot = true; break; }
    }

    if (! hasActiveSlot)
    {
        applyMasterWetDryMix (buffer, dryBuffer);
        const float ogLin = juce::Decibels::decibelsToGain (loadParamOr (outputGainParam, 0.0f), -60.0f);
        if (std::abs (ogLin - 1.0f) > 1.0e-5f)
            for (int ch = 0; ch < bufCh; ++ch)
                buffer.applyGain (ch, 0, buffer.getNumSamples(), ogLin);

        const int ch2 = juce::jmin (bufCh, dryBuffer.getNumChannels());
        const int s2  = juce::jmin (buffer.getNumSamples(), dryBuffer.getNumSamples());
        double inputEnergy = 0.0, outputEnergy = 0.0;
        for (int ch = 0; ch < ch2; ++ch)
        {
            const auto* in  = dryBuffer.getReadPointer (ch);
            const auto* out = buffer.getReadPointer (ch);
            for (int i = 0; i < s2; ++i)
            {
                const double inV  = (double)in[i];
                const double outV = (double)out[i];
                inputEnergy  += inV  * inV;
                outputEnergy += outV * outV;
            }
        }
        const double denom = (double)juce::jmax (1, ch2 * s2);
        lastInputRms.store  ((float)std::sqrt (inputEnergy  / denom), std::memory_order_relaxed);
        lastOutputRms.store ((float)std::sqrt (outputEnergy / denom), std::memory_order_relaxed);
        lastSanitisedSamples.store (0, std::memory_order_relaxed);
        return;
    }

    // Update parameters and run FFT
    pushParameterSnapshotToCards();

    // Detect blackLens (FFT window) changes and rebuild if needed
    if (blackLensParam != nullptr)
    {
        const int reqOrder = fftOrderFromParam (blackLensParam->load (std::memory_order_relaxed));
        if (reqOrder != fftProcessor.getFftOrder())
        {
            const int numCh = (int)dryDelayBuffers.size();
            const double sr = getSampleRate();

            FFTProcessor::Settings fftSettings;
            fftSettings.overlapAmount = 0.499f;
            fftSettings.fftOrder      = reqOrder;
            fftProcessor.setSettings (fftSettings);

            kDryDelaySize = 2 * fftProcessor.getFftSize() - fftProcessor.getHopSize();
            setLatencySamples (kDryDelaySize);

            dryDelayBuffers.assign ((size_t)numCh, std::vector<float> ((size_t)kDryDelaySize, 0.0f));
            dryDelayWritePos.assign ((size_t)numCh, 0);

            for (auto& card : cardRack)
            {
                card->setProcessingContext (sr, kDryDelaySize);
                card->setNumChannels (numCh);
            }
        }
    }

    FFTProcessor::Settings fftSettings;
    fftSettings.overlapAmount = 0.499f;
    fftSettings.fftOrder      = fftProcessor.getFftOrder();
    fftProcessor.setSettings (fftSettings);
    fftProcessor.processBlock (buffer, cardRack);

    // Sanitise NaN/Inf
    const int ch2 = juce::jmin (bufCh, dryBuffer.getNumChannels());
    const int s2  = juce::jmin (buffer.getNumSamples(), dryBuffer.getNumSamples());
    int sanitised = 0;
    double inputEnergy = 0.0, outputEnergy = 0.0;
    for (int ch = 0; ch < ch2; ++ch)
    {
        const auto* in  = dryBuffer.getReadPointer (ch);
        auto*       out = buffer.getWritePointer (ch);
        for (int i = 0; i < s2; ++i)
        {
            const double inV = (double)in[i];
            auto outV = (double)out[i];
            if (! std::isfinite (outV)) { out[i] = in[i]; outV = inV; ++sanitised; }
            inputEnergy  += inV * inV;
            outputEnergy += outV * outV;
        }
    }

    // Delay the dry buffer by kDryDelaySize samples to align with FFT latency
    const int numSamples = dryBuffer.getNumSamples();
    const int numDelayCh = (int)dryDelayBuffers.size();
    for (int ch = 0; ch < juce::jmin (numDelayCh, dryBuffer.getNumChannels()); ++ch)
    {
        auto*  dryData  = dryBuffer.getWritePointer (ch);
        auto&  delayBuf = dryDelayBuffers[(size_t)ch];
        int&   wPos     = dryDelayWritePos[(size_t)ch];
        for (int i = 0; i < numSamples; ++i)
        {
            const float delayed = delayBuf[(size_t)wPos];
            delayBuf[(size_t)wPos] = dryData[i];
            dryData[i] = delayed;
            if (++wPos >= kDryDelaySize) wPos = 0;
        }
    }

    applyMasterWetDryMix (buffer, dryBuffer);

    // Output gain
    const float ogLin = juce::Decibels::decibelsToGain (loadParamOr (outputGainParam, 0.0f), -60.0f);
    if (std::abs (ogLin - 1.0f) > 1.0e-5f)
        for (int ch = 0; ch < bufCh; ++ch)
            buffer.applyGain (ch, 0, buffer.getNumSamples(), ogLin);

    // Output RMS
    double mixedEnergy = 0.0;
    for (int ch = 0; ch < ch2; ++ch)
    {
        const auto* d = buffer.getReadPointer (ch);
        for (int i = 0; i < s2; ++i) { const double v = (double)d[i]; mixedEnergy += v * v; }
    }
    const double denom = (double)juce::jmax (1, ch2 * s2);
    lastInputRms.store  ((float)std::sqrt (inputEnergy  / denom), std::memory_order_relaxed);
    lastOutputRms.store ((float)std::sqrt (mixedEnergy  / denom), std::memory_order_relaxed);
    lastSanitisedSamples.store (sanitised, std::memory_order_relaxed);
}

//==============================================================================
//  applyMasterWetDryMix
//==============================================================================
void CognitoniBlkFxAudioProcessor::applyMasterWetDryMix (juce::AudioBuffer<float>& wetBuffer,
                                                          const juce::AudioBuffer<float>& dryBuffer) const
{
    float wet = 1.0f;
    if (masterWetDryParam != nullptr)
        wet = juce::jlimit (0.0f, 1.0f, masterWetDryParam->load (std::memory_order_relaxed));
    else if (auto* p = apvts.getParameter (paramMasterWetDry))
        wet = juce::jlimit (0.0f, 1.0f, p->getValue());

    if (wet >= 1.0f) return;
    if (wet <= 0.0f) { wetBuffer.makeCopyOf (dryBuffer, true); return; }

    const float dry = 1.0f - wet;
    const int ch2   = juce::jmin (wetBuffer.getNumChannels(), dryBuffer.getNumChannels());
    const int s2    = juce::jmin (wetBuffer.getNumSamples(),  dryBuffer.getNumSamples());
    for (int ch = 0; ch < ch2; ++ch)
    {
        auto*       w = wetBuffer.getWritePointer (ch);
        const auto* d = dryBuffer.getReadPointer  (ch);
        for (int i = 0; i < s2; ++i)
            w[i] = w[i] * wet + d[i] * dry;
    }
}

//==============================================================================
//  queueParameterChanges
//==============================================================================
void CognitoniBlkFxAudioProcessor::queueParameterChanges (std::vector<PendingParamChange> changes,
                                                           bool forceCardRebuild,
                                                           int  presetIndexToStore)
{
    const juce::ScopedLock sl (pendingParamsLock);
    pendingParams.insert (pendingParams.end(),
                          std::make_move_iterator (changes.begin()),
                          std::make_move_iterator (changes.end()));
    if (forceCardRebuild)
        forceCardRebuildWhenDrained.store (true, std::memory_order_relaxed);
    if (presetIndexToStore >= 0)
        pendingPresetIndexToStore.store (presetIndexToStore, std::memory_order_relaxed);
    hasPendingParams.store (true, std::memory_order_release);
}

//==============================================================================
//  setParameterToPlainValue
//==============================================================================
void CognitoniBlkFxAudioProcessor::setParameterToPlainValue (const juce::String& parameterId,
                                                              float plainValue)
{
    if (auto* p = apvts.getParameter (parameterId))
    {
        float norm = plainValue;
        if (auto* rp = dynamic_cast<juce::RangedAudioParameter*> (p))
            norm = rp->convertTo0to1 (plainValue);
        p->setValueNotifyingHost (juce::jlimit (0.0f, 1.0f, norm));
    }
}

//==============================================================================
//  applyPresetByIndex
//==============================================================================
void CognitoniBlkFxAudioProcessor::applyPresetByIndex (int presetIndex)
{
    const auto& presets = getPresetDefinitions();
    if (presets.empty()) return;

    const int clamped = juce::jlimit (0, (int)presets.size() - 1, presetIndex);
    const auto& preset = presets[(size_t)clamped];

    if (! processorInitialized)
    {
        // Construction-time path: no host is connected yet so setValueNotifyingHost
        // creates no undo entries.  Apply the preset synchronously.
        for (const auto& gv : preset.globalValues)
            setParameterToPlainValue (gv.parameterId, gv.plainValue);
        for (const auto& sv : preset.slots)
            for (const auto& v : sv.values)
                setParameterToPlainValue (v.parameterId, v.plainValue);
        currentPresetIndex.store (clamped, std::memory_order_relaxed);
        lastBuiltSlotTypes = { -1, -1, -1 };
        rebuildCardRack();
        pushParameterSnapshotToCards();
        return;
    }

    // Post-init path: queue all changes so they are applied on the audio thread.
    // The VST3 wrapper's paramChanged() skips performEdit() for audio-thread calls,
    // so the entire preset load becomes ONE undoable host event.
    std::vector<PendingParamChange> changes;
    changes.reserve (preset.globalValues.size() +
                     [&]{ size_t n = 0; for (auto& sv : preset.slots) n += sv.values.size(); return n; }());

    auto addChange = [&] (const juce::String& id, float plainValue)
    {
        float norm = plainValue;
        if (auto* p = apvts.getParameter (id))
            if (auto* rp = dynamic_cast<juce::RangedAudioParameter*> (p))
                norm = rp->convertTo0to1 (plainValue);
        changes.push_back ({ id, juce::jlimit (0.0f, 1.0f, norm) });
    };

    for (const auto& gv : preset.globalValues)
        addChange (gv.parameterId, gv.plainValue);
    for (const auto& sv : preset.slots)
        for (const auto& v : sv.values)
            addChange (v.parameterId, v.plainValue);

    queueParameterChanges (std::move (changes), /*forceCardRebuild=*/true, /*presetIndex=*/clamped);
}

//==============================================================================
//  Save / delete presets
//==============================================================================
bool CognitoniBlkFxAudioProcessor::saveCurrentPresetAs (const juce::String& presetName)
{
    if (presetName.isEmpty()) return false;

    PresetDefinition newPreset;
    newPreset.name = presetName;
    newPreset.userPreset = true;
    newPreset.globalValues = {
        { paramMasterWetDry, loadParamOr (masterWetDryParam, 1.0f) },
        { paramBlackLens,    loadParamOr (blackLensParam,    12.0f) }
    };

    for (int s = 0; s < CardSchema::numSlots; ++s)
    {
        SlotPresetValues sv;
        sv.slotIndex = s;
        auto addV = [&] (const juce::String& id) {
            if (auto* p = apvts.getRawParameterValue (id))
                sv.values.push_back ({ id, p->load (std::memory_order_relaxed) });
        };
        addV (CardSchema::cardTypeParam (s));
        addV (CardSchema::amountParam   (s));
        addV (CardSchema::harmTypeParam (s));
        addV (CardSchema::freqAParam    (s));
        addV (CardSchema::freqBParam    (s));
        addV (CardSchema::bypassParam   (s));
        addV (CardSchema::wetDryParam   (s));
        newPreset.slots.push_back (std::move (sv));
    }

    presetDefinitions.push_back (newPreset);
    currentPresetIndex.store ((int)presetDefinitions.size() - 1, std::memory_order_relaxed);
    return savePresetsToJson();
}

bool CognitoniBlkFxAudioProcessor::deletePresetByIndex (int presetIndex)
{
    if (presetIndex < 0 || presetIndex >= (int)presetDefinitions.size()) return false;
    if (! presetDefinitions[(size_t)presetIndex].userPreset) return false;

    presetDefinitions.erase (presetDefinitions.begin() + presetIndex);
    if (presetDefinitions.empty())
        presetDefinitions = createDefaultPresetDefinitions();

    int nextIndex = 0;
    for (int i = 0; i < (int)presetDefinitions.size(); ++i)
        if (presetDefinitions[(size_t)i].name == "Empty") { nextIndex = i; break; }

    applyPresetByIndex (nextIndex);
    return savePresetsToJson();
}

bool CognitoniBlkFxAudioProcessor::isPresetUserDeletable (int presetIndex) const noexcept
{
    if (presetIndex < 0 || presetIndex >= (int)presetDefinitions.size()) return false;

    if (presetIndex == 0) 
        return false;

    return presetDefinitions[(size_t)presetIndex].userPreset;
}

juce::String CognitoniBlkFxAudioProcessor::getPresetAuthor (int presetIndex) const noexcept
{
    if (presetIndex < 0 || presetIndex >= (int)presetDefinitions.size())
        return {};
    // Only show author for factory presets with a non-empty author field.
    // User presets never have an author attribution.
    const auto& p = presetDefinitions[(size_t)presetIndex];
    return p.userPreset ? juce::String{} : p.author;
}

int CognitoniBlkFxAudioProcessor::getCurrentPresetIndex() const noexcept
{
    return currentPresetIndex.load (std::memory_order_relaxed);
}

//==============================================================================
//  State (DAW session)
//==============================================================================
void CognitoniBlkFxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto state = apvts.copyState(); state.isValid())
        if (auto xml = state.createXml())
            copyXmlToBinary (*xml, destData);
}

void CognitoniBlkFxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        const auto vt = juce::ValueTree::fromXml (*xml);
        if (vt.isValid() && vt.hasType (apvts.state.getType()))
        {
            apvts.replaceState (vt);
            lastBuiltSlotTypes = { -1, -1, -1 };
            rebuildCardRack();
            pushParameterSnapshotToCards();
        }
    }
}

//==============================================================================
//  JSON preset persistence
//==============================================================================
juce::File CognitoniBlkFxAudioProcessor::getPresetStorageFile() const
{
    auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
               .getChildFile ("CognitoniBlkFx");
    if (! dir.exists()) dir.createDirectory();
    return dir.getChildFile ("presets.json");
}

juce::var CognitoniBlkFxAudioProcessor::serializePresetToJson (const PresetDefinition& preset)
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty ("name",       preset.name);
    obj->setProperty ("userPreset", preset.userPreset);

    auto* globalObj = new juce::DynamicObject();
    for (const auto& gv : preset.globalValues)
        globalObj->setProperty (gv.parameterId, gv.plainValue);
    obj->setProperty ("global", juce::var (globalObj));

    juce::Array<juce::var> slotsArray;
    for (const auto& sv : preset.slots)
    {
        auto* slotObj = new juce::DynamicObject();
        slotObj->setProperty ("slotIndex", sv.slotIndex);
        for (const auto& v : sv.values)
            slotObj->setProperty (v.parameterId, v.plainValue);
        slotsArray.add (juce::var (slotObj));
    }
    obj->setProperty ("slots", slotsArray);
    return juce::var (obj);
}

bool CognitoniBlkFxAudioProcessor::deserializePresetFromJson (const juce::var& jsonValue,
                                                               PresetDefinition& outPreset)
{
    auto* obj = jsonValue.getDynamicObject();
    if (obj == nullptr) return false;

    outPreset = {};
    outPreset.name       = obj->getProperty ("name").toString();
    outPreset.userPreset = static_cast<bool> (obj->getProperty ("userPreset"));
    if (outPreset.name.isEmpty()) return false;

    if (auto* globalObj = obj->getProperty ("global").getDynamicObject())
    {
        const auto& props = globalObj->getProperties();
        for (int i = 0; i < props.size(); ++i)
            outPreset.globalValues.push_back ({ props.getName (i).toString(),
                                                static_cast<float> (props.getValueAt (i)) });
    }

    if (auto* slotsArr = obj->getProperty ("slots").getArray())
    {
        for (const auto& slotVar : *slotsArr)
        {
            if (auto* slotObj = slotVar.getDynamicObject())
            {
                SlotPresetValues sv;
                sv.slotIndex = static_cast<int> (slotObj->getProperty ("slotIndex"));
                const auto& props = slotObj->getProperties();
                for (int i = 0; i < props.size(); ++i)
                {
                    const auto key = props.getName (i).toString();
                    if (key == "slotIndex") continue;
                    sv.values.push_back ({ key, static_cast<float> (props.getValueAt (i)) });
                }
                outPreset.slots.push_back (std::move (sv));
            }
        }
    }

    return ! outPreset.slots.empty() || ! outPreset.globalValues.empty();
}

bool CognitoniBlkFxAudioProcessor::loadPresetsFromJson()
{
    const auto file = getPresetStorageFile();
    if (! file.existsAsFile()) return savePresetsToJson();

    const auto parsed = juce::JSON::parse (file);
    if (parsed.isVoid()) return false;

    juce::Array<juce::var>* presetsArray = nullptr;
    if (auto* rootObj = parsed.getDynamicObject())
    {
        if (auto* arr = rootObj->getProperty ("presets").getArray())
            presetsArray = arr;
    }
    else if (parsed.isArray())
    {
        presetsArray = parsed.getArray();
    }

    if (presetsArray == nullptr) return false;

    for (const auto& value : *presetsArray)
    {
        PresetDefinition p;
        if (deserializePresetFromJson (value, p) && p.userPreset)
            presetDefinitions.push_back (std::move (p));
    }

    return ! presetDefinitions.empty();
}

bool CognitoniBlkFxAudioProcessor::savePresetsToJson() const
{
    auto* rootObj = new juce::DynamicObject();
    juce::Array<juce::var> arr;
    for (const auto& p : presetDefinitions)
        if (p.userPreset)
            arr.add (serializePresetToJson (p));
    rootObj->setProperty ("presets", arr);
    return getPresetStorageFile().replaceWithText (juce::JSON::toString (juce::var (rootObj), true));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CognitoniBlkFxAudioProcessor();
}
