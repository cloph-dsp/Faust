---
name: faust-spec
description: Define a production-grade Faust DSP brief before coding. Use for effects, instruments, filters, dynamics, modulation, spatial DSP, and utility algorithms intended for plugin or standalone audio products.
user-invokable: true
argument-hint: target behavior, references, constraints
args:
  - name: target
    description: Desired sound or product intent
    required: false
  - name: references
    description: Optional references like recordings, plugins, or NAM captures
    required: false
  - name: constraints
    description: CPU, latency, sample-rate, and platform constraints
    required: false
---

Turn a vague idea into a measurable DSP contract before writing Faust code.

Suggested flow:
1. `/faust-spec`
2. `/faust-architect`
3. `/faust-voice`
4. `/faust-optimize`
5. `/faust-verify`

## Build the DSP Brief

1. Define product intent:
  - Product class: effect, instrument, generator, processor, analysis tool, or hybrid
  - Source or operating context: voice, instrument, bus, master, synthesis voice, spatial scene, or utility pipeline
  - User promise in one sentence: what should the user hear, feel, or be able to do?

2. Define hard constraints:
   - Supported sample rates and channel modes
   - CPU budget at 44.1, 48, and 96 kHz
   - Latency budget and whether lookahead is allowed
   - Expected automation behavior (slow sweeps vs fast modulation)

3. Define sonic and behavioral targets:
  - Core adjectives with technical interpretation (tight low end, stable image, fast transient recovery, clear modulation)
  - 2-3 no-go outcomes (harsh aliasing, unstable pitch, image collapse, pumping, zipper noise)
  - Loudness or output-matching rule for fair comparisons

## Choose Algorithm Family (Branching)

Pick one primary path and one fallback path:

- If target is nonlinear coloration:
  - Start with `conditioning -> nonlinear core -> post-shaping`.
- If target is modulation or time-based processing:
  - Define delay/state topology, modulation sources, and smoothing rules first.
- If target is dynamics-driven:
  - Define detector path, ballistics, and control law before the gain or shaping stage.
- If target is instrument, resonator, or oscillator based:
  - Define excitation, resonant structure, voice model, and pitch/control update strategy first.

Record why you chose the path. If the rationale is unclear, the design is not ready.

## Define Parameter Contract

For each exposed control, specify:

- Name and role in plain language
- Range, taper, and default
- Unit metadata (`dB`, `Hz`, `%`, ratio)
- Smoothing requirement (none, light, strong)
- Interaction notes with other controls

Parameter contract rule:
- Every parameter must have a purpose, a bounded range, and an audible test case.

## Define Acceptance Tests

Create pass/fail checks before coding:

- Tone or behavior checks:
  - Level-matched comparison against references or golden renders at representative low, medium, and high operating states.
- Artifact checks:
  - No zipper noise during automation.
  - No obvious foldback or alias fizz at bright inputs.
- Stability checks:
  - No NaN or Inf behavior at control extremes.
- Performance checks:
  - Meets CPU and latency targets in the target runtime context.

## Completion Gate

Do not move to `/faust-architect` until all are true:

- Problem statement is one sentence and specific.
- Constraints are explicit and measurable.
- Algorithm family and fallback are chosen.
- Parameter contract is complete.
- Acceptance tests are written and realistic.
