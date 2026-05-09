# Freeze95

**Band-Split Quantum Stutter / Freeze — VST3 for Windows**

[//]: # "Insert product screenshot/teaser here — 1280x720 PNG recommended"

A creative glitch/freeze effect that splits your audio into three independent frequency bands and subjects each one to its own stochastic stutter pattern. Like three glitch pedals in parallel, each with its own personality, all housed inside a retro CRT monitor.

---

## Sound

Freeze95 doesn't just repeat audio — it deconstructs it. The signal is split into low, mid, and high bands via a phase-coherent Linkwitz-Riley crossover (220 Hz / 2200 Hz). Each band runs its own independent stutter processor with randomized capture length, repeat rate, window shape, and effect mode (normal / reverse / slow / granular).

**CHAOS** controls the intensity — from subtle shimmer at low settings to complete structural meltdown at maximum. **LO-FI** adds band-aware bit crushing and sample-rate reduction, heavier on the highs, gentler on the lows. Tempo sync snaps stutter rates to your host BPM, or go free-running for polyrhythmic mayhem.

The output runs through a soft saturator, lookahead limiter, and DC blocker — so even at extreme settings, the levels stay under control.

---

## Features

- **3-band LR4 crossover** (220 Hz / 2200 Hz) — independent processing per band
- **Stochastic stutter engine** — each band gets its own random generator for capture position, repeat rate, window shape (Hann / triangle / exponential), and effect mode
- **4 effect modes per band** — Normal, Reverse, Slow-motion, Granular — with smooth crossfades between modes
- **Host BPM sync** or free-running tempo (20–300 BPM manual range)
- **Band-aware lo-fi processing** — bit crushing with compansion, sample-rate reduction, tilt EQ — applied per band with intensity proportional to frequency
- **Stereo modulation** — decorrelated delays, pseudo-random panning, and occasional stereo cross-blend
- **Brickwall limiter + soft saturation + DC blocker** on output
- **6.5-second capture buffer** — ample room for sustained freeze textures
- **Retro CRT monitor UI** — custom vector-drawn hardware aesthetic with interactive knobs, power LED, and scan-line overlay
- **Host resizable** — from 65% to 200% (420×153 to 1680×612)
- **Zero latency** — no FFT processing, no lookahead beyond host block size

---

## Parameters

| Control | Range | What It Does |
|---------|-------|--------------|
| **CHAOS** | 0–100% | Stutter intensity: capture jitter, repeat rate, effect mode probability, gap frequency |
| **LO-FI** | 0–100% | Per-band bit reduction, sample-rate crushing, compansion, and tilt EQ boost |
| **BPM** | 20–300 | Manual tempo (when in Manual mode); host sync via toggle |
| **Sync** | H / M | Toggle between host tempo sync and manual BPM |
| **Power** | On / Off | Bypass the effect (passes dry signal through) |

CHAOS state labels: TAME → SHIFT → FRACTURE → MELT
LO-FI state labels: CLEAN → WARM → GRAIN → CRUSH

---

## System Requirements

| Requirement | Minimum |
|-------------|---------|
| **OS** | Windows 10 or 11 (64-bit) |
| **Format** | VST3 |
| **CPU** | Intel Core i5 / AMD Ryzen 5 or better |
| **RAM** | 128 MB (plugin), ~48 MB for delay buffers |
| **DAW** | Any VST3-compatible DAW (Reaper, Ableton Live, Studio One, FL Studio, Cubase, etc.) |
| **Display** | 1280×720 or larger recommended for full UI at default size |

**Note:** ~48 MB of the plugin's memory footprint is used for the 6.5-second stereo delay buffers. With multiple instances, memory scales linearly.

---

## Installation

1. Download the `.zip` from Gumroad
2. Extract `Freeze95.vst3` from the archive
3. Copy `Freeze95.vst3` to your system VST3 folder:
   - `C:\Program Files\Common Files\VST3\`
4. Rescan plugins in your DAW
5. Find Freeze95 under the **Modulation** category (or search for "Freeze95")

No installer, no license file, no registration. Just drag and drop.

---

## Tips

- **Start with CHAOS at 30-40% and LO-FI at 0%** — hear the stutter engine clean before adding coloration
- **Enable Sync** for tempo-locked glitch patterns; **disable Sync** for drifting, organic textures
- **Crank CHAOS to 100%** with moderate LO-FI for full spectral meltdown — the limiter keeps it controlled
- **Use on drums** for instant glitch-hop or breakcore textures
- **Use on pads or vocals** with low CHAOS for subtle spectral flutter
- **Automate the Sync toggle** — switching between host and manual mid-phrase creates jarring tempo shifts
- **Multiple instances** on different tracks with different chaos seeds = polymorphic glitch textures

---

## Known Limitations

- **Windows VST3 only** — no macOS, no AU/AAX in v1.0
- **Single precision DSP** — sufficient for creative effect, not for mastering
- **No factory presets** — all state persists in your DAW project; tweak and save
- **Host bypass is independent** of the Power button — both produce dry output

---

## Changelog

### v1.0.0 — Initial Release
- 3-band LR4 crossover with independent stutter processors
- Stochastic effect modes: normal, reverse, slow, granular
- Host-synchronized or free-running tempo
- Band-aware lo-fi processing with compansion
- Stereo decorrelation and dynamic panning
- Output limiter, soft saturator, DC blocker
- Custom retro CRT vector UI with host resize
- Zero-latency processing

---

## Credits

**DSP:** Faust 2.85.5  
**Framework:** iPlug2  
**Design & Code:** cloph  
**Contact:** pedrocastromusic@gmail.com  

Freeze95 v1.0.0 — Copyright 2026 cloph
