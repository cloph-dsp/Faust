---
name: vst3-gui-design
description: Create distinctive, production-grade VST3 plugin GUIs with modern aesthetics. Avoids clunky 2000s designs with outdated skeuomorphism. Supports iPlug2, JUCE, and Faust with SVG assets. Focus on professional audio plugin UI patterns.
user-invocable: true
argument-hint: "[plugin context and aesthetic direction]"
---

Design modern, professional plugin GUIs that avoid the clunky 2000s aesthetic. This skill covers typography, color, layout, controls, SVG workflow, and framework-specific implementation for audio plugins.

## Context Gathering

### Required Context
- **Target DAW environment**: Pro Tools, Logic, Ableton, Reaper (dark/light themes)
- **User workflow**: mixing engineer, sound designer, performer, casual user
- **Brand personality**: clinical precision, vintage warmth, modern minimal, creative expressiveness
- **Plugin type**: single-effect, multi-effect, instrument, analyzer

### Aesthetic Direction Examples

**Modern Minimal** (FabFilter style)
- Clean lines, no unnecessary chrome
- Large value displays
- Subtle shadows and depth
- Typography-forward design

**Neo-Skeuomorphic** (UAD, Softube style)
- Realistic knob/slider textures
- Detailed panel aesthetics
- Warm color palette
- Tactile feedback simulation

**Flat Modern** (iZotope style)
- Bold color accents
- Clear hierarchy
- Responsive controls
- Contemporary feel

**Dark Industrial** (Slate Digital, Plugin Alliance style)
- Deep blacks
- High contrast accents
- Geometric precision
- Premium materials feel

## Typography for Plugins

### Font Selection
- **Display/Headers**: Clean sans-serif (Inter, SF Pro, Roboto) or custom display font
- **Value Readouts**: Monospace for numerical precision (SF Mono, JetBrains Mono)
- **Labels**: Compact sans-serif for small sizes

### Type Scale for Fixed-Size Windows
```
Parameter Value: 14-18px, bold
Knob/Slider Label: 10-12px, regular
Section Header: 11-13px, semibold, uppercase
Tooltip/Info: 9-10px, regular
```

### Best Practices
- Use power-of-two font sizes for sharpness
- Ensure text remains readable at plugin's minimum scale
- Avoid decorative fonts that don't scale well
- High contrast for critical values (gain, frequency)

## Color & Theme

### Dark Theme Foundation
```css
/* Example dark theme palette */
--bg-primary: #1a1a1a;
--bg-secondary: #242424;
--bg-tertiary: #2e2e2e;
--border-color: #3a3a3a;
--text-primary: #f0f0f0;
--text-secondary: #a0a0a0;
--accent-primary: #5a9fd4;    /* Subtle blue */
--accent-secondary: #d45a5a;  /* Warm red for warnings */
--knob-fill: #4a4a4a;
```

### Color Theory for Audio UIs
- Use OKLCH or HSL for perceptually uniform colors
- Tint neutrals slightly toward accent hue
- Avoid pure black (#000) and pure white (#fff)
- Maintain WCAG AA contrast for text (4.5:1 minimum)

### Control States
- **Default**: subtle differentiation
- **Hover**: gentle highlight
- **Active/Dragging**: clear active state
- **Disabled**: reduced opacity, no interaction

## Layout & Control Density

### Grid System
- 8px base grid for alignment
- Consistent spacing between related controls (8-12px)
- Clear separation between sections (24-48px)
- Generous margins from window edge (16px)

### Control Grouping
Group by signal flow or function:
```
[INPUT] → [PROCESSING] → [OUTPUT]
    ↓           ↓            ↓
 Trim       Core         Mix
 Gate      Effect       Limiter
```

### Window Size Considerations
- **Standard**: 400-600px width (single effect)
- **Wide**: 800-1000px (multi-effect, channel strips)
- **Compact**: 200-300px (utility plugins)
- Allow resizable when possible

## Control Design

### Knobs
**Types**:
- Continuous rotary (gain, frequency)
- Stepped rotary (mode selection)
- Vertical slider disguised as knob

**Best Practices**:
- Clear center detent or tick marks for semitone/small steps
- Value display on hover or always visible
- Double-click to reset to default
- Shift+drag for fine adjustment
- Visual arc or fill for value indication

### Sliders/Faders
**Types**:
- Vertical faders (mixer-style)
- Horizontal sliders (parameters with range)
- Dual knobs (linked stereo parameters)

**Best Practices**:
- Clear scale markings for important values
- dB scale for level parameters (logarithmic feel)
- Handle size appropriate for precision vs ease
- Smooth animation during automation

### Buttons
**Types**:
- Toggle (on/off state)
- Momentary (press and hold)
- Radio group (mutually exclusive)
- Utility (bypass, preset, menu)

**Best Practices**:
- Clear pressed state
- LED or color change for toggle state
- Icon + text for clarity
- 24-32px minimum touch target

### Meters
**Types**:
- VU meter (average level)
- Peak meter (instant peaks)
- Gain reduction meter
- Spectrum analyzer
- Phase meter

**Best Practices**:
- Green → Yellow → Red gradient for levels
- Peak hold indicators
- Calibrate to -18 dBFS = 0 VU
- Smooth ballistics (not jumpy)

## Motion & Animation

### Parameter Feedback
- Smooth knob/slider movement during automation
- Value display updates in real-time
- Avoid jarring jumps when automation catches up

### Hover States
- Subtle glow or highlight (50-100ms transition)
- Tooltip with detailed information
- Cursor change to indicate draggable

### Loading States
- Show plugin immediately, load DSP async
- Progress indication if loading is slow
- Graceful degradation if assets fail to load

### Animation Timing
```css
/* Recommended timing */
--transition-fast: 100ms;      /* immediate feedback */
--transition-normal: 200ms;     /* state changes */
--transition-slow: 300ms;       /* panel transitions */

--ease-out: cubic-bezier(0.25, 1, 0.5, 1);
--ease-spring: cubic-bezier(0.34, 1.56, 0.64, 1);
```

## SVG Asset Workflow

### Creating SVGs
1. Design in Figma, Illustrator, or Affinity Designer
2. Export with:
   - Optimized SVG (remove metadata)
   - ViewBox-based sizing
   - Flatten transforms where possible
3. Optimize with SVGO or similar

### Framework Implementation

#### iPlug2 SVG Usage
```cpp
// Load and use SVG in iPlug2
auto svg = IGRAPHICS.LoadSVG("knob.svg");

// Attach as custom control base
class MyKnob : public ISVGBase {
public:
  MyKnob(IRECT bounds, int paramIdx, const ISVG& knobSvg)
    : ISVGBase(bounds, paramIdx, knobSvg) {}
  
  void OnMouseDrag(float x, float y, float dX, float dY, 
                   float pressure) override {
    // Handle drag with SVG rotation
    double normalizedValue = x / GetBounds().W();
    // Update parameter
  }
};
```

#### JUCE SVG Usage
```cpp
// Load SVG in JUCE
auto drawable = Drawable::createFromSVG(SVG::parse(File("knob.svg")));

// For performance: render to image and cache
Image cachedKnob = renderSVGToImage(svg, 128, 128);
```

#### Faust UI Customization
- Modify architecture file to include custom SVG paths
- Use UI metadata for parameter hints
- Consider C++ wrapper for full SVG control

## Framework Examples

### iPlug2 Complete Control
```cpp
// Full iPlug2 control setup
mAttachFunc = [&](IGraphics* pGraphics) {
  // Rotary knob with arc
  pGraphics->AttachControl(new IVKnobControl(
    IRECT(20, 60, 80, 120),
    kParamGain,
    "Gain",
    style::KnobStyle::kRotary,
    DEFAULT_STYLE,
    "dB",
    -60, 12
  ));
  
  // Horizontal slider
  pGraphics->AttachControl(new IVSliderControl(
    IRECT(100, 60, 300, 90),
    kParamMix,
    "Mix",
    style::SliderStyle::kHorizontal,
    "dry/wet"
  ));
  
  // Toggle button
  pGraphics->AttachControl(new IVToggleControl(
    IRECT(100, 100, 130, 130),
    kParamBypass
  ));
};
```

### JUCE Component-Based Control
```cpp
// Custom JUCE rotary knob
class RotaryKnob : public juce::Component {
public:
  RotaryKnob(juce::RangedAudioParameter& param)
    : parameter(param) {}
  
  void paint(juce::Graphics& g) override {
    auto bounds = getLocalBounds().toFloat();
    
    // Draw background arc
    g.setColour(Colours::darkgrey);
    g.drawArc(bounds.reduced(10).toFloat(), 
              startAngle, endAngle - startAngle, false, 3.0f);
    
    // Draw value arc
    g.setColour(accentColour);
    float valueAngle = startAngle + 
      (value * (endAngle - startAngle));
    g.drawArc(bounds.reduced(10).toFloat(),
              startAngle, valueAngle - startAngle, false, 3.0f);
    
    // Draw center circle
    g.setColour(Colours::lightgrey);
    g.fillEllipse(bounds.reduced(20).toFloat());
  }
  
private:
  juce::RangedAudioParameter& parameter;
  float value = 0.5f;
  float startAngle = 2.5f;
  float endAngle = 6.0f;
};
```

## Anti-Patterns to Avoid

**2000s Clunky**:
- Gradients on everything
- Embossed/beveled edges
- Fake wood or metallic textures
- Unnecessary 3D effects
- Overly detailed knob textures
- Low-contrast text on metallic backgrounds

**AI-Generated Aesthetic**:
- Purple-to-blue gradient backgrounds
- Neon glow effects
- Glassmorphism panels
- Generic rounded rectangles everywhere
- Thin hairline borders
- Inter font at small sizes

**Modern Professional** Should:
- Purposeful color usage
- Clear hierarchy
- Readable at any scale
- Responsive to parameter changes
- Consistent spacing and alignment
- Subtle depth when needed

## Polish Checklist

- [ ] All knobs have clear value indication
- [ ] Slider scales are marked appropriately
- [ ] Text is readable at minimum window size
- [ ] Colors have sufficient contrast
- [ ] Hover/active states are visible
- [ ] Bypass state is clearly indicated
- [ ] Meters are calibrated correctly
- [ ] No visual artifacts during animation
- [ ] Graceful handling of missing assets
- [ ] Consistent visual language throughout
