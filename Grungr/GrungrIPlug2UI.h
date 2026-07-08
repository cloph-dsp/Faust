#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"

namespace igraphics = iplug::igraphics;

namespace grungr {
namespace ui {

struct Params {
  int grunge = 0;
  int butt = 1;
  int face = 2;
  int loud = 3;
  int bypass = 4;
  int rawVoicing = 5;
};

struct Assets {
  const char* backgroundSVG = GRUNGE_BG_SVG_FN;
  const char* knobSVG = POWERBUTTON_SVG_FN;
  const char* logoSVG = CLOPH_LOGO_SVG_FN;
  const char* titleSVG = GRUNGR_LOGO_SVG_FN;
  const char* stompSoleSVG = STOMP_SOLE_SVG_FN;
  const char* mainFont = NOMOREHERO_FN;
  const char* secondaryFont = FUTUREERODED_FN;
  const char* tertiaryFont = BOXPOT_FN;
};

void BuildOrRelayout(igraphics::IGraphics* pGraphics,
                     const Params& params,
                     const Assets& assets = Assets());

bool HandleGlobalKey(igraphics::IGraphics* pGraphics,
                     const iplug::IKeyPress& key,
                     bool isUp);

}  // namespace ui
}  // namespace grungr
