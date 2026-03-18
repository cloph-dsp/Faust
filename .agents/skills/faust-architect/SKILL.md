---
name: faust-architect
description: Design a robust audio-plugin DSP topology from a spec for Faust, hybrid Faust-wrapper builds, or native C++ VST development. Use for signal flow, gain staging, channel routing, control mapping, state design, and anti-alias strategy.
user-invocable: true
argument-hint: plugin architecture target and constraints
---

Translate a DSP brief into a compilable, testable plugin architecture with clear headroom, routing rules, and an explicit implementation path.

## Pick the Delivery Path First

1. Classify the implementation target:
   - Faust-first DSP authoring
   - Native C++ plugin DSP (`JUCE`, `iPlug2`, raw `VST3`/AU/AAX, or equivalent)
   - Hybrid Faust DSP core wrapped by C++ plugin code

2. State why that path fits the brief:
   - Iteration speed vs low-level control
   - Portability and framework constraints
   - UI, host integration, and state-management needs

3. Lock platform assumptions before topology work:
   - Mono, stereo, dual-mono, mid-side, or wider bus layout
   - Supported sample-rate and block-size range
   - Plugin format, framework, and latency-reporting constraints

## Build the Skeleton First

1. Establish minimal scaffolding for the chosen path:
   - Faust:
     - `import("stdfaust.lib");`
     - Global metadata (`name`, `author`, `version`, optional compile options)
     - Main `process` defined immediately, then refined
   - C++ VST:
     - A clear processor entry point (`processBlock`, `ProcessBlock`, or equivalent)
     - Explicit parameter/state container and transport access points
     - Strict separation between audio-thread DSP, state serialization, and UI/editor code
   - Hybrid:
     - Faust core boundary defined first
     - C++ wrapper owns plugin lifecycle, state, host I/O, and any non-Faust DSP glue

2. Split processing into named stages:
   - Input or excitation conditioning (trim, impedance/input conditioning, HPF/DC removal, denormal protection, trigger shaping)
   - Core processor, generator, nonlinear engine, or resonant block
   - Spectral, dynamic, spatial, or modulation shaping
   - Output conditioning, dry/wet path, voice summing, safety limiting, or routing law

3. Document gain staging per stage:
   - Nominal operating level
   - Maximum expected internal swing
   - Why clipping should or should not happen there

## Topology Decisions (Branching)

- If nonlinearity is strong or abrupt:
  - Add oversampling plan before coding detailed waveshaping.
- If the algorithm depends on envelope, state, or interaction:
  - Add detector or side-path design and smooth control law.
- If stereo behavior matters:
  - Decide early between dual-mono, linked stereo, or mid-side processing.
- If the design is time-based or resonant:
  - Define delay bounds, feedback law, and reset behavior before parameter polish.
- If the design is polyphonic or voice-based:
  - Separate voice-local DSP from global/shared DSP early.
- If phase coherence is critical:
  - Prefer minimum-phase blocks unless linear-phase is explicitly required.

## Parameter and Routing Architecture

1. Define control-to-DSP mapping:
   - UI range and taper
   - Internal mapped variable
   - Where smoothing occurs
   - Which layer owns the mapping (`Faust`, DSP class, plugin wrapper, or shared parameter model)

2. Keep costly transforms out of sample loop when possible:
   - Faust: Put expensive mapping before `si.smoo` if behavior allows.
   - Faust: Use `si.smoo` as late as practical in the control path.
   - C++: Recompute coefficients on parameter or block changes when behavior allows instead of per sample.

3. Channel-safe design:
   - Ensure mono and stereo routes are explicit (`par(i,2,...)` where needed)
   - Make dry/wet law, bypass behavior, and mono-to-stereo expansion explicit
   - Never rely on implicit routing for critical paths

## Implementation Rules

### Faust Correctness Rules

- Respect box connection constraints for `:`, `<:`, `:>`, and `~`.
- Keep delay sizes bounded for `@(...)` expressions.
- Keep GUI pathnames unique when generating repeated controls.
- Avoid naming formal parameters `floor` in helper or pattern-matching definitions; it can conflict with the primitive and break matching.

### Native C++ VST Correctness Rules

- Keep the audio thread real-time safe: no allocations, locks, file I/O, sleeps, or logging in the processing callback.
- Separate parameter transport from DSP state; sample-critical parameters get smoothing inside the DSP layer.
- Make channel layouts, bypass behavior, latency reporting, and sample-rate/block-size reconfiguration explicit.
- Guard against denormals, invalid coefficient states, and stale pointer/resource ownership.
- Keep state serialization/versioning separate from live processing logic.

### Hybrid Faust + C++ Rules

- Decide whether Faust owns only DSP math or also owns parameter declarations and ranges.
- Define the contract between the Faust core and the C++ wrapper: channel layout, parameter normalization, block boundaries, and state restore behavior.
- Validate that gain staging, oversampling, and bypass logic are not split inconsistently across the Faust and C++ layers.

## Output Required

Produce these artifacts:

- Stage-by-stage architecture summary
- Initial skeleton that compiles for the chosen path
- Parameter map table (name, range, taper, smoothing, owner)
- Known technical risks with mitigation plan

## Completion Gate

Do not move to detailed voicing or implementation polish until all are true:

- The implementation path is explicitly chosen and justified.
- Skeleton compiles without connection, routing, or threading-model errors.
- Headroom and gain staging are explicitly defined.
- Oversampling decision is documented.
- Channel topology and bypass behavior are explicit and tested for intended I/O modes.
- Parameter mapping, smoothing policy, and state ownership are complete.
