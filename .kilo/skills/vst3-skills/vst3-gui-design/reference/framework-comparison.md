# VST3 Framework Comparison

Quick reference for choosing between iPlug2, JUCE, and Faust for your plugin.

## At a Glance

| Feature | iPlug2 | JUCE | Faust |
|---------|--------|------|-------|
| **Best For** | Cross-platform VST/AU | Full control, complex apps | DSP algorithm focus |
| **GUI System** | IGraphics (built-in) | Custom components | Architecture-based |
| **SVG Support** | Excellent (ISVGBase) | Via Drawable | Limited (custom) |
| **Learning Curve** | Moderate | Steep | Low (DSP only) |
| **Binary Size** | Small | Medium-Large | DSP-dependent |
| **Development Speed** | Fast | Slower | Fastest for DSP |

## iPlug2

### Pros
- Modern C++ API with good documentation
- Built-in IGraphics system with controls
- Excellent SVG support via ISVGBase
- Good default look, customizable
- Good cross-platform support (VST3, AU, AAX)
- Active community and development

### Cons
- IGraphics learning curve
- Less control than raw JUCE
- Some features require paid tier

### When to Choose
- Want built-in controls without reinventing
- Need good SVG support
- Building standard effect plugins
- Want faster development than JUCE

### Example: Basic Knob
```cpp
pGraphics->AttachControl(new IVKnobControl(
  IRECT(20, 60, 80, 120),
  kParamGain,
  "Gain",
  style::KnobStyle::kRotary,
  DEFAULT_STYLE,
  "dB", -60, 12
));
```

## JUCE

### Pros
- Maximum control over everything
- Custom rendering for unique UIs
- JUCE 8: Direct2D for Windows, WebView for web UIs
- Largest community, most resources
- Full cross-platform (mobile, desktop, web)

### Cons
- Significant boilerplate
- Steeper learning curve
- Larger binaries
- More code to maintain

### When to Choose
- Need completely custom UI
- Building complex applications
- Want WebView/React-based UI
- Need maximum control over rendering
- Already experienced with JUCE

### Example: Custom Knob
```cpp
class MyKnob : public juce::Component {
  void paint(juce::Graphics& g) override {
    // Full custom drawing
    g.drawEllipse(bounds, 2.0f);
  }
};
```

## Faust

### Pros
- Fast DSP development
- Compile to many targets (VST, AU, LV2, Web, etc.)
- DSP-first approach
- Good for algorithms, less for full plugins
- Free, open source

### Cons
- Limited UI flexibility
- Need C++ wrapper for full plugin
- Less control over plugin behavior

### When to Choose
- DSP algorithm is the main focus
- Need to target many formats quickly
- Will wrap in custom C++ framework
- Learning DSP fundamentals

### Example: Basic DSP
```cpp
process = _,_ : *(gain) : fi.lowpass(fr, fs);
```

## Hybrid Approaches

### Faust + iPlug2
```
Faust DSP → iPlug2 wrapper → Full plugin with custom UI
```
Best for: DSP-focused with professional UI

### Faust + JUCE
```
Faust DSP → JUCE wrapper → Full plugin
```
Best when: Need JUCE's features but want Faust DSP

## Decision Guide

1. **How much UI customization?**
   - Standard controls → iPlug2
   - Unique UI → JUCE

2. **What's your primary focus?**
   - DSP algorithms → Faust
   - Full product → iPlug2 or JUCE

3. **How much time for learning?**
   - Limited time → iPlug2
   - Want to learn thoroughly → JUCE
   - DSP only → Faust

4. **Need SVG assets?**
   - Yes → iPlug2 or JUCE (not Faust)

## Resources

- iPlug2: https://iplug2.com
- JUCE: https://juce.com
- Faust: https://faust.grame.fr
- VST3 SDK: steinbergmedia.github.io/vst3_dev_portal