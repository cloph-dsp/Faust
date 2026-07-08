import { orchestratorPrompt } from "./orchestrator.js";
import { specPrompt } from "./spec.js";
import { architectPrompt } from "./architect.js";
import { dspPrompt } from "./dsp.js";
import { guiPrompt } from "./gui.js";
import { validatePrompt } from "./validate.js";

export interface AgentDef {
  id: string;
  prompt: string;
  description: string;
  mode: "primary" | "subagent" | "all";
  temperature?: number;
}

/**
 * Create all VST development agent definitions for oh-my-vst plugin.
 */
export function createAgentDefs(): AgentDef[] {
  return [
    {
      id: "vst-orchestrator",
      prompt: orchestratorPrompt,
      description:
        "VST plugin development orchestrator managing progressive workflow phases",
      mode: "primary",
      temperature: 0.3,
    },
    {
      id: "vst-spec",
      prompt: specPrompt,
      description: "Define VST plugin specification and requirements",
      mode: "subagent",
      temperature: 0.3,
    },
    {
      id: "vst-architect",
      prompt: architectPrompt,
      description: "Design VST plugin architecture and choose framework",
      mode: "subagent",
      temperature: 0.3,
    },
    {
      id: "vst-dsp",
      prompt: dspPrompt,
      description:
        "Implement VST plugin DSP algorithms with numerical validation",
      mode: "subagent",
      temperature: 0.2,
    },
    {
      id: "vst-gui",
      prompt: guiPrompt,
      description: "Design and implement VST plugin GUI with responsive layouts",
      mode: "subagent",
      temperature: 0.3,
    },
    {
      id: "vst-validate",
      prompt: validatePrompt,
      description: "Quality validation and testing for VST plugins",
      mode: "subagent",
      temperature: 0.2,
    },
  ];
}
