---
name: faust-voice
description: Shape and refine Faust algorithm behavior. Use for transfer curves, spectral balance, transient feel, envelope behavior, motion depth, and control calibration across effects and instruments.
user-invokable: true
argument-hint: target behavior and references
args:
  - name: target
    description: Tonal or behavioral direction (for example tight modern, diffuse lush, stable clean, soft transient)
    required: false
  - name: references
    description: Optional tracks, plugins, presets, or NAM captures
    required: false
---

Refine the algorithm from "technically working" to "perceptually convincing" with repeatable voicing moves.

## Voicing Pass Strategy

1. Calibrate listening setup:
   - Level-match comparisons before judging tone
   - Check at low, medium, and high operating states
   - Test both transient and sustained material, plus dense/polyphonic material when relevant

2. Shape harmonic, spectral, or motion signature:
   - Decide harmonic emphasis or spectral contour
   - Decide symmetric vs asymmetric, static vs time-varying behavior
   - Decide whether transient handling or modulation should be hard, soft, or blended

3. Stabilize frequency and time behavior:
   - Tighten low-end buildup or low-frequency motion when needed
   - Control high-end fizz, smear, or roughness
   - Preserve intended articulation, motion, and decay behavior

## Branching Decision Logic

- If the sound is harsh or fizzy:
  - Reduce pre-emphasis into clipping and revisit oversampling filter slope.
- If the sound is muddy or blurred:
  - Increase low-frequency control before the core block and reduce excessive energy build-up.
- If the sound is flat or lifeless:
  - Add level dependence, bias shift, richer motion, or stage blending.
- If motion is unstable or seasick:
  - Reduce modulation depth, feedback sensitivity, or control slew.
- If the sound is too compressed or pinned:
  - Decrease early-stage drive and move part of the character into later shaping.

## Control Calibration

For each audible control:

- Ensure full sweep is useful (no large dead zone)
- Use perceptual mapping where needed (log Hz, dB, psychoacoustic gain behavior)
- Verify automation smoothness and no zipper artifacts
- Verify interactions with other controls remain musical

## Perceptual Quality Checks

Validate all of these during voicing:

- Transients remain intelligible when intended
- Sustained material remains stable and well-controlled
- Dense or polyphonic material retains separation unless intentional collapse is part of the design
- Noise floor remains acceptable at extreme settings
- Bypass and engaged loudness can be level-matched where applicable

## Output Required

Produce:

- Updated voicing rationale (what changed and why)
- Revised transfer and filtering strategy
- Control sweep notes for all exposed parameters
- Before/after listening checklist results

## Completion Gate

Do not move to `/faust-optimize` until all are true:

- Tone or behavior target is met at multiple operating points.
- Extreme control settings are still musically usable.
- Harsh artifacts are controlled.
- Dynamic response matches intended feel.
- A/B level match or reference output normalization has been verified where applicable.
