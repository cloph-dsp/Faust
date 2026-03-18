# DOD Grunge iPlug2 UI Integration

This module gives you a complete SVG-based pedal UI for iPlug2 using the assets already in this repository.

## Files

- tools/iplug2-ui/DODGrungeIPlug2UI.h
- tools/iplug2-ui/DODGrungeIPlug2UI.cpp

## What It Builds

- SVG background panel using `References/Gemini_Generated_Image_4lqcrz4lqcrz4lqc.svg`
- SVG rotary knobs using `References/powerbutton.svg`
- Main font: `References/NoMoreHeroV2-Regular.ttf`
- Secondary font: `References/Future2097Eroded-Regular.otf`
- Controls for:
  - Grunge
  - Butt
  - Face
  - Loud
  - Bypass
  - Enhanced

## How To Wire It

1. Add the two UI source files into your iPlug2 plugin project.
2. Include `DODGrungeIPlug2UI.h` in your plugin source.
3. In your plugin constructor, use this pattern:

```cpp
#include "DODGrungeIPlug2UI.h"

IPlugMyPlugin::IPlugMyPlugin(const InstanceInfo& info)
: iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS,
                        GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](igraphics::IGraphics* pGraphics) {
    dodgrunge::ui::Params params;
    params.grunge = kParamGrunge;
    params.butt = kParamButt;
    params.face = kParamFace;
    params.loud = kParamLoud;
    params.bypass = kParamBypass;
    params.enhanced = kParamEnhanced;

    dodgrunge::ui::Assets assets; // Defaults already point to References/

    dodgrunge::ui::BuildOrRelayout(pGraphics, params, assets);
  };
#endif
}
```

## Notes

- `BuildOrRelayout()` is resize-safe. If your host resizes, calling it again updates control bounds.
- If you later move assets into your plugin resources folder, just override the four paths in `dodgrunge::ui::Assets`.
- The OTF secondary font is loaded from memory when possible, which is more robust on Windows builds.
