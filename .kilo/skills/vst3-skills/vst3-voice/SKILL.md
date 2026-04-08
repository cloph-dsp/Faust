---
name: vst3-voice
description: Shape and refine VST3 plugin DSP behavior. Use for transfer curves, spectral balance, transient feel, envelope behavior, motion depth, and control calibration. Works with iPlug2, JUCE, Faust, and hybrid builds.
user-invocable: true
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

### 1. Calibrate Listening Setup
- Level-match comparisons before judging tone
- Check at low, medium, and high operating states
- Test both transient and sustained material
- Test dense/polyphonic material when relevant

### 2. Shape Harmonic/Spectral Signature
- Decide harmonic emphasis or spectral contour
- Decide symmetric vs asymmetric behavior
- Decide transient handling approach (hard, soft, blended)
- Document the "why" behind each choice

### 3. Stabilize Frequency and Time Behavior
- Control low-end buildup or low-frequency motion
- Control high-end fizz, smear, or roughness
- Preserve intended articulation, motion, and decay behavior

## Sonic Target Categories

### For Effects
- **Coloring effects**: character, warmth, harmonic emphasis
- **Corrective effects**: transparency, precision, minimal artifacts
- **Creative effects**: distinctiveness, expressiveness, musicality

### For Instruments
- **Oscillator character**: warmth, stability, alias rejection
- **Filter behavior**: resonance character, modulation response
- **Envelope accuracy**: attack transient, release behavior
- **Noise handling**: hiss, rumble, digital artifacts

## Branching Decision Logic

### If Sound is Harsh or Fizzy
- Reduce pre-emphasis into clipping
- Revisit oversampling filter slope
- Check for intermodulation artifacts
- Adjust soft-knee vs hard-knee behavior

### If Sound is Muddy or Blurred
- Increase low-frequency control before the core block
- Reduce excessive energy build-up
- Check phase relationships in stereo
- Verify mono compatibility if applicable

### If Sound is Flat or Lifeless
- Add level dependence or bias shift
- Introduce richer motion or stage blending
- Consider asymmetric processing
- Add subtle harmonic content

### If Motion is Unstable
- Reduce modulation depth or feedback sensitivity
- Adjust control slew rates
- Check for interaction between modulators
- Verify DC offset control

### If Sound is Too Compressed/Pinned
- Decrease early-stage drive
- Move part of character into later shaping
- Adjust ratio and threshold relationship
- Consider parallel compression

## Control Calibration

For each audible control:

- **Full sweep usability**: no large dead zones
- **Perceptual mapping**: log Hz, dB, psychoacoustic gain behavior
- **Automation smoothness**: no zipper artifacts
- **Interaction music**: controls remain musical in combination
- **Visual feedback**: what user sees matches what they hear

## Framework-Specific Notes

### iPlug2
- Parameter smoothing time affects perceived responsiveness
- Test with `OnParamChange()` callbacks
- Verify SVG animations sync with parameter changes

### JUCE
- Use `LinearSmoothedValue` or `RAMPEvaluator` for smoothing
- Check automation in various DAWs
- Test `setValueNotifyingHost()` behavior

### Faust
- `si.smoo` smoothing test at various rates
- Verify `si.polySmooth` for polyphonic applications
- Check `-fs` sample rate handling

## Perceptual Quality Checks

Validate all of these during voicing:

- **Transients**: remain intelligible when intended
- **Sustained material**: stable and well-controlled
- **Dense material**: retains separation unless intentional collapse
- **Noise floor**: acceptable at extreme settings
- **Bypass matching**: engaged loudness level-matched

## A/B Testing Methodology

For plugin comparisons:

1. **Level match**: -12 to -18 dBFS pink noise, match output levels within 0.1 dB
2. **Test material**: 3-5 diverse audio clips (transients, sustained, dense, simple)
3. **Blind testing**: use `A/B` plugin or blind toggle
4. **Take notes**: document specific differences, not just "sounds better"

## Output Required

- Updated voicing rationale (what changed and why)
- Revised transfer and filtering strategy
- Control sweep notes for all exposed parameters
- Before/after listening checklist results

## Completion Gate

Do not proceed to optimization until all are true:

- Tone target is met at multiple operating points
- Extreme control settings are musically usable
- Harsh artifacts are controlled
- Dynamic response matches intended feel
- A/B level match or reference normalization verified
