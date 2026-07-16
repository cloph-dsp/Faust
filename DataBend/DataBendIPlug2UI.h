#pragma once

#include "IPlug_include_in_plug_hdr.h"

namespace databend::ui
{
constexpr const char* kLayoutFontID = "DataBendLayoutFont";

void BuildLayout(IGraphics* graphics);
}
