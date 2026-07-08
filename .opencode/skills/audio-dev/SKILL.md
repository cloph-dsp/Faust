# Audio-Dev Skill

Dispatches VST plugin development work to specialized agents.

## When to load

User says "VST", "audio plugin", "DSP", "iPlug2", "Faust", "JUCE", "Cmajor" or shows intent to build/modify a VST plugin.

## Delegation Map

| Trigger | Delegate to | When |
|---------|-------------|------|
| "spec", "requirements", "design a plugin", "new plugin idea" | vst-spec | Before any coding |
| "architecture", "framework choice", "signal flow" | vst-architect | After spec is clear |
| "DSP", "algorithm", "audio processing", "Faust code" | vst-dsp | After architecture |
| "GUI", "UI", "controls", "knobs", "layout", "SVG" | vst-gui | After DSP is stable |
| "test", "validate", "QA", "check quality", "audit" | vst-validate | After implementation |

## Prompts (use as task() prompt when delegating)

### vst-spec
You define production-grade VST plugin requirements before coding.
Turn vague plugin ideas into measurable specifications.
Product Intent: [class] plugin that [promise] for [users] in [context]
Classes: effect, instrument, generator, processor, analysis tool, hybrid
Formats: VST3, AU, AAX, standalone; platforms: macOS, Windows
Hard constraints: sample rates (44.1/48/96k), channels, CPU budget, latency budget
Implementation path: Faust-first, iPlug2, JUCE, Raw VST3, Hybrid
Sonic targets: tight low end, stable image, fast transients, clear modulation
Parameter contract per control: Name, Role, Range, Taper, Default, Unit, Smoothing
Acceptance tests: tone, artifact, stability, performance, host integration

### vst-architect
Design VST plugin architecture based on specification.
Framework options: iPlug2 (SVG UI, cross-platform), JUCE (WebView, complex),
Faust (fast DSP), Cmajor (Amorph), VST3 SDK (minimal), Rust (nih-plug)
Layers: DSP (algorithms/smoothing) -> UI (controls/layout) -> Host (automation/presets)
State: ValueTree (JUCE), raw binary (iPlug2), JSON (Faust/Cmajor)
Smoothing: freq=exp 10-50ms, gain=linear 1-10ms, cutoff=exp 10-50ms, mode=instant, bypass=crossfade
Safety: no allocs in audio thread, pre-allocate, lock-free atomics

### vst-dsp
Implement DSP with real-time safety. No allocs in audio thread.
Denormal prevention: DC offset or flush-to-zero.
Smoothing: linear (1-10ms) ramp, exp damped (10-50ms) alpha
Patterns: Biquad RBJ, State Variable Filter, Wavetable OSC, Compressor
Oversampling: 2x zero-stuff+FIR, 4x polyphase
Validate: level match +/-0.1dB, THD+N < -80dB, no NaN/Inf
Framework: GetParam()->Value (iPlug2), ARVP (JUCE), .dsp (Faust), cmajor proc (Cmajor)

### vst-gui
Design GUI. Dark theme: bg=#0f1417, panel=#1a1f24, accent=#29d3b2, text=white
Layout: compact (600-800px), standard (800-1000px), spacious (1000-1200px)
iPlug2: SVG, circular knobs, responsive via MakeDefaultSizeConstraint
JUCE: Component + Slider + AudioProcessorValueTreeState
Cmajor: WebComponent + shadow CSS + ParameterControl
Controls: knob (continuous), slider (wide), toggle, graph (viz)
Bind: GetParam()->Value (iPlug2), getRawParameterValue (JUCE), setParameterValue (Cmajor)

### vst-validate
Build: zero warnings /WX, compiles, no undefined symbols
Audio: no NaN/Inf, no zipper noise, no aliasing >-100dB, level +/-0.1dB, freq +/-1dB, THD+N < spec
Performance: CPU < budget, latency < budget, no allocs, no leaks
Host: automation works, presets save/load within 0.001, bypass correct, scan <5s
UI: controls respond, param reflects, responsive, no stutter
Signals: sine sweep 20-20kHz, impulse 0dBFS, white noise

## Delegation

Use task(category="quick"|"visual-engineering", load_skills=["vst-creation"], prompt="<prompt from above>").
DSP and GUI can run in parallel if independent (e.g., Faust DSP + iPlug2 UI).
Verify each phase before advancing.
