import type { Plugin, Config, Hooks } from "@opencode-ai/plugin";
import type { AgentConfig } from "@opencode-ai/sdk";
import { createAgents } from "./agents/index.js";

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
const plugin: Plugin = async (_input, _options): Promise<Hooks> => {
  return {
    config: async (config: Config): Promise<void> => {
      const agents = createAgents();
      const agent: Record<string, AgentConfig | undefined> = config.agent ??= {};

      for (const entry of agents) {
        agent[entry.name] = {
          ...(agent[entry.name] ?? {}),
          ...entry.config,
        };
      }
    },
  };
};

export default plugin;
export type { Plugin } from "@opencode-ai/plugin";
export type { AgentConfig, Preset } from "./types.js";
export { createAgents } from "./agents/index.js";
export { presets } from "./presets.js";
