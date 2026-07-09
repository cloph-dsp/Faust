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
- User expects root-cause fixes over symptom suppression — when a bug has both a band-aid and a proper fix, fix the root cause
- **ULTRAWORK rule on CI**: After pushing a CI fix, DO NOT declare done — wait for the run to complete, fetch `gh run view --log-failed`, diagnose the next failure, fix, push, wait, iterate until both platforms are green. Repeatedly applied across multi-hour debugging sessions — the user gets frustrated if you stop after push without watching

## Learned Workspace Facts

- **iPlug2 API gotchas**: `GetParam()->Set()` alone does NOT notify VST3 host — use `IGEditorDelegate::SendParameterValueFromUI` with `BeginInformHostOfParamChangeFromUI`/`SendParameterValueFromUI`/`EndInformHostOfParamChangeFromUI` gesture. `LoadSVG` has 2 overloads — passing 2 string args treats 2nd as `units`, resource lookup silently fails; always call `g.LoadSVG(filename)` with a single arg for embedded SVGs.
- **iPlug2 resource embedding**: `main.rc` uses `X_FN TTF/SVG X_FN` where config.h defines `#define X_FN "file.ext"`. vcxproj needs `<ResourceCompile Include="..\resources\main.rc" />`. Use 2-arg `LoadFont(id, X_FN)` for embedded TTFs. RC only watches main.rc timestamp — after changing an embedded asset, run `touch main.rc` + delete `build-win/.../int/main.res` to force re-link.
- **Freeze95 patterns**: Set `mSendUpdate = true` in OnActivate to trigger `SendCurrentParamValuesFromDelegate()` in OnIdle — all non-distributed iPlug2 plugins need this for GUI to reflect host automation. Also call `SendCurrentParamValuesFromDelegate()` at end of LayoutUI for initial-open sync. Two-tier scale (`mDrawScale x mScreenScale`). SpeakerKnobControl: Lerp(135,405). TinyKnobControl: IKnobControlBase subclass. MiniManualToggleControl: EVAlign::Middle. PLUG_HEIGHT=306.
- **Tuner build + FAUST**: Sources at `Tuner VST/` root, copied to `iPlug2/Examples/IPlugEffect/` via `_local_copy.ps1`, compiled as single TU by `IPlugEffect-vst3.vcxproj`. `build_vst3.ps1` invokes msbuild with SolutionDir. Output is single-file `.vst3` (not bundle). Config.h MUST define `BUNDLE_DOMAIN`, `BUNDLE_NAME`, `PLUG_CLASS_NAME`, and a font macro for RC. FAUST constraints: output buffers sized to PLUG_CHANNEL_IO (no extras), `sample**` is `double**` (never reinterpret_cast to float*), A4 ref via `InitEnum` + `SetDisplayText`, smoothing `alpha = lerp(0.01, 0.51, 1-mSmooth)` where higher mSmooth = more smoothing.
- **macOS deployment**: 10.15+ deployment target, ad-hoc sign with `--options=runtime`, hardened runtime. macOS 15+ AUValidationService subprocess crashes with CocoaUI — guards in patched-iPlug2. `AudioUnit Version` Info.plist key has a space. auval -view/-stress removed in macOS 15.
- **Apple ships bash 3.2.57 as `/bin/bash` (refuses to ship GPLv3)**: any `.sh` script invoked by `shell: bash` in a workflow or by `set -euo pipefail` MUST be bash-3 compatible. Forbidden: `mapfile`/`readarray` (use `while IFS= read -r -d '' f; do ...; done < <(find -print0)`), `${!array[@]}` indirect expansion, `declare -g`, `<<<` herestrings. Also forbidden: `sed -i` (BSD requires `sed -i ''`, GNU accepts `-i` plain) — write to `tmp="${f}.$$"` then `mv`. Round-8 Tuner hit the mapfile trap with `exit code 127` after only one CI run; always run `bash -n` + a synthetic-directory dry-test on bash-3-safe hosts (Linux/WSL git-bash is bash 5, so the bug never manifests locally).
- **PlistBuddy `Set` cannot replace a key that has children**: `Set :Key array` fails with "Cannot Perform Set On Containers" when `:Key` already exists with subkeys. To overwrite an array/dict key, `Delete :Key` first then `Add :Key array`. For idempotent Add-then-Set fallback, use `Add :Key array 2>/dev/null || true` to swallow the "Entry Already Exists" error before further subkey Adds. Applies to AUv2 AudioComponents patching in iPlug2 plugin CI.
- **iPlug2 framework `$(IGRAPHICS_LNK_FLAGS)` references Skia libs not built in CI**: `iPlug2/common-mac.xcconfig` defines `IGRAPHICS_LNK_FLAGS = $(SKIA_LNK_FLAGS) $(FREETYPE_LNK_FLAGS)` which expands to nonexistent `Dependencies/Build/mac/lib/{libskia,libsvg,libfreetype,libskottie,libskparagraph,...}.a` files. For NanoVG plugins (IGRAPHICS_NANOVG), NEVER include `$(IGRAPHICS_LNK_FLAGS)` in the plugin's xcconfig EXTRA_LNK_FLAGS — hardcode `-framework Metal -framework MetalKit -framework OpenGL` only. Otherwise linker fails with "no such file or directory" on every Skia .a.
- **iPlug2 framework's `IPlugEffect-*.plist` already exist in `iPlug2/Examples/IPlugEffect/resources/`**: VST3/CLAP/AU/VST2/AAX/iOS/AUv3 plist templates ship with the framework. Do NOT overwrite them with your plugin's plists (the xcodeproj's INFOPLIST_FILE references them by their IPlugEffect-* names). Instead, copy them as `{PluginName}-*.plist` (matching BUNDLE_NAME from config.h, used by `prepare_resources-mac.py`). Tuner pattern: `cd "$DST/resources"; for f in IPlugEffect-*.plist; do [ -f "$f" ] && cp "$f" "Grungr${f#IPlugEffect}" || true; done`. Without this, xcodebuild bails with `FileNotFoundError: ... {PluginName}-VST3-Info.plist`.
- **iPlug2 framework has unguarded MMX/SSE intrinsics — breaks macOS arm64 build**: `iPlug2/IPlug/IPlugAPIBase.cpp` (and possibly other framework files) uses MMX intrinsics without `#ifdef __SSE__` guards. On Apple Silicon clang 17 trips on `__builtin_ia32_punpckhdq` from `<mmintrin.h>` (x86-only MMX). Quick workaround: drop `-arch arm64` from xcodebuild (build x86_64 only, Apple Silicon runs via Rosetta 2). Root-cause fix: per-plugin `scripts/patch-iPlug2-for-{plugin}.sh` following Freeze95's pattern (`Freeze95/scripts/patch-iPlug2-for-freeze95.sh`) that sed-patches framework sources to add `#ifdef __SSE__` guards before the offending intrinsics.
- **Bitwig file lock**: When Bitwig is running, system VST3 copy fails. Must quit ALL processes: Bitwig Studio, BitwigAudioEngine-X64-AVX2, up to 3x BitwigPluginHost-X64-AVX2, BitwigStudioApp.
- **BronzeNoise patterns**: ISVGKnobControl extends IKnobControlBase (no text rendering — subclass for labels). 1000x700 resizable, dark industrial. Two embedded fonts. KnobWithTextControl: dark pill, hover alpha, value pulse animation. Knob body stays static; rotate a separate pointer via `p->ToNormalized(p->Value())` — must use normalized value (0-1), not raw `p->Value()`, since Amount is 0-100%, Bandwidth 0.01-4, Character -1 to 1. Audio→UI scalar data: store peaks in `std::atomic<float>` written in ProcessBlock, load with `memory_order_relaxed` in OnIdle. Double-click reset: override `OnMouseDblClick` -> `GetParam()->SetToDefault()` + `SetValueFromDelegate(p->ToNormalized(p->GetDefault()))` + `SetDirty()`. `IKnobControlBase::IsFineControl(mod, wheel)` already does Shift=10x fine adjust; do not re-implement.
- **IParam::GetDisplay enum string**: 4-arg signature `(double value, bool force, WDL_String& str, bool asEnumDisplay=true)`. Pass `true` for the last arg to get display string (e.g. "4096"); `false` returns the raw index/number. Required when drawing segmented-selector cells — without `true`, cells show "3" instead of "4096".
- **Windows GDI rejects CFF-based OTF**: OpenType fonts with CFF outlines (magic bytes `OTTO` = `0x4F 54 54 4F`) fail to load via `AddFontMemResourceEx`, so embedded iPlug2 fonts render as fallback. For embedded TTF resources, always use TrueType (magic `0x00 01 00 00`). Convert OTF→TTF with `python -m otf2ttf.cli SRC -o DST` (fontTools + otf2ttf). Verify magic with a hex read of the first 4 bytes.
- **Custom controls used as header member types must not live in anonymous namespaces**: `class IOMeterControl;` forward decl in the public header is at file scope, but defining the class in an anonymous namespace in the .cpp creates a different type → `C2872 ambiguous symbol`. Either move the class to file scope, store as `IControl*` in the header, or wrap in a named namespace.
- **Windows x64 / macOS x86_64 denormal protection**: Set `_mm_setcsr(_mm_getcsr() | 0x8040)` in ProcessBlock to enable FTZ (bit 15) + DAZ (bit 6) for SSE/AVX. Required in iPlug2 plugins with heavy IIR/ADAA Faust processing to prevent denormal stalls. **Cross-platform caveat:** `__SSE__` is x86/x64-only — Clang does NOT define it on Apple Silicon (GitHub Actions `macos-26-arm64` image, default for `macos-latest`), so neither `<intrin.h>` nor `<xmmintrin.h>` resolves `<xmmintrin.h>` and `_mm_setcsr` is undeclared. NEON uses FPCR/FPSCR instead of MXCSR; macOS math library handles denormals at OS level — no runtime SSE flush needed on arm64. Define a project-level macro (e.g. `TUNER_HAS_SSE_DENORMALS` = `((_MSC_VER && (_M_IX86 || _M_X64)) || __SSE__)`) that gates BOTH the include AND the call site; otherwise the macOS arm64 build dies with `use of undeclared identifier '_mm_getcsr'`.
- **Faust ADAA startup NaN**: ADAA divided-difference `(G(x[n]) - G(x[n-1])) / (x[n] - x[n-1])` produces NaN when both filter state and input are zero at init (delta=0 → 0/0). Fix with a warm-up `compute(8, tiny_ramp_signal)` in OnReset() after init() + param sync. Keep NaN clamp in ProcessBlock as safety net.
- **iPlug2 z-order**: Controls render in attachment order — last-attached paints on top. If a control's Draw() fills its bounding rect with opaque content (e.g. faceplate SVG redraw on stomp engagement), it covers earlier-attached controls in the overlapping area. Attach overlays (logos, badges) AFTER the control that redraws behind them.
- **Grungr build setup**: MSBuild at `C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe`. VS BuildTools `vswhere` returns empty array — cannot rely on `vswhere` for this edition. Build vcxproj without .sln needs explicit `/p:SolutionDir="<Grungr_dir>\\"`. Postbuild script always fails on AAX icon/ARM64EC paths (exit 255) but VST3 binary is compiled before postbuild runs — ignore postbuild errors. Grungr authoritative Faust header is `Grungr/DSP/GrungrFaustDSP.h`; `Grungr/DSPGrungrFaustDSP.h` at root is stale duplicate. PLUG_CLASS_NAME=Grungr (class `Grungr : public Plugin`). macOS xcconfig at `Grungr/config/Grungr-mac.xcconfig`. Pre-existing Info.plist templates for all formats in `Grungr/resources/`. CLAP Windows output: `Grungr/build-win/clap/x64/Release/Grungr.clap`.
- **iPlug2 `Scripts/parse_config.py` is whitespace-fragile**: `extractStringElement` slices off `"#define MACRO "` (single trailing space) and treats everything between as the macro value. Any alignment whitespace on `#define` lines corrupts every macro used in paths/strings (Info.plist paths, copy destinations) — e.g. `#define BUNDLE_NAME           "Tuner"` makes `config['BUNDLE_NAME']` come out as `          "Tuner` (10 spaces + opening quote), so the script tries to read `…/resources/          "Tuner-VST3-Info.plist` → `FileNotFoundError`. Use single-space `#define MACRO "value"` only — never pad for alignment.
- **PowerShell `-Command "..."` from bash in GitHub Actions runners is fragile**: bash escape mangling of `\$h`/`\$p`/pipes/semicolons inside PowerShell scripts trips bash's `misuse-of-builtin` detection, surfacing as opaque `exit 2` (no PowerShell output reaches the log). Robust pattern: single-quoted heredoc → `.ps1` file → `pwsh.exe -File`. Single-quoting the heredoc delimiter (`<<'PSEOF'`) disables bash `$`-expansion so PowerShell variables stay literal; substitute bash-resolved paths into the script via `sed` after the heredoc closes.
- **Single-file VST3 deploys need CI steps mirroring local build layout**: local iPlug2 + Faust + IPlugUI plugins produce a ~1 MB single-file `.vst3` DLL (loose, not bundle). CI verify / load-test / stage steps must treat `.vst3` as a file, not as `.vst3/Contents/x86_64-win/` directory. 500 KB is a safe size floor; 1 MB falsely rejects valid builds. Use `Copy-Item -Force` (no `-Recurse`) when staging. The single-file shape is user-preferred and confirmed working in Bitwig alongside Freeze95's deploy.
- **iPlug2 framework bakes `AcmeInc` / `IPlugEffect` / `com.AcmeInc` template strings into compiled binaries**: these survive the plugin-name rename in `_local_copy.ps1` because they live in upstream iPlug2 framework source compiled into the binary. Removing them at the framework level requires a `scripts/patch-iPlug2-for-{plugin}.sh` that sed-replaces upstream sources before build. Freeze95 has one as the reference (`Freeze95/scripts/patch-iPlug2-for-freeze95.sh` — patches `IGraphicsMac.mm`, `IGraphicsMac_view.mm`, `IGraphicsCoreText.mm`, `IPlugAU_view_factory.mm` for crash guards). Without this script, the binary-string hygiene check (`strings | grep -ciE "IPlugEffect|AcmeInc|com\.AcmeInc"`) fails.
- **GitHub Actions workflow YAML: orphan-key trap on step deletion**: when removing a `with:` step block (e.g. deleting `Stage VST3 artifact`), any `path:` / `if-no-files-found:` lines that lived inside that block reproduce as duplicates at the next step's `with:` level. YAML parser rejects the file: `'path' is already defined (Line: X, Col: Y)`. Whole workflow refuses to load — no jobs run, no logs surface. After any workflow edit, run `git grep -n "^          path:\|^          if-no-files-found:"` to spot duplicates before commit.
- **Host-free DSP modules must be header-only when shipping into iPlug2's IPlugEffect**: iPlug2 vcxproj and pbxproj have explicit `<ClCompile>` / `PBXFileReference` entries for the single `IPlugEffect.cpp` TU — no wildcards, no glob, no auto-discovery. Adding a separate `DSP.cpp` requires editing the project file in three places (Windows `<ClCompile>`, Windows `vcxproj.filters`, macOS `project.pbxproj`) plus a 4th: the iOS/AUv2 variant. Each addition is fragile and breaks both IDEs on the next iPlug2 checkout. **Solution:** split into `<Module>.h` (forward decls + class shape) + `<Module>_impl.h` (function bodies, `#include`-d at the bottom of the public header). Mark every method `inline` for ODR safety. The host-free TU flows through as a header file copied into `iPlug2/Examples/IPlugEffect/` and `#include`-d from `IPlugEffect.cpp` — zero project-file changes. Reference: `Tuner VST/TunerAnalysis.h` + `Tuner VST/TunerAnalysis_impl.h` (commit `e663af8`).
- **iPlug2 VST3 `OnRestoreState` override needed when DSP depends on sample rate**: The VST3 state-restoration flow is `setComponentState` → `UnserializeState()` → `OnRestoreState()`. The default `OnRestoreState` only calls `SendCurrentParamValuesFromDelegate()` to push restored params to the GUI. If the DSP module has internal state that depends on `mSampleRate` (e.g. YIN/MPM pitch detectors, fixed-lag analysis), **override `OnRestoreState`** to call `mDetector.Init(GetSampleRate())` + re-sync smoothing/A4 from restored params. Without this, hosts like Bitwig can skip `IAudioProcessor::setupProcessing()` when the sample rate hasn't changed since the previous session, leaving the DSP with a stale `mSampleRate`. Symptom: GUI loads but shows no values/needle stale; deleting + re-inserting the plugin (forces fresh init) fixes it immediately. Reference: `Tuner` commit `195e4c5`.
