# VST3 Developments Agent Guide

This repository contains **multiple independent VST3 plugin projects** using iPlug2 + Faust. See `/docs/` for detailed guides.

## Key Facts (What Agents Miss Without Help)

### 1. Multi-Project, Not Monorepo
- Each plugin is **self-contained** with its own `.vcxproj`, `config.h`, and build scripts
- **No shared build order** — projects do not depend on each other
- Building one plugin does NOT require building others

### 2. Build System: Visual Studio ONLY
- **Never** run `.bat` scripts directly — they depend on VS build variables (`%1`-`%9`, shifted after 6× `shift`)
- **Always** open `.vcxproj` in Visual Studio (or use `msbuild` via VS toolchain)
- Output: `build-win/vst3/x64/Release/PluginName.vst3`

### 3. Resource Embedding Varies by Plugin
- **Freeze95**: SVGs in `resources/main.rc` (compiled into DLL); fonts copied to `Contents/Resources/` by postbuild script
- **Grungr**: SVG assets compiled to C++ headers via Python script (`tools/generate_embedded_assets.py`)
- **Others**: Simple `.ico` icons, no special embedding

### 4. Faust Sources Need Explicit Integration
- `.dsp` files compile to C++ via `faust` CLI — **not** auto-included in builds
- `Freeze95/test_*.dsp` = standalone test fixtures (compile via Faust IDE only)
- `Grungr/Grungr.dsp` has iPlug2 wrapper in `tools/iplug2-ui/` (separate project)

### 5. Plugin Configs Are Authoritative
- Each plugin has `config.h` with `PLUG_CHANNEL_IO`, `PLUG_WIDTH`, `PLUG_HEIGHT`, latency, etc.
- These define actual plugin behavior — filenames alone don't reveal I/O or UI size

### 6. Testing Is Decentralized
- **Faust tests** → Faust IDE only (not DAW-based)
- **C++ tests** (Freeze95) → standalone executables
- **Host testing** → manual DAW load (no automated test suite)

### 7. Multi-Format Plugins (Sore Throat)
- Builds VST2, VST3, AAX, and standalone app from single codebase
- Multiple `.vcxproj` files in `projects/` — each format built separately

## Quick Reference

| Plugin | Type | I/O | Config | Build Command |
|--------|------|-----|--------|---------------|
| Freeze95 | Faust/IPlug2 | 2-2 | `config.h` | VS: `Freeze95` project |
| DataBend | iPlug2 | 2-2 | `config.h` | `msbuild DataBend-vst3.vcxproj` |
| Grungr | Faust→iPlug2 | 2-2 | UI code | VS: `DODGrungeIPlug2` |
| SpectrumMatch | iPlug2 | 2-2 | `config.h` | `msbuild RussetNoise-vst3.vcxproj` |
| Sore Throat | iPlug2 multi | 2-2 | Varies | `msbuild *.sln` (all formats) |
| Tuner | Faust→iPlug2 | 2-2 | `config.h` | `pwsh build_vst3.ps1` |
| Bronze Noise | iPlug2 (spectral) | 1-1/2-2 | `config.h` | VS: `BronzeNoise` project |

## Where to Look

- **Build instructions** → `docs/BUILDING.md`
- **Per-plugin guides** → `docs/PLUGIN_GUIDES.md`  
- **Faust workflow** → `docs/FAUST_WORKFLOW.md`
- **Project structure** → `docs/PROJECT_STRUCTURE.md`
- **Main README** → Root `README.md` (Faust experiments overview)

## oh-my-vst Plugin

`oh-my-vst/` is an **OpenCode plugin** (`@opencode-ai/plugin`) that provides framework presets, embedded agent prompts, and a CLI installer.

> ⚠️ Agent registration happens via `opencode.json`'s `agent` field, not via plugin hooks. The plugin provides prompt content (in `src/agents/`) but doesn't register agents at runtime.

**5 VST subagents** are registered in `.opencode/opencode.json`:

| Agent | @mention | Role |
|-------|----------|------|
| `vst-spec` | `@vst-spec` | Defines product brief, feature scope, control layout, I/O spec |
| `vst-architect` | `@vst-architect` | Selects framework (iPlug2/JUCE/Faust/Cmajor/Rust), designs signal flow |
| `vst-dsp` | `@vst-dsp` | Implements algorithms, numerical validation, real-time safety |
| `vst-gui` | `@vst-gui` | Designs and implements plugin GUI with responsive layouts |
| `vst-validate` | `@vst-validate` | Tests, benchmarks, host compatibility, release checklist |

**audio-dev skill**: `.opencode/skills/audio-dev/SKILL.md` tells the orchestrator (Sisyphus) which subagent to delegate to and when, with full prompts for each phase.

**Framework presets** (bundled in plugin): iPlug2, JUCE, Faust, Cmajor, Rust — each specifies agent skills, MCPs, and prompt additions.

> ⚠️ The old `.agents/skills/vst3-skills/` (21 skills) has been deleted. VST domain knowledge now lives in the audio-dev skill (orchestrator-facing) and the oh-my-vst plugin presets.

## Learned User Preferences

- User prefers **single-file VST3** (loose DLL with `.vst3` extension) over iPlug2's bundle wrapper — confirmed working in Bitwig alongside Freeze95's single-file deploy
- When asked "is this valid?" or asked to identify an unknown asset, verify against evidence rather than guessing — **browse online** (webfetch/context7/websearch) to confirm specs, identity, or behaviour before acting
- **Config-based agent registration** over plugin-based hooks — `@opencode-ai/plugin` v1 config hook / v2 `define()` API both fail to register agents at runtime. Agents must be defined directly in `opencode.json`'s `agent` field
- **audio-dev keyword** activates a skill at `.opencode/skills/audio-dev/SKILL.md` that tells the orchestrator about available VST subagents and when to delegate to them
- User uses **CLOPH** as the manufacturer prefix for new plugins (TnR1, F951, etc.)
- Multi-line PowerShell one-liners via the `bash` tool often fail because `snip` parses `if`, `;`, and backticks; write a `.ps1` file and run `powershell -ExecutionPolicy Bypass -File <path>` instead
- Custom SVG knob caps (ISVGKnobControl) for every knob — static body + rotating pointer indicator (not full-cap rotation), never ship stock IVectorBase dots as the final look
- Inline click-target bars for enum selectors (N visible cells, click to pick); reject popup menus and cyclic steppers. Abbreviate display strings so cells stay legible ("Equal-loudness" -> "Loud")
- Larger UI text by default: control labels >= 16, value text >= 18, plugin title >= 36
- Visualizer/spectrum should fill meaningful space but not dominate 80%+ of window — reserve at least 130-150px at bottom for controls
- Two-font typography for plugins: embedded TITLE font (chunky display face, e.g. IronSans) for the plugin name, embedded SECONDARY font (e.g. Kenyan Coffee Rg) for body labels/values. Load both via 2-arg `LoadFont` with separate font IDs
- For "finished tool" feel: tooltips on every control, double-click-to-reset on knobs, I/O meters flanking the visualizer, brand/version mark in the title bar, subtle idle animation (breathing LED)
- User expects root-cause fixes over symptom suppression — when a bug has both a band-aid and a proper fix, fix the root cause

## Learned Workspace Facts

- **iPlug2 API gotchas**: `GetParam()->Set()` alone does NOT notify VST3 host — use `IGEditorDelegate::SendParameterValueFromUI` with `BeginInformHostOfParamChangeFromUI`/`SendParameterValueFromUI`/`EndInformHostOfParamChangeFromUI` gesture. `LoadSVG` has 2 overloads — passing 2 string args treats 2nd as `units`, resource lookup silently fails; always call `g.LoadSVG(filename)` with a single arg for embedded SVGs.
- **iPlug2 resource embedding**: `main.rc` uses `X_FN TTF/SVG X_FN` where config.h defines `#define X_FN "file.ext"`. vcxproj needs `<ResourceCompile Include="..\resources\main.rc" />`. Use 2-arg `LoadFont(id, X_FN)` for embedded TTFs. RC only watches main.rc timestamp — after changing an embedded asset, run `touch main.rc` + delete `build-win/.../int/main.res` to force re-link.
- **Freeze95 patterns**: Set `mSendUpdate = true` in OnActivate to trigger `SendCurrentParamValuesFromDelegate()` in OnIdle — all non-distributed iPlug2 plugins need this for GUI to reflect host automation. Also call `SendCurrentParamValuesFromDelegate()` at end of LayoutUI for initial-open sync. Two-tier scale (`mDrawScale x mScreenScale`). SpeakerKnobControl: Lerp(135,405). TinyKnobControl: IKnobControlBase subclass. MiniManualToggleControl: EVAlign::Middle. PLUG_HEIGHT=306.
- **Tuner build + FAUST**: Sources at `Tuner VST/` root, copied to `iPlug2/Examples/IPlugEffect/` via `_local_copy.ps1`, compiled as single TU by `IPlugEffect-vst3.vcxproj`. `build_vst3.ps1` invokes msbuild with SolutionDir. Output is single-file `.vst3` (not bundle). Config.h MUST define `BUNDLE_DOMAIN`, `BUNDLE_NAME`, `PLUG_CLASS_NAME`, and a font macro for RC. FAUST constraints: output buffers sized to PLUG_CHANNEL_IO (no extras), `sample**` is `double**` (never reinterpret_cast to float*), A4 ref via `InitEnum` + `SetDisplayText`, smoothing `alpha = lerp(0.01, 0.51, 1-mSmooth)` where higher mSmooth = more smoothing.
- **macOS deployment**: 10.15+ deployment target, ad-hoc sign with `--options=runtime`, hardened runtime. macOS 15+ AUValidationService subprocess crashes with CocoaUI — guards in patched-iPlug2. `AudioUnit Version` Info.plist key has a space. auval -view/-stress removed in macOS 15.
- **Bitwig file lock**: When Bitwig is running, system VST3 copy fails. Must quit ALL processes: Bitwig Studio, BitwigAudioEngine-X64-AVX2, up to 3x BitwigPluginHost-X64-AVX2, BitwigStudioApp.
- **BronzeNoise patterns**: ISVGKnobControl extends IKnobControlBase (no text rendering — subclass for labels). 1000x700 resizable, dark industrial. Two embedded fonts. KnobWithTextControl: dark pill, hover alpha, value pulse animation. Knob body stays static; rotate a separate pointer via `p->ToNormalized(p->Value())` — must use normalized value (0-1), not raw `p->Value()`, since Amount is 0-100%, Bandwidth 0.01-4, Character -1 to 1. Audio→UI scalar data: store peaks in `std::atomic<float>` written in ProcessBlock, load with `memory_order_relaxed` in OnIdle. Double-click reset: override `OnMouseDblClick` -> `GetParam()->SetToDefault()` + `SetValueFromDelegate(p->ToNormalized(p->GetDefault()))` + `SetDirty()`. `IKnobControlBase::IsFineControl(mod, wheel)` already does Shift=10x fine adjust; do not re-implement.
- **IParam::GetDisplay enum string**: 4-arg signature `(double value, bool force, WDL_String& str, bool asEnumDisplay=true)`. Pass `true` for the last arg to get display string (e.g. "4096"); `false` returns the raw index/number. Required when drawing segmented-selector cells — without `true`, cells show "3" instead of "4096".
- **Windows GDI rejects CFF-based OTF**: OpenType fonts with CFF outlines (magic bytes `OTTO` = `0x4F 54 54 4F`) fail to load via `AddFontMemResourceEx`, so embedded iPlug2 fonts render as fallback. For embedded TTF resources, always use TrueType (magic `0x00 01 00 00`). Convert OTF→TTF with `python -m otf2ttf.cli SRC -o DST` (fontTools + otf2ttf). Verify magic with a hex read of the first 4 bytes.
- **Custom controls used as header member types must not live in anonymous namespaces**: `class IOMeterControl;` forward decl in the public header is at file scope, but defining the class in an anonymous namespace in the .cpp creates a different type → `C2872 ambiguous symbol`. Either move the class to file scope, store as `IControl*` in the header, or wrap in a named namespace.
- **Windows x64 denormal protection**: Set `_mm_setcsr(_mm_getcsr() | 0x8040)` in ProcessBlock to enable FTZ (bit 15) + DAZ (bit 6) for SSE/AVX. Required in iPlug2 plugins with heavy IIR/ADAA Faust processing to prevent denormal stalls.
- **Faust ADAA startup NaN**: ADAA divided-difference `(G(x[n]) - G(x[n-1])) / (x[n] - x[n-1])` produces NaN when both filter state and input are zero at init (delta=0 → 0/0). Fix with a warm-up `compute(8, tiny_ramp_signal)` in OnReset() after init() + param sync. Keep NaN clamp in ProcessBlock as safety net.
- **iPlug2 z-order**: Controls render in attachment order — last-attached paints on top. If a control's Draw() fills its bounding rect with opaque content (e.g. faceplate SVG redraw on stomp engagement), it covers earlier-attached controls in the overlapping area. Attach overlays (logos, badges) AFTER the control that redraws behind them.
- **Grungr build setup**: MSBuild at `C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe`. VS BuildTools `vswhere` returns empty array — cannot rely on `vswhere` for this edition. Build vcxproj without .sln needs explicit `/p:SolutionDir="<Grungr_dir>\\"`. Postbuild script always fails on AAX icon/ARM64EC paths (exit 255) but VST3 binary is compiled before postbuild runs — ignore postbuild errors. Grungr authoritative Faust header is `Grungr/DSP/GrungrFaustDSP.h`; `Grungr/DSPGrungrFaustDSP.h` at root is stale duplicate.
