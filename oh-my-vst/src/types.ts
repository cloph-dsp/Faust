/** Agent configuration matching the @opencode-ai/sdk AgentConfig type */
export interface AgentConfig {
  model?: string;
  temperature?: number;
  top_p?: number;
  prompt?: string;
  description?: string;
  mode?: "subagent" | "primary" | "all";
  color?: string;
  maxSteps?: number;
  disable?: boolean;
  permissions?: {
    edit?: "ask" | "allow" | "deny";
    bash?: "ask" | "allow" | "deny" | Record<string, "ask" | "allow" | "deny">;
    webfetch?: "ask" | "allow" | "deny";
  };
  [key: string]: unknown;
}

/** Framework preset data */
export interface Preset {
  name: string;
  description: string;
  /** Agent-specific overrides: skills, MCPs, prompt additions */
  agents: Record<string, {
    skills?: string[];
    mcps?: string[];
  }>;
  /** Per-agent prompt additions specific to this framework */
  prompts: Record<string, string>;
}


