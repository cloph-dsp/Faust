---
name: vst3-polish
description: Perform a final quality pass on VST3 plugin GUIs. Fix alignment, spacing, consistency, and micro-detail issues. Use when the UI looks close to done but something feels off, or when polishing for release.
user-invocable: true
argument-hint: "[target]"
---

Polish plugin GUIs to professional release quality.

## Pre-Polish Assessment

### Review Completeness
- Is it functionally complete?
- Are there known issues to preserve?
- What's the quality bar? (MVP vs flagship)
- When does it ship?

### Identify Polish Areas
- Visual inconsistencies
- Spacing and alignment issues
- Control state gaps
- Value display formatting
- Edge cases and error states

## Polish Dimensions

### Visual Alignment & Spacing

**Pixel-perfect alignment**:
- All controls on grid (8px baseline)
- Related controls grouped with consistent gaps
- Section headers aligned with controls

**Spacing scale**:
```
xs: 4px    (between label and control)
sm: 8px    (between related elements)
md: 16px   (between control groups)
lg: 24px   (between sections)
xl: 32px   (major section breaks)
```

### Typography Refinement

- **Hierarchy consistency**: Same elements use same sizes/weights
- **Value displays**: Monospace fonts for numbers, 14-18px
- **Labels**: Sans-serif, 10-12px, secondary color
- **Section headers**: Uppercase or title case, consistent

### Color & Contrast

- **Contrast ratios**: All text meets minimum 4.5:1
- **Token usage**: No hard-coded colors, use theme tokens
- **Theme consistency**: Works in dark DAW backgrounds
- **Accent usage**: Consistent (one primary accent color)

### Control States

Every control needs:
- **Default**: Resting state
- **Hover**: Subtle feedback (50-100ms)
- **Active/Dragging**: Clear engagement state
- **Disabled**: Reduced opacity, non-interactive
- **Bypass/Off**: Clear visual distinction

### Value Display

- Consistent number formatting (1 decimal vs 2)
- Appropriate units (dB, Hz, %, ms)
- Handle edge cases (-inf, max values)
- Proper alignment (center or right)

### Meters & Visualization

- Correct ballistics (attack/release)
- Peak hold indicators working
- Color zones correct (green/yellow/red)
- Update rate smooth (not jumpy)

### Edge Cases

- **Long parameter names**: Truncate or wrap gracefully
- **Small window size**: All controls visible and usable
- **Preset loading**: UI updates correctly
- **Bypass state**: Clear visual indication

## Polish Checklist

- [ ] All knobs aligned to grid
- [ ] Consistent spacing between control groups
- [ ] Typography hierarchy is clear
- [ ] All control states implemented (hover, active, disabled)
- [ ] Value displays formatted consistently
- [ ] Bypass state clearly visible
- [ ] Meters update smoothly
- [ ] Works at minimum window size
- [ ] No visual artifacts during animation
- [ ] Colors have sufficient contrast
- [ ] Theme tokens used throughout
- [ ] No leftover debug visuals

## Verify Polish Quality

- **Squint test**: Does hierarchy remain clear when blurred?
- **In-context test**: Does it look good in actual DAW?
- **Comparison**: Does it match top-tier plugin quality?
- **Edge case test**: Try extreme values, resize window

Remember: Polish is the difference between a working plugin and a professional product. Details matter.