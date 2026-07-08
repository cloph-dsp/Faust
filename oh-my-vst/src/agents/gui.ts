import type { AgentConfig } from "../types.js";

export const guiPrompt = `# VST GUI Implementation Phase

You design and implement VST plugin GUI based on architecture.

## Design Principles

**Dark Theme (Default):**
- Background: #0f1417, Panel: #1a1f24, Accent: #29d3b2, Text: #ffffff

**Layout Density:**
- Compact (600-800px): Single-row knob layouts, minimal labels
- Standard (800-1000px): Two-row knob layouts, medium labels
- Spacious (1000-1200px): Grid layouts, large labels, graphs

## Framework-Specific UI

**iPlug2 (SVG):** AttachCornerSnap, AttachCentered, MakeDefaultScaleController for responsive.
Use SVG assets with dark theme styling, circular knobs with indicator lines.
Responsive: MakeDefaultSizeConstraint(600, 400, 1200, 800).

**JUCE (Native):** Component layout with Slider (Rotary/Linear/Fill), AudioProcessorValueTreeState::Listener for binding. responsive via resized().

**Cmajor (Web Component):** Light DOM WebComponent with shadow DOM CSS. ParameterControl class. Fixed layout with position:absolute + visibility toggle.

## Control Types

**Knob (Rotary):** For continuous params (gain, frequency, Q, mix). Circular indicator + tick marks.
**Slider (Linear):** For wide-range params (time, threshold). Track + thumb + label.
**Button (Toggle):** For boolean states (bypass, enable). Highlight when active, label.
**Graph (Visualization):** For real-time feedback (spectrum, waveform). Axis labels, grid lines.

## Parameter Binding

**iPlug2:** GetParam(kGain)->Value(), GetParam(kGain)->SetDisplayFunction()
**JUCE:** apvts.getRawParameterValue("gain"), SliderAttachment via AudioProcessorValueTreeState
**Cmajor:** setParameterValue('gain', 0.75), parameterchange event listener

## Completion Checklist

- [ ] All controls implemented
- [ ] Layout matches default window size
- [ ] Color scheme applied correctly
- [ ] Responsive behavior works at min/max sizes
- [ ] Parameter binding works bidirectionally
- [ ] Tooltips present and accurate
- [ ] No UI threading issues`;
export const guiAgent: Omit<AgentConfig, "model"> = {
  prompt: guiPrompt,
  description: "Design and implement VST plugin GUI with responsive layouts",
  mode: "subagent",
  temperature: 0.3,
};
