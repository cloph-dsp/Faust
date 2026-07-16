#include "DataBendIPlug2UI.h"

#include <array>

namespace
{
constexpr float kTitleSize = 36.f;
constexpr float kSubtitleSize = 16.f;
constexpr float kFooterSize = 16.f;
constexpr float kKnobLabelSize = 16.f;
constexpr float kKnobValueSize = 18.f;
constexpr float kPopupMenuSize = 16.f;
}

namespace databend::ui
{
void BuildLayout(IGraphics* graphics)
{
  const IColor backgroundColor(255, 14, 15, 18);
  const IColor panelColor(255, 24, 27, 33);
  const IColor frameColor(255, 72, 80, 90);
  const IColor accentColor(255, 219, 123, 73);
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
  const IRECT titleRect(bounds.L, bounds.T + 4.f, bounds.R, bounds.T + 56.f);
  const IRECT subtitleRect(bounds.L, titleRect.B - 2.f, bounds.R, titleRect.B + 28.f);

  graphics->AttachControl(new ITextControl(titleRect,
                                           "DATABEND",
                                           IText(kTitleSize, accentColor, uiFont, EAlign::Near, EVAlign::Middle)));

  graphics->AttachControl(new ITextControl(subtitleRect,
                                           "Real-time packet loss, rewind slips, and crushed decoder damage.",
                                           IText(kSubtitleSize, secondaryTextColor, uiFont, EAlign::Near, EVAlign::Middle)));

  const float gap = 16.f;
  const float selectorTop = subtitleRect.B + 20.f;
  const float selectorHeight = 68.f;
  const float selectorWidth = (bounds.W() - gap * 2.f) / 3.f;

  const IRECT modeRect(bounds.L, selectorTop, bounds.L + selectorWidth, selectorTop + selectorHeight);
  const IRECT bitsRect(modeRect.R + gap, selectorTop, modeRect.R + gap + selectorWidth, selectorTop + selectorHeight);
  const IRECT rateRect(bitsRect.R + gap, selectorTop, bitsRect.R + gap + selectorWidth, selectorTop + selectorHeight);

  auto* modeSelector = new EnumSelectorControl(modeRect, kMode, "Mode", selectorStyle);
  modeSelector->SetTooltip("Choose the dominant corruption gesture.");
  graphics->AttachControl(modeSelector);

  auto* bitsSelector = new EnumSelectorControl(bitsRect, kBits, "Bits", selectorStyle);
  bitsSelector->SetTooltip("Quantization depth for the corruption stage.");
  graphics->AttachControl(bitsSelector);

  auto* rateSelector = new EnumSelectorControl(rateRect, kRateReduce, "Rate", selectorStyle);
  rateSelector->SetTooltip("Sample-hold reduction rate for stepped digital breakup.");
  graphics->AttachControl(rateSelector);

  const float knobTop = selectorTop + selectorHeight + 24.f;
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

  auto* intensityKnob = new IVKnobControl(row1Col1, kIntensity, "Intensity", knobStyle);
  intensityKnob->SetTooltip("How hard each glitch event diverges from the dry signal.");
  graphics->AttachControl(intensityKnob);

  auto* densityKnob = new IVKnobControl(row1Col2, kDensity, "Density", knobStyle);
  densityKnob->SetTooltip("How often the engine schedules new corruption events.");
  graphics->AttachControl(densityKnob);

  auto* windowKnob = new IVKnobControl(row1Col3, kWindowMs, "Window", knobStyle);
  windowKnob->SetTooltip("Base duration of each repeat, dropout, or rewind slip.");
  graphics->AttachControl(windowKnob);

  auto* rewindKnob = new IVKnobControl(row1Col4, kRewindMs, "Rewind", knobStyle);
  rewindKnob->SetTooltip("How far back the rewind mode jumps into the history buffer.");
  graphics->AttachControl(rewindKnob);

  auto* jitterKnob = new IVKnobControl(row2Col1, kJitter, "Jitter", knobStyle);
  jitterKnob->SetTooltip("Random variation applied to event timing, size, and offset.");
  graphics->AttachControl(jitterKnob);

  auto* toneKnob = new IVKnobControl(row2Col2, kTone, "Tone", knobStyle);
  toneKnob->SetTooltip("Brightness of the post-corruption filter.");
  graphics->AttachControl(toneKnob);

  auto* mixKnob = new IVKnobControl(row2Col3, kMix, "Mix", knobStyle);
  mixKnob->SetTooltip("Blend between the clean input and the corrupted wet path.");
  graphics->AttachControl(mixKnob);

  auto* outputKnob = new IVKnobControl(row2Col4, kOutputTrim, "Output", knobStyle);
  outputKnob->SetTooltip("Final trim after the glitch engine and safety clipper.");
  graphics->AttachControl(outputKnob);

  const IRECT footerRect(bounds.L, bounds.B - 24.f, bounds.R, bounds.B);
  graphics->AttachControl(new ITextControl(footerRect,
                                           "Repeat grabs the last fragment, Rewind slips to older audio, Dropout kills packets, Hybrid rotates between all three.",
                                           IText(kFooterSize, secondaryTextColor, uiFont, EAlign::Center, EVAlign::Middle)));
}
}
