#pragma once

#include "IPlug_include_in_plug_hdr.h"

using namespace iplug;
using namespace igraphics;

namespace databend::ui
{
constexpr const char* kLayoutFontID = "DataBendLayoutFont";

void BuildLayout(IGraphics* graphics);
}
