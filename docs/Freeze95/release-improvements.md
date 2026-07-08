# Freeze95 v1.0.0 — Release Improvement Plan

**Source:** Audits in `docs/Freeze95_RELEASE_AUDIT.md`, `docs/Freeze95_PERFORMANCE_AUDIT.md`, `docs/Freeze95_POLISH_AUDIT.md`
**Excluded:** Factory presets (SM-01 — intentional, per-user decision)
**Target:** All improvements applied, code clean, ready for public distribution

---

## Phase 1 — P2: Medium Priority (Core Stability)

### 1.1 Increase Buffer Preallocation
| Field | Value |
|-------|-------|
| **Issue** | `OnReset()` preallocates `max(8192, GetBlockSize())` — at 96kHz this is only ~85ms. Risky with oversized host blocks. |
| **File** | `Freeze95/Freeze95.cpp` — line 2511 |
| **Fix** | Change `8192` → `16384` (covers ~170ms @ 96kHz, zero cost since it's a one-time allocation in `OnReset()`) |
| **Category** | `quick` |
| **Skills** | none needed |
| **QA** | Verify clean build, confirm no regression in ProcessBlock |

### 1.2 Document Memory Footprint in System Requirements
| Field | Value |
|-------|-------|
| **Issue** | PF-01: ~48 MB static delay buffer allocation is undocumented. Users with memory-constrained systems or many instances need to know. |
| **Files** | `Freeze95/config.h` (add comment) + Gumroad README |
| **Fix** | Add `PLUG_MEMORY_FOOTPRINT` comment to config.h. Already covered in Gumroad README (separate task). |
| **Category** | `quick` |
| **Skills** | none needed |

### 1.3 Bypass: Add Native VST3 Bypass Support
| Field | Value |
|-------|-------|
| **Issue** | HI-01: Plugin uses Faust `Bypass` param for power on/off. Host's native bypass button may not stay in sync with plugin state. |
| **Files** | `Freeze95/Freeze95.cpp`, `Freeze95/Freeze95.h`, `Freeze95/config.h` |
| **Fix** | Add `IParam::SetBypass()` support. The power param already handles bypass logic (inverts to Faust Bypass). Need to wire iPlug2's native bypass to use the same param, and handle the case where the host bypasses independently of the power button. For v1.0, simplest path: add a comment that host bypass = Faust Bypass, and the power button = Faust Bypass too (they're equivalent). |
| **Category** | `unspecified-high` |
| **Agents** | `oh-my-vst` → `vst-validate` + `vst-dsp` |
| **QA** | Load in DAW, toggle host bypass vs power button — both should silence output |

---

## Phase 2 — P3: Low Priority (Enhancements)

### 2.1 Power Button Hover Fade — Fix Last-Frame Issue
| Field | Value |
|-------|-------|
| **Issue** | `MonitorPowerButtonControl` uses wall-clock timing in `Draw()` for hover fade instead of `SetAnimation()`. On fast mouse-out, last fade frame may not render. |
| **File** | `Freeze95/Freeze95.cpp` — lines 2066-2079 |
| **Fix** | Replace wall-clock hover fade with `SetAnimation()` like the other controls use |
| **Category** | `visual-engineering` |
| **Agents** | `oh-my-vst` → `vst-gui` |
| **QA** | Wave mouse over/off power button quickly — no visual glitch on fade-out |

### 2.2 Transport Aging Colors — Promote to Palette
| Field | Value |
|-------|-------|
| **Issue** | `TransportGroupPanelControl.Draw()` uses inline `IColor{N, R, G, B}` values for dust circles and scratches instead of deriving from `kShell*` palette. Code hygiene. |
| **File** | `Freeze95/Freeze95.cpp` — lines 1498-1511 |
| **Fix** | Replace inline colors with `WithAlpha(kShellDeep, N)` or similar palette-derived colors |
| **Category** | `visual-engineering` |
| **Agents** | `oh-my-vst` → `vst-gui` |
| **QA** | Visual diff at extreme resize scales — no color shift |

### 2.3 Add HAND Cursor to BPM Field on Hover
| Field | Value |
|-------|-------|
| **Issue** | `ManualBpmTextControl` doesn't set `ECursor::HAND` on `OnMouseOver()`. Users may not realize the field is interactive (clickable for text entry). |
| **File** | `Freeze95/Freeze95.cpp` — `ManualBpmTextControl` class |
| **Fix** | Add `OnMouseOver()` override that calls `GetUI()->SetMouseCursor(ECursor::HAND)` (same pattern as `SpeakerKnobControl` and `MonitorPowerButtonControl`) |
| **Category** | `quick` |
| **Skills** | none needed |
| **QA** | Hover over BPM field → cursor changes to hand |

### 2.4 Expose Smoothing Time Constants as Compile-Time Options
| Field | Value |
|-------|-------|
| **Issue** | PA-01: `fast_glide_ms=8` and `slow_glide_ms=35` are hardcoded in Faust DSP. Different parameters would benefit from different smoothing rates. |
| **File** | `Freeze95/Freeze95DSP.dsp` — lines 8-9 |
| **Fix** | Add `declare` metadata for smoothing constants or expose via config. Requires Faust regeneration. For v1.0, document as known limitation and address in v1.1. |
| **Category** | `unspecified-high` |
| **Agents** | `oh-my-vst` → `vst-dsp` |
| **QA** | Faust recompilation produces identical output at default values |

### 2.5 Dynamic CPU Scaling When Chaos/Lo-Fi at Zero
| Field | Value |
|-------|-------|
| **Issue** | PF-02: All 3-band stutter + lo-fi processors run at full complexity even when Chaos=0 or Lo-Fi=0. Could bypass processing stages when parameters are at zero. |
| **Files** | `Freeze95/Freeze95DSP.dsp` — core processor logic |
| **Fix** | Add conditional bypass in Faust: when Chaos < epsilon, bypass stutter processing and pass dry signal through band trim. When Lo-Fi < epsilon, bypass lo-fi processing. Requires Faust regeneration. For v1.0, this is non-trivial DSP restructuring — defer to v1.1. |
| **Category** | `unspecified-high` |
| **Agents** | `oh-my-vst` → `vst-dsp` + `vst-validate` |
| **QA** | Signal at Chaos=0 matches dry signal exactly; CPU drops measurably |

---

## Phase 3 — P4: Cosmetic (Code Hygiene)

### 3.1 Promote LED Colors to Named Palette Constants
| Field | Value |
|-------|-------|
| **Issue** | Power button red LED colors (`ledOff`, `ledOn`, `ledGlowOff`, `ledGlowOn`) are `static const` local to `Draw()`. Should be file-level constants in the `k` namespace for consistency. |
| **File** | `Freeze95/Freeze95.cpp` — lines 2056-2059 |
| **Fix** | Move LED colors to file-level constants (e.g., `kLedOff`, `kLedOn`) alongside `kShellBg`, `kCoolOn`, etc. |
| **Category** | `quick` |
| **Skills** | none needed |
| **QA** | Visual diff — no color change, no recompilation warnings |

### 3.2 Promote DrawEmbossedBadgeText Colors to Palette
| Field | Value |
|-------|-------|
| **Issue** | Badge face/highlight/shadow colors at lines 540-542 use hardcoded hex values instead of deriving from `kShell*` palette. |
| **File** | `Freeze95/Freeze95.cpp` — lines 540-542 |
| **Fix** | Derive `badgeFace`, `badgeHighlight`, `badgeShadow` from `kShell*` colors using `BlendColor()` |
| **Category** | `quick` |
| **Skills** | none needed |
| **QA** | Visual diff — badge text should look identical |

### 3.3 Add Snap8() to Transport Aging Coordinates
| Field | Value |
|-------|-------|
| **Issue** | Dust circles and scratch lines in TransportGroupPanelControl use raw float positions. At extreme resize scales (>2x), pixel alignment may drift. |
| **File** | `Freeze95/Freeze95.cpp` — lines 1498-1511 |
| **Fix** | Apply `Snap8()` to the position expressions |
| **Category** | `visual-engineering` |
| **Agents** | `oh-my-vst` → `vst-gui` |
| **QA** | Visual comparison at 0.65x, 1.0x, and 2.0x scale — no position jitter |

---

## Phase 4 — Documentation & Finalize

### 4.1 Create Gumroad Commercial README
| Field | Value |
|-------|-------|
| **File** | `Freeze95/GUMROAD_README.md` |
| **Content** | Product name, features, screenshots reference, system requirements (include ~48 MB RAM note), installation, developer info, license |
| **Category** | `writing` |
| **Skills** | none needed |
| **QA** | Proofread, verify all claims match actual plugin behavior |

### 4.2 Final Build & Verify
| Field | Value |
|-------|-------|
| **Action** | Clean rebuild with all changes applied |
| **Command** | `build_vst3.ps1` |
| **QA** | Zero errors, zero warnings, .vst3 output matches expected size |

---

## Execution Order (Dependency-Aware)

```
Phase 1 (P2 fixes)
  1.1 Buffer preallocation ─┐
  1.2 Memory doc comment ───┤ (independent — parallel)
  1.3 Native bypass ────────┘
       │
Phase 2 (P3 enhancements)
  2.1 Hover fade fix ───────┐
  2.2 Transport colors ─────┤
  2.3 BPM HAND cursor ──────┤ (all independent — parallel)
  2.4 Smoothing constants ──┤
  2.5 Dynamic CPU scaling ──┘
       │
Phase 3 (P4 cosmetic)
  3.1 LED colors to palette ─┐
  3.2 Badge colors to palette ┤ (all independent — parallel)
  3.3 Snap8 transport coords ─┘
       │
Phase 4 (finalize & ship)
  4.1 Gumroad README ───────┐ (parallel)
  4.2 Clean build ──────────┘
```

**Parallel waves are safe** within each phase since changes touch different code sections.

## Task Delegation Summary

| Task | Category | Skills | Est. Time |
|------|----------|--------|-----------|
| 1.1 Buffer prealloc fix | `quick` | — | 5 min |
| 1.2 Memory doc comment | `quick` | — | 5 min |
| 1.3 Native bypass | `unspecified-high` | `vst3-harden` | 1-2h |
| 2.1 Hover fade fix | `visual-engineering` | `vst3-polish` | 10 min |
| 2.2 Transport colors | `visual-engineering` | `vst3-polish` | 15 min |
| 2.3 BPM cursor | `quick` | — | 5 min |
| 2.4 Smoothing constants | `unspecified-high` | `faust-dsp` | 2h (defer) |
| 2.5 Dynamic CPU scaling | `unspecified-high` | `vst3-optimize`, `faust-dsp` | 4h+ (defer) |
| 3.1 LED colors to palette | `quick` | — | 10 min |
| 3.2 Badge colors to palette | `quick` | — | 10 min |
| 3.3 Snap8 transport coords | `visual-engineering` | `vst3-polish` | 15 min |
| 4.1 Gumroad README | `writing` | — | 30 min |
| 4.2 Clean build verify | `quick` | — | 2 min |

**Deferred to v1.1:** 2.4 (smoothing constants), 2.5 (dynamic CPU scaling)
**Apply now:** All P2 + P3(UI) + all P4 + Gumroad README
