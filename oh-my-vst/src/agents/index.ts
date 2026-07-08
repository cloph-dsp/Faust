import type { AgentConfig } from "../types.js";
import { orchestratorPrompt } from "./orchestrator.js";
import { specPrompt } from "./spec.js";
import { architectPrompt } from "./architect.js";
import { dspPrompt } from "./dsp.js";
import { guiPrompt } from "./gui.js";
import { validatePrompt } from "./validate.js";

export type { orchestratorPrompt } from "./orchestrator.js";
export type { specPrompt } from "./spec.js";
export type { architectPrompt } from "./architect.js";
export type { dspPrompt } from "./dsp.js";
export type { guiPrompt } from "./gui.js";
export type { validatePrompt } from "./validate.js";

export interface AgentEntry {
  name: string;
  config: AgentConfig;
}

/**
 * Create all VST development agents.
 * Returns a flat array of { name, config } ready for config.agent registration.
 */
export function createAgents(options?: {
  model?: string;
}): AgentEntry[] {
  const model = options?.model;

  return [
    {
      name: "vst-orchestrator",
      config: {
        model,
        prompt: orchestratorPrompt,
        description: "VST plugin development orchestrator managing progressive workflow phases",
        mode: "primary",
        temperature: 0.3,
      },
    },
    {
      name: "vst-spec",
      config: {
        model,
        prompt: specPrompt,
        description: "Define VST plugin specification and requirements",
        mode: "subagent",
        temperature: 0.3,
      },
    },
    {
      name: "vst-architect",
      config: {
        model,
        prompt: architectPrompt,
        description: "Design VST plugin architecture and choose framework",
        mode: "subagent",
        temperature: 0.3,
      },
    },
    {
      name: "vst-dsp",
      config: {
        model,
        prompt: dspPrompt,
        description: "Implement VST plugin DSP algorithms with numerical validation",
        mode: "subagent",
        temperature: 0.2,
      },
    },
    {
      name: "vst-gui",
      config: {
        model,
        prompt: guiPrompt,
        description: "Design and implement VST plugin GUI with responsive layouts",
        mode: "subagent",
        temperature: 0.3,
      },
    },
    {
      name: "vst-validate",
      config: {
        model,
        prompt: validatePrompt,
        description: "Quality validation and testing for VST plugins",
        mode: "subagent",
        temperature: 0.2,
      },
    },
  ];
}
