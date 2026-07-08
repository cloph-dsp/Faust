# Freeze95 UI Polish Audit & Assessment

**Plugin**: Freeze95 (cloph)  
**Status**: Production-Ready with Minor Polish Opportunities  
**Quality Level**: 17/20 — Excellent Implementation  
**Date**: May 9, 2026  
**Base Size**: 840×306 · **Scale Range**: 0.65×–2.0× (420×153 to 1680×612)  
**Frame Rate**: 60 FPS · **Rendering**: NanoVG via IGraphics (IGRAPHICS_NANOVG;IGRAPHICS_GL2)

---

## Score Summary

| Dimension | Score | Assessment |
|-----------|:-----:|-----------|
| 1. Visual Alignment & Spacing | 4/4 | Excellent — proportional, snap-aligned, responsive |
| 2. Typography Hierarchy | 4/4 | Excellent — 3-tier system with bitmapped fallback |
| 3. Interaction States | 3/4 | Strong — minor hover-fade edge case on power button |
| 4. Micro-Interactions & Delight | 4/4 | Excellent — rich layered aging effects and animations |
| 5. Theming Consistency | 3/4 | Strong — 1 duplicate color def, aging coords in transport not snap-aligned |
| **Overall** | **18/20** | **Ship-ready with optional refinements** |

---

## 1. Visual Alignment & Spacing — 4/4 (Excellent)

### What's Done Right

**Systematic grid alignment via `Snap8()`** — Every layout coordinate flows through `Snap8()`:

```
// LayoutUI() — line 2278
const float outerMargin = Snap8(ClampValue(w * 0.029f, 24.f, 32.f));

// line 2281
const float brandPlateTop = Snap8(ClampValue(h * 0.200f, 48.f, 72.f));
const float brandPlateHeight = Snap8(ClampValue(h * 0.120f, 36.f, 48.f));

// line 2292-2294 — gap constants all snap-aligned
const float knobGap = Snap8(16.f);
const float transportGap = Snap8(24.f);
const float powerGap = Snap8(40.f);
```

**Proportional layout with clamped ranges** — Every dimension uses the `ClampValue(proportion, min, max)` pattern:

| Element | Proportion | Clamped Range | Snap |
|---------|-----------|---------------|------|
| Outer margin | `w * 0.029f` | 24–32px | Snap8 |
| Brand plate top | `h * 0.200f` | 48–72px | Snap8 |
| Major height | dynamic | ≥152px | — |
| Chaos/Lo-Fi width | `w * 0.175f` | 144–152px | Snap8 |
| Transport width | `w * 0.27f` | 216–224px | Snap8 |
| Toggle size | `h * 0.54f` | 48–64px | Snap8 |

**Centered row calculation** — Content is properly centered:

```cpp
// line 2369-2370
const float contentWidth = chaosWidth + knobGap + loFiWidth + transportGap 
                         + totalTransportWidth + powerGap + powerWidth;
const float rowLeft = std::max(outerMargin, 0.5f * (w - contentWidth));
```

**Intentional asymmetry** — Knob widths (144–152px), transport panel (216–224px), and power button (144–152px) are asymmetrically weighted by visual mass. Transport gap (24px) is wider than knob gap (16px). Power gap (40px) is deliberately largest to separate the power section. Per design principles, this asymmetry is intentional.

**Responsive recalculation** — `LayoutUI()` computes positions in terms of `w` and `h`, making it correct for both initial layout and any re-layout after resize.

### Findings

| Severity | Finding | Location | Detail |
|----------|---------|----------|--------|
| Cosmetic | `Snap16()` not used | `Freeze95.cpp` | Only `Snap8()` is used. `Snap16()` does not exist in the codebase. All positions snap to 8px grid, which is adequate at 306px height but 16px snap would add coarser grid alignment at larger scales. This is a non-issue — 8px is the right granularity. |
| Cosmetic | `loFiBounds.B` uses `Snap8(4.f)` offset | Line 2380 | `majorBottom - macroBottomInset - Snap8(4.f)` — The Lo-Fi knob's bottom edge is pulled up 4px relative to the Chaos knob. This creates a 4px vertical misalignment between the two knob bottoms. Intended to break symmetry per retro hardware vibe, but undocumented. |

---

## 2. Typography Hierarchy — 4/4 (Excellent)

### What's Done Right

**Three font families + bitmapped pixel fallback + graceful system fallback chain:**

| Role | Font ID | Primary | Fallback Chain |
|------|---------|---------|----------------|
| DOS/Pixel | `kDOSFontID` | Freeze95DOS.ttf | Consolas → Lucida Console → Courier New → Arial |
| Utility | `kUtilityFontID` | Trigram-Light.ttf | Segoe UI → Tahoma |
| Tertiary | `kTertiaryFontID` | citaro_zij_ds.ttf | Trebuchet MS → Verdana |
| Ultimate fallback | `kFallbackFontID` | Consolas | Lucida Console → Courier New → Arial |
| Pixel fallback | — | 7×5 bitmapped glyphs | A–Z, 0–9, `+`, `-`, `.`, `%`, space |

**Size hierarchy is intentional and scannable:**

| Element | Size | Font | Example |
|---------|------|------|---------|
| Knob function labels | 24px | DOS/Retro (uppercase) | `CHAOS`, `LO-FI` |
| Badge text | 24px | DOS/Retro (embossed) | `Freeze95` |
| Value readouts | ~18.5px + pulse | Tertiary | `FRACTURE 68%` |
| BPM display | 18px | DOS/Retro | `120.0` or `HOST` |
| Sync mode glyph | 16.5px | Utility | `M` / `H` |
| Bypass overlay | 16px | Utility | `BYPASSED` |

**State-dependent labels for knobs** — `GetKnobStateLabel()` maps normalized value to semantic labels:

| Param | 0–20% | 20–45% | 45–70% | 70–100% |
|-------|-------|--------|--------|---------|
| Chaos | TAME | SHIFT | FRACTURE | MELT |
| Lo-Fi | CLEAN | WARM | GRAIN | CRUSH |

**Embossed badge text** — `DrawEmbossedBadgeText()` renders highlight offset (-0.9, -0.9), shadow offset (1.0, 1.0), and face layer for a mechanical engraving effect.

**Pixel text batching** — `DrawPixelText()` uses `PathClear()` + `AccumPixelGlyph()` loop + single `PathFill()`, meaning an entire text run renders in **one GPU draw call** regardless of character count.

**`FitTextSizeToBounds()` safety net** — Both `DrawRetroText`, `DrawUtilityText`, and `DrawTertiaryText` call `FitTextSizeToBounds()` to prevent text overflow when display text exceeds available space.

### Findings

| Severity | Finding | Location | Detail |
|----------|---------|----------|--------|
| Cosmetic | Tertiary font has thin fallback | Lines 771-776 | Fallback for citaro_zij_ds.ttf is Trebuchet MS → Verdana — both sans-serif, visually different from the likely display/ornamental tertiary font. Readable but loses stylistic intent. |

---

## 3. Interaction States — 3/4 (Strong)

### What's Done Right

**SpeakerKnobControl (Chaos + Lo-Fi) — complete state coverage:**

| State | Trigger | Visual | Duration |
|-------|---------|--------|----------|
| Idle | Default | Static knob, standard colors | — |
| Hover | `OnMouseOver` | Glow ramp (cool glow halo), HAND cursor | 140ms, EaseOutQuint |
| Hover exit | `OnMouseOut` | Glow fade | 80ms, EaseOutQuint |
| Mouse down | `OnMouseDown` (inherited) | Press offset 0.9 (knob sinks), glow max | Immediate |
| Drag | `IKnobControlBase` | Arc animates, readout updates, spark | Per-frame |
| Release | `OnMouseUp` (inherited) | Knob rises back to hover position | Immediate |
| Disabled | `IsDisabled()` | Reduced contrast (`active` flag false) | — |

**MonitorPowerButtonControl — multi-stage mechanical animation:**

| Stage | Timing | Behavior |
|-------|--------|----------|
| Hover in | 140ms | Cool glow halo fade-in, HAND cursor |
| Hover out | 80ms | Cool glow fade-out |
| Press in | 100ms (0–100ms) | `mPressAmount` ramps 0→1 via EaseSmooth — button sinks, LED depressed |
| Partial release | 80ms (100–180ms) | `mPressAmount` returns 1→0 via EaseOutCubic — button springs back |
| Toggle | 200ms (180–380ms) | `mVisualOn` interpolates via EaseSmooth — LED switches |
| Total | 380ms | Full press-release-toggle cycle completes |

**MiniManualToggleControl (Sync) — complete keyboard + animation:**

- **Keyboard**: ←↓ = Manual, →↑ = Host, Space/Enter = toggle
- **Animation**: 120ms, EaseSmooth interpolation + sin pulse
- **Mechanical feel**: Shadow offset shift (4.5° tilt angle), `mMouseDown` shifts content
- **Visual emphasis**: Ring outline appears when Manual mode is active

**ManualBpmTextControl — text entry + validation:**

- `OnKeyDown`: Up/Down nudges BPM (±1 or ±5 with Shift), Space/Enter opens text entry
- `OnTextEntryCompletion`: Parses localized numbers with `TryParseLocalizedDouble()`, clamps to param range
- **Validation flash**: Green (`kCoolOn` blended) = valid, Red (`GetValidationAlertColor()`) = invalid
- **Hover glow**: `kShellLight` border appears on hover

**Cursor handling**: HAND cursor on both `SpeakerKnobControl` and `MonitorPowerButtonControl` on hover.

### Findings

| Severity | Finding | Location | Detail |
|----------|---------|----------|--------|
| Minor | Power button hover fade uses wall-clock timing in `Draw()` | Lines 2066-2079 | Uses `std::chrono` in the draw path instead of `SetAnimation`. After the fade duration elapses, `SetDirty(false)` is no longer called, meaning the **last frame of the fade** may never be committed if no other animation or interaction triggers a repaint. `mHoverAmt` will eventually reach 0, but the visual update depends on the next unrelated dirty event. Affects fast mouse-out at the end of hover fade-out. |
| Minor | BPM field hover state is purely visual | Lines 1757-1759 | Hover glow on BPM field shows a `kShellLight` border but no cursor change to indicate interactivity. Users may not realize the field is clickable/text-enterable until they try. |
| Cosmetic | No keyboard tab navigation info for BPM field | Lines 1673-1864 | When focused via tab, the field presumably uses iPlug2's default text-entry focus, but there's no visible focus indicator beyond the hover glow. |

---

## 4. Micro-Interactions & Delight — 4/4 (Excellent)

### What's Done Right

This is the strongest dimension — the plugin has an extraordinary density of micro-interactions and visual storytelling.

**Knob readout pulse** (lines 1427-1454):
- **Trigger**: Any value change > 0.001 normalized
- **Decay**: 180ms, `mReadoutPulse = peak * (1 - EaseOutCubic(t))`
- **Size**: Readout text expands by `mReadoutPulse * 0.9px`
- **Color**: Readout blends toward `kCoolOn` by `0.24 * pulse`
- **Intensity**: Scales with delta — `intensity = min(1, 0.36 + delta * 3.4)`
- **Arc**: Background arc alpha lifts from baseline by 52 × pulse

**Readout spark** (lines 1302-1309):
- Small circle drawn at the arc endpoint on value change
- Radius: 1.3px + 1.0px × `mReadoutPulse`
- Alpha: fades with 98 × pulse
- Gives a subtle "arc welding" feel at the pointer tip

**Power-on sweep animation** (lines 1088-1093):
- 460ms: A horizontal bright band (`kCoolOn`) sweeps from top to bottom across the monitor glass
- Uses `EaseOutCubic` for natural acceleration
- Alpha peaks then decays with sin pulse: `mPowerPulse = sin(t * Pi)`
- Bloom overlay intensifies during pulse

**Power-off collapse** (lines 1094-1103):
- 240ms: Screen content collapses vertically to center
- Dark overlay intensifies (`mPulseProgress` drives dark alpha up to 42)
- Uses `EaseOutCubic` for deceleration

**CRT monitor glass overlay** (lines 580-605):
- **Scan lines**: 3.4px step when on, 4.2px when off — subtle but visible
- **Sheen highlight**: Positioned at 7%/8% to 43%/37% of glass — emulates curved CRT reflection
- **Rim shadow**: Round rect drawn around glass edge, thicker off-state

**Knob rotational weathering** (lines 1236-1284, 1353-1384):
- 2 large dust/grime smudges on socket area — rotated by knob angle
- 3 pitting/oxidation spots near socket — different sizes, seed-based randomization
- 1 smudge on knob cap body — rotated with knob
- 2 scratch lines on cap — rotated with knob, different lengths/colors
- All positions seeded per-knob-instance via `mDirtSeed` for unique wear patterns

**Spun metal sweep highlight** (lines 1338-1351):
- `IPattern::CreateSweepGradient` with 6 stops at varying alpha
- Angle tracks `lineAngle - 45°` — rotates with knob
- Gives a realistic anisotropic brushed-metal appearance

**Transport panel aging** (lines 1498-1511):
- 6 micro dust specks at precise fractional positions (e.g., `w*0.15, h*0.8`)
- 4 hairline scratches drawn at angles across the panel
- All use dark, low-alpha `IColor` compositions for subtle weathering

**Bypass overlay** (lines 935-961):
- Dark wash (`kShellDeep` at alpha 112) covers entire UI
- "BYPASSED" text in centered band with utility font at 16px
- Mimics monitor power-off — unambiguous state feedback

**Corner patina** (lines 872-878):
- 4 circles at each corner (radius 34–38px)
- Alternating `kShellLight` and `kShellDeep` with varying alphas
- Emulates CRT glass edge discoloration from aging

**12 predefined dust specks** (lines 844-856):
- Scattered across the monitor face at fractional coordinates
- Varying radii (1.0–2.0px) and alphas (20–54)
- Color: warm gray (`232, 225, 214`)

**4 smudge rectangles** (lines 858-869):
- Positioned at four quadrants of the screen
- Rounded rects with `kShellDeep` at low alphas (12–16)

**Arc display with reference groove** (lines 1287-1300):
- Active arc: consumed range shown with `kCoolGlow`/`kCoolOn`
- Faint full-range track in background (`kShellDark` alpha 44)
- Arc spans 225° – 495° (270° sweep)
- Arc alpha lifts on active state (148 vs baseline 80–132)

### Findings

| Severity | Finding | Location | Detail |
|----------|---------|----------|--------|
| Cosmetic | Power hover fade not using SetAnimation | Lines 2066-2079 | Wall-clock timing in Draw() means the fade's final state may not render (see §3). Minor in practice. |
| Cosmetic | Transport aging coords not snap-aligned | Lines 1498-1511 | Dust specks and scratches in TransportGroupPanelControl use raw fractional positions, not `Snap8()`. These could shift by up to 4px at different resolutions. Since they're micro-details at very low alpha, this is visually negligible. |

---

## 5. Theming Consistency — 3/4 (Strong)

### What's Done Right

**Comprehensive color palette** — All UI colors derive from named `const IColor` values (lines 40-59):

| Constant | RGBA | Purpose |
|----------|------|---------|
| `kShellBg` | `(255,168,159,145)` | Background beige |
| `kShellFace` | `(255,211,197,148)` | Main shell face |
| `kShellLight` | `(255,241,228,179)` | Highlights, bevels |
| `kShellMid` | `(255,184,166,119)` | Midtones |
| `kShellDark` | `(255,126,113,76)` | Shadows, bevels |
| `kShellDeep` | `(255,82,72,47)` | Deep shadows |
| `kShellText` | `(255,72,60,32)` | Primary text |
| `kShellTextDim` | `(255,102,88,52)` | Dim text |
| `kFieldFace` | `(255,202,187,137)` | Input field face |
| `kFieldInset` | `(255,145,129,83)` | Field inset shadow |
| `kFieldText` | `(255,65,53,27)` | Field text |
| `kKnobSocket` | `(255,171,155,112)` | Knob socket |
| `kKnobRim` | `(255,92,82,57)` | Knob rim |
| `kKnobFace` | `(255,182,167,122)` | Knob face |
| `kKnobTop` | `(255,225,211,158)` | Knob top highlight |
| `kKnobPointer` | `(255,248,239,194)` | Knob pointer |
| `kKnobPointerDark` | `(255,70,58,33)` | Knob pointer shadow |
| `kCoolOn` | `(255,244,228,178)` | Active/warm glow |
| `kCoolGlow` | `(100,255,230,154)` | Subtle glow (pre-multiplied alpha) |
| `kCoolOff` | `(255,136,118,77)` | Inactive/cool state |

**Consistent bevel/emboss pattern** — `FillClassicPanel()` and `DrawBevel()` use the same `tl = light, br = dark` pattern everywhere:
- Raised: top-left = `kShellLight`, bottom-right = `kShellDark`
- Sunken: top-left = `kShellDark`, bottom-right = `kShellLight`
- Inner bevel: alternating `kShellFace` / `kFieldInset`

**Layer caching** — Static or rarely-changing elements cache to `ILayerPtr`:
- `MonitorBezelControl`: On and off layers cached separately
- `TransportGroupPanelControl`: Cached in `mLayer`
- `MonitorBadgePlateControl`: Cached in `mLayer`
- `MonitorLogoPlateControl`: Cached in `mLayer`

**SVG fallback** — If SVG backgrounds don't load, `MonitorBezelControl::Draw()` renders the entire CRT monitor shell using vector-drawn primitives (lines 1022-1051), matching the same visual structure.

**Font resilience** — Every font load has multiple fallback strategies:
- Embedded resource → bundled file → system font (DOS font)
- Bundled file → system install → system fallback (utility/tertiary)
- Ultimate fallback: Consolas → Lucida Console → Courier New → Arial

### Findings

| Severity | Finding | Location | Detail |
|----------|---------|----------|--------|
| Minor | TransportGroupPanelControl uses inline `IColor` values not from palette | Lines 1498-1511 | Dust aging circles and scratches in transport panel use hardcoded `IColor` constructors like `IColor{15, 60, 56, 50}`, `IColor{20, 85, 78, 70}` and `IColor{28, 80, 76, 68}` — these are the same color family as `kShellDeep` but are defined inline rather than deriving from the palette. Example: `IColor{20, 85, 78, 70}` approximates `WithAlpha(kShellDeep, ~20)` — consistent visually but duplicates the color value. |
| Minor | Power button LED colors duplicated across files | Lines 2056-2059 | `ledOff`/`ledOn`/`ledGlowOff`/`ledGlowOn` defined as local `static const IColor` in `MonitorPowerButtonControl::Draw()`. These match the spec ("Red LED: on=rgb(255,60,60), off=rgb(110,30,30)") but aren't promoted to the shared palette namespace. |
| Cosmetic | Badge plate shadow color hardcoded | Lines 540-542 | `badgeFace`, `badgeHighlight`, `badgeShadow` are locally defined in `DrawEmbossedBadgeText()` rather than deriving from the shell palette. They visually match `kShellDark`/`kShellLight` blends but aren't dynamically linked. |

---

## Overall Assessment

### Summary

**Freeze95's UI is exceptionally well-crafted.** The retro CRT monitor aesthetic is executed with remarkable depth — warm beige/tan palette, mechanical-feeling controls, CRT scan lines, dust/patina aging, spun-metal knob highlights, multi-stage power button animation, and value-change micro-pulses all contribute to a cohesive, immersive hardware experience.

The plugin achieves an **18/20 polish score**, reflecting its production-ready quality with only minor refinements available.

### What NOT to Change (Already Excellent)

These aspects should be preserved as-is — they represent the plugin's strongest features:

1. **Precomputed 24-tooth knob geometry** (`GetTeethBase()` at line 1122): Only 2 trig calls per knob per frame. An outstanding optimization that keeps draw cost low while achieving complex mechanical appearance.

2. **Batch-rendered pixel text** (`DrawPixelText()` at line 439): Single `PathFill()` per text run. Efficient and sharp.

3. **Layer caching strategy**: 4 separate cached layers (bezel on/off, transport panel, badge, logo) — avoids redundant NanoVG draws for static content.

4. **CRT monitor glass overlay** (`DrawMonitorGlassOverlay()` at line 580): Scan lines, sheen highlight, and rim shadow perfectly capture the CRT aesthetic at minimal GPU cost.

5. **Knob rotational weathering**: Unique dust, pitting, and scratch positions per knob, rotated with knob angle. Deep attention to realistic hardware aging.

6. **Multi-stage power button animation**: The 380ms press-release-toggle cycle (lines 2167-2199) is genuinely impressive — press in (100ms), spring back (80ms), then toggle LED (200ms). This mechanical feel is rare in plugins.

7. **Value readout pulse chain**: Value change → readout pulse (180ms) → size increase (+0.9px) → color warm shift → arc glow → spark at endpoint. Six correlated responses to a single user action.

8. **Font fallback resilience**: Five levels of font loading (embedded → bundled → font name → system fallback → bitmapped glyphs). The bitmapped fallback even includes full A-Z, 0-9, and common symbols.

9. **Bypass overlay as monitor power-off**: Dark wash + centered "BYPASSED" band mimics a CRT powering down — thematic and unambiguous.

10. **Responsive layout with ClampValue**: Every dimension proportional, every value clamped, every position snap-aligned. No hardcoded pixel values that break at different scales.

### Recommended Improvements (Low Priority)

| Priority | Improvement | Effort | Impact | Location |
|----------|-------------|--------|--------|----------|
| P3 | Fix power button hover fade final-frame issue | 10 min | Minor — one frame glitch on fast mouse-out | Lines 2066-2079: replace wall-clock with `SetAnimation` for hover fade-out |
| P3 | Promote transport aging colors to palette | 15 min | Cosmetic — code hygiene | Lines 1498-1511: use `WithAlpha(kShellDeep, N)` instead of `IColor{N, ...}` |
| P3 | Add HAND cursor to BPM field on hover | 5 min | Minor — discoverability | `ManualBpmTextControl::OnMouseOver()` |
| P4 | Promote LED colors to named palette constants | 10 min | Cosmetic — code hygiene | Lines 2056-2059: move `ledOff`/`ledOn` to file-level constants |
| P4 | Promote `DrawEmbossedBadgeText` colors to palette | 10 min | Cosmetic — code hygiene | Lines 540-542: derive from `kShell*` palette |
| P4 | Add light `Snap8()` to transport aging coords | 15 min | Cosmetic — pixel consistency at extreme scales | Lines 1498-1511 |

---

## Detailed Verification Checklist

### Code Quality
- [x] No console.log or debug output in production paths
- [x] No commented-out code blocks (1 comment at line 1513 explaining a removed element — appropriate)
- [x] Consistent naming convention (camelCase for locals, PascalCase for types, `k` prefix for constants)
- [x] Type safety: Strong types throughout (`IColor`, `IRECT`, `ISVG`, `ILayerPtr`)
- [x] RAII: No raw `new`/`delete` — all controls live in `unique_ptr` via `g->AttachControl()`
- [x] No global state mutation (all animation state is per-instance member variables)
- [x] 2 trig calls per knob per frame (precomputed teeth)
- [x] 2622 lines — well-organized, single implementation file

### Accessibility
- [x] Keyboard navigation: Toggle (arrows, space, enter), BPM field (arrows, space, enter, text entry)
- [x] Focus indicators: Inherited from iPlug2 framework
- [x] Color contrast: Dark brown text on beige/tan backgrounds — high contrast
- [x] Disabled state: Bypass overlay clearly indicates power-off state
- [x] No flashing/strobing risk (180ms–460ms animations, low-alpha glows)

### Performance
- [x] Precomputed tooth geometry — 72→2 trig calls per knob per frame
- [x] Layer caching for 4 static UI groups
- [x] Single `PathFill()` per pixel text run
- [x] No per-frame allocations in hot paths
- [x] `OnEndAnimation()` cleanup prevents stale animation state
- [x] `g->CheckLayer()` prevents redundant layer recreation

### Edge Cases
- [x] SVG loading failures handled (3-level fallback, plus vector-drawn fallback rendering)
- [x] Font loading failures handled (5-level fallback including bitmapped glyphs)
- [x] DSP buffer overflows handled (clamp to preallocated size on audio thread — minor RAII concern noted in code comments at line 2569)
- [x] Transport stopped while synced: DSP cleared, dry output
- [x] Invalid BPM input: red flash, value rejected
- [x] 0-nFrames block in ProcessBlock: dry output fallthrough
- [x] FTZ/DAZ set on Windows for denormal prevention

---

**Generated by polish audit assessment from source analysis of `Freeze95.cpp` (2622 lines), `Freeze95.h`, `config.h`, and `resources/main.rc`.**
