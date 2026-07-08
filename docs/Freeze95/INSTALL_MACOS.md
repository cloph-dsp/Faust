# Freeze95 — macOS Installation Guide

## Which file do I need?

| Format | File | Hosts |
|--------|------|-------|
| **Audio Unit (AU)** | `Freeze95.component` | Logic Pro, GarageBand, MainStage |
| **VST3** | `Freeze95.vst3` | Ableton Live, FL Studio, Reaper, Bitwig, Cubase |
| **CLAP** | `Freeze95.clap` | Bitwig, Reaper (with CLAP), Studio One |

> **Logic Pro / GarageBand users:** macOS does not support VST3. You MUST use the AU (`.component`) version.

## How to install

Each plugin is a **bundle** (a special folder that macOS treats as a single file). Do not open it — copy the whole folder.

### Option A: Copy to system-wide location (all users)

```bash
# VST3 (requires admin password)
sudo cp -R Freeze95.vst3 /Library/Audio/Plug-Ins/VST3/

# CLAP
sudo cp -R Freeze95.clap /Library/Audio/Plug-Ins/CLAP/

# AU
sudo cp -R Freeze95.component /Library/Audio/Plug-Ins/Components/
```

### Option B: Copy to user location (your account only)

```bash
mkdir -p ~/Library/Audio/Plug-Ins/VST3
cp -R Freeze95.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

## After installing

- **AU users:** Open `Audio MIDI Setup` → `Audio Units Manager` and verify Freeze95 appears.
- **VST3/CLAP users:** Rescan plugins in your DAW.
- If your DAW doesn't see the plugin, try restarting it.

## Troubleshooting

**"Freeze95.vst3 shows as a folder, not a file"** — This is normal. `.vst3`, `.clap`, and `.component` are macOS bundle directories. They look like folders in the Finder but your DAW recognises them as plugins. **Do not open them — copy them whole.**

**"macOS says Freeze95 is damaged/unidentified"** — Go to `System Settings` → `Privacy & Security` → scroll down and click `Open Anyway`.

**"I see a Unix executable file inside"** — That is the plugin binary (`Contents/MacOS/Freeze95`). You don't need to run it directly. Copy the **outer folder** (Freeze95.vst3 / Freeze95.clap / Freeze95.component) to your Plug-Ins folder.
