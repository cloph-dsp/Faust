# Grungr Final Fixes

## Fix 1: Knob Angle
- File: `Grungr/GrungrIPlug2UI.cpp` line 1337
- Change `pKnob->SetAngles(90.f, 360.f)` → `pKnob->SetAngles(0.f, 300.f)`
- SVG indicator at 0° rotation = 7 o'clock; 300° sweep brings it to ~5 o'clock at max

## Fix 2: Value Text Font
- `Grungr/resources/boxpot1.ttf` exists (160KB, valid TrueType)
- Already wired: `kTertiaryFontTag = "BoxPot"`, loaded in `LoadFonts()`, used in `valueFontID`
- Probable issue: font loading fails silently — `g.LoadFont(kTertiaryFontTag, assets.tertiaryFont)` returning false
- Verify the `.ttf` has the correct internal font name or try loading by absolute path
- Build + deploy as single-file VST3

## Fix 3: Deploy
- Copy `Grungr/build-win/vst3/x64/Release/Grungr.vst3` → `C:\Program Files\Common Files\VST3\Grungr.vst3`
