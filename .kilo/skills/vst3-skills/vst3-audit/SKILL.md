---
name: vst3-audit
description: Run technical quality checks on VST3 plugins. Use for accessibility checks, performance audits, compatibility testing, and anti-pattern detection. Works with iPlug2, JUCE, and Faust builds.
user-invocable: true
argument-hint: "[area]"
---

Run systematic technical quality checks on VST3 plugins.

## Audit Dimensions

### 1. Real-Time Safety
- No allocations in audio thread
- No locks in process callbacks
- No file I/O during processing
- Proper handling of variable block sizes
- Guard against denormals

### 2. Parameter & Automation
- Smooth parameter automation (no zipper noise)
- Proper parameter value clamping
- Correct smoothing behavior
- Host automation curve quality
- Parameter ID stability across versions

### 3. State Management
- Preset save/load works correctly
- No state corruption on load failures
- Factory presets included and working
- User preset migration if needed

### 4. Host Integration (VST3)
- Correct plugin category
- Latency reporting accurate
- Processor/editor communication working
- Parameter changes notify host
- Bypass handling correct

### 5. Performance
- CPU within budget at all sample rates
- Memory usage stable over time
- No memory leaks during extended use
- GUI rendering performant

### 6. Anti-Patterns (GUI)

Check for dated aesthetics:
- [ ] Heavy gradients on every surface
- [ ] Embossed/beveled edges
- [ ] Fake wood or metal textures
- [ ] 3D buttons with deep shadows
- [ ] Low-contrast text
- [ ] Controls too small to use
- [ ] No visual hierarchy
- [ ] Cluttered, no grouping

Check for AI slop:
- [ ] Generic Inter/Roboto fonts
- [ ] Purple-blue gradients
- [ ] Glassmorphism panels
- [ ] Generic rounded rectangles

## Scoring

Rate each dimension 0-4:

| Score | Meaning |
|-------|---------|
| 4 | Excellent - production ready |
| 3 | Good - minor improvements possible |
| 2 | Acceptable - notable issues |
| 1 | Major problems |
| 0 | Critical - blocks release |

## Host Compatibility Matrix

| DAW | Test Automation | Test Presets | Test Bypass | Notes |
|-----|-----------------|--------------|-------------|-------|
| Reaper | ✓ | ✓ | ✓ | |
| Ableton | ✓ | ✓ | ✓ | |
| Pro Tools | ✓ | ✓ | ✓ | AAX |
| Logic | ✓ | ✓ | ✓ | AU |

## Report Format

### Health Score

| Dimension | Score | Key Issue |
|-----------|-------|-----------|
| Real-Time Safety | /4 | |
| Automation | /4 | |
| State Management | /4 | |
| Host Integration | /4 | |
| Performance | /4 | |
| GUI Quality | /4 | |
| **Total** | **/24** | |

### Severity Definitions
- **Critical**: Blocks release
- **Major**: Significant user impact
- **Minor**: Noticeable but not blocking
- **Cosmetic**: No functional impact

### Recommended Fixes

List issues in priority order with suggested commands:
- `/vst3-optimize` for DSP performance
- `/vst3-polish` for UI polish
- `/vst3-arrange` for layout issues

## Completion

- Document all findings with severity
- Provide specific fix recommendations
- Note any acceptable trade-offs
- Create action plan for remaining issues

Remember: A thorough audit catches issues before users do. Be systematic and honest about findings.