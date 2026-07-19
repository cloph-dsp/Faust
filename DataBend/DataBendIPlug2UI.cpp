#include "DataBendIPlug2UI.h"

#include "config.h"
#include "DataBend.h"
#include "IControls.h"

#include <atomic>
#include <cmath>

namespace databend::ui
{
constexpr float kTitleSize = 42.f;
constexpr float kSubtitleSize = 17.f;
constexpr float kVersionSize = 14.f;
constexpr float kFooterSize = 16.f;
constexpr float kKnobLabelSize = 16.f;
constexpr float kKnobValueSize = 18.f;
constexpr float kPopupMenuSize = 16.f;

// Inline N-cell click-target bar for enum params.
// Active cell: filled accent bg + primary text.
// Inactive cell: outline border + secondary text.
// Click → host-inform gesture (per AGENTS.md Two-param mutual-bounds clamp pattern).
class EnumBarControl final : public IControl, public IVectorBase
{
public:
  EnumBarControl(const IRECT& bounds, int paramIdx, const char* const* labels, int nLabels, const char* tooltip, const IVStyle& style)
    : IControl(bounds, paramIdx), IVectorBase(style, false, true)
    , mLabels(labels), mN(nLabels)
  {
    AttachIControl(this, nullptr);
    SetTooltip(tooltip);
  }

  void Draw(IGraphics& g) override
  {
    const float cw = mRECT.W() / static_cast<float>(mN);
    const float gap = 3.f;
    const float cellPad = 2.f;
    const int activeIdx = static_cast<int>(GetParam()->Value() + 0.5f);

    for (int i = 0; i < mN; ++i)
    {
      const bool active = (activeIdx == i);
      const IRECT cell(
        mRECT.L + i * cw + cellPad,
        mRECT.T + cellPad,
        mRECT.L + (i + 1) * cw - cellPad,
        mRECT.B - cellPad
      );
      const IRECT inner = cell.GetPadded(-gap);

      if (active)
        g.FillRoundRect(GetColor(kPR), inner, 3.f, &mBlend);
      else
        g.DrawRoundRect(GetColor(kFR), inner, 3.f, &mBlend, 1.f);

      g.DrawText(
        active ? mStyle.valueText : mStyle.labelText,
        mLabels[i],
        inner,
        &mBlend
      );
    }

    // F: Cyan glow line below the active cell when mouse is over.
    if (mMouseIsOver)
    {
      const float cw = mRECT.W() / static_cast<float>(mN);
      const float cellPad = 2.f;
      const int activeIdx = static_cast<int>(GetParam()->Value() + 0.5f);
      const IRECT activeCell(
        mRECT.L + activeIdx * cw + cellPad,
        mRECT.T + cellPad,
        mRECT.L + (activeIdx + 1) * cw - cellPad,
        mRECT.B - cellPad
      );
      const IRECT glowLine(activeCell.L, activeCell.B, activeCell.R, activeCell.B + 3.f);
      g.FillRect(IColor(255, 103, 196, 214).WithOpacity(0.8f), glowLine);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (IsDisabled() || !GetParam()) return;
    const int n = GetParam()->NDisplayTexts();
    if (n == 0) return;
    const float cw = mRECT.W() / static_cast<float>(n);
    int idx = static_cast<int>((x - mRECT.L) / cw);
    idx = std::max(0, std::min(idx, n - 1));
    if (idx == static_cast<int>(GetParam()->Value() + 0.5f)) return; // already selected
    // Host-inform gesture (AGENTS.md Two-param mutual-bounds clamp pattern)
    GetDelegate()->BeginInformHostOfParamChangeFromUI(GetParamIdx());
    GetDelegate()->GetParam(GetParamIdx())->Set(static_cast<double>(idx));
    GetDelegate()->EndInformHostOfParamChangeFromUI(GetParamIdx());
    SetDirty(true);
  }

private:
  const char* const* mLabels;
  int mN;
};

// Custom SVG knob cap: static SVG body + rotating line pointer indicator.
// Per AGENTS.md "Custom SVG knob caps — static body + rotating pointer
// indicator (NOT full-cap rotation), no stock IVectorBase dots shipped".
// Subclasses IControl + IVectorBase (matching EnumBarControl's pattern
// in this file) and implements its own drag + dbl-click-reset so we don't
// pull in the stock default-look knob's vector primes.
class SVGKnobControl final : public IControl, public IVectorBase
{
public:
  SVGKnobControl(const IRECT& bounds, const ISVG& svg, int paramIdx, const IVStyle& style)
    : IControl(bounds, paramIdx), IVectorBase(style, true, false), mSVG(svg)
  {
    AttachIControl(this, nullptr);
    SetPromptShowsParamLabel(false);
  }

  void Draw(IGraphics& graphics) override
  {
    graphics.DrawSVG(mSVG, mRECT, &mBlend);

    constexpr double kPiD = 3.14159265358979323846;
    const double v = GetValue();
    const double angle = (-135.0 + v * 270.0) * (kPiD / 180.0);
    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    const float radius = mRECT.W() * 0.5f * 0.78f;
    const float px = static_cast<float>(cx + std::sin(angle) * radius);
    const float py = static_cast<float>(cy - std::cos(angle) * radius);

    const IColor pointerColor = GetColor(kPR);
    const float alpha = (mMouseIsOver && !IsDisabled()) ? 1.f : 0.85f;
    // DrawLine(IColor, x0, y0, x1, y1, blend, thickness) — arg order: blend
    // before thickness, hence the swap vs the cropped IControls.h doc-string.
    graphics.DrawLine(pointerColor.WithOpacity(alpha),
                      cx, cy, px, py, &mBlend, 2.5f);

    const     IParam* p = GetParam();
    if (p)
    {
      WDL_String disp;
      const_cast<IParam*>(p)->GetDisplay(disp); // IParam::GetDisplay(WDL_String&) is non-const on this iPlug2 build
      const IRECT valueLabelRect(mRECT.L, mRECT.B - 18.f, mRECT.R, mRECT.B - 4.f);
      graphics.DrawText(mStyle.valueText, disp.Get(), valueLabelRect, &mBlend);
    }

    if (mParamName.GetLength() > 0)
    {
      const IRECT labelRect(mRECT.L, mRECT.T + 2.f, mRECT.R, mRECT.T + 18.f);
      graphics.DrawText(mStyle.labelText, mParamName.Get(), labelRect, &mBlend);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (IsDisabled() || !GetParam()) return;
    PromptUserInput(mRECT);
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    const IParam* p = GetParam();
    if (IsDisabled() || !p) return;
    const double v = p->Value() - dY * p->GetRange() / 200.f;
    SetValueFromUserInput(std::clamp(v, p->GetMin(), p->GetMax()));
  }

  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override
  {
    const IParam* p = GetParam();
    if (IsDisabled() || !p) return;
    SetValueFromUserInput(p->GetDefault(true));
  }

  void SetDirty(bool triggerAction = true, int valueIndex = kNoValIdx) override
  {
    IControl::SetDirty(triggerAction, valueIndex);
    if (const IParam* p = GetParam()) p->GetDisplay(mValueStr);
  }

  void SetParamName(const char* name)
  {
    if (name) mParamName.Set(name);
    else      mParamName.Set("");
  }

private:
  ISVG mSVG;
  WDL_String mParamName;
};

// Vertical peak meter: RMS body (exponential decay) + peak-hold bar (accent).
// Uses a companion std::atomic<float>* for real-time DSP peak capture.
// IsDirty()→true pattern (IFPSDisplayControl) gives per-frame Draw() calls.
class IPeakMeterControl final : public IControl
{
public:
  IPeakMeterControl(const IRECT& bounds, std::atomic<float>* peak, const IColor& accent)
    : IControl(bounds, kNoParameter)
    , mPeak(peak)
    , mAccent(accent)
    , mPeakHold(0.f)
    , mRmsLevel(0.f)
  {
  }

  bool IsDirty() override { return true; }

  void Draw(IGraphics& graphics) override
  {
    // Read and decay from DSP atomics on every draw (60fps via IsDirty=true)
    const float p = mPeak->load();
    mPeakHold = std::max(mPeakHold * 0.995f, p);
    mRmsLevel = mRmsLevel * 0.93f + p * 0.07f;

    const IRECT r = mRECT;
    graphics.FillRect(IColor(255, 30, 32, 38), r);

    const float rmsH = r.H() * mRmsLevel;
    const float peakH = r.H() * mPeakHold;

    if (rmsH > 0.5f)
    {
      const IRECT rmsRect(r.L, r.B - rmsH, r.R, r.B);
      const float alpha = 0.25f + 0.75f * mRmsLevel;
      graphics.FillRect(mAccent.WithOpacity(alpha), rmsRect);
    }

    if (peakH > 0.5f)
    {
      const IRECT peakRect(r.L, r.B - peakH, r.R, r.B - peakH + 2.f);
      graphics.FillRect(mAccent, peakRect);
    }

    graphics.DrawRect(IColor(255, 60, 65, 75), r, nullptr, 0.5f);
  }

private:
  std::atomic<float>* mPeak;
  IColor mAccent;
  float mPeakHold;
  float mRmsLevel;
};

// Breathing LED: circle with sin-wave alpha oscillation (period 2.5s).
// IsDirty()→true gives per-frame Draw() calls; phase advances each Draw.
class BreathingLEDControl final : public IControl
{
public:
  BreathingLEDControl(const IRECT& bounds, const IColor& color)
    : IControl(bounds, kNoParameter)
    , mColor(color)
    , mPhase(0.f)
  {
  }

  bool IsDirty() override { return true; }

  void Draw(IGraphics& graphics) override
  {
    // Advance phase: 2π/2.5s at 60fps → Δphase = 2π/(2.5*60) ≈ 0.0419 rad/frame
    mPhase += 0.0419f;
    if (mPhase > 6.283185307179586f)
      mPhase -= 6.283185307179586f;

    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    const float radius = std::min(mRECT.W(), mRECT.H()) * 0.5f;
    const float alpha = 0.4f + 0.3f * (std::sin(mPhase) + 1.f);
    graphics.FillCircle(mColor.WithOpacity(std::max(0.f, std::min(1.f, alpha))),
                        cx, cy, radius, nullptr);
  }

private:
  IColor mColor;
  float mPhase;
};

void BuildLayout(IGraphics* graphics, DataBend* plugin)
{
  const IColor backgroundColor(255, 14, 15, 18);
  const IColor panelColor(255, 24, 27, 33);
  const IColor frameColor(255, 72, 80, 90);
  const IColor accentColor(255, 255, 92, 130);
  const IColor secondaryAccent(255, 103, 196, 214);
  const IColor primaryTextColor(255, 243, 239, 232);
  const IColor secondaryTextColor(255, 158, 164, 171);

  const char* uiFont = nullptr;

  if (graphics->LoadFont(kLayoutFontID, "Segoe UI", ETextStyle::Normal)
      || graphics->LoadFont(kLayoutFontID, "Arial", ETextStyle::Normal)
      || graphics->LoadFont(kLayoutFontID, "Verdana", ETextStyle::Normal))
  {
    uiFont = kLayoutFontID;
  }

  graphics->AttachPanelBackground(backgroundColor);

  IText popupText(kPopupMenuSize, primaryTextColor, uiFont, EAlign::Near, EVAlign::Middle);
  graphics->AttachPopupMenuControl(popupText);
  graphics->EnableMouseOver(true);
  graphics->EnableTooltips(true);
  graphics->AttachBubbleControl();
  graphics->AssignParamNameToolTips();

  const IVStyle knobStyle = DEFAULT_STYLE
    .WithColor(kBG, panelColor)
    .WithColor(kFG, IColor(255, 92, 99, 112))
    .WithColor(kPR, accentColor)
    .WithColor(kFR, frameColor)
    .WithColor(kHL, secondaryAccent)
    .WithColor(kX1, primaryTextColor)
    .WithLabelText(IText(kKnobLabelSize, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Top))
    .WithValueText(IText(kKnobValueSize, primaryTextColor, uiFont, EAlign::Center, EVAlign::Bottom))
    .WithRoundness(0.18f)
    .WithDrawShadows(false);

  const IVStyle selectorStyle = knobStyle.WithShowValue(false);

  const IRECT bounds = graphics->GetBounds().GetPadded(-24.f);
  const IRECT titleRect(bounds.L + 4.f, bounds.T + 4.f, bounds.R - 4.f, bounds.T + 58.f);
  const IRECT subtitleRect(bounds.L, titleRect.B - 2.f, bounds.R, titleRect.B + 28.f);

  // A: RGB-shift — three overlapping "DATABEND" layers for chromatic-aberration effect.
  // Red channel offset 2px left, original accent center, cyan 2px right.
  graphics->AttachControl(new ITextControl(titleRect.GetFromLeft(titleRect.W()),
                                           "DATABEND",
                                           IText(kTitleSize, IColor(255, 255, 80, 80), uiFont, EAlign::Center, EVAlign::Middle)));
  graphics->AttachControl(new ITextControl(IRECT(titleRect.L + 2.f, titleRect.T, titleRect.R + 2.f, titleRect.B),
                                           "DATABEND",
                                           IText(kTitleSize, accentColor, uiFont, EAlign::Center, EVAlign::Middle)));
  graphics->AttachControl(new ITextControl(IRECT(titleRect.L - 2.f, titleRect.T, titleRect.R - 2.f, titleRect.B),
                                           "DATABEND",
                                           IText(kTitleSize, IColor(255, 103, 196, 214), uiFont, EAlign::Center, EVAlign::Middle)));

  graphics->AttachControl(new ITextControl(subtitleRect,
                                           "Real-time packet loss, rewind slips, and crushed decoder damage.",
                                           IText(kSubtitleSize, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle)));

  // ---- I/O Peak Meters (flanking title area) ----
  constexpr float kMeterW = 10.f;
  constexpr float kMeterH = 44.f;
  const IRECT leftMeterRect(bounds.L, titleRect.MH() - kMeterH * 0.5f,
                             bounds.L + kMeterW, titleRect.MH() + kMeterH * 0.5f);
  const IRECT rightMeterRect(bounds.R - kMeterW, titleRect.MH() - kMeterH * 0.5f,
                              bounds.R, titleRect.MH() + kMeterH * 0.5f);

  graphics->AttachControl(new IPeakMeterControl(leftMeterRect,  &plugin->mInputPeak,  accentColor));
  graphics->AttachControl(new IPeakMeterControl(rightMeterRect, &plugin->mOutputPeak, secondaryAccent));

  // ---- Brand mark + Breathing LED (top-right) ----
  // Breathing LED: small circle at top-right corner
  constexpr float kLedSize = 10.f;
  const IRECT ledRect(bounds.R - kLedSize, bounds.T + 4.f,
                       bounds.R,            bounds.T + 4.f + kLedSize);
  graphics->AttachControl(new BreathingLEDControl(ledRect, accentColor));

  // Brand mark: "CLOPH · 1.0.0" right-aligned, just left of LED
  const IRECT brandRect(bounds.R - kLedSize - 120.f, bounds.T + 4.f,
                         bounds.R - kLedSize - 4.f,  bounds.T + 20.f);
  graphics->AttachControl(new ITextControl(brandRect,
                                           PLUG_MFR " \xb7 " PLUG_VERSION_STR,
                                           IText(kVersionSize, secondaryTextColor, uiFont,
                                                 EAlign::Far, EVAlign::Middle)));

  // ---- Custom SVG knob body (loaded once, shared by reference across 8 knobs) ----
  // Loaded from the embedded resource referenced by KNOB_BODY_FN. Per AGENTS.md
  // "iPlug2 resource embedding": ResourceCompile + EmbeddedResource entries in
  // vcxproj make the SVG bytes reachable via LoadSVG at runtime. The pointer
  // rotation is computed live per-knob in SVGKnobControl::Draw (no SVG with
  // a 'tracking layer' — simpler, smaller DLL, same visual outcome).
  const ISVG knobSvg = graphics->LoadSVG(KNOB_BODY_FN);

  // ---- Enum bars (Mode / Bits / Rate) ------------------------------------
  constexpr int kModeN = 4;
  constexpr int kBitsN = 7;
  constexpr int kRateN = 8;

  const char* modeLabels[kModeN] = {"RPT", "DROP", "RWD", "HYB"};
  const char* bitsLabels[kBitsN] = {"4", "6", "8", "10", "12", "14", "16"};
  const char* rateLabels[kRateN]  = {"x1", "x2", "x4", "x8", "x12", "x16", "x24", "x32"};

  const float barAreaTop = subtitleRect.B + 20.f;
  const float barWidth  = bounds.W();
  const float modeBarH  = 52.f;
  const float bitsBarH  = 52.f;
  const float rateBarH  = 52.f;
  const float barGap    = 10.f;

  const IRECT modeBarRect(bounds.L, barAreaTop,                         bounds.L + barWidth, barAreaTop + modeBarH);
  const IRECT bitsBarRect(bounds.L, modeBarRect.B + barGap,             bounds.L + barWidth, modeBarRect.B + barGap + bitsBarH);
  const IRECT rateBarRect(bounds.L, bitsBarRect.B + barGap,             bounds.L + barWidth, bitsBarRect.B + barGap + rateBarH);

  graphics->AttachControl(new EnumBarControl(modeBarRect, kMode,   modeLabels, kModeN,  "Choose the dominant corruption gesture.",             selectorStyle));
  graphics->AttachControl(new EnumBarControl(bitsBarRect, kBits,   bitsLabels, kBitsN, "Quantization depth for the corruption stage.",          selectorStyle));
  graphics->AttachControl(new EnumBarControl(rateBarRect, kRateReduce, rateLabels, kRateN, "Sample-hold reduction rate for stepped digital breakup.", selectorStyle));

  // ---- Knob rows -----------------------------------------------------------
  const float knobTop = rateBarRect.B + 24.f;
  const float knobSize = 96.f;
  const float knobGap = 18.f;
  const float rowWidth = knobSize * 4.f + knobGap * 3.f;
  const float rowLeft = bounds.MW() - rowWidth * 0.5f;

  const IRECT row1Col1(rowLeft, knobTop, rowLeft + knobSize, knobTop + knobSize);
  const IRECT row1Col2(row1Col1.R + knobGap, knobTop, row1Col1.R + knobGap + knobSize, knobTop + knobSize);
  const IRECT row1Col3(row1Col2.R + knobGap, knobTop, row1Col2.R + knobGap + knobSize, knobTop + knobSize);
  const IRECT row1Col4(row1Col3.R + knobGap, knobTop, row1Col3.R + knobGap + knobSize, knobTop + knobSize);

  const float row2Top = knobTop + knobSize + 24.f;
  const IRECT row2Col1(rowLeft, row2Top, rowLeft + knobSize, row2Top + knobSize);
  const IRECT row2Col2(row2Col1.R + knobGap, row2Top, row2Col1.R + knobGap + knobSize, row2Top + knobSize);
  const IRECT row2Col3(row2Col2.R + knobGap, row2Top, row2Col2.R + knobGap + knobSize, row2Top + knobSize);
  const IRECT row2Col4(row2Col3.R + knobGap, row2Top, row2Col3.R + knobGap + knobSize, row2Top + knobSize);

  auto* intensityKnob = new SVGKnobControl(row1Col1, knobSvg, kIntensity, knobStyle);
  intensityKnob->SetTooltip("How hard each glitch event diverges from the dry signal.");
  intensityKnob->SetParamName("Intensity");
  graphics->AttachControl(intensityKnob);

  auto* densityKnob = new SVGKnobControl(row1Col2, knobSvg, kDensity, knobStyle);
  densityKnob->SetTooltip("How often the engine schedules new corruption events.");
  densityKnob->SetParamName("Density");
  graphics->AttachControl(densityKnob);

  auto* windowKnob = new SVGKnobControl(row1Col3, knobSvg, kWindowMs, knobStyle);
  windowKnob->SetTooltip("Base duration of each repeat, dropout, or rewind slip.");
  windowKnob->SetParamName("Window");
  graphics->AttachControl(windowKnob);

  auto* rewindKnob = new SVGKnobControl(row1Col4, knobSvg, kRewindMs, knobStyle);
  rewindKnob->SetTooltip("How far back the rewind mode jumps into the history buffer.");
  rewindKnob->SetParamName("Rewind");
  graphics->AttachControl(rewindKnob);

  auto* jitterKnob = new SVGKnobControl(row2Col1, knobSvg, kJitter, knobStyle);
  jitterKnob->SetTooltip("Random variation applied to event timing, size, and offset.");
  jitterKnob->SetParamName("Jitter");
  graphics->AttachControl(jitterKnob);

  auto* toneKnob = new SVGKnobControl(row2Col2, knobSvg, kTone, knobStyle);
  toneKnob->SetTooltip("Brightness of the post-corruption filter.");
  toneKnob->SetParamName("Tone");
  graphics->AttachControl(toneKnob);

  auto* mixKnob = new SVGKnobControl(row2Col3, knobSvg, kMix, knobStyle);
  mixKnob->SetTooltip("Blend between the clean input and the corrupted wet path.");
  mixKnob->SetParamName("Mix");
  graphics->AttachControl(mixKnob);

  auto* outputKnob = new SVGKnobControl(row2Col4, knobSvg, kOutputTrim, knobStyle);
  outputKnob->SetTooltip("Final trim after the glitch engine and safety clipper.");
  outputKnob->SetParamName("Output");
  graphics->AttachControl(outputKnob);

  const IRECT footerRect(bounds.L, bounds.B - 24.f, bounds.R, bounds.B);
  graphics->AttachControl(new ITextControl(footerRect,
                                           "Repeat grabs the last fragment, Rewind slips to older audio, Dropout kills packets, Hybrid rotates between all three.",
                                           IText(kFooterSize, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle)));
}
}
