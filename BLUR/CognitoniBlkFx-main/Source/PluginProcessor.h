/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Cards/AutoHarmCard.h"
#include "Cards/ContrastCard.h"
#include "Cards/SawsCard.h"
#include "Cards/SmearCard.h"
#include "CardSchema.h"
#include "SpectralEngine/FFTProcessor.h"

#include <atomic>
#include <memory>
#include <vector>

//==============================================================================
class CognitoniBlkFxAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CognitoniBlkFxAudioProcessor();
    ~CognitoniBlkFxAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() noexcept { return apvts; }
    float getCurrentNyquistHz() const noexcept;
    juce::StringArray getPresetNames() const;
    void applyPresetByIndex (int presetIndex);
    bool saveCurrentPresetAs (const juce::String& presetName);
    bool deletePresetByIndex (int presetIndex);
    bool isPresetUserDeletable (int presetIndex) const noexcept;
    int getCurrentPresetIndex() const noexcept;
    /** Returns the author string for a factory preset, or empty string for user presets. */
    juce::String getPresetAuthor (int presetIndex) const noexcept;
    float getLastInputRms()  const noexcept;
    float getLastOutputRms() const noexcept;
    int   getLastSanitisedSamples() const noexcept;
    int   getLastInputChannels()  const noexcept;
    int   getLastOutputChannels() const noexcept;

    //==============================================================================
    /** A pending normalised parameter change to be applied on the audio thread. */
    struct PendingParamChange
    {
        juce::String paramId;
        float normValue = 0.0f;
    };

    /** Queue a batch of normalised [0,1] parameter changes to be applied on the
        next processBlock call (audio thread).

        Because these writes happen on the audio thread, the VST3 wrapper's
        paramChanged() takes the non-message-thread path and skips performEdit(),
        so the host (e.g. Ableton) does NOT create a per-param undo entry. The
        entire batch therefore becomes a single undoable moment.

        JUCE's ParameterAttachment already defers its slider update to the message
        thread via triggerAsyncUpdate(), so UI components remain thread-safe.

        @param changes            Normalised {paramId, value} pairs.
        @param forceCardRebuild   Pass true when slot card types are changing.
        @param presetIndexToStore Pass >= 0 to update currentPresetIndex atomically.
    */
    void queueParameterChanges (std::vector<PendingParamChange> changes,
                                bool forceCardRebuild   = false,
                                int  presetIndexToStore = -1);

private:
    // Per-slot runtime parameter pointers
    struct SlotRuntimeParameters
    {
        std::atomic<float>* cardType = nullptr;   // 0..4 float (CardType enum)
        std::atomic<float>* amount   = nullptr;
        std::atomic<float>* harmType = nullptr;
        std::atomic<float>* freqA    = nullptr;
        std::atomic<float>* freqB    = nullptr;
        std::atomic<float>* bypass   = nullptr;
        std::atomic<float>* wetDry   = nullptr;
    };

    // Preset storage
    struct PresetParameterValue
    {
        juce::String parameterId;
        float plainValue = 0.0f;
    };

    struct SlotPresetValues
    {
        int   slotIndex = 0;
        std::vector<PresetParameterValue> values;  // includes cardType param
    };

    struct PresetDefinition
    {
        juce::String name;
        bool userPreset = false;
        juce::String author;   // displayed as "by <author>" under the preset selector; empty hides the label
        std::vector<PresetParameterValue> globalValues;
        std::vector<SlotPresetValues>     slots;
    };

    const std::vector<PresetDefinition>& getPresetDefinitions() const;
    static std::vector<PresetDefinition> createDefaultPresetDefinitions();
    bool loadPresetsFromJson();
    bool savePresetsToJson() const;
    juce::File getPresetStorageFile() const;
    static juce::var  serializePresetToJson    (const PresetDefinition& preset);
    static bool       deserializePresetFromJson (const juce::var& jsonValue,
                                                 PresetDefinition& outPreset);
    void setParameterToPlainValue (const juce::String& parameterId, float plainValue);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void bindRuntimeParameters();
    void pushParameterSnapshotToCards();
    void rebuildCardRack();   // rebuilds SpectralCard instances to match slot types
    void applyMasterWetDryMix (juce::AudioBuffer<float>& wetBuffer,
                               const juce::AudioBuffer<float>& dryBuffer) const;

    juce::AudioProcessorValueTreeState apvts;

    // Per-slot runtime params + global params
    std::array<SlotRuntimeParameters, CardSchema::numSlots> slotRuntimeParams;
    std::atomic<float>* masterWetDryParam = nullptr;
    std::atomic<float>* inputGainParam    = nullptr;
    std::atomic<float>* outputGainParam   = nullptr;
    std::atomic<float>* blackLensParam    = nullptr;

    // Card rack — rebuilt when slot cardType params change
    FFTProcessor fftProcessor;
    std::vector<std::unique_ptr<SpectralCard>> cardRack;
    // Track the cardType value last used to build each slot's card so we
    // only reallocate when the type actually changes.
    std::array<int, CardSchema::numSlots> lastBuiltSlotTypes { -1, -1, -1 };

    // Pending parameter changes queue ─────────────────────────────────────────
    // Changes are enqueued on the message thread and drained on the audio thread
    // so the host sees no per-param performEdit() calls (= no N-step undo).
    juce::CriticalSection           pendingParamsLock;
    std::vector<PendingParamChange> pendingParams;
    std::atomic<bool>               hasPendingParams            { false };
    std::atomic<bool>               forceCardRebuildWhenDrained { false };
    std::atomic<int>                pendingPresetIndexToStore   { -1 };
    // Flag set in prepareToPlay(); guards the construction-time path in
    // applyPresetByIndex() that bypasses the queue (no host is attached yet).
    bool                            processorInitialized        { false };

    // Dry-signal delay compensation:
    // Delay the dry reference by the current FFT window size so wet/dry are
    // time-aligned.  Updated in prepareToPlay whenever the FFT order changes.
    int kDryDelaySize = FFTProcessor::defaultFftSize;
    std::vector<std::vector<float>> dryDelayBuffers;   // [channel][kDryDelaySize]
    std::vector<int>                dryDelayWritePos;  // per-channel write position

    // Preset & meters
    std::vector<PresetDefinition> presetDefinitions;
    std::atomic<int>   currentPresetIndex   { 0 };
    std::atomic<float> lastInputRms         { 0.0f };
    std::atomic<float> lastOutputRms        { 0.0f };
    std::atomic<int>   lastSanitisedSamples { 0 };
    std::atomic<int>   lastInputChannels    { 0 };
    std::atomic<int>   lastOutputChannels   { 0 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CognitoniBlkFxAudioProcessor)
};
