#!/usr/bin/env node

import { readFileSync, writeFileSync, existsSync, mkdirSync } from "node:fs";
import { homedir } from "node:os";
import { join } from "node:path";

const CONFIG_DIR = join(homedir(), ".config", "opencode");
const CONFIG_PATH = join(CONFIG_DIR, "opencode.json");
const VST_CONFIG_PATH = join(CONFIG_DIR, "oh-my-vst.json");

const HELP = `
oh-my-vst — VST plugin development agents for OpenCode

Usage:
  oh-my-vst install   Install agents into opencode.json
  oh-my-vst help      Show this help

The plugin adds 6 specialized agents:
  vst-orchestrator  — workflow manager
  vst-spec          — product brief & requirements
  vst-architect     — framework selection & architecture
  vst-dsp           — DSP algorithm implementation
  vst-gui           — UI design & implementation
  vst-validate      — quality validation
`.trim();

interface VSTConfig extends Record<string, unknown> {
  model?: string;
  framework?: string;
}

function readJSON(path: string): Record<string, unknown> | null {
  try {
    if (existsSync(path)) {
      return JSON.parse(readFileSync(path, "utf-8"));
    }
  } catch {
    console.error(`Warning: could not read ${path}`);
  }
  return null;
}

function writeJSON(path: string, data: Record<string, unknown>): void {
  writeFileSync(path, JSON.stringify(data, null, 2) + "\n");
}

async function install(): Promise<void> {
  if (!existsSync(CONFIG_DIR)) {
    mkdirSync(CONFIG_DIR, { recursive: true });
  }

  // Read or create VST config
  let vstConfig: VSTConfig = {};
  const existingVST = readJSON(VST_CONFIG_PATH);
  if (existingVST && typeof existingVST === "object") {
    vstConfig = existingVST as VSTConfig;
  }
  writeJSON(VST_CONFIG_PATH, vstConfig);

  // Read existing opencode config
  const config = readJSON(CONFIG_PATH) ?? {};

  // Add plugin reference
  const plugins = (config.plugin as Array<string | [string, unknown]>) ?? [];
  const hasPlugin = plugins.some((p) => {
    const name = typeof p === "string" ? p : p[0];
    return name === "oh-my-vst";
  });

  if (!hasPlugin) {
    plugins.push("oh-my-vst");
  }

  config.plugin = plugins;
  writeJSON(CONFIG_PATH, config);

  console.log("✓ oh-my-vst installed!");
  console.log(`  Plugin added to ${CONFIG_PATH}`);
  console.log(`  Config at ${VST_CONFIG_PATH}`);
  console.log("  Restart OpenCode to activate the VST agents.");
  console.log("\n  Agents added: vst-orchestrator, vst-spec, vst-architect, vst-dsp, vst-gui, vst-validate");
}

async function main(): Promise<void> {
  const cmd = process.argv[2]?.toLowerCase();

  switch (cmd) {
    case "install":
      await install();
      break;
    case "help":
    case "--help":
    case "-h":
    case undefined:
      console.log(HELP);
      break;
    default:
      console.error(`Unknown command: ${cmd}\n`);
      console.log(HELP);
      process.exit(1);
  }
}

main().catch((err) => {
  console.error("Install failed:", err);
  process.exit(1);
});
