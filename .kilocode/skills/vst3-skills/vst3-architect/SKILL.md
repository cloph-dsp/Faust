---
name: vst3-architect
description: Design a robust VST3 plugin architecture from a spec for Faust, JUCE, iPlug2, or hybrid builds. Use for signal flow, gain staging, channel routing, control mapping, state design, and host integration patterns.
user-invocable: true
argument-hint: plugin architecture target and constraints
---

Translate a plugin brief into a compilable, testable plugin architecture with clear headroom, routing rules, and an explicit implementation path.

## Pick the Implementation Path

Classify the implementation target:

1. **Faust-first DSP authoring**
2. **iPlug2 plugin**
3. **JUCE plugin**
4. **Raw VST3 SDK / C++**
5. **Hybrid Faust DSP core wrapped by C++ plugin code**

State why that path fits the brief: iteration speed vs low-level control, portability, UI requirements.

## Lock Platform Assumptions

- Plugin format: VST3, AU, AAX, standalone
- Channel topology: mono, stereo, dual-mono, mid-side, or wider bus
- Sample-rate and block-size range
- Latency reporting requirements
- Host compatibility requirements

## Build the Processing Skeleton

### Generic DSP Structure

Split processing into named stages:

1. **Input conditioning**: trim, DC removal, denormal protection, trigger shaping
2. **Core processor**: nonlinear engine, resonant block, generator, or synthesis core
3. **Shaping**: spectral, dynamic, spatial, or modulation processing
4. **Output conditioning**: dry/wet, safety limiting, routing

Document gain staging per stage with nominal levels and maximum expected swing.

### Framework-Specific Skeletons

#### iPlug2 Skeleton

```cpp
// iPlug2 plugin structure
class MyPlugin : public iplug::Plugin {
public:
  MyPlugin(const iplug::InstanceConfig& config)
    : iplug::Plugin(config) {
    // Add parameters
    MakeParam("Gain", 0.0, -60.0, 12.0, 0.01, "dB");
    
    // Initialize graphics
    mMakeGraphicsFunc = [&]() {
      return MakePNGModeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS);
    };
    
    // Attach controls
    mAttachFunc = [&](igraphics::IGraphics* pGraphics) {
      pGraphics->AttachControl(new igraphics::IVKnob(...
    };
  }
  
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
};
```

#### JUCE Skeleton

```cpp
// JUCE plugin structure
class MyPlugin : public gin::Plugin {
public:
  static const Properties properties;
  
  MyPlugin();
  ~MyPlugin() override;
  
  void prepareToPlay(double sampleRate, int maxBlockSize) override;
  void processBlock(AudioBuffer&, MidiBuffer&) override;
  
  AudioProcessorEditor* createEditor() override;
  
  // State
  void getStateInformation(MemoryBlock&) override;
  void setStateInformation(const void*, int) override;
};
```

#### Faust + iPlug2 Hybrid

```cpp
// Wrap Faust DSP in iPlug2
class FaustDspWrapper : public iplug::DelegatePlugin {
public:
  FaustDspWrapper()
    : DelegatePlugin(1, 1)
    , fDsp(new mydsp()) {
    // Delegate audio to Faust
    SetDelegateDsp(fDsp.get());
  }
  
private:
  std::unique_ptr<mydsp> fDsp;
};
```

#### Faust + JUCE Hybrid

```cpp
// Wrap Faust DSP in JUCE
class FaustDspProcessor : public gin::Processor {
public:
  FaustDspProcessor() {
    fDsp = new mydsp();
    // Initialize with sample rate
    fDsp->init(48000);
  }
  
  void processBlock(AudioBuffer& buffer, MidiBuffer&) override {
    // Convert and call Faust
    float* inputs[] = { buffer.getWritePointer(0) };
    float* outputs[] = { buffer.getWritePointer(0) };
    fDsp->compute(buffer.getNumSamples(), inputs, outputs);
  }
  
private:
  mydsp* fDsp;
};
```

## Topology Decisions

### Oversampling Strategy
- If nonlinearity is strong or abrupt: add oversampling plan before coding
- Consider 2x, 4x, or 8x oversampling options

### Channel Processing Mode
- Decide early: dual-mono, linked stereo, or mid-side processing
- Document routing for all supported channel counts

### Time-Based Processing
- Define delay bounds, feedback law, reset behavior
- Document modulation source topology

### Polyphony (Instruments)
- Separate voice-local DSP from global/shared DSP
- Document voice allocation strategy

## Parameter and Routing Architecture

### Control-to-DSP Mapping

For each parameter:
- VST3 parameter ID (must be unique, stable)
- UI range and taper
- Internal mapped variable
- Smoothing policy (where smoothing occurs)
- Automation handling

### Real-Time Safety Rules

**CRITICAL**: No allocations, locks, or file I/O in audio processing callbacks.

- Parameter transport happens outside sample loop when possible
- Sample-critical parameters get smoothing inside DSP layer
- Guard against denormals in very low-level tails
- Verify stable behavior at extreme control values

### State Serialization

- Keep state serialization/versioning separate from live processing
- Document preset format and migration strategy
- Handle host save/restore gracefully

## Implementation Rules by Framework

### iPlug2 Rules
- Use `DelegatePlugin` for Faust wrapping
- Parameter smoothing via `SetSmoothingTimeMs()`
- SVG assets via `LoadSVG()` and `ISVGBase`
- Use `IGraphics::AttachControl()` for UI binding
- Handle `OnParamChange()` for parameter updates

### JUCE Rules
- Separate audio thread from message thread
- Use `AudioParameterFloat` with appropriate smoothing
- `processBlock()` must be real-time safe
- Use `AudioVisualiserComponent` or custom for meters
- JUCE 8+: consider Direct2D rendering for Windows

### Faust Rules
- Respect box connection constraints for `:`, `<:`, `:>`, and `~`
- Keep delay sizes bounded for `@(...)` expressions
- Use `si.smoo` for parameter smoothing
- Document `-mcd` (memory cell delay) strategy

### Hybrid Rules
- Define contract between DSP core and plugin wrapper
- Channel layout, parameter normalization, block boundaries
- Validate gain staging not split inconsistently

## Output Required

Produce these artifacts:

- Stage-by-stage architecture summary
- Initial skeleton that compiles for the chosen path
- Parameter map table (name, range, taper, smoothing, owner)
- VST3 parameter ID strategy
- Known technical risks with mitigation plan

## Completion Gate

Do not proceed to detailed implementation until all are true:

- Implementation path is explicitly chosen and justified
- Skeleton compiles without errors
- Headroom and gain staging are explicitly defined
- Oversampling decision is documented
- Channel topology and bypass behavior are explicit
- Parameter mapping and smoothing policy are complete
- State serialization approach is defined
