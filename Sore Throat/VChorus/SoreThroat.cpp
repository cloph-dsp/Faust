#include "SoreThroat.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "IVDisplay.h"
#include "IPlugPaths.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <immintrin.h>

namespace
{
constexpr float kMaxInternalDelayMs = 5000.f;
constexpr float kMaxRateSamplesPerSample = 0.95f;
constexpr int kFastInterpolationVoiceThreshold = 96;
constexpr double kSilenceThreshold = 1.0e-20;

struct StereoMacroState
{
  double rotationRad = 0.0;
  float widthFactor = 1.0f;
};

inline StereoMacroState StereoMacroStateFromValue(float stereoVal)
{
  StereoMacroState state;

  if (stereoVal < 0.f)
  {
    state.rotationRad = static_cast<double>(stereoVal) * 1.8 * PI / 180.0;
  }
  else
  {
    state.widthFactor = 1.0f + (stereoVal * 0.01f);
  }

  return state;
}

inline float DemonicFeedbackShape(float input, float drive, float asymmetry)
{
  const float driven = input * drive;
  const float edge = driven * driven;
  const float shaped = (driven >= 0.f)
    ? (driven + (asymmetry * edge))
    : (driven - (0.58f * asymmetry * edge));

  return static_cast<float>(std::tanh(static_cast<double>(shaped)));
}

inline double CoeffFromTimeMs(double sampleRate, double timeMs)
{
  const double t = std::max(0.1, timeMs);
  return 1.0 - std::exp(-1.0 / (0.001 * t * sampleRate));
}

inline const char* TempoSourceHint(int unit)
{
  if (unit == kTimeNoteSyncHost)
    return "Tempo Source: Host Sync follows your DAW BPM.";

  if (unit == kTimeNoteSyncManualBPM)
    return "Tempo Source: Manual Sync uses Manual BPM.";

  return "Tempo Source: Free-run mode (tempo sync off).";
}

bool LoadBundledFont(IGraphics* g, const char* fontID, const char* fileNameOrResID)
{
  if (!g || !fontID || !fontID[0] || !fileNameOrResID || !fileNameOrResID[0])
    return false;

#ifdef OS_WIN
  if (g->GetWinModuleHandle())
  {
    WDL_String resolvedResourceID;
    const EResourceLocation resourceLocation = LocateResource(fileNameOrResID, "ttf", resolvedResourceID,
      g->GetBundleID(), g->GetWinModuleHandle(), nullptr);

    if (resourceLocation == EResourceLocation::kWinBinary)
    {
      int dataSize = 0;
      void* fontData = const_cast<void*>(LoadWinResource(resolvedResourceID.Get(), "ttf", dataSize, g->GetWinModuleHandle()));

      if (fontData && dataSize > 0 && g->LoadFont(fontID, fontData, dataSize))
        return true;
    }
  }
#endif

  return g->LoadFont(fontID, fileNameOrResID);
}

class IVPopupSelectorControl final : public IControl
                                  , public IVectorBase
{
public:
  IVPopupSelectorControl(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE)
  : IControl(bounds, paramIdx)
  , IVectorBase(style, false, true)
  {
    AttachIControl(this, label);
    DisablePrompt(false);
    SetPromptShowsParamLabel(false);

    WDL_String tip;
    if (label && label[0])
      tip.Set(label);
    else
      tip.Set("Selector");

    tip.Append(". Use arrows or wheel. Press Enter to open.");
    SetTooltip(tip.Get());
  }

  void Draw(IGraphics& g) override
  {
    const IParam* pParam = GetParam();
    const bool hasSelection = pParam && (std::abs(GetValue() - pParam->GetDefault(true)) > 1.0e-4);
    const bool hasFocus = mMouseIsOver && !IsDisabled();

    DrawBackground(g, mRECT);
    DrawPressableRectangle(g, mWidgetBounds, false, hasFocus, IsDisabled());

    if (hasSelection)
    {
      const IRECT activeTint = mWidgetBounds.GetPadded(-1.5f);
      g.FillRoundRect(GetColor(kPR).WithOpacity(0.20f), activeTint, 2.2f, &mBlend);

      const IRECT accentRail(activeTint.L + 1.f, activeTint.T + 1.f, activeTint.L + 4.5f, activeTint.B - 1.f);
      g.FillRoundRect(GetColor(kX1).WithOpacity(0.9f), accentRail, 1.1f, &mBlend);
    }

    DrawLabel(g);

    g.DrawRoundRect(GetColor(kFR).WithOpacity(hasSelection ? 0.9f : 0.66f), mWidgetBounds.GetPadded(-0.75f), 2.4f, &mBlend, hasSelection ? 1.35f : 1.f);

    if (hasFocus)
      g.DrawRect(GetColor(kHL).WithOpacity(0.86f), mWidgetBounds.GetPadded(-1.f), &mBlend, 1.4f);

    const IRECT content = mValueBounds.GetPadded(-4.f);
    const IRECT arrowArea = content.GetFromRight(18.f);
    const IRECT valueArea = IRECT(content.L + 18.f, content.T, content.R - 18.f, content.B);
    const IText valueText = hasSelection
      ? mStyle.valueText.WithFGColor(GetColor(kX1).WithOpacity(0.98f))
      : mStyle.valueText;
    g.DrawText(valueText, mValueStr.Get(), valueArea, &mBlend);

    // Simple dropdown arrow indicator.
    const IRECT ar = arrowArea.GetMidVPadded(8.f).GetPadded(-2.f);
    const IColor arrowColor = hasSelection
      ? GetColor(kX1).WithOpacity(0.98f)
      : GetColor(kX1).WithOpacity(0.76f);
    g.FillTriangle(arrowColor, ar.L, ar.T + 2.f, ar.R, ar.T + 2.f, ar.MW(), ar.B - 1.f, &mBlend);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (IsDisabled() || !GetParam())
      return;

    PromptUserInput(mWidgetBounds);
    SetDirty(false);
  }

  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override
  {
    if (IsDisabled() || !GetParam() || std::abs(d) < 0.001f)
      return;

    const int stride = mod.S ? 4 : 1;
    const int delta = (d > 0.f) ? stride : -stride;
    StepBy(delta);
  }

  bool OnKeyDown(float x, float y, const IKeyPress& key) override
  {
    if (IsDisabled() || !GetParam())
      return false;

    const int stride = key.S ? 4 : 1;

    switch (key.VK)
    {
      case kVK_LEFT:
      case kVK_DOWN:
        return StepBy(-stride);

      case kVK_RIGHT:
      case kVK_UP:
        return StepBy(stride);

      case kVK_HOME:
        return SetBoundary(false);

      case kVK_END:
        return SetBoundary(true);

      case kVK_RETURN:
      case kVK_SPACE:
        PromptUserInput(mWidgetBounds);
        SetDirty(false);
        return true;

      default:
        return false;
    }
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override
  {
    IControl::OnMouseOver(x, y, mod);
    SetDirty(false);
  }

  void OnMouseOut() override
  {
    IControl::OnMouseOut();
    SetDirty(false);
  }

  void OnResize() override
  {
    SetTargetRECT(MakeRects(mRECT));
    SetDirty(false);
  }

  void OnInit() override
  {
    if (const IParam* p = GetParam())
    {
      if (!mLabelStr.GetLength())
        mLabelStr.Set(p->GetName());

      p->GetDisplay(mValueStr);
    }
  }

  void SetDirty(bool triggerAction = true, int valIdx = kNoValIdx) override
  {
    IControl::SetDirty(triggerAction, valIdx);

    if (const IParam* p = GetParam())
      p->GetDisplay(mValueStr);
  }

private:
  bool StepBy(int delta)
  {
    const IParam* p = GetParam();
    if (!p || delta == 0)
      return false;

    const int nChoices = p->NDisplayTexts();
    if (nChoices > 1)
    {
      int idx = static_cast<int>(std::lround(p->FromNormalized(GetValue())));
      idx = Clip(idx, 0, nChoices - 1);

      const int next = Clip(idx + delta, 0, nChoices - 1);
      if (next == idx)
        return false;

      SetValueFromUserInput(p->ToNormalized(static_cast<double>(next)));
      return true;
    }

    const double range = std::max(1.0e-9, p->GetRange());
    double step = p->GetStep() / range;

    if (!std::isfinite(step) || step <= 0.0)
      step = 0.02;

    const double next = Clip(GetValue() + (step * static_cast<double>(delta)), 0.0, 1.0);
    if (std::abs(next - GetValue()) < 1.0e-9)
      return false;

    SetValueFromUserInput(next);
    return true;
  }

  bool SetBoundary(bool toMax)
  {
    const IParam* p = GetParam();
    if (!p)
      return false;

    const int nChoices = p->NDisplayTexts();
    if (nChoices > 1)
    {
      const int target = toMax ? (nChoices - 1) : 0;
      const double norm = p->ToNormalized(static_cast<double>(target));

      if (std::abs(norm - GetValue()) < 1.0e-9)
        return false;

      SetValueFromUserInput(norm);
      return true;
    }

    const double norm = toMax ? 1.0 : 0.0;
    if (std::abs(norm - GetValue()) < 1.0e-9)
      return false;

    SetValueFromUserInput(norm);
    return true;
  }
};

class IVThroatTabSwitchControl final : public IVTabSwitchControl
{
public:
  using IVTabSwitchControl::IVTabSwitchControl;

  void DrawWidget(IGraphics& g) override
  {
    const int selectedIdx = GetSelectedIdx();
    ETabSegment segment = ETabSegment::Start;

    for (int i = 0; i < mNumStates; i++)
    {
      const IRECT r = mButtons.Get()[i];

      if (i > 0)
        segment = ETabSegment::Mid;

      if (i == mNumStates - 1)
        segment = ETabSegment::End;

      const bool pressed = (i == selectedIdx);
      DrawButton(g, r, pressed, mMouseOverButton == i, segment);

      if (pressed)
      {
        const float underlineH = std::max(2.5f, r.H() * 0.085f);
        const IRECT underline(r.L + 4.f, r.B - underlineH - 1.f, r.R - 4.f, r.B - 1.f);
        g.FillRoundRect(GetColor(kX1).WithOpacity(IsDisabled() ? 0.34f : 0.95f), underline, underlineH * 0.48f, &mBlend);
      }

      if (mTabLabels.Get(i))
      {
        const IText txt = pressed
          ? mStyle.valueText.WithFGColor(GetColor(kFG).WithOpacity(0.97f))
          : mStyle.valueText.WithFGColor(mStyle.valueText.mFGColor.WithOpacity(0.9f));
        g.DrawText(txt, mTabLabels.Get(i)->Get(), r, &mBlend);
      }
    }
  }
};

class IVSectionPanelControl final : public IControl
{
public:
  IVSectionPanelControl(const IRECT& bounds,
                        const char* title,
                        float headerWidth,
                        const IText& titleText,
                        IColor bgTop,
                        IColor bgBottom,
                        IColor border,
                        IColor innerBorder,
                        IColor header,
                        IColor headerEdge,
                        float borderOpacity = 0.36f,
                        float ruleOpacity = 0.44f,
                        float ruleRightInset = 12.f,
                        float cornerRadius = 3.f)
  : IControl(bounds)
  , mHeaderWidth(headerWidth)
  , mTitleText(titleText)
  , mBgTop(bgTop)
  , mBgBottom(bgBottom)
  , mBorder(border)
  , mInnerBorder(innerBorder)
  , mHeader(header)
  , mHeaderEdge(headerEdge)
  , mBorderOpacity(borderOpacity)
  , mRuleOpacity(ruleOpacity)
  , mRuleRightInset(ruleRightInset)
  , mCornerRadius(cornerRadius)
  {
    mTitle.Set(title);
  }

  void Draw(IGraphics& g) override
  {
    // Gradient fill using path API (FillRoundRect only accepts IColor, so use Path).
    g.PathClear();
    g.PathRoundRect(mRECT, mCornerRadius);
    const IPattern bgGrad = IPattern::CreateLinearGradient(mRECT, EDirection::Vertical, {{mBgTop, 0.f}, {mBgBottom, 1.f}});
    g.PathFill(bgGrad, IFillOptions(), &mBlend);

    g.DrawRoundRect(mBorder.WithOpacity(mBorderOpacity), mRECT, mCornerRadius, &mBlend, 0.9f);

    const float headerW = std::min(mHeaderWidth, mRECT.W() - 28.f);
    const IRECT titleRect(mRECT.L + 12.f, mRECT.T + 6.f, mRECT.L + 12.f + headerW, mRECT.T + 24.f);
    const float ruleY = mRECT.T + 17.f;
    const float ruleL = titleRect.R + 10.f;

    if (ruleL < mRECT.R - mRuleRightInset)
      g.DrawLine(mHeaderEdge.WithOpacity(mRuleOpacity), ruleL, ruleY, mRECT.R - mRuleRightInset, ruleY, &mBlend, 0.9f);

    g.DrawText(mTitleText, mTitle.Get(), titleRect, &mBlend);
  }

private:
  WDL_String mTitle;
  float mHeaderWidth;
  IText mTitleText;
  IColor mBgTop;
  IColor mBgBottom;
  IColor mBorder;
  IColor mInnerBorder;
  IColor mHeader;
  IColor mHeaderEdge;
  float mBorderOpacity;
  float mRuleOpacity;
  float mRuleRightInset;
  float mCornerRadius;
};

class IVThroatKnobControl final : public IVKnobControl
{
public:
  using IVKnobControl::IVKnobControl;

  void DrawWidget(IGraphics& g) override
  {
    const float cx = mWidgetBounds.MW();
    const float cy = mWidgetBounds.MH();
    const float outerR = std::min(mWidgetBounds.W(), mWidgetBounds.H()) * 0.5f - 1.5f;
    const float arcR = outerR * 0.88f;
    const float faceR = outerR * 0.72f;
    const float capR = outerR * 0.08f;
    const float angle = mAngle1 + (static_cast<float>(GetValue()) * (mAngle2 - mAngle1));
    const IColor outerFill = GetColor(kFG).WithOpacity(IsDisabled() ? 0.42f : 1.f);
    const IColor innerFill = GetColor(kX2).WithOpacity(IsDisabled() ? 0.38f : 0.94f);
    const IColor active = GetColor(kX1).WithOpacity(IsDisabled() ? 0.36f : 0.95f);
    const IColor inactive = GetColor(kX3).WithOpacity(IsDisabled() ? 0.22f : 0.56f);
    const IColor pointerShadow(74, 0, 0, 0);

    if (!mMouseDown && !IsDisabled())
      g.FillCircle(GetColor(kSH).WithOpacity(0.16f), cx, cy + 2.2f, outerR, &mBlend);

    g.FillCircle(outerFill, cx, cy, outerR, &mBlend);
    g.FillCircle(innerFill, cx, cy, faceR, &mBlend);

    g.DrawCircle(GetColor(kFR).WithOpacity(0.5f), cx, cy, outerR, &mBlend, 1.f);
    g.DrawCircle(GetColor(kFR).WithOpacity(0.18f), cx, cy, faceR, &mBlend, 0.8f);
    g.DrawArc(inactive, cx, cy, arcR, mAngle1, mAngle2, &mBlend, 2.f);
    g.DrawArc(active, cx, cy, arcR, mAngle1, angle, &mBlend, 2.5f);

    g.DrawRadialLine(pointerShadow, cx, cy + 1.f, angle, faceR * 0.1f, faceR * 0.84f, &mBlend, 3.f);
    g.DrawRadialLine(active, cx, cy, angle, faceR * 0.14f, faceR * 0.8f, &mBlend, 1.9f);
    g.FillCircle(active, cx, cy, capR, &mBlend);

    if (mMouseIsOver && !IsDisabled())
      g.DrawCircle(GetColor(kHL).WithOpacity(0.48f), cx, cy, outerR + 1.4f, &mBlend, 1.1f);
  }
};

class IVThroatToggleControl final : public IControl, public IVectorBase
{
public:
  IVThroatToggleControl(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style = DEFAULT_STYLE)
  : IControl(bounds, paramIdx)
  , IVectorBase(style, false, false)
  {
    AttachIControl(this, label);
  }

  void Draw(IGraphics& g) override
  {
    const bool isOn = GetValue() > 0.5f;
    const bool hasFocus = mMouseIsOver && !IsDisabled();
    
    DrawBackground(g, mRECT);
    DrawPressableRectangle(g, mWidgetBounds, false, hasFocus, IsDisabled());
    
    // Draw filled background when ON
    if (isOn)
    {
      const IRECT activeTint = mWidgetBounds.GetPadded(-1.5f);
      g.FillRoundRect(GetColor(kX1).WithOpacity(0.25f), activeTint, 2.2f, &mBlend);
    }
    
    DrawLabel(g);
    
    // Draw border
    g.DrawRoundRect(GetColor(kFR).WithOpacity(isOn ? 0.9f : 0.66f), mWidgetBounds.GetPadded(-0.75f), 2.4f, &mBlend, isOn ? 1.35f : 1.f);
    
    if (hasFocus)
      g.DrawRect(GetColor(kHL).WithOpacity(0.86f), mWidgetBounds.GetPadded(-1.f), &mBlend, 1.4f);
    
    // Draw ON/OFF text
    const char* text = isOn ? "ON" : "OFF";
    const IRECT content = mValueBounds.GetPadded(-4.f);
    const IText valueText = isOn
      ? mStyle.valueText.WithFGColor(GetColor(kX1).WithOpacity(0.98f))
      : mStyle.valueText.WithFGColor(GetColor(kX3).WithOpacity(0.76f));
    g.DrawText(valueText, text, content, &mBlend);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (IsDisabled() || !GetParam())
      return;
    
    // Toggle value
    const bool newState = GetValue() < 0.5f;
    SetValueFromUserInput(newState ? 1.0 : 0.0);
    SetDirty(false);
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override
  {
    IControl::OnMouseOver(x, y, mod);
    SetDirty(false);
  }

  void OnMouseOut() override
  {
    IControl::OnMouseOut();
    SetDirty(false);
  }

  void OnResize() override
  {
    SetTargetRECT(MakeRects(mRECT));
    SetDirty(false);
  }

  void OnInit() override
  {
    if (const IParam* p = GetParam())
    {
      if (!mLabelStr.GetLength())
        mLabelStr.Set(p->GetName());
    }
  }

  void SetDirty(bool triggerAction = true, int valIdx = kNoValIdx) override
  {
    IControl::SetDirty(triggerAction, valIdx);
  }
};
}

SoreThroat::SoreThroat(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  // === MACRO UI PARAMETERS ===
  // Primary 12 knobs
  GetParam(kMix)->InitPercentage("Mix", 100.0);
  GetParam(kVoices)->InitInt("Voices", 50, 1, kMaxVoices);
  GetParam(kMinDelay)->InitMilliseconds("Min Delay", 2.0, 0.0, 250.0);
  GetParam(kDelaySpread)->InitDouble("Delay Spread", 20.0, 0.0, 1000.0, 0.1, "ms");
  GetParam(kFeedback)->InitDouble("Feedback", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kScatter)->InitDouble("Scatter", 0.0, 0.0, 10.0, 0.1);
  GetParam(kStereo)->InitDouble("Stereo", 0.0, -100.0, 100.0, 0.1, "%");  // -100%=rotation, 0%=normal, 100%=width
  GetParam(kSpeed)->InitDouble("Speed", 500.0, 1.0, 4000.0, 0.1, "ms");
  GetParam(kRandom)->InitDouble("Random", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kTone)->InitDouble("Tone", 0.0, -100.0, 100.0, 0.1, "%");  // Tilt EQ: -100%=dark, 0%=flat, 100%=bright
  GetParam(kMod)->InitDouble("Mod", 0.0, -100.0, 100.0, 0.1, "%");  // Combined LFO + Env modulation
  
  // Selector (LFO Shape)
  GetParam(kLFOShape)->InitEnum("LFO Shape", kLFOSine, 
    {"Sine", "Triangle", "Square", "Ramp Up", "Ramp Down", 
     "S&H", "Smooth S&H", "Sawtooth", "Reverse Saw", "Bitcrush"});
  
  // Global SYNC toggle
  GetParam(kSync)->InitBool("SYNC", false);

  // === INTERNAL PROCESSING PARAMETERS ===
  // Legacy/compat
  GetParam(kPostGain)->InitGain("Post Gain", 0.0);
  GetParam(kDelayRange)->InitDouble("Delay Range", 20.0, 0.0, 1000.0, 0.1, "ms");  // Maps to kDelaySpread
  GetParam(kLegacyDamper)->InitDouble("Damper", 0.0, 0.0, 10.0, 0.1);  // Maps to kScatter
  GetParam(kLowpass)->InitFrequency("Lowpass", 12000.0, 20.0, 20000.0);
  GetParam(kRateRange)->InitDouble("Rate Range", 80.0, 0.0, 1200.0, 0.1, "ms/s");
  GetParam(kRateUpdate)->InitDouble("Rate Update", 500.0, 1.0, 4000.0, 0.1, "ms");

  // Stereo
  GetParam(kStereoMode)->InitEnum("Stereo Mode", kStereoFree, {"Independent", "Linked", "Mirror", "Half-Width"});
  GetParam(kRotation)->InitDouble("Rotation", 0.0, -180.0, 180.0, 0.1, "deg");
  GetParam(kWidth)->InitDouble("Width", 100.0, 0.0, 200.0, 0.1, "%");

  // Rate/timing
  GetParam(kMinRate)->InitDouble("Min Rate", 10.0, 0.0, 1200.0, 0.1, "ms/s");
  GetParam(kUpdateUnit)->InitEnum("Update Unit", kTimeMilliseconds, {"Milliseconds", "Seconds", "Host Sync", "Manual Sync"});
  GetParam(kUpdateQuantize)->InitBool("Update Quantize", false);
  GetParam(kUpdateOnCollision)->InitBool("Update On Collision", false);
  GetParam(kManualBPM)->InitDouble("Manual BPM", 120.0, 20.0, 300.0, 0.1, "BPM");

  // Filter
  GetParam(kHighpass)->InitFrequency("High-Pass", 20.0, 10.0, 10000.0);
  GetParam(kLPRes)->InitDouble("LP Resonance", 0.707, 0.2, 12.0, 0.01);
  GetParam(kHPRes)->InitDouble("HP Resonance", 0.707, 0.2, 12.0, 0.01);

  // Modulation
  GetParam(kEnvLP)->InitDouble("Env to LP", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kEnvHP)->InitDouble("Env to HP", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kEnvDelay)->InitDouble("Env to Delay", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kLFORate)->InitDouble("LFO Rate", 500.0, 1.0, 4000.0, 0.1, "ms");
  GetParam(kLFOLP)->InitDouble("LFO to LP", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kLFOHP)->InitDouble("LFO to HP", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kLFODelay)->InitDouble("LFO to Delay", 0.0, -100.0, 100.0, 0.1, "%");

  // Dynamics
  GetParam(kPregain)->InitGain("Pre Gain", 0.0);
  GetParam(kFollowLevel)->InitEnum("Follow Level", kFollowOff, {"Off", "Normal", "Fast Decay", "Slow Decay"});

#if IPLUG_DSP
  ResizeDelayBuffer();
  ResetDSPState();
#endif

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);
    pGraphics->EnableTooltips(true);
    bool hasFallbackFont = LoadBundledFont(pGraphics, "Roboto-Regular", ROBOTO_FN);
    bool hasSecondaryFont = LoadBundledFont(pGraphics, "Corazon", CORAZON_FN);
    bool hasTitleFont = LoadBundledFont(pGraphics, "Witchwoode", WITCHWOODE_FN);
    const ISVG throatBgSVG = pGraphics->LoadSVG(THROAT_BG_FN);

    if (!hasFallbackFont)
      hasFallbackFont = pGraphics->LoadFont("Roboto-Regular", "Segoe UI", ETextStyle::Normal);

    // Ensure requested font IDs always resolve to loaded data on all targets.
    if (!hasSecondaryFont && hasFallbackFont)
      hasSecondaryFont = LoadBundledFont(pGraphics, "Corazon", ROBOTO_FN);

    if (!hasSecondaryFont)
      hasSecondaryFont = pGraphics->LoadFont("Corazon", "Georgia", ETextStyle::Normal);

    if (!hasTitleFont && hasSecondaryFont)
      hasTitleFont = LoadBundledFont(pGraphics, "Witchwoode", CORAZON_FN);

    if (!hasTitleFont && hasFallbackFont)
      hasTitleFont = LoadBundledFont(pGraphics, "Witchwoode", ROBOTO_FN);

    if (!hasTitleFont)
      hasTitleFont = pGraphics->LoadFont("Witchwoode", "Times New Roman", ETextStyle::Bold);

    const char* bodyFontID = hasSecondaryFont ? "Corazon" : "Roboto-Regular";
    const char* titleFontID = hasTitleFont ? "Witchwoode" : bodyFontID;
    const char* microFontID = bodyFontID;

    // Typography scale
    constexpr float kTypeHint = 14.75f;
    constexpr float kTypeSubSection = 15.5f;
    constexpr float kTypeControlLabel = 13.0f;
    constexpr float kTypeKnobLabel = 12.5f;
    constexpr float kTypeSection = 16.75f;
    constexpr float kTypeControlValue = 14.0f;
    constexpr float kTypeHeroTitle = 28.0f;

    // CLOPH palette: dark purples, muted mauve, amber accent
    const IColor cBgTop(255, 26, 19, 24);
    const IColor cBgBot(255, 17, 12, 16);
    const IColor cPanelTop(255, 42, 30, 39);
    const IColor cPanelBot(255, 34, 25, 32);
    const IColor cPanelBorder(255, 116, 86, 104);
    const IColor cPanelInset(255, 64, 46, 58);
    const IColor cHeader(255, 36, 25, 33);
    const IColor cHeaderEdge(255, 188, 150, 168);
    const IColor cText(255, 242, 228, 236);
    const IColor cSubText(255, 188, 152, 168);
    const IColor cTitleText(255, 248, 238, 244);
    const IColor cAccent(255, 224, 168, 110);
    const IColor cWidget(255, 46, 33, 42);
    const IColor cFrame(255, 128, 96, 114);
    const IColor cHover(72, 188, 150, 168);
    const IColor cShadow(46, 0, 0, 0);
    const IColor cDisplayBack(255, 15, 10, 14);
    const IColor cDisplayFrame(255, 100, 72, 90);
    const IColor cDisplayAccent(255, 224, 170, 116);
    const IColor cDisplayPartner(255, 176, 144, 162);
    const IColor cSelectorFrame(255, 126, 94, 114);
    const IColor cIdleTrack(255, 134, 100, 120);

    const IVStyle baseStyle{
      true,
      true,
      {
        COLOR_TRANSPARENT,
        cWidget,
        IColor(255, 56, 40, 52),
        cFrame,
        cHover,
        cShadow,
        cAccent,
        cPanelInset,
        cIdleTrack
      },
      IText(kTypeControlLabel, cSubText, microFontID, EAlign::Near, EVAlign::Middle),
      IText(kTypeControlValue, cText, bodyFontID, EAlign::Center, EVAlign::Middle),
      true,
      true,
      true,
      false,
      0.24f,
      1.1f,
      1.45f,
      0.94f,
      0.f
    };

    // Knob style for macro controls - primary (main grid)
    const IVStyle knobStyle = baseStyle
      .WithShowValue(false)
      .WithWidgetFrac(0.88f)
      .WithLabelText(IText(kTypeKnobLabel, cText, bodyFontID, EAlign::Center, EVAlign::Middle));

    // Hero style - larger knobs for header controls (Mix, Output)
    const IVStyle heroKnobStyle = knobStyle.WithWidgetFrac(0.92f);

    // Secondary style - smaller knobs for less important controls
    const IVStyle secondaryKnobStyle = knobStyle.WithWidgetFrac(0.78f);

    // Selector style
    const IVStyle selectorStyle = baseStyle
      .WithWidgetFrac(0.92f)
      .WithRoundness(0.2f)
      .WithColor(kFG, IColor(255, 37, 26, 33))
      .WithColor(kPR, IColor(255, 128, 92, 74))
      .WithColor(kFR, cSelectorFrame)
      .WithColor(kHL, IColor(70, 188, 150, 168))
      .WithColor(kX1, cAccent)
      .WithLabelText(IText(kTypeControlLabel, cSubText, microFontID, EAlign::Near, EVAlign::Middle))
      .WithValueText(IText(kTypeControlValue, cText, bodyFontID, EAlign::Center, EVAlign::Middle));

    // Toggle style for SYNC
    const IVStyle toggleStyle = baseStyle
      .WithWidgetFrac(0.92f)
      .WithRoundness(0.2f)
      .WithColor(kFG, IColor(255, 37, 26, 33))
      .WithColor(kPR, IColor(255, 128, 92, 74))
      .WithColor(kFR, cSelectorFrame)
      .WithColor(kHL, IColor(70, 188, 150, 168))
      .WithColor(kX1, cAccent)
      .WithLabelText(IText(kTypeControlLabel, cSubText, microFontID, EAlign::Near, EVAlign::Middle))
      .WithValueText(IText(kTypeControlValue, cText, bodyFontID, EAlign::Center, EVAlign::Middle));

    const IVStyle displayStyle = baseStyle
      .WithShowLabel(false)
      .WithShowValue(false)
      .WithWidgetFrac(1.f)
      .WithDrawFrame(false)
      .WithColor(kBG, cDisplayBack)
      .WithColor(kFG, cDisplayBack)
      .WithColor(kPR, cDisplayBack)
      .WithColor(kX1, cDisplayAccent)
      .WithColor(kX2, cDisplayPartner)
      .WithColor(kFR, cDisplayFrame);

    const IText sectionText(kTypeSection, cTitleText, bodyFontID, EAlign::Near, EVAlign::Middle);
    const IText heroTitle(kTypeHeroTitle, cTitleText, titleFontID, EAlign::Near, EVAlign::Middle);

    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(cBgBot);

    const IRECT full = pGraphics->GetBounds();
    const IPattern bgGrad = IPattern::CreateLinearGradient(full, EDirection::Vertical,
      {{cBgTop, 0.f}, {cBgBot, 1.f}});
    pGraphics->AttachControl(new IPanelControl(full, bgGrad));

    // Layout constants
    const float margin = 12.f;
    const float gap = 8.f;
    const IRECT root = full.GetPadded(-margin);
    const float headerH = 70.f;
    const float footerH = 100.f;  // Space for visualizer

    const IRECT headerRect(root.L, root.T, root.R, root.T + headerH);
    const IRECT contentRect(root.L, headerRect.B + gap, root.R, root.B);

    auto attach = [&](IControl* control, int tag = kNoTag, const char* group = nullptr) {
      if (group)
        pGraphics->AttachControl(control, tag, group);
      else
        pGraphics->AttachControl(control, tag);
    };

    auto addSection = [&](const IRECT& r, const char* title, float headerWidth) -> IRECT {
      attach(new IVSectionPanelControl(r, title, headerWidth, sectionText,
        cPanelTop, cPanelBot, cPanelBorder, cPanelInset, cHeader, cHeaderEdge));
      return r.GetPadded(-8.f).GetReducedFromTop(24.f);
    };

    // Helper lambdas for creating controls
    auto addKnob = [&](const IRECT& cell, int paramIdx, const char* label, const IVStyle& style) {
      auto* control = new IVThroatKnobControl(cell.GetPadded(-1.f), paramIdx, label, style);
      attach(control);
    };

    auto addHeroKnob = [&](const IRECT& cell, int paramIdx, const char* label) {
      addKnob(cell, paramIdx, label, heroKnobStyle);
    };

    auto addSecondaryKnob = [&](const IRECT& cell, int paramIdx, const char* label) {
      addKnob(cell, paramIdx, label, secondaryKnobStyle);
    };

    auto addDefaultKnob = [&](const IRECT& cell, int paramIdx, const char* label) {
      addKnob(cell, paramIdx, label, knobStyle);
    };

    auto addSelector = [&](const IRECT& cell, int paramIdx, const char* label) {
      auto* control = new IVPopupSelectorControl(cell.GetPadded(-1.f), paramIdx, label, selectorStyle);
      attach(control);
    };

    auto addText = [&](const IRECT& r, const char* text, const IText& style) {
      attach(new ITextControl(r, text, style));
    };

    // === HEADER ===
    attach(new IPanelControl(headerRect, cHeader));
    attach(new IPanelControl(IRECT(headerRect.L, headerRect.B - 1.f, headerRect.R, headerRect.B),
      cHeaderEdge.WithOpacity(0.55f)));

    // Title on left
    addText(IRECT(headerRect.L + 16.f, headerRect.T, headerRect.L + 400.f, headerRect.B), "SORE THROAT", heroTitle);

    // Decorative manifesto text on right
    const IText versionText(kTypeControlLabel, cSubText.WithOpacity(0.4f), microFontID, EAlign::Far, EVAlign::Middle);
    addText(IRECT(headerRect.R - 300.f, headerRect.T, headerRect.R - 16.f, headerRect.B), "CLOPH_DSP // EXP-02 VOCAL PROCESSOR", versionText);

    // === MAIN CONTENT ===
    // Asymmetric 3-column layout per CLOPH architecture rules
    const float contentGap = 24.f; // Generous rhythmic separation
    const float leftW = contentRect.W() * 0.40f;
    const float rightW = contentRect.W() * 0.28f;
    const float middleW = contentRect.W() - leftW - rightW - (contentGap * 2.f);

    const IRECT leftPanel(contentRect.L, contentRect.T, contentRect.L + leftW, contentRect.B);
    const IRECT midPanel(leftPanel.R + contentGap, contentRect.T, leftPanel.R + contentGap + middleW, contentRect.B);
    const IRECT rightPanel(midPanel.R + contentGap, contentRect.T, contentRect.R, contentRect.B);

    // === LEFT PANEL: TELEMETRY & SWARM ===
    // Large context-driven visualizer up top, engine controls below
    const float visualizerH = leftPanel.H() * 0.60f;
    const IRECT visArea(leftPanel.L, leftPanel.T, leftPanel.R, leftPanel.T + visualizerH);
    const IRECT swarmArea(leftPanel.L, visArea.B + contentGap, leftPanel.R, leftPanel.B);

    const IRECT visSection = addSection(visArea, "TELEMETRY", 110.f);
    attach(new IVDotsDisplayControl(visSection.GetPadded(-4.f), "", displayStyle, throatBgSVG), kCtrlTagDisplay);

    const IRECT swarmSection = addSection(swarmArea, "SWARM ENGINE", 140.f);
    // 3 macro parameters as dictated by cognitive economy manifesto
    addDefaultKnob(swarmSection.SubRectHorizontal(3, 0), kVoices, "VOICES");
    addSecondaryKnob(swarmSection.SubRectHorizontal(3, 1), kScatter, "SCATTER");
    addSecondaryKnob(swarmSection.SubRectHorizontal(3, 2), kStereo, "STEREO");

    // === MIDDLE PANEL: TEMPORAL ===
    const IRECT tempSection = addSection(midPanel, "TEMPORAL DECAY", 160.f);
    
    // Rhythmically diverse spacing inside the container
    const float tempRow1H = tempSection.H() * 0.45f;
    const IRECT tempRow1(tempSection.L, tempSection.T + 12.f, tempSection.R, tempSection.T + tempRow1H);
    addDefaultKnob(tempRow1.SubRectHorizontal(2, 0), kMinDelay, "MIN DELAY");
    addDefaultKnob(tempRow1.SubRectHorizontal(2, 1), kDelaySpread, "SPREAD");

    const IRECT tempRow2(tempSection.L, tempRow1.B + 8.f, tempSection.R, tempSection.B);
    addHeroKnob(tempRow2.SubRectHorizontal(2, 0), kSpeed, "SPEED");
    
    const IRECT syncCell = tempRow2.SubRectHorizontal(2, 1);
    auto* syncToggle = new IVThroatToggleControl(syncCell.GetCentredInside(syncCell.W() * 0.65f, 30.f), kSync, "SYNC", toggleStyle);
    attach(syncToggle, kCtrlTagSyncToggle);

    // === RIGHT PANEL: MUTATION & MASTER ===
    const float mutationH = rightPanel.H() * 0.52f;
    const IRECT mutArea(rightPanel.L, rightPanel.T, rightPanel.R, rightPanel.T + mutationH);
    const IRECT outArea(rightPanel.L, mutArea.B + contentGap, rightPanel.R, rightPanel.B);

    const IRECT mutSection = addSection(mutArea, "MUTATION", 100.f);
    const float mutKW = mutSection.W() / 2.f;
    const float mutKH = mutSection.H() / 2.f;
    
    addDefaultKnob(IRECT(mutSection.L, mutSection.T, mutSection.L + mutKW, mutSection.T + mutKH), kFeedback, "FEEDBACK");
    addSecondaryKnob(IRECT(mutSection.L + mutKW, mutSection.T, mutSection.R, mutSection.T + mutKH), kRandom, "RANDOM");
    
    addSecondaryKnob(IRECT(mutSection.L, mutSection.T + mutKH, mutSection.L + mutKW, mutSection.B), kMod, "MOD");
    
    const IRECT lfoCell(mutSection.L + mutKW, mutSection.T + mutKH, mutSection.R, mutSection.B);
    addSelector(lfoCell.GetCentredInside(lfoCell.W() * 0.90f, 26.f), kLFOShape, "LFO");

    const IRECT outSection = addSection(outArea, "OUTPUT STAGE", 140.f);
    const float toneH = outSection.H() * 0.40f;
    addSecondaryKnob(IRECT(outSection.L, outSection.T, outSection.R, outSection.T + toneH), kTone, "TONE");
    
    const IRECT mainOut(outSection.L, outSection.T + toneH, outSection.R, outSection.B);
    addHeroKnob(mainOut.SubRectHorizontal(2, 0), kMix, "MIX");
    addHeroKnob(mainOut.SubRectHorizontal(2, 1), kPostGain, "OUTPUT");
  };
#endif
}

#if IPLUG_EDITOR
void SoreThroat::OnParamChangeUI(int paramIdx, EParamSource source)
{
  (void) source;

  if (paramIdx != kUpdateUnit && paramIdx != kManualBPM)
    return;

  IGraphics* pGraphics = GetUI();
  if (!pGraphics)
    return;

  const int unit = Clip(static_cast<int>(GetParam(kUpdateUnit)->Value()), 0, kNumTimeUnits - 1);
  const bool useManualBPM = (unit == kTimeNoteSyncManualBPM);

  if (auto* hintControl = static_cast<ITextControl*>(pGraphics->GetControlWithTag(kCtrlTagTempoModeHint)))
  {
    hintControl->SetStr(TempoSourceHint(unit));
    hintControl->SetDirty(false);
  }

  if (auto* manualBPMControl = pGraphics->GetControlWithTag(kCtrlTagManualBPMKnob))
  {
    manualBPMControl->SetDisabled(!useManualBPM);
    manualBPMControl->SetTooltip(useManualBPM
      ? "Manual BPM. Sets tempo used when Time Base is Manual Sync."
      : "Manual BPM is disabled. Set Time Base to Manual Sync to enable it.");
    manualBPMControl->SetDirty(false);
  }
}
#endif

#if IPLUG_DSP
uint32_t SoreThroat::NextPow2(uint32_t v)
{
  if (v < 2)
    return 2;

  v--;
  v |= v >> 1U;
  v |= v >> 2U;
  v |= v >> 4U;
  v |= v >> 8U;
  v |= v >> 16U;
  return ++v;
}

void SoreThroat::ResizeDelayBuffer()
{
  const double sr = std::max(1.0, GetSampleRate());
  const uint32_t target = static_cast<uint32_t>(std::ceil(sr * (kMaxInternalDelayMs * 0.001))) + 8U;
  mBufferSize = NextPow2(target);
  mBufferMask = mBufferSize - 1U;

  if (mBuffer.GetSize() != static_cast<int>(mBufferSize * kMaxChannels))
    mBuffer.Resize(static_cast<int>(mBufferSize * kMaxChannels));

  mBuffer.Set(nullptr, static_cast<int>(mBufferSize * kMaxChannels));
  mWriteAddress = 0;
}

void SoreThroat::ResetDSPState()
{
  mDots.fill(0);
  mVoiceStates.fill({});
  mLPState.fill({});
  mHPState.fill({});
  mFeedbackTap = {0.f, 0.f};

  mSmMix = Clip(GetParam(kMix)->Value() / 100.0, 0.0, 1.0);
  mSmPregain = GetParam(kPregain)->DBToAmp();
  mSmPostgain = GetParam(kPostGain)->DBToAmp();
  mSmFeedback = Clip(GetParam(kFeedback)->Value() / 100.0, 0.0, 0.99);
  mSmRotationRad = StereoMacroStateFromValue(static_cast<float>(GetParam(kStereo)->Value())).rotationRad;
  mSmLowpassHz = GetParam(kLowpass)->Value();
  mSmHighpassHz = GetParam(kHighpass)->Value();
  mSmLPQ = GetParam(kLPRes)->Value();
  mSmHPQ = GetParam(kHPRes)->Value();
  mSmEnvLP = GetParam(kEnvLP)->Value() / 100.0;
  mSmEnvHP = GetParam(kEnvHP)->Value() / 100.0;
  mSmEnvDelay = GetParam(kEnvDelay)->Value() / 100.0;
  mSmLFOLP = GetParam(kLFOLP)->Value() / 100.0;
  mSmLFOHP = GetParam(kLFOHP)->Value() / 100.0;
  mSmLFODelay = GetParam(kLFODelay)->Value() / 100.0;

  mInputEnv = 0.0;
  mOutputEnv = 0.0;
  mFollowGain = 1.0;
  mUpdateSamplesRemaining = 0.0;
  mLFOPhase = 0.0;
  mUseFastDelayInterp = false;
  mLastVoiceCount = 0;
  mLastStereoMode = -1;
}

float SoreThroat::Random01()
{
  // xorshift32: deterministic and fast on the audio thread.
  mRngState ^= mRngState << 13;
  mRngState ^= mRngState >> 17;
  mRngState ^= mRngState << 5;
  return static_cast<float>(mRngState & 0x00FFFFFFu) / 16777215.0f;
}

float SoreThroat::RandomRateSamplesPerSample(float minRateMsPerSec, float rateRangeMsPerSec)
{
  const float magnitude = std::max(0.f, minRateMsPerSec + (Random01() * std::max(0.f, rateRangeMsPerSec)));
  const float step = std::min(kMaxRateSamplesPerSample, magnitude * 0.001f);
  const float sign = (Random01() > 0.5f) ? 1.f : -1.f;
  return sign * step;
}

void SoreThroat::RandomizeVoice(int voiceIdx, int stereoMode, int nChans, float delayRangeSamples, float minRateMsPerSec, float rateRangeMsPerSec, float legacyDamper)
{
  auto& v = mVoiceStates[voiceIdx];
  const float range = std::max(0.f, delayRangeSamples);
  const float damp = std::max(0.f, legacyDamper);

  v.delayL = Random01() * range;
  v.delayR = Random01() * range;
  v.rateL = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
  v.rateR = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);

  if (range <= 0.f)
  {
    v.delayL = v.delayR = 0.f;
    v.rateL = v.rateR = 0.f;
  }

  if (nChans > 1)
  {
    if (stereoMode == kStereoSlave)
    {
      v.delayR = v.delayL;
      v.rateR = v.rateL;
    }
    else if (stereoMode == kStereoAntiSlave)
    {
      v.delayR = range - v.delayL;
      v.rateR = -v.rateL;
    }
    else if (stereoMode == kStereoHalf)
    {
      if ((voiceIdx & 1) == 0)
      {
        v.delayR = 0.5f * v.delayL;
        v.rateR = 0.5f * v.rateL;
      }
      else
      {
        v.delayL = 0.5f * v.delayR;
        v.rateL = 0.5f * v.rateR;
      }
    }
  }

  const float dNormL = (range > 0.f) ? (v.delayL / range) : 0.f;
  const float dNormR = (range > 0.f) ? (v.delayR / range) : 0.f;

  const float dampL = std::pow(0.5f, dNormL * damp * 0.5f);
  const float dampR = std::pow(0.5f, dNormR * damp * 0.5f);

  v.gainL = (0.65f + (0.35f * Random01())) * dampL;
  v.gainR = (0.65f + (0.35f * Random01())) * dampR;
}

void SoreThroat::RandomizeRates(int voiceCount, int stereoMode, int nChans, float minRateMsPerSec, float rateRangeMsPerSec)
{
  for (int i = 0; i < voiceCount; ++i)
  {
    auto& v = mVoiceStates[i];

    if (nChans < 2 || stereoMode == kStereoFree)
    {
      v.rateL = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
      if (nChans > 1)
        v.rateR = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
    }
    else if (stereoMode == kStereoSlave)
    {
      v.rateL = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
      v.rateR = v.rateL;
    }
    else if (stereoMode == kStereoAntiSlave)
    {
      v.rateL = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
      v.rateR = -v.rateL;
    }
    else if (stereoMode == kStereoHalf)
    {
      if ((i & 1) == 0)
      {
        v.rateL = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
        v.rateR = v.rateL * 0.5f;
      }
      else
      {
        v.rateR = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
        v.rateL = v.rateR * 0.5f;
      }
    }
  }
}

void SoreThroat::EnsureVoiceState(int voiceCount, int stereoMode, int nChans, float delayRangeSamples, float minRateMsPerSec, float rateRangeMsPerSec, float legacyDamper)
{
  if (voiceCount != mLastVoiceCount || stereoMode != mLastStereoMode)
  {
    for (int i = 0; i < voiceCount; ++i)
      RandomizeVoice(i, stereoMode, nChans, delayRangeSamples, minRateMsPerSec, rateRangeMsPerSec, legacyDamper);

    mLastVoiceCount = voiceCount;
    mLastStereoMode = stereoMode;
  }
}

double SoreThroat::ResolveTempoBPM(int unit, double manualBPM) const
{
  const double manual = Clip(manualBPM, 20.0, 300.0);

  if (unit == kTimeNoteSyncManualBPM)
    return manual;

  if (unit == kTimeNoteSyncHost)
  {
    double hostTempo = GetTempo();
    if (!std::isfinite(hostTempo) || hostTempo <= 0.0)
      hostTempo = manual;

    if (!std::isfinite(hostTempo) || hostTempo <= 0.0)
      hostTempo = 120.0;

    return Clip(hostTempo, 20.0, 400.0);
  }

  return 120.0;
}

double SoreThroat::NoteDivisionQN(double normalized, bool quantize) const
{
  static constexpr std::array<double, 21> kNoteDivisionsQN = {
    1.0 / 24.0, 1.0 / 16.0, 3.0 / 32.0,
    1.0 / 12.0, 1.0 / 8.0,  3.0 / 16.0,
    1.0 / 6.0,  1.0 / 4.0,  3.0 / 8.0,
    1.0 / 3.0,  1.0 / 2.0,  3.0 / 4.0,
    2.0 / 3.0,  1.0,        1.5,
    4.0 / 3.0,  2.0,        3.0,
    4.0,        8.0,        16.0
  };

  const double scaled = Clip(normalized, 0.0, 1.0) * static_cast<double>(kNoteDivisionsQN.size() - 1);

  if (quantize)
  {
    const int idx = Clip(static_cast<int>(std::lround(scaled)), 0, static_cast<int>(kNoteDivisionsQN.size() - 1));
    return kNoteDivisionsQN[static_cast<size_t>(idx)];
  }

  const int i0 = Clip(static_cast<int>(std::floor(scaled)), 0, static_cast<int>(kNoteDivisionsQN.size() - 1));
  const int i1 = Clip(i0 + 1, 0, static_cast<int>(kNoteDivisionsQN.size() - 1));
  const double frac = scaled - static_cast<double>(i0);
  return kNoteDivisionsQN[static_cast<size_t>(i0)] + ((kNoteDivisionsQN[static_cast<size_t>(i1)] - kNoteDivisionsQN[static_cast<size_t>(i0)]) * frac);
}

double SoreThroat::TimeFromUnitMs(double normalized, int unit, bool quantize, double bpm) const
{
  const double n = Clip(normalized, 0.0, 1.0);

  if (unit == kTimeMilliseconds)
  {
    double ms = 1.0 + (n * 3999.0);
    if (quantize)
      ms = std::round(ms);
    return std::max(1.0, ms);
  }

  if (unit == kTimeSeconds)
  {
    double sec = 0.01 + (n * 7.99);
    if (quantize)
      sec = std::round(sec);
    return std::max(1.0, sec * 1000.0);
  }

  const double qn = NoteDivisionQN(n, quantize);
  const double safeBPM = Clip(bpm, 20.0, 400.0);
  return std::max(1.0, (60000.0 / safeBPM) * qn);
}

double SoreThroat::LFOValue(int shape) const
{
  const double p = mLFOPhase;
  const double tri = (2.0 * std::abs((2.0 * p) - 1.0)) - 1.0;
  const double sqr = (p < 0.5) ? 1.0 : -1.0;
  const double up = (2.0 * p) - 1.0;
  const double down = 1.0 - (2.0 * p);
  const double sine = std::sin(2.0 * PI * p);

  if (shape == kLFOTriangle) return tri;
  if (shape == kLFOSquare) return sqr;
  if (shape == kLFORampUp) return up;
  if (shape == kLFORampDown) return down;
  if (shape == kLFOSine) return sine;

    return sine;
}

float SoreThroat::UpdateVoiceDelay(float& delay, float& rate, float delayRangeSamples, bool updateOnCollision, float minRateMsPerSec, float rateRangeMsPerSec)
{
  const float range = std::max(0.f, delayRangeSamples);

  if (range <= 0.f)
  {
    delay = 0.f;
    rate = 0.f;
    return 0.f;
  }

  delay += rate;
  bool collision = false;

  if (delay < 0.f)
  {
    delay = -delay;
    collision = true;
  }
  else if (delay > range)
  {
    delay = (2.f * range) - delay;
    collision = true;
  }

  if (collision)
  {
    if (updateOnCollision)
      rate = RandomRateSamplesPerSample(minRateMsPerSec, rateRangeMsPerSec);
    else
      rate = -rate;
  }

  delay = Clip(delay, 0.f, range);

  if (!std::isfinite(delay)) delay = 0.f;
  if (!std::isfinite(rate)) rate = 0.f;

  return delay;
}

float SoreThroat::ReadDelayHermite(int channel, float delaySamples) const
{
  const auto* buffer = mBuffer.Get();
  if (!buffer || mBufferSize < 4U)
    return 0.f;

  const int ch = Clip(channel, 0, kMaxChannels - 1);
  const uint32_t offset = static_cast<uint32_t>(ch) * mBufferSize;
  const uint32_t maxDelay = (mBufferSize > 4U) ? (mBufferSize - 4U) : 0U;

  const float d = Clip(delaySamples, 0.f, static_cast<float>(maxDelay));
  float readPos = static_cast<float>(mWriteAddress) - d;

  if (mUseFastDelayInterp)
  {
    if (readPos < 0.f)
      readPos += static_cast<float>(mBufferSize);

    const int idx0 = static_cast<int>(readPos);
    const float frac = readPos - static_cast<float>(idx0);
    const uint32_t i0 = static_cast<uint32_t>(idx0) & mBufferMask;
    const uint32_t i1 = (i0 + 1U) & mBufferMask;

    const float s0 = buffer[offset + i0];
    const float s1 = buffer[offset + i1];
    return s0 + (frac * (s1 - s0));
  }

  if (readPos < 0.f)
    readPos += static_cast<float>(mBufferSize);
  if (readPos >= static_cast<float>(mBufferSize))
    readPos -= static_cast<float>(mBufferSize);

  const int idx = static_cast<int>(std::floor(readPos));
  const float frac = readPos - static_cast<float>(idx);

  const uint32_t i0 = static_cast<uint32_t>(idx) & mBufferMask;
  const uint32_t iM1 = (i0 - 1U) & mBufferMask;
  const uint32_t i1 = (i0 + 1U) & mBufferMask;
  const uint32_t i2 = (i0 + 2U) & mBufferMask;

  const float xm1 = buffer[offset + iM1];
  const float x0 = buffer[offset + i0];
  const float x1 = buffer[offset + i1];
  const float x2 = buffer[offset + i2];

  const float c0 = x0;
  const float c1 = 0.5f * (x1 - xm1);
  const float c2 = xm1 - (2.5f * x0) + (2.f * x1) - (0.5f * x2);
  const float c3 = (0.5f * (x2 - xm1)) + (1.5f * (x0 - x1));

  return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

float SoreThroat::ProcessSVF(SVFState& state, float input, double cutoffHz, double q, bool highPass, double sr)
{
  // sr is pre-validated by the caller (guaranteed >= 1.0).
  const double cutoff = Clip(cutoffHz, 10.0, sr * 0.45);
  const double resonance = Clip(q, 0.2, 12.0);

  const double g = std::tan(PI * cutoff / sr);
  const double k = 1.0 / resonance;

  const double a1 = 1.0 / (1.0 + g * (g + k));
  const double a2 = g * a1;
  const double a3 = g * a2;

  const double v0 = static_cast<double>(input);
  const double v3 = v0 - state.ic2eq;
  const double v1 = (a1 * state.ic1eq) + (a2 * v3);
  const double v2 = state.ic2eq + (a2 * state.ic1eq) + (a3 * v3);

  state.ic1eq = (2.0 * v1) - state.ic1eq;
  state.ic2eq = (2.0 * v2) - state.ic2eq;

  double out = highPass ? (v0 - (k * v1) - v2) : v2;
  if (std::abs(out) < kSilenceThreshold)
    out = 0.0;
  if (!std::isfinite(out))
    out = 0.0;

  return static_cast<float>(out);
}

void SoreThroat::OnReset()
{
  ResizeDelayBuffer();
  ResetDSPState();

  const double sr = std::max(1.0, GetSampleRate());
  mParamAlpha = CoeffFromTimeMs(sr, 5.0);
  mFilterAlpha = CoeffFromTimeMs(sr, 15.0);
  // Voicing: slowed attack/release so env-driven modulation tracks RMS not peaks.
  // 25ms attack prevents sudden filter/delay jumps on transients.
  // 220ms release sustains modulation smoothly through note tails.
  mEnvAttack = CoeffFromTimeMs(sr, 25.0);
  mEnvRelease = CoeffFromTimeMs(sr, 220.0);
  mOutputEnvRelease = CoeffFromTimeMs(sr, 300.0);
}

void SoreThroat::OnIdle()
{
  mDisplaySender.TransmitData(*this);
}

void SoreThroat::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (nFrames <= 0)
    return;

  const int nInConnected = std::max(0, NInChansConnected());
  const int nOutConnected = std::max(0, NOutChansConnected());
  const int nIn = std::max(1, nInConnected);
  const int nOut = std::max(1, nOutConnected);
  const int nChans = Clip(nOut, 1, kMaxChannels);
  const double sr = std::max(1.0, GetSampleRate());

  const sample* inputPtrs[kMaxChannels] = {nullptr, nullptr};
  sample* outputPtrs[kMaxChannels] = {nullptr, nullptr};

  if (inputs)
  {
    const int inCount = std::min(kMaxChannels, nInConnected);
    for (int c = 0; c < inCount; ++c)
      inputPtrs[c] = inputs[c];

    if (!inputPtrs[1])
      inputPtrs[1] = inputPtrs[0];
  }

  if (outputs)
  {
    const int outCount = std::min(kMaxChannels, nOutConnected);
    for (int c = 0; c < outCount; ++c)
      outputPtrs[c] = outputs[c];

    // Clear channels beyond the plugin's stereo width to avoid stale host buffers.
    for (int c = kMaxChannels; c < nOutConnected; ++c)
    {
      if (outputs[c])
        std::fill(outputs[c], outputs[c] + nFrames, static_cast<sample>(0));
    }
  }

  if (!outputPtrs[0] && !outputPtrs[1])
    return;

  auto* buffer = mBuffer.Get();
  if (!buffer || mBufferSize < 8U || mBufferMask == 0U)
  {
    const double post = GetParam(kPostGain)->DBToAmp();

    for (int s = 0; s < nFrames; ++s)
    {
      const float inL = (inputPtrs[0] && std::isfinite(inputPtrs[0][s])) ? static_cast<float>(inputPtrs[0][s]) : 0.f;
      const float inR = (inputPtrs[1] && std::isfinite(inputPtrs[1][s])) ? static_cast<float>(inputPtrs[1][s]) : inL;

      if (outputPtrs[0])
        outputPtrs[0][s] = static_cast<sample>(inL * static_cast<float>(post));

      if (nChans > 1 && outputPtrs[1])
        outputPtrs[1][s] = static_cast<sample>(inR * static_cast<float>(post));
    }

    return;
  }

  const int voices = Clip(static_cast<int>(GetParam(kVoices)->Value()), 1, kMaxVoices);
  mUseFastDelayInterp = (voices > kFastInterpolationVoiceThreshold);
  
  // Derive stereo rotation/width targets from the macro knob.
  const StereoMacroState stereoState = StereoMacroStateFromValue(static_cast<float>(GetParam(kStereo)->Value()));
  const double rotationTarget = stereoState.rotationRad;
  const float displayWidthFactor = stereoState.widthFactor;
  
  const float minDelaySamples = static_cast<float>(GetParam(kMinDelay)->Value() * sr * 0.001);
  const float delayRangeSamples = static_cast<float>(GetParam(kDelayRange)->Value() * sr * 0.001);
  const float minRateMsPerSec = static_cast<float>(GetParam(kMinRate)->Value());
  const float rateRangeMsPerSec = static_cast<float>(GetParam(kRateRange)->Value());
  const float legacyDamper = static_cast<float>(GetParam(kLegacyDamper)->Value());

  const int stereoMode = (nChans < 2) ? kStereoFree : Clip(static_cast<int>(GetParam(kStereoMode)->Value()), 0, kNumStereoModes - 1);
  const bool updateOnCollision = GetParam(kUpdateOnCollision)->Bool();
  const int updateUnit = Clip(static_cast<int>(GetParam(kUpdateUnit)->Value()), 0, kNumTimeUnits - 1);
  const bool updateQuantize = GetParam(kUpdateQuantize)->Bool();

  const int lfoShape = Clip(static_cast<int>(GetParam(kLFOShape)->Value()), 0, kNumLFOShapes - 1);

  const double manualBPM = GetParam(kManualBPM)->Value();
  const double updateTempo = ResolveTempoBPM(updateUnit, manualBPM);

  const double updateIntervalMs = TimeFromUnitMs(GetParam(kRateUpdate)->GetNormalized(), updateUnit, updateQuantize, updateTempo);
  const double updateIntervalSamples = std::max(1.0, updateIntervalMs * sr * 0.001);
  const double lfoPeriodMs = std::max(1.0, GetParam(kLFORate)->Value());
  const double lfoPhaseInc = 1.0 / std::max(1.0, lfoPeriodMs * sr * 0.001);
  const double lfoRateHz = 1000.0 / lfoPeriodMs;
  const double lfoRateStress = Clip((lfoRateHz - 1.5), 0.0, 6.5) / 6.5;

  // Soft-clipping delay modulation keeps extreme sweeps musical while preserving center feel.
  const double delayModDrive = 1.35;
  const double delayModNorm = 1.0 / std::tanh(delayModDrive);

  if (mUpdateSamplesRemaining <= 0.0 || mUpdateSamplesRemaining > (updateIntervalSamples * 4.0))
    mUpdateSamplesRemaining = updateIntervalSamples;

  EnsureVoiceState(voices, stereoMode, nChans, delayRangeSamples, minRateMsPerSec, rateRangeMsPerSec, legacyDamper);

  const double mixTarget = Clip(GetParam(kMix)->Value() / 100.0, 0.0, 1.0);
  const double preTarget = GetParam(kPregain)->DBToAmp();
  const double postTarget = GetParam(kPostGain)->DBToAmp();
  const double feedbackTarget = Clip(GetParam(kFeedback)->Value() / 100.0, 0.0, 0.99);
  // const double rotationTarget = GetParam(kRotation)->Value() * PI / 180.0;
  const double lowpassTarget = GetParam(kLowpass)->Value();
  const double highpassTarget = GetParam(kHighpass)->Value();
  const double lpqTarget = GetParam(kLPRes)->Value();
  const double hpqTarget = GetParam(kHPRes)->Value();
  const double envLPAmountTarget = GetParam(kEnvLP)->Value() / 100.0;
  const double envHPAmountTarget = GetParam(kEnvHP)->Value() / 100.0;
  const double envDelayAmountTarget = GetParam(kEnvDelay)->Value() / 100.0;
  const double lfoLPAmountTarget = GetParam(kLFOLP)->Value() / 100.0;
  const double lfoHPAmountTarget = GetParam(kLFOHP)->Value() / 100.0;
  const double lfoDelayAmountTarget = GetParam(kLFODelay)->Value() / 100.0;

  const int followMode = Clip(static_cast<int>(GetParam(kFollowLevel)->Value()), 0, kNumFollowLevels - 1);
  const double followAttack =
    (followMode == kFollowFastDecay) ? CoeffFromTimeMs(sr, 2.0)
    : (followMode == kFollowSlowDecay) ? CoeffFromTimeMs(sr, 15.0)
                                      : CoeffFromTimeMs(sr, 5.0);

  const double followRelease =
    (followMode == kFollowFastDecay) ? CoeffFromTimeMs(sr, 35.0)
    : (followMode == kFollowSlowDecay) ? CoeffFromTimeMs(sr, 450.0)
                                       : CoeffFromTimeMs(sr, 120.0);

  const float voiceNorm = 1.f / std::sqrt(static_cast<float>(std::max(1, voices)));

  if (!std::isfinite(mInputEnv)) mInputEnv = 0.0;
  if (!std::isfinite(mOutputEnv)) mOutputEnv = 0.0;
  if (!std::isfinite(mFollowGain)) mFollowGain = 1.0;
  if (!std::isfinite(mUpdateSamplesRemaining)) mUpdateSamplesRemaining = updateIntervalSamples;
  if (!std::isfinite(mLFOPhase)) mLFOPhase = 0.0;
  while (mLFOPhase < 0.0)
    mLFOPhase += 1.0;
  while (mLFOPhase >= 1.0)
    mLFOPhase -= 1.0;

  // --- Block-level constants hoisted out of the per-sample loop ---

  // Per-voice LFO phase stagger: constant for the block (voices doesn't change mid-block).
  const double voiceLFOSpread = (voices > 1) ? (1.0 / static_cast<double>(voices)) : 0.0;
  // Scratch buffer for precomputed per-voice LFO values (populated each sample).
  float voiceLFOVals[kMaxVoices];

  // For sine shape: precompute step cos/sin once per block for iterative phasor walk.
  // This replaces N sin() calls/sample with 2 sin/cos init + N multiplies.
  double lfoSineSC = 1.0, lfoSineSS = 0.0;
  if (lfoShape == kLFOSine && voiceLFOSpread > 0.0)
  {
    const double stepAngle = voiceLFOSpread * (2.0 * PI);
    lfoSineSC = std::cos(stepAngle);
    lfoSineSS = std::sin(stepAngle);
  }

  // Rotation trig computed once per block using target value.
  // rotationTarget barely changes within a typical block; saving 2 trig calls/sample.
  const float targetRotCos = static_cast<float>(std::cos(rotationTarget));
  const float targetRotSin = static_cast<float>(std::sin(rotationTarget));

  // Flush-To-Zero and Denormals-Are-Zero: prevents denormal slowdowns in SVF state,
  // envelope followers, and feedback taps when they decay to near-zero values.
  const int savedMXCSR = _mm_getcsr();
  _mm_setcsr(savedMXCSR | 0x8040); // bit15=FTZ, bit6=DAZ

  for (int s = 0; s < nFrames; ++s)
  {
    mSmMix = SmoothTo(mSmMix, mixTarget, mParamAlpha);
    mSmPregain = SmoothTo(mSmPregain, preTarget, mParamAlpha);
    mSmPostgain = SmoothTo(mSmPostgain, postTarget, mParamAlpha);
    mSmFeedback = SmoothTo(mSmFeedback, feedbackTarget, mParamAlpha);
    mSmRotationRad = SmoothTo(mSmRotationRad, rotationTarget, mParamAlpha);
    mSmLowpassHz = SmoothTo(mSmLowpassHz, lowpassTarget, mFilterAlpha);
    mSmHighpassHz = SmoothTo(mSmHighpassHz, highpassTarget, mFilterAlpha);
    mSmLPQ = SmoothTo(mSmLPQ, lpqTarget, mFilterAlpha);
    mSmHPQ = SmoothTo(mSmHPQ, hpqTarget, mFilterAlpha);
    mSmEnvLP = SmoothTo(mSmEnvLP, envLPAmountTarget, mParamAlpha);
    mSmEnvHP = SmoothTo(mSmEnvHP, envHPAmountTarget, mParamAlpha);
    mSmEnvDelay = SmoothTo(mSmEnvDelay, envDelayAmountTarget, mParamAlpha);
    mSmLFOLP = SmoothTo(mSmLFOLP, lfoLPAmountTarget, mParamAlpha);
    mSmLFOHP = SmoothTo(mSmLFOHP, lfoHPAmountTarget, mParamAlpha);
    mSmLFODelay = SmoothTo(mSmLFODelay, lfoDelayAmountTarget, mParamAlpha);

    mUpdateSamplesRemaining -= 1.0;
    if (mUpdateSamplesRemaining <= 0.0)
    {
      RandomizeRates(voices, stereoMode, nChans, minRateMsPerSec, rateRangeMsPerSec);
      mUpdateSamplesRemaining += updateIntervalSamples;
    }

    float dry[kMaxChannels] = {0.f, 0.f};
    float writeIn[kMaxChannels] = {0.f, 0.f};

    double inputLevel = 0.0;
    for (int c = 0; c < nChans; ++c)
    {
      const int srcIdx = (c < nIn) ? c : 0;
      const sample* src = (srcIdx < kMaxChannels) ? inputPtrs[srcIdx] : inputPtrs[0];
      dry[c] = (src && std::isfinite(src[s])) ? static_cast<float>(src[s]) : 0.f;
      writeIn[c] = dry[c] * static_cast<float>(mSmPregain);
      inputLevel += std::abs(static_cast<double>(writeIn[c]));
    }

    inputLevel /= static_cast<double>(nChans);
    const double envCoeff = (inputLevel > mInputEnv) ? mEnvAttack : mEnvRelease;
    mInputEnv += (inputLevel - mInputEnv) * envCoeff;

    double followFeedbackGain = mSmFeedback;
    if (followMode != kFollowOff)
      followFeedbackGain *= mFollowGain;

    for (int c = 0; c < nChans; ++c)
    {
      const uint32_t offset = static_cast<uint32_t>(c) * mBufferSize;
      const float fb = mFeedbackTap[c] * static_cast<float>(followFeedbackGain);
      buffer[offset + mWriteAddress] = writeIn[c] + fb;
    }

    mLFOPhase += lfoPhaseInc;
    while (mLFOPhase >= 1.0)
      mLFOPhase -= 1.0;

    const double lfo = LFOValue(lfoShape);

    float wet[kMaxChannels] = {0.f, 0.f};

    // Precompute per-voice LFO into voiceLFOVals[].
    // Sine: iterative phasor (angle-addition recurrence) — 2 trig init + N*4 muls
    //   instead of the previous N sin() calls (100x cheaper at N=200, sine shape).
    // Other shapes: incremental phase + single subtract wrap (avoids std::floor/voice).
    if (lfoShape == kLFOSine)
    {
      double si = std::sin(mLFOPhase * (2.0 * PI));
      double co = std::cos(mLFOPhase * (2.0 * PI));
      for (int i = 0; i < voices; ++i)
      {
        voiceLFOVals[i] = static_cast<float>(si);
        const double nsi = si * lfoSineSC + co * lfoSineSS;
        co = co * lfoSineSC - si * lfoSineSS;
        si = nsi;
      }
    }
    else
    {
      for (int i = 0; i < voices; ++i)
      {
        double p = mLFOPhase + static_cast<double>(i) * voiceLFOSpread;
        if (p >= 1.0) p -= 1.0; // max sum < 2.0, single sub sufficient
        if (lfoShape == kLFOTriangle)
          voiceLFOVals[i] = static_cast<float>((2.0 * std::abs((2.0 * p) - 1.0)) - 1.0);
        else if (lfoShape == kLFOSquare)
          voiceLFOVals[i] = (p < 0.5) ? 1.f : -1.f;
        else if (lfoShape == kLFORampUp)
          voiceLFOVals[i] = static_cast<float>((2.0 * p) - 1.0);
        else
          voiceLFOVals[i] = static_cast<float>(1.0 - (2.0 * p));
      }
    }

    const bool updateDots = ((mWriteAddress & 63U) == 0U);
    const double feedbackForMotion = Clip(mSmFeedback, 0.0, 1.0);
    const double delayLfoTrim = 1.0 - (0.30 * lfoRateStress * std::pow(feedbackForMotion, 0.68));

    for (int i = 0; i < voices; ++i)
    {
      auto& v = mVoiceStates[i];

      if (nChans < 2 || stereoMode == kStereoFree)
      {
        UpdateVoiceDelay(v.delayL, v.rateL, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
        if (nChans > 1)
          UpdateVoiceDelay(v.delayR, v.rateR, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
      }
      else if (stereoMode == kStereoSlave)
      {
        const float d = UpdateVoiceDelay(v.delayL, v.rateL, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
        v.delayR = d;
        v.rateR = v.rateL;
      }
      else if (stereoMode == kStereoAntiSlave)
      {
        const float d = UpdateVoiceDelay(v.delayL, v.rateL, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
        v.delayR = std::max(0.f, delayRangeSamples - d);
        v.rateR = -v.rateL;
      }
      else
      {
        if ((i & 1) == 0)
        {
          const float d = UpdateVoiceDelay(v.delayL, v.rateL, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
          v.delayR = d * 0.5f;
          v.rateR = v.rateL * 0.5f;
        }
        else
        {
          const float d = UpdateVoiceDelay(v.delayR, v.rateR, delayRangeSamples, updateOnCollision, minRateMsPerSec, rateRangeMsPerSec);
          v.delayL = d * 0.5f;
          v.rateL = v.rateR * 0.5f;
        }
      }

      const double voiceLFO = static_cast<double>(voiceLFOVals[i]);
        const double delayModRaw = (mSmEnvDelay * mInputEnv) + (mSmLFODelay * delayLfoTrim * voiceLFO);
        const double delayMod = Clip(std::tanh(delayModRaw * delayModDrive) * delayModNorm, -1.0, 1.0);
      const float delayModSamples = static_cast<float>(delayMod * static_cast<double>(delayRangeSamples));

      const float delayL = Clip(minDelaySamples + v.delayL + delayModSamples, 0.f, static_cast<float>(mBufferSize - 4U));
      wet[0] += ReadDelayHermite(0, delayL) * v.gainL * voiceNorm;

      if (nChans > 1)
      {
        const float delayR = Clip(minDelaySamples + v.delayR + delayModSamples, 0.f, static_cast<float>(mBufferSize - 4U));
        wet[1] += ReadDelayHermite(1, delayR) * v.gainR * voiceNorm;
      }
    }

    if (updateDots)
    {
      const float displayRotCos = static_cast<float>(std::cos(mSmRotationRad));
      const float displayRotSin = static_cast<float>(std::sin(mSmRotationRad));
      const float center = 0.5f;
      const float widthFactor = std::max(0.01f, displayWidthFactor);

      for (int i = 0; i < voices; ++i)
      {
        const auto& v = mVoiceStates[i];
        const float normL = (delayRangeSamples > 0.f) ? Clip(v.delayL / delayRangeSamples, 0.f, 1.f) : 0.f;
        const float normR = (nChans > 1 && delayRangeSamples > 0.f) ? Clip(v.delayR / delayRangeSamples, 0.f, 1.f) : normL;
        const float rotL = normL - center;
        const float rotR = normR - center;
        const float rotatedL = rotL * displayRotCos - rotR * displayRotSin;
        const float rotatedR = rotL * displayRotSin + rotR * displayRotCos;
        const float finalL = Clip(center + (rotatedL * widthFactor), 0.f, 1.f);
        const float finalR = Clip(center + (rotatedR * widthFactor), 0.f, 1.f);

        mDots[static_cast<size_t>(i)] = static_cast<uint8_t>(finalL * 255.f);
        mDots[static_cast<size_t>(kMaxVoices + i)] = static_cast<uint8_t>(finalR * 255.f);
      }
    }

    const double lpModOct = Clip((mSmEnvLP * mInputEnv) + (mSmLFOLP * lfo), -1.0, 1.0) * 3.0;
    const double hpModOct = Clip((mSmEnvHP * mInputEnv) + (mSmLFOHP * lfo), -1.0, 1.0) * 3.0;

    // Adaptive tone guardrails: rein in low-end buildup and high-end fizz as feedback rises.
    const double feedbackAmt = Clip(mSmFeedback, 0.0, 1.0);
    const double feedbackCurve = std::pow(feedbackAmt, 1.25);
    const double lpAdaptiveMax = std::max(4200.0, 19000.0 - (10000.0 * feedbackCurve));
    const double hpAdaptiveMin = 16.0 + (52.0 * feedbackCurve);

    double lpCut = Clip(mSmLowpassHz * std::exp2(lpModOct), 20.0, std::min(sr * 0.45, lpAdaptiveMax));
    double hpCut = Clip(mSmHighpassHz * std::exp2(hpModOct), std::max(10.0, hpAdaptiveMin), sr * 0.45);
    hpCut = std::min(hpCut, lpCut * 0.95);

    double lpA1, lpA2, lpA3, lpK;
    {
      const double cutoff = Clip(lpCut, 10.0, sr * 0.45);
      const double resonance = Clip(mSmLPQ, 0.2, 12.0);
      const double g = std::tan(PI * cutoff / sr);
      lpK = 1.0 / resonance;
      lpA1 = 1.0 / (1.0 + g * (g + lpK));
      lpA2 = g * lpA1;
      lpA3 = g * lpA2;
    }

    double hpA1, hpA2, hpA3, hpK;
    {
      const double cutoff = Clip(hpCut, 10.0, sr * 0.45);
      const double resonance = Clip(mSmHPQ, 0.2, 12.0);
      const double g = std::tan(PI * cutoff / sr);
      hpK = 1.0 / resonance;
      hpA1 = 1.0 / (1.0 + g * (g + hpK));
      hpA2 = g * hpA1;
      hpA3 = g * hpA2;
    }

    for (int c = 0; c < nChans; ++c)
    {
      {
        const double v0 = static_cast<double>(wet[c]);
        const double v3 = v0 - mLPState[c].ic2eq;
        const double v1 = (lpA1 * mLPState[c].ic1eq) + (lpA2 * v3);
        const double v2 = mLPState[c].ic2eq + (lpA2 * mLPState[c].ic1eq) + (lpA3 * v3);

        mLPState[c].ic1eq = (2.0 * v1) - mLPState[c].ic1eq;
        mLPState[c].ic2eq = (2.0 * v2) - mLPState[c].ic2eq;

        wet[c] = static_cast<float>(v2);
      }

      {
        const double v0 = static_cast<double>(wet[c]);
        const double v3 = v0 - mHPState[c].ic2eq;
        const double v1 = (hpA1 * mHPState[c].ic1eq) + (hpA2 * v3);
        const double v2 = mHPState[c].ic2eq + (hpA2 * mHPState[c].ic1eq) + (hpA3 * v3);

        mHPState[c].ic1eq = (2.0 * v1) - mHPState[c].ic1eq;
        mHPState[c].ic2eq = (2.0 * v2) - mHPState[c].ic2eq;

        double out = v0 - (hpK * v1) - v2;
        if (std::abs(out) < kSilenceThreshold)
          out = 0.0;
        if (!std::isfinite(out))
          out = 0.0;

        wet[c] = static_cast<float>(out);
      }
    }

    double outputLevel = 0.0;
    for (int c = 0; c < nChans; ++c)
      outputLevel += std::abs(static_cast<double>(wet[c]));
    outputLevel /= static_cast<double>(nChans);

    const double outCoeff = (outputLevel > mOutputEnv) ? mEnvAttack : mOutputEnvRelease;
    mOutputEnv += (outputLevel - mOutputEnv) * outCoeff;

    if (followMode == kFollowOff)
    {
      mFollowGain = 1.0;
    }
    else
    {
      const double ratio = Clip((mInputEnv + 1e-5) / (mOutputEnv + 1e-5), 0.0, 1.0);
      const double coeff = (ratio < mFollowGain) ? followAttack : followRelease;
      mFollowGain += (ratio - mFollowGain) * coeff;
      mFollowGain = Clip(mFollowGain, 0.0, 1.0);
    }

    const double screamCurve = std::pow(feedbackAmt, 0.72);
    const float fbDrive = static_cast<float>(1.0 + (0.24 * feedbackCurve) + (0.44 * screamCurve));
    const float fbAsymmetry = static_cast<float>(0.04 + (0.18 * screamCurve));
    const float fbTone = static_cast<float>(1.0 - (0.16 * feedbackCurve));
    if (nChans > 1)
    {
      const float rotL = (wet[0] * targetRotCos) - (wet[1] * targetRotSin);
      const float rotR = (wet[0] * targetRotSin) + (wet[1] * targetRotCos);
      mFeedbackTap[0] = DemonicFeedbackShape(rotL, fbDrive, fbAsymmetry) * fbTone;
      mFeedbackTap[1] = DemonicFeedbackShape(rotR, fbDrive * 0.985f, fbAsymmetry * 0.92f) * fbTone;
    }
    else
    {
      mFeedbackTap[0] = DemonicFeedbackShape(wet[0], fbDrive, fbAsymmetry) * fbTone;
      mFeedbackTap[1] = 0.f;
    }

    const double mixClamped = Clip(mSmMix, 0.0, 1.0);
    const float dryMixGain = static_cast<float>(std::sqrt(1.0 - mixClamped));
    const float wetMixGain = static_cast<float>(std::sqrt(mixClamped));

    for (int c = 0; c < nChans; ++c)
    {
      const float mixed = (dry[c] * dryMixGain) + (wet[c] * wetMixGain);
      const float out = mixed * static_cast<float>(mSmPostgain);

      if (outputPtrs[c])
      {
        outputPtrs[c][s] = static_cast<sample>(out);

        if (!std::isfinite(outputPtrs[c][s]) || std::abs(outputPtrs[c][s]) < kSilenceThreshold)
          outputPtrs[c][s] = 0.f;
      }
    }

    mWriteAddress = (mWriteAddress + 1U) & mBufferMask;
  }

  _mm_setcsr(savedMXCSR); // Restore FP control state.

  mDots[kMaxVoices * 2 + 0] = static_cast<uint8_t>(voices);
  mDots[kMaxVoices * 2 + 1] = static_cast<uint8_t>(voices);
  mDisplaySender.PushData(ISenderData<1, std::array<uint8_t, kDisplaySlots> >(kCtrlTagDisplay, {mDots}));
}
#endif
