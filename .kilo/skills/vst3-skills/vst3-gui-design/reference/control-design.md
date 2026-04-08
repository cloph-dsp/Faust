# Control Design Best Practices for VST3 Plugins

Comprehensive guide to designing knobs, sliders, buttons, and meters for professional audio plugin UIs.

## Knob Design

### Types of Knobs

#### 1. Continuous Rotary
**Use for**: Gain, frequency, time, most continuous parameters

**Design Elements**:
```
- Body: 60-80px diameter for primary controls
- Indicator: Arc, dot, or line showing position
- Scale: Optional tick marks for important values
- Value display: Optional numeric readout
```

**Visual Patterns**:
```
Arc indicator (most common):
    315°
   ╱     ╲
  ●───────●  ← 270° to 90° range
   ╲     ╱
    ←───→
    
Dot indicator:
    ● (on arc)
    
Segmented arc:
    ●──●──●──●──● (shows discrete steps)
```

#### 2. Stepped Rotary
**Use for**: Mode selection, filter type, ratio selection

**Design Elements**:
```
- Detents: Visual markers for each step
- Steps: 3-12 typical
- Value: Display current selection text
```

**Implementation**:
```cpp
// Stepped behavior
const int numSteps = 5;
const float stepSize = 1.0f / numSteps;

void OnMouseDrag(float delta) {
  int currentStep = floor(value / stepSize);
  int newStep = floor((value + delta * sensitivity) / stepSize);
  
  if (newStep != currentStep && newStep >= 0 && newStep < numSteps) {
    value = newStep * stepSize;
    SetValue(value);
    UpdateDisplay(stepLabels[newStep]);
  }
}
```

#### 3. Vertical Slider (Knob Style)
**Use for**: Space-constrained layouts, mix controls

**Design Elements**:
```
- Track: Vertical line showing range
- Thumb: Draggable element at position
- Value: Optional inline or nearby display
```

### Knob Visual States

| State | Appearance |
|-------|------------|
| Default | Normal colors, no highlight |
| Hover | Subtle glow or brightness increase |
| Dragging | Active color accent, clear indicator |
| Disabled | Reduced opacity (40-60%), no interaction |
| Automating | Animated position, optional "auto" indicator |

### Knob Color Usage
```cpp
// Example color scheme
struct KnobColors {
  Color body = Color::fromRGB(45, 45, 45);
  Color bodyHighlight = Color::fromRGB(60, 60, 60);
  Color indicator = Color::fromRGB(90, 160, 220); // accent blue
  Color indicatorInactive = Color::fromRGB(70, 70, 70);
  Color valueText = Color::fromRGB(240, 240, 240);
  Color labelText = Color::fromRGB(160, 160, 160);
};
```

### Knob Interaction Best Practices

1. **Center detent**: Optional "notch" at default position
2. **Fine control**: Shift+drag for 10x precision
3. **Double-click**: Reset to default value
4. **Scroll wheel**: Optional fine adjustment
5. **Value entry**: Double-click value to type exact number
6. **Context menu**: Right-click for reset, MIDI learn, etc.

```cpp
// Fine control implementation
void OnMouseDrag(float x, float y, float dX, float dY) {
  bool fine = GetKeyState(VK_SHIFT) & 0x8000;
  float sensitivity = fine ? 0.001f : 0.01f;
  
  float newValue = GetValue() + dX * sensitivity;
  SetValue(Clamp(newValue, 0.0f, 1.0f));
}
```

## Slider Design

### Horizontal Sliders

**Use for**: Mix (dry/wet), pan, length, time-based parameters

**Design Elements**:
```
┌──────────────────────────────────────────┐
│ ═══════════●════════════════════════════ │
│           ↑                             │
│        thumb                            │
│                                          │
│ ←──────────[ -3.2 dB ]──────────────→    │
│   min        value           max         │
└──────────────────────────────────────────┘
```

### Vertical Sliders (Faders)

**Use for**: Channel strip faders, level controls

**Design Elements**:
```
    ┌──┐
    │  │ ← cap
    ├──┤
    │  │
    │  │
    │  │
    ├──┤
    │  │ ← track
    ├──┤
    └──┘
    
  ┌────────────────┐
  │  0.0 dB  ─────●│
  │       ─────── │
  │  -inf   ────  │
  │       ────     │
  │       ──       │
  │       ─        │
  │      ─         │
  │      ◢         │
  │     ──         │
  └────────────────┘
    fader track with scale
```

### Slider Color Usage
```cpp
// Slider-specific colors
struct SliderColors {
  Color trackBg = Color::fromRGB(30, 30, 30);
  Color trackFill = Color::fromRGB(90, 160, 220);
  Color thumb = Color::fromRGB(200, 200, 200);
  Color thumbActive = Color::fromRGB(255, 255, 255);
  Color scaleText = Color::fromRGB(120, 120, 120);
};
```

### Scale Marking Guidelines

For faders and level sliders, use logarithmic scales:
```
Level fader scale:
  0 dB    (top or right)
  -6 dB
  -12 dB
  -24 dB
  -48 dB
  -∞

Frequency slider scale:
  20 Hz
  50 Hz
  100 Hz
  200 Hz
  500 Hz
  1 kHz
  2 kHz
  5 kHz
  10 kHz
  20 kHz
```

## Button Design

### Toggle Buttons
**Use for**: Bypass, enable, mode switches

**States**:
```
Off:    [  ○ OFF  ]    (subtle, muted colors)
On:     [  ● ON   ]    (accent color, bright)
```

### LED-Style Indicators
```cpp
// LED implementation
class LEDButton : public ButtonControl {
public:
  void Draw(IGraphics& g) override {
    // LED glow when on
    if (GetValue() > 0.5f) {
      g.DrawEllipse(glowRect, Color::fromRGBA(accent.r, accent.g, accent.b, 100), 8);
    }
    
    // LED body
    g.FillEllipse(ledRect, isOn ? accentColor : dimColor);
    
    // Icon or text if present
    if (!label.empty()) {
      g.DrawText(label, textRect, textColor);
    }
  }
};
```

### Radio Group Buttons
**Use for**: Mode selection, filter type, etc.

```
┌──────┬──────┬──────┐
│ LP   │ HP   │ BP   │   ← LP selected
└──────┴──────┴──────┘
```

## Meter Design

### VU Meter
**Characteristics**: Ballistics approximate human hearing, warm response

```
Recommended ballistics:
- Attack: ~300ms to reach 99% of input
- Release: ~300ms to fall 20dB

Color zones:
Green:  -20 dB to -6 dB
Yellow: -6 dB to 0 dB
Red:    0 dB and above
```

### Peak Meter
**Characteristics**: Fast attack, peak hold

```
Recommended ballistics:
- Attack: 0ms (instant)
- Release: variable (500ms to 2s for peak hold)

Peak hold:
- Hold time: 1-2 seconds
- Fall rate: slower than visual decay
```

### Gain Reduction Meter
**Characteristics**: Inverted display (bars go down)

```
┌─────────┐
│░░░░░░░░░│  ← no reduction
│░░░░░░░░░│
│░░░░░░░░░│
│░░░░░░░░░│
│█████████│  ← -10 dB GR
│█████████│
│█████████│
└─────────┘
```

### Spectrum Analyzer
**Characteristics**: FFT-based frequency display

**Design considerations**:
- Logarithmic frequency scale
- dB magnitude scale (typically -60 to 0 dB)
- Color gradient (cool to warm)
- Frame rate: 30-60 fps
- Smoothing: Some time averaging for stability

### Meter Implementation Tips
```cpp
// Efficient meter update (process meters at lower rate)
class MeterProcessor {
public:
  void ProcessBlock(const float* input, int nSamples) {
    for (int i = 0; i < nSamples; i++) {
      float sample = std::abs(input[i]);
      peakHold = std::max(peakHold, sample);
      rmsAccumulator += sample * sample;
    }
    
    rmsValue = std::sqrt(rmsAccumulator / nSamples);
    sampleCount += nSamples;
  }
  
  void UpdateMetersIfNeeded() {
    if (sampleCount > meterUpdateSamples) {
      // Convert to dB
      float peakDb = 20 * std::log10(peakHold + 1e-9);
      float rmsDb = 20 * std::log10(rmsValue + 1e-9);
      
      // Apply ballistics (simplified)
      displayPeak = displayPeak * 0.9f + peakDb * 0.1f;
      displayRms = displayRms * 0.95f + rmsDb * 0.05f;
      
      // Reset
      peakHold = 0;
      rmsAccumulator = 0;
      sampleCount = 0;
    }
  }
};
```

## Value Display Design

### Numeric Readouts
**Typography**: Monospace, fixed-width for alignment
**Size**: 14-18px for main values, 10-12px for labels
**Format**: Appropriate precision (1-2 decimal places typically)

```cpp
// Value formatting
std::string FormatDb(float value) {
  if (value <= -60.0f) return "-inf";
  return String::formatted("%.1f dB", value);
}

std::string FormatHz(float value) {
  if (value >= 1000.0f) {
    return String::formatted("%.2f kHz", value / 1000.0f);
  }
  return String::formatted("%.0f Hz", value);
}

std::string FormatPercent(float value) {
  return String::formatted("%.0f%%", value * 100.0f);
}
```

### Label Design
```
Position: Below knob by default, above for top-row controls
Font: Sans-serif, 10-12px, secondary text color
Alignment: Centered with knob
Content: Parameter name, unit (optional)
```

## Control Spacing

### Grid-Based Layout
```
Base unit: 8px

Knob spacing: 16-24px between centers
Section gaps: 32-48px between control groups
Label spacing: 8px below knob, 4px below label
```

### Example Layout
```
     ┌───┐   ┌───┐   ┌───┐   ┌───┐
     │   │   │   │   │   │   │   │
     │ A │   │ B │   │ C │   │ D │   ← 24px spacing
     └───┘   └───┘   └───┘   └───┘
    Input    Threshold  Ratio   Attack
      ↑        ↑         ↑       ↑
     8px      8px       8px     8px
```

## Accessibility Considerations

### Visual Accessibility
- Minimum contrast ratio: 4.5:1 for text
- Consider colorblind users (don't use color alone)
- Provide alternative indicators (icons + color)
- Ensure visibility in various DAW themes

### Interaction Accessibility
- Keyboard navigation support
- Scroll wheel for knob adjustment
- Double-click to reset
- Optional keyboard shortcuts for common parameters

## Common Patterns from Top Plugins

### FabFilter
- Large value displays
- Arc indicators on knobs
- Subtle shadows
- Clean typography

### UAD
- Realistic knob textures
- LED-style meters
- Warm color palette
- Hardware-inspired layout

### iZotope
- Bold accent colors
- Clear iconography
- Card-based sections
- Modern typography

### Plugin Alliance
- Dark backgrounds
- High contrast
- LED meters
- Industrial feel
