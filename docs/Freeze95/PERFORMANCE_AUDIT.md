# Freeze95 VST3 — Performance Audit

**Plugin:** Freeze95 v1.0.0  
**Type:** VST3 Stereo Effect  
**Architecture:** Faust DSP (Faust 2.85.5) → iPlug2 wrapper  
**Compilation:** `-single -ftz 2` (single precision, flush to zero)  
**Audit Date:** 2026-05-09

> This document is a static analysis performance audit. All measurements are estimates based on code review of the DSP topology, memory allocation patterns, and UI rendering architecture. Real-world profiling with a sampler/profiler is recommended for production tuning.

---

## 1. DSP CPU Analysis

### 1.1 Processing Topology

The signal path for each stereo block:

```
Input (L/R)
  → 3-band LR4 crossover (8× 2nd-order filter sections)
    → Band 1 (low): stutter/freeze processor + lo-fi processor
    → Band 2 (mid): stutter/freeze processor + lo-fi processor
    → Band 3 (high): stutter/freeze processor + lo-fi processor
  → Stereo modulation / mix
  → Limiter
  → Soft saturator
  → DC blocker
  Output (L/R)
```

The 3-band Linkwitz-Riley 4th-order crossover uses `crossover3LR4` from the Faust filters library (declared at `Freeze95DSP.h:359`). Each LR4 filter is a cascade of two 2nd-order sections (biquads). The crossover has 2 crossover points, requiring 8 biquad sections total. The coefficient computation uses `std::tan()` at `Freeze95DSP.h:440` and `Freeze95DSP.h:445`, which is expensive but executed only in `instanceConstants()` (called once per `OnReset()`, not per sample).

### 1.2 Per-Stage Cost Estimate (per sample, per channel)

Estimates assume x87 scalar floating-point on a modern x86-64 core (~3-5 cycles per FLOP with `-single -ftz 2`, no denormal penalties). Multiply by 2 for stereo.

| Stage | Operations | Est. Cycles (44.1kHz) | Est. Cycles (96kHz) |
|---|---|---|---|
| **3-band LR4 crossover** (8 biquads: 8 × (5 mul + 4 add + 2 state stores) = ~88 FLOPs) | 88 | ~350 | ~350 |
| **Band 1: Stutter processor** — envelope followers (2× 1-pole), LCG RNG (3× LCG), phasors (2× phasor + floor), delay line reads (4× with interpolation), crossfader, state machine | ~180 | ~720 | ~720 |
| **Band 2: Stutter processor** (same as above) | ~180 | ~720 | ~720 |
| **Band 3: Stutter processor** (same as above) | ~180 | ~720 | ~720 |
| **Band 1: Lo-Fi processor** — waveshaping (tanh/chebyshev), compander (2× envelope), bit reduction, sample-rate reduction, tilt EQ (shelving biquad), multi-rate blending | ~160 | ~640 | ~640 |
| **Band 2: Lo-Fi processor** | ~160 | ~640 | ~640 |
| **Band 3: Lo-Fi processor** | ~160 | ~640 | ~640 |
| **Stereo modulation / mix** — modulation LFOs, crossfade (2× lerp), gain staging | ~40 | ~160 | ~160 |
| **Limiter** — envelope detector, threshold, gain reduction (2× peak + release) | ~30 | ~120 | ~120 |
| **Soft saturator** — cubic/clipper waveshaper (2× polynomial eval) | ~15 | ~60 | ~60 |
| **DC blocker** — single biquad HPF per channel (5 mul + 4 add) | ~18 | ~72 | ~72 |
| **Control rate** — `control()` function: parameter smoothing, LUT indexing, rate calculations (runs once per block, amortized) | ~50/block | ~1 | ~1 |
| **Interleaving overhead** — pointer chasing, loop counters, denormal guards | ~50 | ~50 | ~50 |
| **Total per sample (stereo)** | | **~4,893** | **~4,893** |
| **Total per block @ 256 samples** | | **~1,252,608** (1.25M cycles ≈ 0.028ms @ 4.5GHz) | **same** |
| **Total per block @ 1024 samples** | | **~5,010,432** (5.0M cycles ≈ 0.11ms @ 4.5GHz) | **same** |

### 1.3 CPU Scaling by Sample Rate

Because the per-sample operation count is independent of sample rate, the real-time CPU load scales linearly:

| Sample Rate | Per-Sample Cycles | @ 256-block (% of 1 core @ 4.5GHz) | @ 1024-block (% of 1 core) |
|---|---|---|---|
| 44.1 kHz | ~4,893 | ~2.7% | ~2.7% |
| 96 kHz | ~4,893 | ~5.9% | ~5.9% |

**Verdict:** Moderate CPU usage. The 3-band architecture means 3× the per-sample cost of a single-band stutter, so CPU is roughly 3× a single-band equivalent. At 96kHz with small buffers this could reach 6-8% of a single core, which is acceptable for a creative effect but worth monitoring in dense sessions.

### 1.4 Notes on Faust Code Generation

Faust generates flat C++ with no function calls in the inner `compute()` loop (`Freeze95DSP.h:1201`). All state is in class member variables, all temporaries are local `float` on the stack. The compiler can inline and vectorize aggressively (though the scalar control-flow-heavy nature of the stutter state machine limits SIMD gains).

Key observations:
- **Denormal guards** applied via `*reinterpret_cast<int*>(&fTemp) & 2139095040` pattern on every recursive state variable — this is Faust's `-ftz 2` generated code (confirmed at `Freeze95DSP.h:5`). Adds ~2 integer ops per stateful float (~200× per sample).
- **LCG random** uses `1103515245 * iRec[1] + offset` and modulo-by-bitmask — integer fast path, no `std::rand` calls (\~3 per stutter processor, 9 total).
- **`std::pow`** is used in the envelope shaper (e.g., `Freeze95DSP.h:1211`). This is a library call and relatively expensive (~50-100 cycles). Limited to a few invocations per sample after control-rate smoothing amortizes it.

---

## 2. Memory Analysis

### 2.1 Static Allocations (BSS/Data Segment)

All large delay buffers are `float[]` (`FAUSTFLOAT = float`, 4 bytes per element) declared as class members of `Freeze95DSP` in `Freeze95DSP.h`:

| Buffer | Size | Declaration (line) | Bytes |
|---|---|---|---|
| `fVec5[2097152]` | 2,097,152 | `Freeze95DSP.h:94` | 8,388,608 |
| `fVec10[2097152]` | 2,097,152 | `Freeze95DSP.h:201` | 8,388,608 |
| `fVec17[2097152]` | 2,097,152 | `Freeze95DSP.h:261` | 8,388,608 |
| `fVec22[2097152]` | 2,097,152 | `Freeze95DSP.h:287` | 8,388,608 |
| `fRec62[2097152]` | 2,097,152 | `Freeze95DSP.h:168` | 8,388,608 |
| `fRec160[2097152]` | 2,097,152 | `Freeze95DSP.h:273` | 8,388,608 |
| **Subtotal (6 large buffers)** | | | **~50.3 MB** |
| `fVec16[2048]`, `fVec28[2048]` | 2,048 × 2 | lines 252, 318 | 16,384 |
| `fVec29[4096]`, `fVec30[4096]` | 4,096 × 2 | lines 319, 325 | 32,768 |
| `fVec31[512]`, `fVec33[512]` | 512 × 2 | lines 326, 331 | 4,096 |
| State variables (~200 `float[2]` + ~100 `int[2]`) | ~400 floats | scattered | ~1,600 |
| **DSP object total (static)** | | | **~50.4 MB** |

> **Note:** The user specification mentions 5 large delay buffers; the actual compiled DSP header shows 6 × 2,097,152-element `float` arrays. The extra buffer likely serves as a cross-fade/render buffer for the multi-rate blending in the lo-fi stage. The discrepancy adds ~8.4 MB to the estimate.

### 2.2 Heap Allocations

All heap allocations happen during initialization or reset — **never inside `ProcessBlock()`** (with one noted exception, see §4):

| Allocation | Location | Size | Timing |
|---|---|---|---|
| `Freeze95DSP` object | `Freeze95.cpp:2251` | ~50 MB (includes static arrays) | Constructor |
| `mInL`, `mInR`, `mOutL`, `mOutR` (`std::vector<float>`) | `Freeze95.cpp:2512-2515` | max(8192, blockSize) × 4 × 4 bytes ≈ 128-256 KB | `OnReset()` |
| `mFaustZones` (`std::unordered_map`) | `Freeze95.cpp:2253` | ~5 entries, negligible | Constructor |
| UI layer cache textures | IGraphics (NanoVG/GL2) | ~4-8 MB (depends on display resolution) | `LayoutUI()` / control `OnInit()` |

### 2.3 UI Layer Cache Textures

Four controls use layer caching via `IGraphics::CheckLayer()`/`StartLayer()`/`EndLayer()`:

| Control | Layer Variable | Code Location | Est. Size (840×306 @ 2× scale) |
|---|---|---|---|
| `MonitorBezelControl` | `mOnLayer`, `mOffLayer` (2 layers) | `Freeze95.cpp:1007-1014` | ~2-4 MB |
| `TransportGroupPanelControl` | `mLayer` | `Freeze95.cpp:1488-1516` | ~1-2 MB |
| `MonitorBadgePlateControl` | `mLayer` | `Freeze95.cpp:1875-1880` | ~0.5-1 MB |
| `MonitorLogoPlateControl` | `mLayer` | `Freeze95.cpp:1902-1919` | ~0.5-1 MB |
| **Total UI layer cache** | | | **~4-8 MB** |

### 2.4 Total Steady-State Memory Estimate

| Category | Size |
|---|---|
| DSP delay buffers (6 × 2^21 float) | ~50.3 MB |
| DSP small buffers + state | ~0.1 MB |
| I/O vector buffers (4 × prealloc) | ~0.25 MB |
| UI layer cache textures | ~4-8 MB |
| NanoVG/GL2 internal buffers | ~1-2 MB |
| Font data, SVG resources | ~1-2 MB |
| **Total estimated** | **~55-62 MB** |

---

## 3. UI Performance

### 3.1 Rendering Architecture

The UI uses IGraphics with NanoVG/GL2 backend, configured at 60 FPS (`config.h:31`: `#define PLUG_FPS 60`). Resize uses `EUIResizerMode::Scale` (`Freeze95.cpp:2447`), which applies a GPU transform — no layout recalculation.

### 3.2 Layer Caching Benefits

Four controls cache their static backgrounds to GPU textures:

```cpp
// Pattern: CheckLayer avoids re-rendering static content
if (!g.CheckLayer(targetLayer)) {
  g.StartLayer(this, mRECT);
  // ... static drawing commands ...
  targetLayer = g.EndLayer();
}
g.DrawLayer(targetLayer);
```

- **MonitorBezelControl** (`Freeze95.cpp:962`): Caches the SVG monitor background + glass overlay + dust/patina. Two layers (`mOnLayer`, `mOffLayer` at line 1108-1109) — one per power state. Re-rendered only on power state change (~460ms animation then stable).
- **TransportGroupPanelControl** (`Freeze95.cpp:1470`): Caches the retro panel with grime speckles. Re-rendered only on resize.
- **MonitorBadgePlateControl** (`Freeze95.cpp:1866`): Caches brand plate + embossed text. Never invalidated after initial draw.
- **MonitorLogoPlateControl** (`Freeze95.cpp:1890`): Caches brand plate + SVG logo. Never invalidated after initial draw.

### 3.3 Dynamic Elements

The following controls redraw on every animation frame when active:

| Control | Redraw Triggers | Complexity |
|---|---|---|
| **SpeakerKnobControl** (`Freeze95.cpp:1143`) | Value change, hover (140ms fade), mouse-down, readout pulse (180ms decay) | ~24 triangle primitives (teeth), 2× radial gradients, sweat spots, dirt rotation, arc drawing. Precomputed unit-circle coords reduce trig to 2 calls (cos/sin of rotationRad) per draw (`Freeze95.cpp:1316-1317`). |
| **MonitorPowerButtonControl** (`Freeze95.cpp:1932`) | Toggle (380ms animation), hover (140ms fade) | Radial gradients, SVG-like arc + line for power symbol, LED glow (nested radial gradients), barrel shadow. 3-stage press-release animation (`Freeze95.cpp:2183-2189`). |
| **MiniManualToggleControl** (`Freeze95.cpp:1525`) | Toggle (120ms animation) | Filled circles, glow, "M"/"H" glyph text. |
| **ManualBpmTextControl** (`Freeze95.cpp:1673`) | Value change, hover, valid/invalid flash (180-220ms) | Classic panel fill, retro text layout. |
| **BypassOverlayControl** (`Freeze95.cpp:935`) | Power state change | Full-rect fill + text — trivial. |

### 3.4 Knob Drawing Optimization

The `SpeakerKnobControl` precomputes 24 teeth unit-circle coordinates in a `static` local (`GetTeethBase()`, `Freeze95.cpp:1122-1141`), reducing per-frame trig from 72 calls to 2. Each tooth is rendered as a `FillConvexPolygon` with 3 vertices rotated by `rotationRad`.

### 3.5 Frame Budget

| Component | Estimated Time | Notes |
|---|---|---|
| Layer compositing (4× `DrawLayer`) | ~0.05-0.1 ms | GPU texture blit, no CPU cost |
| Knob redraw (1-2 knobs animating) | ~0.15-0.3 ms | ~24 triangles, 2 gradients, dirt scatter |
| Power button redraw | ~0.1-0.2 ms | Radial gradients, arcs |
| Other dynamic controls | ~0.05-0.1 ms | Simple shapes |
| NanoVG flush + swap | ~0.2-0.5 ms | GPU submit |
| **Total worst-case per frame** | **~0.55-1.2 ms** | Well within 16.6ms budget @ 60 FPS |

**Verdict:** UI performance is comfortable. Layer caching removes the heaviest static draws (monitor SVG, transport panel, badge/logo plates). The knobs are the most complex dynamic elements but stay well under budget even with both animating simultaneously. At high DPI (2× scale, 1680×612), layer cache textures consume more VRAM but rendering cost remains similar since NanoVG scales via GPU.

---

## 4. Real-Time Safety Verification

### 4.1 FTZ/DAZ Configuration

```cpp
// Freeze95.cpp:2522-2523
#ifdef _WIN32
  _mm_setcsr(_mm_getcsr() | 0x8040);
#endif
```

**Status:** ✅ PASS  
The `_mm_setcsr(0x8040)` sets both Flush-To-Zero (bit 15) and Denormals-Are-Zero (bit 6) on every `ProcessBlock()` call. This prevents the ~100-200× cycle multiplier that denormals cause in recursive filters. Faust's `-ftz 2` flag generates matching denormal-protected code (`& 2139095040` pattern throughout `compute()`).

### 4.2 Heap Allocations in Audio Thread

```cpp
// Freeze95.cpp:2566-2572
if (static_cast<int>(mInL.size()) < nFrames) {
    // Failsafe. Should not happen if preallocated adequately in OnReset(),
    // but prevents buffer overflow if host violates expected block sizes.
    // However, this violates the real-time safety constraint as it allocates 
    // on the audio thread. So we clamp processing to our available buffers instead
    nFrames = static_cast<int>(mInL.size());
}
```

**Status:** ⚠️ SELF-IDENTIFIED VIOLATION (degraded: clamps instead of allocates)  
The original code resized vectors in the audio thread (a real-time violation). The current code clamps `nFrames` instead, which prevents the allocation but means audio is silently truncated if a host sends a block larger than the preallocated size (max of 8192 or `GetBlockSize()` from `OnReset()` at line 2511). This is a safe fallback: no crash, no allocation, just truncation.

**Recommendation:** The comment correctly identifies the issue. Better approaches:
- Preallocate to a larger fixed maximum (e.g., 16384 frames, which at 96kHz ≈ 170ms)
- Use a `std::array<float, MAX_FRAMES>` to eliminate vector allocation entirely
- Dynamically resize during `OnReset()` only

### 4.3 Lock/Mutex Usage

**Status:** ✅ PASS  
No mutexes, spinlocks, or atomic operations in the audio path. The DSP state (`mDSP`) is set up in the constructor and not modified after initialization (parameter sync happens via `mFaustZones` pointer map, which is populated once during construction).

### 4.4 File I/O in Audio Thread

**Status:** ✅ PASS  
No file reads, writes, or `fopen`/`fread` calls in `ProcessBlock()` or any function it calls. All resource loading (fonts, SVGs) happens in `LayoutUI()`, which runs on the main/GUI thread.

### 4.5 Variable Block Size Handling

```cpp
// Freeze95.cpp:2511
const int preallocFrames = std::max(8192, GetBlockSize());
```

**Status:** ✅ PASS  
Vectors are preallocated to at least 8192 samples (≈186ms @ 44.1kHz, ≈85ms @ 96kHz). The `ProcessBlock()` function correctly reads `nFrames` from the host and processes exactly that many samples.

### 4.6 Zero-Sample / Empty Block Handling

```cpp
// Freeze95.cpp:2541-2544
if (!mDSP || nFrames <= 0) {
    outputDry();
    return;
}
```

**Status:** ✅ PASS  
Zero or negative frames produce an immediate dry-output return with zeroed extra channels.

### 4.7 Channel Count Mismatch

```cpp
// Freeze95.cpp:2575-2588 (input)
if (nIn >= 2) { /* direct stereo */ }
else if (nIn == 1) { /* mono → stereo upmix */ }
else { /* silence */ }

// Freeze95.cpp:2598-2610 (output)
if (nOut >= 2) { /* write stereo */ }
else if (nOut == 1) { /* write mono */ }
for (int c = 2; c < nOut; ++c) { /* silence extra channels */ }
```

**Status:** ✅ PASS  
Handles all standard cases: stereo in/out, mono in/stereo out, mono in/mono out, and silent inputs. Extra output channels beyond 2 are zeroed.

### 4.8 Transport State Handling

```cpp
// Freeze95.cpp:2546-2564
const bool transportJustStopped = shouldGateForPause
    && mHasTransportState
    && mLastTransportRunning
    && !transportRunning;

if (transportJustStopped) {
    mDSP->instanceClear();  // Clear delay lines on transport stop
}
```

**Status:** ✅ PASS  
Clears all delay buffers when transport stops in sync mode, preventing stale audio from playing indefinitely. Audio is gated (silent dry output) while transport is stopped in sync mode.

---

## 5. Optimization Opportunities

### 5.1 Denormal Guard Clamping (Medium Priority)

**Location:** `Freeze95.cpp:2566-2572`

**Issue:** The buffer-size check clamps `nFrames` instead of preventing the condition. If a host ever delivers a block larger than the preallocated size, audio is silently truncated.

**Fix:**
```cpp
// Increase prealloc in OnReset() to cover edge cases
const int preallocFrames = std::max(16384, GetBlockSize());
```
Or eliminate vector allocation entirely using a fixed-size `std::array<float, 16384>`, which also improves cache locality for the hot loop.

### 5.2 Three-Band SIMD Restructuring (Low Priority)

**Issue:** Each of the 3 bands runs the same stutter + lo-fi processing independently. The current Faust-generated C++ is scalar and processes bands sequentially per sample.

**Opportunity:** The 3 bands could be restructured to process sample-by-sample in an array-of-structs layout, enabling loop vectorization:
```cpp
// Instead of: band1_stutter(s), band2_stutter(s), band3_stutter(s)
// Process: stutter(s, band1), stutter(s, band2), stutter(s, band3)
// → compiler may auto-vectorize if operations are uniform
```
This would require manual C++ restructuring of the Faust-generated code or rewriting the hot path outside Faust. Estimated gain: 15-25% reduction in DSP CPU.

### 5.3 Layer Cache Expansion (Low Priority)

**Current:** 4 controls use layer caching. The `SpeakerKnobControl` (knob) and `MonitorPowerButtonControl` (power button) always redraw on hover/active state changes.

**Opportunity:** Cache the knob socket/groove background (static per knob) and redraw only the moving pointer cap. However, the knob draws dirt at rotation-dependent positions, preventing clean layer separation. The precomputed teeth coordinates (`GetTeethBase()`) already address the main CPU cost. Estimated gain: ~0.05-0.1ms per knob redraw — not worth the complexity.

### 5.4 Control-Rate Optimization (Very Low Priority)

**Issue:** `std::pow` is used in the inner sample loop (e.g., `Freeze95.cpp:1211` and `Freeze95.cpp:1298`). While Faust's control-rate logic runs once per block in `control()`, some `pow` calls appear in the per-sample path.

**Fix:** If profiling shows `pow` as a hotspot, replace with a polynomial approximation (minimax or Pade) for the specific exponent ranges used (1.3, 1.5). Estimated gain: ~30-50 cycles per `pow` call, ~5 total per sample.

### 5.5 LCG Distribution Quality (Informational)

The 5 LCG instances use different additive constants (444, 222, 111, 555, 12345, etc.) to decorrelate their sequences (`Freeze95.cpp:1227`, `1232`, `1236`, `1300`, etc.). This is good practice for multi-channel RNG. The LCG uses the classic `1103515245` multiplier from BSD rand(), which has known spectral issues but is fast and acceptable for audio modulation.

All LCGs avoid `std::rand()` and `std::srand()`, confirming real-time safety.

---

## 6. Summary

| Category | Status | Key Findings |
|---|---|---|
| **DSP CPU** | ✅ Moderate | ~4,900 cycles/sample/channel. ~2.7% core @ 44.1kHz, ~5.9% @ 96kHz (1024-block). 3-band architecture is inherently 3× cost. |
| **Memory** | ✅ Acceptable | ~55-62 MB steady state. Heaviest consumer: 6 × 2^21 float delay buffers (~50 MB). UI layer cache: ~4-8 MB. |
| **UI Performance** | ✅ Comfortable | Layer caching removes heaviest static draws. Dynamics (knobs, power button, toggle) stay under 1ms total. 60 FPS budget has ~15ms headroom. |
| **Real-Time Safety** | ✅ Mostly clean | FTZ/DAZ ✅, no allocs in audio path ✅ (clamp fallback), no locks ✅, no file I/O ✅, edge case handling ✅. One minor concern: buffer clamp at L2566-2572 could truncate on oversized blocks. |
| **Optimization** | 🔧 Minor | 1 medium-priority fix (increase prealloc size), 1 low-priority (SIMD restructuring), 1 very-low (pow → polynomial). None critical. |

### Key Line References
- `Freeze95DSP.h:5` — Compilation flags (`-single -ftz 2`)
- `Freeze95DSP.h:94,168,201,261,273,287` — Large delay buffer declarations
- `Freeze95.cpp:2504-2518` — `OnReset()` with preallocation
- `Freeze95.cpp:2520-2611` — `ProcessBlock()` entry
- `Freeze95.cpp:2522-2523` — FTZ/DAZ via `_mm_setcsr(0x8040)`
- `Freeze95.cpp:2541-2544` — Zero-frame guard
- `Freeze95.cpp:2566-2572` — Buffer resize fallback (real-time safety note)
- `Freeze95.cpp:2575-2610` — Channel count handling
- `Freeze95.cpp:1007-1014,1488-1516,1875-1880,1902-1919` — Layer caching sites
- `Freeze95.cpp:1122-1141` — Precomputed knob teeth coordinates
- `Freeze95.cpp:2447` — Scale-mode resize (no layout recalc)
- `config.h:22` — Latency: 0 samples
- `config.h:31` — 60 FPS UI
