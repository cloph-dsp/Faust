# VST3 Skills

A comprehensive set of skills for creating professional VST3 audio plugins with modern aesthetics. Supports iPlug2, JUCE, Faust, and raw C++ implementations.

## Skills Overview

### Core DSP Skills
| Skill | Purpose |
|-------|---------|
| `/vst3-spec` | Define plugin brief and requirements |
| `/vst3-architect` | Design plugin architecture |
| `/vst3-voice` | Refine DSP behavior and tone |
| `/vst3-optimize` | Optimize CPU, latency, real-time safety |
| `/vst3-verify` | Validate quality and release readiness |

### GUI Design Skills
| Skill | Purpose |
|-------|---------|
| `/vst3-gui-design` | Create modern plugin GUIs with SVG support |
| `/vst3-polish` | Final quality pass for UI polish |
| `/vst3-arrange` | Improve layout and visual hierarchy |

### Quality Skills
| Skill | Purpose |
|-------|---------|
| `/vst3-audit` | Technical quality and compatibility checks |
| `/vst3-harden` | Edge case robustness |

## When to Use Which Skill

### Starting a New Plugin
1. Run `/vst3-spec` to define your brief
2. Run `/vst3-architect` to design the architecture
3. Run `/vst3-gui-design` to create the UI

### During Development
- `/vst3-voice` - Refine the DSP behavior
- `/vst3-optimize` - Performance optimization
- `/vst3-arrange` - Layout improvements

### Before Release
- `/vst3-verify` - Full validation
- `/vst3-audit` - Technical audit
- `/vst3-polish` - Final polish
- `/vst3-harden` - Edge case testing

## Framework Support

All skills support examples for:
- **iPlug2**: Modern C++ framework with IGraphics
- **JUCE**: Full-featured C++ framework
- **Faust**: DSP-first approach
- **Hybrid**: Faust DSP with C++ wrapper

## GUI Design Philosophy

These skills emphasize:
- Modern aesthetics (no clunky 2000s look)
- SVG asset support
- Professional color and typography
- Clear visual hierarchy
- Framework-agnostic principles

## Reference Files

Located in `vst3-gui-design/reference/`:
- `svg-workflow.md` - SVG creation and implementation
- `modern-plugin-trends.md` - 2024-2025 UI trends
- `control-design.md` - Knobs, sliders, meters, buttons

## Usage

Invoke any skill directly:
```
/vst3-spec [target behavior]
/vst3-architect [architecture brief]
/vst3-gui-design [plugin context]
```

Skills are independent - use them in any order as needed.