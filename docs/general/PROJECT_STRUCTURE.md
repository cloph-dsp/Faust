# Project Structure Overview

## Repository Layout
```
VST3 Developments/
├── AGENTS.md                    # Agent guidance (this file's parent)
├── README.md                     # Main project README
├── docs/                         # Centralized documentation
├── BLUR/                         # Spectral Blur plugin
│   ├── Spectral/
│   │   ├── spectralblur.cpp      # Core DSP implementation
│   │   └── spectralblur.h
│   ├── RELEASE_AUDIT_2026-04-04.md
│   └── .impeccable.md
├── DataBend/                     # iPlug2 VST3 plugin
│   ├── config.h                  # Plugin defines (name, I/O, latency, etc.)
│   ├── DataBend.cpp/.h
│   ├── projects/
│   │   └── DataBend-vst3.vcxproj
│   └── scripts/
│       ├── prebuild-win.bat
│       └── postbuild-win.bat
├── Freeze95/                     # Faust delay/freezer plugin
│   ├── test*.dsp/.h/.cpp         # Faust test fixtures
│   ├── resources/
│   │   └── main.rc               # SVG/font embedding
│   ├── scripts/
│   │   ├── prebuild-win.bat
│   │   ├── postbuild-win.bat
│   │   └── postbuild-freeze95-resources.bat
│   └── IPlugEffect.vst3          # Built plugin
├── Grungr/                       # DOD Grunge model (Faust-based)
│   ├── Grungr.dsp
│   ├── analysis/                 # A/B validation harness
│   ├── tools/
│   │   ├── iplug2-ui/            # UI + embedded assets
│   │   └── generate_embedded_assets.py
│   └── resources/
├── SpectrumMatch/                # iPlug2 VST3 experiment
│   ├── projects/
│   └── scripts/
├── Sore Throat/                  # Multi-target (VST2/VST3/AAX)
│   └── VChorus/
│       ├── projects/             # Multiple .vcxproj targets
│       └── scripts/
├── MetallicSnare/                # iPlug2 plugin
└── [DSPLAB]*/                    # Stand-alone Faust experiments
    ├── Pandemonium.dsp
    ├── LR8_stereo_shifter.dsp
    └── ...
```

## Key Directories

### Per-Plugin Structure
Each plugin follows a consistent pattern:
- **`<PluginName>/projects/*.vcxproj`** — Visual Studio project files (x64 only)
- **`<PluginName>/config.h`** — Plugin metadata (PLUG_NAME, PLUG_CHANNEL_IO, latency, UI size)
- **`<PluginName>/scripts/prebuild-win.bat`** — Pre-build hook (usually empty)
- **`<PluginName>/scripts/postbuild-win.bat`** — Post-build: copies VST3, handles bundling, VST2/AAX deploy
- **`<PluginName>/resources/`** — Icons, fonts, SVGs (embedding method varies by plugin)

### Build Output
- **`build-win/vst3/`** — Intermediate build outputs (per configuration: Debug/Release)
- Final `.vst3` bundles deployed to system VST3 folder or local build dir

### Faust Files
- **Root `*.dsp`** — Stand-alone experiments (compile with Faust IDE)
- **`Freeze95/test_*.dsp`** — Faust test fixtures (reference implementations)
- **`Grungr/Grungr.dsp`** — Faust source for Grungr plugin

## Framework
- **iPlug2** — All C++ VST3 plugins use iPlug2 framework
- **VST3 SDK** — Via iPlug2 dependencies
- **Faust** — For DSP prototyping and some plugin implementations

## Platform
- **Windows x64 only** — All projects target `x64` platform
- **Visual Studio 2022** — PlatformToolset v143
- **No CMake** — VS solution/projects are the build system