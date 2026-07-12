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
- **Freeze95**: SVGs in `resources/main.rc` (compiled into DLL); fonts copied to `Contents/Resources/` by postbuild script
- **Grungr**: SVGs compiled to C++ headers via Python script (`tools/generate_embedded_assets.py`); UI code lives at project root (`GrungrIPlug2UI.cpp/.h` in namespace `grungr::ui`, not the `tools/iplug2-ui/DODGrunge*` copies)
- **Others**: Simple `.ico` icons, no special embedding

### 4. Faust Sources Need Explicit Integration
- `.dsp` files compile to C++ via `faust` CLI — **not** auto-included in builds
- `Freeze95/test_*.dsp` = standalone test fixtures (compile via Faust IDE only)
- `Grungr/Grungr.dsp` compiles to `DSP/GrungrFaustDSP.h`; the iPlug2 wrapper is `Grungr.cpp`/`Grungr.h` (class `Grungr`) at project root — `tools/iplug2-ui/DODGrunge*` copies are stale references

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
| Grungr | Faust→iPlug2 | 2-2 | `config.h` | VS: `Grungr-vst3` / `Grungr-clap` projects |
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
- For unblocking a stalled CI on a private repo when GitHub Actions is blocked at the account level, **flip the repo public temporarily and delete the README** rather than upgrading the plan or swapping CI providers. After re-triggering and confirming green, flip back to private. The public-flip trick works for **spending-limit** holds (Free-plan minutes exhausted) and for **payment-method-attached** accounts; it does NOT work for accounts with an **unpaid balance** ("recent account payments have failed" error) — only paying the bill unblocks those.
## Learned Workspace Facts

- **iPlug2 API gotchas**: `GetParam()->Set()` alone does NOT notify VST3 host — use `IGEditorDelegate::SendParameterValueFromUI` with `BeginInformHostOfParamChangeFromUI`/`SendParameterValueFromUI`/`EndInformHostOfParamChangeFromUI` gesture. `LoadSVG` has 2 overloads — passing 2 string args treats 2nd as `units`, resource lookup silently fails; always call `g.LoadSVG(filename)` with a single arg for embedded SVGs. `IRECT` has `Intersects(other)` for hit-test, NOT `Overlap()`. Control's own rect is the public `mRECT` member on `IControl` — there is no `GetMouseRect()` accessor. `IMouseMod` exposes only `L/R/S/C/A` flags — there is NO `mod.N` click-count; double-click-to-reset requires overriding the virtual `void OnMouseDblClick(float x, float y, const IMouseMod& mod)` on your control class. `IParam` has NO `Reset()` method — inside a control override do `SetValueFromUserInput(GetParam()->GetDefault(true))` (chains through `SetDirty(true)` for redraw).
- **iPlug2 resource embedding**: `main.rc` uses `X_FN TTF/SVG X_FN` where config.h defines `#define X_FN "file.ext"`. vcxproj needs `<ResourceCompile Include="..\resources\main.rc" />`. Use 2-arg `LoadFont(id, X_FN)` for embedded TTFs. RC only watches main.rc timestamp — after changing an embedded asset, run `touch main.rc` + delete `build-win/.../int/main.res` to force re-link.
- **iPlug2 framework macOS-CI gotchas (two distinct flavors)**: (a) unguarded i386 intrinsics in `iPlug2/IPlug/IPlugAPIBase.cpp` (and possibly other framework files) break Apple Silicon clang 17 (`__builtin_ia32_punpckhdq` from `<mmintrin.h>`) — quick fix drop `-arch arm64`, root-cause fix per-plugin `scripts/patch-iPlug2-for-{plugin}.sh` adding `#ifdef __SSE__` guards before the offending intrinsics. (b) `iPlug2/common-mac.xcconfig` defines `IGRAPHICS_LNK_FLAGS = $(SKIA_LNK_FLAGS) $(FREETYPE_LNK_FLAGS)` expanding to nonexistent `Dependencies/Build/mac/lib/{libskia,libsvg,libfreetype,libskottie,libskparagraph,...}.a` — for NanoVG plugins (IGRAPHICS_NANOVG) hardcode `-framework Metal -framework MetalKit -framework OpenGL`, NEVER include `$(IGRAPHICS_LNK_FLAGS)` in EXTRA_LNK_FLAGS.
- **IPlugEffect template usage invariants** (three things to know before customizing): (a) The `IPlugEffect-*.plist` files in `iPlug2/Examples/IPlugEffect/resources/` ship with the framework — copy as `{PluginName}-*.plist` (matching BUNDLE_NAME from config.h), don't overwrite, or xcodebuild bails with `FileNotFoundError: ... {PluginName}-VST3-Info.plist`. Tuner pattern: `cd "$DST/resources"; for f in IPlugEffect-*.plist; do [ -f "$f" ] && cp "$f" "Grungr${f#IPlugEffect}" || true; done`. (b) iPlug2 vcxproj and pbxproj have explicit `<ClCompile>` / `PBXFileReference` entries for the single `IPlugEffect.cpp` TU — no wildcards. To add host-free DSP, ship as `<Module>.h` (forward decls + class shape) + `<Module>_impl.h` (bodies `#include`'d at bottom of public header, all methods `inline` for ODR safety). Reference: `TunerAnalysis.h` + `TunerAnalysis_impl.h`. (c) VST3 `OnRestoreState` default only calls `SendCurrentParamValuesFromDelegate()` — if DSP depends on `mSampleRate`, override and call `mDetector.Init(GetSampleRate())` + re-sync smoothing. Without it, hosts like Bitwig skip `IAudioProcessor::setupProcessing()` when SR hasn't changed since previous session (symptom: GUI loads but shows no values; deleting + re-inserting fixes it).
- **Cross-platform denormal protection with `__SSE__` caveat**: Set `_mm_setcsr(_mm_getcsr() | 0x8040)` in ProcessBlock for FTZ+DAZ. **`__SSE__` is x86/x64-only** — Clang does NOT define it on Apple Silicon (GitHub Actions `macos-26-arm64` default), so neither `<intrin.h>` nor `<xmmintrin.h>` resolves and `_mm_setcsr` is undeclared. NEON uses FPCR/FPSCR instead of MXCSR; macOS math library handles denormals at OS level. Gate BOTH the include AND the call site with a project macro (e.g. `TUNER_HAS_SSE_DENORMALS` = `((_MSC_VER && (_M_IX86 || _M_X64)) || __SSE__)`), otherwise arm64 build dies with `use of undeclared identifier '_mm_getcsr'`.
- **Apple ships bash 3.2.57 as `/bin/bash`**: any `.sh` script invoked by `shell: bash` in a workflow or by `set -euo pipefail` MUST be bash-3 compatible. Forbidden: `mapfile`/`readarray` (use `while IFS= read -r -d '' f; do ...; done < <(find -print0)`), `${!array[@]}` indirect expansion, `declare -g`, `<<<` herestrings, `sed -i` (BSD requires `sed -i ''` — write to `tmp="${f}.$$"` then `mv`). PowerShell from bash: single-quoted heredoc → `.ps1` file → `pwsh.exe -File` (don't pass `-Command` from bash). Round-8 Tuner hit the mapfile trap with `exit code 127` after only one CI run; always run `bash -n` + a synthetic-directory dry-test on bash-3-safe hosts (Linux/WSL git-bash is bash 5, so the bug never manifests locally).
- **macOS pbxproj patching essentials**: scheme target UUIDs derive from `xcshareddata/xcschemes/macOS-*.xcscheme` BlueprintIdentifier; matching PBXSourcesBuildPhase UUID shares the same 24-hex prefix (BronzeNoise-confirmed: VST3 scheme `4F9828AA140A9EB700F3FCC1` → Sources phase `4F9828B5140A9EB700F3FCC1`; CLAP `4FD8698F27206006005A5F28` → `4FD8699227206006005A5F28`; AUv2 `4F3AE17812C0E5E2001FD7A4` → `4F3AE1A212C0E5E2001FD7A4`). Use scheme xcscheme BlueprintIdentifier (or grep for scheme name in xcscheme files) as the entry point — never guess from scheme name strings in `PBXNativeTarget` (macOS scheme name strings drift). Same file UUID also appears in PBXBuildFile decls section AND Sources-phase `files = (...)` lists — disambiguate by trailing char: PBXBuildFile entries end `};`, Sources-phase list entries end `,`; anchor regex with `,$` (or `;$`). `next` in awk fires rule-once-per-line; for paired-source/insertion cases use string `replace()` with fixed-occurrence count or `str.find()` second-position lookup.
- **iPlug2 framework source-patching + UI-lifecycle pattern**: (a) Patcher scripts are awk-based (bash-3 safe, no Python/ruby/gems), read unique source-anchor UUIDs as insertion points (e.g. IPlugVST3.cpp / IPlugCLAP.cpp / IPlugAU.cpp trailing `,`), write to mktemp then `mv` for atomicity, leave `.bak`, restore from `.bak` on sanity-check failure, idempotent on marker-UUID `grep -q` short-circuit. iPlug2 cloned as **workspace-root sibling** (not nested) of plugin dir, patcher resolves via `$PROJECT_ROOT/../../iPlug2/...` and accepts `PBXPROJ_OVERRIDE` env for local layouts. (b) **CRITICAL: null out cached control pointers BEFORE every `GetUI()->RemoveAllControls()`** — both resize (`OnParentWindowResize`) AND editor-close (`OnUIClose`) override paths. The editor-close path is `CloseWindow → OnUIClose → IGraphics::~IGraphics → RemoveAllControls`, which destroys every attached control while `mInputMeter`/`mOutputMeter`/`mLatencyLabel`/`mVisControl` member pointers still point to freed memory — in-flight `OnIdle` or audio-thread `SetData` then dereferences dangling pointers and crashes. Fix: nullptr all members BEFORE RemoveAllControls in BOTH paths; let `LayoutUI()` re-assign on next open. Defensive belt: guard `OnIdle` with `ui->NControls() > 0`. BronzeNoise needed both — resize-only fix left close path crashing.
- **iPlug2 CLAP/AU build two-customer gotchas**: (a) `IPlug_include_in_plug_hdr.h` does `using Plugin = PLUGIN_API_BASE;` (where `PLUGIN_API_BASE` is `IPlugCLAP`/`IPlugAU`) — unqualified `Plugin` lookup resolves to parent template (`clap::helpers::Plugin<...>`), producing `error: type 'Plugin<...>' is not a direct or virtual base of {Class}` on both clang and MSVC. **Fix:** fully-qualify as `iplug::Plugin` in class declaration AND member-initializer (`: iplug::Plugin(info, ...)`). Do NOT use `using Plugin = iplug::Plugin;` in plugin code — same name still resolves to template. (b) **Plugin-side enum names collide with iPlug2 framework enum values** (`kReset`, `kABCompare`, `kMix`, `kStereoMode`, `kAutoGain`, `kVisualMode` are all already-defined framework enums). Prefix plugin-specific names (BronzeNoise uses `kBn*`: `kBnMix`, `kBnStereoMode`, etc.) to avoid CI build errors that may NOT show locally. (c) WDL FFT symbols (`WDL_fft_init`/`WDL_fft`/`WDL_fft_permute` from `iPlug2/WDL/fft.c`) are NOT compiled by `IPlugEffect.cpp` — patch each build target: macOS via `scripts/patch-iPlugEffect-for-bronzenoise.sh` injecting PBXSourcesBuildPhase entry using unique iPlug-path UUIDs as anchors; Windows via `scripts/patch-iPlugEffect-clap-for-bronzenoise.sh` injecting `<ClCompile Include="..\..\..\WDL\fft.c" />` (Python for safe multi-line XML). Both MUST dedup pre-existing fft.c entries first — two parallel cl.exe write-locks cause MSB8027 + C1083 Permission denied on the `.obj`. (d) **iPlug2 framework drawing API conventions**: color alpha is `IColor::WithOpacity(float)` (NOT `.WithAlpha`). Text alignment defaults to `EAlign::Near`/`EVAlign::Top` — pass no args for default; pass named values like `EAlign::Center`, `EVAlign::Middle` from `<IGraphics/IGraphicsConstants.h>`.
- **`.gitignore` whitelist vcxproj for CI + always commit `.h` alongside `.cpp`**: root `.gitignore` ignores `*.vcxproj`/`*.vcxproj.filters`/`*.vcxproj.user` to avoid sandboxed files, but CI builds need the per-plugin project file committed. Whitelist each per-plugin project explicitly: `!<PluginName>/projects/*.vcxproj` and `!<PluginName>/projects/*.vcxproj.filters`. Force-add with `git add -f` when needed. Without this, CI runners hit `MSB1009 Project file does not exist`. **Always commit `.h` AND `.cpp` together** when adding new enum values / params — a new `kBnMix` declared only in `.cpp` (forgot to stage `.h`) produces `use of undeclared identifier 'kBnMix'` in CI but not locally if the local object dir still has the previous compile. CI runs from a clean clone and reads the committed `.h`, not your local one.
- **Faust DSP gotchas (Grungr-era hard-won, five distinct traps)**: (a) `aa.*` functions from `aanl.lib` are ALREADY 2nd-order ADAA — `aa.arctan2(x) = ADAA2(EPS, f, F1, F2, x)`, `aa.hardclip2(x) = ADAA2(...)`. Do NOT re-wrap as anti-aliased variants — no-op. `aa` is just the environment identifier. (b) `co.limiter_lad_mono(LA, ceiling, ...)` ceiling is LINEAR amplitude, NOT dB — for -0.5 dBFS pass `10^(-0.5/20) ≈ 0.944`. (c) `co.peak_expansion_gain_*_db(...)` returns ONLY the GAIN signal, NOT the processed signal — for the full expander/gate chain use `co.expander_N_chan(strength, thresh, range, att, hold, rel, knee, prePost, link, _, maxHold, N)` with 12 args. (d) `pow(0.97)` parses as partial application `pow(0.97, _)` not a constant exponent — for any constant-exponent curve define a named fn `pow097(x) = x ^ 0.97` then close it. (e) Bypass/limiter/expander/makeup helpers MUST be 1-in-1-out (mono) inside the per-channel `grunge_process` chain — wrap for stereo via the OUTER `par(i, 2, ...)` in `process`. Wrapping these inside the per-channel chain causes 2-in-1-out mismatch errors.
- **3BeatProblem JS reference design**: `3BeatProblem/3-Beat-Problem-old-javascript/` is the original vanilla-JS Tone.js prototype (index.html / styles.css / uicontroller.js / audioengine.js / statemanager.js / midiexport.js / app.js) — up to 12 lanes with side-by-side circle + grid step editors, Play/Panic/BPM/Bars/Swing transport, lane tools (Mute/Solo/Copy/Paste/Clear/Fill 2/3/4/Invert/Rotate L/R/Remove), accent cycling (Soft/Mid/Hard), Undo/Redo/Mutate, session JSON import/export, MIDI export, and IndexedDB+localStorage presets. Teal palette (`#29d3b2` accent, `#0f1417` bg) and Space Grotesk + IBM Plex Mono fonts match the C++ port exactly — when porting UI changes to the iPlug2/NanoVG build, this directory is the authoritative visual reference.
