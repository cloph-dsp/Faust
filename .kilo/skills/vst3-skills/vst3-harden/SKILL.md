---
name: vst3-harden
description: Improve VST3 plugin robustness against edge cases, errors, and real-world scenarios. Use when plugin crashes, hangs, or misbehaves under unusual conditions. Works with iPlug2, JUCE, and Faust.
user-invokable: true
argument-hint: "[target]"
---

Harden plugins against edge cases that break idealized designs.

## Hardening Areas

### 1. Audio Thread Safety

**Real-time constraints**:
```cpp
// ❌ Never do this in processBlock
void processBlock(AudioBuffer& buffer, MidiBuffer&) {
  // This allocates - BAD
  std::vector<float> temp(buffer.getNumSamples());
  
  // This logs - BAD  
  DBG("Processing " << buffer.getNumSamples() << " samples");
}

// ✅ Pre-allocate or use stack buffers
void prepareToPlay(double sampleRate, int maxBlockSize) override {
  tempBuffer.setSize(1, maxBlockSize, false, true);
  tempBuffer.clear();
}
```

**Denormal protection**:
```cpp
// Add tiny offset to prevent denormal slowdowns
float PreventDenormal(float value) {
  return value + 1e-18f;
}

// Or use SSE/NEON intrinsics
inline float fastTanh(float x) {
  // Uses SSE for speed, handles denormals
}
```

### 2. Parameter Edge Cases

**Value clamping**:
```cpp
// Always clamp parameters to valid range
void SetParameterValue(float value) override {
  value = std::clamp(value, 0.0f, 1.0f);
  // Use clamped value, not raw input
}
```

**NaN/Inf handling**:
```cpp
// Check for and fix invalid values
float SafeDivide(float numerator, float denominator) {
  if (denominator == 0.0f) return 0.0f;
  float result = numerator / denominator;
  return std::isfinite(result) ? result : 0.0f;
}
```

**Extremes**:
```cpp
// Test parameters at 0, 0.5, 1.0 and beyond
// Verify algorithm handles:
// - Gain at -60dB (practically silent)
// - Gain at +12dB (clipping headroom)
// - Frequency at 20Hz and 20kHz
// - Time at 0ms (instant) and max
```

### 3. Host Integration

**Lost device handling**:
```cpp
// Handle audio device disconnection
void DeviceChange() {
  // Gracefully stop processing
  // Clear any queued state
  // Resume cleanly when device returns
}
```

**Buffer underruns**:
```cpp
// Handle zero-sample blocks
void processBlock(AudioBuffer& buffer, MidiBuffer&) override {
  if (buffer.getNumSamples() == 0) {
    return;  // Don't process, avoid edge cases
  }
  // Normal processing
}
```

### 4. State Management

**Corrupted preset loading**:
```cpp
// Validate state data before use
void setStateInformation(const void* data, int size) override {
  if (data == nullptr || size < sizeof(PluginState)) {
    // Load defaults
    LoadDefaults();
    return;
  }
  
  // Validate header/magic number
  auto* state = static_cast<const PluginState*>(data);
  if (state->magic != kStateMagic) {
    LoadDefaults();
    return;
  }
  
  // Safe to use
  LoadFromState(*state);
}
```

**Sample rate changes**:
```cpp
// Recalculate when sample rate changes
void setSampleRate(double newSampleRate) override {
  sampleRate = newSampleRate;
  
  // Update any sample-rate dependent calculations
  UpdateFilterCoefficients();
  UpdateDelayTimes();
  // Reinitialize any tables
}
```

### 5. UI Robustness

**Window resize**:
```cpp
// Handle minimum size constraints
void resized() override {
  auto minWidth = 300;
  auto minHeight = 200;
  
  // Enforce minimum
  if (getWidth() < minWidth || getHeight() < minHeight) {
    setSize(max(getWidth(), minWidth), max(getHeight(), minHeight));
  }
  
  // Update control positions
  LayoutControls();
}
```

**Missing assets**:
```cpp
// Graceful fallback for missing resources
void LoadResources() {
  if (knobSvg.exists()) {
    knobGraphic = LoadSVG(knobSvg);
  } else {
    // Fallback to built-in knob
    useFallbackKnob = true;
  }
}
```

### 6. Concurrency

**Parameter changes during processing**:
```cpp
// Thread-safe parameter handling
std::atomic<float> currentGain{0.0f};

// In processBlock - read atomically
float gain = currentGain.load(std::memory_order_relaxed);

// In UI thread - write atomically  
void SetGainParameter(float value) {
  currentGain.store(value, std::memory_order_release);
}
```

## Testing Strategy

### Edge Case Testing

1. **Parameter extremes**: Set all params to min/max
2. **Rapid automation**: Fast parameter sweeps
3. **Preset churn**: Load/save many presets rapidly
4. **Sample rate changes**: Switch between 44.1 and 96k
5. **Block size changes**: DAW buffer size changes
6. **Host suspend/resume**: Pause and resume playback

### Stress Testing

1. **Long sessions**: Run for hours, check memory
2. **High CPU**: Set CPU limit in DAW, check behavior
3. **Many instances**: Load multiple copies

### Error Injection

1. **Corrupt presets**: Manually damage preset file
2. **Missing files**: Rename/remove resource files
3. **Audio device issues**: Disconnect audio interface

## Hardening Checklist

- [ ] No allocations in audio callback
- [ ] Denormal protection in DSP code
- [ ] NaN/Inf values handled
- [ ] Parameter values clamped
- [ ] Corrupt state handled gracefully
- [ ] Sample rate changes handled
- [ ] Zero-sample blocks handled
- [ ] Missing assets handled
- [ ] UI resize constraints set
- [ ] Thread-safe parameter access
- [ ] Memory stable over time
- [ ] Works with various buffer sizes

## Fix Recommendations

For each issue found:
1. Identify root cause
2. Implement fix in appropriate layer
3. Add test case to prevent regression

Remember: Plugins run in diverse environments with diverse usage. Hardening for production means expecting the unexpected.