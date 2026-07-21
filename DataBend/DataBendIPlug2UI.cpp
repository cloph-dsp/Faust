#include "DataBendIPlug2UI.h"
#include "DataBend.h"
#include "IControls.h"
#include <cmath>

namespace
{
constexpr const char* kModeChoices[] = { "Repeat", "Dropout", "Rewind", "Hybrid" };
constexpr const char* kBitChoices[]  = { "4", "6", "8", "10", "12", "14", "16" };
constexpr const char* kRateChoices[] = { "1x", "2x", "4x", "8x", "12x", "16x", "24x", "32x" };
} // namespace

namespace databend::ui
{

} // namespace

::DataBend* sPlugin = nullptr;

namespace databend::ui
{

void BuildLayout(iplug::igraphics::IGraphics* graphics, void* plugin)
{
  using namespace iplug::igraphics;
  sPlugin = static_cast<::DataBend*>(plugin);

  const IColor bgColor(255, NeonColors::BG_VAL, NeonColors::BG_VAL, NeonColors::BG_VAL);
  const IColor panelColor(255, NeonColors::PANEL_VAL, NeonColors::PANEL_VAL, NeonColors::PANEL_VAL);

  graphics->AttachPanelBackground(bgColor);
  graphics->AttachControl(new ScanLinesControl(graphics->GetBounds()));

  const char* uiFont = "LayoutFont";
  if (!graphics->LoadFont("LayoutFont", LAYOUT_FN)) {
    if (!graphics->LoadFont("LayoutFont", "Segoe UI", ETextStyle::Normal)) {
      if (!graphics->LoadFont("LayoutFont", "Arial", ETextStyle::Normal)) {
        uiFont = "Verdana";
        graphics->LoadFont("LayoutFont", "Verdana", ETextStyle::Normal);
      }
    }
  }

  graphics->EnableMouseOver(true);
  graphics->EnableTooltips(true);
  graphics->AttachBubbleControl();
  graphics->AssignParamNameToolTips();

  const IRECT canvas = graphics->GetBounds();
  const float PAD = 36.f;
  const IRECT content(canvas.L + PAD, canvas.T + PAD,
                      canvas.R - PAD, canvas.B - PAD);

  const float ZONE_TITLE_H    = 80.f;
  const float ZONE_SELECTOR_H = 88.f;
  const float ZONE_KNOB_H     = 108.f;
  const float ZONE_KNOB_GAP   = 24.f;
  const float ZONE_FOOTER_H   = 28.f;
  const float ZONE_GAP_H      = 16.f;

  const float titleTop    = content.T;
  const float selectorTop = titleTop + ZONE_TITLE_H + 20.f;
  const float knob1Top   = selectorTop + ZONE_SELECTOR_H + ZONE_GAP_H;
  const float knob2Top   = knob1Top + ZONE_KNOB_H + ZONE_KNOB_GAP;
  const float footerTop  = knob2Top + ZONE_KNOB_H + 20.f;

  const float KNOB_SIZE  = 116.f;
  const float KNOB_GAP  = 20.f;
  const float KNOB_COL_W = KNOB_SIZE + KNOB_GAP;
  const float KNOB_ROW_W = KNOB_COL_W * 4.f - KNOB_GAP;
  const float KNOB_ROW_L = content.MW() - KNOB_ROW_W * 0.5f;

  const float SEL_GAP = 18.f;
  const float SEL_W = (content.W() - SEL_GAP * 2.f) / 3.f;
  const float selL1 = content.L;
  const float selL2 = selL1 + SEL_W + SEL_GAP;
  const float selL3 = selL2 + SEL_W + SEL_GAP;

  const IRECT modeRect(selL1, selectorTop, selL1 + SEL_W, selectorTop + ZONE_SELECTOR_H);
  const IRECT bitsRect(selL2, selectorTop, selL2 + SEL_W, selectorTop + ZONE_SELECTOR_H);
  const IRECT rateRect(selL3, selectorTop, selL3 + SEL_W, selectorTop + ZONE_SELECTOR_H);

  const IRECT r1c1(KNOB_ROW_L,           knob1Top, KNOB_ROW_L + KNOB_SIZE, knob1Top + ZONE_KNOB_H);
  const IRECT r1c2(r1c1.R + KNOB_GAP,    knob1Top, r1c1.R + KNOB_GAP + KNOB_SIZE, knob1Top + ZONE_KNOB_H);
  const IRECT r1c3(r1c2.R + KNOB_GAP,    knob1Top, r1c2.R + KNOB_GAP + KNOB_SIZE, knob1Top + ZONE_KNOB_H);
  const IRECT r1c4(r1c3.R + KNOB_GAP,    knob1Top, r1c3.R + KNOB_GAP + KNOB_SIZE, knob1Top + ZONE_KNOB_H);
  const IRECT r2c1(KNOB_ROW_L,           knob2Top, KNOB_ROW_L + KNOB_SIZE, knob2Top + ZONE_KNOB_H);
  const IRECT r2c2(r2c1.R + KNOB_GAP,    knob2Top, r2c1.R + KNOB_GAP + KNOB_SIZE, knob2Top + ZONE_KNOB_H);
  const IRECT r2c3(r2c2.R + KNOB_GAP,    knob2Top, r2c2.R + KNOB_GAP + KNOB_SIZE, knob2Top + ZONE_KNOB_H);
  const IRECT r2c4(r2c3.R + KNOB_GAP,    knob2Top, r2c3.R + KNOB_GAP + KNOB_SIZE, knob2Top + ZONE_KNOB_H);

  const IColor pinkAccent(255, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B);
  const IColor cyanAccent(255, NeonColors::NEON_CYAN_R, NeonColors::NEON_CYAN_G, NeonColors::NEON_CYAN_B);
  const IColor purpleAccent(255, NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B);
  const IColor warmText(255, NeonColors::TEXT_WARM_R, NeonColors::TEXT_WARM_G, NeonColors::TEXT_WARM_B);
  const IColor dimText(255, NeonColors::TEXT_DIM_R, NeonColors::TEXT_DIM_G, NeonColors::TEXT_DIM_B);

  const IVStyle knobStyle = DEFAULT_STYLE
    .WithColor(kBG, panelColor)
    .WithColor(kFG, IColor(255, 50, 55, 65))
    .WithColor(kPR, pinkAccent)
    .WithColor(kFR, IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL))
    .WithColor(kHL, cyanAccent)
    .WithColor(kX1, warmText)
    .WithLabelText(IText(13.f, dimText, uiFont, EAlign::Center, EVAlign::Top))
    .WithValueText(IText(15.f, warmText, uiFont, EAlign::Center, EVAlign::Bottom))
    .WithRoundness(0.20f)
    .WithDrawShadows(false);

  auto* titleCtrl = new NeonTitleControl(
    IRECT(content.L, titleTop, content.L + 280.f, titleTop + ZONE_TITLE_H),
    "DATABEND", uiFont);
  graphics->AttachControl(titleCtrl);
  if (sPlugin)
    titleCtrl->SetPeakRef(&sPlugin->mInputPeak);

  const float brandW = 130.f;
  const IRECT brandRect(content.R - brandW, titleTop, content.R, titleTop + 22.f);
  graphics->AttachControl(new ITextControl(brandRect, "CLOPH",
    IText(16.f, dimText, uiFont, EAlign::Far, EVAlign::Top)));

  const IRECT verRect(content.R - brandW, titleTop + 22.f, content.R, titleTop + 38.f);
  graphics->AttachControl(new ITextControl(verRect, "v1.0.0",
    IText(11.f, IColor(180, NeonColors::FRAME_VAL + 20, NeonColors::FRAME_VAL + 20, NeonColors::FRAME_VAL + 20), uiFont, EAlign::Far, EVAlign::Top)));

  const IRECT ledRect(content.R - 22.f, titleTop + 4.f, content.R - 4.f, titleTop + 22.f);
  graphics->AttachControl(new BreathingLedControl(ledRect));

  const float meterW = 8.f;
  const float meterH = 44.f;
  const IRECT inMeterL(content.L + 8.f, titleTop + 16.f, content.L + 8.f + meterW, titleTop + 16.f + meterH);
  const IRECT outMeterR(content.R - 28.f, titleTop + 16.f, content.R - 28.f + meterW, titleTop + 16.f + meterH);

  graphics->AttachControl(new NeonMeterControl(inMeterL,
    sPlugin ? &sPlugin->mInputPeak : nullptr));
  graphics->AttachControl(new NeonMeterControl(outMeterR,
    sPlugin ? &sPlugin->mOutputPeak : nullptr));

  const IRECT subtitleRect(content.L, titleTop + ZONE_TITLE_H - 8.f, content.R, titleTop + ZONE_TITLE_H + 14.f);
  graphics->AttachControl(new NeonSubtitleControl(subtitleRect,
    "Real-time packet loss, rewind slips, and crushed decoder damage."));

  const float visLeft = content.L + 340.f;
  const IRECT visRect(visLeft, titleTop + 10.f, content.R - 8.f, titleTop + ZONE_TITLE_H - 8.f);
  graphics->AttachControl(new WaveformSpectrumControl(visRect, uiFont));

  const IRECT glitchRect(content.L, selectorTop + ZONE_SELECTOR_H + 2.f, content.R, selectorTop + ZONE_SELECTOR_H + 2.f + ZONE_GAP_H - 4.f);
  graphics->AttachControl(new GlitchOverlayControl(glitchRect));

  auto* modeSel = new NeonSelectorControl(
    modeRect, kMode, "Mode",
    kModeChoices, 4,
    pinkAccent, IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL), warmText);
  modeSel->SetTooltip("Choose the dominant corruption gesture.");
  graphics->AttachControl(modeSel);

  auto* bitsSel = new NeonSelectorControl(
    bitsRect, kBits, "Bits",
    kBitChoices, 7,
    cyanAccent, IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL), warmText);
  bitsSel->SetTooltip("Quantization depth for the corruption stage.");
  graphics->AttachControl(bitsSel);

  auto* rateSel = new NeonSelectorControl(
    rateRect, kRateReduce, "Rate",
    kRateChoices, 8,
    purpleAccent, IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL), warmText);
  rateSel->SetTooltip("Sample-hold reduction rate for stepped digital breakup.");
  graphics->AttachControl(rateSel);

  auto* intensityKnob = new IVKnobControl(r1c1, kIntensity, "Intensity", knobStyle);
  intensityKnob->SetTooltip("How hard each glitch event diverges from the dry signal.");
  graphics->AttachControl(intensityKnob);

  auto* densityKnob = new IVKnobControl(r1c2, kDensity, "Density", knobStyle);
  densityKnob->SetTooltip("How often the engine schedules new corruption events.");
  graphics->AttachControl(densityKnob);

  auto* windowKnob = new IVKnobControl(r1c3, kWindowMs, "Window", knobStyle);
  windowKnob->SetTooltip("Base duration of each repeat, dropout, or rewind slip.");
  graphics->AttachControl(windowKnob);

  auto* rewindKnob = new IVKnobControl(r1c4, kRewindMs, "Rewind", knobStyle);
  rewindKnob->SetTooltip("How far back the rewind mode jumps into the history buffer.");
  graphics->AttachControl(rewindKnob);

  auto* jitterKnob = new IVKnobControl(r2c1, kJitter, "Jitter", knobStyle);
  jitterKnob->SetTooltip("Random variation applied to event timing, size, and offset.");
  graphics->AttachControl(jitterKnob);

  auto* toneKnob = new IVKnobControl(r2c2, kTone, "Tone", knobStyle);
  toneKnob->SetTooltip("Brightness of the post-corruption filter.");
  graphics->AttachControl(toneKnob);

  auto* mixKnob = new IVKnobControl(r2c3, kMix, "Mix", knobStyle);
  mixKnob->SetTooltip("Blend between the clean input and the corrupted wet path.");
  graphics->AttachControl(mixKnob);

  auto* outputKnob = new IVKnobControl(r2c4, kOutputTrim, "Output", knobStyle);
  outputKnob->SetTooltip("Final trim after the glitch engine and safety clipper.");
  graphics->AttachControl(outputKnob);

  const IRECT pktRect(content.L, knob2Top + 8.f,
                       content.R - PAD, footerTop - 8.f);
  graphics->AttachControl(new PacketStreamControl(pktRect, uiFont));

  const IRECT footerRect(content.L, footerTop, content.R, footerTop + ZONE_FOOTER_H);
  graphics->AttachControl(new ITextControl(footerRect,
    "Repeat grabs the last fragment  |  Rewind slips to older audio  |  Dropout kills packets  |  Hybrid blends all",
    IText(12.f, dimText, uiFont, EAlign::Center, EVAlign::Middle)));
}

} // namespace databend::ui
