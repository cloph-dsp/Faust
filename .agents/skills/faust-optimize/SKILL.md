---
name: faust-optimize
description: Optimize and harden Faust DSP for production audio use. Use for CPU reduction, anti-alias efficiency, control-rate optimization, memory strategy, and real-time safety in plugin or standalone DSP.
user-invokable: true
argument-hint: performance target and platform constraints
args:
  - name: target
    description: What to optimize (CPU, memory, latency, alias rejection)
    required: false
  - name: constraints
    description: Target DAW, sample rates, expected block sizes, CPU budget
    required: false
---

Push the algorithm to production readiness without changing intended tone.

## Real-Time Safety First

Treat plugin or live processing as hard real-time unless the target is explicitly offline-only:

- No allocations, locks, or file I/O in audio processing callbacks.
- No UI-thread assumptions in DSP code paths.
- Handle variable block sizes and occasional zero-sample blocks safely.

## Rate-Domain Optimization

Distribute work into slower domains when possible:

1. Compile or specialization time:
   - Constants and static structure decisions
2. Init time:
   - Table builds and sample-rate-derived constants
3. Control rate:
   - Parameter mapping and expensive nonlinear transforms where possible
4. Sample rate:
   - Only what must run per-sample

Important Faust rule:
- If smoothing is needed, put expensive transforms before `si.smoo` when behavior allows, to avoid moving expensive math into the sample loop.

## Branching for Performance Bottlenecks

- If CPU is high in nonlinear core:
  - Consider tabulation (`rdtable` or tabulation helpers) for expensive unary functions.
- If delay-heavy design burns memory:
  - Revisit `-mcd` and `-dlt` strategy.
- If scalar code underperforms:
  - Benchmark vectorization paths and related Faust options.
- If alias control is expensive:
  - Rebalance oversampling ratio, filter order, and where distortion happens.
- If polyphonic or multichannel scaling is poor:
  - Split voice-local work from shared/global work and benchmark realistic voice counts.

## Robustness Hardening

- Guard against denormal slowdowns in very low-level tails.
- Verify stable behavior at extreme control values.
- Verify no NaN or Inf propagation.
- Ensure bounded delay and table indices at runtime.

## Benchmark and Compare

Use repeatable test settings:

- Sample rates: 44.1, 48, 96 kHz
- Block sizes: small, medium, large
- Input material: quiet, nominal, and hot
- Compare quality impact after each optimization pass

## Output Required

Produce:

- Optimization change log (what changed, expected gain)
- CPU and memory before/after summary
- Any quality tradeoffs introduced
- Recommended compile options for current target

## Completion Gate

Do not move to `/faust-verify` until all are true:

- Real-time safety constraints are respected.
- CPU and latency targets are met or explicitly documented as unmet.
- No stability regressions at stress settings.
- Quality delta from optimization is reviewed and accepted.
