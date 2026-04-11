/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CardSchema.h"
#include "Fonts/EmbeddedFonts.h"

//==============================================================================
class CognitoniBlkFxAudioProcessorEditor  : public juce::AudioProcessorEditor
                                           , private juce::Timer
                                           , public juce::DragAndDropContainer
{
public:
    CognitoniBlkFxAudioProcessorEditor (CognitoniBlkFxAudioProcessor&);
    ~CognitoniBlkFxAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;  // dismiss card picker on outside click

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    //  Level meter 
    class LevelMeterComponent : public juce::Component
    {
    public:
        void setLevel (float rmsLinear) noexcept
        {
            const auto dBfs = rmsLinear > 1.0e-6f
                                  ? 20.0f * std::log10 (rmsLinear)
                                  : -60.0f;
            currentLevel = juce::jlimit (0.0f, 1.0f, (dBfs + 60.0f) / 60.0f);
            if (currentLevel >= peakLevel) { peakLevel = currentLevel; peakHold = 45; }
            else if (peakHold > 0) --peakHold;
            else peakLevel = juce::jmax (0.0f, peakLevel - 0.007f);
        }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat();
            g.setColour (juce::Colour::fromRGB (22, 26, 32));
            g.fillRoundedRectangle (b, 4.0f);

            constexpr int segs = 14;
            const float segH = (b.getHeight() - 6.0f) / segs;
            const float segW = b.getWidth() - 6.0f;
            const int lit = juce::roundToInt (currentLevel * segs);

            for (int i = 0; i < segs; ++i)
            {
                const float sy = b.getBottom() - 3.0f - (i + 1) * segH + 1.5f;
                juce::Colour c = (i >= segs - 2) ? juce::Colour::fromRGB (200, 70, 70)
                               : (i >= segs - 4) ? juce::Colour::fromRGB (200, 175, 55)
                                                 : juce::Colour::fromRGB (65, 185, 115);
                g.setColour (i < lit ? c : c.withAlpha (0.14f));
                g.fillRoundedRectangle (b.getX() + 3.0f, sy, segW, segH - 1.5f, 1.5f);
            }
            if (peakLevel > 0.02f)
            {
                const int ps = juce::jlimit (0, segs - 1, juce::roundToInt (peakLevel * segs) - 1);
                const float py = b.getBottom() - 3.0f - (ps + 1) * segH + 1.5f;
                juce::Colour pc = (ps >= segs - 2) ? juce::Colour::fromRGB (220, 90, 90)
                                : (ps >= segs - 4) ? juce::Colour::fromRGB (220, 195, 70)
                                                   : juce::Colour::fromRGB (80, 210, 130);
                g.setColour (pc);
                g.fillRoundedRectangle (b.getX() + 3.0f, py, segW, segH - 1.5f, 1.5f);
            }
        }
    private:
        float currentLevel = 0.0f;
        float peakLevel    = 0.0f;
        int   peakHold     = 0;
    };

    //  Slider subclass with public text-box refresh 
    class KnobSlider : public juce::Slider
    {
    public:
        KnobSlider() = default;
        /** Force the text box label to re-render using the current textFromValueFunction. */
        void forceTextBoxUpdate() { updateText(); }
    };

    //  Card-picker modal overlay 
    class CardPickerOverlay : public juce::Component
    {
    public:
        CardPickerOverlay();
        void paint (juce::Graphics&) override;
        void mouseMove  (const juce::MouseEvent&) override;
        void mouseExit  (const juce::MouseEvent&) override;
        void mouseUp    (const juce::MouseEvent&) override;
        void mouseDown  (const juce::MouseEvent&) override;

        void setTypefaces (juce::Typeface::Ptr regular, juce::Typeface::Ptr bold)
        {
            regularTypeface = regular;
            boldTypeface    = bold;
        }

        std::function<void (CardSchema::CardType)> onCardChosen;
        std::function<void()>                      onDismiss;

        juce::Rectangle<float> popupRect;  // set by showCardPickerModal; items are relative to this

    private:
        int hoveredItem = -1;
        juce::Typeface::Ptr regularTypeface;
        juce::Typeface::Ptr boldTypeface;
        juce::Rectangle<float> itemRect (int idx) const;
    };

    //  Generic slot card component 
    class CardComponent : public juce::Component
                        , public juce::DragAndDropTarget
    {
    public:
        CardComponent();

        // Set embedded typefaces (called by parent editor after loading)
        void setTypefaces (juce::Typeface::Ptr regular, juce::Typeface::Ptr bold)
        {
            regularTypeface = regular;
            boldTypeface    = bold;
        }

        // Set the card type and update all visual/control state accordingly
        void setCardType (CardSchema::CardType newType, const juce::String& titleText = {});
        CardSchema::CardType getCardType() const noexcept { return currentCardType; }

        void resized() override;
        void paint   (juce::Graphics& g) override;

        // Mouse event overrides
        void mouseEnter (const juce::MouseEvent&) override;
        void mouseExit  (const juce::MouseEvent&) override;
        void mouseDown  (const juce::MouseEvent&) override;
        void mouseDrag  (const juce::MouseEvent&) override;
        void mouseUp    (const juce::MouseEvent&) override;

        // DragAndDropTarget
        bool isInterestedInDragSource (const SourceDetails& details) override;
        void itemDragEnter (const SourceDetails& details) override;
        void itemDragExit  (const SourceDetails& details) override;
        void itemDropped   (const SourceDetails& details) override;

        // Controls that the editor wires to APVTS (public for access by editor)
        juce::Label        title;
        juce::ToggleButton bypassButton;
        KnobSlider         amountKnob;
        juce::Label        amountHeaderLabel;   // "dB"  above knob
        juce::Slider       harmonicType;        // Rotary knob for harmonic type (0=Odd,1=Even,2=Both,3=Between)
        juce::Label        harmonicTypeLabel;   // "Type" header + current value text
        KnobSlider         wetDryKnob;
        juce::Label        wetDryHeaderLabel;   // "Value" above knob
        juce::TextButton   removeButton;        // ""  shown when filled

        juce::Slider       frequencyRangeSlider;
        juce::Label        freqStartHeader;    // "Start" ABOVE slider
        juce::Label        freqEndHeader;      // "End"   ABOVE slider
        juce::Label        frequencyALabel;    // value below left
        juce::Label        frequencyBLabel;    // value below right

        juce::Colour       accent { juce::Colour::fromRGB (185, 180, 172) };

        std::function<void(int)>              onAddCardClicked;
        std::function<void(int)>              onDragStarted;
        std::function<void(int)>              onDragEnded;
        int slotIndex = -1;

        static void drawAutoHarmIcon (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour col);
        static void drawContrastIcon (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour col);
        static void drawSawsIcon     (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour col);
        static void drawSmearIcon    (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour col);

        void setDragSourceVisual (bool shouldBeSource)
        {
            if (isDragSource == shouldBeSource)
                return;

            isDragSource = shouldBeSource;
            repaint();
        }

    private:
        CardSchema::CardType currentCardType = CardSchema::CardType::empty;
        bool isHovered    = false;  // Hover state for empty card
        bool isDragSource = false;  // Dimmed while being dragged
        bool canStartDrag = false;  // Set in mouseDown if pointer is in title row
        bool dragStarted  = false;  // Drag initiated flag
        juce::Point<int> dragClickPos;  // Mouse-down position within component (for ghost offset)

        juce::Typeface::Ptr regularTypeface;  // Montserrat Regular (set by parent editor)
        juce::Typeface::Ptr boldTypeface;     // Montserrat Bold    (set by parent editor)
        std::unique_ptr<juce::Drawable> dragHandleIcon;

        void showFilledControls (bool show);
    };

    //  Icon button 
    class IconButton : public juce::Button
    {
    public:
        IconButton (const juce::String& buttonName) : juce::Button (buttonName) {}

        void setSvgIcon (const juce::String& svgText)
        {
            auto svgXml = juce::parseXML (svgText);
            if (svgXml != nullptr)
                drawable = juce::Drawable::createFromSVG (*svgXml);
        }

        void paintButton (juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override
        {
            auto bounds = getLocalBounds().toFloat().reduced (0.5f);
            const bool en = isEnabled();
            juce::Colour fill;
            if      (!en)              fill = juce::Colour::fromRGB (200, 195, 188);  // disabled: slightly darker
            else if (isButtonDown)     fill = juce::Colour::fromRGB (192, 187, 180);  // pressed
            else if (isMouseOverButton)fill = juce::Colour::fromRGB (212, 207, 200);  // hover
            else                       fill = juce::Colour::fromRGB (225, 220, 214);  // normal: matches header bg

            g.setColour (fill);
            g.fillRoundedRectangle (bounds, 8.0f);

            if (en && (isMouseOverButton || isButtonDown))
            {
                g.setColour (juce::Colour::fromRGB (185, 180, 172).withAlpha (0.6f));
                g.drawRoundedRectangle (bounds, 8.0f, 1.0f);
            }

            if (drawable != nullptr)
            {
                const float alpha    = en ? 1.0f : 0.4f;
                const auto  iconBase = isButtonDown ? juce::Colour::fromRGB (88, 84, 78)
                                                     : juce::Colour::fromRGB (116, 111, 104);
                const auto  iconCol  = iconBase.withAlpha (alpha);
                const float iconInset = juce::jmax (4.0f, juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.22f);
                auto iconBounds = bounds.reduced (iconInset).toNearestInt();

                if (auto icon = drawable->createCopy())
                {
                    icon->replaceColour (juce::Colours::black, iconCol);
                    icon->replaceColour (juce::Colours::white, fill.brighter (0.05f).withAlpha (alpha));
                    icon->drawWithin (g, iconBounds.toFloat(), juce::RectanglePlacement::centred, alpha);
                }
            }
        }
    private:
        std::unique_ptr<juce::Drawable> drawable;
    };

    //  Look and feel 
    class CognitoniLookAndFeel final : public juce::LookAndFeel_V4
    {
    public:
        CognitoniLookAndFeel();

        void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                     float sliderPosProportional, float rotaryStartAngle,
                     float rotaryEndAngle, juce::Slider&) override;

        void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                     float sliderPos, float minSliderPos, float maxSliderPos,
                     juce::Slider::SliderStyle, juce::Slider&) override;

        void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                   int buttonX, int buttonY, int buttonW, int buttonH,
                   juce::ComboBox&) override;

        void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                   bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        void positionComboBoxText (juce::ComboBox&, juce::Label&) override;

        void drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&) override;

        juce::Label* createSliderTextBox (juce::Slider&) override;
    };

    //  Helpers 
    static void configureAmountKnob (juce::Slider& slider);
    static void configureRangeSlider (juce::Slider& slider);
    juce::String normalisedToHzText (double normalisedValue) const;
    static juce::String normalisedToPercentText (double normalisedValue);
    void syncRangeSlidersFromParams();
    void pushRangeSliderToParams();
    void setParameterNormalised (const juce::String& parameterId, float value);
    void timerCallback() override;
    void refreshFrequencyLabels();
    void refreshPresetSelectorItems();
    void showSavePresetDialog();
    void showDeletePresetDialog();
    void rebuildSlotAttachments (int slotIndex);
    void showCardPickerModal (int slotIndex);
    void randomizeCards();
    void dragOperationStarted (const juce::DragAndDropTarget::SourceDetails& details) override;
    void dragOperationEnded   (const juce::DragAndDropTarget::SourceDetails& details) override;
    int slotIndexForScreenPoint (juce::Point<int> screenPoint) const;
    int slotIndexFromDragDescription (const juce::var& description) const;
    int determinePreviewTargetSlot (int srcSlot, juce::Point<int> localPoint) const;
    void updateDragPreviewFromMouse();
    std::array<int, CardSchema::numSlots> buildShiftedOrder (int srcSlot, int dstSlot) const;
    void applySlotCardBoundsOrder (const std::array<int, CardSchema::numSlots>& order, bool animate);
    void setDragPreviewTarget (int srcSlot, int dstSlot);
    void clearDragPreview (bool animate);
    void commitShiftReorder (int srcSlot, int dstSlot);
    void finaliseDragReorderFromSource (int srcSlot);

    CognitoniBlkFxAudioProcessor& audioProcessor;

    //  Header 
    juce::Label     pluginNameLabel;
    juce::Label     presetLabel;
    juce::ComboBox  presetSelector;
    juce::Label     presetAuthorLabel;   // "by <author>" shown below selector for factory presets
    IconButton      savePresetButton  { "Save Preset"   };
    IconButton      deletePresetButton{ "Delete Preset" };
    IconButton      randomizeButton   { "Randomize"     };
    juce::HyperlinkButton versionLabel;

    //  3 slot cards 
    std::array<CardComponent, CardSchema::numSlots> slotCards;

    //  Card picker overlay (shown modally when "+" clicked) 
    std::unique_ptr<CardPickerOverlay> cardPickerOverlay;
    int pendingCardPickerSlot = 0;

    //  Right panel 
    LevelMeterComponent inputLevelMeter;
    LevelMeterComponent outputLevelMeter;
    juce::Label         inputMeterLabel;
    juce::Label         outputMeterLabel;
    juce::Slider        inputGainKnob;
    juce::Slider        outputGainKnob;
    KnobSlider          masterWetDryKnob;
    juce::Label         masterWetDryLabel;
    KnobSlider          blackLensKnob;
    juce::Label         blackLensLabel;

    // Cached right-panel bounds (used in paint and resized for consistent alignment)
    juce::Rectangle<int> rightPanelRect;

    // Embedded Montserrat typefaces (loaded once, shared across the editor)
    juce::Typeface::Ptr montserratRegular;
    juce::Typeface::Ptr montserratBold;

    CognitoniLookAndFeel lookAndFeel;

    //  APVTS attachments (per slot) 
    struct SlotAttachments
    {
        std::unique_ptr<SliderAttachment>   amount;
        std::unique_ptr<SliderAttachment>   harmType;   // Harmonic type slider attachment
        std::unique_ptr<SliderAttachment>   wetDry;
        std::unique_ptr<ButtonAttachment>   bypass;
    };
    std::array<SlotAttachments, CardSchema::numSlots> slotAttachments;

    std::unique_ptr<SliderAttachment>   masterWetDryAttachment;
    std::unique_ptr<SliderAttachment>   inputGainAttachment;
    std::unique_ptr<SliderAttachment>   outputGainAttachment;
    std::unique_ptr<SliderAttachment>   blackLensAttachment;

    // Raw param pointers for freq range sliders (not attached via SliderAttachment
    // because TwoValueHorizontal requires manual sync)
    std::array<std::atomic<float>*, CardSchema::numSlots> slotFreqAParams { nullptr, nullptr, nullptr };
    std::array<std::atomic<float>*, CardSchema::numSlots> slotFreqBParams { nullptr, nullptr, nullptr };

    std::array<int, CardSchema::numSlots> lastKnownCardTypes { -1, -1, -1 };
    std::array<juce::Rectangle<int>, CardSchema::numSlots> slotBaseBounds;

    juce::ComponentAnimator cardAnimator;
    int dragPreviewSourceSlot = -1;
    int dragPreviewTargetSlot = -1;

    bool updatingRangeControls = false;
    int  lastAppliedPresetSelectorId = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CognitoniBlkFxAudioProcessorEditor)
};
