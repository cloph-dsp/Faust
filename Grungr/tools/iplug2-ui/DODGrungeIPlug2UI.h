#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"

namespace dodgrunge {
namespace ui {

struct Params {
  int grunge = 0;
  int butt = 1;
  int face = 2;
  int loud = 3;
  int bypass = 4;
  int enhanced = 5;
};

struct Assets {
  const char* backgroundSVG = "References/Gemini_Generated_Image_4lqcrz4lqcrz4lqc.svg";
  const char* knobSVG = "References/powerbutton.svg";
  const char* mainFont = "References/NoMoreHeroV2-Regular.ttf";
  const char* secondaryFont = "References/Future2097Eroded-Regular.otf";
};

void BuildOrRelayout(igraphics::IGraphics* pGraphics,
                     const Params& params,
                     const Assets& assets = Assets());

}  // namespace ui
}  // namespace dodgrunge
