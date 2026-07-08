import type { Plugin } from "@opencode-ai/plugin";
/**
 * oh-my-vst — Progressive VST plugin development agents for OpenCode.
 *
 * Registers 6 specialized agents:
 *   vst-orchestrator  — workflow manager (primary)
 *   vst-spec          — product brief & requirements
 *   vst-architect     — framework selection & architecture
 *   vst-dsp           — DSP algorithm implementation
 *   vst-gui           — UI design & implementation
 *   vst-validate      — quality validation
 *
 * Usage: add "oh-my-vst" to opencode.json's plugin array.
 */
declare const plugin: Plugin;
export default plugin;
export type { Plugin } from "@opencode-ai/plugin";
export type { AgentConfig, Preset } from "./types.js";
export { createAgents } from "./agents/index.js";
export { presets } from "./presets.js";
