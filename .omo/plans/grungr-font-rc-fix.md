# Grungr Font RC + Knob Angle Fix

## Issue: BoxPot font not loading
**Root cause**: `Grungr/resources/main.rc` embeds fonts as Windows resources for runtime loading. BoxPot is NOT registered there. The other fonts (NoMoreHero, FutureEroded, Roboto) are — they work because the `.rc` file has entries like:
```
NOMOREHERO_FN TTF "NoMoreHeroV2-Regular.ttf"
```
BoxPot needs this same treatment.

## Fix 1: Add BoxPot to main.rc
Two places in `Grungr/resources/main.rc`:

1. TEXTINCLUDE 1 section (line ~146): Add `"BOXPOT_FN TTF \"boxpot1.ttf\"\0"` after the CLOPH_LOGO line.

2. Main resource section (line ~244): Add `BOXPOT_FN TTF "boxpot1.ttf"` after the ROBOTO_FN line.

## Fix 2: Knob angle
In `Grungr/GrungrIPlug2UI.cpp` line 1337: Change `pKnob->SetAngles(90.f, 360.f)` → `pKnob->SetAngles(0.f, 300.f)`
- SVG indicator at 0° rotation faces ~7 o'clock
- 300° clockwise sweep brings it to ~5 o'clock at max
