import type { AgentConfig } from "../types.js";
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
export declare function createAgents(options?: {
    model?: string;
}): AgentEntry[];
