#include "MetallicKnobs.h"
#include "config.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include <array>
#include <random>

// Custom SVG Knob that retains IVKnobControl's label/value and interaction logic
class ClophSVGKnobControl : public IVKnobControl {
private:
  ISVG mSVG;
  static ClophSVGKnobControl* sFocused;
public:
  ClophSVGKnobControl(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, const ISVG& svg)
  : IVKnobControl(bounds, paramIdx, label, style), mSVG(svg) { }

  static void SetFocusedControl(ClophSVGKnobControl* c) { sFocused = c; }
  bool HasKeyboardFocus() const { return sFocused == this; }

  void DrawWidget(IGraphics& g) override {
    if (mSVG.IsValid()) {
      const float cx = mWidgetBounds.MW();
      const float cy = mWidgetBounds.MH();
      const float angle = mAngle1 + (static_cast<float>(GetValue()) * (mAngle2 - mAngle1));
      const float sz = std::min(mWidgetBounds.W(), mWidgetBounds.H());
      g.DrawRotatedSVG(mSVG, cx, cy, sz, sz, angle);
    } else {
      IVKnobControl::DrawWidget(g); // Fallback to flat vector rendering
    }

    // Draw keyboard focus ring when focused
    if (HasKeyboardFocus()) {
      IColor clrFocus = IColor(255, 120, 200, 255);
      const float cx = mWidgetBounds.MW();
      const float cy = mWidgetBounds.MH();
      const float radius = std::min(mWidgetBounds.W(), mWidgetBounds.H()) * 0.5f + 6.0f;
      g.DrawCircle(clrFocus.WithOpacity(0.9f), cx, cy, radius, nullptr, 3.0f);
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
    pGraphics->LoadFont("Arial", "Arial", ETextStyle::Normal);
    pGraphics->LoadFont("Courier", "Courier New", ETextStyle::Normal);
    
    // Dark Titanium Palette
    const IColor clrTitaniumBase = IColor(255, 30, 32, 35);
    const IColor clrTitaniumDark = IColor(255, 18, 19, 21);
    const IColor clrTitaniumLight = IColor(255, 60, 62, 65);
    const IColor clrRackEar = IColor(255, 20, 20, 20);
    const IColor clrShatterRed = IColor(255, 200, 30, 40);
    const IColor clrVuGreen = IColor(255, 40, 200, 80);
    const IColor clrTextMain = IColor(255, 220, 220, 220);
    const IColor clrTextDim = IColor(255, 130, 130, 135);

    // Responsive spacing tokens (scale to current bounds)
    const IRECT b = pGraphics->GetBounds();
    const float uiScale = std::min(b.W() / static_cast<float>(PLUG_WIDTH), b.H() / static_cast<float>(PLUG_HEIGHT));
    const float kEarWidth = 24.0f * uiScale;
    const float kTelemetryWidth = 80.0f * uiScale;
    const float kShatterWidth = 220.0f * uiScale;
    const float kMainPadding = 12.0f * uiScale;
    const float kLargeKnobSize = 120.0f * uiScale;
    const float kShatterKnobSize = 180.0f * uiScale;

    // Reset keyboard-focusable control list for this layout
    mKeyboardOrder.clear();
    mKeyboardFocusIdx = -1;

    // Load screw SVG and generate per-instance random rotation angles
    ISVG screwSvg = pGraphics->LoadSVG(SCREW_SVG_FN);
    if (!screwSvg.IsValid()) {
      WDL_String fallbackPath;
      fallbackPath.SetFormatted(256, "%s/%s", SHARED_RESOURCES_SUBPATH, SCREW_SVG_FN);
      screwSvg = pGraphics->LoadSVG(fallbackPath.Get());
    }
    std::array<float,4> screwAngles;
    {
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_real_distribution<float> dist(0.0f, 360.0f);
      for (int i = 0; i < 4; ++i) screwAngles[i] = dist(rng);
    }

    // 19-inch Rackmount Background with Linear Gradients
    pGraphics->AttachControl(new ILambdaControl(pGraphics->GetBounds(), 
      [clrTitaniumBase, clrTitaniumDark, clrRackEar, kEarWidth, kShatterWidth, screwSvg, screwAngles](ILambdaControl* pCaller, IGraphics& g, IRECT& b) {
        
        // Linear gradient background
        IPattern pattern = IPattern::CreateLinearGradient(b.L, b.T, b.L, b.B, {
            {clrTitaniumDark, 0.0f},
            {clrTitaniumBase, 0.5f},
            {clrTitaniumDark, 1.0f}
        });
        g.PathRect(b);
        g.PathFill(pattern);
        
        // Rack Ears
        g.FillRect(clrRackEar, b.GetFromLeft(kEarWidth));
        g.FillRect(clrRackEar, b.GetFromRight(kEarWidth));
        
        // Screws (SVG heads, rotated randomly per instance)
        IColor clrScrew = IColor(255, 50, 50, 50);
        const float screwRadius = 10.0f * (kEarWidth / 24.0f); // scale with ui
        const float screwSize = screwRadius * 2.0f;
        const float sx[4] = { b.L + kEarWidth / 2.0f, b.L + kEarWidth / 2.0f, b.R - kEarWidth / 2.0f, b.R - kEarWidth / 2.0f };
        const float sy[4] = { b.T + 40.0f, b.B - 40.0f, b.T + 40.0f, b.B - 40.0f };
        for (int i = 0; i < 4; ++i) {
          if (screwSvg.IsValid()) {
            g.DrawRotatedSVG(screwSvg, sx[i], sy[i], screwSize, screwSize, screwAngles[i]);
          } else {
            g.DrawCircle(clrScrew, sx[i], sy[i], screwRadius, nullptr, 3.0f);
          }
        }

        // Rack Panel Dividers
        g.DrawLine(clrTitaniumDark, b.L + kEarWidth + 100.0f, b.T, b.L + kEarWidth + 100.0f, b.B, nullptr, 3.f);
        g.DrawLine(clrTitaniumDark, b.R - kEarWidth - (kShatterWidth), b.T, b.R - kEarWidth - (kShatterWidth), b.B, nullptr, 3.f);
      }, -1, false));

    const IRECT innerBounds = b.GetReducedFromLeft(kEarWidth).GetReducedFromRight(kEarWidth);

    // LEFT: Telemetry/VU Sidebar
    const IRECT telemetryZone = innerBounds.GetFromLeft(kTelemetryWidth);
    const IRECT vuMeterRect = telemetryZone.GetCentredInside(24).GetReducedFromTop(60).GetReducedFromBottom(60);
    pGraphics->AttachControl(new IVMeterControl<1>(vuMeterRect, "",
      DEFAULT_STYLE.WithColor(kFG, clrVuGreen).WithColor(kBG, clrTitaniumDark).WithDrawFrame(false),
      EDirection::Vertical, {}, 0.f, 1.f, 0.f), kNoParameter, "VU_METER");

    // BOTTOM/HEADER: Snare Brand
    pGraphics->AttachControl(new ITextControl(telemetryZone.GetFromTop(70).GetTranslated(0, 20), 
      "MK-01", IText(28, clrTextDim, "Arial", EAlign::Center, EVAlign::Middle)));

    // Interactive Status Bar (Center-Bottom)
    const IRECT statusRect = innerBounds.GetFromBottom(30.0f).GetPadded(-10.0f);
    pGraphics->AttachControl(new ILambdaControl(statusRect, [this, clrTextDim, clrShatterRed](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      if (GetParam(kMidiLearn)->Bool()) {
         g.DrawText(IText(12, clrShatterRed, "Arial", EAlign::Center, EVAlign::Middle), "WAITING FOR MIDI CC...", r);
         pCaller->StartAnimation(1000);
         float alpha = 0.5f + 0.5f * std::sin(pCaller->GetAnimationProgress() * 2.0 * M_PI);
         g.FillRect(clrShatterRed.WithOpacity(0.1f * alpha), r);
      } else {
         g.DrawText(IText(10, clrTextDim, "Arial", EAlign::Near, EVAlign::Middle), "METALLIC SNARE MK-01 // READY", r);
      }
    }, -1, false));

    // Hover tooltip: shows param name + value when pointer is over a control
    const IRECT tipRect = IRECT(innerBounds.MW() - 140.0f, innerBounds.T + 8.0f, innerBounds.MW() + 140.0f, innerBounds.T + 40.0f);
    pGraphics->AttachControl(new ILambdaControl(tipRect, [this, clrTextMain, clrTextDim](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      const int idx = GetHoverParam();
      if (idx < 0 || idx >= kNumParams) return;
      const char* name = "";
      char valBuf[64] = {0};
      if (idx == kShatter) {
        name = "SHATTER"; sprintf(valBuf, "%d%%", static_cast<int>(std::round(GetParam(idx)->Value() * 100.0)));
      } else if (idx == kTension) { name = "TENSION"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kPang) { name = "PANG"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kGrit) { name = "GRIT"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kPunch) { name = "PUNCH"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kWeight) { 
        WDL_String display;
        GetParam(idx)->GetDisplay(display);
        sprintf(valBuf, "%s", display.Get());
      }

      // Background pill
      g.PathRoundRect(r.GetPadded(6.0f), 6.0f);
      g.PathFill(IPattern::CreateLinearGradient(r.L, r.T, r.L, r.B, {{IColor(220,20,20,20), 0.0f}, {IColor(200,30,30,30), 1.0f}}));

      // Draw name + value
      IRECT nameRect = IRECT(r.L + 8.0f, r.T, r.MW(), r.B);
      IRECT valRect = IRECT(r.MW(), r.T, r.R - 8.0f, r.B);
      g.DrawText(IText(14, clrTextDim, "Arial", EAlign::Near, EVAlign::Middle), name, nameRect);
      g.DrawText(IText(14, clrTextMain, "Courier", EAlign::Far, EVAlign::Middle), valBuf, valRect);
    }, -1, false));

    // MIDI learn toggle in telemetry zone
    const float midiW = 64.0f * uiScale;
    const float midiH = 26.0f * uiScale;
    const IRECT midiRect = IRECT(telemetryZone.MW() - midiW * 0.5f, telemetryZone.B - 64.0f * uiScale, telemetryZone.MW() + midiW * 0.5f, telemetryZone.B - 64.0f * uiScale + midiH);
    pGraphics->AttachControl(new IVToggleControl(midiRect, kMidiLearn, "MIDI", DEFAULT_STYLE.WithLabelText(IText(12, clrTextDim, "Arial", EAlign::Center, EVAlign::Middle)).WithShowLabel(true)), kNoParameter, "MIDI_LEARN_TOGGLE");

    // RIGHT: Massive Shatter Zone
    const IRECT shatterZone = innerBounds.GetFromRight(kShatterWidth);
    
    // Warning background overlay
    pGraphics->AttachControl(new ILambdaControl(shatterZone, 
      [clrShatterRed](ILambdaControl* pCaller, IGraphics& g, IRECT& b) {
        g.DrawLine(clrShatterRed.WithOpacity(0.3f), b.L + 20.0f, b.T + 20.0f, b.L + 20.0f, b.B - 20.0f, nullptr, 6.0f);
        g.DrawLine(clrShatterRed.WithOpacity(0.3f), b.R - 20.0f, b.T + 20.0f, b.R - 20.0f, b.B - 20.0f, nullptr, 6.0f);
      }, -1, false));

    ISVG knobSvg = pGraphics->LoadSVG(KNOB_SVG_FN);

    // Red styled, massive knob for SHATTER
    const IVStyle shatterStyle = DEFAULT_STYLE
      .WithColor(kFG, clrShatterRed)
      .WithColor(kBG, clrTitaniumDark)
      .WithLabelText(IText(32, clrShatterRed, "Arial", EAlign::Center, EVAlign::Top))
      .WithValueText(IText(22, clrTextMain, "Courier", EAlign::Center, EVAlign::Bottom))
      .WithShowLabel(false)
      .WithShowValue(false)
      .WithWidgetFrac(1.0f);

    const IRECT shatterKnobRect = shatterZone.GetCentredInside(kShatterKnobSize).GetTranslated(0, 10);
    {
      auto* pShatterKnob = new ClophSVGKnobControl(shatterKnobRect, kShatter, "", shatterStyle, knobSvg);
      pGraphics->AttachControl(pShatterKnob);
      mKeyboardOrder.push_back(pShatterKnob);
    }

    // SHATTER ARM toggle (safe / armed) - explicit arming for full destructive range
    const IRECT armRect = IRECT(shatterZone.R - (84.0f * std::min(pGraphics->GetBounds().W() / static_cast<float>(PLUG_WIDTH), pGraphics->GetBounds().H() / static_cast<float>(PLUG_HEIGHT))), shatterZone.T + 12.0f, shatterZone.R - 12.0f, shatterZone.T + 48.0f);
    pGraphics->AttachControl(new IVToggleControl(armRect, kShatterArm, "ARM", DEFAULT_STYLE.WithLabelText(IText(12, clrTextDim, "Arial", EAlign::Center, EVAlign::Middle)).WithShowLabel(true)), kNoParameter, "SHATTER_ARM");

    // Clip LED (reads from audio thread peak)
    const float uiScaleForClip = std::min(pGraphics->GetBounds().W() / static_cast<float>(PLUG_WIDTH), pGraphics->GetBounds().H() / static_cast<float>(PLUG_HEIGHT));
    const IRECT clipRect = IRECT(shatterZone.R - 40.0f * uiScaleForClip, shatterZone.B - 40.0f * uiScaleForClip, shatterZone.R - 12.0f * uiScaleForClip, shatterZone.B - 12.0f * uiScaleForClip);
    pGraphics->AttachControl(new ILambdaControl(clipRect, [this, clrShatterRed](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      const float peak = mPeak.load();
      if (peak > 0.85f) {
        g.FillCircle(clrShatterRed, r.MW(), r.MH(), std::max(6.f, r.W() * 0.25f));
        pCaller->StartAnimation(200); // Glow decay
        float glow = static_cast<float>(pCaller->GetAnimationProgress());
        g.DrawCircle(clrShatterRed.WithOpacity(0.4f * (1.0f - glow)), r.MW(), r.MH(), r.W() * 0.4f, nullptr, 2.0f);
      } else {
        g.FillCircle(IColor(255, 40, 40, 40), r.MW(), r.MH(), std::max(4.f, r.W() * 0.18f));
      }
    }, -1, false));

    // CENTER: Control Board (Dials)
    // Area left between Telemetry and Shatter
    const IRECT mainBoard = innerBounds.GetReducedFromLeft(kTelemetryWidth).GetReducedFromRight(kShatterWidth).GetPadded(-kMainPadding);

    const IVStyle dialStyle = DEFAULT_STYLE
      .WithColor(kFG, clrVuGreen)
      .WithColor(kBG, clrTitaniumDark)
      .WithLabelText(IText(18, clrTextDim, "Courier", EAlign::Center, EVAlign::Top))
      .WithValueText(IText(14, clrVuGreen, "Courier", EAlign::Center, EVAlign::Bottom))
      .WithShowLabel(false)
      .WithShowValue(false)
      .WithWidgetFrac(0.85f);

    // 5 knobs, arranged in a dense, staggered layout to use the space effectively
    // Let's do a large 2x2 grid for the main 4, and the 5th (PANG) centered at the bottom or top
    // For visual rhythm:
    // TENSION   WEIGHT
    //   PUNCH
    // PANG      GRIT
    
    float rowH = mainBoard.H() / 3.0f;
    float colW = mainBoard.W() / 2.0f;
    float sz = kLargeKnobSize; // prominent size for inner dials
    
    // Top Row
    IRECT rTension = IRECT(mainBoard.L, mainBoard.T, mainBoard.L + colW, mainBoard.T + rowH).GetCentredInside(sz).GetTranslated(0, 20);
    IRECT rWeight = IRECT(mainBoard.L + colW, mainBoard.T, mainBoard.R, mainBoard.T + rowH).GetCentredInside(sz).GetTranslated(0, 20);
    
    // Middle Row (Centered)
    IRECT rPunch = IRECT(mainBoard.L, mainBoard.T + rowH, mainBoard.R, mainBoard.T + rowH * 2).GetCentredInside(sz + 30.0f); // Slightly larger center
    
    // Bottom Row
    IRECT rPang = IRECT(mainBoard.L, mainBoard.T + rowH * 2, mainBoard.L + colW, mainBoard.B).GetCentredInside(sz).GetTranslated(0, -20);
    IRECT rGrit = IRECT(mainBoard.L + colW, mainBoard.T + rowH * 2, mainBoard.R, mainBoard.B).GetCentredInside(sz).GetTranslated(0, -20);

    {
      auto* pTension = new ClophSVGKnobControl(rTension, kTension, "", dialStyle, knobSvg);
      pGraphics->AttachControl(pTension);
      mKeyboardOrder.push_back(pTension);

      auto* pWeight = new ClophSVGKnobControl(rWeight, kWeight, "", dialStyle, knobSvg);
      pGraphics->AttachControl(pWeight);
      mKeyboardOrder.push_back(pWeight);

      auto* pPunch = new ClophSVGKnobControl(rPunch, kPunch, "", dialStyle.WithColor(kFG, clrTitaniumLight), knobSvg);
      pGraphics->AttachControl(pPunch);
      mKeyboardOrder.push_back(pPunch);

      auto* pPang = new ClophSVGKnobControl(rPang, kPang, "", dialStyle, knobSvg);
      pGraphics->AttachControl(pPang);
      mKeyboardOrder.push_back(pPang);

      auto* pGrit = new ClophSVGKnobControl(rGrit, kGrit, "", dialStyle, knobSvg);
      pGraphics->AttachControl(pGrit);
      mKeyboardOrder.push_back(pGrit);
    }

    // Numeric readouts under knobs (improves discoverability & precision)
    auto attachReadout = [&](const IRECT& knobRect, int paramIdx) {
      const IRECT readRect = IRECT(knobRect.L, knobRect.B - 28.0f * uiScale, knobRect.R, knobRect.B);
      pGraphics->AttachControl(new ILambdaControl(readRect, [this, paramIdx, clrTextMain](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
        char buf[64];
        if (paramIdx == kWeight) {
          WDL_String display;
          GetParam(paramIdx)->GetDisplay(display);
          strcpy(buf, display.Get());
        } else if (paramIdx == kShatter) {
          sprintf(buf, "%d%%", static_cast<int>(std::round(GetParam(paramIdx)->Value() * 100.0)));
        } else {
          sprintf(buf, "%.2f", GetParam(paramIdx)->Value());
        }
        g.DrawText(IText(12, clrTextMain, "Courier", EAlign::Center, EVAlign::Middle), buf, r);
      }, -1, false));
    };

    attachReadout(shatterKnobRect, kShatter);
    attachReadout(rTension, kTension);
    attachReadout(rWeight, kWeight);
    attachReadout(rPunch, kPunch);
    attachReadout(rPang, kPang);
    attachReadout(rGrit, kGrit);

    // Global key handler: forward key events to the currently focused control (if any)
    pGraphics->SetKeyHandlerFunc([this](const IKeyPress& key, bool isUp) -> bool {
      if (mKeyboardFocusIdx < 0 || mKeyboardFocusIdx >= static_cast<int>(mKeyboardOrder.size())) return false;
      if (isUp) return false; // only handle key-down events here
      IControl* c = mKeyboardOrder[mKeyboardFocusIdx];
      return c->OnKeyDown(0.0f, 0.0f, key);
    });

    // Keyboard-visible tooltip for focused control (ARIA-like label/value)
    const IRECT focusTipRect = IRECT(innerBounds.MW() - 140.0f, innerBounds.T + 44.0f, innerBounds.MW() + 140.0f, innerBounds.T + 76.0f);
    pGraphics->AttachControl(new ILambdaControl(focusTipRect, [this, clrTextMain, clrTextDim](ILambdaControl* pCaller, IGraphics& g, IRECT& r) {
      if (mKeyboardFocusIdx < 0 || mKeyboardFocusIdx >= static_cast<int>(mKeyboardOrder.size())) return;
      IControl* c = mKeyboardOrder[mKeyboardFocusIdx];
      IVKnobControl* knob = static_cast<IVKnobControl*>(c);
      const int idx = knob->GetParamIdx();
      const char* name = "";
      char valBuf[64] = {0};
      if (idx == kShatter) { name = "SHATTER"; sprintf(valBuf, "%d%%", static_cast<int>(std::round(GetParam(idx)->Value() * 100.0))); }
      else if (idx == kTension) { name = "TENSION"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kPang) { name = "PANG"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kGrit) { name = "GRIT"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kPunch) { name = "PUNCH"; sprintf(valBuf, "%.2f", GetParam(idx)->Value()); }
      else if (idx == kWeight) { WDL_String display; GetParam(idx)->GetDisplay(display); sprintf(valBuf, "%s", display.Get()); }

      // Background pill
      g.PathRoundRect(r.GetPadded(6.0f), 6.0f);
      g.PathFill(IPattern::CreateLinearGradient(r.L, r.T, r.L, r.B, {{IColor(220,20,20,20), 0.0f}, {IColor(200,30,30,30), 1.0f}}));

      // Draw name + value
      IRECT nameRect = IRECT(r.L + 8.0f, r.T, r.MW(), r.B);
      IRECT valRect = IRECT(r.MW(), r.T, r.R - 8.0f, r.B);
      g.DrawText(IText(14, clrTextDim, "Arial", EAlign::Near, EVAlign::Middle), name, nameRect);
      g.DrawText(IText(14, clrTextMain, "Courier", EAlign::Far, EVAlign::Middle), valBuf, valRect);
    }, -1, false));
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
