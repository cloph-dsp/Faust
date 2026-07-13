#include "GrungrIPlug2UI.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

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
static constexpr float kTitleLogoAspect = 2072.f / 512.f;

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
// LCD readout theme — black panel + red 7-segment style digits + subtle red glow.
static const igraphics::IColor kTokenLcdBackground(255, 8, 4, 4);
static const igraphics::IColor kTokenLcdBorder(255, 26, 8, 8);
static const igraphics::IColor kTokenLcdText(255, 255, 60, 36);
static const igraphics::IColor kTokenLcdGlow(110, 255, 40, 24);
static const igraphics::IColor kTokenLcdHighlight(120, 210, 150, 130);
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
static constexpr const char* kTooltipGrunge = "Drive (Grunge) (0–100): Adds distortion and texture. Higher = more grit and harmonic richness.";
static constexpr const char* kTooltipButt = "Body (Butt) (0–100): Adds low-end weight and thickness. Higher = deeper, punchier bass response.";
static constexpr const char* kTooltipFace = "Bite (Face) (0–100): Adds upper-mid cut and presence. Higher = more attack and clarity.";
static constexpr const char* kTooltipLoud = "Level (Loud) (0–100): Sets the pedal output volume. Match input level or boost for solo.";
static constexpr const char* kTooltipStomp = "Stomp: Turns the effect on/off. Click or Space. Press M to reduce motion.";
static constexpr const char* kTooltipRaw = "Voicing: RAW for open/gritty, MOD for tight/smooth, BASS for bass guitar voicing.";
static constexpr const char* kVoicingRawText = "RAW";
static constexpr const char* kVoicingModText = "MOD";
static constexpr const char* kVoicingBassText = "BASS";

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
  igraphics::IColor lcdBackground;
  igraphics::IColor lcdBorder;
  igraphics::IColor lcdText;
  igraphics::IColor lcdGlow;
  igraphics::IColor lcdHighlight;
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

class NinetiesPhotoOverlay final : public igraphics::IControl
{
public:
  NinetiesPhotoOverlay(const igraphics::IRECT& bounds, uint32_t seed)
  : IControl(bounds, -1)
  , mSeed(seed ? seed : 0x9E3779B1u)
  ,   mBlend7(igraphics::EBlend::Default, 0.07f)
  {
    mDblAsSingleClick = false;
    mIgnoreMouse = true;
    SetTooltip("90s photo overlay (Kodak Gold 200, late afternoon)");
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const float w = mRECT.W();
    const float h = mRECT.H();
    if (w <= 1.f || h <= 1.f) {
      return;
    }

    DrawWarmWash(g);
    DrawVignette(g);
    DrawGrain(g, w, h);
  }

private:
  static igraphics::IColor WithAlpha(const igraphics::IColor& c, int a)
  {
    return igraphics::IColor(a, c.R, c.G, c.B);
  }

  void DrawWarmWash(igraphics::IGraphics& g) const
  {
    const igraphics::IColor goldCast(255, 248, 198, 96);
    g.FillRect(goldCast, mRECT, &mBlend7);
  }

  void DrawVignette(igraphics::IGraphics& g) const
  {
    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    const float w = mRECT.W();
    const float h = mRECT.H();
    const float diag = std::sqrt(w * w + h * h);

    const igraphics::IColor dark(255, 0, 0, 0);
    g.FillCircle(dark, cx, cy, diag * 0.55f, &mBlend7);
  }

  void DrawGrain(igraphics::IGraphics& g, float w, float h) const
  {
    constexpr int kCellSize = 5;
    const int cols = static_cast<int>(w) / kCellSize + 1;
    const int rows = static_cast<int>(h) / kCellSize + 1;

    uint32_t state = mSeed;
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        const int brightness = 200 + static_cast<int>((state >> 24) & 0x1Fu);
        const int tint = static_cast<int>((state >> 16) & 0x0Fu) - 7;
        const igraphics::IColor grain(255, brightness, brightness - tint / 2, brightness - tint);
        const float x = mRECT.L + col * kCellSize;
        const float y = mRECT.T + row * kCellSize;
        g.FillRect(grain, igraphics::IRECT(x, y, x + kCellSize, y + kCellSize), &mBlend7);
      }
    }
  }

  uint32_t mSeed;
  igraphics::IBlend mBlend7;
};

class AnimatedStompBypassControl final : public igraphics::IControl
{
public:
  AnimatedStompBypassControl(const igraphics::IRECT& bounds,
                             int paramIdx,
                             const ThemePalette& palette,
                             bool reduceMotion,
                             const igraphics::ISVG& shoeSoleSVG,
                             const igraphics::ISVG& faceplateSVG,
                             const igraphics::IRECT& backgroundBounds,
                             const char* textFontID = "Roboto-Regular")
  : IControl(bounds, paramIdx)
  , mReduceMotion(reduceMotion)
  , mShoeSoleSVG(shoeSoleSVG)
  , mFaceplateSVG(faceplateSVG)
  , mBackgroundBounds(backgroundBounds)
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

    // Cave-in offset: the stomp plate sinks when pressed.
    const float caveOffset = mPressT * 3.0f + mVisualDown * 1.5f;

    // Cave-in: the stomp plate sinks when pressed. Draw the faceplate SVG shifted
    // down within the stomp control, clipped to its bounds. A dark fill at the top
    // creates the gap where the plate separated from the body.
    if (caveOffset > 0.1f && mFaceplateSVG.IsValid()) {
      g.StartLayer(this, mRECT);

      const igraphics::IRECT shiftedBG(mBackgroundBounds.L,
                                        mBackgroundBounds.T + caveOffset,
                                        mBackgroundBounds.R,
                                        mBackgroundBounds.B + caveOffset);
      g.DrawSVG(mFaceplateSVG, shiftedBG, &mBlend);

      g.FillRect(igraphics::IColor(255, 2, 2, 2),
                 igraphics::IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + caveOffset), &mBlend);

      igraphics::ILayerPtr caveLayer = g.EndLayer();
      g.DrawLayer(caveLayer, &mBlend);
    }

    // LED position in control-local coordinates (stays fixed on the chassis).
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

    // LED lens with breathing animation when idle (engaged but not pulsing)
    float ledAlpha = mVisualDown;
    if (!mReduceMotion && mEngagePulseT < 0.001f && mVisualDown > 0.01f) {
      // Breathing: 0.85 + 0.15 * sin(2π * 0.3 * t)
      static constexpr float kTwoPi = 6.28318530f;
      static constexpr float kBreathFreq = 0.3f;  // 0.3 Hz
      const float breathMod = 0.85f + 0.15f * std::sin(kTwoPi * kBreathFreq * mBreathingT);
      ledAlpha *= breathMod;
    }
    const igraphics::IColor ledColor = LerpColor(mLedOff, mLedOn, ledAlpha);
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
        const float cy = treadTop + treadH * 0.08f + soleH * 0.5f + caveOffset;

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

    // Accumulate time for breathing animation (assuming ~60fps)
    static constexpr float kFrameTime = 1.f / 60.f;
    mBreathingT += kFrameTime;
    if (mBreathingT > 1000.f)
      mBreathingT = 0.f;  // wrap to prevent float precision issues


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
  float mBreathingT = 0.f;  // idle animation time accumulator
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
  igraphics::ISVG mFaceplateSVG;
  igraphics::IRECT mBackgroundBounds;
  const char* mFontID = "Roboto-Regular";
};

// 3-brick toggle plate voicing selector: inline 3-cell click-target bar
// using existing rawHardwarePlate* materials. Active cell has deeper shadow
// + warm white LED indicator. Click anywhere on a cell to switch directly.
class VoicingSelectControl final : public igraphics::IControl
{
public:
  VoicingSelectControl(const igraphics::IRECT& bounds,
                       int voicingParamIdx,
                       const ThemePalette& palette,
                       const char* textFontID)
  : IControl(bounds, voicingParamIdx)
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
    mLedOn = igraphics::IColor(255, 255, 250, 240);  // warm white LED
    mFocusRing = palette.focusRing;
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const int mode = GetMode();
    const float cellGap = std::max(3.f, mRECT.W() * 0.04f);
    const float cellW = (mRECT.W() - 2.f * cellGap) / 3.f;
    const float cellH = mRECT.H();

    // Draw 3 cells side-by-side
    for (int i = 0; i < 3; ++i) {
      const float cellX = mRECT.L + cellGap + i * (cellW + cellGap);
      const igraphics::IRECT cellRect(cellX, mRECT.T, cellX + cellW, mRECT.T + cellH);
      const bool isActive = (i == mode);
      const bool isHovered = GetMouseIsOver() && mRECT.Intersects(cellRect);

      // Depression animation offset (1-2px on press)
      const float pressOffset = (mPressT > 0.001f && mPressedCell == i) ? (mPressT * 1.5f) : 0.f;
      const igraphics::IRECT drawRect = cellRect.GetTranslated(0.f, pressOffset);

      // Plate colors: active = deeper, inactive = raised
      const igraphics::IColor plateOuter = isActive
          ? mMetalPlate.WithContrast(-0.12f)  // darker when active
          : mMetalPlate;
      const igraphics::IColor plateInner = isActive
          ? mPlateInset.WithContrast(-0.08f)
          : mPlateInset;
      const igraphics::IColor strokeColor = mMetalStroke.WithOpacity(isActive ? 0.85f : 0.65f);

      const float plateCorner = std::max(2.f, cellH * 0.18f);

      // Outer plate (shadow for depth)
      g.FillRoundRect(plateOuter, drawRect, plateCorner, &mBlend);

      // Inner inset plate
      const float insetPad = std::max(1.f, cellH * 0.06f);
      const igraphics::IRECT insetRect(drawRect.L + insetPad,
                                        drawRect.T + insetPad,
                                        drawRect.R - insetPad,
                                        drawRect.B - insetPad);
      g.FillRoundRect(plateInner, insetRect, std::max(1.5f, plateCorner - insetPad), &mBlend);

      // Border stroke
      const float frameThickness = std::max(0.8f, mRECT.W() * 0.012f);
      g.DrawRoundRect(strokeColor, drawRect, plateCorner, &mBlend, frameThickness);

      // Draw label in center of cell
      const char* label = (i == 0) ? kVoicingRawText
                        : (i == 1) ? kVoicingModText
                        : kVoicingBassText;

      // Scale font to fit cell
      igraphics::IRECT measureRect = drawRect;
      igraphics::IText testText(16.f, mLabelColor.WithOpacity(0.90f), mFontID,
                                igraphics::EAlign::Center, igraphics::EVAlign::Middle);
      g.MeasureText(testText, label, measureRect);
      float labelW = measureRect.W();
      float fontSize = 16.f;
      if (labelW > drawRect.W() * 0.85f && labelW > 0.001f) {
        fontSize = std::clamp(16.f * (drawRect.W() * 0.85f / labelW), 10.f, 16.f);
      }

      igraphics::IText labelText(fontSize, mLabelColor.WithOpacity(0.90f), mFontID,
                                  igraphics::EAlign::Center, igraphics::EVAlign::Middle);
      g.DrawText(labelText, label, drawRect, &mBlend);

      // Active cell LED indicator at bottom-center
      if (isActive) {
        const float ledR = std::max(2.5f, cellW * 0.08f);
        const float ledX = drawRect.MW();
        const float ledY = drawRect.B - insetPad - ledR - 2.f;

        // LED glow
        const igraphics::IColor ledGlow = mLedOn.WithOpacity(0.35f);
        g.FillCircle(ledGlow, ledX, ledY, ledR * 2.0f, &mBlend);

        // LED lens
        g.FillCircle(mLedOn, ledX, ledY, ledR, &mBlend);

        // LED highlight
        const igraphics::IColor ledHi(200, 255, 255, 250);
        g.FillCircle(ledHi, ledX - ledR * 0.25f, ledY - ledR * 0.25f, ledR * 0.4f, &mBlend);
      }
    }

    // Focus ring around entire control
    const float hoverStrength = GetMouseIsOver() ? 0.35f : 0.f;
    const float focusStrength = std::clamp(hoverStrength + mKeyboardFocus, 0.f, 1.f);

    if (focusStrength > 0.001f) {
      const int focusA = static_cast<int>(kFocusAlphaBase + (kFocusAlphaRange * focusStrength));
      const igraphics::IColor focusColor(focusA, mFocusRing.R, mFocusRing.G, mFocusRing.B);
      const float focusPad = 2.f;
      const float focusThickness = std::max(1.0f, mRECT.W() * 0.012f);
      const float focusCorner = std::max(4.f, mRECT.H() * 0.15f);
      g.DrawRoundRect(focusColor, mRECT.GetPadded(-focusPad), focusCorner, &mBlend, focusThickness);
    }

    if (mKeyboardFocus > 0.001f) {
      mKeyboardFocus -= kKeyboardFocusDecayRate;
      if (mKeyboardFocus < 0.f)
        mKeyboardFocus = 0.f;
      SetDirty(false);
    }

    // Decay press animation
    if (mPressT > 0.001f) {
      mPressT -= 0.08f;  // ~80ms decay (assuming 60fps)
      if (mPressT < 0.f)
        mPressT = 0.f;
      SetDirty(false);
    }
  }

  void OnMouseDown(float x, float y, const igraphics::IMouseMod& mod) override
  {
    if (mod.R)
      return;

    // Determine which cell was clicked
    const float cellGap = std::max(3.f, mRECT.W() * 0.04f);
    const float cellW = (mRECT.W() - 2.f * cellGap) / 3.f;

    for (int i = 0; i < 3; ++i) {
      const float cellX = mRECT.L + cellGap + i * (cellW + cellGap);
      const igraphics::IRECT cellRect(cellX, mRECT.T, cellX + cellW, mRECT.T + mRECT.H());
      if (cellRect.Contains(x, y)) {
        mPressedCell = i;
        mPressT = 1.f;
        SetMode(i);
        return;
      }
    }
  }

  bool OnKeyDown(float x, float y, const iplug::IKeyPress& key) override
  {
    if (key.C || key.A)
      return false;

    if (key.VK == iplug::kVK_SPACE || key.VK == iplug::kVK_RETURN) {
      // Cycle through modes on keyboard
      const int currentMode = GetMode();
      const int nextMode = (currentMode + 1) % 3;
      SetMode(nextMode);
      mKeyboardFocus = 1.f;
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
    mPressedCell = -1;
    SetDirty(false);
  }

private:
  int GetMode() const
  {
    // 0 = RAW, 1 = MOD, 2 = BASS
    // InitEnum maps indices to normalized values: 0→0.0, 1→0.5, 2→1.0
    const double val = GetValue();
    if (val < 1.0 / 3.0) return 0;
    if (val < 2.0 / 3.0) return 1;
    return 2;
  }

  void SetMode(int mode)
  {
    // Use normalized values for 3-value InitEnum: 0→0.0, 1→0.5, 2→1.0
    const double value = (mode == 0) ? 0.0 : (mode == 1) ? 0.5 : 1.0;
    SetValueFromUserInput(value);
    SetDirty(true);
  }

  const char* mFontID = "Roboto-Regular";
  igraphics::IColor mLabelColor;
  igraphics::IColor mMetalPlate;
  igraphics::IColor mPlateInset;
  igraphics::IColor mMetalStroke;
  igraphics::IColor mLedOn;
  igraphics::IColor mFocusRing;
  float mKeyboardFocus = 0.f;
  float mPressT = 0.f;  // depression animation
  int mPressedCell = -1;
};

// Simple vertical bar meter control for I/O visualization.
// Provides visual placeholder that can be connected to audio-thread peak values later.
class SimpleMeterControl final : public igraphics::IControl
{
public:
  SimpleMeterControl(const igraphics::IRECT& bounds,
                    const char* label,
                    const ThemePalette& palette,
                    const char* fontID = nullptr)
  : IControl(bounds, -1)
  , mLabel(label)
  , mFontID(fontID ? fontID : "Roboto-Regular")
  {
    mIgnoreMouse = true;
    SetPalette(palette);
  }

  void SetPalette(const ThemePalette& palette)
  {
    mBackground = palette.vectorBG.WithContrast(-0.15f);
    mMeterFill = palette.vectorPR;
    mMeterHot = igraphics::IColor(255, 255, 60, 36);
    mMeterClip = igraphics::IColor(255, 255, 90, 90);
    mBorder = palette.focusRing;
    mTextColor = palette.toggleLabelText;
  }

  void SetLevels(float leftLevel, float rightLevel)
  {
    mLevelL = leftLevel;
    mLevelR = rightLevel;
    SetDirty(false);
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const float labelH = 14.f;
    const float meterPad = 4.f;
    const igraphics::IRECT labelRect(mRECT.L, mRECT.T, mRECT.R, mRECT.T + labelH);
    const igraphics::IRECT meterArea(mRECT.L + meterPad,
                                     mRECT.T + labelH + meterPad,
                                     mRECT.R - meterPad,
                                     mRECT.B - meterPad);

    // Background
    g.FillRoundRect(mBackground, mRECT, 3.f, &mBlend);
    g.DrawRoundRect(mBorder.WithOpacity(0.5f), mRECT, 3.f, &mBlend, 1.f);

    // Label
    igraphics::IText labelText(9.f, mTextColor.WithOpacity(0.8f), mFontID,
                              igraphics::EAlign::Center, igraphics::EVAlign::Middle);
    g.DrawText(labelText, mLabel, labelRect, &mBlend);

    // Meter bars (L/R)
    const float barGap = 2.f;
    const float barW = (meterArea.W() - barGap) * 0.5f;
    const igraphics::IRECT barL(meterArea.L, meterArea.T, meterArea.L + barW, meterArea.B);
    const igraphics::IRECT barR(barL.R + barGap, meterArea.T, meterArea.R, meterArea.B);

    // Draw empty meter backgrounds
    g.FillRoundRect(mBackground.WithOpacity(0.6f), barL, 2.f, &mBlend);
    g.FillRoundRect(mBackground.WithOpacity(0.6f), barR, 2.f, &mBlend);

    // Draw filled portions
    auto drawMeterBar = [&](const igraphics::IRECT& bar, float level) {
      if (level < 0.001f) return;
      const float fillH = bar.H() * std::clamp(level, 0.f, 1.f);
      const igraphics::IRECT fillRect(bar.L, bar.B - fillH, bar.R, bar.B);

      // Color based on level: normal, hot (>0.7), clip (>0.95)
      igraphics::IColor fillColor;
      if (level > 0.95f) fillColor = mMeterClip;
      else if (level > 0.7f) fillColor = mMeterHot;
      else fillColor = mMeterFill;

      g.FillRoundRect(fillColor, fillRect, 1.f, &mBlend);
    };

    drawMeterBar(barL, mLevelL);
    drawMeterBar(barR, mLevelR);
  }

private:
  const char* mLabel;
  const char* mFontID;
  float mLevelL = 0.f;
  float mLevelR = 0.f;
  igraphics::IColor mBackground;
  igraphics::IColor mMeterFill;
  igraphics::IColor mMeterHot;
  igraphics::IColor mMeterClip;
  igraphics::IColor mBorder;
  igraphics::IColor mTextColor;
};

// LCD-style value readout: black rounded rectangle with red 7-segment style
// digits, a subtle inner bevel, and a soft red glow.
class LcdValueControl final : public igraphics::IControl
{
public:
  LcdValueControl(const igraphics::IRECT& bounds,
                  int paramIdx,
                  const ThemePalette& palette,
                  const char* fontID,
                  float fontSize)
  : IControl(bounds, paramIdx)
  , mFontID((fontID && fontID[0] != '\0') ? fontID : "Roboto-Regular")
  , mFontSize(fontSize)
  {
    mIgnoreMouse = true;
    SetPalette(palette);
  }

  void SetPalette(const ThemePalette& palette)
  {
    mBgColor = palette.lcdBackground;
    mBorderColor = palette.lcdBorder;
    mTextColor = palette.lcdText;
    mGlowColor = palette.lcdGlow;
    mHighlightColor = palette.lcdHighlight;
    mText = igraphics::IText(mFontSize,
                              mTextColor,
                              mFontID,
                              igraphics::EAlign::Center,
                              igraphics::EVAlign::Middle);
  }

  void SetFontSize(float fontSize)
  {
    if (std::fabs(fontSize - mFontSize) < 0.01f)
      return;

    mFontSize = fontSize;
    mText.mSize = fontSize;
    SetDirty(false);
  }

  void Draw(igraphics::IGraphics& g) override
  {
    const float cornerRadius = std::clamp(mRECT.H() * 0.22f, 3.f, 7.f);

    // Outer black panel.
    g.FillRoundRect(mBgColor, mRECT, cornerRadius, &mBlend);

    // Inner beveled edge — gives the LCD a slight inset look.
    const igraphics::IRECT innerRect(mRECT.L + 1.0f, mRECT.T + 1.0f,
                                     mRECT.R - 1.0f, mRECT.B - 1.0f);
    g.DrawRoundRect(mBorderColor, innerRect,
                    std::max(1.f, cornerRadius - 1.f),
                    &mBlend, 1.f);

    // Subtle top highlight (1px) for a glassy LCD edge.
    const float hiY = mRECT.T + 1.5f;
    const igraphics::IRECT topHi(mRECT.L + cornerRadius, hiY,
                                 mRECT.R - cornerRadius, hiY + 1.f);
    g.FillRect(mHighlightColor, topHi, &mBlend);

    // Text rect — padding keeps digits off the LCD bezel.
    const float padX = std::clamp(mRECT.W() * 0.06f, 3.f, 8.f);
    const float padY = std::clamp(mRECT.H() * 0.12f, 2.f, 5.f);
    const igraphics::IRECT textRect(mRECT.L + padX,
                                    mRECT.T + padY,
                                    mRECT.R - padX,
                                    mRECT.B - padY);

    // Fetch the param's display string (formatted by Grungr.cpp SetDisplayFunc).
    const iplug::IParam* pParam = GetParam();
    WDL_String displayStr;
    if (pParam) {
      pParam->GetDisplay(GetValue(), true, displayStr, true);
    }
    const char* txt = displayStr.Get();
    if (!txt || !txt[0]) {
      txt = "0";
    }

    // Soft red glow — two passes with increasing offset and decreasing opacity.
    igraphics::IText glowOuter = mText;
    glowOuter.mFGColor = mGlowColor.WithOpacity(0.30f);
    g.DrawText(glowOuter, txt, textRect.GetTranslated(-1.f, 0.f), &mBlend);
    g.DrawText(glowOuter, txt, textRect.GetTranslated(1.f, 0.f), &mBlend);
    g.DrawText(glowOuter, txt, textRect.GetTranslated(0.f, -1.f), &mBlend);
    g.DrawText(glowOuter, txt, textRect.GetTranslated(0.f, 1.f), &mBlend);

    igraphics::IText glowInner = mText;
    glowInner.mFGColor = mGlowColor.WithOpacity(0.55f);
    g.DrawText(glowInner, txt, textRect, &mBlend);

    // Main bright red digits.
    g.DrawText(mText, txt, textRect, &mBlend);
  }

private:
  const char* mFontID;
  float mFontSize;
  igraphics::IColor mBgColor;
  igraphics::IColor mBorderColor;
  igraphics::IColor mTextColor;
  igraphics::IColor mGlowColor;
  igraphics::IColor mHighlightColor;
  igraphics::IText mText;
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

  void OnMouseDown(float x, float y, const igraphics::IMouseMod& mod) override
  {
    if (mod.R) {
      // Right-click: prompt for text entry (existing behavior)
      PromptUserInput();
      return;
    }

    // Normal click: let base class handle drag
    ISVGKnobControl::OnMouseDown(x, y, mod);
  }

  void OnMouseDblClick(float x, float y, const igraphics::IMouseMod& mod) override
  {
    // Double-click: reset to default. SetValueFromUserInput handles
    // value-change check + SetDirty for redraw. This matches the iPlug2
    // idiom (see IControl.cpp default OnMouseDblClick for sliders etc.)
    if (GetParam())
      SetValueFromUserInput(GetParam()->GetDefault(true));
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
  kTagVersion,
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
  kTagRawToggle,
  kTagPhotoOverlay,
  kTagInputMeter,
  kTagOutputMeter
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

  // I/O meters in the lower control zone
  igraphics::IRECT inputMeter;
  igraphics::IRECT outputMeter;
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
  r.knobValueFontSize = knobSize * 0.28f;

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
  const float valueW = knobSize * 0.90f;

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

  r.titleFontSize = (variant == LayoutVariant::Compact) ? 62.f : (variant == LayoutVariant::Spacious) ? 96.f : 82.f;
  const float titleH = r.titleFontSize * 1.16f;
  const float ledStripY = backgroundBounds.T + backgroundBounds.H() * kLedYNorm;

  const float rawToggleW = knobSize * ((variant == LayoutVariant::Compact) ? 1.45f : 1.29f);
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
                                  titleMaxY) - 12.f;

  const float titleCenterX = backgroundBounds.MW();
  float titleLogoH = titleH;
  float titleLogoW = titleLogoH * kTitleLogoAspect;
  const float maxTitleW = backgroundBounds.W() * 0.92f;
  if (titleLogoW > maxTitleW) {
    titleLogoW = maxTitleW;
    titleLogoH = titleLogoW / kTitleLogoAspect;
  }
  r.title = igraphics::IRECT(titleCenterX - (titleLogoW * 0.5f),
                             titleY - (titleLogoH * 0.5f),
                             titleCenterX + (titleLogoW * 0.5f),
                             titleY + (titleLogoH * 0.5f));

  const float logoH = std::clamp(rawToggleH * 0.82f, knobSize * 0.34f, knobSize * 0.44f);
  const float logoW = logoH * kClophLogoAspect;
  const float rawToggleLeftInset = rawCenterX - (rawToggleW * 0.5f) - backgroundBounds.L;
  const float logoRight = backgroundBounds.R - rawToggleLeftInset;
  const float logoCenterX = logoRight - (logoW * 0.5f) - 30.f;
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

  // I/O meters: place adjacent to GRUNGR title on the right side (per user feedback m0294).
  // Both meters sit side-by-side, just to the right of r.title, vertically aligned with the title block.
  const float meterW = 26.f;
  const float meterGap = 8.f;
  const float meterAreaTop = r.title.T;
  const float meterAreaBottom = r.title.B;
  const float metersLeftX = r.title.R + 14.f;

  // Input meter immediately right of title
  r.inputMeter = igraphics::IRECT(metersLeftX,
                                   meterAreaTop,
                                   metersLeftX + meterW,
                                   meterAreaBottom);

  // Output meter just to the right of the input meter
  r.outputMeter = igraphics::IRECT(metersLeftX + meterW + meterGap,
                                    meterAreaTop,
                                    metersLeftX + meterW + meterGap + meterW,
                                    meterAreaBottom);

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

  SetBoundsIfPresent(g, kTagInputMeter, layout.inputMeter);
  SetBoundsIfPresent(g, kTagOutputMeter, layout.outputMeter);

  if (auto* pOverlay = g.GetControlWithTag(kTagPhotoOverlay)) {
    pOverlay->SetTargetAndDrawRECTs(g.GetBounds());
  }
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
  palette.lcdBackground = kTokenLcdBackground;
  palette.lcdBorder = kTokenLcdBorder;
  palette.lcdText = kTokenLcdText;
  palette.lcdGlow = kTokenLcdGlow;
  palette.lcdHighlight = kTokenLcdHighlight;

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
      .WithLabelText(igraphics::IText(16.f,
                                      palette.toggleLabelText,
                                      textFontID,
                                      igraphics::EAlign::Center,
                                      igraphics::EVAlign::Middle))
      .WithValueText(igraphics::IText(16.f,
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

void SetLCDValueStyleIfPresent(igraphics::IGraphics& g,
                               int tag,
                               const ThemePalette& palette,
                               float fontSize)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pLcd = dynamic_cast<LcdValueControl*>(pControl)) {
      pLcd->SetPalette(palette);
      pLcd->SetFontSize(fontSize);
      pControl->SetDirty(false);
    }
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

void SetVoicingStyleIfPresent(igraphics::IGraphics& g,
                              int tag,
                              const ThemePalette& palette)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    if (auto* pVoicing = dynamic_cast<VoicingSelectControl*>(pControl)) {
      pVoicing->SetPalette(palette);
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
  SetVoicingStyleIfPresent(g, kTagRawToggle, palette);

  SetVectorStyleIfPresent(g, kTagGrungeKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagButtKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagFaceKnob, vectorKnobStyle);
  SetVectorStyleIfPresent(g, kTagLoudKnob, vectorKnobStyle);

  SetTextStyleIfPresent(g, kTagTitle, palette.titleText, layout.titleFontSize);
  SetTextStyleIfPresent(g, kTagGrungeLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagButtLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagFaceLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetTextStyleIfPresent(g, kTagLoudLabel, palette.knobLabelText, layout.knobLabelFontSize);
  SetLCDValueStyleIfPresent(g, kTagGrungeValue, palette, layout.knobValueFontSize);
  SetLCDValueStyleIfPresent(g, kTagButtValue, palette, layout.knobValueFontSize);
  SetLCDValueStyleIfPresent(g, kTagFaceValue, palette, layout.knobValueFontSize);
  SetLCDValueStyleIfPresent(g, kTagLoudValue, palette, layout.knobValueFontSize);
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

  g.AttachControl(new ITextControl(labelRect, label, labelText, COLOR_TRANSPARENT), labelTag);
  g.AttachControl(new LcdValueControl(valueRect, paramIdx, palette, valueFontID, valueFontSize), valueTag);
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

  g.AttachControl(new ITextControl(labelRect, label, labelText, COLOR_TRANSPARENT), labelTag);
  g.AttachControl(new LcdValueControl(valueRect, paramIdx, palette, valueFontID, valueFontSize), valueTag);
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
      auto* pVoicing = new VoicingSelectControl(relayout.rawToggle,
                                                 params.rawVoicing,
                                                 palette,
                                                 rawFontID);
      pVoicing->SetTooltip(kTooltipRaw);
      pGraphics->AttachControl(pVoicing, kTagRawToggle);
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
  const igraphics::ISVG titleSVG = kForceSafeVectorUIMode ? igraphics::ISVG(nullptr) : pGraphics->LoadSVG(assets.titleSVG);

  if (!kForceSafeVectorUIMode && (!knobSVG.IsValid() || knobSVG.W() <= 0.f || knobSVG.H() <= 0.f)) {
    knobSVG = pGraphics->LoadSVG("knob-cropped.svg");
  }

  const bool hasBackgroundSVG = (!kForceSafeVectorUIMode) && backgroundSVG.IsValid() && backgroundSVG.W() > 0.f && backgroundSVG.H() > 0.f;
  const bool hasKnobSVG = (!kForceSafeVectorUIMode) && kUseSVGKnobs && knobSVG.IsValid() && knobSVG.W() > 0.f && knobSVG.H() > 0.f;
  const bool hasLogoSVG = (!kForceSafeVectorUIMode) && logoSVG.IsValid() && logoSVG.W() > 0.f && logoSVG.H() > 0.f;
  const bool hasTitleSVG = (!kForceSafeVectorUIMode) && titleSVG.IsValid() && titleSVG.W() > 0.f && titleSVG.H() > 0.f;

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

  if (hasTitleSVG) {
    pGraphics->AttachControl(new StrokedSVGControl(layout.title, titleSVG), kTagTitle);
  }
  else {
    const igraphics::IText titleText(layout.titleFontSize,
                                     palette.titleText,
                                     titleFontID,
                                     igraphics::EAlign::Center,
                                     igraphics::EVAlign::Middle);
    pGraphics->AttachControl(new StrokedTitleControl(layout.title,
                                                      kTitleText,
                                                      titleText),
                             kTagTitle);
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
                                                        backgroundSVG,
                                                        layout.background,
                                                        labelFontID);
  pBypassToggle->SetTooltip(kTooltipStomp);
  pGraphics->AttachControl(pBypassToggle, kTagBypassToggle);

  auto* pVoicing = new VoicingSelectControl(layout.rawToggle,
                                              params.rawVoicing,
                                              palette,
                                              labelFontID);
  pVoicing->SetTooltip(kTooltipRaw);
  pGraphics->AttachControl(pVoicing, kTagRawToggle);

  // I/O meters in the lower control zone (flanking the bypass toggle area)
  {
    auto* pInputMeter = new SimpleMeterControl(layout.inputMeter, "IN", palette, valueFontID);
    pInputMeter->SetTooltip("Input level (L/R)");
    pGraphics->AttachControl(pInputMeter, kTagInputMeter);

    auto* pOutputMeter = new SimpleMeterControl(layout.outputMeter, "OUT", palette, valueFontID);
    pOutputMeter->SetTooltip("Output level (L/R)");
    pGraphics->AttachControl(pOutputMeter, kTagOutputMeter);
  }

  // Logo is attached LAST so it renders on top of the stomp bypass control,
  // whose cave-in redraws the faceplate SVG over its bounds when engaged.
  // StrokedSVGControl sets mIgnoreMouse=true, so it never blocks the stomp.
  if (hasLogoSVG) {
    pGraphics->AttachControl(new StrokedSVGControl(layout.logo, logoSVG), kTagLogo);
  }

  pGraphics->AttachControl(new NinetiesPhotoOverlay(uiBounds, 0xC0FFEE17u), kTagPhotoOverlay);

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

void UpdateMeterLevels(igraphics::IGraphics* pGraphics,
                       float inputL, float inputR,
                       float outputL, float outputR)
{
  if (!pGraphics) return;
  if (auto* pInput = pGraphics->GetControlWithTag(kTagInputMeter)) {
    if (auto* pMeter = dynamic_cast<SimpleMeterControl*>(pInput)) {
      pMeter->SetLevels(inputL, inputR);
    }
  }
  if (auto* pOutput = pGraphics->GetControlWithTag(kTagOutputMeter)) {
    if (auto* pMeter = dynamic_cast<SimpleMeterControl*>(pOutput)) {
      pMeter->SetLevels(outputL, outputR);
    }
  }
}

}  // namespace ui
}  // namespace grungr

