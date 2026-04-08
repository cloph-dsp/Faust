---
name: vst3-spec
description: Define a production-grade VST3 plugin brief before coding. Use for effects, instruments, filters, dynamics, and utility algorithms intended for VST3, AU, AAX, or standalone audio products. Supports iPlug2, JUCE, Faust, and raw C++ implementations.
user-invocable: true
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

Turn a vague idea into a measurable plugin contract before writing code.

## Build the Product Brief

### Define Product Intent
- **Product class**: effect, instrument, generator, processor, analysis tool, or hybrid
- **Operating context**: voice, instrument, bus, master, synthesis voice, or utility pipeline
- **User promise**: what should the user hear, feel, or be able to do?
- **Target DAW users**: general producers, mixing engineers, sound designers, performers

### Define Plugin Format Decisions
- Primary format: VST3, AU, AAX, or standalone
- Secondary formats: which fallbacks are required
- Platform targets: macOS, Windows, or both
- Plugin category (VST3): Fx, Instruments, Analyzers, Spatial, etc.

### Define Hard Constraints
- Supported sample rates and channel modes (mono, stereo, multichannel)
- CPU budget at 44.1, 48, and 96 kHz
- Latency budget and lookahead allowance
- Expected automation behavior (slow sweeps vs fast modulation)
- Memory footprint limits

## Choose Implementation Path

Decide on the development approach:

1. **Faust-first DSP authoring**
   - Best for: rapid prototyping, signal processing algorithms, cross-platform DSP
   - Limitations: UI flexibility, host integration complexity

2. **iPlug2**
   - Best for: cross-platform plugins, modern tooling, good default UI, SVG support
   - Limitations: learning curve for IGraphics API

3. **JUCE**
   - Best for: full control, complex applications, web UI via WebView, custom rendering
   - Limitations: larger binary size, more boilerplate

4. **Raw VST3 SDK**
   - Best for: minimal footprint, learning, specific host requirements
   - Limitations: significant boilerplate, no cross-platform abstraction

5. **Hybrid (Faust DSP + C++ wrapper)**
   - Best for: Faust algorithm with custom C++ plugin infrastructure
   - Choose when: Faust DSP quality + iPlug2/JUCE UI desired

## Define Sonic and Behavioral Targets

Document target characteristics:

- **Core adjectives**: tight low end, stable image, fast transient recovery, clear modulation
- **No-go outcomes**: harsh aliasing, unstable pitch, image collapse, pumping, zipper noise
- **Loudness matching**: output-matching rule for fair comparisons
- **Reference plugins**: FabFilter Pro-Q, Valhalla, UAD, etc. for tone reference

## Define Parameter Contract

For each exposed control, specify:

- Name and role in plain language
- Range, taper, and default
- Unit metadata (dB, Hz, %, ratio)
- VST3 parameter ID strategy
- Automation behavior requirements
- Smoothing requirement (none, light, strong)
- Interaction notes with other controls

**Parameter contract rule**: every parameter must have a purpose, a bounded range, and an audible test case.

## Define Acceptance Tests

Create pass/fail checks before coding:

- **Tone checks**: level-matched comparison against references at representative low, medium, and high states
- **Artifact checks**: no zipper noise during automation, no foldback or alias fizz at bright inputs
- **Stability checks**: no NaN or Inf behavior at control extremes
- **Performance checks**: meets CPU and latency targets in target runtime context
- **Host integration checks**: automation, preset save/load, bypass behavior

## Define State Management

- Preset format: default chunk state, XML, or JSON
- Factory presets: which ones ship with the plugin
- User preset handling: location, migration strategy
- MIDI/CC mapping requirements

## Completion Gate

Do not proceed to architecture work until all are true:

- Problem statement is one sentence and specific
- Implementation path is chosen and justified
- Constraints are explicit and measurable
- Algorithm family and fallback are chosen
- Parameter contract is complete
- Acceptance tests are written and realistic
- State management approach is defined
