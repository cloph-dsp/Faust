#pragma once

#include "IControl.h"
#include "ISender.h"
#include "IPlugStructs.h"
#include "IGraphicsStructs.h"

#include <algorithm>

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

class IVDotsDisplayControl : public IControl
                     , public IVectorBase
{
public:
    IVDotsDisplayControl(const IRECT& bounds, const char* label = "",
                         const IVStyle& style = DEFAULT_STYLE,
                         const ISVG& bgSVG = ISVG(nullptr))
  : IControl(bounds)
  , IVectorBase(style)
  , mBgSVG(bgSVG)
  {
    AttachIControl(this, label);
  }

  void Draw(IGraphics& g) override
  {
    DrawWidget(g);
  }

  void DrawWidget(IGraphics& g) override
  {
    IRECT r = mWidgetBounds.GetPadded(-mPadding);
    const IRECT plot = r.GetPadded(-3.f);
    const int ndotsl    = std::max(0, std::min(200, static_cast<int>(mBuf.vals[0][200 * 2 + 0])));
    const int ndotsr    = std::max(0, std::min(200, static_cast<int>(mBuf.vals[0][200 * 2 + 1])));
    const int voiceCount = std::max(ndotsl, ndotsr);
    const uint8_t* pl   = &mBuf.vals[0][0];
    const uint8_t* pr   = &mBuf.vals[0][200];

    const IColor accent     = GetColor(kX1);
    const IColor partner    = GetColor(kX2);
    const IColor backColor  = GetColor(kBG);
    const IColor frameColor = GetColor(kFR);
    const IColor topWash    = accent.WithOpacity(0.08f);
    const float  xPerData  = plot.W() / 255.f;
    const float  cr        = std::min(12.f, r.H() * 0.06f);

    g.FillRoundRect(backColor, r, cr, &mBlend);

    // Throat anatomy SVG background — render once into a cached layer.
    if (mBgSVG.IsValid())
    {
      if (!g.CheckLayer(mBgLayer))
      {
        g.StartLayer(this, r);
        // Crop a little tighter and bias the portrait upward so it reads as atmosphere.
        const float svgW = mBgSVG.W() > 0.f ? mBgSVG.W() : 1.f;
        const float svgH = mBgSVG.H() > 0.f ? mBgSVG.H() : 1.f;
        const float scale = std::max(r.W() / svgW, r.H() / svgH) * 1.1f;
        const float drawW = svgW * scale;
        const float drawH = svgH * scale;
        const float drawL = r.L + (r.W() - drawW) * 0.5f + r.W() * 0.04f;
        const float drawT = r.T + (r.H() - drawH) * 0.5f - r.H() * 0.035f;
        const IRECT svgDest(drawL,
                            drawT,
                            drawL + drawW,
                            drawT + drawH);
        g.DrawSVG(mBgSVG, svgDest);
        mBgLayer = g.EndLayer();
      }
      // Composite SVG lightly so the data field wins first.
      IBlend svgBlend(EBlend::Default, 0.15f);
      g.DrawLayer(mBgLayer, &svgBlend);

      // Soften the anatomical core behind the densest part of the dot field.
      const float washCx = plot.MW();
      const float washCy = plot.MH() - plot.H() * 0.02f;
      const float washRx = plot.W() * 0.36f;
      const float washRy = plot.H() * 0.30f;
      g.PathClear();
      g.PathEllipse(washCx, washCy, washRx, washRy);
      const IPattern focusFade = IPattern::CreateRadialGradient(washCx, washCy, std::max(washRx, washRy),
      {
        {backColor.WithOpacity(0.13f), 0.f},
        {backColor.WithOpacity(0.07f), 0.58f},
        {backColor.WithOpacity(0.0f), 1.f}
      });
      g.PathFill(focusFade, IFillOptions(), &mBlend);
    }

    g.FillRoundRect(topWash, IRECT(r.L + 1.f, r.T + 1.f, r.R - 1.f, r.T + r.H() * 0.16f), cr, &mBlend);
    g.DrawRoundRect(frameColor.WithOpacity(0.72f), r, cr, &mBlend, 1.f);

    const float xCenter = plot.L + plot.W() * 0.5f;
    g.DrawVerticalLine(frameColor.WithOpacity(0.24f), xCenter, plot.T, plot.B, &mBlend, 1.2f);

    for (int i = 1; i < 4; ++i)
    {
      const float y = plot.T + plot.H() * static_cast<float>(i) / 4.f;
      g.DrawHorizontalLine(frameColor.WithOpacity(0.035f), y, plot.L, plot.R, &mBlend, 1.f);
    }

    if (voiceCount < 1)
      return;

    const float topPad  = 2.f;
    const float usableH = std::max(8.f, plot.H() - topPad * 2.f);
    const float rowStep = usableH / static_cast<float>(voiceCount);
    const float dotR    = std::min(4.2f, std::max(1.0f, rowStep * 0.45f));

    for (int i = 0; i < voiceCount; ++i)
    {
      const bool hasL = i < ndotsl;
      const bool hasR = i < ndotsr;
      if (!hasL && !hasR)
        continue;

      const float y  = plot.T + topPad + rowStep * (static_cast<float>(i) + 0.5f);
      const float xL = hasL ? (plot.L + static_cast<float>(pl[i]) * xPerData) : 0.f;
      const float xR = hasR ? (plot.L + static_cast<float>(pr[i]) * xPerData) : 0.f;

      if (hasL && hasR)
        g.DrawLine(frameColor.WithOpacity(0.08f), xL, y, xR, y, &mBlend, 1.f);

      if (hasL)
      {
        g.FillCircle(accent.WithOpacity(0.07f), xL, y, dotR * 4.8f, &mBlend);
        g.FillCircle(accent.WithOpacity(0.18f), xL, y, dotR * 2.4f, &mBlend);
        g.FillCircle(accent.WithOpacity(0.94f), xL, y, dotR,        &mBlend);
      }

      if (hasR)
      {
        g.FillCircle(partner.WithOpacity(0.04f), xR, y, dotR * 4.5f,   &mBlend);
        g.FillCircle(partner.WithOpacity(0.12f), xR, y, dotR * 2.1f,   &mBlend);
        g.FillCircle(partner.WithOpacity(0.96f), xR, y, dotR * 0.88f,  &mBlend);
      }
    }
  }

  void OnResize() override
  {
    SetTargetRECT(MakeRects(mRECT));
    mBgLayer = nullptr; // force SVG layer re-render at new size
    SetDirty(false);
  }

  void OnMsgFromDelegate(int msgTag, int dataSize, const void* pData) override
  {
    if (!IsDisabled() && msgTag == ISender<>::kUpdateMessage)
    {
      IByteStream stream(pData, dataSize);
      int pos = 0;
      pos = stream.Get(&mBuf, pos);
      SetDirty(false);
    }
  }

private:
  ISenderData<1, std::array<uint8_t, 200 * 2 + 2> > mBuf;
  float mPadding = 0.f;
  ISVG mBgSVG { nullptr };
  ILayerPtr mBgLayer;
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
