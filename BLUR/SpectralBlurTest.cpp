#include "SpectralBlurTest.h"

#include <algorithm>
#include <cstring>
#include <array>
#include <cmath>

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "EmbeddedFonts.h"

namespace {

constexpr int kFixedFftSizeIndex = 5;

// ─────────────────────────────────────────────────────────────────────────────
// Palette v5 — CRYO
//
// Concept: cryogenic STFT processor — spectral material suspended in
// crystalline stasis. Pure glacier ice palette: no warmth, no teal. A single
// high-refractive accent reads as cold light through polar ice. Everything
// else recedes into the dark void of the hull.
// ─────────────────────────────────────────────────────────────────────────────

IColor MakeBackgroundColor() { return IColor(255,  5,  8, 18); }   // void black ice
IColor MakeSurfaceColor()    { return IColor(255, 10, 16, 28); }   // cryo plate
IColor MakeBorderColor()     { return IColor(255, 22, 42, 78); }   // glacier shoreline
IColor MakeTextColor()       { return IColor(255, 214, 232, 250); } // frost white
IColor MakeMutedTextColor()  { return IColor(255, 58, 96, 152); }  // deep ice mute
IColor MakeAccentColor()     { return IColor(255, 138, 208, 255); } // glacier ice — COLD
IColor MakeAccentDimColor()  { return IColor(128, 138, 208, 255); } // accent at half alpha
IColor MakeAccentSoftColor() { return IColor(255, 32, 76, 155); }  // deep ice fill
IColor MakeTrackColor()      { return IColor(255,  8, 16, 32); }   // cryo groove
IColor MakeDeepColor()       { return IColor(255,  5,  8, 16); }   // void body

IText MakeTitleText(float size, EAlign align = EAlign::Near)
{
  return IText(size, MakeTextColor(), "Squarified", align, EVAlign::Middle);
}

IText MakeCaptionText(float size, EAlign align = EAlign::Center)
{
  return IText(size, MakeMutedTextColor(), "Sono-Regular", align, EVAlign::Middle);
}

void AttachGroup(IGraphics* pGraphics, const char* label, const char* groupName, const IVStyle& style)
{
  pGraphics->AttachControl(new IVGroupControl(label, groupName, 10.f, 16.f, 10.f, 10.f, style));
}

// Base instrument style.
// Roundness 0.38 — organic tactile hardware feel per manifesto skeuomorphism.
// Shadows enabled — physical depth, not flat UI cards.
// kHL is the "glow state" colour — hot white-cyan for active/pressed indicators.
IVStyle MakeProtocolStyle()
{
  return DEFAULT_STYLE
    .WithColor(kBG, MakeSurfaceColor())
    .WithColor(kFG, MakeAccentSoftColor())
    .WithColor(kPR, MakeAccentColor())
    .WithColor(kFR, MakeBorderColor())
    .WithColor(kHL, IColor(255, 220, 242, 255))   // frost glow — ice luminance
    .WithColor(kSH, IColor(72, 0, 0, 0))           // shadow: alpha 72, visible depth
    .WithColor(kX1, MakeTrackColor())
    .WithColor(kX2, MakeDeepColor())
    .WithColor(kX3, IColor(255, 9, 13, 20))
    .WithLabelText(IText(17.0f, MakeMutedTextColor(), "Sono-Regular", EAlign::Center, EVAlign::Middle))
    .WithValueText(IText(14.0f, MakeTextColor(), "Roboto-Bold", EAlign::Center, EVAlign::Middle))
    .WithRoundness(0.38f)
    .WithDrawFrame(true)
    .WithDrawShadows(true);
}


// ─────────────────────────────────────────────────────────────────────────────
// Responsive Scaling — Desktop DAW Adaptation
//
// CRYO scales with DAW window size. Reference: 760×400px.
// Scale factor = min(currentW / 760, currentH / 400)
// All dimensions scale uniformly; layout structure (3-col) preserved.
//
// Responsive range: 600–1600px width. Min 600×300 (hard limit for 3-col layout).
// ─────────────────────────────────────────────────────────────────────────────

// Compute uniform scale factor from current bounds
inline float ComputeScale(const IRECT& bounds)
{
  constexpr float kRefWidth = 760.f;
  constexpr float kRefHeight = 400.f;
  const float wScale = bounds.W() / kRefWidth;
  const float hScale = bounds.H() / kRefHeight;
  return std::min(wScale, hScale);
}

// Clamp scaled dimension to min/max range to preserve readability
// (std::clamp available in C++17; fallback for older standards)
inline float ClampedScale(float baseValue, float scale, float minVal, float maxVal)
{
  const float scaled = baseValue * scale;
  return scaled < minVal ? minVal : (scaled > maxVal ? maxVal : scaled);
}


// ─────────────────────────────────────────────────────────────────────────────
// IceBackgroundControl — renders ice-background.svg filling the full viewport
//
// Uses "cover" scaling mode: the SVG is scaled so its width fills the viewport
// exactly, then centered vertically (overflow is clipped by the control bounds).
// Falls back to a solid dark fill when the SVG is unavailable.
// ─────────────────────────────────────────────────────────────────────────────
class IceBackgroundControl : public IControl
{
  ISVG mSVG;
public:
  IceBackgroundControl(const IRECT& bounds, const ISVG& svg)
    : IControl(bounds, kNoParameter), mSVG(svg)
  {
  }

  void Draw(IGraphics& g) override
  {
    // Void black ice — the primary background. The SVG is rendered
    // as a subtle textural accent at reduced opacity, not as the main visual.
    g.FillRect(IColor(255, 3, 5, 12), mRECT);

    if (!mSVG.IsValid()) return;

    // Render SVG at ~20% opacity: subtle crystalline texture, does not
    // compete with controls. Cover mode: scale to fill width, centre vertically.
    const float coverScale = mRECT.W() / mSVG.W();
    const float drawH      = mSVG.H() * coverScale;
    const float vertOffset = (drawH - mRECT.H()) * 0.5f;
    const IRECT dest(mRECT.L,
                     mRECT.T - vertOffset,
                     mRECT.R,
                     mRECT.T - vertOffset + drawH);
    
    static const IBlend lowOpacity(EBlend::Default, 0.20f);
    g.DrawSVG(mSVG, dest, &lowOpacity);
  }
};

// ─────────────────────────────────────────────────────────────────────────────
// Brand Logo with Ice Effect — renders the brand SVG wordmark with a frost
// crystalline overlay (ice signal layer).
//
// The SVG has a natural aspect ratio of ~2.568:1 (viewBox 1461.67 × 569).
// It is aspect-fit (contain) inside the control rect, centred vertically.
// Ice layer adds snowflake asterisks and horizontal frost streaks on top.
// ─────────────────────────────────────────────────────────────────────────────
class LogoWithIceControl : public IControl
{
  static constexpr float kPi        = 3.14159265358979f;
  static constexpr float kSvgAspect = 1461.67f / 569.f;   // logo viewBox W/H

  ISVG mSVG;

  // Aspect-contain the logo inside r, centred
  IRECT GetFitRect(const IRECT& r) const {
    const float ra = r.W() / r.H();
    if (ra > kSvgAspect) {
      const float fitW = r.H() * kSvgAspect;
      return r.GetCentredInside(fitW, r.H());
    } else {
      const float fitH = r.W() / kSvgAspect;
      return r.GetCentredInside(r.W(), fitH);
    }
  }

  // Draw a 3-armed asterisk (6-pointed ice crystal) at (cx, cy)
  void DrawCrystal(IGraphics& g, float cx, float cy, float sz, const IColor& col) const {
    for (int j = 0; j < 3; j++) {
      const float a = (static_cast<float>(j) / 3.f) * kPi;
      g.DrawLine(col,
        cx - std::cos(a) * sz, cy - std::sin(a) * sz,
        cx + std::cos(a) * sz, cy + std::sin(a) * sz,
        nullptr, 0.8f);
    }
  }

  // Ice signal layer: frost streaks + corner asterisks
  void DrawIceLayer(IGraphics& g, const IRECT& r) const {
    const IColor streak(38, 138, 208, 255);   // dim glacier — barely visible bands
    const IColor crystal(55, 138, 208, 255);  // slightly brighter for crystals
    const float sw = 0.9f;

    // 4 horizontal frost streaks at irregular offsets
    g.DrawLine(streak, r.L,              r.T + r.H() * 0.20f, r.L + r.W() * 0.70f, r.T + r.H() * 0.20f, nullptr, sw);
    g.DrawLine(streak, r.L + r.W()*0.12f, r.T + r.H() * 0.48f, r.R,                 r.T + r.H() * 0.48f, nullptr, sw);
    g.DrawLine(streak, r.L,              r.T + r.H() * 0.72f, r.L + r.W() * 0.85f, r.T + r.H() * 0.72f, nullptr, sw);
    g.DrawLine(streak, r.L + r.W()*0.05f, r.T + r.H() * 0.90f, r.R,                 r.T + r.H() * 0.90f, nullptr, sw);

    // Ice crystal asterisks at four corners
    const float csz = std::min(r.W(), r.H()) * 0.09f;
    const float pad = csz * 1.6f;
    DrawCrystal(g, r.L + pad, r.T + pad,  csz, crystal);   // TL
    DrawCrystal(g, r.R - pad, r.T + pad,  csz, crystal);   // TR
    DrawCrystal(g, r.L + pad, r.B - pad,  csz, crystal);   // BL
    DrawCrystal(g, r.R - pad, r.B - pad,  csz, crystal);   // BR

    // Very subtle cold tint wash over entire logo rect
    g.FillRect(IColor(10, 138, 208, 255), r);
  }

public:
  LogoWithIceControl(const IRECT& bounds, const ISVG& svg)
    : IControl(bounds), mSVG(svg) {}

  void Draw(IGraphics& g) override {
    if (mSVG.IsValid()) {
      const IRECT dest = GetFitRect(mRECT);
      g.DrawSVG(mSVG, dest);
    }
    DrawIceLayer(g, mRECT);
  }
};


// ─────────────────────────────────────────────────────────────────────────────
// HeaderPanelControl — full-bleed header surface with panel edge detail
//
// Draws a slightly raised cryo-plate surface under the logo/title/bypass zone,
// cleanly separates the instrument header from the control body.
// ─────────────────────────────────────────────────────────────────────────────
class HeaderPanelControl : public IControl
{
public:
  HeaderPanelControl(const IRECT& bounds) : IControl(bounds, kNoParameter) {}

  void Draw(IGraphics& g) override
  {
    // Cryo plate surface — one tone above void background
    g.FillRect(MakeSurfaceColor(), mRECT);
    // Top bevel — bright-edge catch on the machined top face
    g.DrawLine(IColor(255, 20, 38, 72), mRECT.L, mRECT.T, mRECT.R, mRECT.T, nullptr, 1.f);
    // Frost gleam — cold light refracted across upper panel face
    g.DrawLine(IColor(12, 158, 198, 255),
      mRECT.L + mRECT.W() * 0.04f, mRECT.T + 1.f,
      mRECT.L + mRECT.W() * 0.52f, mRECT.T + 1.f, nullptr, 1.0f);
    // Bottom separator — hard edge dividing header from body
    g.DrawLine(MakeBorderColor(), mRECT.L, mRECT.B - 1.f, mRECT.R, mRECT.B - 1.f, nullptr, 1.0f);
  }
};


// ─────────────────────────────────────────────────────────────────────────────
// HeaderTitleControl — two-line brand block: CRYO product name + SPECTRAL
// sub-caption. Font sizes derived from mRECT.H() at draw time so they scale
// correctly with responsive resize without requiring a rebuild.
// ─────────────────────────────────────────────────────────────────────────────
class HeaderTitleControl : public IControl
{
public:
  HeaderTitleControl(const IRECT& bounds) : IControl(bounds, kNoParameter) {}

  void Draw(IGraphics& g) override
  {
    const float h = mRECT.H();
    // Primary brand — dominant Squarified weight
    const float cryoSz = std::max(14.f, h * 0.62f);
    g.DrawText(
      IText(cryoSz, MakeTextColor(), "Squarified", EAlign::Near, EVAlign::Middle),
      "CRYO", IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + h * 0.68f));
    // Sub-caption — product family, muted ghost label
    const float subSz = std::max(7.f, h * 0.19f);
    g.DrawText(
      IText(subSz, MakeMutedTextColor(), "Sono-Regular", EAlign::Near, EVAlign::Middle),
      "SPECTRAL", IRECT(mRECT.L, mRECT.T + h * 0.72f, mRECT.R, mRECT.B));
  }
};


// ─────────────────────────────────────────────────────────────────────────────
// GhostKnobControl — custom IKnobControlBase rendering the "Ghost Machine" SVG
// design using IGraphics path primitives.
//
// Visual layers (back→front):
//   outer machined ring · progress arc · 60 tick marks · body fill
//   phosphor indicator with glow · value / label text
// ─────────────────────────────────────────────────────────────────────────────
class GhostKnobControl : public IKnobControlBase
{
  struct TickGeometry
  {
    float outerX = 0.f;
    float outerY = 0.f;
    float innerMajorX = 0.f;
    float innerMajorY = 0.f;
    float innerMinorX = 0.f;
    float innerMinorY = 0.f;
    float angleDeg = 0.f;
    bool  major = false;
    bool  inRange = false;
  };

  const char* mLabel;
  bool        mMouseDown = false;
  mutable float mCachedW = 0.f;
  mutable float mCachedH = 0.f;
  mutable std::array<TickGeometry, 60> mTickCache {};

  void UpdateCache() const
  {
    if (mCachedW == mRECT.W() && mCachedH == mRECT.H()) {
      return;
    }

    mCachedW = mRECT.W();
    mCachedH = mRECT.H();

    static constexpr float kPi = 3.14159265358979f;
    for (int i = 0; i < 60; ++i) {
      const float taDeg = -180.f + 6.f * static_cast<float>(i);
      const float ta = (taDeg - 90.f) * (kPi / 180.f);
      TickGeometry& tick = mTickCache[static_cast<size_t>(i)];
      tick.outerX = cosf(ta) * 119.f;
      tick.outerY = sinf(ta) * 119.f;
      tick.innerMajorX = cosf(ta) * 109.f;
      tick.innerMajorY = sinf(ta) * 109.f;
      tick.innerMinorX = cosf(ta) * 115.f;
      tick.innerMinorY = sinf(ta) * 115.f;
      tick.angleDeg = taDeg;
      tick.major = (i % 5 == 0);
      tick.inRange = (taDeg >= -135.f && taDeg <= 135.f);
    }
  }

public:
  GhostKnobControl(const IRECT& bounds, int paramIdx, const char* label)
    : IKnobControlBase(bounds, paramIdx), mLabel(label) {}

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    mMouseDown = true;
    IKnobControlBase::OnMouseDown(x, y, mod);
  }
  void OnMouseUp(float x, float y, const IMouseMod& mod) override
  {
    mMouseDown = false;
    IKnobControlBase::OnMouseUp(x, y, mod);
  }

  void Draw(IGraphics& g) override
  {
    static constexpr float kPi = 3.14159265358979f;
    UpdateCache();
    const float cx = mRECT.MW(), cy = mRECT.MH();
    // Derive scale from the available square; the SVG reference radius is 140.
    const float ro    = std::min(mRECT.W(), mRECT.H()) * 0.5f - 2.f;
    const float sc    = ro / 140.f;
    const float bodyR = 104.f * sc;
    const bool  hover = GetMouseIsOver();
    const bool  disabled = IsDisabled();

    // Indicator angle: -135°→+135° from north; adjust by -90° for cos/sin east-origin
    const float val      = static_cast<float>(GetValue());
    const float angleDeg = -135.f + val * 270.f;
    const float angleRad = (angleDeg - 90.f) * (kPi / 180.f);
    const int disabledAlpha = disabled ? 78 : 255;
    const int dimAlpha = disabled ? 52 : 255;

    // ── Outer cryo ring ──────────────────────────────────────────────────────
    g.DrawCircle(IColor(disabledAlpha, 10, 20, 40),  cx, cy, 128.f * sc, nullptr, 16.f * sc);
    g.DrawCircle(IColor(disabledAlpha,  7, 16, 34),  cx, cy, 119.f * sc, nullptr, 0.8f);
    g.DrawCircle(IColor(disabledAlpha,  7, 16, 34),  cx, cy, 137.f * sc, nullptr, 0.8f);
    // Crystalline frost gleam — refracted cold light on ring edges
    if (!disabled) {
      // Primary refraction — upper-left face, dominant cold-light catch
      g.DrawArc(IColor(26, 175, 215, 255), cx, cy, 134.f * sc, -82.f, -24.f, nullptr, 1.4f * sc + 0.5f);
      // Secondary — lower-right counter-gleam
      g.DrawArc(IColor(11, 150, 194, 255), cx, cy, 133.f * sc,  16.f,  50.f, nullptr, 0.8f * sc + 0.3f);
      // Micro-catches — complex multi-point refraction on machined ring face
      g.DrawArc(IColor( 8, 140, 178, 255), cx, cy, 134.5f*sc, -18.f,  -8.f, nullptr, 0.6f * sc + 0.2f);
      g.DrawArc(IColor( 6, 130, 168, 255), cx, cy, 133.5f*sc, 142.f, 155.f, nullptr, 0.5f * sc + 0.2f);
    }

    // ── Progress arc on outer ring ────────────────────────────────────────────
    // Track dim: shows full travel extent. Value arc: lit from min to current.
    {
      const float trackR   = 124.f * sc;
      const float trackThk = 3.f * sc + 1.5f;
      const IColor trackFg = mMouseDown
        ? IColor(255, 218, 240, 255)
        : IColor(200, 130, 202, 255);
      g.DrawArc(IColor(disabledAlpha, 28, 120, 185), cx, cy, trackR, -135.f, 135.f, nullptr, trackThk);
      if (val > 1e-4f)
        g.DrawArc(disabled ? IColor(120, 136, 168, 198) : trackFg, cx, cy, trackR, -135.f, angleDeg, nullptr, trackThk);
    }

    // ── Travel endpoint markers ───────────────────────────────────────────────
    {
      const float mr  = 124.f * sc;
      const float mlR = (-135.f - 90.f) * (kPi / 180.f);
      const float mrR = ( 135.f - 90.f) * (kPi / 180.f);
      const IColor mc = IColor(disabled ? 48 : 120, 108, 168, 218);
      g.FillCircle(mc, cx + cosf(mlR) * mr, cy + sinf(mlR) * mr, 2.5f * sc + 1.0f);
      g.FillCircle(mc, cx + cosf(mrR) * mr, cy + sinf(mrR) * mr, 2.5f * sc + 1.0f);
    }

    // ── 60 tick marks — active range lit below current value ──────────────────
    // Ticks use north-origin degrees matching DrawArc convention.
    // ta_north: degrees from top (north), CW positive.  Range = [-135..+135].
    // ta_rad  : east-origin radians for cos/sin spatial placement.
    for (const TickGeometry& tick : mTickCache) {
      const bool lit = tick.inRange && (tick.angleDeg <= angleDeg);
      IColor tickCol;
      if (disabled)     tickCol = IColor(tick.major ? 82 : 54, 86, 112, 138);
      else if (lit)     tickCol = IColor(tick.major ? 200 : 110, 138, 208, 255);
      else if (tick.inRange) tickCol = tick.major ? IColor(255, 18, 42, 80) : IColor(255,  9, 26, 56);
      else              tickCol = IColor(40,  8, 18, 44);
      g.DrawLine(tickCol,
        cx + tick.outerX * sc, cy + tick.outerY * sc,
        cx + (tick.major ? tick.innerMajorX : tick.innerMinorX) * sc,
        cy + (tick.major ? tick.innerMajorY : tick.innerMinorY) * sc,
        nullptr, tick.major ? 1.2f : 0.6f);
    }

    // ── Inner bevel ring ─────────────────────────────────────────────────────
    // Machined precision edge separating tick field from body — layered hardware depth.
    g.DrawCircle(IColor(disabledAlpha,  5, 12, 28), cx, cy, 108.5f * sc, nullptr, 2.5f);
    g.DrawCircle(IColor(disabledAlpha, 20, 40, 76), cx, cy, 107.2f * sc, nullptr, 0.7f);
    if (!disabled) {
      g.DrawArc(IColor(13, 152, 192, 255), cx, cy, 108.0f * sc, -118.f, -50.f, nullptr, 1.1f);
    }

    // ── Body ─────────────────────────────────────────────────────────────────
    g.FillCircle(IColor(dimAlpha,  6, 10, 20), cx, cy, 107.f * sc);
    g.FillCircle(IColor(disabledAlpha,  8, 13, 24), cx, cy, bodyR);
    g.DrawCircle(IColor(disabledAlpha, 12, 26, 52), cx, cy, bodyR, nullptr, 1.f);
    // Frost sphere lighting — ambient fill + rim arc on body edge (physically accurate)
    if (!disabled) {
      // Diffuse ambient — dim northern luminance blush on body interior
      g.FillCircle(IColor(7, 188, 222, 255), cx - bodyR * 0.10f, cy - bodyR * 0.26f, bodyR * 0.40f);
      // Rim arc — crisp cold light refracted at body edge, upper-left face
      g.DrawArc(IColor(36, 205, 235, 255), cx, cy, bodyR * 0.968f, -148.f, -52.f, nullptr, 3.2f * sc + 0.7f);
      g.DrawArc(IColor(16, 172, 208, 255), cx, cy, bodyR * 0.972f, -156.f, -44.f, nullptr, 5.8f * sc + 1.0f);
    }

    // ── Indicator — ice needle with frost glow ────────────────────────────────
    {
      const float ix1 = cx + cosf(angleRad) * 26.f * sc;
      const float iy1 = cy + sinf(angleRad) * 26.f * sc;
      const float ix2 = cx + cosf(angleRad) * 80.f * sc;
      const float iy2 = cy + sinf(angleRad) * 80.f * sc;

      IStrokeOptions rnd;
      rnd.mCapOption = ELineCap::Round;

      // Drag halo (large, intense) then hover halo
      if (mMouseDown) {
        g.PathClear();
        g.PathMoveTo(ix1, iy1); g.PathLineTo(ix2, iy2);
        g.PathStroke(IColor(80, 138, 208, 255), 22.f * sc, rnd);
      } else if (hover) {
        g.PathClear();
        g.PathMoveTo(ix1, iy1); g.PathLineTo(ix2, iy2);
        g.PathStroke(IColor(42, 138, 208, 255), 14.f * sc, rnd);
      }
      // Frost glow — intensified when active
      if (!disabled) {
        const int glowAlpha = mMouseDown ? 100 : (hover ? 70 : 50);
        g.PathClear();
        g.PathMoveTo(ix1, iy1); g.PathLineTo(ix2, iy2);
        g.PathStroke(IColor(glowAlpha, 138, 208, 255), 8.f * sc, rnd);
      }
      // Ice needle
      const int lineAlpha = disabled ? 126 : (mMouseDown ? 255 : (hover ? 240 : 217));
      g.PathClear();
      g.PathMoveTo(ix1, iy1); g.PathLineTo(ix2, iy2);
      g.PathStroke(IColor(lineAlpha, 138, 208, 255), 2.f, rnd);
      // Crystal tip — expands when grabbed
      g.FillCircle(IColor(disabled ? 100 : 200, 152, 216, 255), ix2, iy2, (mMouseDown ? 4.5f : 3.f) * sc);
      // Hub cap — machined precision socket covering needle base
      g.FillCircle(IColor(dimAlpha, 10, 18, 36), cx, cy, 24.f * sc);
      g.DrawCircle(IColor(disabledAlpha, 22, 44, 84), cx, cy, 24.f * sc, nullptr, 1.0f);
      // Centre jewel — ice phosphor point, dims when disabled
      g.FillCircle(IColor(disabled ? 68 : 165, 138, 208, 255), cx, cy, 5.0f * sc);
    }

    // ── Value + label ─────────────────────────────────────────────────────────
    // Use mRECT-relative positioning so text always sits in the visible lower
    // half of the control regardless of knob size.  PathClear ensures no
    // residual NanoVG path/stencil state from the indicator strokes bleeds into
    // the text rendering path.
    {
      g.PathClear();

      const float textH  = std::max(12.f, std::min(26.f, bodyR * 0.32f));
      const float lblH   = std::max(10.f, std::min(18.f, bodyR * 0.24f));
      const float hintH  = std::max(9.f,  std::min(13.f, bodyR * 0.14f));
      const float textW  = bodyR * 2.0f;
      // Y positions staggered to prevent overlap at small knob sizes.
      const float valY   = cy + bodyR * 0.14f;
      const float lblY   = cy + bodyR * 0.50f;
      const float hintY  = cy + bodyR * 0.72f;

      WDL_String display;
      const IParam* pParam = GetParam();
      if (pParam) pParam->GetDisplayWithLabel(display);

      const char* valStr = (pParam && display.GetLength() > 0) ? display.Get() : "--";

      // Readout pill — dark panel with engraved border
      const float pw = textW * 0.9f, ph = textH * 1.6f;
      const IRECT pillR = IRECT(cx - pw*0.5f, valY - ph*0.5f, cx + pw*0.5f, valY + ph*0.5f);
      g.FillRoundRect(IColor(disabled ? 82 : 128, 3, 6, 18), pillR, ph * 0.35f);
      g.DrawRoundRect(IColor(disabled ? 36 : 76, 22, 44, 82), pillR, ph * 0.35f, nullptr, 0.7f);

      g.DrawText(IText(textH, disabled ? MakeMutedTextColor() : MakeTextColor(), "Roboto-Bold", EAlign::Center, EVAlign::Middle),
        valStr, IRECT(cx - textW*0.5f, valY - textH, cx + textW*0.5f, valY + textH));

      if (mLabel && mLabel[0]) {
        g.DrawText(IText(lblH, disabled ? IColor(255, 88, 122, 168) : MakeMutedTextColor(), "Sono-Regular", EAlign::Center, EVAlign::Middle),
          mLabel, IRECT(cx - textW*0.5f, lblY - lblH, cx + textW*0.5f, lblY + lblH));
        if (disabled && std::strcmp(mLabel, "Hold") == 0) {
          g.DrawText(IText(hintH, IColor(255, 92, 128, 176), "Sono-Regular", EAlign::Center, EVAlign::Middle),
            "Locked in Fixed", IRECT(cx - textW*0.5f, hintY - hintH, cx + textW*0.5f, hintY + hintH));
        }
      }
    }
  }
};

class GhostToggleControl : public IVToggleControl
{
  const char* mLabel;
public:
  GhostToggleControl(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style)
  : IVToggleControl(bounds, paramIdx, "", style.WithShowLabel(false)
      .WithValueText(IText(15.f, MakeTextColor(), "Sono-Regular", EAlign::Center, EVAlign::Middle)),
      label, label)
  , mLabel(label)
  {}

  void DrawValue(IGraphics& g, bool mouseOver) override
  {
    const bool on = GetValue() > 0.5;
    const bool disabled = IsDisabled();
    
    // Determine colors based on state
    IColor fill, border, dotCol;
    if (disabled) {
      fill    = IColor(255, 6, 10, 18);
      border  = IColor(255, 16, 28, 52);
      dotCol  = IColor(255, 28, 52, 92);
    } else {
      fill    = on ? IColor(255, 24, 58, 110) : IColor(255,  8, 16, 30);
      border  = on ? MakeAccentColor()        : IColor(255, 38, 66, 104);
      dotCol  = on ? MakeAccentColor()        : IColor(35, 138, 208, 255);
    }

    g.FillRoundRect(fill,   mValueBounds, 7.f, &mBlend);
    g.DrawRoundRect(border, mValueBounds, 7.f, &mBlend, 1.1f);

    const float dotW = std::max(3.f, mValueBounds.H() * 0.16f);
    const IRECT dot  = IRECT(mValueBounds.L + 8.f,
                             mValueBounds.MH() - dotW * 0.5f,
                             mValueBounds.L + 8.f + dotW,
                             mValueBounds.MH() + dotW * 0.5f);
    g.FillRoundRect(dotCol, dot, dotW * 0.45f, &mBlend);

    // Hover overlay
    if (mouseOver && !disabled)
      g.FillRoundRect(IColor(42, 138, 208, 255), mValueBounds, 7.f, &mBlend);

    const IText text = mStyle.valueText.WithFGColor(disabled ? IColor(255, 52, 80, 120) : (on ? MakeTextColor() : MakeMutedTextColor()));
    g.DrawText(text, mLabel, mValueBounds.GetReducedFromLeft(14.f), &mBlend);
  }
};

class HoldSeparatorControl : public IControl
{
public:
  HoldSeparatorControl(const IRECT& bounds) : IControl(bounds, kNoParameter) {}
  void Draw(IGraphics& g) override {
    g.DrawLine(MakeBorderColor(), mRECT.L, mRECT.MH(), mRECT.R, mRECT.MH(), nullptr, 1.f);
  }
};

class CycleButtonControl : public IControl
{
private:
  int mLocalState = 0;  // Track local state to avoid async param read delays

public:
  CycleButtonControl(const IRECT& bounds, int paramIdx, const IVStyle& /*style*/)
    : IControl(bounds, paramIdx) {}

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    static const char* kDesc[] = { "Hold locked", "Hold tracks input", "Hold drifts" };
    // Cycle using local state instead of reading param (which may be stale)
    mLocalState = (mLocalState + 1) % 3;
    SetValue(mLocalState / 2.0);  // Direct normalized: 0→0.0, 1→0.5, 2→1.0
    SetTooltip(kDesc[mLocalState]);
    SetDirty(true);
  }

  void SetValueFromDelegate(double value, int midiMsgStartIdx) override
  {
    // Sync local state when param changes from external source (undo, preset load, etc.)
    mLocalState = static_cast<int>(std::round(value * 2.0));
    IControl::SetValueFromDelegate(value, midiMsgStartIdx);
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override
  {
    IControl::OnMouseOver(x, y, mod);
    static const char* kDesc[] = { "Hold locked", "Hold tracks input", "Hold drifts" };
    SetTooltip(kDesc[std::max(0, std::min(2, mLocalState))]);
    SetDirty(false);
  }

  void OnMouseOut() override
  {
    IControl::OnMouseOut();
    SetDirty(false);  // Trigger redraw when leaving
  }

  void Draw(IGraphics& g) override
  {
    const int  state    = mLocalState;
    const bool hover    = GetMouseIsOver();
    const bool disabled = IsDisabled();
    static const char* kLabels[] = { "Fixed", "Track", "Ghost" };
    IColor fill, border, textCol;
    
    // Determine colors based on state
    if (disabled) {
      fill    = IColor(255, 6, 10, 18);
      border  = IColor(255, 16, 28, 52);
      textCol = IColor(255, 52, 80, 120);
    } else {
      switch (state) {
        default:
        case 0:  // Fixed — latched, neutral
          fill    = IColor(255, 10, 18, 34);
          border  = IColor(255, 28, 52, 96);
          textCol = MakeMutedTextColor();
          break;
        case 1:  // Track — following signal
          fill    = IColor(255, 14, 36, 80);
          border  = IColor(255, 54, 122, 192);
          textCol = IColor(255, 154, 206, 252);
          break;
        case 2:  // Ghost — spectral drift
          fill    = IColor(255, 20, 50, 108);
          border  = MakeAccentColor();
          textCol = MakeTextColor();
          break;
      }
    }
    
    // Draw base button
    g.FillRoundRect(fill, mRECT, 8.f, &mBlend);
    g.DrawRoundRect(border, mRECT, 8.f, &mBlend, 1.2f);
    
    // Hover / Active overlay
    if (hover && !disabled) {
      g.FillRoundRect(IColor(28, 200, 230, 255), mRECT, 8.f, &mBlend);
    }
    
    // Draw text
    g.DrawText(
      IText(18.f, textCol, "Sono-Regular", EAlign::Center, EVAlign::Middle),
      kLabels[state], mRECT, &mBlend);

  }
};


class GhostSliderControl : public ISliderControlBase
{
  bool mDrag = false;

  IRECT CalcTrackRect() const
  {
    // Responsive label/value widths based on text height
    const float kLblW = std::max(35.f, mRECT.H() * 2.0f);  // scale with control height
    const float kValW = std::max(50.f, mRECT.H() * 2.5f);  // scale with control height
    constexpr float kTrkH = 4.f;
    return IRECT(mRECT.L + kLblW, mRECT.MH() - kTrkH * 0.5f,
                 mRECT.R - kValW, mRECT.MH() + kTrkH * 0.5f);
  }

public:
  GhostSliderControl(const IRECT& bounds, int paramIdx)
    : ISliderControlBase(bounds, paramIdx, EDirection::Horizontal, DEFAULT_GEARING)
  {
    mTrackBounds = CalcTrackRect();
  }

  void OnResize() override
  {
    mTrackBounds = CalcTrackRect();
    SetDirty(false);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    mDrag = true;
    ISliderControlBase::OnMouseDown(x, y, mod);
  }

  void OnMouseUp(float x, float y, const IMouseMod& mod) override
  {
    mDrag = false;
    ISliderControlBase::OnMouseUp(x, y, mod);
  }

  void Draw(IGraphics& g) override
  {
    const float val  = static_cast<float>(GetValue());
    const bool hover = GetMouseIsOver();
    const IRECT tr   = CalcTrackRect();
    const float tL   = tr.L;
    const float tW   = tr.W();
    const float tMY  = tr.MH();
    const float tH   = tr.H();
    const float hx   = tL + tW * val;
    const float hHW  = mDrag ? 6.f : (hover ? 5.5f : 4.5f);
    const float hHH  = std::min(mRECT.H() - 6.f, 18.f);
    const IRECT hndl = IRECT(hx - hHW, tMY - hHH * 0.5f,
                             hx + hHW, tMY + hHH * 0.5f);
    // Responsive text size — scales with control height
    const float txtH = std::max(10.f, std::min(18.f, mRECT.H() * 0.40f));

    // ── Track groove ─────────────────────────────────────────────────────────
    // Outer shadow frame makes groove appear recessed into the panel.
    g.FillRoundRect(IColor(255, 3, 6, 14), tr.GetPadded(1.0f), tH * 0.5f + 1.f);
    g.FillRoundRect(MakeTrackColor(), tr, tH * 0.5f);
    // Inner-top shadow line — engraved depth cue
    g.DrawLine(IColor(160, 2, 4, 10), tL + tH, tr.T + 0.8f, tr.R - tH, tr.T + 0.8f, nullptr, 0.6f);

    // ── 0dB unity marker — tick through centre of travel ──────────────────
    // Param range is -40..+40 dB; 0dB = normalised 0.5 exactly.
    const float zeroX = tL + tW * 0.5f;
    g.DrawLine(IColor(65, 138, 208, 255), zeroX, tr.T - 3.f, zeroX, tr.B + 3.f, nullptr, 0.8f);

    // ── Fill: lit range from min to handle position ───────────────────────
    if (hx > tL + 1.f) {
      IRECT fillR = tr; fillR.R = hx;
      g.FillRoundRect(IColor(255, 22, 60, 118), fillR, tH * 0.5f);
      // Top-edge gleam on filled portion — lit groove feel
      g.DrawLine(IColor(55, 138, 208, 255), tL + tH, tr.T + 0.9f, hx - 1.f, tr.T + 0.9f, nullptr, 0.8f);
    }

    // Track border
    g.DrawRoundRect(MakeBorderColor(), tr, tH * 0.5f, nullptr, 0.8f);

    // End-stop circles at track terminations
    g.FillCircle(IColor(80, 90, 140, 200), tL,   tMY, tH * 0.68f);
    g.FillCircle(IColor(80, 90, 140, 200), tr.R, tMY, tH * 0.68f);

    // ── Handle glow halo ─────────────────────────────────────────────────────
    if (mDrag)
      g.FillRoundRect(IColor(55, 138, 208, 255), hndl.GetPadded(5.f), hHW + 5.f);
    else if (hover)
      g.FillRoundRect(IColor(25, 138, 208, 255), hndl.GetPadded(3.f), hHW + 3.f);

    // ── Fader cap — vertical capsule ─────────────────────────────────────────
    const IColor hFill = mDrag ? MakeAccentColor() : IColor(255, 68, 130, 196);
    g.FillRoundRect(hFill, hndl, hHW);
    g.DrawRoundRect(MakeAccentColor(), hndl, hHW, nullptr, 1.2f);
    // Top gleam — cold light on upper cap face
    g.DrawLine(IColor(48, 205, 232, 255),
               hndl.L + hHW * 0.4f, hndl.T + 2.0f,
               hndl.R - hHW * 0.4f, hndl.T + 2.0f, nullptr, 0.8f);
    // Machined centre groove — positional reference mark
    g.DrawLine(IColor(90, 2, 4, 12),
               hndl.L + hHW * 0.28f, tMY,
               hndl.R - hHW * 0.28f, tMY, nullptr, 1.0f);
    g.DrawLine(IColor(32, 200, 228, 255),
               hndl.L + hHW * 0.28f, tMY + 1.2f,
               hndl.R - hHW * 0.28f, tMY + 1.2f, nullptr, 0.6f);

    // ── GAIN label — fixed left, responsive size ─────────────────────────────
    g.DrawText(
      IText(txtH, MakeMutedTextColor(), "Sono-Regular", EAlign::Near, EVAlign::Middle),
      "GAIN", IRECT(mRECT.L, mRECT.T, tr.L - 4.f, mRECT.B));

    // ── Value readout — engraved pill on the right ────────────────────────────
    WDL_String disp;
    if (GetParam()) GetParam()->GetDisplayWithLabel(disp);
    const char* valStr = disp.GetLength() > 0 ? disp.Get() : "--";
    const IRECT valZone = IRECT(tr.R + 4.f, mRECT.T, mRECT.R, mRECT.B);
    const float pH = std::min(mRECT.H() * 0.58f, valZone.H() - 4.f);
    const IRECT pillR = valZone.GetCentredInside(valZone.W() - 4.f, pH);
    g.FillRoundRect(IColor(115, 3, 6, 18), pillR, pillR.H() * 0.30f);
    g.DrawRoundRect(IColor(64, 22, 44, 82),  pillR, pillR.H() * 0.30f, nullptr, 0.7f);
    g.DrawText(
      IText(txtH, MakeTextColor(), "Roboto-Bold", EAlign::Center, EVAlign::Middle),
      valStr, valZone);
  }
};

}  // namespace

SpectralBlur::SpectralBlur(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kParamBlurAmount)->InitDouble("Blur amount", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamVarianceType)->InitEnum("Variance", 0, {"Fixed", "Track", "Ghost"});
  GetParam(kParamBlurVariance)->InitDouble("Blur variance", 0.5, 0.0, 100.0, 0.1, "%");
  GetParam(kParamLoBinCutoff)->InitDouble("Lo bin cutoff", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamHiBinCutoff)->InitDouble("Hi bin cutoff", 100.0, 0.0, 100.0, 0.1, "%");
  GetParam(kParamRandomizePhases)->InitBool("Randomize phases", true, "", IParam::kFlagsNone, "", "Off", "On");
  GetParam(kParamFFTSize)->InitEnum("FFT Size", 0, {"16384"});
  GetParam(kParamOutputGain)->InitDouble("Gain", 0.0, -40.0, 40.0, 0.1, "dB");
  GetParam(kParamBypass)->InitBool("Bypass", false, "", IParam::kFlagsNone, "", "Off", "On");

  SyncParamsToProcessor();

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) { LayoutUI(pGraphics); };
#endif
}

#if IPLUG_EDITOR
void SpectralBlur::ConstrainEditorSize(int& w, int& h) const
{
  // Enforce 1.9:1 (760:400) aspect ratio.
  // Width drives; height is derived. Then clamp to allowed limits.
  constexpr float kAspect   = 760.f / 400.f;
  constexpr int   kMinWidth = 600;
  constexpr int   kMaxWidth = 1600;
  constexpr int   kMinH     = 300;
  constexpr int   kMaxH     = 860;

  w = std::max(kMinWidth, std::min(kMaxWidth, w));
  h = static_cast<int>(std::round(w / kAspect));

  if (h < kMinH) { h = kMinH; w = static_cast<int>(std::round(h * kAspect)); }
  if (h > kMaxH) { h = kMaxH; w = static_cast<int>(std::round(h * kAspect)); }
}

void SpectralBlur::OnParentWindowResize(int width, int height)
{
  // Compute the aspect-ratio-correct dimensions using the same logic as
  // ConstrainEditorSize, then push the corrected size back to the host.
  int w = width;
  int h = height;
  ConstrainEditorSize(w, h);

  if (GetUI())
  {
    // needsPlatformResize=false: the host already resized its window (enforced
    // by checkSizeConstraint before onSize was called). We only need to tell
    // IGraphics to update its canvas to the new dimensions. Passing true here
    // would call plugFrame->resizeView back into the host, causing a feedback
    // loop that races with the in-flight LayoutUI call and misaligns controls.
    GetUI()->Resize(w, h, 1.f, false);
  }
}

void SpectralBlur::LayoutUI(IGraphics* pGraphics)
{
  const IRECT bounds = pGraphics->GetBounds();
  const float scale  = ComputeScale(bounds);

  // ── Mathematical spacing system ──────────────────────────────────────────
  // Base unit: kUnit = 8px. All dimensions are exact N×kUnit multiples at
  // reference scale (1.0 = 760×400), giving a strict 8-pt proportional grid.
  //
  // Knob size series — 19:25:32 unit ratios (≈ 5:6:8, pure harmonic series):
  //   kLoHiSz = 19 units = 152px
  //   kHoldSz = 25 units = 200px   (200/152 ≈ 5:4, major third)
  //   kBlurSz = 32 units = 256px   (256/152 ≈ 8:5, Fibonacci-adjacent)
  //
  // Geometric lock: with 16px canvas padding, body height at reference = 312px,
  // and kLoHiSz stack (152×2 + inGap 8) = 312px — exact fill, no rounding.
  //
  // Header title/subtitle split at golden ratio: kGolden = 1/φ ≈ 0.618.
  // All clamping bounds are integer multiples of kUnit.
  static constexpr float kUnit      = 8.f;
  static constexpr float kGolden    = 0.6180339887f;  // 1/φ — golden ratio
  static constexpr float kCanvasPad = kUnit * 2.f;    // 16px — 2 units fixed margin

  const IRECT padded = bounds.GetPadded(-kCanvasPad);

  // ── Responsive dimensions (all N×kUnit, clamped to integer unit bounds) ──
  const float headerH   = ClampedScale(kUnit * 6,  scale, kUnit * 5,  kUnit * 8);  // 48 → [40, 64]
  const float headerGap = ClampedScale(kUnit,       scale, kUnit,      kUnit * 2);  //  8 → [ 8, 16]
  const float colGap    = ClampedScale(kUnit * 2,  scale, kUnit,      kUnit * 3);  // 16 → [ 8, 24]
  const float inGap     = ClampedScale(kUnit,       scale, kUnit,      kUnit * 2);  //  8 → [ 8, 16]
  const float kBlurSz   = ClampedScale(kUnit * 32, scale, kUnit * 24, kUnit * 52); // 256 → [192, 416] — max extends to window ceiling
  const float kLoHiSz   = ClampedScale(kUnit * 19, scale, kUnit * 15, kUnit * 32); // 152 → [120, 256] — max extends to window ceiling
  const float kHoldSz   = ClampedScale(kUnit * 25, scale, kUnit * 19, kUnit * 42); // 200 → [152, 336] — max extends to window ceiling
  const float kBtnH     = ClampedScale(kUnit * 6,  scale, kUnit * 5,  kUnit * 7);  //  48 → [ 40,  56]
  const float kGainH    = ClampedScale(kUnit * 5,  scale, kUnit * 4,  kUnit * 6);  //  40 → [ 32,  48]

  // ── Compute all layout rects (computed on every call) ───────────────────
  const IRECT headerArea    = padded.GetFromTop(headerH);
  const IRECT headerPanelR  = IRECT(bounds.L, bounds.T, bounds.R, headerArea.B);
  const IRECT bodyArea      = padded.GetReducedFromTop(headerH + headerGap);

  // ── Header: title left · Bypass aligned with hold column below (logo hidden) ──────
  // Logo is hidden to reduce visual clutter (CRYO + SPECTRAL text is sufficient)
  const float titleW  = ClampedScale(kUnit * 42, scale, kUnit * 32, kUnit * 52); // 336→[256,416] — expanded title space

  // Logo: subtle watermark in bottom-right corner (responsive scaling with design system)
  // Size: 80 units at reference → scales responsively with scale factor
  const float logoSz = ClampedScale(kUnit * 10, scale, kUnit * 8, kUnit * 15);  // 80 → [64, 120] responsive
  const IRECT logoR  = IRECT(padded.R - logoSz, padded.B - logoSz, padded.R, padded.B);
  // Title positioned from left with full available space
  const IRECT titleR  = headerArea.GetFromLeft(titleW);

  // Column widths with sensible min/max bounds
  const float kBlurColW_target = std::max(100.f, bodyArea.W() * 0.40f);  // at least 100px wide
  const float kLoHiColW_target = std::max(80.f, bodyArea.W() * 0.27f);   // at least 80px wide
  // ACTUAL widths used (account for caps to maintain column alignment)
  const float blurColW = std::min(kBlurColW_target, bodyArea.W() * 0.5f);  // cap at 50% to ensure space for other cols
  const float loHiColW = std::min(kLoHiColW_target, bodyArea.W() * 0.35f); // cap at 35%
  // Columns positioned using ACTUAL widths, not targets
  const IRECT blurCol   = bodyArea.GetFromLeft(blurColW);
  const IRECT loHiCol   = bodyArea.GetReducedFromLeft(blurColW + colGap).GetFromLeft(loHiColW);
  const IRECT holdCol   = bodyArea.GetReducedFromLeft(blurColW + colGap + loHiColW + colGap);

  // Bypass aligned with hold column left edge, centered vertically in header
  const IRECT bypassR = IRECT(holdCol.L, headerArea.T, holdCol.R, headerArea.B).GetCentredInside(holdCol.W(), headerH * 0.7f);

  const IRECT blurKnobR = blurCol.GetFromTop(kBlurSz + inGap * 2.f).GetCentredInside(kBlurSz, kBlurSz);
  const IRECT gainR     = blurCol.GetReducedFromTop(kBlurSz + inGap * 2.f + inGap)
                            .GetFromTop(kGainH).GetCentredInside(blurCol.W() * 0.9f, kGainH);

  const IRECT stackArea = loHiCol.GetFromTop(kLoHiSz * 2.f + inGap);
  const IRECT loKnobR   = stackArea.GetFromTop(kLoHiSz).GetCentredInside(kLoHiSz, kLoHiSz);
  const IRECT hiKnobR   = stackArea.GetFromBottom(kLoHiSz).GetCentredInside(kLoHiSz, kLoHiSz);
  // Band label positioned in the mid-gap between Lo and Hi knobs (responsive height)
  const float bandLblH  = std::max(10.f, inGap * 0.5f);
  const float bandLblY  = stackArea.T + kLoHiSz;  // center line between knobs
  const IRECT bandLabelR = IRECT(loHiCol.L, bandLblY - bandLblH * 0.5f, loHiCol.R, bandLblY + bandLblH * 0.5f);

  // Hold column: scatter → cycle → hold knob → mode hint
  // Responsive padding: scale inset from fixed 2px to responsive fraction of button height
  const float btnPadding = std::max(1.f, kBtnH * 0.04f);  // ~2-4% of button height
  const float sepInset   = std::max(2.f, colGap * 0.5f);  // responsive separator inset
  const float contentH    = kBtnH + inGap + kBtnH + inGap + kHoldSz + inGap + kBtnH;
  const IRECT holdContent = holdCol.GetFromTop(contentH);
  const IRECT phaseScatterR = holdContent.GetFromTop(kBtnH).GetPadded(-btnPadding);
  const IRECT separatorR  = IRECT(holdCol.L + sepInset, holdContent.T + kBtnH + inGap * 0.40f,
                                   holdCol.R - sepInset, holdContent.T + kBtnH + inGap * 0.60f);
  const IRECT cycleR      = holdContent.GetReducedFromTop(kBtnH + inGap)
                              .GetFromTop(kBtnH).GetPadded(-btnPadding);
  const IRECT holdKnobR   = holdContent.GetReducedFromTop(kBtnH + inGap + kBtnH + inGap)
                              .GetFromTop(kHoldSz).GetCentredInside(kHoldSz, kHoldSz);
  const IRECT modeHintR   = holdContent.GetReducedFromTop(kBtnH + inGap + kBtnH + inGap + kHoldSz + inGap)
                              .GetFromTop(kBtnH).GetPadded(-btnPadding);

  // ── RESIZE PATH: reposition existing controls, do not recreate ──────────
  if (pGraphics->NControls()) {
    pGraphics->GetBackgroundControl()->SetTargetAndDrawRECTs(bounds);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagHeaderPanel))   c->SetTargetAndDrawRECTs(headerPanelR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagLogo))          c->SetTargetAndDrawRECTs(logoR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagTitle))          c->SetTargetAndDrawRECTs(titleR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagBypass))         c->SetTargetAndDrawRECTs(bypassR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagGainSlider))     c->SetTargetAndDrawRECTs(gainR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagBlurKnob))       c->SetTargetAndDrawRECTs(blurKnobR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagLoKnob))         c->SetTargetAndDrawRECTs(loKnobR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagHiKnob))         c->SetTargetAndDrawRECTs(hiKnobR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagBandLabel))      c->SetTargetAndDrawRECTs(bandLabelR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagPhaseScatter))   c->SetTargetAndDrawRECTs(phaseScatterR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagSeparator))      c->SetTargetAndDrawRECTs(separatorR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagVarianceType))   c->SetTargetAndDrawRECTs(cycleR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagHoldKnob))       c->SetTargetAndDrawRECTs(holdKnobR);
    if (auto* c = pGraphics->GetControlWithTag(kCtrlTagModeHint))       c->SetTargetAndDrawRECTs(modeHintR);
    return;
  }

  // ── FIRST-TIME SETUP ─────────────────────────────────────────────────────
  pGraphics->SetLayoutOnResize(true);
  pGraphics->EnableMouseOver(true);
  pGraphics->EnableTooltips(true);
  pGraphics->AttachControl(new IceBackgroundControl(bounds, pGraphics->LoadSVG(ICE_BG_FN)));
  pGraphics->AttachTextEntryControl();
#ifndef OS_IOS
  pGraphics->AttachPopupMenuControl(DEFAULT_LABEL_TEXT);
#endif
  pGraphics->AttachBubbleControl();
  pGraphics->LoadFont("Squarified",            (void*)kFontSquarified,            kFontSquarified_size);
  pGraphics->LoadFont("Roboto-Bold",            (void*)kFontRobotoBold,            kFontRobotoBold_size);
  pGraphics->LoadFont("Montserrat-LightItalic", (void*)kFontMontserratLightItalic, kFontMontserratLightItalic_size);
  pGraphics->LoadFont("Trigram-Light",          TRIGRAM_FN);
  pGraphics->LoadFont("Roboto-Regular",         ROBOTO_FN);
  pGraphics->LoadFont("Sono-Regular",           (void*)kFontSonoRegular,           kFontSonoRegular_size);

  const IVStyle style = MakeProtocolStyle();

  pGraphics->AttachControl(new HeaderPanelControl(headerPanelR),                                                              kCtrlTagHeaderPanel);
  pGraphics->AttachControl(new LogoWithIceControl(logoR, pGraphics->LoadSVG(LOGO_FN)),                                       kCtrlTagLogo);
  pGraphics->AttachControl(new HeaderTitleControl(titleR),                                                                    kCtrlTagTitle);
  pGraphics->AttachControl(new GhostToggleControl(bypassR, kParamBypass, "BYPASS", style),                                                        kCtrlTagBypass);
  pGraphics->AttachControl(new GhostSliderControl(gainR, kParamOutputGain),                                                  kCtrlTagGainSlider);

  pGraphics->AttachControl(new GhostKnobControl(blurKnobR, kParamBlurAmount,    "Blur"),                                    kCtrlTagBlurKnob);
  pGraphics->AttachControl(new GhostKnobControl(loKnobR,   kParamLoBinCutoff,   "Lo"),                                     kCtrlTagLoKnob);
  pGraphics->AttachControl(new GhostKnobControl(hiKnobR,   kParamHiBinCutoff,   "Hi"),                                     kCtrlTagHiKnob);
  pGraphics->AttachControl(new ITextControl(bandLabelR, "Band", MakeCaptionText(std::max(12.f, kBtnH * 0.35f), EAlign::Center)), kCtrlTagBandLabel);
  pGraphics->AttachControl(new GhostToggleControl(phaseScatterR, kParamRandomizePhases, "SCATTER", style),                   kCtrlTagPhaseScatter);
  pGraphics->AttachControl(new HoldSeparatorControl(separatorR),                                                              kCtrlTagSeparator);
  pGraphics->AttachControl(new CycleButtonControl(cycleR, kParamVarianceType, style),                                        kCtrlTagVarianceType);
  pGraphics->AttachControl(new GhostKnobControl(holdKnobR, kParamBlurVariance,   "Hold"),                                   kCtrlTagHoldKnob);

  if (IControl* pHold = pGraphics->GetControlWithTag(kCtrlTagHoldKnob)) {
    const bool isFixed = GetParam(kParamVarianceType)->Value() < 0.5;
    pHold->SetDisabled(isFixed);
  }
}
#endif

void SpectralBlur::OnParamChange(int paramIdx)
{
  if (paramIdx == kParamVarianceType) {
    // Disable Hold knob when Fixed mode (0) is selected
    if (IGraphics* pUI = GetUI()) {
      if (IControl* pHold = pUI->GetControlWithTag(kCtrlTagHoldKnob)) {
        const bool isFixed = GetParam(kParamVarianceType)->Value() < 0.5;
        pHold->SetDisabled(isFixed);
      }
    }
  }
}

void SpectralBlur::OnReset()
{
  const int channelCount = std::max(1, std::min(2, MaxNChannels(ERoute::kOutput)));
  const double sampleRate = (GetSampleRate() > 0.0) ? GetSampleRate() : 44100.0;
  mProcessor.prepare(sampleRate, std::max(1, GetBlockSize()), channelCount);
  mProcessor.reset();
  SyncParamsToProcessor();
}

void SpectralBlur::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // Sync once per block on the processing thread to avoid cross-thread DSP mutation.
  SyncParamsToProcessor();

  const int nIn = NInChansConnected();
  const int nOut = NOutChansConnected();
  const int processChannels = std::max(1, std::min(2, std::max(nIn, nOut)));

  if (nFrames <= 0) {
    return;
  }

  // Check bypass state
  const bool bypassOn = GetParam(kParamBypass)->Bool();

  // If bypass is engaged, pass input directly to output
  if (bypassOn) {
    for (int channel = 0; channel < nOut; ++channel) {
      if (outputs != nullptr && outputs[channel] != nullptr) {
        if (inputs != nullptr && nIn > 0 && inputs[channel] != nullptr) {
          std::memcpy(outputs[channel], inputs[channel], nFrames * sizeof(sample));
        } else {
          std::memset(outputs[channel], 0, nFrames * sizeof(sample));
        }
      }
    }
    return;
  }

  for (int channel = 0; channel < 2; ++channel) {
    if (static_cast<int>(mInputBuffers[channel].size()) < nFrames) {
      mInputBuffers[channel].resize(nFrames);
      mOutputBuffers[channel].resize(nFrames);
    }
  }

  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex) {
    const bool hasInput0 = (inputs != nullptr && nIn > 0 && inputs[0] != nullptr);
    const bool hasInput1 = (inputs != nullptr && nIn > 1 && inputs[1] != nullptr);
    const float in0 = hasInput0 ? static_cast<float>(inputs[0][sampleIndex]) : 0.0f;
    const float in1 = hasInput1 ? static_cast<float>(inputs[1][sampleIndex]) : in0;
    mInputBuffers[0][sampleIndex] = in0;
    mInputBuffers[1][sampleIndex] = in1;
  }

  for (int channel = 0; channel < 2; ++channel) {
    mInputPtrs[channel] = mInputBuffers[channel].data();
    mOutputPtrs[channel] = mOutputBuffers[channel].data();
  }

  mProcessor.processBlock(mInputPtrs.data(), mOutputPtrs.data(), processChannels, nFrames);


  for (int sampleIndex = 0; sampleIndex < nFrames; ++sampleIndex) {
    if (outputs != nullptr && nOut > 0 && outputs[0] != nullptr) {
      outputs[0][sampleIndex] = static_cast<sample>(mOutputBuffers[0][sampleIndex]);
    }

    if (outputs != nullptr && nOut > 1 && outputs[1] != nullptr) {
      outputs[1][sampleIndex] = static_cast<sample>(mOutputBuffers[1][sampleIndex]);
    }

    for (int channel = 2; channel < nOut; ++channel) {
      if (outputs != nullptr && outputs[channel] != nullptr) {
        outputs[channel][sampleIndex] = 0.0;
      }
    }
  }
}

void SpectralBlur::SyncParamsToProcessor()
{
  spectralblur::Parameters parameters;
  parameters.blurAmountPercent = static_cast<float>(GetParam(kParamBlurAmount)->Value());
  parameters.varianceType = GetParam(kParamVarianceType)->Int();
  parameters.blurVariancePercent = static_cast<float>(GetParam(kParamBlurVariance)->Value());
  parameters.loBinCutoffPercent = static_cast<float>(GetParam(kParamLoBinCutoff)->Value());
  parameters.hiBinCutoffPercent = static_cast<float>(GetParam(kParamHiBinCutoff)->Value());
  parameters.randomizePhases = GetParam(kParamRandomizePhases)->Bool();
  parameters.fftSizeIndex = kFixedFftSizeIndex;
  parameters.outputGainDb = static_cast<float>(GetParam(kParamOutputGain)->Value());
  mProcessor.setParameters(parameters);
}
