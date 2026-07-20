#pragma once

#include "IControls.h"
#include <atomic>
#include <cmath>

namespace databend::ui
{

struct NeonColors
{
  static constexpr int BG_VAL      = 10;
  static constexpr int PANEL_VAL   = 18;
  static constexpr int FRAME_VAL   = 38;

  static constexpr int NEON_PINK_R   = 255, NEON_PINK_G   = 45,  NEON_PINK_B   = 149;
  static constexpr int NEON_CYAN_R  = 0,   NEON_CYAN_G   = 240, NEON_CYAN_B   = 255;
  static constexpr int NEON_PURPLE_R = 157, NEON_PURPLE_G = 78,  NEON_PURPLE_B = 221;

  static constexpr int TEXT_WARM_R  = 255, TEXT_WARM_G  = 210, TEXT_WARM_B  = 195;
  static constexpr int TEXT_DIM_R    = 140, TEXT_DIM_G    = 155, TEXT_DIM_B    = 170;
};

void BuildLayout(iplug::igraphics::IGraphics* graphics, void* plugin);

class NeonTitleControl final : public iplug::igraphics::IControl
{
public:
  NeonTitleControl(const iplug::igraphics::IRECT& bounds, const char*)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    using NC = NeonColors;
    const char* txt = "DATABEND";
    const iplug::igraphics::IText baseStyle(52.f, iplug::igraphics::COLOR_WHITE, nullptr,
                                            iplug::igraphics::EAlign::Center,
                                            iplug::igraphics::EVAlign::Middle);

    iplug::igraphics::IRECT pr = mRECT.GetTranslated(-3.f, -2.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(255, NC::NEON_PINK_R, NC::NEON_PINK_G, NC::NEON_PINK_B)), txt, pr, nullptr);
    iplug::igraphics::IRECT cr = mRECT.GetTranslated(3.f, 2.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(255, NC::NEON_CYAN_R, NC::NEON_CYAN_G, NC::NEON_CYAN_B)), txt, cr, nullptr);
    iplug::igraphics::IRECT pgr = mRECT.GetTranslated(-6.f, 0.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(160, NC::NEON_PURPLE_R, NC::NEON_PURPLE_G, NC::NEON_PURPLE_B)), txt, pgr, nullptr);
    pgr = mRECT.GetTranslated(6.f, 0.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(160, NC::NEON_PURPLE_R, NC::NEON_PURPLE_G, NC::NEON_PURPLE_B)), txt, pgr, nullptr);
    g.DrawText(baseStyle, txt, mRECT, nullptr);
  }
};

class ScanLinesControl final : public iplug::igraphics::IControl
{
public:
  ScanLinesControl(const iplug::igraphics::IRECT& bounds)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    for (float y = static_cast<float>(mRECT.T); y < mRECT.B; y += 3.f)
      g.DrawLine(iplug::igraphics::IColor(22, 0, 0, 0),
                 static_cast<float>(mRECT.L), y,
                 static_cast<float>(mRECT.R), y, nullptr, 1.f);
  }
};

class NeonSelectorControl final : public iplug::igraphics::IControl,
                                 public iplug::igraphics::IVectorBase
{
public:
  NeonSelectorControl(const iplug::igraphics::IRECT& bounds, int paramIdx, const char* label,
                     const char* const* choices, int nChoices,
                     iplug::igraphics::IColor activeColor,
                     iplug::igraphics::IColor frameColor,
                     iplug::igraphics::IColor labelColor)
    : iplug::igraphics::IControl(bounds, paramIdx)
    , iplug::igraphics::IVectorBase(
        iplug::igraphics::DEFAULT_STYLE
          .WithColor(iplug::igraphics::kBG, frameColor)
          .WithColor(iplug::igraphics::kFR, frameColor)
          .WithLabelText(iplug::igraphics::IText(13.f, labelColor, nullptr,
                                                 iplug::igraphics::EAlign::Center,
                                                 iplug::igraphics::EVAlign::Middle))
          .WithValueText(iplug::igraphics::IText(14.f,
                                                 iplug::igraphics::IColor(255, NeonColors::TEXT_WARM_R,
                                                                          NeonColors::TEXT_WARM_G,
                                                                          NeonColors::TEXT_WARM_B),
                                                 nullptr,
                                                 iplug::igraphics::EAlign::Center,
                                                 iplug::igraphics::EVAlign::Middle)),
        false, true)
    , mChoices(choices)
    , mNChoices(nChoices)
    , mActiveColor(activeColor)
  {
    AttachIControl(this, label);
    DisablePrompt(true);
    SetPromptShowsParamLabel(false);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    if (!GetParam())
      return;

    using namespace iplug::igraphics;
    const int val = static_cast<int>(GetValue());
    const int n = mNChoices;
    if (n <= 0)
      return;

    const float totalW = mRECT.W();
    const float cellGap = 3.f;
    const float cellW = (totalW - cellGap * static_cast<float>(n - 1)) / static_cast<float>(n);
    const float cellH = mRECT.H() - 28.f;
    const float valH = 22.f;
    const float topY = mRECT.T;

    for (int i = 0; i < n; ++i)
    {
      const float cx = mRECT.L + i * (cellW + cellGap);
      const IRECT cr(cx, topY, cx + cellW, topY + cellH);
      const bool active = (i == val);

      if (active)
      {
        g.FillRoundRect(IColor(30, mActiveColor.R, mActiveColor.G, mActiveColor.B), cr.GetPadded(4.f), 4.f, nullptr);
        g.FillRoundRect(IColor(80, mActiveColor.R, mActiveColor.G, mActiveColor.B), cr, 3.f, nullptr);
        g.DrawRoundRect(mActiveColor, cr, 3.f, nullptr, 1.5f);
      }
      else
      {
        g.FillRoundRect(IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL), cr, 3.f, nullptr);
        g.DrawRoundRect(IColor(180, 30, 30, 30), cr, 3.f, nullptr, 0.8f);
      }

      g.DrawText(IText(12.f,
                       active ? COLOR_WHITE
                              : IColor(255, NeonColors::TEXT_DIM_R, NeonColors::TEXT_DIM_G, NeonColors::TEXT_DIM_B),
                       nullptr, EAlign::Center, EVAlign::Middle),
                  mChoices[i], cr, nullptr);
    }

    WDL_String buf;
    GetParam()->GetDisplay(buf, false);
    const IRECT valRect(mRECT.L, topY + cellH + 4.f, mRECT.R, topY + cellH + valH);
    g.DrawText(mStyle.valueText, buf.Get(), valRect, nullptr);

    const IRECT lblRect(mRECT.L, topY + cellH + valH - 2.f, mRECT.R, mRECT.B);
    g.DrawText(mStyle.labelText, GetLabelStr(), lblRect, nullptr);
  }

  void OnMouseDown(float x, float, const iplug::igraphics::IMouseMod&) override
  {
    if (!GetParam() || IsDisabled())
      return;

    const int n = mNChoices;
    if (n <= 0)
      return;

    const float totalW = mRECT.W();
    const float cellGap = 3.f;
    const float cellW = (totalW - cellGap * static_cast<float>(n - 1)) / static_cast<float>(n);

    int cell = static_cast<int>((x - mRECT.L) / (cellW + cellGap));
    cell = std::max(0, std::min(n - 1, cell));

    GetDelegate()->BeginInformHostOfParamChangeFromUI(GetParamIdx());
    SetValueFromUserInput(static_cast<double>(cell));
    GetDelegate()->EndInformHostOfParamChangeFromUI(GetParamIdx());
  }

  void OnInit() override
  {
    if (GetParam())
      SetDirty(false);
  }

private:
  const char* const* mChoices;
  int mNChoices;
  iplug::igraphics::IColor mActiveColor;
};

class BreathingLedControl final : public iplug::igraphics::IControl
{
public:
  BreathingLedControl(const iplug::igraphics::IRECT& bounds)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    using NC = NeonColors;
    const double phase = static_cast<double>(mTick++) * 0.04;
    const float brightness = 0.3f + 0.7f * static_cast<float>(0.5 + 0.5 * std::sin(phase));
    const int a = static_cast<int>(brightness * 255.f);

    g.FillEllipse(iplug::igraphics::IColor(a / 3, NC::NEON_PINK_R, NC::NEON_PINK_G, NC::NEON_PINK_B),
                  mRECT.GetPadded(3.f), nullptr);
    g.FillEllipse(iplug::igraphics::IColor(a, NC::NEON_PINK_R, NC::NEON_PINK_G, NC::NEON_PINK_B),
                  mRECT, nullptr);
  }

private:
  int mTick = 0;
};

class NeonMeterControl final : public iplug::igraphics::IControl
{
public:
  NeonMeterControl(const iplug::igraphics::IRECT& bounds, std::atomic<float>* peakRef)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
    , mPeakRef(peakRef)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    if (!mPeakRef)
      return;

    using namespace iplug::igraphics;
    const float peak = mPeakRef->load(std::memory_order_relaxed);
    const float norm = std::max(0.f, std::min(1.f, peak));
    const float fillW = mRECT.W() * norm;

    g.FillRoundRect(IColor(255, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL),
                    mRECT, 2.f, nullptr);

    if (fillW > 1.f)
    {
      const IRECT fill(mRECT.L, mRECT.T, mRECT.L + fillW, mRECT.B);
      IColor fc;
      if (norm < 0.7f)
        fc = IColor(255, NeonColors::NEON_CYAN_R, NeonColors::NEON_CYAN_G, NeonColors::NEON_CYAN_B);
      else if (norm < 0.9f)
        fc = IColor(255, NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B);
      else
        fc = IColor(255, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B);

      g.FillRoundRect(fc, fill, 2.f, nullptr);
      g.FillRoundRect(IColor(60, fc.R, fc.G, fc.B), fill.GetPadded(1.f), 3.f, nullptr);
    }
  }

private:
  std::atomic<float>* mPeakRef;
};

class NeonSubtitleControl final : public iplug::igraphics::ITextControl
{
public:
  NeonSubtitleControl(const iplug::igraphics::IRECT& bounds, const char* text)
    : iplug::igraphics::ITextControl(bounds, text,
                                     iplug::igraphics::IText(13.f,
                                     iplug::igraphics::IColor(255, NeonColors::TEXT_DIM_R,
                                                              NeonColors::TEXT_DIM_G,
                                                              NeonColors::TEXT_DIM_B),
                                     nullptr,
                                     iplug::igraphics::EAlign::Near,
                                     iplug::igraphics::EVAlign::Middle))
  {
    DisablePrompt(true);
  }
};

} // namespace databend::ui
