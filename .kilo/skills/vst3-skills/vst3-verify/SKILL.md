---
name: vst3-verify
description: Validate VST3 plugin quality and readiness for release. Use for compile diagnostics, artifact checks, objective and subjective evidence, automation behavior, and host compatibility testing.
user-invocable: true
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

### 1. Compile Diagnostics
- Build with warnings enabled (treat warnings as errors)
- Resolve syntax, connection, and range issues
- Verify all framework dependencies link correctly
- Test on Windows and macOS if targeting both

### 2. Functional Behavior
- Confirm expected channel or voice behavior (mono, stereo, polyphonic)
- Confirm controls stay in valid ranges and remain stable
- Confirm bypass behavior and level matching
- Test all preset save/load scenarios

### 3. Artifact Checks
- Automation sweeps for zipper noise and clicks
- Bright or high-energy stress test for alias/foldback behavior
- Extreme setting stress test for blowups or unstable states
- Test with various input levels (quiet, nominal, hot)

### 4. Performance Checks
- CPU usage under target rates and block sizes
- Memory footprint under normal operation
- No runtime spikes under fast automation
- Latency matches documented specification

### 5. Host Integration Checks
- Test in primary target DAWs
- Automation curve quality
- Preset compatibility between hosts
- Offline bounce behavior
- MIDI CC mapping if applicable

## Objective and Subjective Evidence

Collect both, not just one:

### Objective
- Spectral deltas, crest factor trends, alias-band energy
- CPU usage graphs under load
- Null test results against references
- Latency measurements

### Subjective
- Level-matched listening across diverse material
- Notes at low, medium, and high operating states
- Control responsiveness assessment
- Bypass transparency evaluation

## DAW Compatibility Testing Matrix

| DAW | Version | Automation | Presets | Bypass | Notes |
|-----|---------|------------|---------|--------|-------|
| Reaper | Latest | ✓ | ✓ | ✓ | Test all modes |
| Ableton Live | Latest | ✓ | ✓ | ✓ | Check freezing |
| Pro Tools | Latest | ✓ | ✓ | ✓ | AAX validation |
| Logic Pro | Latest | ✓ | ✓ | ✓ | AU validation |

## Artifact Testing Protocol

### Zipper Noise Test
1. Create 1kHz sine wave at -20 dBFS
2. Automate parameter from min to max over 5 seconds
3. Listen for clicks, zipper noise, or discontinuities
4. Repeat from max to min

### Alias/Foldback Test
1. Feed sawtooth wave at 10% of sample rate (e.g., 4.8kHz @ 48kHz)
2. Increase input level gradually
3. Monitor for alias artifacts above 10kHz
4. Check spectral waterfall for folding

### Stress Test
1. Set all parameters to extreme values
2. Feed dense program material
3. Monitor for blowups, NaN, or instability
4. Test at multiple sample rates

## Branching on Failures

- If tone misses target → return to voicing work
- If aliasing is unacceptable → revisit optimization strategy
- If CPU fails → return to optimization with measured bottlenecks
- If host integration fails → investigate framework-specific issues
- If functional correctness fails → review architecture decisions

## Report Format

Produce a concise verification report:

- **Pass/fail per category**: tone, artifacts, performance, stability, integration
- **Evidence summary**: specific measurements and observations
- **Open risks**: with severity assessment
- **Next actions**: prioritized fixes with effort estimates

### Severity Definitions
- **Critical**: Blocks release (crash, audio artifacts, data loss)
- **Major**: Significant user impact (poor automation, CPU spikes)
- **Minor**: Noticeable but not blocking (minor UI issues)
- **Cosmetic**: No user impact (non-critical polish)

## Release Gate

Release-ready only if all are true:

- Core tone target is met and documented
- No critical artifacts under normal use
- CPU stays within budget under stress
- Stability holds under extreme settings
- Host integration works in target DAWs
- Remaining risks are minor and explicitly accepted
