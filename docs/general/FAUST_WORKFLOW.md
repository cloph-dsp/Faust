# Faust DSP Workflow

## Quick Start

### 1. Edit .dsp File
```bash
cd Grungr
edit Grungr.dsp
```

### 2. Compile & Test in Faust IDE
- Open https://faustide.grame.fr
- Paste or upload `.dsp` file
- Test audio in browser
- Export as C++ when ready

### 3. Generate C++ for iPlug2
```bash
faust -a iPlug2-GUI.cpp Grungr.dsp -o Grungr.cpp
```

Or for standalone test:
```bash
faust Grungr.dsp -o Grungr.cpp
```

### 4. Integrate into Plugin
- Add generated `.cpp` to Visual Studio project
- Wire up iPlug2 parameter controls
- Build in VS

## Common Faust Commands

| Command | Purpose |
|---------|---------|
| `faust filename.dsp` | Generate C++ code |
| `faust -svg filename.dsp` | Generate block diagram SVG |
| `faust -a jack-gtk.cpp filename.dsp` | JACK GTK application |
| `faust -a iPlug2-GUI.cpp filename.dsp` | iPlug2-compatible with UI |
| `faust -a minimal-cpp.cpp filename.dsp` | Minimal C++ wrapper |

## Freeze95 Pattern

1. Edit `.dsp` → test in Faust IDE
2. Generate C++: `faust test.dsp -o test.cpp`
3. Compare against test harness in `test.cpp`
4. No manual iPlug2 integration — standalone test only
5. `test_*.dsp` are reference implementations

## Grungr Pattern

1. Edit `Grungr.dsp`
2. Run A/B harness: `python tools/grunge_ab_harness.py`
3. Update iPlug2 UI code if needed (`tools/iplug2-ui/`)
4. Embedded assets: `tools/generate_embedded_assets.py`
5. Build iPlug2 plugin project

## Testing Faust Code

### Faust IDE (Recommended)
- Real-time compilation and testing
- Built-in audio routing
- Parameter automation
- Export as C++, WebAudio, etc.

### Standalone C++ Test (Freeze95 style)
```bash
# Compile test.cpp with generated DSP
cl /EHsc test.cpp /o test.exe
./test.exe
# Compare output against reference
```

## DSP-to-iPlug2 Integration

### Manual Wiring (DataBend style)
1. Generate Faust C++ (or write DSP manually)
2. Create iPlug2 parameter declarations in `config.h`
3. Wire Faust controls to iPlug2 parameters
4. Add UI controls in iPlug2 editor

### Automatic (Faust -a iPlug2)
1. `faust -a iPlug2-GUI.cpp dsp.dsp -o plugin.cpp`
2. Add to VS project
3. Build

## Parameter Mapping

### Faust → iPlug2
```faust
// Faust
grunge_knob = hslider("[1]Grunge[style:knob]", 0.70, 0, 1, 0.01);
```

```cpp
// config.h (iPlug2)
#define PLUG_NPARAMS 6
// Parameter ranges, defaults handled in DSP or UI
```

### Control Styles
- `style:knob` — Rotary knob (default mapping)
- `style:switch` — Toggle button
- `style:slider` — Linear slider
- `style:vslider` — Vertical slider
- `style:hslider` — Horizontal slider

## Debugging

### Check Generated C++
```bash
faust -tg Grungr.dsp  # Print DSP structure
faust -dsp Grungr.dsp # Debug output
```

### Verify Signal Flow
- Use Faust IDE's signal flow view
- Check with `-svg` for block diagram

### Numerical Issues
- Watch for denormals: add `ftz 1` option
- Check sample rate handling: `fSampleRate` vs `fConst0`
- Buffer sizes: Faust vector size vs plugin block size

## Optimization

### Faust Compiler Flags
```bash
faust -O4 Grungr.dsp  # Aggressive optimization
faust -vs 64 Grungr.dsp  # Vector size
faust -ftz 1 Grungr.dsp  # Flush to zero
```

### Manual Optimizations
- Pre-compute constants in `instanceConstants()`
- Use SIMD where available
- Avoid branches in audio thread
- One-pole smoothing for parameter changes

## Documentation

### File Headers
```faust
// Plugin Name
// Report-driven Faust model centered on...

declare name "Grungr";
declare author "cloph";
declare version "2.7";
```

### Metadata
```faust
m->declare("platform.lib/name", "Generic Platform Library");
m->declare("filters.lib/name", "Faust Filters Library");
```