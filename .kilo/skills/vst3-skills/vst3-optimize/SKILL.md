---
name: vst3-optimize
description: Optimize and harden VST3 plugins for production audio use. Use for CPU reduction, anti-alias efficiency, control-rate optimization, memory strategy, and real-time safety. Works with iPlug2, JUCE, Faust, and hybrid builds.
user-invocable: true
argument-hint: performance target and platform constraints
args:
  - name: target
    description: What to optimize (CPU, memory, latency, alias rejection)
    required: false
  - name: constraints
    description: Target DAW, sample rates, expected block sizes, CPU budget
    required: false
---

Push the plugin to production readiness without changing intended tone.

## Real-Time Safety First

Treat plugin processing as hard real-time unless explicitly offline-only:

- **No allocations** in audio processing callbacks
- **No locks** in the audio path
- **No file I/O** in processing callbacks
- **No UI-thread assumptions** in DSP code paths
- Handle variable block sizes and zero-sample blocks safely

## Rate-Domain Optimization

Distribute work into slower domains when possible:

1. **Compile/specialization time**: constants, static structure decisions
2. **Init time**: table builds, sample-rate-derived constants
3. **Control rate**: parameter mapping, expensive nonlinear transforms
4. **Sample rate**: only what must run per-sample

## CPU Optimization Strategies

### For iPlug2
```cpp
// Pre-compute expensive operations in OnParamChange
void OnParamChange(int paramIdx) override {
  if (paramIdx == kParamDrive) {
    // Pre-compute waveshaper curve on parameter change
    precomputedCurve = MakeWaveshaperCurve(GetParam(kParamDrive)->Value());
  }
}

// Simple per-sample processing
void ProcessBlock(...) override {
  // Use precomputed curve, not recalculate
  for (int s = 0; s < nFrames; s++) {
    output[s] = ApplyCurve(precomputedCurve, input[s]);
  }
}
```

### For JUCE
```cpp
// Use SIMD where beneficial
void processBlock(AudioBuffer& buffer, MidiBuffer&) override {
  auto* data = buffer.getWritePointer(0);
  int n = buffer.getNumSamples();
  
  // Let JUCE handle vectorization for simple ops
  // For custom DSP, use dsp::SIMDRegister
  dsp::FloatVectorOperations::multiply(data, data, gain, n);
}
```

### For Faust
```cpp
// Put expensive transforms before si.smoo when behavior allows
smoothedControl = expensiveFunction(rawControl) : si.smoo;

// Use rdtable for expensive unary functions
cosTable = rotable, n (cos(2.0 * PI * i / float(n)));
output = input : *(2.0 * PI / float(n)) : *(n/2) : rdtable(n, cosTable, _);
```

## Memory Optimization

### For iPlug2
- Use fixed-size buffers over dynamic allocation
- Share buffers between channels where possible
- Consider memory pool for voice management

### For JUCE
- Pre-allocate DSP buffers in `prepareToPlay()`
- Use `AudioBuffer` with fixed sizes
- Pool allocated objects at initialization

### For Faust
```bash
# Compile options for memory
faust -mcd 1024 ...  # Memory cell delay (shared delays)
faust -dlt 1024 ...  # Delay line table size
```

## Anti-Aliasing Optimization

### Oversampling Strategy
- Consider 2x, 4x, or 8x based on algorithm type
- Match filter quality to oversampling ratio
- Document CPU cost trade-offs

```cpp
// Example: 4x oversampling with 8-point filter
// Per-stage: ~2-3% CPU, 3-sample latency
// 2 stages: ~5-6% CPU, 6-sample latency
```

### Alias Rejection Targets
- 80+ dB rejection at Nyquist for high-frequency content
- 60+ dB for general nonlinear processing
- Test with sawtooth wave at 10% of sample rate

## Latency Optimization

### Sources of Latency
- Lookahead buffers
- Oversampling filter groups
- MIDI processing buffers
- Oversampled feedback paths

### Reporting Latency
```cpp
// VST3 latency reporting
int64_t getLatencySamples() override {
  return lookaheadSamples + oversamplingLatency;
}

// AU latency reporting
void setProperty(AudioUnitPropertyID id, AudioUnitScope scope, 
                 AudioUnitElement element, const void* data) override {
  if (id == kAudioUnitProperty_Latency) {
    return lookaheadSamples + oversamplingLatency;
  }
}
```

## Benchmark Methodology

Use repeatable test settings:

- **Sample rates**: 44.1, 48, 96 kHz
- **Block sizes**: 64, 256, 512, 1024
- **Input material**: quiet, nominal, and hot
- **DAW testing**: Reaper, Ableton, Pro Tools, Logic (if applicable)

## Robustness Hardening

- Guard against denormal slowdowns (add tiny offset: 1e-18)
- Verify stable behavior at extreme control values
- Verify no NaN or Inf propagation
- Ensure bounded delay and table indices at runtime
- Test parameter jumping (automation cliffs)

## Framework-Specific Performance Notes

### iPlug2
- `IGraphics` rendering runs on UI thread, not audio thread
- SVG rendering can be expensive; cache rendered bitmaps
- Profile with `TraceMem()` for allocation tracking

### JUCE
- `processBlock()` is the audio thread
- `AudioProcessorEditor` is on the message thread
- Use `dsp::ProcessorDuplicator` for polyphonic processing
- JUCE 8+: Direct2D can be faster on Windows

### Faust
- `-os` for single-sample optimization (can be slower)
- `-vec` for vectorization
- `-vs 8` for 8-sample vector size
- Profile generated code with `-loud` option

## Output Required

- Optimization change log (what changed, expected gain)
- CPU and memory before/after summary
- Quality trade-offs introduced
- Recommended compile options for target platform
- Latency specification

## Completion Gate

Do not proceed to verification until all are true:

- Real-time safety constraints are respected
- CPU targets are met or explicitly documented as unmet
- Latency is measured and documented
- No stability regressions at stress settings
- Quality delta from optimization is reviewed
