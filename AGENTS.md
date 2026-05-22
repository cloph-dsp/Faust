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