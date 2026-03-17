
# **Faust DSP Experiments and Plugins**

This repository features DSP experiments and creative audio tools built with [Faust](https://faust.grame.fr/), a language for high-performance real-time audio processing.

---

## **🌌 Spectral Blur** ✨ NEW

**Spectral Blur** (`BLUR/Spectral/spectralblur.cpp`) is now a real C++ overlap-add spectral processor. It smooths frame-to-frame spectral change inside a proper STFT resynthesis path, so the sound hangs in time like a glassy afterimage instead of behaving like a resonant or saturating filterbank.

### **Key Features**
- **Glassy Ghostly Effect**: Past audio content persists as faint echoes
- **True Temporal Spectral Blur**: Low-pass filters spectral change from one frame to the next
- **Multi-Variance Modes**: LFO, random, amplitude-linked, and inverted modulation
- **Frequency Range Control**: Selectively blur or preserve bass/treble
- **Phase Randomization**: Optional diffuse, crystalline stereo image
- **Real FFT Size Control**: 512 - 65536 values for larger or smaller STFT windows

### **Implementation Notes**
- Uses a **phase-vocoder-style STFT overlap-add core** in portable C++
- **Magnitude and phase advance are both smoothed** for bins inside the selected blur range
- **Dry signal is latency-aligned** before wet/dry mixing
- **Optional phase decorrelation** widens the wet image without turning the effect into obvious distortion
- No Faust dependency in the canonical implementation

### **Usage**
1. Integrate `BLUR/Spectral/spectralblur.h` and `BLUR/Spectral/spectralblur.cpp` into your host or plugin project
2. Full documentation in `BLUR/Spectral/spectralblur.md`

### **Audio Character**
- **0% Blur**: Original signal passes through
- **50% Blur**: Moderate ghosting effect, past/present blend smoothly
- **100% Blur**: Maximum blur, sustained drone-like textures

[📖 Full Spectral Blur Documentation](./BLUR/Spectral/spectralblur.md)

---

## **Pandemonium**

**Pandemonium** is a feedback delay DSP with integrated panning, designed to create dynamic and evolving spatial effects.

Automate the panning parameter for dynamic effects. Remember to use **internal panning**, not DAW/host panning.  

### **Panning Modes**
Pandemonium offers five distinct panning styles:
- **Simple Motion**: A single left-to-right pan.  
- **Recursive Motion**: Repeated panning across feedbacks.  
- **Reverse Motion**: Mirrors the pan direction (e.g., left-to-right becomes right-to-left).  
- **Asynchronous Ping-Pong**: Uneven, offbeat stereo shifts.  
- **Randomized Panning**: Includes three variations:  
  - Fixed intervals.  
  - Synced to delay time.  
  - Noise-driven smooth variation.

---

### **Usage**
- Test, tweak it and compile it directly in the [Faust IDE](https://faustide.grame.fr).

---

Stay tuned for more creative DSPs and experiments!


# LR8 Stereo Shifter - Faust Implementation

This is a Faust DSP implementation of the Pure Data patch `LR8-new.pd`, which creates a stereo crossfade/panning effect with tempo-synchronized modulation.

## Functionality

The plugin performs the following operations:

1. **Frequency Splitting**: Separates the stereo input into low and high frequency bands
2. **Tempo-Synced LFO**: Creates a tempo-synchronized oscillator for modulation
3. **Stereo Crossfading**: Shifts left and right channel content based on the LFO
4. **Delay Enhancement**: Applies delay processing to the high-frequency content
5. **Signal Recombination**: Mixes the processed high frequencies with unprocessed low frequencies

## Parameters

- **On**: Enable/disable the effect (bypass)
- **Host Sync**: Sync to host tempo (currently for UI only)
- **BPM**: Base tempo for the LFO (1-10000 BPM)
- **Rate**: Power-of-2 multiplier for tempo (0-14, where 7 = 1x, 8 = 2x, 6 = 1/2x, etc.)
- **Low Pass**: Frequency cutoff for the low-pass filter (20-1000 Hz)
- **High Pass Enhanced**: Frequency cutoff for the high-pass filter (1000-20000 Hz)
- **Cross**: Amount of crossfading between left and right channels (0-1)
- **Seconds**: Delay time parameter for high-frequency enhancement (1-300ms)

## Rate Values Explanation

The Rate parameter uses power-of-2 subdivisions/multiplications of the base BPM:

- 0 = 1/64 (very slow)
- 1 = 1/32
- 2 = 1/16
- 3 = 1/8
- 4 = 1/4
- 5 = 1/2
- 6 = 1x (base BPM)
- 7 = 2x (default)
- 8 = 4x
- 9 = 8x
- 10 = 16x
- 11 = 32x
- 12 = 64x
- 13 = 128x
- 14 = 256x (very fast)

## Usage

To compile this Faust code:

```bash
# Compile to various targets
faust LR8_stereo_shifter.dsp                    # Generate C++ code
faust -a jack-gtk.cpp LR8_stereo_shifter.dsp    # JACK application with GTK GUI
faust -svg LR8_stereo_shifter.dsp                # Generate block diagram
```

## Technical Details

The implementation closely follows the Pure Data patch structure:

- Uses the same power-of-2 rate multipliers as the original hradio
- Implements equivalent low-pass and high-pass filtering
- Recreates the delay-based enhancement processing
- Maintains the same parameter ranges and default values
- Preserves the stereo crossfading algorithm

The effect is particularly useful for creating rhythmic stereo movement and spatial effects in music production.