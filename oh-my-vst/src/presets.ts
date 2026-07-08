import type { Preset } from "./types.js";

/**
 * Built-in VST framework presets.
 * Each preset provides framework-specific overrides for agent prompts,
 * MCP connections, and skill requirements.
 */
export const presets: Record<string, Preset> = {
  iplug2: {
    name: "iPlug2",
    description: "Cross-platform VST/AU/AAX with SVG UI, CMake+Ninja build",
    agents: {
      dsp: { skills: ["vst-creation"], mcps: ["context7"] },
      gui: { skills: ["vst-creation"], mcps: ["context7"] },
    },
    prompts: {
      architect:
        "Prefer iPlug2 architecture: IPlugPlugin subclass, IGraphics for SVG-based UI, IControl for custom controls. Use CMake+Ninja build system. File structure: plugin.h/cpp main class, separate UI file in IGraphics/.",
      dsp: "Implement DSP in iPlug2 ProcessBlock. Use GetParam()->Value() for parameters. Prefer pre-allocated std::array buffers. Use WDL std:: or raw C arrays for real-time safety.",
      gui: "Use SVG graphics with ISVG control. Path resources/ for SVGs. Use IControl::OnDrag for interaction. Support dark/light themes by swapping SVG sets.",
    },
  },

  juce: {
    name: "JUCE",
    description: "Full control, complex plugins, WebView UI, Projucer project",
    agents: {
      dsp: { skills: ["vst-creation"], mcps: ["context7"] },
      gui: { skills: ["vst-creation"], mcps: ["context7"] },
    },
    prompts: {
      architect:
        "Prefer JUCE architecture: AudioProcessor + AudioProcessorEditor. Use AudioProcessorValueTreeState for parameters. Use juce::dsp::* for DSP modules. File structure: PluginProcessor.h/cpp + PluginEditor.h/cpp under Source/.",
      dsp: "Implement DSP in processBlock. Use AudioProcessorValueTreeState::getRawParameterValue(). Use juce::dsp::Oversampling for antialiasing. Prefer AudioBuffer<float> with getWritePointer().",
      gui: "Use AudioProcessorEditor with Component layout. Use Slider (Rotary/Linear/Fill) for controls. Attach via SliderAttachment or AudioProcessorValueTreeState::Listener. Support resized() for responsive layout.",
    },
  },

  faust: {
    name: "Faust",
    description: "Rapid DSP prototyping with C++ wrapper for production",
    agents: {
      dsp: { skills: ["vst-creation"], mcps: ["context7"] },
    },
    prompts: {
      architect:
        "Hybrid approach: Faust for DSP algorithm authorship + C++ wrapper for VST3 integration. Use faust2api or manual wrapper generation. File structure: myplugin.dsp + wrapper/ (plugin + UI).",
      dsp: "Write DSP in Faust .dsp file. Use standard Faust libraries (stdfaust.lib). Export parameters as hslider/vslider/nentry. Use import() for modular DSP. Generate C++ via faust2api or faust2vst.",
      gui: "UI depends on wrapper: iPlug2 SVG if using iPlug2 wrapper, native JUCE if JUCE wrapper. Prefer Faust's built-in layout (hgroup/vgroup) for automatic parameter organization in the wrapper.",
    },
  },

  cmajor: {
    name: "Cmajor",
    description: "Modern DSP language for the Amorph platform",
    agents: {
      dsp: { mcps: ["amorph", "amorph-instrument", "amorph-midi"] },
      gui: { mcps: ["amorph", "amorph-instrument", "amorph-midi"] },
    },
    prompts: {
      architect:
        "Cmajor + Amorph: single-file dsp.cmajor (processor) + index.js (UI WebComponent). Use Amorph MCP for compilation, QA probes, and draft application. Variant: fx/instrument/midi depending on plugin type.",
      dsp: "Cmajor conventions: processor Name, output before input endpoints, float64 for phase accumulators, processor.frequency for sample rate, no u literal suffix, no double keyword. Use struct for biquad/buffer patterns. Event handlers for MIDI. var inside loops, not let.",
      gui: "Light DOM WebComponent. Constructor with no args, anonymous subclass pattern for patchConnection. :host,element-name CSS. ParameterControl class. No import/export cross-file. Fixed layout with position:absolute + visibility toggle.",
    },
  },

  rust: {
    name: "Rust",
    description: "Memory-safe audio plugins with nih-plug or vst-rs",
    agents: {
      dsp: { skills: ["vst-creation"], mcps: ["context7"] },
    },
    prompts: {
      architect:
        "Use nih-plug or vst-rs for Rust VST development. nih-plug preferred for modern features (automation, GUI, CLAP/VST3). File structure: Cargo.toml + src/lib.rs. Use cpal for standalone mode, VST3 SDK bindings for plugin mode.",
      dsp: "Implement Plugin::process() trait. Use f32/f64 types. Pre-allocate Vec with capacity in initialize(). Use std::sync::atomic for parameter sharing. Implement Default trait for plugin struct.",
      gui: "Use nih-plug's params::editor module or baseview/egui for UI. Options: baseview (native), egui (immediate mode), iced (elm architecture). Parameter binding via param::internals::ParamPtr.",
    },
  },
};
