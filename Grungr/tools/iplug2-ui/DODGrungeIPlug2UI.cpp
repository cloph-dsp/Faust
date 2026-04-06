#include "DODGrungeIPlug2UI.h"

#include <algorithm>

namespace dodgrunge {
namespace ui {
namespace {

static constexpr const char* kMainFontID = "NoMoreHeroMain";
static constexpr const char* kSecondaryFontID = "FutureErodedSecondary";

enum ECtrlTags {
  kTagBackground = 1000,
  kTagTitle,
  kTagSubtitle,
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
  kTagBypassToggle,
  kTagEnhancedToggle
};

struct LayoutRects {
  igraphics::IRECT background;
  igraphics::IRECT title;
  igraphics::IRECT subtitle;

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
  igraphics::IRECT enhancedToggle;
};

igraphics::IRECT CenteredRect(const igraphics::IRECT& b, float nx, float ny, float w, float h)
{
  const float cx = b.L + (b.W() * nx);
  const float cy = b.T + (b.H() * ny);
  return {cx - (w * 0.5f), cy - (h * 0.5f), cx + (w * 0.5f), cy + (h * 0.5f)};
}

LayoutRects MakeLayout(const igraphics::IRECT& b)
{
  LayoutRects r;
  r.background = b;

  const float knobSize = std::min(b.W() * 0.27f, b.H() * 0.2f);
  const float labelH = std::max(22.f, knobSize * 0.16f);
  const float valueH = std::max(20.f, knobSize * 0.14f);
  const float labelPad = std::max(6.f, knobSize * 0.06f);

  r.title = CenteredRect(b, 0.5f, 0.105f, b.W() * 0.9f, b.H() * 0.09f);
  r.subtitle = CenteredRect(b, 0.5f, 0.165f, b.W() * 0.75f, b.H() * 0.045f);

  r.grungeKnob = CenteredRect(b, 0.3f, 0.42f, knobSize, knobSize);
  r.buttKnob = CenteredRect(b, 0.7f, 0.42f, knobSize, knobSize);
  r.faceKnob = CenteredRect(b, 0.3f, 0.63f, knobSize, knobSize);
  r.loudKnob = CenteredRect(b, 0.7f, 0.63f, knobSize, knobSize);

  const float labelW = knobSize * 1.35f;
  const float valueW = knobSize * 1.35f;

  auto makeLabel = [&](const igraphics::IRECT& knob) {
    return igraphics::IRECT(knob.MW() - (labelW * 0.5f), knob.B + labelPad, knob.MW() + (labelW * 0.5f), knob.B + labelPad + labelH);
  };

  auto makeValue = [&](const igraphics::IRECT& label) {
    return igraphics::IRECT(label.MW() - (valueW * 0.5f), label.B + 1.f, label.MW() + (valueW * 0.5f), label.B + 1.f + valueH);
  };

  r.grungeLabel = makeLabel(r.grungeKnob);
  r.buttLabel = makeLabel(r.buttKnob);
  r.faceLabel = makeLabel(r.faceKnob);
  r.loudLabel = makeLabel(r.loudKnob);

  r.grungeValue = makeValue(r.grungeLabel);
  r.buttValue = makeValue(r.buttLabel);
  r.faceValue = makeValue(r.faceLabel);
  r.loudValue = makeValue(r.loudLabel);

  const float toggleW = b.W() * 0.28f;
  const float toggleH = std::max(48.f, b.H() * 0.08f);
  r.bypassToggle = CenteredRect(b, 0.3f, 0.86f, toggleW, toggleH);
  r.enhancedToggle = CenteredRect(b, 0.7f, 0.86f, toggleW, toggleH);

  return r;
}

void SetBoundsIfPresent(igraphics::IGraphics& g, int tag, const igraphics::IRECT& bounds)
{
  if (auto* pControl = g.GetControlWithTag(tag)) {
    pControl->SetTargetAndDrawRECTs(bounds);
  }
}

void Relayout(igraphics::IGraphics& g, const LayoutRects& layout)
{
  SetBoundsIfPresent(g, kTagBackground, layout.background);
  SetBoundsIfPresent(g, kTagTitle, layout.title);
  SetBoundsIfPresent(g, kTagSubtitle, layout.subtitle);

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
  SetBoundsIfPresent(g, kTagEnhancedToggle, layout.enhancedToggle);
}

void LoadFonts(igraphics::IGraphics& g, const Assets& assets)
{
  g.LoadFont(kMainFontID, assets.mainFont);

  WDL_TypedBuf<uint8_t> secondaryFontData = g.LoadResource(assets.secondaryFont, "otf");

  if (secondaryFontData.GetSize() > 0) {
    g.LoadFont(kSecondaryFontID,
               static_cast<void*>(secondaryFontData.Get()),
               secondaryFontData.GetSize());
  }
  else {
    g.LoadFont(kSecondaryFontID, assets.secondaryFont);
  }
}

void AttachKnobWithText(igraphics::IGraphics& g,
                        const igraphics::ISVG& knobSVG,
                        int paramIdx,
                        const char* label,
                        const igraphics::IRECT& knobRect,
                        const igraphics::IRECT& labelRect,
                        const igraphics::IRECT& valueRect,
                        int knobTag,
                        int labelTag,
                        int valueTag)
{
  using namespace igraphics;

  auto* pKnob = new ISVGKnobControl(knobRect, knobSVG, paramIdx);
  pKnob->SetStartAngle(-135.f);
  pKnob->SetEndAngle(135.f);
  pKnob->SetTooltip(label);
  g.AttachControl(pKnob, knobTag);

  const IText labelText(22.f,
                        IColor(255, 235, 225, 205),
                        kSecondaryFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  const IText valueText(19.f,
                        IColor(255, 250, 245, 235),
                        kSecondaryFontID,
                        EAlign::Center,
                        EVAlign::Middle);

  g.AttachControl(new ITextControl(labelRect, label, labelText, COLOR_TRANSPARENT), labelTag);
  g.AttachControl(new ICaptionControl(valueRect, paramIdx, valueText, COLOR_TRANSPARENT, true), valueTag);
}

}  // namespace

void BuildOrRelayout(igraphics::IGraphics* pGraphics,
                     const Params& params,
                     const Assets& assets)
{
  if (!pGraphics) {
    return;
  }

  const LayoutRects layout = MakeLayout(pGraphics->GetBounds());

  if (pGraphics->GetControlWithTag(kTagBackground)) {
    Relayout(*pGraphics, layout);
    return;
  }

  LoadFonts(*pGraphics, assets);

  const igraphics::ISVG backgroundSVG = pGraphics->LoadSVG(assets.backgroundSVG);
  const igraphics::ISVG knobSVG = pGraphics->LoadSVG(assets.knobSVG);

  pGraphics->AttachControl(new igraphics::ISVGControl(layout.background, backgroundSVG, true), kTagBackground);

  const igraphics::IText titleText(62.f,
                                   igraphics::IColor(255, 246, 240, 232),
                                   kMainFontID,
                                   igraphics::EAlign::Center,
                                   igraphics::EVAlign::Middle);

  const igraphics::IText subtitleText(24.f,
                                      igraphics::IColor(255, 230, 220, 200),
                                      kSecondaryFontID,
                                      igraphics::EAlign::Center,
                                      igraphics::EVAlign::Middle);

  pGraphics->AttachControl(new igraphics::ITextControl(layout.title,
                                                       "DOD GRUNGE",
                                                       titleText,
                                                       igraphics::COLOR_TRANSPARENT),
                           kTagTitle);

  pGraphics->AttachControl(new igraphics::ITextControl(layout.subtitle,
                                                       "DIGITAL MODERN DRIVE",
                                                       subtitleText,
                                                       igraphics::COLOR_TRANSPARENT),
                           kTagSubtitle);

  AttachKnobWithText(*pGraphics,
                     knobSVG,
                     params.grunge,
                     "GRUNGE",
                     layout.grungeKnob,
                     layout.grungeLabel,
                     layout.grungeValue,
                     kTagGrungeKnob,
                     kTagGrungeLabel,
                     kTagGrungeValue);

  AttachKnobWithText(*pGraphics,
                     knobSVG,
                     params.butt,
                     "BUTT",
                     layout.buttKnob,
                     layout.buttLabel,
                     layout.buttValue,
                     kTagButtKnob,
                     kTagButtLabel,
                     kTagButtValue);

  AttachKnobWithText(*pGraphics,
                     knobSVG,
                     params.face,
                     "FACE",
                     layout.faceKnob,
                     layout.faceLabel,
                     layout.faceValue,
                     kTagFaceKnob,
                     kTagFaceLabel,
                     kTagFaceValue);

  AttachKnobWithText(*pGraphics,
                     knobSVG,
                     params.loud,
                     "LOUD",
                     layout.loudKnob,
                     layout.loudLabel,
                     layout.loudValue,
                     kTagLoudKnob,
                     kTagLoudLabel,
                     kTagLoudValue);

  const igraphics::IVStyle toggleStyle = igraphics::DEFAULT_STYLE
                                           .WithColor(igraphics::kBG, igraphics::IColor(80, 20, 17, 16))
                                           .WithColor(igraphics::kFG, igraphics::IColor(255, 232, 223, 205))
                                           .WithColor(igraphics::kPR, igraphics::IColor(255, 245, 95, 45))
                                           .WithColor(igraphics::kFR, igraphics::IColor(200, 190, 180, 165))
                                           .WithLabelText(igraphics::IText(20.f,
                                                                           igraphics::IColor(255, 233, 222, 198),
                                                                           kSecondaryFontID,
                                                                           igraphics::EAlign::Center,
                                                                           igraphics::EVAlign::Middle))
                                           .WithValueText(igraphics::IText(18.f,
                                                                           igraphics::IColor(255, 247, 238, 224),
                                                                           kSecondaryFontID,
                                                                           igraphics::EAlign::Center,
                                                                           igraphics::EVAlign::Middle))
                                           .WithRoundness(0.15f)
                                           .WithFrameThickness(2.f);

  pGraphics->AttachControl(new igraphics::IVToggleControl(layout.bypassToggle,
                                                           params.bypass,
                                                           "BYPASS",
                                                           toggleStyle,
                                                           "OFF",
                                                           "ON"),
                           kTagBypassToggle);

  pGraphics->AttachControl(new igraphics::IVToggleControl(layout.enhancedToggle,
                                                           params.enhanced,
                                                           "ENHANCED",
                                                           toggleStyle,
                                                           "STOCK",
                                                           "MODERN"),
                           kTagEnhancedToggle);
}

}  // namespace ui
}  // namespace dodgrunge
