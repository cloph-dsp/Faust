# Per-Plugin Quick Guides

## Freeze95 — Faust Delay/Freezer
**Location:** `Freeze95/`  
**Type:** Faust-generated DSP (IPlug2 wrapper)  
**I/O:** 2-2 stereo  
**UI:** Yes (iPlug2)

### Build
```cmd
cd Freeze95
# Open Visual Studio: projects\Freeze95-vst3.vcxproj (if exists)
# OR: Use main solution with all plugins
```

### Test Artifacts
- `test_*.dsp` — Faust source test fixtures
- `test_*.h` — Generated C++ test code
- `test.cpp` — Standalone test harness
- Compile tests via Faust IDE: https://faustide.grame.fr

### Resources
- Fonts: `resources/*.ttf` → copied to `Contents/Resources/`
- SVGs: embedded in DLL via `resources/main.rc`

### Output
- `IPlugEffect.vst3` — Built plugin

---

## DataBend — Spectral Modulation
**Location:** `DataBend/`  
**Type:** iPlug2 VST3  
**I/O:** 2-2 stereo  
**Config:** `config.h`

### Build
```cmd
cd DataBend\projects
msbuild DataBend-vst3.vcxproj /p:Configuration=Release /p:Platform=x64
```

### Config.h Key Defines
- `PLUG_NAME "DataBend"`
- `PLUG_CHANNEL_IO "2-2"`
- `PLUG_WIDTH 980`, `PLUG_HEIGHT 560`
- `PLUG_LATENCY 0`
- `PLUG_HAS_UI 1`

### Dependencies
- `$(IPLUG2_ROOT)` — iPlug2 framework (VST3 SDK included)
- No external libs beyond iPlug2

### Output
- `build-win/vst3/x64/Release/DataBend.vst3`

---

## Grungr — DOD Grunge Model (Faust → iPlug2)
**Location:** `Grungr/`  
**Type:** Faust DSP → iPlug2 analysis harness + plugin  
**I/O:** 2-2  
**Special:** Report-driven model (based on DOD Grunge pedal measurements)

### Build (iPlug2 UI)
```cmd
cd Grungr\tools\iplug2-ui
# Open DODGrungeIPlug2.vcxproj in Visual Studio
```

### DSP (Faust)
```cmd
cd Grungr
faust -a iPlug2/Grungr.cpp Grungr.dsp  # Generate iPlug2-compatible code
```

### Analysis Harness
- `analysis/` — A/B validation against real DOD pedal captures
- `tools/grunge_ab_harness.py` — Runs comparisons
- Results: `analysis/grunge_ab_summary.md`

### Embedded Assets
- SVGs → C++ via `tools/generate_embedded_assets.py`
- Output: `tools/iplug2-ui/EmbeddedAssets.h`

### UI Spec
- `.impeccable.md` — Design context: gritty, vintage, utilitarian pedal aesthetic

---

## SpectrumMatch — Russet Noise
**Location:** `SpectrumMatch/`  
**Type:** iPlug2 VST3  
**I/O:** 2-2  

### Build
```cmd
cd SpectrumMatch\projects
msbuild RussetNoise-vst3.vcxproj /p:Configuration=Release /p:Platform=x64
```

---

## MetallicSnare — Knob Plugin
**Location:** `MetallicSnare/`  
**Type:** iPlug2 VST3  
**Key:** Custom metallic knob assets

### Build
```cmd
cd MetallicSnare\MetallicKnobs\projects
msbuild MetallicKnobs-vst3.vcxproj /p:Configuration=Release /p:Platform=x64
```

---

## Sore Throat — Multi-Target (VST2/VST3/AAX/App)
**Location:** `Sore Throat/VChorus/`  
**Type:** iPlug2 multi-format  
**Targets:** VST2, VST3, AAX, Standalone app

### Projects
- `SoreThroat-vst3.vcxproj` — VST3
- `SoreThroat-vst2.vcxproj` — VST2
- `SoreThroat-aax.vcxproj` — AAX
- `SoreThroat-app.vcxproj` — Standalone

### Build All
```cmd
cd Sore Throat\VChorus\projects
msbuild SoreThroat.sln /p:Configuration=Release /p:Platform=x64
```

### Note
- Builds multiple formats simultaneously
- Each format has separate post-build deploy logic
- More complex than single-format plugins

---

## Stand-Alone Faust Experiments
**Location:** Root `*.dsp` files  
**Type:** Faust DSP only (no iPlug2 wrapper)

### Files
- `Pandemonium.dsp` — Feedback delay + panning
- `LR8_stereo_shifter.dsp` — Tempo-synced stereo crossfade
- `3dpanner.dsp`, `Granul8or.dsp`, etc.

### Compile
```bash
faust Pandemonium.dsp -o Pandemonium.cpp          # C++ code
faust -a jack-gtk.cpp Pandemonium.dsp -o gui.cpp # JACK app
faust -svg Pandemonium.dsp                       # Block diagram
```

### Test
Open in [Faust IDE](https://faustide.grame.fr) for real-time testing

---

## BLUR (Spectral Blur Processor)
**Location:** `BLUR/`  
**Type:** C++ STFT processor (no Faust)  
**Special:** Full release audit trail, deployed VST3

### Core
- `BLUR/Spectral/spectralblur.cpp` / `.h` — Overlap-add STFT processor
- No Faust dependency
- Uses phase-vocoder-style resynthesis

### Build Status
- ✅ **Release x64 built & deployed** — `SpectralProtocol.vst3`
- `BLUR/RELEASE_AUDIT_2026-04-04.md` — Full audit (18.5/20)
- `BLUR/RELEASE_READINESS.md` — Release checklist
- `BLUR/PERFORMANCE_AUDIT.md` — CPU/latency analysis
- `BLUR/POLISH_AUDIT.md` — UI/UX review

### Build Logs
- `BLUR/build_m.txt` — MSBuild output with post-build variables
- `BLUR/build_out.txt` — Build output details
- `BLUR/copy_vst_log.txt` — Deployment log

### Key Metrics (from audit)
- **CPU:** 2–5% @ 44.1kHz stereo
- **Latency:** ~186ms
- **UI:** Responsive layout with token-based theming
- **Status:** Release-ready, minor DAW resize validation pending

### Deployed Binary
`SpectralProtocol.vst3` in system VST3 folder

### Post-Build Notes
- Bundler occasionally flaky (MSB3073) — manual deploy works 100%
- Uses iPlug2 post-build scripts (`create_bundle.bat`)
- VST2/AAX targets configured but not always built

---

## BLUR — Spectral Processor
**Location:** `BLUR/`  
**Type:** C++ STFT processor (not Faust)  
**Special:** Release-ready build with extensive audit trail

### Core
- `BLUR/Spectral/spectralblur.cpp` / `.h` — Overlap-add STFT
- No Faust dependency

### Documentation
- `RELEASE_READINESS.md` — Release checklist
- `RELEASE_AUDIT_2026-04-04.md` — Full audit (18.5/20)
- `PERFORMANCE_AUDIT.md` — CPU/latency analysis
- `POLISH_AUDIT.md` — UI/UX review
- `.impeccable.md` — Design context

### Build Status
- ✅ Release x64 built and deployed
- ✅ DAW integration pending (responsive layout validation)
- ⚠️ Post-build bundler occasionally flaky (use manual deploy)

### Copy Command (Manual Deploy)
```cmd
copy /y Release\SpectralProtocol.vst3 "C:\Program Files\Common Files\VST3\"
```