---
name: faust-verify
description: Validate Faust DSP quality and readiness for release. Use for compile diagnostics, artifact checks, objective and subjective evidence, automation behavior, and go or no-go decisions for plugin or standalone builds.
user-invokable: true
argument-hint: verification scope and references
args:
  - name: scope
    description: What to validate (tone, artifacts, performance, integration)
    required: false
  - name: references
    description: Optional target references such as captures, tracks, or existing plugin behavior
    required: false
---

Run a strict verification pass and produce a release decision with clear next actions.

## Verification Stack

1. Compile diagnostics:
   - Build with warnings enabled where available.
   - Resolve syntax, connection, range, and duplicate-path issues.

2. Functional behavior:
  - Confirm expected channel or voice behavior (mono, stereo, polyphonic, routing mode).
   - Confirm controls stay in valid ranges and remain stable.
   - Confirm bypass behavior and level matching.

3. Artifact checks:
   - Automation sweeps for zipper noise and clicks
  - Bright or high-energy stress test for alias/foldback behavior
   - Extreme setting stress test for blowups or unstable states

4. Performance checks:
   - CPU and latency under target rates and block sizes
   - No runtime spikes under fast automation or input transients

## Objective and Subjective Evidence

Collect both, not just one:

- Objective:
  - Spectral deltas, crest factor trends, alias-band energy snapshots
  - Use offline render plus A/B harness workflow only when it exists and meaningfully helps; otherwise use null tests, scripted sweeps, spectrograms, reference renders, or host-based checks
- Subjective:
  - Level-matched listening across diverse program material
  - Notes at low, medium, and high operating states

A/B harnesses are optional accelerators, not mandatory release gates.

## Branching on Failures

- If tone misses target:
  - Return to `/faust-voice` with specific mismatch notes.
- If aliasing is unacceptable:
  - Return to `/faust-architect` or `/faust-optimize` for oversampling strategy changes.
- If CPU or latency fails:
  - Return to `/faust-optimize` with measured bottlenecks.
- If functional correctness fails:
  - Return to `/faust-architect` for routing and range fixes.

## Report Format

Produce a concise verification report:

- Pass or fail per category: tone or behavior, artifacts, performance, stability, integration
- Evidence summary for each category
- Open risks and severity
- Next command recommendation (`/faust-voice`, `/faust-optimize`, or `/faust-architect`) if not release-ready

## Release Gate

Release-ready only if all are true:

- Core tone or behavior target is met and documented.
- No critical artifacts under normal use.
- Performance and latency stay within budget.
- Stability holds under stress and edge settings.
- Remaining risks are minor and explicitly accepted.
