#include "GrungrIPlug2UI.h"

#include <array>
#include <algorithm>
#include <cmath>

#ifdef OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

namespace grungr {
namespace ui {
namespace {

using namespace iplug;
using namespace igraphics;

static constexpr const char* kMainFontTag = "NoMoreHeroMain";
static constexpr const char* kSecondaryFontTag = "FutureErodedSecondary";
static constexpr const char* kTertiaryFontTag = "BoxPot";
static constexpr bool kForceSafeVectorUIMode = false;
static constexpr bool kUseSVGKnobs = true;

// Knob circle centers measured from GrungrFaceplate.svg.
// Positions are normalized to the cropped SVG viewBox.
static constexpr std::array<float, 4> kKnobCentersXNorm = {
  0.147585f,
  0.380634f,
  0.615140f,
  0.846733f};
static constexpr float kKnobCenterYNorm = 0.180890f;
static constexpr float kKnobDiameterNorm = 0.152210f;
static constexpr float kKnobSizeBoost = 1.20f;
static constexpr float kLedYNorm = 0.504f;       // SVG red-dot y in background coords
static constexpr float kTreadTopNorm = 0.565f;    // y where tread rubber visually begins
static constexpr float kStompLeftNorm = 0.055f;
static constexpr float kStompRightNorm = 0.945f;
static constexpr float kStompTopNorm = 0.46f;     // stomp rect extends above LED
static constexpr float kStompBottomNorm = 0.955f;
static constexpr float kDesignWidth = static_cast<float>(PLUG_WIDTH);
static constexpr float kDesignHeight = static_cast<float>(PLUG_HEIGHT);
static constexpr float kMinInteractiveTargetPx = 44.f;
static constexpr float kClophLogoAspect = 1461.f / 569.f;

// Single-theme design tokens for Grungr.
static const igraphics::IColor kTokenFallbackBackground(255, 124, 96, 106);
static const igraphics::IColor kTokenTitleText(236, 218, 201, 189);
static const igraphics::IColor kTokenKnobLabelText(236, 218, 201, 189);
static const igraphics::IColor kTokenKnobValueText(248, 241, 230, 214);
static const igraphics::IColor kTokenToggleLabelText(242, 233, 214, 202);
static const igraphics::IColor kTokenToggleValueText(250, 244, 234, 218);
static const igraphics::IColor kTokenVectorBG(164, 48, 38, 44);
static const igraphics::IColor kTokenVectorFG(250, 237, 225, 210);
static const igraphics::IColor kTokenVectorPR(255, 160, 60, 40);
static const igraphics::IColor kTokenVectorFR(188, 88, 66, 73);
static const igraphics::IColor kTokenStripeFill(255, 16, 12, 13);
static const igraphics::IColor kTokenStripeHighlight(24, 240, 224, 205);
static const igraphics::IColor kTokenLedBezelOuter(255, 28, 20, 20);
static const igraphics::IColor kTokenLedBezelInner(255, 48, 36, 36);
static const igraphics::IColor kTokenLedLensHighlight(255, 240, 230, 220);
// Use a subtle, neutral focus ring to avoid harsh yellow hover rectangles.
static const igraphics::IColor kTokenFocusRing(255, 40, 40, 40);
static const igraphics::IColor kTokenRipple(255, 255, 255, 255);
static const igraphics::IColor kTokenRawHardwarePlateOuter(252, 166, 126, 133);
static const igraphics::IColor kTokenRawHardwarePlateInner(248, 214, 188, 191);
static const igraphics::IColor kTokenRawHardwarePlateEdge(244, 99, 60, 67);
static const igraphics::IColor kTokenRawHardwareSlotFill(255, 31, 30, 32);
static const igraphics::IColor kTokenRawHardwareSlotHighlight(158, 184, 145, 132);
static const igraphics::IColor kTokenRawHardwareHandleFill(255, 95, 84, 82);
static const igraphics::IColor kTokenRawHardwareHandleEdge(250, 224, 198, 202);
static const igraphics::IColor kTokenRawHardwareHandleGrip(200, 44, 37, 38);
static constexpr float kTokenUnderlayContrast = -0.08f;
static constexpr float kRippleDecayRate = 0.045f;
static constexpr float kKeyboardFocusDecayRate = 0.085f;
static constexpr float kKeyboardFocusDecayReducedRate = 0.22f;
static constexpr float kRippleMaxAlpha = 50.f;
// Reduce focus alpha to make hover/focus outlines much more subtle.
static constexpr float kFocusAlphaBase = 18.f;
static constexpr float kFocusAlphaRange = 48.f;

// Clear, action-oriented copy for controls and tooltips.
static constexpr const char* kLabelGrunge = "DRIVE";
static constexpr const char* kLabelButt = "BODY";
static constexpr const char* kLabelFace = "BITE";
static constexpr const char* kLabelLoud = "LEVEL";
static constexpr const char* kTitleText = "GRUNGR";
static constexpr const char* kTooltipGrunge = "Drive (Grunge): Adds distortion and texture.";
static constexpr const char* kTooltipButt = "Body (Butt): Adds low-end weight and thickness.";
static constexpr const char* kTooltipFace = "Bite (Face): Adds upper-mid cut and presence.";
static constexpr const char* kTooltipLoud = "Level (Loud): Sets the pedal output volume.";
static constexpr const char* kTooltipStomp = "Stomp: Turns the effect on/off. Click or Space. Press M to reduce motion.";
static constexpr const char* kTooltipRaw = "RAW voicing: Choose RAW for an open, gritty tone or MOD for a tighter, smoother tone.";
static constexpr const char* kRawStateOnText = "RAW";
static constexpr const char* kRawStateEnhancedText = "MOD";
static constexpr const char* kRawTrackLeftText = "RAW";
static constexpr const char* kRawTrackRightText = "ENH";

bool DetectReducedMotionPreference()
{
#ifdef OS_WIN
  BOOL clientAreaAnimations = TRUE;

  if (SystemParametersInfoA(SPI_GETCLIENTAREAANIMATION, 0, &clientAreaAnimations, 0)) {
    return clientAreaAnimations == FALSE;
  }
#endif

  return false;
}

struct RuntimeBehaviorState {
  bool reduceMotion = DetectReducedMotionPreference();
};

RuntimeBehaviorState gRuntimeBehavior;

struct ThemePalette {
  igraphics::IColor fallbackBackground;
  igraphics::IColor underlayBackground;
  igraphics::IColor titleText;
  igraphics::IColor knobLabelText;
  igraphics::IColor knobValueText;
  igraphics::IColor toggleLabelText;
  igraphics::IColor toggleValueText;
  igraphics::IColor vectorBG;
  igraphics::IColor vectorFG;
  igraphics::IColor vectorPR;
  igraphics::IColor vectorFR;
  igraphics::IColor stripeFill;
  igraphics::IColor stripeHighlight;
  igraphics::IColor ledBezelOuter;
  igraphics::IColor ledBezelInner;
  igraphics::IColor ledLensHighlight;
  igraphics::IColor rawHardwarePlateOuter;
  igraphics::IColor rawHardwarePlateInner;
  igraphics::IColor rawHardwarePlateEdge;
  igraphics::IColor rawHardwareSlotFill;
  igraphics::IColor rawHardwareSlotHighlight;
  igraphics::IColor rawHardwareHandleFill;
  igraphics::IColor rawHardwareHandleEdge;
  igraphics::IColor rawHardwareHandleGrip;
  igraphics::IColor focusRing;
  igraphics::IColor ripple;
};

// A simple solid rounded-rect panel, used for the rubber knob stripe.
class KnobStripeControl final : public igraphics::IControl
{
public:
  KnobStripeControl(const igraphics::IRECT& bounds,
                   float cornerRadius,
                   const igraphics::IColor& fillColor,
                   const igraphics::IColor& highlightColor)
  : IControl(bounds, -1)
  , mCornerRadius(cornerRadius)
  , mFillColor(fillColor)
  , mHighlightColor(highlightColor)
  {}

  void Draw(igraphics::IGraphics& g) override
  {
    g.FillRoundRect(mFillColor, mRECT, mCornerRadius, &mBlend);
    // Single-pixel inner highlight along top edge gives depth.
    const igraphics::IRECT hiLine(mRECT.L + mCornerRadius, mRECT.T + 1.f,
                                  mRECT.R - mCornerRadius, mRECT.T + 2.5f);
    g.FillRect(mHighlightColor, hiLine, &mBlend);
  }

private:
  float mCornerRadius;
  igraphics::IColor mFillColor;
  igraphics::IColor mHighlightColor;
};

class AnimatedStompBypassControl final : public igraphics::IControl
{
public:
  AnimatedStompBypassControl(const igraphics::IRECT& bounds,
                             int paramIdx,
                             const ThemePalette& palette,
                             bool reduceMotion,
                             const igraphics::ISVG& shoeSoleSVG,
                             const char* textFontID = "Roboto-Regular")
  : IControl(bounds, paramIdx)
  , mReduceMotion(reduceMotion)
  , mShoeSoleSVG(shoeSoleSVG)
  , mFontID((textFontID && textFontID[0] != '\0') ? textFontID : "Roboto-Regular")
  {
    mDblAsSingleClick = true;
    SetPalette(palette);
  }

  void OnInit() override
  {
    mVisualDown = IsEngaged() ? 1.f : 0.f;
    mPrevEngaged = IsEngaged();
    mEngagePulseT = 0.f;
  }

  void SetPalette(const ThemePalette& palette)
  {
    mLedBezelOuter = palette.ledBezelOuter;
    mLedBezelInner = palette.ledBezelInner;
    mLedOn = igraphics::IColor(255, 220, 40, 50);
    mLedOff = palette.vectorBG.WithContrast(-0.30f);
    mLedGlow = mLedOn.WithOpacity(0.45f);
    mLedLensHighlight = palette.ledLensHighlight;
    mFocusRing = palette.focusRing;
    mRipple = palette.ripple;
  }

  void SetReducedMotion(bool reduceMotion)
  {
    if (mReduceMotion == reduceMotion)
      return;

    mReduceMotion = reduceMotion;
    if (mReduceMotion) {
      mRippleT = 0.f;
      mEngagePulseT = 0.f;
    }

    SetDirty(false);
  }

  void ToggleFromKeyboard()
  {
    static constexpr float kTreadRelY =
        (kTreadTopNorm - kStompTopNorm) / (kStompBottomNorm - kStompTopNorm);
    const float treadTop = mRECT.T + mRECT.H() * kTreadRelY;
    const float treadCenterY = (treadTop + mRECT.B) * 0.5f;
    TriggerToggle(mRECT.MW(), treadCenterY, true);
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const bool engaged = IsEngaged();
    const float targetDown = engaged ? 1.f : 0.f;

    if (engaged != mPrevEngaged) {
      mPrevEngaged = engaged;
      if (!mReduceMotion && engaged)
        mEngagePulseT = 1.f;
    }

    if (mReduceMotion) {
      if (std::fabs(targetDown - mVisualDown) > 0.0005f) {
        mVisualDown = targetDown;
        SetDirty(false);
      }
    }
    else {
      // Fast press, slow release — asymmetric spring rates.
      const float rate = (targetDown > mVisualDown) ? 0.38f : 0.14f;
      const float diff = targetDown - mVisualDown;

      if (std::fabs(diff) > 0.0005f) {
        mVisualDown += diff * rate;
        if (std::fabs(targetDown - mVisualDown) < 0.001f)
          mVisualDown = targetDown;
        SetDirty(false);
      }
    }

    // LED position in control-local coordinates.
    static constexpr float kLedRelY =
        (kLedYNorm - kStompTopNorm) / (kStompBottomNorm - kStompTopNorm);

    const float ledOuterR = mRECT.W() * 0.044f;
    const float ledInnerR = ledOuterR * 0.62f;
    const float ledX = mRECT.MW();
    const float ledY = mRECT.T + mRECT.H() * kLedRelY;

    // Outer bezel / socket.
    g.FillCircle(mLedBezelOuter, ledX, ledY, ledOuterR * 1.25f, &mBlend);
    g.FillCircle(mLedBezelInner, ledX, ledY, ledOuterR, &mBlend);

    // Glow halo — layered falloff for a realistic light spread.
    if (mVisualDown > 0.01f) {
      float pulse = 1.f;

      if (!mReduceMotion && mEngagePulseT > 0.001f) {
        static constexpr float kTwoPi = 6.28318530f;
        static constexpr float kPulseCycles = 2.2f;
        const float phase = (1.f - mEngagePulseT) * kTwoPi * kPulseCycles;
        pulse = 0.90f + 0.16f * std::sin(phase) * mEngagePulseT + 0.10f * mEngagePulseT;
      }

      const float glowStrength = mVisualDown * pulse;
      // Outer soft spread.
      const float outerGlow = glowStrength * 0.12f;
      g.FillCircle(igraphics::IColor(static_cast<int>(mLedGlow.A * outerGlow),
                                     mLedGlow.R, mLedGlow.G, mLedGlow.B),
                   ledX, ledY, ledOuterR * 4.8f, &mBlend);
      // Mid halo.
      const float midGlow = glowStrength * 0.30f;
      g.FillCircle(igraphics::IColor(static_cast<int>(mLedGlow.A * midGlow),
                                     mLedGlow.R, mLedGlow.G, mLedGlow.B),
                   ledX, ledY, ledOuterR * 2.6f, &mBlend);
      // Bright inner corona (matching LED colour).
      const float innerGlow = glowStrength * 0.70f;
      g.FillCircle(igraphics::IColor(static_cast<int>(255 * innerGlow),
                                      mLedOn.R, mLedOn.G, mLedOn.B),
                   ledX, ledY, ledOuterR * 1.3f, &mBlend);
    }

    // LED lens.
    const igraphics::IColor ledColor = LerpColor(mLedOff, mLedOn, mVisualDown);
    g.FillCircle(ledColor, ledX, ledY, ledInnerR, &mBlend);

    // Muted specular highlight on lens (removed the harsh white glow/halo).
    const float hiR = ledInnerR * 0.38f;
    const igraphics::IColor hiColor = mLedLensHighlight.WithOpacity(0.4f * mVisualDown);
    g.FillCircle(hiColor, ledX - ledInnerR * 0.3f, ledY - ledInnerR * 0.3f, hiR, &mBlend);

    // Secondary highlight with interpolated brightness.
    const igraphics::IColor hiColor2(static_cast<int>(120 * mVisualDown + 40 * (1.f - mVisualDown)),
                                     mLedLensHighlight.R,
                                     mLedLensHighlight.G,
                                     mLedLensHighlight.B);
    g.FillCircle(hiColor2, ledX - ledInnerR * 0.22f, ledY - ledInnerR * 0.26f, hiR, &mBlend);

    // Stomp ripple — expands and fades from click point over the tread.
    if (!mReduceMotion && mRippleT > 0.001f) {
      static constexpr float kTreadRelY =
          (kTreadTopNorm - kStompTopNorm) / (kStompBottomNorm - kStompTopNorm);
      const float treadTop = mRECT.T + mRECT.H() * kTreadRelY;
      const float maxR = std::max(mRECT.W(), mRECT.B - treadTop) * 0.80f;
      const float rippleR = maxR * (1.f - mRippleT);
      const int rippleA = static_cast<int>(kRippleMaxAlpha * mRippleT);
      g.FillCircle(igraphics::IColor(rippleA, mRipple.R, mRipple.G, mRipple.B),
                   mRippleX, mRippleY, rippleR, &mBlend);
      mRippleT -= kRippleDecayRate;
      if (mRippleT < 0.f) mRippleT = 0.f;
      SetDirty(false);
    }

    const float hoverStrength = GetMouseIsOver() ? 0.35f : 0.f;
    const float focusStrength = std::clamp(hoverStrength + mKeyboardFocus, 0.f, 1.f);

    if (focusStrength > 0.001f) {
      const int focusA = static_cast<int>(kFocusAlphaBase + (kFocusAlphaRange * focusStrength));
      const igraphics::IColor focusColor(focusA, mFocusRing.R, mFocusRing.G, mFocusRing.B);
      const float focusPad = std::max(2.f, mRECT.W() * 0.010f);
      const float focusThickness = std::max(1.4f, mRECT.W() * 0.012f);
      const float focusCorner = std::max(7.f, mRECT.H() * 0.055f);
      g.DrawRoundRect(focusColor, mRECT.GetPadded(-focusPad), focusCorner, &mBlend, focusThickness);
    }

    if (mKeyboardFocus > 0.001f) {
      mKeyboardFocus -= mReduceMotion ? kKeyboardFocusDecayReducedRate : kKeyboardFocusDecayRate;
      if (mKeyboardFocus < 0.f)
        mKeyboardFocus = 0.f;
      SetDirty(false);
    }

    // Shoe sole overlay — fades in/out, depth-shifted by the cave-in offset.
    if (mPressT > 0.001f) {
      if (mShoeSoleSVG.IsValid()) {
        static constexpr float kTreadRelY =
            (kTreadTopNorm - kStompTopNorm) / (kStompBottomNorm - kStompTopNorm);
        const float treadTop = mRECT.T + mRECT.H() * kTreadRelY;
        const float treadH = mRECT.B - treadTop;

        const float soleW = mRECT.W() * 1.22f;
        const float aspect = mShoeSoleSVG.H() / mShoeSoleSVG.W();
        const float soleH = soleW * aspect;
        const float cx = mRECT.MW();
        const float cy = treadTop + treadH * 0.08f + soleH * 0.5f;

        const igraphics::IRECT soleRect(cx - soleW * 0.5f,
                                         cy - soleH * 0.5f,
                                         cx + soleW * 0.5f,
                                         cy + soleH * 0.5f);

        igraphics::IBlend soleBlend(igraphics::EBlend::SrcOver, mPressT * 0.5f);
        g.DrawSVG(mShoeSoleSVG, soleRect, &soleBlend);
      }

      mPressT -= 0.07f;
      if (mPressT < 0.f) mPressT = 0.f;
      SetDirty(false);
    }

    if (!mReduceMotion && mEngagePulseT > 0.001f) {
      static constexpr float kEngagePulseDecayRate = 0.070f;
      mEngagePulseT -= kEngagePulseDecayRate;
      if (mEngagePulseT < 0.f)
        mEngagePulseT = 0.f;
      SetDirty(false);
    }


  }

  void OnMouseDown(float x, float y, const igraphics::IMouseMod& mod) override
  {
    if (mod.R) {
      PromptUserInput();
      return;
    }

    TriggerToggle(x, y, false);
  }

  bool OnKeyDown(float x, float y, const iplug::IKeyPress& key) override
  {
    if (key.C || key.A)
      return false;

    if (key.VK == iplug::kVK_SPACE || key.VK == iplug::kVK_RETURN) {
      ToggleFromKeyboard();
      return true;
    }

    return false;
  }

  void OnMouseOver(float x, float y, const igraphics::IMouseMod& mod) override
  {
    IControl::OnMouseOver(x, y, mod);
    SetDirty(false);
  }

  void OnMouseOut() override
  {
    IControl::OnMouseOut();
    SetDirty(false);
  }

private:
  void TriggerToggle(float rippleX, float rippleY, bool keyboardInitiated)
  {
    mPressT = 1.f;
    if (!mReduceMotion) {
      mRippleX = rippleX;
      mRippleY = rippleY;
      mRippleT = keyboardInitiated ? 0.55f : 1.f;
    }
    else {
      mRippleT = 0.f;
    }

    if (keyboardInitiated)
      mKeyboardFocus = 1.f;

    const double nextBypassValue = IsEngaged() ? 1.0 : 0.0;
    SetValueFromUserInput(nextBypassValue);
    SetDirty(false);
  }

  static igraphics::IColor LerpColor(const igraphics::IColor& from,
                                     const igraphics::IColor& to,
                                     float t)
  {
    const float clampedT = std::clamp(t, 0.f, 1.f);
    auto lerpPart = [clampedT](int a, int b) {
      return static_cast<int>(a + (b - a) * clampedT);
    };

    return igraphics::IColor(lerpPart(from.A, to.A),
                             lerpPart(from.R, to.R),
                             lerpPart(from.G, to.G),
                             lerpPart(from.B, to.B));
  }

  bool IsEngaged() const
  {
    // In this project, bypass=0 means the effect is engaged and the stomp is "down".
    return GetValue() < 0.5;
  }

  float mVisualDown = 0.f;
  float mPressT = 0.f;
  float mEngagePulseT = 0.f;
  float mRippleT = 0.f;
  float mRippleX = 0.f;
  float mRippleY = 0.f;
  igraphics::IColor mLedBezelOuter;
  igraphics::IColor mLedBezelInner;
  igraphics::IColor mLedOn;
  igraphics::IColor mLedOff;
  igraphics::IColor mLedGlow;
  igraphics::IColor mLedLensHighlight;
  igraphics::IColor mFocusRing;
  igraphics::IColor mRipple;
  bool mReduceMotion = false;
  float mKeyboardFocus = 0.f;
  bool mPrevEngaged = false;
  igraphics::ISVG mShoeSoleSVG;
  const char* mFontID = "Roboto-Regular";
};

class RawModeToggleControl final : public igraphics::IControl
{
public:
  RawModeToggleControl(const igraphics::IRECT& bounds,
                       int rawVoicingParamIdx,
                       const ThemePalette& palette,
                       const char* textFontID)
  : IControl(bounds, rawVoicingParamIdx)
  , mFontID((textFontID && textFontID[0] != '\0') ? textFontID : "Roboto-Regular")
  {
    mDblAsSingleClick = true;
    SetPalette(palette);
  }

  void SetPalette(const ThemePalette& palette)
  {
    mLabelColor = palette.toggleLabelText;
    mMetalPlate = palette.rawHardwarePlateOuter;
    mPlateInset = palette.rawHardwarePlateInner;
    mMetalStroke = palette.rawHardwarePlateEdge;
    mSlotFill = palette.rawHardwareSlotFill;
    mSlotHighlight = palette.rawHardwareSlotHighlight;
    mSliderCap = palette.rawHardwareHandleFill;
    mSliderEdge = palette.rawHardwareHandleEdge;
    mHandleGrip = palette.rawHardwareHandleGrip;
    mFocusRing = palette.focusRing;
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const bool rawOn = IsRawOn();
    const float labelW = std::clamp(mRECT.W() * 0.38f, 48.f, 108.f);
    const float sideGap = std::max(6.f, mRECT.W() * 0.045f);
    const igraphics::IRECT labelRect(mRECT.L, mRECT.T, mRECT.L + labelW, mRECT.B);
    const igraphics::IRECT switchArea(labelRect.R + sideGap, mRECT.T, mRECT.R, mRECT.B);

    const float switchW = std::clamp(switchArea.W() * 0.94f, 60.f, 150.f);
    const float switchH = std::clamp(switchArea.H() * 0.62f, 24.f, 46.f);
    const float centerX = switchArea.MW();
    const float centerY = switchArea.MH();

    const igraphics::IRECT topPlate(centerX - switchW * 0.5f,
                                    centerY - switchH * 0.5f,
                                    centerX + switchW * 0.5f,
                                    centerY + switchH * 0.5f);

    const float plateCorner = std::max(2.4f, topPlate.H() * 0.18f);
    g.FillRoundRect(mMetalPlate, topPlate, plateCorner, &mBlend);

    const float insetPad = std::max(1.2f, topPlate.H() * 0.08f);
    const igraphics::IRECT insetPlate(topPlate.L + insetPad,
                                      topPlate.T + insetPad,
                                      topPlate.R - insetPad,
                                      topPlate.B - insetPad);
    g.FillRoundRect(mPlateInset,
                    insetPlate,
                    std::max(1.8f, plateCorner - insetPad),
                    &mBlend);

    const float frameThickness = std::max(1.0f, mRECT.W() * 0.014f);
    const float frameBoost = GetMouseIsOver() ? 0.18f : 0.f;
    g.DrawRoundRect(mMetalStroke.WithOpacity(0.72f + frameBoost),
                    topPlate,
                    plateCorner,
                    &mBlend,
                    frameThickness);

    const float slotW = insetPlate.W() * 0.56f;
    const float slotH = insetPlate.H() * 0.25f;
    const igraphics::IRECT slotRect(insetPlate.MW() - slotW * 0.5f,
                                    insetPlate.MH() - slotH * 0.5f,
                                    insetPlate.MW() + slotW * 0.5f,
                                    insetPlate.MH() + slotH * 0.5f);
    g.FillRoundRect(mSlotFill,
                    slotRect,
                    std::max(1.0f, slotRect.H() * 0.45f),
                    &mBlend);

    // Muted/removed harsh horizontal highlight on the toggle slot.
    g.DrawRoundRect(mSlotHighlight.WithOpacity(0.3f),
                    slotRect,
                    std::max(1.0f, slotRect.H() * 0.45f),
                    &mBlend,
                    1.0f);

    const float sliderSize = std::clamp(insetPlate.H() * 0.46f, 12.f, 20.f);
    const float travel = (slotRect.W() - sliderSize) * 0.5f;
    const float sliderX = slotRect.MW() + (rawOn ? -travel : travel);
    const igraphics::IRECT sliderRect(sliderX - sliderSize * 0.5f,
                                      insetPlate.MH() - sliderSize * 0.5f,
                                      sliderX + sliderSize * 0.5f,
                                      insetPlate.MH() + sliderSize * 0.5f);
    g.FillRect(mSliderCap, sliderRect, &mBlend);
    g.DrawRect(mSliderEdge,
               sliderRect,
               &mBlend,
               std::max(1.0f, frameThickness));

    const float gripInset = std::max(2.0f, sliderRect.W() * 0.26f);
    const float gripW = std::max(1.2f, sliderRect.W() * 0.08f);
    const igraphics::IRECT leftGrip(sliderRect.L + gripInset,
                                    sliderRect.T + 2.f,
                                    sliderRect.L + gripInset + gripW,
                                    sliderRect.B - 2.f);
    const igraphics::IRECT rightGrip(sliderRect.R - gripInset - gripW,
                                     sliderRect.T + 2.f,
                                     sliderRect.R - gripInset,
                                     sliderRect.B - 2.f);
    g.FillRect(mHandleGrip, leftGrip, &mBlend);
    g.FillRect(mHandleGrip, rightGrip, &mBlend);

    const char* stateLabel = rawOn ? kRawStateOnText : kRawStateEnhancedText;
    const float maxStateWidth = labelRect.W() * 0.92f;

    // Size the text to fill the label, using the longer string as reference.
    igraphics::IRECT measureRect = labelRect;
    igraphics::IText testText(16.f, mLabelColor.WithOpacity(0.90f), mFontID, igraphics::EAlign::Center, igraphics::EVAlign::Middle);
    g.MeasureText(testText, kRawStateOnText, measureRect);
    float rawW = measureRect.W();
    g.MeasureText(testText, kRawStateEnhancedText, measureRect);
    float enhancedW = measureRect.W();

    float maxStrW = std::max(rawW, enhancedW);
    float stateSize = 16.f;
    if (maxStrW > maxStateWidth && maxStrW > 0.001f) {
      stateSize = std::clamp(16.f * (maxStateWidth / maxStrW), 10.f, 16.f);
    }

    igraphics::IText stateText(stateSize, mLabelColor.WithOpacity(0.90f), mFontID, igraphics::EAlign::Center, igraphics::EVAlign::Middle);
    g.DrawText(stateText, stateLabel, labelRect, &mBlend);

    const float hoverStrength = GetMouseIsOver() ? 0.35f : 0.f;
    const float focusStrength = std::clamp(hoverStrength + mKeyboardFocus, 0.f, 1.f);

    if (focusStrength > 0.001f) {
      const int focusA = static_cast<int>(kFocusAlphaBase + (kFocusAlphaRange * focusStrength));
      const igraphics::IColor focusColor(focusA, mFocusRing.R, mFocusRing.G, mFocusRing.B);
      const igraphics::IRECT focusRect(labelRect.L,
                                       topPlate.T - 3.f,
                                       topPlate.R + 3.f,
                                       topPlate.B + 4.f);
      const float focusThickness = std::max(1.1f, mRECT.W() * 0.015f);
      const float focusCorner = std::max(3.5f, focusRect.H() * 0.14f);
      g.DrawRoundRect(focusColor, focusRect, focusCorner, &mBlend, focusThickness);
    }

    if (mKeyboardFocus > 0.001f) {
      mKeyboardFocus -= kKeyboardFocusDecayRate;
      if (mKeyboardFocus < 0.f)
        mKeyboardFocus = 0.f;
      SetDirty(false);
    }
  }

  void OnMouseDown(float x, float y, const igraphics::IMouseMod& mod) override
  {
    if (mod.R)
      return;

    ToggleRawMode(false);
  }

  bool OnKeyDown(float x, float y, const iplug::IKeyPress& key) override
  {
    if (key.C || key.A)
      return false;

    if (key.VK == iplug::kVK_SPACE || key.VK == iplug::kVK_RETURN) {
      ToggleRawMode(true);
      return true;
    }

    return false;
  }

  void OnMouseOver(float x, float y, const igraphics::IMouseMod& mod) override
  {
    IControl::OnMouseOver(x, y, mod);
    SetDirty(false);
  }

  void OnMouseOut() override
  {
    IControl::OnMouseOut();
    SetDirty(false);
  }

private:
  bool IsRawOn() const
  {
    // RAW voicing is stored in the Faust "Enhanced" switch.
    // raw=true  => enhanced=false (0)
    // raw=false => enhanced=true  (1)
    return GetValue() < 0.5;
  }

  void ToggleRawMode(bool keyboardInitiated)
  {
    const bool nextRawOn = !IsRawOn();
    const double nextRawVoicingValue = nextRawOn ? 0.0 : 1.0;

    if (keyboardInitiated)
      mKeyboardFocus = 1.f;

    SetValueFromUserInput(nextRawVoicingValue);
    SetDirty(false);
  }

  const char* mFontID = "Roboto-Regular";
  igraphics::IColor mLabelColor;
  igraphics::IColor mMetalPlate;
  igraphics::IColor mPlateInset;
  igraphics::IColor mMetalStroke;
  igraphics::IColor mSlotFill;
  igraphics::IColor mSlotHighlight;
  igraphics::IColor mSliderCap;
  igraphics::IColor mSliderEdge;
  igraphics::IColor mHandleGrip;
  igraphics::IColor mFocusRing;
  float mKeyboardFocus = 0.f;
};

class StrokedTitleControl final : public igraphics::IControl
{
public:
  StrokedTitleControl(const igraphics::IRECT& bounds,
                      const char* label,
                      const igraphics::IText& text)
    : igraphics::IControl(bounds)
    , mLabel(label)
  {
    mText = text;
    mIgnoreMouse = true;
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const float off = std::max(1.15f, mText.mSize * 0.060f);
    igraphics::IText strokeStyle = mText;
    strokeStyle.mFGColor = igraphics::IColor(240, 0, 0, 0);
    const int steps[3] = { -1, 0, 1 };

    for (int dx : steps) {
      for (int dy : steps) {
        if (dx == 0 && dy == 0)
          continue;

        g.DrawText(strokeStyle,
                   mLabel.Get(),
                   mRECT.GetTranslated(dx * off, dy * off));
      }
    }

    igraphics::IText fillStyle = mText;
    fillStyle.mFGColor = igraphics::IColor(255, 255, 255, 255);
    g.DrawText(fillStyle, mLabel.Get(), mRECT);
  }

private:
  WDL_String mLabel;
};

class StrokedSVGControl final : public igraphics::IControl
{
public:
  StrokedSVGControl(const igraphics::IRECT& bounds, const igraphics::ISVG& svg)
    : igraphics::IControl(bounds)
    , mSVG(svg)
  {
    mIgnoreMouse = true;
  }

  void Draw(igraphics::IGraphics& g) override
  {
    if (!mSVG.IsValid())
      return;

    g.DrawSVG(mSVG, mRECT, &mBlend);
  }

private:
  igraphics::ISVG mSVG;
};



// ISVGKnobControl stores mSVG and mStartAngle/mEndAngle as PRIVATE with no
// public setters, and its Draw() uses its own private members.  We subclass,
// store our own SVG+angles copy, and override Draw() to use our values so
// the knob rotates 0°–270° (matching the SVG artwork) instead of -135°–135°.
class ConfiguredSVGKnobControl final : public ISVGKnobControl
{
public:
  ConfiguredSVGKnobControl(const igraphics::IRECT& bounds, const igraphics::ISVG& svg, int paramIdx = kNoParameter)
    : ISVGKnobControl(bounds, svg, paramIdx)
    , mSVG(svg)
  {
  }

  void SetAngles(float start, float end) {
    mStartAngle = start;
    mEndAngle = end;
  }

  void Draw(igraphics::IGraphics& g) override
  {
    g.DrawRotatedSVG(mSVG, mRECT.MW(), mRECT.MH(), mRECT.W(), mRECT.H(),
                     mStartAngle + GetValue() * (mEndAngle - mStartAngle), &mBlend);
  }

private:
  igraphics::ISVG mSVG;
  float mStartAngle = 0.f;
  float mEndAngle = 270.f;
};

enum ECtrlTags {
  // Use a high tag range to avoid collisions with host/framework controls.
  kTagBackground = 10000,
  kTagUnderlay,
  kTagTitle,
  kTagLogo,
  kTagGrungeKnob,
  kTagButtKnob,
  kTagFaceKnob,
  kTagLoudKnob,
  kTagGrungeLabel,
  kTagButtLabel,
  kTagFaceLabel,
  kTagLoudLabel,
  kTagGrungeValue,
  kTagButtValue,
  kTagFaceValue,
  kTagLoudValue,
  kTagKnobStripe,
  kTagBypassToggle,
  kTagRawToggle
};

enum class LayoutVariant {
  Compact,
  Standard,
  Spacious
};

struct LayoutRects {
  LayoutVariant variant = LayoutVariant::Standard;
  igraphics::IRECT underlay;
  igraphics::IRECT background;
  igraphics::IRECT knobStripe;
  igraphics::IRECT title;
  igraphics::IRECT logo;
  float knobStripeCornerRadius = 0.f;
  float titleFontSize = 0.f;
  float knobLabelFontSize = 0.f;
  float knobValueFontSize = 0.f;

  igraphics::IRECT grungeKnob;
  igraphics::IRECT buttKnob;
  igraphics::IRECT faceKnob;
  igraphics::IRECT loudKnob;

  igraphics::IRECT grungeLabel;
  igraphics::IRECT buttLabel;
  igraphics::IRECT faceLabel;
  igraphics::IRECT loudLabel;

  igraphics::IRECT grungeValue;
  igraphics::IRECT buttValue;
  igraphics::IRECT faceValue;
  igraphics::IRECT loudValue;

  igraphics::IRECT bypassToggle;
  igraphics::IRECT rawToggle;
};

struct FontLoadState {
  const char* mainFontTag = nullptr;
  const char* secondaryFontTag = nullptr;
  const char* tertiaryFontTag = nullptr;
};

// Adaptive host-size variants keep layout usable in cramped windows while
// preserving proportions in spacious hosts.
LayoutVariant ResolveLayoutVariant(const igraphics::IRECT& uiBounds, float uiScale)
{
  const bool cramped = (uiScale < 0.88f) || (uiBounds.W() < 380.f) || (uiBounds.H() < 640.f);
  const bool spacious = (uiScale > 1.20f) && (uiBounds.W() > 520.f) && (uiBounds.H() > 860.f);

  if (cramped)
    return LayoutVariant::Compact;
  if (spacious)
    return LayoutVariant::Spacious;

  return LayoutVariant::Standard;
}

void ClampRectToBounds(igraphics::IRECT& rect, const igraphics::IRECT& bounds)
{
  const float rectW = std::min(rect.W(), bounds.W());
  const float rectH = std::min(rect.H(), bounds.H());
  const float left = std::clamp(rect.L, bounds.L, bounds.R - rectW);
  const float top = std::clamp(rect.T, bounds.T, bounds.B - rectH);

  rect = igraphics::IRECT(left, top, left + rectW, top + rectH);
}

void EnsureMinInteractiveTarget(igraphics::IRECT& rect,
                                const igraphics::IRECT& bounds,
                                float minSize)
{
  const float targetW = std::min(std::max(rect.W(), minSize), bounds.W());
  const float targetH = std::min(std::max(rect.H(), minSize), bounds.H());
  const float halfW = targetW * 0.5f;
  const float halfH = targetH * 0.5f;
  const float centerX = std::clamp((rect.L + rect.R) * 0.5f, bounds.L + halfW, bounds.R - halfW);
  const float centerY = std::clamp((rect.T + rect.B) * 0.5f, bounds.T + halfH, bounds.B - halfH);

  rect = igraphics::IRECT(centerX - halfW,
                          centerY - halfH,
                          centerX + halfW,
                          centerY + halfH);
}

void ValidateAccessibilityLayout(LayoutRects& r,
                                 const igraphics::IRECT& uiBounds,
                                 float minInteractiveTargetPx)
{
  EnsureMinInteractiveTarget(r.grungeKnob, uiBounds, minInteractiveTargetPx);
  EnsureMinInteractiveTarget(r.buttKnob, uiBounds, minInteractiveTargetPx);
  EnsureMinInteractiveTarget(r.faceKnob, uiBounds, minInteractiveTargetPx);
  EnsureMinInteractiveTarget(r.loudKnob, uiBounds, minInteractiveTargetPx);

  EnsureMinInteractiveTarget(r.bypassToggle, uiBounds, minInteractiveTargetPx);
  EnsureMinInteractiveTarget(r.rawToggle, uiBounds, minInteractiveTargetPx);

  ClampRectToBounds(r.title, uiBounds);
  ClampRectToBounds(r.logo, uiBounds);

  ClampRectToBounds(r.grungeLabel, uiBounds);
  ClampRectToBounds(r.buttLabel, uiBounds);
  ClampRectToBounds(r.faceLabel, uiBounds);
  ClampRectToBounds(r.loudLabel, uiBounds);
  ClampRectToBounds(r.grungeValue, uiBounds);
  ClampRectToBounds(r.buttValue, uiBounds);
  ClampRectToBounds(r.faceValue, uiBounds);
  ClampRectToBounds(r.loudValue, uiBounds);
}

LayoutRects MakeLayout(const igraphics::IRECT& uiBounds, const igraphics::IRECT& backgroundBounds, LayoutVariant variant = LayoutVariant::Standard)
{
  LayoutRects r;
  r.underlay = uiBounds;
  r.background = backgroundBounds;

  r.variant = variant;

  // Spacing Scale: replace fixed pixel gaps with proportional values based on knobSize.
  float knobSize = 90.f;
  if (variant == LayoutVariant::Compact) knobSize = 74.f;
  else if (variant == LayoutVariant::Spacious) knobSize = 106.f;
  const float labelGap = knobSize * 0.08f;
  const float valueGap = knobSize * 0.08f;
  const float labelH = knobSize * 0.22f;
  const float valueH = knobSize * 0.34f;
  r.knobLabelFontSize = knobSize * 0.19f;
  r.knobValueFontSize = knobSize * 0.27f;

  auto knobRectFromNorm = [&](size_t idx) {
    const float cx = backgroundBounds.L + (backgroundBounds.W() * kKnobCentersXNorm[idx]);
    const float cy = backgroundBounds.T + (backgroundBounds.H() * kKnobCenterYNorm);
    return igraphics::IRECT(cx - (knobSize * 0.5f), cy - (knobSize * 0.5f), cx + (knobSize * 0.5f), cy + (knobSize * 0.5f));
  };

  r.grungeKnob = knobRectFromNorm(0);
  r.buttKnob = knobRectFromNorm(1);
  r.faceKnob = knobRectFromNorm(2);
  r.loudKnob = knobRectFromNorm(3);

  const float labelW = knobSize * 1.18f;
  const float valueW = knobSize * 0.96f;

  // Labels are placed BELOW the knob, value text above the knob.
  auto makeLabel = [&](const igraphics::IRECT& knob) {
    const float top = knob.B + labelGap;
    return igraphics::IRECT(knob.MW() - (labelW * 0.5f), top, knob.MW() + (labelW * 0.5f), top + labelH);
  };

  auto makeValue = [&](const igraphics::IRECT& knob) {
    const float bottom = knob.T - valueGap;
    return igraphics::IRECT(knob.MW() - (valueW * 0.5f), bottom - valueH, knob.MW() + (valueW * 0.5f), bottom);
  };

  r.grungeLabel = makeLabel(r.grungeKnob);
  r.buttLabel = makeLabel(r.buttKnob);
  r.faceLabel = makeLabel(r.faceKnob);
  r.loudLabel = makeLabel(r.loudKnob);

  r.grungeValue = makeValue(r.grungeKnob);
  r.buttValue = makeValue(r.buttKnob);
  r.faceValue = makeValue(r.faceKnob);
  r.loudValue = makeValue(r.loudKnob);

  const float stripeVPadTop = knobSize * 0.09f;
  const float stripeVPadBottom = knobSize * 0.13f;
  const float stripeHInset = backgroundBounds.W() * 0.045f; // Increased for breathing room
  const float stripeT = std::min({r.grungeValue.T, r.buttValue.T, r.faceValue.T, r.loudValue.T}) - stripeVPadTop;
  const float stripeB = std::max({r.grungeLabel.B, r.buttLabel.B, r.faceLabel.B, r.loudLabel.B}) + stripeVPadBottom;
  r.knobStripeCornerRadius = knobSize * 0.09f;
  r.knobStripe = igraphics::IRECT(backgroundBounds.L + stripeHInset,
                                  stripeT,
                                  backgroundBounds.R - stripeHInset,
                                  stripeB);

  r.titleFontSize = (variant == LayoutVariant::Compact) ? 52.f : (variant == LayoutVariant::Spacious) ? 84.f : 68.f;
  const float titleH = r.titleFontSize * 1.16f;
  const float ledStripY = backgroundBounds.T + backgroundBounds.H() * kLedYNorm;

  const float rawToggleW = knobSize * 1.29f;
  const float rawToggleH = knobSize * 0.51f;
  const float rawCenterX = backgroundBounds.L + (backgroundBounds.W() * 0.17f);
  const float rawCenterY = ledStripY - 4.f;
  r.rawToggle = igraphics::IRECT(rawCenterX - rawToggleW * 0.5f,
                                 rawCenterY - rawToggleH * 0.5f,
                                 rawCenterX + rawToggleW * 0.5f,
                                 rawCenterY + rawToggleH * 0.5f);

  const float titleBandTop = r.knobStripe.B + knobSize * 0.18f;
  const float titleBandBottom = r.rawToggle.T - knobSize * 0.09f;
  const float titleMinY = titleBandTop + titleH * 0.5f;
  const float titleMaxY = std::max(titleMinY, titleBandBottom - titleH * 0.5f);
  const float titleY = std::clamp((titleBandTop + titleBandBottom) * 0.5f,
                                  titleMinY,
                                  titleMaxY);

  const float titleHalfWidth = backgroundBounds.W() * 0.50f;
  const float titleCenterX = backgroundBounds.MW();
  r.title = igraphics::IRECT(titleCenterX - titleHalfWidth,
                             titleY - (titleH * 0.5f),
                             titleCenterX + titleHalfWidth,
                             titleY + (titleH * 0.5f));

  const float logoH = std::clamp(rawToggleH * 0.95f, knobSize * 0.43f, knobSize * 0.53f);
  const float logoW = logoH * kClophLogoAspect;
  const float rawToggleLeftInset = rawCenterX - (rawToggleW * 0.5f) - backgroundBounds.L;
  const float logoRight = backgroundBounds.R - rawToggleLeftInset;
  const float logoCenterX = logoRight - (logoW * 0.5f);
  const float logoCenterY = rawCenterY + 2.f; // Sub-pixel nudge for visual baseline alignment
  r.logo = igraphics::IRECT(logoCenterX - (logoW * 0.5f),
                            logoCenterY - logoH * 0.5f,
                            logoCenterX + (logoW * 0.5f),
                            logoCenterY + logoH * 0.5f);

  // The stomp switch should map to the pedal tread region, not a small floating button.
  r.bypassToggle = igraphics::IRECT(backgroundBounds.L + (backgroundBounds.W() * kStompLeftNorm),
                                    backgroundBounds.T + (backgroundBounds.H() * kStompTopNorm),
                                    backgroundBounds.L + (backgroundBounds.W() * kStompRightNorm),
                                    backgroundBounds.T + (backgroundBounds.H() * kStompBottomNorm));

  ValidateAccessibilityLayout(r, uiBounds, kMinInteractiveTargetPx);

  return r;
}

igraphics::IRECT EffectiveLayoutBounds(const igraphics::IRECT& bounds)
{
  // Some hosts may issue an early layout callback with tiny bounds.
  // Keep a sensible layout rect so controls are always visible.
  if (bounds.W() < 64.f || bounds.H() < 64.f) {
    return igraphics::IRECT(0.f, 0.f, static_cast<float>(PLUG_WIDTH), static_cast<float>(PLUG_HEIGHT));
  }

  return bounds;
}

igraphics::IRECT ContainRectCentered(const igraphics::IRECT& bounds, float srcW, float srcH)
{
  if (srcW <= 0.f || srcH <= 0.f || bounds.W() <= 0.f || bounds.H() <= 0.f) {
    return bounds;
  }

  const float scale = std::min(bounds.W() / srcW, bounds.H() / srcH);
  const float drawW = srcW * scale;
  const float drawH = srcH * scale;
  const float left = bounds.L + ((bounds.W() - drawW) * 0.5f);
  const float top = bounds.T + ((bounds.H() - drawH) * 0.5f);

  // Show the full background artwork without crop, centered in the available UI bounds.
  return igraphics::IRECT(left, top, left + drawW, top + drawH);
}

void SetBoundsIfPresent(igraphics::IGraphics& g, int tag, const igraphics::IRECT& bounds)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    pControl->SetTargetAndDrawRECTs(bounds);
  }
}

void Relayout(igraphics::IGraphics& g, const LayoutRects& layout)
{
  SetBoundsIfPresent(g, kTagUnderlay, layout.underlay);
  SetBoundsIfPresent(g, kTagBackground, layout.background);
  SetBoundsIfPresent(g, kTagKnobStripe, layout.knobStripe);
  SetBoundsIfPresent(g, kTagTitle, layout.title);
  SetBoundsIfPresent(g, kTagLogo, layout.logo);

  SetBoundsIfPresent(g, kTagGrungeKnob, layout.grungeKnob);
  SetBoundsIfPresent(g, kTagButtKnob, layout.buttKnob);
  SetBoundsIfPresent(g, kTagFaceKnob, layout.faceKnob);
  SetBoundsIfPresent(g, kTagLoudKnob, layout.loudKnob);

  SetBoundsIfPresent(g, kTagGrungeLabel, layout.grungeLabel);
  SetBoundsIfPresent(g, kTagButtLabel, layout.buttLabel);
  SetBoundsIfPresent(g, kTagFaceLabel, layout.faceLabel);
  SetBoundsIfPresent(g, kTagLoudLabel, layout.loudLabel);

  SetBoundsIfPresent(g, kTagGrungeValue, layout.grungeValue);
  SetBoundsIfPresent(g, kTagButtValue, layout.buttValue);
  SetBoundsIfPresent(g, kTagFaceValue, layout.faceValue);
  SetBoundsIfPresent(g, kTagLoudValue, layout.loudValue);

  SetBoundsIfPresent(g, kTagBypassToggle, layout.bypassToggle);
  SetBoundsIfPresent(g, kTagRawToggle, layout.rawToggle);
}

FontLoadState LoadFonts(igraphics::IGraphics& g, const Assets& assets)
{
  FontLoadState state;

  if (g.LoadFont(kMainFontTag, assets.mainFont)) {
    state.mainFontTag = kMainFontTag;
  }

  if (g.LoadFont(kSecondaryFontTag, assets.secondaryFont)) {
    state.secondaryFontTag = kSecondaryFontTag;
  }

  if (g.LoadFont(kTertiaryFontTag, assets.tertiaryFont)) {
    state.tertiaryFontTag = kTertiaryFontTag;
  }

  return state;
}

ThemePalette BuildThemePalette()
{
  ThemePalette palette;

  palette.fallbackBackground = kTokenFallbackBackground;
  palette.underlayBackground = palette.fallbackBackground.WithContrast(kTokenUnderlayContrast);
  palette.titleText = kTokenTitleText;
  palette.knobLabelText = kTokenKnobLabelText;
  palette.knobValueText = kTokenKnobValueText;
  palette.toggleLabelText = kTokenToggleLabelText;
  palette.toggleValueText = kTokenToggleValueText;
  palette.vectorBG = kTokenVectorBG;
  palette.vectorFG = kTokenVectorFG;
  palette.vectorPR = kTokenVectorPR;
  palette.vectorFR = kTokenVectorFR;
  palette.stripeFill = kTokenStripeFill;
  palette.stripeHighlight = kTokenStripeHighlight;
  palette.ledBezelOuter = kTokenLedBezelOuter;
  palette.ledBezelInner = kTokenLedBezelInner;
  palette.ledLensHighlight = kTokenLedLensHighlight;
  palette.rawHardwarePlateOuter = kTokenRawHardwarePlateOuter;
  palette.rawHardwarePlateInner = kTokenRawHardwarePlateInner;
  palette.rawHardwarePlateEdge = kTokenRawHardwarePlateEdge;
  palette.rawHardwareSlotFill = kTokenRawHardwareSlotFill;
  palette.rawHardwareSlotHighlight = kTokenRawHardwareSlotHighlight;
  palette.rawHardwareHandleFill = kTokenRawHardwareHandleFill;
  palette.rawHardwareHandleEdge = kTokenRawHardwareHandleEdge;
  palette.rawHardwareHandleGrip = kTokenRawHardwareHandleGrip;
  palette.focusRing = kTokenFocusRing;
  palette.ripple = kTokenRipple;

  return palette;
}

igraphics::IVStyle BuildVectorKnobStyle(const ThemePalette& palette, const char* textFontID)
{
  return igraphics::DEFAULT_STYLE
      .WithColor(igraphics::kBG, palette.vectorBG)
      .WithColor(igraphics::kFG, palette.vectorFG)
      .WithColor(igraphics::kPR, palette.vectorPR)
      .WithColor(igraphics::kFR, palette.vectorFR)
      .WithShowLabel(false)
      .WithShowValue(false)
      .WithRoundness(1.f)
      .WithFrameThickness(1.4f)
      .WithShadowOffset(0.f)
      .WithDrawShadows(false)
      .WithLabelText(igraphics::IText(14.f,
                                      palette.toggleLabelText,
                                      textFontID,
                                      igraphics::EAlign::Center,
                                      igraphics::EVAlign::Middle))
      .WithValueText(igraphics::IText(14.f,
                                      palette.toggleValueText,
                                      textFontID,
                                      igraphics::EAlign::Center,
                                      igraphics::EVAlign::Middle));
}

void SetTextStyleIfPresent(igraphics::IGraphics& g,
                           int tag,
                           const igraphics::IColor& color,
                           float size)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    pControl->SetText(pControl->GetText().WithFGColor(color).WithSize(size));
    pControl->SetDirty(false);
  }
}

void SetVectorStyleIfPresent(igraphics::IGraphics& g, int tag, const igraphics::IVStyle& style)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pVectorControl = dynamic_cast<igraphics::IVectorBase*>(pControl)) {
      pVectorControl->SetStyle(style);
      pControl->SetDirty(false);
    }
  }
}

void SetPanelColorIfPresent(igraphics::IGraphics& g, int tag, const igraphics::IColor& color)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pPanel = dynamic_cast<igraphics::IPanelControl*>(pControl)) {
      pPanel->SetPattern(igraphics::IPattern(color));
      pControl->SetDirty(false);
    }
  }
}

void SetStompStyleIfPresent(igraphics::IGraphics& g,
                            int tag,
                            const ThemePalette& palette,
                            bool reduceMotion)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pStompControl = dynamic_cast<AnimatedStompBypassControl*>(pControl)) {
      pStompControl->SetPalette(palette);
      pStompControl->SetReducedMotion(reduceMotion);
      pControl->SetDirty(false);
    }
  }
}

void SetRawToggleStyleIfPresent(igraphics::IGraphics& g,
                                int tag,
                                const ThemePalette& palette)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pRawToggle = dynamic_cast<RawModeToggleControl*>(pControl)) {
      pRawToggle->SetPalette(palette);
      pControl->SetDirty(false);
    }
  }
}

const char* ResolveFontIDFromTag(igraphics::IGraphics& g, int tag, const char* fallbackFont)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    const char* fontID = pControl->GetText().mFont;

    if (fontID && fontID[0] != '\0') {
      return fontID;
    }
  }

  return fallbackFont;
}

void ApplyRuntimeStyle(igraphics::IGraphics& g)
{
  const ThemePalette palette = BuildThemePalette();
  const char* secondaryFontID = ResolveFontIDFromTag(g, kTagGrungeLabel, "Roboto-Regular");

  const igraphics::IRECT uiBounds = EffectiveLayoutBounds(g.GetBounds());
  igraphics::IRECT backgroundBounds = uiBounds;

  if (auto* pBackground = g.GetControlWithTag(kTagBackground)) {
    backgroundBounds = pBackground->GetRECT();
  }

  const LayoutRects layout = MakeLayout(uiBounds, backgroundBounds);
  const igraphics::IVStyle vectorKnobStyle = BuildVectorKnobStyle(palette, secondaryFontID);

  SetPanelColorIfPresent(g, kTagUnderlay, palette.underlayBackground);
  SetStompStyleIfPresent(g, kTagBypassToggle, palette, gRuntimeBehavior.reduceMotion);
  SetRawToggleStyleIfPresent(g, kTagRawToggle, palette);

  SetVectorStyleIfPresent(g, kTagGrungeKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagButtKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagFaceKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagLoudKnob, vectorKnobStyle);

  SetTextStyleIfPresent(g, kTagTitle, palette.titleText, layout.titleFontSize);
  SetTextStyleIfPresent(g, kTagGrungeLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagButtLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagFaceLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagLoudLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagGrungeValue, palette.knobValueText, layout.knobValueFontSize);
  SetTextStyleIfPresent(g, kTagButtValue, palette.knobValueText, layout.knobValueFontSize);
  SetTextStyleIfPresent(g, kTagFaceValue, palette.knobValueText, layout.knobValueFontSize);
  SetTextStyleIfPresent(g, kTagLoudValue, palette.knobValueText, layout.knobValueFontSize);
}

void AttachKnobWithText(igraphics::IGraphics& g,
                        const igraphics::ISVG& knobSVG,
                        int paramIdx,
                        const char* label,
                        const char* tooltip,
                        const char* labelFontID,
                        const char* valueFontID,
                        const ThemePalette& palette,
                        const igraphics::IRECT& knobRect,
                        const igraphics::IRECT& labelRect,
                        const igraphics::IRECT& valueRect,
                        float labelFontSize,
                        float valueFontSize,
                        int knobTag,
                        int labelTag,
                         int valueTag)
{
  using namespace igraphics;

  auto* pKnob = new ConfiguredSVGKnobControl(knobRect, knobSVG, paramIdx);
  pKnob->SetAngles(0.f, 300.f);
  pKnob->SetTooltip((tooltip && tooltip[0] != '\0') ? tooltip : label);
  g.AttachControl(pKnob, knobTag);

  const IText labelText(labelFontSize,
                        palette.knobLabelText,
                        labelFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  const IText valueText(valueFontSize,
                        palette.knobValueText,
                        valueFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  g.AttachControl(new ITextControl(labelRect, label, labelText, COLOR_TRANSPARENT), labelTag);
  g.AttachControl(new ICaptionControl(valueRect, paramIdx, valueText, COLOR_TRANSPARENT, false), valueTag);
}

void AttachVectorKnobWithText(igraphics::IGraphics& g,
                              int paramIdx,
                              const char* label,
                              const char* tooltip,
                              const char* labelFontID,
                              const char* valueFontID,
                              const ThemePalette& palette,
                              const igraphics::IRECT& knobRect,
                              const igraphics::IRECT& labelRect,
                              const igraphics::IRECT& valueRect,
                              float labelFontSize,
                              float valueFontSize,
                              int knobTag,
                              int labelTag,
                               int valueTag)
{
  using namespace igraphics;

  auto* pKnob = new IVKnobControl(knobRect, paramIdx, label,
                                   BuildVectorKnobStyle(palette, labelFontID));
  pKnob->SetTooltip((tooltip && tooltip[0] != '\0') ? tooltip : label);
  g.AttachControl(pKnob, knobTag);

  const IText labelText(labelFontSize,
                        palette.knobLabelText,
                        labelFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  const IText valueText(valueFontSize,
                        palette.knobValueText,
                        valueFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  g.AttachControl(new ITextControl(labelRect, label, labelText, COLOR_TRANSPARENT), labelTag);
  g.AttachControl(new ICaptionControl(valueRect, paramIdx, valueText, COLOR_TRANSPARENT, false), valueTag);
}

}  // namespace

void BuildOrRelayout(igraphics::IGraphics* pGraphics,
                     const Params& params,
                     const Assets& assets)
{
  if (!pGraphics) {
    return;
  }

  const igraphics::IRECT uiBounds = EffectiveLayoutBounds(pGraphics->GetBounds());
  const ThemePalette palette = BuildThemePalette();
  pGraphics->SetLayoutOnResize(true);

  const bool controlsAlreadyAttached =
      (pGraphics->GetControlWithTag(kTagGrungeKnob) != nullptr) &&
      (pGraphics->GetControlWithTag(kTagBypassToggle) != nullptr);

  if (controlsAlreadyAttached) {
    igraphics::IRECT backgroundBounds = uiBounds;

    if (auto* pBackground = pGraphics->GetControlWithTag(kTagBackground)) {
      const igraphics::IRECT previousBackground = pBackground->GetRECT();

      if (previousBackground.W() > 0.f && previousBackground.H() > 0.f) {
        // Preserve the previous aspect ratio so relayout does not need to reparse SVG resources.
        backgroundBounds = ContainRectCentered(uiBounds, previousBackground.W(), previousBackground.H());
      }
    }

    const LayoutRects relayout = MakeLayout(uiBounds, backgroundBounds);
    Relayout(*pGraphics, relayout);

    if (pGraphics->GetControlWithTag(kTagRawToggle) == nullptr) {
      const char* rawFontID = ResolveFontIDFromTag(*pGraphics, kTagGrungeValue, "Roboto-Regular");
      auto* pRawToggle = new RawModeToggleControl(relayout.rawToggle,
                                                   params.rawVoicing,
                                                   palette,
                                                   rawFontID);
      pRawToggle->SetTooltip(kTooltipRaw);
      pGraphics->AttachControl(pRawToggle, kTagRawToggle);
    }

    if (pGraphics->GetControlWithTag(kTagLogo) == nullptr) {
      const igraphics::ISVG logoSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr)
                                                              : pGraphics->LoadSVG(assets.logoSVG);
      if (logoSVG.IsValid() && logoSVG.W() > 0.f && logoSVG.H() > 0.f) {
        pGraphics->AttachControl(new StrokedSVGControl(relayout.logo, logoSVG), kTagLogo);
      }
    }

    ApplyRuntimeStyle(*pGraphics);
    return;
  }

  // Roboto-Regular MUST be loaded first — it is DEFAULT_FONT and every iPlug2
  // vector control (IVKnobControl, IVToggleControl, ITextControl, etc.) will
  // crash on first draw if it cannot find this font handle in NanoVG.
  pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

  const FontLoadState fonts = kForceSafeVectorUIMode ? FontLoadState{} : LoadFonts(*pGraphics, assets);

  const char* titleFontID = (fonts.secondaryFontTag != nullptr) ? fonts.secondaryFontTag
                            : (fonts.mainFontTag != nullptr) ? fonts.mainFontTag
                            : "Roboto-Regular";
  const char* labelFontID = (fonts.mainFontTag != nullptr) ? fonts.mainFontTag
                            : (fonts.secondaryFontTag != nullptr) ? fonts.secondaryFontTag
                            : "Roboto-Regular";
  // BoxPot has full symbol coverage (% . -) needed for value display.
  const char* valueFontID = (fonts.tertiaryFontTag != nullptr) ? fonts.tertiaryFontTag
                            : (fonts.secondaryFontTag != nullptr) ? fonts.secondaryFontTag
                            : (fonts.mainFontTag != nullptr) ? fonts.mainFontTag
                            : "Roboto-Regular";

  const igraphics::ISVG backgroundSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr) : pGraphics->LoadSVG(assets.backgroundSVG);
  igraphics::ISVG knobSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr) : pGraphics->LoadSVG(assets.knobSVG);
  const igraphics::ISVG logoSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr) : pGraphics->LoadSVG(assets.logoSVG);

  if (!kForceSafeVectorUIMode && (!knobSVG.IsValid() || knobSVG.W() <= 0.f || knobSVG.H() <= 0.f)) {
    knobSVG = pGraphics->LoadSVG("knob-cropped.svg");
  }

  const bool hasBackgroundSVG = (!kForceSafeVectorUIMode) && backgroundSVG.IsValid() && backgroundSVG.W() > 0.f && backgroundSVG.H() > 0.f;
  const bool hasKnobSVG = (!kForceSafeVectorUIMode) && kUseSVGKnobs && knobSVG.IsValid() && knobSVG.W() > 0.f && knobSVG.H() > 0.f;
  const bool hasLogoSVG = (!kForceSafeVectorUIMode) && logoSVG.IsValid() && logoSVG.W() > 0.f && logoSVG.H() > 0.f;

  const igraphics::IRECT backgroundBounds = hasBackgroundSVG
      ? ContainRectCentered(uiBounds, backgroundSVG.W(), backgroundSVG.H())
      : uiBounds;

LayoutRects layout = MakeLayout(uiBounds, backgroundBounds, ResolveLayoutVariant(uiBounds, 1.0f));

  pGraphics->AttachControl(new igraphics::IPanelControl(layout.underlay,
                                                        palette.underlayBackground,
                                                        false),
                           kTagUnderlay);

  if (hasBackgroundSVG) {
    pGraphics->AttachControl(new StrokedSVGControl(layout.background, backgroundSVG), kTagBackground);
  }
  else {
    pGraphics->AttachControl(new igraphics::IPanelControl(layout.background,
                                                          palette.fallbackBackground,
                                                          false),
                             kTagBackground);
  }

  // Rubber stripe sits above background, below knobs.
  {
    pGraphics->AttachControl(new KnobStripeControl(layout.knobStripe,
                                                   layout.knobStripeCornerRadius,
                                                   palette.stripeFill,
                                                   palette.stripeHighlight),
                             kTagKnobStripe);
  }

  const igraphics::IText titleText(layout.titleFontSize,
                                   palette.titleText,
                                   titleFontID,
                                   igraphics::EAlign::Center,
                                   igraphics::EVAlign::Middle);

  pGraphics->AttachControl(new StrokedTitleControl(layout.title,
                                                    kTitleText,
                                                    titleText),
                           kTagTitle);

  if (hasLogoSVG) {
    pGraphics->AttachControl(new StrokedSVGControl(layout.logo, logoSVG), kTagLogo);
  }

  if (hasKnobSVG) {
    AttachKnobWithText(*pGraphics,
                       knobSVG,
                       params.grunge,
                       kLabelGrunge,
                       kTooltipGrunge,
                       labelFontID,
                       valueFontID,
                       palette,
                       layout.grungeKnob,
                       layout.grungeLabel,
                       layout.grungeValue,
                       layout.knobLabelFontSize,
                       layout.knobValueFontSize,
                       kTagGrungeKnob,
                       kTagGrungeLabel,
                        kTagGrungeValue);

    AttachKnobWithText(*pGraphics,
                       knobSVG,
                       params.butt,
                       kLabelButt,
                       kTooltipButt,
                       labelFontID,
                       valueFontID,
                       palette,
                       layout.buttKnob,
                       layout.buttLabel,
                       layout.buttValue,
                       layout.knobLabelFontSize,
                       layout.knobValueFontSize,
                       kTagButtKnob,
                       kTagButtLabel,
                        kTagButtValue);

    AttachKnobWithText(*pGraphics,
                       knobSVG,
                       params.face,
                       kLabelFace,
                       kTooltipFace,
                       labelFontID,
                       valueFontID,
                       palette,
                       layout.faceKnob,
                       layout.faceLabel,
                       layout.faceValue,
                       layout.knobLabelFontSize,
                       layout.knobValueFontSize,
                       kTagFaceKnob,
                       kTagFaceLabel,
                        kTagFaceValue);

    AttachKnobWithText(*pGraphics,
                       knobSVG,
                       params.loud,
                       kLabelLoud,
                       kTooltipLoud,
                       labelFontID,
                       valueFontID,
                       palette,
                       layout.loudKnob,
                       layout.loudLabel,
                       layout.loudValue,
                       layout.knobLabelFontSize,
                       layout.knobValueFontSize,
                       kTagLoudKnob,
                       kTagLoudLabel,
                        kTagLoudValue);
  }
  else {
    AttachVectorKnobWithText(*pGraphics,
                             params.grunge,
                             kLabelGrunge,
                             kTooltipGrunge,
                             labelFontID,
                             valueFontID,
                             palette,
                             layout.grungeKnob,
                             layout.grungeLabel,
                             layout.grungeValue,
                             layout.knobLabelFontSize,
                             layout.knobValueFontSize,
                             kTagGrungeKnob,
                             kTagGrungeLabel,
                              kTagGrungeValue);

    AttachVectorKnobWithText(*pGraphics,
                             params.butt,
                             kLabelButt,
                             kTooltipButt,
                             labelFontID,
                             valueFontID,
                             palette,
                             layout.buttKnob,
                             layout.buttLabel,
                             layout.buttValue,
                             layout.knobLabelFontSize,
                             layout.knobValueFontSize,
                             kTagButtKnob,
                             kTagButtLabel,
                              kTagButtValue);

    AttachVectorKnobWithText(*pGraphics,
                             params.face,
                             kLabelFace,
                             kTooltipFace,
                             labelFontID,
                             valueFontID,
                             palette,
                             layout.faceKnob,
                             layout.faceLabel,
                             layout.faceValue,
                             layout.knobLabelFontSize,
                             layout.knobValueFontSize,
                             kTagFaceKnob,
                             kTagFaceLabel,
                              kTagFaceValue);

    AttachVectorKnobWithText(*pGraphics,
                             params.loud,
                             kLabelLoud,
                             kTooltipLoud,
                             labelFontID,
                             valueFontID,
                             palette,
                             layout.loudKnob,
                             layout.loudLabel,
                             layout.loudValue,
                             layout.knobLabelFontSize,
                             layout.knobValueFontSize,
                             kTagLoudKnob,
                             kTagLoudLabel,
                              kTagLoudValue);
  }

  const igraphics::ISVG stompSoleSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr)
      : pGraphics->LoadSVG(assets.stompSoleSVG);
  auto* pBypassToggle = new AnimatedStompBypassControl(layout.bypassToggle,
                                                        params.bypass,
                                                        palette,
                                                        gRuntimeBehavior.reduceMotion,
                                                        stompSoleSVG,
                                                        labelFontID);
  pBypassToggle->SetTooltip(kTooltipStomp);
  pGraphics->AttachControl(pBypassToggle, kTagBypassToggle);

  auto* pRawToggle = new RawModeToggleControl(layout.rawToggle,
                                               params.rawVoicing,
                                               palette,
                                               labelFontID);
  pRawToggle->SetTooltip(kTooltipRaw);
  pGraphics->AttachControl(pRawToggle, kTagRawToggle);

  ApplyRuntimeStyle(*pGraphics);
}

bool HandleGlobalKey(igraphics::IGraphics* pGraphics,
                     const iplug::IKeyPress& key,
                     bool isUp)
{
  if (!pGraphics || isUp)
    return false;

  if (key.C || key.A)
    return false;

  // Do not hijack typing while a text-entry prompt is active.
  if (pGraphics->GetControlInTextEntry() != nullptr)
    return false;

  if (key.VK == iplug::kVK_SPACE || key.VK == iplug::kVK_RETURN) {
    if (auto* pControl = pGraphics->GetControlWithTag(kTagBypassToggle)) {
      if (auto* pStomp = dynamic_cast<AnimatedStompBypassControl*>(pControl)) {
        if (!pStomp->IsHidden() && !pStomp->IsDisabled()) {
          pStomp->ToggleFromKeyboard();
          return true;
        }
      }
    }
  }

  if (key.VK == 'M') {
    gRuntimeBehavior.reduceMotion = !gRuntimeBehavior.reduceMotion;
    ApplyRuntimeStyle(*pGraphics);
    return true;
  }

  return false;
}

}  // namespace ui
}  // namespace grungr

