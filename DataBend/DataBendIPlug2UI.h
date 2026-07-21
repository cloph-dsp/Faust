#pragma once

#include "IControls.h"
#include "DataBend.h"
#include <atomic>
#include <cmath>

extern ::DataBend* sPlugin;

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
  NeonTitleControl(const iplug::igraphics::IRECT& bounds, const char*, const char* font)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
    , mFont(font)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    using NC = NeonColors;
    const char* txt = "DATABEND";
    const float shift = 3.f + (mPeakRef ? mPeakRef->load(std::memory_order_relaxed) * 6.f : 0.f);
    const iplug::igraphics::IText baseStyle(52.f, iplug::igraphics::COLOR_WHITE, mFont,
                                            iplug::igraphics::EAlign::Center,
                                            iplug::igraphics::EVAlign::Middle);

    iplug::igraphics::IRECT pr = mRECT.GetTranslated(-shift, -shift * 0.6f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(255, NC::NEON_PINK_R, NC::NEON_PINK_G, NC::NEON_PINK_B)), txt, pr, nullptr);
    iplug::igraphics::IRECT cr = mRECT.GetTranslated(shift, shift * 0.6f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(255, NC::NEON_CYAN_R, NC::NEON_CYAN_G, NC::NEON_CYAN_B)), txt, cr, nullptr);
    iplug::igraphics::IRECT pgr = mRECT.GetTranslated(-shift * 2.f, 0.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(160, NC::NEON_PURPLE_R, NC::NEON_PURPLE_G, NC::NEON_PURPLE_B)), txt, pgr, nullptr);
    pgr = mRECT.GetTranslated(shift * 2.f, 0.f);
    g.DrawText(baseStyle.WithFGColor(iplug::igraphics::IColor(160, NC::NEON_PURPLE_R, NC::NEON_PURPLE_G, NC::NEON_PURPLE_B)), txt, pgr, nullptr);
    g.DrawText(baseStyle, txt, mRECT, nullptr);
  }

  void SetPeakRef(std::atomic<float>* ref) { mPeakRef = ref; }

private:
  const char* mFont;
  std::atomic<float>* mPeakRef = nullptr;
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

class GlitchOverlayControl final : public iplug::igraphics::IControl
{
public:
  GlitchOverlayControl(const iplug::igraphics::IRECT& bounds)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    if (!sPlugin)
      return;

    using namespace iplug::igraphics;
    const float intensity = static_cast<float>(sPlugin->GetParam(kIntensity)->Value());
    const float density   = static_cast<float>(sPlugin->GetParam(kDensity)->Value());
    const int mode       = static_cast<int>(sPlugin->GetParam(kMode)->Value());

    if (intensity < 0.01f && density < 0.01f)
      return;

    const float t = static_cast<float>(mTick++);
    const float active = (mActive > 0.f) ? 1.f : 0.f;

    if (active > 0.5f && intensity > 0.05f)
    {
      const int a = static_cast<int>(intensity * 180.f);
      IColor c;
      if (mode == 0)      c = IColor(a, NeonColors::NEON_CYAN_R, NeonColors::NEON_CYAN_G, NeonColors::NEON_CYAN_B);
      else if (mode == 1) c = IColor(a, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B);
      else if (mode == 2) c = IColor(a, NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B);
      else                c = IColor(a, 255, 255, 100);

      const float y0 = mRECT.T;
      const float y1 = mRECT.B;
      const float spacing = 22.f;
      for (float y = y0; y < y1; y += spacing)
      {
        const float ox = std::sin(y * 0.13f + t * 0.09f) * intensity * 25.f;
        const float len = mRECT.W() * (0.4f + 0.6f * intensity);
        g.DrawLine(c,
                   mRECT.L + ox + std::sin(t * 0.17f) * 5.f, y,
                   mRECT.L + ox + len + std::sin(t * 0.13f) * 3.f, y + 3.f,
                   nullptr, 1.f);
      }
    }

    const float nextGlitch = std::sin(t * 0.11f) * 0.5f + 0.5f;
    if (nextGlitch < density * 0.4f)
      mActive = 6.f;
    else if (mActive > 0.f)
      mActive -= 1.f;
  }

  void OnInit() override { mTick = 0; mActive = 0.f; }

private:
  int mTick;
  float mActive = 0.f;
};

class PacketStreamControl final : public iplug::igraphics::IControl
{
public:
  PacketStreamControl(const iplug::igraphics::IRECT& bounds, const char* font)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
    , mFont(font)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    using namespace iplug::igraphics;
    const float t = static_cast<float>(mTick++);

    g.FillRoundRect(IColor(20, 0, 0, 0), mRECT, 3.f, nullptr);

    const float barH = 9.f;
    const float gap = 4.f;
    const float lineH = barH + gap;
    const int lineCount = static_cast<int>((mRECT.H() - 4.f) / lineH);
    const float glyphW = 7.f;
    const int cols = static_cast<int>((mRECT.W() - 8.f) / glyphW);

    const float peak = (sPlugin && sPlugin->GetParam(kIntensity)) ?
      static_cast<float>(sPlugin->GetParam(kIntensity)->Value()) : 0.f;
    const int mode = (sPlugin && sPlugin->GetParam(kMode)) ?
      static_cast<int>(sPlugin->GetParam(kMode)->Value()) : 0;

    IColor streamCol;
    if (mode == 0)      streamCol = IColor(180, NeonColors::NEON_CYAN_R, NeonColors::NEON_CYAN_G, NeonColors::NEON_CYAN_B);
    else if (mode == 1) streamCol = IColor(180, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B);
    else if (mode == 2) streamCol = IColor(180, NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B);
    else                streamCol = IColor(180, 255, 200, 60);

    IColor dimCol(100, NeonColors::TEXT_DIM_R, NeonColors::TEXT_DIM_G, NeonColors::TEXT_DIM_B);
    IText streamTxt(9.f, streamCol, mFont, iplug::igraphics::EAlign::Near, iplug::igraphics::EVAlign::Top);
    IText dimTxt(9.f, dimCol, mFont, iplug::igraphics::EAlign::Near, iplug::igraphics::EVAlign::Top);

    for (int line = 0; line < lineCount; ++line)
    {
      const float lineY = mRECT.T + 4.f + line * lineH;
      const float scrollSpeed = (mode == 2) ? -0.7f : ((mode == 1) ? (std::sin(t * 0.12f) > 0.5f ? 0.f : 1.5f) : 0.9f);
      const float xOff = std::fmod((t * scrollSpeed * 8.f) * glyphW, mRECT.W());
      const bool glitchLine = (std::sin(t * 0.07f + static_cast<float>(line) * 2.3f) * 0.5f + 0.5f) < peak * 0.3f;

      char addr[16];
      snprintf(addr, sizeof(addr), "%04X:", line * 16);
      g.DrawText(dimTxt, addr, IRECT(mRECT.L + 4.f, lineY, mRECT.L + 44.f, lineY + barH), nullptr);

      char hex[80], ascii[40];
      int hi = 0, ai = 0;
      for (int c = 0; c < cols && c < 8; ++c)
      {
        const float x = mRECT.L + 48.f + c * glyphW - xOff;
        if (x < mRECT.L || x > mRECT.R - 4.f)
          continue;

        int val;
        if (glitchLine) {
          val = static_cast<int>(std::rand()) & 0xFF;
        } else {
          const float noise = std::sin(t * 0.03f + c * 0.7f + line * 1.1f) * 0.5f + 0.5f;
          val = (mode == 0) ? (0xA0 + ((c * 7 + line * 13) % 0x30)) :
                 (mode == 1) ? (0x80 + static_cast<int>(noise * 0x40)) :
                 (mode == 2) ? (0xC0 - ((c * 5 + line * 9) % 0x20)) :
                                (std::rand() & 0xFF);
        }

        char hexC[4];
        snprintf(hexC, sizeof(hexC), "%02X ", val);
        hex[hi++] = hexC[0];
        hex[hi++] = hexC[1];
        hex[hi++] = hexC[2];
        hex[hi++] = ' ';
        ascii[ai++] = (val >= 32 && val < 127) ? val : '.';
      }
      hex[hi] = '\0';
      ascii[ai] = '\0';

      if (glitchLine)
        g.FillRoundRect(IColor(30, NeonColors::NEON_PINK_R, 0, 0),
                        IRECT(mRECT.L + 4.f, lineY, mRECT.R - 4.f, lineY + barH), 2.f, nullptr);

      g.DrawText(streamTxt, hex, IRECT(mRECT.L + 48.f, lineY, mRECT.L + 180.f, lineY + barH), nullptr);

      IText ascTxt(9.f, IColor(100, 180, 180, 180), mFont, iplug::igraphics::EAlign::Near, iplug::igraphics::EVAlign::Top);
      g.DrawText(ascTxt, ascii, IRECT(mRECT.L + 184.f, lineY, mRECT.R - 4.f, lineY + barH), nullptr);
    }

    IRECT headerRect(mRECT.L, mRECT.T, mRECT.R, mRECT.T + 14.f);
    g.FillRect(IColor(12, 0, 0, 0), headerRect, nullptr);
    IText hdrTxt(8.f, dimCol, mFont, iplug::igraphics::EAlign::Near, iplug::igraphics::EVAlign::Top);
    g.DrawText(hdrTxt, "PKT_STREAM", headerRect, nullptr);
    g.DrawRect(IColor(NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B),
               IRECT(mRECT.L, mRECT.T + 14.f, mRECT.R, mRECT.T + 15.f), nullptr);
  }

  void OnInit() override { mTick = 0; }

private:
  const char* mFont;
  int mTick;
};

class WaveformSpectrumControl final : public iplug::igraphics::IControl
{
public:
  WaveformSpectrumControl(const iplug::igraphics::IRECT& bounds, const char* font)
    : iplug::igraphics::IControl(bounds, iplug::kNoParameter)
    , mFont(font)
  {
    DisablePrompt(true);
  }

  void Draw(iplug::igraphics::IGraphics& g) override
  {
    if (!sPlugin)
      return;

    using namespace iplug::igraphics;
    g.FillRoundRect(IColor(12, 0, 0, 0), mRECT, 2.f, nullptr);

    const float t = static_cast<float>(mTick++);
    const int mode = static_cast<int>(sPlugin->GetParam(kMode)->Value());
    const float intensity = static_cast<float>(sPlugin->GetParam(kIntensity)->Value());
    const float density = static_cast<float>(sPlugin->GetParam(kDensity)->Value());

    const float specH = mRECT.H() * 0.55f;
    const float specT = mRECT.T + 4.f;
    const int nBars = 32;
    const float barW = (mRECT.W() - 4.f) / static_cast<float>(nBars);
    const float gap = 1.f;
    const float drawBarW = barW - gap;

    const float scrollSpeed = (mode == 0) ? 0.08f : (mode == 1) ? 0.22f : (mode == 2) ? -0.12f : 0.18f;
    const float tOff = t * scrollSpeed;

    for (int i = 0; i < nBars; ++i)
    {
      const float pos = static_cast<float>(i) / static_cast<float>(nBars);
      float barH;

      if (mode == 3)
      {
        barH = specH * (0.08f + 0.6f * static_cast<float>(std::sin(t * 0.4f + pos * 8.f) * 0.5f + 0.5f) * (0.3f + intensity * 0.7f));
      }
      else if (mode == 2)
      {
        barH = specH * (0.05f + 0.5f * static_cast<float>(std::sin((1.f - pos) * 7.f + tOff) * 0.5f + 0.5f) * (0.4f + intensity * 0.6f));
      }
      else
      {
        barH = specH * (0.1f + 0.5f * static_cast<float>(std::sin(pos * 7.f + tOff) * 0.5f + 0.5f) * (0.5f + intensity * 0.5f) * (0.7f + 0.3f * density));
      }

      if (mode == 1 && std::sin(t * 0.3f + pos * 5.f) > 0.5f)
        barH *= 0.15f;

      const float x = mRECT.L + 2.f + static_cast<float>(i) * barW;

      IColor barCol;
      if (barH < specH * 0.3f)
        barCol = IColor(220, NeonColors::NEON_CYAN_R, NeonColors::NEON_CYAN_G, NeonColors::NEON_CYAN_B);
      else if (barH < specH * 0.65f)
        barCol = IColor(220, NeonColors::NEON_PURPLE_R, NeonColors::NEON_PURPLE_G, NeonColors::NEON_PURPLE_B);
      else
        barCol = IColor(220, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B);

      const float barY = specT + specH - barH;

      g.FillRect(IColor(barCol.A / 4, barCol.R, barCol.G, barCol.B),
                 IRECT(x - 1.f, barY - 1.f, x + drawBarW + 1.f, specT + specH + 1.f), nullptr);
      g.FillRect(barCol, IRECT(x, barY, x + drawBarW, specT + specH), nullptr);

      if (barH > 3.f)
        g.FillRect(IColor(barCol.A, barCol.R, barCol.G, barCol.B),
                   IRECT(x, barY, x + drawBarW, barY + 2.f), nullptr);
    }

    const float waveT = specT + specH + 6.f;
    const float waveH = mRECT.B - waveT - 4.f;
    const float waveY = waveT + waveH * 0.5f;

    g.DrawRect(IColor(25, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL, NeonColors::FRAME_VAL),
               IRECT(mRECT.L + 2.f, waveT, mRECT.R - 2.f, waveT + 1.f), nullptr);

    const int nPts = static_cast<int>(mRECT.W() - 8.f);
    const uint32_t wIdx = sPlugin->mWaveformWriteIdx.load(std::memory_order_relaxed);
    const int bufSize = sPlugin->kWaveformSize;

    const float xScale = (mRECT.W() - 8.f) / static_cast<float>(nPts);

    float prevX = mRECT.L + 4.f;
    float prevY = waveY;
    for (int i = 0; i < nPts; ++i)
    {
      const float x = mRECT.L + 4.f + static_cast<float>(i) * xScale;

      const int histIdx = (mode == 1 && std::sin(t * 0.4f + i * 0.3f) > 0.4f) ? 0 :
        (static_cast<int>(wIdx) - nPts + i + bufSize) % bufSize;
      const float val = sPlugin->mWaveformHistory[histIdx].load(std::memory_order_relaxed);
      const float clampedVal = std::max(-1.f, std::min(1.f, val));
      const float y = waveY - clampedVal * waveH * 0.45f;

      if (i > 0)
      {
        g.DrawLine(IColor(200, NeonColors::NEON_PINK_R, NeonColors::NEON_PINK_G, NeonColors::NEON_PINK_B),
                   prevX, prevY, x, y, nullptr, 1.5f);
        g.DrawLine(IColor(60, NeonColors::NEON_PINK_R / 3, NeonColors::NEON_PINK_G / 3, NeonColors::NEON_PINK_B / 3),
                   prevX, prevY - 1.f, x, y - 1.f, nullptr, 0.5f);
      }

      prevX = x;
      prevY = y;
    }

    g.DrawRoundRect(IColor(NeonColors::FRAME_VAL, 0, 0, 0), mRECT, 2.f, nullptr, 0.5f);
  }

  void OnInit() override { mTick = 0; }

private:
  const char* mFont;
  int mTick = 0;
};

} // namespace databend::ui
