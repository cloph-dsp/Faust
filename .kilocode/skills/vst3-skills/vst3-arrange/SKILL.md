---
name: vst3-arrange
description: Improve VST3 plugin layout, spacing, and visual hierarchy. Use when the UI feels cramped, cluttered, or has weak hierarchy. Works with iPlug2, JUCE, and Faust UIs.
user-invocable: true
argument-hint: "[target]"
---

Improve plugin layout for better usability and visual hierarchy.

## Assess Current Layout

### Identify Issues
1. **Spacing**: Is spacing arbitrary or consistent?
2. **Visual hierarchy**: What's most important? Can you tell at a glance?
3. **Grouping**: Are related controls clearly grouped?
4. **Flow**: Does the layout follow signal flow or logical sections?
5. **Density**: Too cramped? Too sparse?

### Common Plugin Layout Problems
- Controls crammed together
- No clear section boundaries
- Important parameters hidden in small text
- Signal flow not apparent
- Mixed layout approaches (grid + freeform)

## Improve Layout Systematically

### Establish Spacing System
```
Base unit: 8px

Knob spacing:
- Related controls: 16-24px between centers
- Control rows: 16px vertical spacing

Section spacing:
- Between groups: 32-48px
- Section headers: 24px above, 8px below

Window margins:
- Minimum: 16px all sides
- Header area: 8px padding
```

### Group by Function
```
[INPUT SECTION]
  └─ Input gain, Input routing, Phase

[PROCESSING SECTION]
  └─ Filter, Dynamics, Effects chain

[OUTPUT SECTION]
  └─ Output gain, Mix, Limiter
```

### Create Visual Hierarchy
- First glance: Which parameters are most important?
- Second glance: What are the main sections?
- Clear labels for each section
- Primary controls larger, secondary smaller

### Signal Flow Layout
For effects, arrange left-to-right or top-to-bottom:
```
INPUT → [ PROCESSING STAGES ] → OUTPUT
        ↓
     BYPASS
```

### Control Size Strategy
- **Large (60-80px)**: Primary parameters (gain, threshold, mix)
- **Medium (40-50px)**: Secondary parameters (attack, release)
- **Small (24-32px)**: Tertiary (mode, routing, options)

## Framework Layout Systems

### iPlug2 Layout
```cpp
// Grid-based layout
mAttachFunc = [&](IGraphics* pGraphics) {
  int y = 20;
  
  // Section header
  pGraphics->AttachControl(new ITextControl(
    IRECT(20, y, 200, y + 20),
    "INPUT", headerStyle
  ));
  y += 30;
  
  // Controls
  pGraphics->AttachControl(new IVKnobControl(
    IRECT(20, y, 80, y + 60), kParamInput
  ));
  pGraphics->AttachControl(new IVKnobControl(
    IRECT(100, y, 160, y + 60), kParamInputGain
  ));
  y += 80;
  
  // Next section...
};
```

### JUCE Layout
```cpp
// FlexBox or manual layout
void AudioPluginEditor::resized() {
  auto area = getLocalBounds().reduced(16);
  
  // Header
  auto header = area.removeFromTop(30);
  logo.setBounds(header.removeFromLeft(100));
  title.setBounds(header);
  
  // Section 1
  auto section1 = area.removeFromTop(100);
  knob1.setBounds(section1.removeFromLeft(80));
  knob2.setBounds(section1.removeFromLeft(80));
  
  // Section 2
  auto section2 = area.removeFromTop(100);
  // ...
}
```

### Layout Considerations

**Window resizing**:
- Allow horizontal or vertical expansion
- Use FlexBox or Grid for responsive layouts
- Maintain control proportions

**Minimum size**:
- Set `setResizeLimits(minW, minH, maxW, maxH)`
- Ensure all controls visible at minimum size

## Verify Layout Improvements

- **Squint test**: Clear hierarchy at a glance?
- **Signal flow**: Can users trace the audio path?
- **Spacing rhythm**: Consistent gaps, or variety with purpose?
- **Proportional sizing**: Primary controls larger than secondary?
- **In-context**: Works in actual DAW window?

Remember: Good layout makes the plugin feel intuitive and professional. Bad layout makes even great DSP feel amateur.