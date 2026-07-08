# Grungr Stomp Polish + Value Size

## Fix 1: Bigger value text
- File: `Grungr/GrungrIPlug2UI.cpp` line ~926
- Change `r.knobValueFontSize = knobSize * 0.16f` → `knobSize * 0.18f`
- Also bump valueH if needed: `valueH = knobSize * 0.24f` (was 0.22f)

## Fix 2: Remove ON/BYPASS badge + realistic stomp depression
File: `Grungr/GrungrIPlug2UI.cpp`

### Remove badge rect + text
- In `AnimatedStompBypassControl::Draw()` ~lines 316-356: Remove the entire status badge block (badge rect, state text drawing, badge FillRoundRect/DrawRoundRect calls)
- Can remove: `kStompStateOnText`, `kStompStateBypassText` constants (unused after)

### Add realistic depression animation
Instead of the badge, when the stomp is toggled ON:
- The tread area should visually "depress" — like a real pedal being stepped on
- Approach: draw a semi-transparent dark overlay on the tread when engaged, with a subtle shadow at the bottom edge (simulating depth)
- Use `mVisualDown` (already exists, 0→1 lerp when engaged) to drive the depression effect
- When `mVisualDown > 0.5`: draw a dark overlay + bottom shadow
- Keep existing: LED glow, ripple effect, engage pulse

Implementation for depression:
```cpp
// Visual pedal depression — tread darkens and shadow appears when stomped.
if (mVisualDown > 0.01f) {
  // Dark overlay simulating the tread being pressed down
  const float depressStrength = mVisualDown * 0.18f;
  g.FillRect(igraphics::IColor(static_cast<int>(255 * depressStrength), 0, 0, 0),
             mRECT, &mBlend);
  
  // Bottom shadow — gets darker when pressed
  if (mVisualDown > 0.3f) {
    const float shadowHeight = mRECT.H() * 0.06f;
    const float shadowAlpha = std::min(mVisualDown * 0.50f, 0.35f);
    const igraphics::IRECT shadow(mRECT.L, mRECT.B - shadowHeight, mRECT.R, mRECT.B);
    g.FillRect(igraphics::IColor(static_cast<int>(255 * shadowAlpha), 0, 0, 0),
               shadow, &mBlend);
  }
}
```

### What to keep
- LED (multi-layer: bezel → glow → lens → specular) 
- Ripple animation on click
- Engage pulse on LED
- Focus ring (keep for keyboard accessibility)
- Reduced motion support

## Build + Deploy
```
powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
$src = "Grungr\build-win\vst3\x64\Release\Grungr.vst3"
$dst = "C:\Program Files\Common Files\VST3\Grungr.vst3"
Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
Copy-Item $src $dst -Force
```
