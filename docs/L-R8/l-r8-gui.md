# L-R8 GUI Implementation

## Current Understanding

### Project
- **Name**: L-R8 Stereo Shifter (plugin ID `LR8S`)
- **Type**: iPlug2 VST3 + Faust DSP
- **Size**: 780×420 (min 390×210, max 1560×840, FPS 60)
- **Author**: cloph — dark industrial aesthetic

### Parameters (8 total)
| Param | Type | Range | Label | Notes |
|-------|------|-------|-------|-------|
| Bypass | Bool | Off/On | "Bypass" | Power button |
| Sync | Bool | Off/On | "Sync" | Toggle S |
| Rate | Int | 0-14 | "Rate" | Sync division mapping |
| BPM | Double | 1-300 | "BPM" | Manual/auto tempo |
| Free Rate | Double | 0.1-10 | "Free Rate" | Knob — "Rate" label |
| Crossfade | Double | 0-1 | "Crossfade" | Knob — "Cross" label |
| High Pass | Double | 20-1000 | "High Pass" | Knob — "Split" label |
| Low Pass | Double | 1000-20000 | "Low Pass" | Knob — "Air" label |

### Current GUI Controls (all custom vector-drawn)
1. **PowerControl** (ISwitchControlBase) — Circle with power icon + glow animation
2. **SyncControl** (ISwitchControlBase) — Circle with "S" letter + glow
3. **KnobControl** (IKnobControlBase) — Arc track + traveling dot, hover/pulse animations
4. **ScopeControl** (IControl) — Mid/Side waterfall display, grays+blues+reds, scroll-zoom
5. **BpmDisplayControl** (IControl) — Click-to-edit BPM + rate division display

### Current Layout
```
┌──────────────────────────────────────────────────────────┐
│ [⚡]                                       L-R8          │ ← Top bar (28px)
├──────┬──────────────────────────────────────┬──────┤
│      │  L   ─────────────   R               │      │
│ Rate │                                      │ Split│
│      │  ┌────────────────────────────┐      │      │
│      │  │        SCOPE               │      │      │
│ Cross│  │   (waterfall M/S)          │      │ Air  │
│      │  └────────────────────────────┘      │      │
├──────┴──────────────────────────────────────┴──────┤
│ [⏺]  ┌──────────────────────────────────┐         │ ← Bottom bar (54px)
│ Sync │  120.0 BPM                       │         │
│      │  x1 Rate                         │         │
└──────┴──────────────────────────────────┴─────────┘
```

- Left side: Rate knob + Cross knob (105px max width)
- Center: L/R labels + Scope waterfall (fills remaining)
- Right side: Split knob + Air knob (105px max width)
- Bottom: Sync toggle + BPM display

### Color Palette
- colBG {14,16,22} — near-black background
- colPN {48,50,62} — panel borders
- colTX {225,228,240} — primary text (almost white)
- colTD {140,145,160} — dim text
- colTM {85,90,105} — muted text
- colBL {60,140,255} — blue (side L)
- colRD {235,60,70} — red (side R)
- colGY {150,155,165} — gray (mid)
- colBLGlow — blue glow
- colGR {42,44,56} — grid lines
- colSB {20,22,32} — scope background
- colKB {36,38,50} — knob background
- colKA {50,52,64} — knob arc dim
- colFB {24,26,36} — field background

### DSP
- Faust-generated L8FaustDSP from `Faust/L-R8.dsp`
- Latency: 2048 samples
- Stereo shifter: Linkwitz-Riley crossovers, all-pass decorrelation, BPM-synced LFO, crossfade blend
- Parameters synced via ZoneCaptureUI → Faustinternal mapping

### Build
- Win-only currently (VS2022, v143)
- MSBuild via LR8-vst3.vcxproj
- Output: `build-win/vst3/x64/Release/L-R8.vst3`

## User Design Decisions (from 10 questions)

1. **Aesthetic**: Dark industrial — keep and evolve with more refinement
2. **Scope**: Improve waterfall — more resolution, richer colors, better feedback
3. **Knobs**: SVG-based — better visual quality, custom SVG assets
4. **Layout**: Expansive — better use of 780px horizontal space, groups side-by-side
5. **Headers**: No section headers — clean design, implicit organization
6. **BPM**: Keep click-to-edit — current pattern is fine
7. **Value feedback**: Tooltip on hover — popup with value when hovering
8. **Colors**: Change accents — keep dark industrial but new accent colors (not just blue/red/gray)
9. **Animation**: More animations — micro-interactions, transitions
10. **Responsive**: Scale uniform — kept as current behavior

## Implementation Plan (Oracle-Reviewed)

### Oracle Review Key Points (2026-06-24)
1. **Reorder**: Colors → Scope → SVG Knobs → Animation → Tooltip
2. **SVG knobs**: Use ISVGKnobControl with vector overlay for hover/glow (hybrid approach)
3. **Test SVG early** — nanosvg may differ from browser rendering
4. **Tooltip**: Global overlay approach with mIgnoreMouse=true
5. **Animation**: Migrate KnobControl's std::chrono pulse to SetAnimation()
6. **Layout**: Bump side panels to 130px+ (from 105px), add scope adornments
7. **Code quality**: Extract controls at 800+ lines, fix label-identification hack, remove dead GetVisualizerData accessors
8. **Faust bypass inversion** (line 584): Add comment documenting intentional reverse logic

### Revised Phases

| Phase | What | How |
|-------|------|-----|
| **1** | **Accent Color Refresh** | @designer: choose new palette. Apply: replace all colBL/colRD/colGY refs. Side panels to 130px+. Fix label-identification hack. |
| **2** | **Scope Enhancement** | Higher resolution, richer gradients using new palette, level meters. Add decorrelation/zoom controls. |
| **3** | **SVG Knob Assets & Control** | Design SVGs to match palette. ISVGKnobControl + vector overlay. Create SVG resources. |
| **4** | **Animation Polish** | Micro-interactions, migrate chrono→SetAnimation(), power toggle refinement. |
| **5** | **Tooltip System** | Global overlay, param name + formatted value on hover. |
| **6** | **Code Cleanup** | Remove dead GetVisualizerData, extract controls if file >800 lines, document bypass inversion. |

### Accent Palette Direction
User chose "Mudar accents" — keep dark industrial foundation (colBG, colPN, colTX, colTD) but replace L/R/Mid colors (colBL/colRD/colGY) with a different accent set that fits dark industrial aesthetic.

## Plan Status

- [x] Discovery and understanding complete
- [x] 10 GUI questions asked
- [x] User answers gathered
- [x] Plan drafted and reviewed by @oracle
- [ ] Phase 1: Accent Color Refresh + Layout Width Bump
- [ ] Phase 2: Scope Enhancement
- [ ] Phase 3: SVG Knob Assets & Control
- [ ] Phase 4: Animation Polish
- [ ] Phase 5: Tooltip System
- [ ] Phase 6: Code Cleanup
- [ ] Final validation
