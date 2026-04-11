#include "MetallicKnobs.h"
#include "config.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include <array>
#include <random>

// Custom SVG Knob with value arc indicator
class ClophSVGKnobControl : public IVKnobControl {
private:
  ISVG mSVG;
  static ClophSVGKnobControl* sFocused;
  IColor mArcColor;
public:
  ClophSVGKnobControl(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, const ISVG& svg, IColor arcColor = IColor(255, 100, 100, 100))
  : IVKnobControl(bounds, paramIdx, label, style), mSVG(svg), mArcColor(arcColor) { }

  void SetArcColor(IColor color) { mArcColor = color; }

  static void SetFocusedControl(ClophSVGKnobControl* c) { sFocused = c; }
  bool HasKeyboardFocus() const { return sFocused == this; }

  void DrawWidget(IGraphics& g) override {
    const float cx = mWidgetBounds.MW();
    const float cy = mWidgetBounds.MH();
    const float sz = std::min(mWidgetBounds.W(), mWidgetBounds.H()) * 0.85f;
    const float angle = mAngle1 + (static_cast<float>(GetValue()) * (mAngle2 - mAngle1));
    
    // Draw value arc background
    const IColor clrArcBg(255, 60, 60, 60);
    g.DrawArc(clrArcBg, cx, cy, sz * 0.5f + 8.f, mAngle1, mAngle2, nullptr, 4.0f);
    
    // Draw value arc fill
    if (GetValue() > 0.001f) {
      g.DrawArc(mArcColor, cx, cy, sz * 0.5f + 8.f, mAngle1, angle, nullptr, 4.0f);
    }

    // Draw SVG knob rotated
    if (mSVG.IsValid()) {
      g.DrawRotatedSVG(mSVG, cx, cy, sz, sz, angle);
    } else {
      // Fallback: draw simple circle
      IColor clrKnob(255, 80, 80, 82);
      g.FillCircle(clrKnob, cx, cy, sz * 0.5f);
      // Indicator line
      float indX = cx + std::cos(angle - static_cast<float>(M_PI) * 0.5f) * sz * 0.35f;
      float indY = cy + std::sin(angle - static_cast<float>(M_PI) * 0.5f) * sz * 0.35f;
      g.DrawLine(IColor(255, 240, 240, 240), cx, cy, indX, indY, nullptr, 2.0f);
    }

    // Draw keyboard focus ring when focused
    if (HasKeyboardFocus()) {
      IColor clrFocus = IColor(255, 100, 220, 255);
      const float radius = sz * 0.5f + 12.0f;
      g.DrawCircle(clrFocus.WithOpacity(0.9f), cx, cy, radius, nullptr, 2.0f);
    }
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override {
    IVKnobControl::OnMouseOver(x, y, mod);
    if (IGEditorDelegate* d = GetDelegate()) {
      auto* plugin = static_cast<MetallicKnobs*>(d);
      plugin->SetHoverParam(GetParamIdx());
    }
  }

  void OnMouseOut() override {
    IVKnobControl::OnMouseOut();
    if (IGEditorDelegate* d = GetDelegate()) {
      auto* plugin = static_cast<MetallicKnobs*>(d);
      plugin->SetHoverParam(-1);
    }
  }

  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
    IVKnobControl::OnMouseDblClick(x, y, mod);
    PromptUserInput();
  }

  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override {
    // Wheel increments with modifier for finer control
    const double baseStep = 0.01;
    double scale = 1.0;
    if (mod.S) scale = 0.1; else if (mod.C) scale = 0.25;
    double newVal = GetValue() + d * baseStep * scale;
    if (newVal < 0.0) newVal = 0.0;
    if (newVal > 1.0) newVal = 1.0;
    SetValueFromUserInput(newVal);
  }

  void CreateContextMenu(IPopupMenu& contextMenu) override {
    contextMenu.AddItem("MIDI Learn", 100);
    contextMenu.AddItem("Reset to Default", 101);
  }

  void OnContextSelection(int itemSelected) override {
    if (itemSelected == 100) {
      if (IGEditorDelegate* d = GetDelegate()) {
        d->SendParameterValueFromUI(kMidiLearn, 1.0);
      }
    } else if (itemSelected == 101) {
      SetValueToDefault();
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    IVKnobControl::OnMouseDown(x, y, mod);
    sFocused = this;
    if (IGEditorDelegate* d = GetDelegate()) {
      auto* plugin = static_cast<MetallicKnobs*>(d);
      plugin->SetKeyboardFocusToControl(this);
    }
  }

  bool OnKeyDown(float x, float y, const IKeyPress& key) override {
    // Use names that won't collide with platform VK_ macros
    const int kVK_LEFT = 0x25;
    const int kVK_UP = 0x26;
    const int kVK_RIGHT = 0x27;
    const int kVK_DOWN = 0x28;
    const int kVK_PRIOR = 0x21; // PageUp
    const int kVK_NEXT = 0x22;  // PageDown
    const int kVK_HOME = 0x24;
    const int kVK_END = 0x23;
    const int kVK_RETURN = 0x0D;
    const int kVK_SPACE = 0x20;
    const int kVK_TAB = 0x09;

    if (key.VK == kVK_TAB) {
      if (IGEditorDelegate* d = GetDelegate()) {
        auto* plugin = static_cast<MetallicKnobs*>(d);
        bool forward = !key.S; // Shift+Tab goes backwards
        plugin->CycleKeyboardFocus(forward);
        return true;
      }
    }

    double baseStep = 0.01;
    if (key.S) baseStep *= 0.1; else if (key.C) baseStep *= 0.25;

    double v = GetValue();
    bool handled = true;
    if (key.VK == kVK_LEFT || key.VK == kVK_DOWN) v -= baseStep;
    else if (key.VK == kVK_RIGHT || key.VK == kVK_UP) v += baseStep;
    else if (key.VK == kVK_PRIOR) v += 0.1;
    else if (key.VK == kVK_NEXT) v -= 0.1;
    else if (key.VK == kVK_HOME) v = 0.0;
    else if (key.VK == kVK_END) v = 1.0;
    else if (key.VK == kVK_RETURN || key.VK == kVK_SPACE) { PromptUserInput(); return true; }
    else handled = false;

    if (handled) {
      if (v < 0.0) v = 0.0;
      if (v > 1.0) v = 1.0;
      SetValueFromUserInput(v);
    }
    return handled;
  }
};

// Static focus pointer definition
ClophSVGKnobControl* ClophSVGKnobControl::sFocused = nullptr;

MetallicKnobs::MetallicKnobs(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, 1))
{
  GetParam(kShatter)->InitDouble("SHATTER", 0.5, 0.0, 1.0, 0.01, "");
  GetParam(kTension)->InitDouble("TENSION", 0.5, 0.0, 1.0, 0.01, "");
  GetParam(kPang)->InitDouble("PANG", 0.3, 0.0, 1.0, 0.01, "");
  GetParam(kGrit)->InitDouble("GRIT", 0.2, 0.0, 1.0, 0.01, "");
  GetParam(kPunch)->InitDouble("PUNCH", 0.5, 0.0, 1.0, 0.01, "");
  // WEIGHT: discretized to semitone steps (-18 .. +18 semitones)
  GetParam(kWeight)->InitEnum("WEIGHT", 18, {
     "-18 st","-17 st","-16 st","-15 st","-14 st","-13 st","-12 st","-11 st","-10 st",
     "-9 st","-8 st","-7 st","-6 st","-5 st","-4 st","-3 st","-2 st","-1 st",
     "0 st",
     "+1 st","+2 st","+3 st","+4 st","+5 st","+6 st","+7 st","+8 st","+9 st","+10 st",
     "+11 st","+12 st","+13 st","+14 st","+15 st","+16 st","+17 st","+18 st"
  });
    // SHATTER ARM: safe / armed toggle for destructive shatter ranges
    GetParam(kShatterArm)->InitEnum("SHATTER ARM", 0, {"SAFE", "ARMED"});
    // MIDI learn toggle (UI affordance)
    GetParam(kMidiLearn)->InitBool("MIDI LEARN", false, "", IParam::kFlagsNone, "", "Off", "On");

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [this](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    
    // Load custom fonts with fallbacks
    bool primaryLoaded = pGraphics->LoadFont("Primary", FONT_PRIMARY_FN);
    bool secondaryLoaded = pGraphics->LoadFont("Secondary", FONT_SECONDARY_FN);
    if (!primaryLoaded) {
      pGraphics->LoadFont("Primary", "Arial", ETextStyle::Bold);
    }
    if (!secondaryLoaded) {
      pGraphics->LoadFont("Secondary", "Consolas", ETextStyle::Normal);
    }
    
    // Neutral Technical Palette
    const IColor clrBgPrimary(255, 13, 13, 13);
    const IColor clrBgSecondary(255, 20, 20, 20);
    const IColor clrBgTertiary(255, 26, 26, 26);
    const IColor clrBorder(255, 42, 42, 42);
    const IColor clrBorderBright(255, 61, 61, 61);
    const IColor clrTextMain(255, 240, 240, 240);
    const IColor clrTextDim(255, 160, 160, 160);
    const IColor clrTextMuted(255, 96, 96, 96);
    const IColor clrAccentRed(255, 217, 48, 48);
    const IColor clrAccentGreen(255, 48, 217, 96);
    const IColor clrAccentAmber(255, 217, 160, 48);
    const IColor clrMetallicDark(255, 42, 42, 44);
    const IColor clrMetallicMid(255, 74, 74, 78);

    // Layout dimensions - 8px grid system
    const IRECT b = pGraphics->GetBounds();
    const float uiScale = std::min(b.W() / static_cast<float>(PLUG_WIDTH), b.H() / static_cast<float>(PLUG_HEIGHT));
    
    // Grid units (8px base)
    const float u = 8.0f * uiScale;
    const float kHeaderHeight = 8u * uiScale;      // 64px
    const float kFooterHeight = 4u * uiScale;      // 32px
    const float kPanelWidth = 120.0f * uiScale;       // Input/Output panels
    const float kPadding = 2u * uiScale;         // 16px
    const float kGap = u;                      // 8px
    const float kKnobSize = 80.0f * uiScale;    // Main knobs
    const float kShatterSize = 100.0f * uiScale; // Shatter knob

    // Reset keyboard-focusable control list
    mKeyboardOrder.clear();
    mKeyboardFocusIdx = -1;

    // Load screw SVG (static aligned, not random)
    ISVG screwSvg = pGraphics->LoadSVG(SCREW_SVG_FN);
    if (!screwSvg.IsValid()) {
      WDL_String fallbackPath;
      fallbackPath.SetFormatted(256, "%s/%s", SHARED_RESOURCES_SUBPATH, SCREW_SVG_FN);
      screwSvg = pGraphics->LoadSVG(fallbackPath.Get());
    }

    // BACKGROUND: Deep black with subtle border
    pGraphics->AttachControl(new ILambdaControl(b, 
      [clrBgPrimary, clrBgSecondary, clrBorder, clrMetallicDark, kHeaderHeight, kFooterHeight, screwSvg, u](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
        // Main background
        g.FillRect(clrBgPrimary, r);
        
        // Header background (rackmount style)
        IRECT headerRect = r.GetFromTop(kHeaderHeight);
        g.FillRect(clrBgSecondary, headerRect);
        
        // Header bottom border
        g.DrawLine(clrBorder, headerRect.L, headerRect.B, headerRect.R, headerRect.B, nullptr, 1.f);
        
        // Footer background
        IRECT footerRect = r.GetFromBottom(kFooterHeight);
        g.FillRect(clrBgSecondary, footerRect);
        
        // Footer top border
        g.DrawLine(clrBorder, footerRect.L, footerRect.T, footerRect.R, footerRect.T, nullptr, 1.f);
        
        // Header screws (aligned, not random)
        const float screwSize = 16.0f;
        if (screwSvg.IsValid()) {
          g.DrawRotatedSVG(screwSvg, headerRect.L + 20.f, headerRect.MH(), screwSize, screwSize, 45.f);
          g.DrawRotatedSVG(screwSvg, headerRect.R - 20.f, headerRect.MH(), screwSize, screwSize, 45.f);
        } else {
          g.FillCircle(clrMetallicDark, headerRect.L + 20.f, headerRect.MH(), 6.f);
          g.FillCircle(clrMetallicDark, headerRect.R - 20.f, headerRect.MH(), 6.f);
        }
      }, -1, false));

    // HEADER: MK-01 branding with Scratched Letters font
    const IRECT headerTextRect = b.GetFromTop(kHeaderHeight);
    pGraphics->AttachControl(new ITextControl(headerTextRect, "MK-01",
      IText(static_cast<int>(28 * uiScale), clrTextMain, "Primary", EAlign::Center, EVAlign::Middle)));

    // Main content area
    const IRECT content = b.GetReducedFromTop(kHeaderHeight).GetReducedFromBottom(kFooterHeight);
    
    // INPUT PANEL (left) - Tether/Input controls
    const IRECT inputPanel = content.GetFromLeft(kPanelWidth).GetPadded(kPadding);
    
    // VU Meter
    const IRECT vuRect = inputPanel.GetFromTop(100.0f * uiScale).GetPadded(kGap);
    pGraphics->AttachControl(new IVMeterControl<1>(vuRect, "",
      DEFAULT_STYLE.WithColor(kFG, clrAccentGreen).WithColor(kBG, clrBgSecondary).WithDrawFrame(false),
      EDirection::Vertical, {}, 0.f, 1.f, 0.f), kNoParameter, "VU_METER");
    
    // VU Label
    const IRECT vuLabelRect = IRECT(vuRect.L, vuRect.B + 4.0f * uiScale, vuRect.R, vuRect.B + 16.0f * uiScale);
    pGraphics->AttachControl(new ITextControl(vuLabelRect, "LEVEL",
      IText(static_cast<int>(10 * uiScale), clrTextMuted, "Secondary", EAlign::Center, EVAlign::Middle)));
    
    // MIDI toggle
    const float midiW = 56.0f * uiScale;
    const float midiH = 24.0f * uiScale;
    const IRECT midiRect = IRECT(inputPanel.MW() - midiW * 0.5f, inputPanel.B - 48.0f * uiScale, inputPanel.MW() + midiW * 0.5f, inputPanel.B - 48.0f * uiScale + midiH);
    pGraphics->AttachControl(new IVToggleControl(midiRect, kMidiLearn, "MIDI",
      DEFAULT_STYLE.WithLabelText(IText(static_cast<int>(10 * uiScale), clrTextDim, "Secondary", EAlign::Center, EVAlign::Middle)).WithShowLabel(true)), kNoParameter, "MIDI_LEARN_TOGGLE");

    // OUTPUT PANEL (right) - Shatter/Output controls
    const IRECT outputPanel = content.GetFromRight(kPanelWidth).GetPadded(kPadding);
    
    // Shatter label
    const IRECT shatterLabelRect = outputPanel.GetFromTop(20.0f * uiScale);
    pGraphics->AttachControl(new ITextControl(shatterLabelRect, "SHATTER",
      IText(static_cast<int>(10 * uiScale), clrTextMuted, "Secondary", EAlign::Center, EVAlign::Middle)));
    
    // Load knob SVG
    ISVG knobSvg = pGraphics->LoadSVG(KNOB_SVG_FN);

    // SHATTER knob (main)
    const IVStyle shatterStyle = DEFAULT_STYLE
      .WithColor(kFG, clrAccentRed)
      .WithColor(kBG, clrBgSecondary)
      .WithShowLabel(false)
      .WithShowValue(false);

    const IRECT shatterKnobRect = outputPanel.GetCentredInside(kShatterSize).GetTranslated(0, 24.0f * uiScale);
    auto* pShatterKnob = new ClophSVGKnobControl(shatterKnobRect, kShatter, "", shatterStyle, knobSvg, clrAccentRed);
    pGraphics->AttachControl(pShatterKnob);
    mKeyboardOrder.push_back(pShatterKnob);

    // ARM toggle inline with shatter
    const IRECT armRect = IRECT(outputPanel.MW() - 28.0f * uiScale, shatterKnobRect.B - 28.0f * uiScale, outputPanel.MW() + 28.0f * uiScale, shatterKnobRect.B - 4.0f * uiScale);
    pGraphics->AttachControl(new IVToggleControl(armRect, kShatterArm, "ARM",
      DEFAULT_STYLE.WithLabelText(IText(static_cast<int>(10 * uiScale), clrTextDim, "Secondary", EAlign::Center, EVAlign::Middle)).WithShowLabel(true)), kNoParameter, "SHATTER_ARM");

    // Clip LED
    const IRECT clipRect = IRECT(outputPanel.R - 24.0f * uiScale, outputPanel.B - 24.0f * uiScale, outputPanel.R - 4.0f * uiScale, outputPanel.B - 4.0f * uiScale);
    pGraphics->AttachControl(new ILambdaControl(clipRect, [this, clrAccentRed, clrBgTertiary](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      const float peak = mPeak.load();
      if (peak > 0.85f) {
        g.FillCircle(clrAccentRed, r.MW(), r.MH(), r.W() * 0.4f);
        pCaller->StartAnimation(200);
        float glow = static_cast<float>(pCaller->GetAnimationProgress());
        g.DrawCircle(clrAccentRed.WithOpacity(0.4f * (1.0f - glow)), r.MW(), r.MH(), r.W() * 0.45f, nullptr, 2.0f);
      } else {
        g.FillCircle(clrBgTertiary, r.MW(), r.MH(), r.W() * 0.35f);
      }
    }, -1, false));

    // CENTER PANEL: Body Synthesis Controls - Tight grid layout
    const IRECT centerPanel = content.GetReducedFromLeft(kPanelWidth).GetReducedFromRight(kPanelWidth);
    
    // Grid: 3 columns x 2 rows with consistent spacing
    const float gridCols = 3.0f;
    const float gridRows = 2.0f;
    const float gridPadding = 16.0f * uiScale;
    const float gridGapX = 24.0f * uiScale;
    const float gridGapY = 24.0f * uiScale;
    
    const float gridW = centerPanel.W() - gridPadding * 2;
    const float gridH = centerPanel.H() - 48.0f * uiScale; // space for label
    const float knobW = (gridW - gridGapX * (gridCols - 1)) / gridCols;
    const float knobH = (gridH - gridGapY * (gridRows - 1)) / gridRows;
    const float knobSize = std::min(knobW, knobH);
    
    // Column positions
    const float col1 = centerPanel.L + gridPadding;
    const float col2 = col1 + knobSize + gridGapX;
    const float col3 = col2 + knobSize + gridGapX;
    
    // Row positions (top row = tuning, bottom row = character)
    const float row1 = centerPanel.T + 40.0f * uiScale;
    const float row2 = row1 + knobSize + gridGapY;
    
    // Group labels
    const IRECT labelRow1 = IRECT(centerPanel.L, centerPanel.T + 8.0f * uiScale, centerPanel.R, centerPanel.T + 28.0f * uiScale);
    pGraphics->AttachControl(new ITextControl(labelRow1, "TUNING",
      IText(static_cast<int>(10 * uiScale), clrTextMuted, "Secondary", EAlign::Center, EVAlign::Middle)));
    const IRECT labelRow2 = IRECT(centerPanel.L, row2 - 24.0f * uiScale, centerPanel.R, row2 - 4.0f * uiScale);
    pGraphics->AttachControl(new ITextControl(labelRow2, "CHARACTER",
      IText(static_cast<int>(10 * uiScale), clrTextMuted, "Secondary", EAlign::Center, EVAlign::Middle)));

    const IVStyle dialStyle = DEFAULT_STYLE
      .WithColor(kFG, clrAccentGreen)
      .WithColor(kBG, clrBgSecondary)
      .WithShowLabel(false)
      .WithShowValue(false);

    // Top row: TENSION | WEIGHT | PUNCH
    IRECT rTension = IRECT(col1, row1, col1 + knobSize, row1 + knobSize);
    IRECT rWeight = IRECT(col2, row1, col2 + knobSize, row1 + knobSize);
    IRECT rPunch = IRECT(col3, row1, col3 + knobSize, row1 + knobSize);
    
    // Bottom row: PANG | GRIT | (spacer)
    IRECT rPang = IRECT(col1, row2, col1 + knobSize, row2 + knobSize);
    IRECT rGrit = IRECT(col2, row2, col2 + knobSize, row2 + knobSize);

    auto* pTension = new ClophSVGKnobControl(rTension, kTension, "", dialStyle, knobSvg, clrAccentAmber);
    pGraphics->AttachControl(pTension);
    mKeyboardOrder.push_back(pTension);

    auto* pWeight = new ClophSVGKnobControl(rWeight, kWeight, "", dialStyle, knobSvg, clrTextMain);
    pGraphics->AttachControl(pWeight);
    mKeyboardOrder.push_back(pWeight);

    auto* pPunch = new ClophSVGKnobControl(rPunch, kPunch, "", dialStyle.WithColor(kFG, clrTextDim), knobSvg, clrAccentAmber);
    pGraphics->AttachControl(pPunch);
    mKeyboardOrder.push_back(pPunch);

    auto* pPang = new ClophSVGKnobControl(rPang, kPang, "", dialStyle, knobSvg, clrAccentGreen);
    pGraphics->AttachControl(pPang);
    mKeyboardOrder.push_back(pPang);

    auto* pGrit = new ClophSVGKnobControl(rGrit, kGrit, "", dialStyle, knobSvg, clrAccentRed);
    pGraphics->AttachControl(pGrit);
    mKeyboardOrder.push_back(pGrit);

    // Value readouts below each knob
    auto attachReadout = [&](const IRECT& knobRect, int paramIdx, IColor textColor) {
      const IRECT readRect = IRECT(knobRect.L, knobRect.B + 4.0f * uiScale, knobRect.R, knobRect.B + 20.0f * uiScale);
      pGraphics->AttachControl(new ILambdaControl(readRect, [this, paramIdx, textColor, uiScale](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
        char buf[32];
        if (paramIdx == kWeight) {
          WDL_String display;
          GetParam(paramIdx)->GetDisplay(display);
          strcpy(buf, display.Get());
        } else {
          sprintf(buf, "%.0f%%", GetParam(paramIdx)->Value() * 100.0);
        }
        g.DrawText(IText(static_cast<int>(11 * uiScale), textColor, "Secondary", EAlign::Center, EVAlign::Middle), buf, r);
      }, -1, false));
    };

    attachReadout(rTension, kTension, clrAccentAmber);
    attachReadout(rWeight, kWeight, clrTextMain);
    attachReadout(rPunch, kPunch, clrTextDim);
    attachReadout(rPang, kPang, clrAccentGreen);
    attachReadout(rGrit, kGrit, clrAccentRed);
    attachReadout(shatterKnobRect, kShatter, clrAccentRed);

    // FOOTER: Status bar
    const IRECT statusRect = b.GetFromBottom(kFooterHeight).GetPadded(kGap);
    pGraphics->AttachControl(new ILambdaControl(statusRect, [this, clrTextDim, clrAccentRed, uiScale](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      if (GetParam(kMidiLearn)->Bool()) {
        g.DrawText(IText(static_cast<int>(10 * uiScale), clrAccentRed, "Secondary", EAlign::Center, EVAlign::Middle), "WAITING FOR MIDI CC...", r);
        pCaller->StartAnimation(1000);
        float alpha = 0.5f + 0.5f * std::sin(pCaller->GetAnimationProgress() * 2.0 * M_PI);
        g.FillRect(clrAccentRed.WithOpacity(0.08f * alpha), r);
      } else {
        g.DrawText(IText(static_cast<int>(10 * uiScale), clrTextDim, "Secondary", EAlign::Center, EVAlign::Middle), "METALLIC SNARE // READY", r);
      }
    }, -1, false));

    // Hover tooltip
    const IRECT tipRect = IRECT(b.MW() - 110.0f, b.T + kHeaderHeight + 6.0f, b.MW() + 110.0f, b.T + kHeaderHeight + 32.0f);
    pGraphics->AttachControl(new ILambdaControl(tipRect, [this, clrTextMain, clrTextDim, clrBgTertiary, clrBorder, uiScale](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      const int idx = GetHoverParam();
      if (idx < 0 || idx >= kNumParams) return;
      const char* name = "";
      char valBuf[32] = {0};
      if (idx == kShatter) { name = "SHATTER"; sprintf(valBuf, "%d%%", static_cast<int>(std::round(GetParam(idx)->Value() * 100.0))); }
      else if (idx == kTension) { name = "TENSION"; sprintf(valBuf, "%.0f%%", GetParam(idx)->Value() * 100.0); }
      else if (idx == kPang) { name = "PANG"; sprintf(valBuf, "%.0f%%", GetParam(idx)->Value() * 100.0); }
      else if (idx == kGrit) { name = "GRIT"; sprintf(valBuf, "%.0f%%", GetParam(idx)->Value() * 100.0); }
      else if (idx == kPunch) { name = "PUNCH"; sprintf(valBuf, "%.0f%%", GetParam(idx)->Value() * 100.0); }
      else if (idx == kWeight) { name = "WEIGHT"; WDL_String display; GetParam(idx)->GetDisplay(display); sprintf(valBuf, "%s", display.Get()); }

      g.PathRoundRect(r.GetPadded(6.0f), 6.0f);
      g.PathFill(clrBgTertiary);
      g.PathStroke(clrBorder, 1.0f);

      IRECT nameRect = IRECT(r.L + 10.f, r.T, r.MW(), r.B);
      IRECT valRect = IRECT(r.MW(), r.T, r.R - 10.f, r.B);
      g.DrawText(IText(static_cast<int>(11 * uiScale), clrTextDim, "Secondary", EAlign::Near, EVAlign::Middle), name, nameRect);
      g.DrawText(IText(static_cast<int>(12 * uiScale), clrTextMain, "Secondary", EAlign::Far, EVAlign::Middle), valBuf, valRect);
    }, -1, false));

    // Keyboard handler
    pGraphics->SetKeyHandlerFunc([this](const IKeyPress& key, bool isUp) -> bool {
      if (mKeyboardFocusIdx < 0 || mKeyboardFocusIdx >= static_cast<int>(mKeyboardOrder.size())) return false;
      if (isUp) return false;
      IControl* c = mKeyboardOrder[mKeyboardFocusIdx];
      return c->OnKeyDown(0.0f, 0.0f, key);
    });
  };
#endif
}

void MetallicKnobs::OnReset()
{
  mSampleRate = GetSampleRate();
  mPeak.store(0.0f);
  mHoverParam.store(-1);
  mImpact.Prepare(mSampleRate);
  mSnap.Prepare(mSampleRate);
  const float delayTimes[3] = { 2.0f, 5.3f, 11.1f };
  for (int i = 0; i < 3; i++) {
    mDiffuser[i].Prepare(mSampleRate, delayTimes[i]);
  }
  mFilter.Prepare(mSampleRate);
  mRingMod.Prepare(mSampleRate);
  mComb.Prepare(mSampleRate, 3.7f);
  mHFShelf.Prepare(mSampleRate);
  for (int i = 0; i < NUM_MODES; i++) {
    mPhases[i] = 0.0;
    mAmplitudes[i] = 0.0;
  }
}

void MetallicKnobs::CycleKeyboardFocus(bool forward)
{
  if (mKeyboardOrder.empty()) return;
  if (mKeyboardFocusIdx < 0) {
    mKeyboardFocusIdx = 0;
  } else {
    if (forward) mKeyboardFocusIdx = (mKeyboardFocusIdx + 1) % static_cast<int>(mKeyboardOrder.size());
    else mKeyboardFocusIdx = (mKeyboardFocusIdx - 1 + static_cast<int>(mKeyboardOrder.size())) % static_cast<int>(mKeyboardOrder.size());
  }
  IControl* c = mKeyboardOrder[mKeyboardFocusIdx];
  ClophSVGKnobControl::SetFocusedControl(static_cast<ClophSVGKnobControl*>(c));
}

void MetallicKnobs::SetKeyboardFocusToControl(IControl* c)
{
  for (int i = 0; i < static_cast<int>(mKeyboardOrder.size()); ++i) {
    if (mKeyboardOrder[i] == c) {
      mKeyboardFocusIdx = i;
      ClophSVGKnobControl::SetFocusedControl(static_cast<ClophSVGKnobControl*>(c));
      return;
    }
  }
  mKeyboardFocusIdx = -1;
  ClophSVGKnobControl::SetFocusedControl(nullptr);
}

void MetallicKnobs::ProcessMidiMsg(const IMidiMsg& msg)
{
  if (msg.StatusMsg() == IMidiMsg::kControlChange)
  {
    if (GetParam(kMidiLearn)->Bool())
    {
      // Finalize mapping here in a real implementation
      // For now, toggle MIDI LEARN off once a CC is received
      SetParameterValue(kMidiLearn, 0.0);
    }
  }

  if (msg.StatusMsg() == IMidiMsg::kNoteOn && msg.Velocity() > 0)
  {
    mVelocity = (double)msg.Velocity() / 127.0;
    mImpact.Trigger();
    mSnap.Trigger();
    mRingMod.Trigger();
    mComb.Reset();
    mHFShelf.Reset();
    for (int i = 0; i < NUM_MODES; i++) {
      mPhases[i] = 0.0;
      mAmplitudes[i] = 1.0; 
    }
    for (int i = 0; i < 2; i++) {
        mShellPhases[i] = 0.0;
        mShellAmplitudes[i] = 0.5;
    }
    mFundamentalFreq = 440.0 * std::pow(2.0, (double)(msg.NoteNumber() - 69) / 12.0);
    mEnvBody = 1.0; 
    mPitchEnv = 1.0;
  }
}

void MetallicKnobs::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();
  const float shatterRaw = static_cast<float>(GetParam(kShatter)->Value()); 
  const bool shatterArmed = GetParam(kShatterArm)->Value() > 0.5;
  const float shatter = shatterArmed ? shatterRaw : std::min(shatterRaw, 0.5f);
  const float tension = static_cast<float>(GetParam(kTension)->Value()); 
  const float pang = static_cast<float>(GetParam(kPang)->Value());      
  const float grit = static_cast<float>(GetParam(kGrit)->Value());       
  const float punch = static_cast<float>(GetParam(kPunch)->Value());     
  // Weight is quantized to semitone steps (enum). Convert normalized enum value
  // to an integer semitone offset in [-18, +18] and map to frequency ratio.
  const double weightNorm = GetParam(kWeight)->Value();
  const int kWeightSteps = 37; // -18 .. +18
  const int weightIdx = static_cast<int>(std::round(weightNorm * (kWeightSteps - 1)));
  const int semitone = weightIdx - (kWeightSteps - 1) / 2; // center at 0

  // Nonlinear, perceptual mapping for a wider and more musical sweep
  // 0.5s at lowest tension, 0.005s (very choked) at max tension using an exponential curve
  const double bodyDecayTime = 0.5 * std::pow(0.01, tension);
  const float bodyDecay = static_cast<float>(std::exp(-1.0 / (bodyDecayTime * mSampleRate)));
  const float pitchDecay = static_cast<float>(std::exp(-1.0 / (0.012 * mSampleRate))); 

  // Modal damping: log scaling for a natural roll-off of high frequencies
  const double harmonicDamping = std::pow(tension, 0.4); 
  const float harmonicDampingF = static_cast<float>(harmonicDamping);

  // Weight centers the fundamental frequency around the MIDI note
  const double weightOffset = std::pow(2.0, static_cast<double>(semitone) / 12.0);
  const float f1PitchBase = static_cast<float>(mFundamentalFreq * weightOffset);

  // Pre-calculate loop constants with perceptually smooth squaring for intensity knobs
  const float punchSq = punch * punch;
  const float punchScale = punchSq * 24.0f; // More extreme drop, but nuanced at low values
  const float gritSq = grit * grit;
  const float gritScaled = 0.2f + gritSq * 1.2f;
  const float tensionSplashScale = 1.8f - tension * 1.2f; // Much splashier when loose
  const float pangSq = pang * pang;
  const float diffuserGain = 0.2f + pangSq * 0.6f; // Wider diffusion sweep (max 0.8 to void feedback loops)
  const float filterRes = 0.5f + pangSq * 2.5f; // More "ring" at max pang
  const float filterDrive = 1.0f + gritSq * 8.0f;
  const float sigmoidDrive = 1.0f + gritSq * 20.0f; // More aggressive saturation curve
  
  // Metallic sound parameters
  const float ringModDepth = pang * pang * 0.6f; // Ring mod for clangorous harmonics
  const float ringModFreq = 2500.0f + pangSq * 8000.0f; // High freq modulation for metallic edge
  const float combFeedback = gritSq * 0.7f; // Comb filter resonance
  const float combDamping = 0.85f - gritSq * 0.3f;
  const float hfBoost = pangSq * 8.0f; // High freq shelf boost for shimmer
  
  const float invSampleRate = 1.0f / static_cast<float>(mSampleRate);
  const float velocityF = static_cast<float>(mVelocity);
  const float twoPi = 2.0f * static_cast<float>(M_PI);

  float localPeak = 0.0f;
  for (int s = 0; s < nFrames; s++) {
    mEnvBody *= bodyDecay;
    mPitchEnv *= pitchDecay;
    
    // Denormal protection
    if (mEnvBody < DENORMAL_GUARD) mEnvBody = 0.0f;
    if (mPitchEnv < DENORMAL_GUARD) mPitchEnv = 0.0f;

    const float envBodyF = static_cast<float>(mEnvBody);
    const float pitchEnvF = static_cast<float>(mPitchEnv);

    // 1. Excitation logic
    float impact = mImpact.Process();
    float snap = mSnap.Process() * gritScaled * tensionSplashScale;
    float excitation = impact + snap;
    
    // 2. Diffusion
    float diffused = excitation;
    for (int i = 0; i < 3; i++) {
        diffused = mDiffuser[i].Process(diffused, diffuserGain);
    }
    
    // 3. Modal Sum
    const float pEnvSq = pitchEnvF * pitchEnvF;
    const float f1Pitch = f1PitchBase * (1.0f + pEnvSq * punchScale);
    
    float modalSum = 0.0f;
    for (int i = 0; i < NUM_MODES; i++) {
      const float modeFreq = f1Pitch * static_cast<float>(MODE_RATIOS[i]);
      mPhases[i] += modeFreq * invSampleRate;
      if (mPhases[i] >= 1.0f) mPhases[i] -= 1.0f;
      
      float damping = std::pow(harmonicDampingF, static_cast<float>(i) * 0.5f);
      float modeAmp = envBodyF * (1.0f / (1.0f + i * 0.2f)) * damping;
      modalSum += std::sin(static_cast<float>(mPhases[i]) * twoPi) * modeAmp;
    }

    // Shell Resonances
    for (int i = 0; i < 2; i++) {
        const float shellFreq = static_cast<float>(SHELL_MODES[i]) * (0.8f + static_cast<float>(weightNorm) * 0.4f);
        mShellPhases[i] += shellFreq * invSampleRate;
        if (mShellPhases[i] >= 1.0f) mShellPhases[i] -= 1.0f;
        
        modalSum += std::sin(static_cast<float>(mShellPhases[i]) * twoPi) * envBodyF * pang * 0.05f;
    }

    // Apply combined excitation/diffusion to the total modal sum
    modalSum = modalSum * (0.05f + std::abs(diffused) * 0.8f);
    
    // 4. Nonlinear Filter
    float cutoff = 2000.0f + pEnvSq * 12000.0f;
    float filterOut = mFilter.Process(modalSum, cutoff, filterRes, filterDrive);
    
    // 5. Ring Modulation for metallic clangorous harmonics
    float ringModOut = mRingMod.Process(filterOut, ringModFreq, ringModDepth);
    
    // 6. Comb Filter for metallic resonance/ringing
    float combOut = mComb.Process(ringModOut, combFeedback, combDamping);
    
    // 7. HF Emphasis for shimmer
    float hfOut = mHFShelf.Process(combOut, hfBoost);
    
    // 8. Brutality Shaper
    float shaped = AsymmetricSigmoid::Process(hfOut, sigmoidDrive);
    
    // 9. Shatter
    float shattered = mShatter.Process(shaped, shatter, envBodyF);
    
    float outMix = shattered * velocityF;

    for (int c = 0; c < nChans; c++) {
      if (outputs[c]) outputs[c][s] = outMix * 0.5f;
    }
    // Track peak for UI clip indicator (audio thread safe via atomic)
    localPeak = std::max(localPeak, std::abs(outMix));
  }

  // Smooth and decay previous peak for display
  const float prevPeak = mPeak.load();
  const float newPeak = std::max(prevPeak * 0.96f, localPeak);
  mPeak.store(newPeak);
}
