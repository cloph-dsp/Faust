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
  const char* logoSVG = cloph_logo_svg_fn;
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

// Push I/O meter peak values to the SimpleMeterControl widgets.
// The zero-latency signal path lets both meters show the current block.
void UpdateMeterLevels(igraphics::IGraphics* pGraphics,
                       float inputL, float inputR,
                       float outputL, float outputR);

}  // namespace ui
}  // namespace grungr
