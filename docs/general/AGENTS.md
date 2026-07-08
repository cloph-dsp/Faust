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

## Where to Look

- **Build instructions** → `docs/BUILDING.md`
- **Per-plugin guides** → `docs/PLUGIN_GUIDES.md`  
- **Faust workflow** → `docs/FAUST_WORKFLOW.md`
- **Project structure** → `docs/PROJECT_STRUCTURE.md`
- **Main README** → Root `README.md` (Faust experiments overview)

## Learned Workspace Facts

- Grungr stomp plate cave-in: faceplate SVG clipped via StartLayer, shifted down by `caveOffset = mPressT * 3.0 + mVisualDown * 1.5`, dark fill at top creates separation gap; LED stays fixed on chassis, shoe sole follows offset
- Grungr build_vst3.ps1 treats VST3 SDK header lookup failures (`string.h`, `stddef.h`, `windows.h`) as non-fatal when run from VS Build Tools without full Windows SDK — output VST3 is still produced
- Freeze95's CI build workflow (`build.yml`) only triggers on push to `main` or PR to `main` with `Freeze95/**` path filter — branch pushes alone do NOT trigger CI; use a PR or `workflow_dispatch`
- Freeze95 TinyKnobControl: custom `IKnobControlBase` subclass matching SpeakerKnobControl design language — `Lerp(135,405,value)` angle, kKnobTop fill, DrawRadialLine indicator, kCoolOn/kCoolGlow travel arc, faint full-range track, drop shadow, hover
- Freeze95 `OnParentWindowResize` Windows fixes: (a) call `RemoveAllControls()` before re-layout to prevent stale bounds; (b) divide host-provided physical size by `GetUI()->GetScreenScale()` to avoid double-scaling (`TotalScale = mDrawScale × mScreenScale`) when DPI Awareness is ON in hosts like Cakewalk Sonar — mDrawScale should only track user drag-zoom, not DPI
- Freeze95 macOS Catalina support: set `MACOSX_DEPLOYMENT_TARGET = 10.15` in xcconfig and `LSMinimumSystemVersion '10.15'` in Info.plist; iPlug2's `@available(macOS 10.14)` check is safe at 10.15
- Freeze95 layout geometry: PLUG_HEIGHT=306, transport panel y=226..258, power button y=66..274, 16px gap between panel bottoms, 32px lower margin for extra controls
- Freeze95 macOS 15+ must ad-hoc sign with `--options=runtime` (codesign on both inner binary and .vst3 wrapper); hardened runtime flag + DAW's `disable-library-validation` is sufficient — entitlements can't be embedded in ad-hoc signatures on macOS 15+ (no CMS wrapper)
- macOS identifies blocked plugins by: 1) CFBundleIdentifier (in Info.plist), 2) CDHash (code signature), 3) file path in quarantine xattr. To force macOS to treat a rebuilt plugin as "new", change `BUNDLE_MFR` in `config.h` (changes the bundle ID) AND re-sign (changes CDHash).
- Freeze95 MiniManualToggleControl sync H/M glyph: draw with `EVAlign::Middle` in `mButtonBounds.GetPadded(-2.f)` — do NOT add vertical nudges (removed `textBounds.T += 1.f` that made "H"/"M" look off-center)
- Freeze95 VST3 GUI parameter sync fix: `mSendUpdate = true` in OnActivate triggers OnIdle to call SendCurrentParamValuesFromDelegate(); UpdateParams syncs VST3 parameter container (mParameters) with iPlug parameters since setComponentState is no-op in non-distributed VST3
- iPlug2 framework source lives at `D:\Github\VST3 Developments\iPlug2` (root-level git checkout), NOT inside Freeze95/ as a submodule. Freeze95's `patched-iPlug2/` contains patched macOS IGraphics files (IGraphicsMac.mm, IGraphicsMac_view.mm, IGraphicsCoreText.mm) and AU view factory (IPlugAU_view_factory.mm), applied via `scripts/patch-iPlug2-for-freeze95.sh` — patches add @try/@catch wrappers for CI/auval robustness
- iPlug2 DPI architecture: two-tier scale — `GetTotalScale() = mDrawScale (user zoom) × mScreenScale (DPI)`. On Windows, `GetScaleForScreen()` always returns 1.0 (only iOS implements it); DPI is set via `SetScreenScale()`/`GetScaleForHWND()` in `OpenWindow()`. The default `IGEditorDelegate::OnParentWindowResize` divides physical size by `GetPlatformWindowScale()` (= `mScreenScale`) to prevent double-counting; plugin overrides must replicate this.
- IPlugAPIBase::EditorResizeFromUI: when `needsPlatformResize=false`, returns `true` unconditionally (not just when resizeView was triggered). This means `PlatformResize(parentHasResized=true)` only resizes the child window (mPlugWnd), NOT the parent host window — prevents resize feedback loops but means child may be smaller than host container when aspect ratios differ.
- VST3 view creation chain: `IPlugVST3View::attached()` → `mOwner.OpenWindow(pParent)` → platform-specific `IGraphicsMac::OpenWindow()` (or Windows/Linux variant). No try/catch in attached() itself — it just returns `kResultFalse` on unsupported platform types
- `IPlugVST3ControllerBase::Initialize()` must run (via `component->initialize()`) before `getParameterCount()` returns real values — VST3 SDK's ParameterContainer is empty until iPlug registers parameters during initialize()
- macOS 15+ AUValidationService may spawn a subprocess that tests `kAudioUnitProperty_CocoaUI` even during `auval -v`. The ObjC runtime in that subprocess is NOT initialized enough for IGraphics/Cocoa interaction, causing SIGSEGV in `OpenWindow(nullptr)`. Freeze95 fix: `patched-iPlug2/IPlug/AUv2/IPlugAU_view_factory.mm` checks main thread, NSApp existence, and wraps OpenWindow in @try/@catch — @try/@catch cannot recover from hard SIGSEGV but the check + guard prevents entering the crash path in the subprocess.
- CI auval validation runs basic test (`auval -v`). The `-view` and `-stress` flags were REMOVED in macOS 15 (return "Invalid component specifiers"). Basic `-v` already tests CocoaUI property availability. macOS 15's AUValidationService tests view creation automatically. CI FAILS if basic auval fails (previously AU failures were ignored with "Don't fail the build").
- AU Info.plist `AudioUnit Version` key has a space in it. Use PlistBuddy: `-c "Set 'AudioUnit Version' '0x00010300'"` — wrong syntax `-c "Set :AudioUnit Version 0x00010300"` creates a NEW key called `AudioUnit` with value `Version 0x00010300` as a string, leaving the real key untouched.
- L-R8 (L-R8/) is an iPlug2 stereo shifter plugin (Faust DSP), 780×420, 8 params, dark industrial palette (teal L / amber R / cool gray mid). SVG knob cap via .rc resource embed (`KNOB_CAP_FN`), loaded with `g->LoadSVG()`, drawn with `DrawRotatedSVG()` + vector fallback.
- L-R8 tooltip system: singleton `TooltipOverlayControl` with `mIgnoreMouse=true`, static `ShowTooltip()`/`HideTooltip()` methods (renamed to avoid `IControl::Hide(bool)` conflict), positioned above/below source control, drawn as dark pill with `Label: Value` text.
- L-R8 knob display mode: `KnobMode` enum (Generic/RateKnob) replaces fragile string-prefix check; RateKnob mode reads `kParamSync` to toggle between note-division and Hz display.
- L-R8 animation convention: all animations use `SetAnimation(callback, ms)` lambdas with `EC()` (cubic smoothstep) or `EQ()` (quintic-out) easing. No `std::chrono` in controls.
- L-R8 .vcxproj imports `config/LR8-win.props` via `$(SolutionDir)` — no .sln file exists. To build with msbuild from command line, set `SolutionDir` env var to the project root (with trailing backslash) and run from a VS developer prompt.
- L-R8 .vcxproj imports `config/LR8-win.props` via `$(SolutionDir)` — no .sln file exists. To build with msbuild from command line, set SolutionDir env var and also manually set WindowsSdkDir/WindowsSDKVersion env vars before calling msbuild with `/p:UseEnv=true /p:ClearDevCommandPromptEnvVars=false`. Workaround batch file: call vcvarsall.bat x64, then set WindowsSdkDir/C:/Program Files (x86)/Windows Kits/10/, WindowsSDKVersion=10.0.18362.0\, UniversalCRTSdkDir, UCRTVersion, then msbuild with UseEnv=true. Without these manual overrides, VS Build Tools + winget-installed Windows SDK fails MSB8036 because vcvarsall doesn't auto-detect the SDK.