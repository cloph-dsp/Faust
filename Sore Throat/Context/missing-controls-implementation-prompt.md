# Prompt: Implement Missing Non-Sequencer Charsiesis Controls in VChorus

You are implementing missing Charsiesis-style controls in this codebase:
- `VChorus/VChorus.h`
- `VChorus/VChorus.cpp`
- `VChorus/IVDisplay.h`

Use iPlug2 idioms and keep the plugin stable/performance-safe.

## Goal
Bring `VChorus` closer to the Charsiesis control set found in the skin/action files, but **exclude all sequencer features** (`Seq*` actions and step sliders).

## Current State (already implemented)
Current params in `VChorus` are:
- `Gain`
- `Delay Range` (single delay range control)
- `Voices`
- `Damper` (custom attenuation control)
- `Mix`
- `Low-Pass` (simple wet LP amount)
- `Rate Range`
- `Rate Update`

Current GUI is a simple knob grid and one dots display. Most Charsiesis controls are missing.

## Source Of Truth For Missing Controls
Use the non-sequencer actions from the skin files:
- `Voices`, `MinDelay`, `DelayRange`, `StereoMode`
- `MinRate`, `RateRange`, `RateUpdate`, `UpdateUnit`, `UpdateQuantize`, `UpdateOnCollision`
- `Feedback`, `Rotation`, `FollowLevel`
- `Lowpass`, `Highpass`, `LPRes`, `HPRes`, `EnvLP`, `EnvHP`, `EnvDelay`
- `Mix`, `Pregain`, `Postgain`
- `LFORate`, `LFOUnit`, `LFOQuant`, `LFOShape`, `LFOLP`, `LFOHP`, `LFODelay`

## Required Features To Add (Non-Sequencer)

### 1) Delay/randomization core parity
- Add `MinDelay` (ms) separate from `DelayRange`.
- Add `MinRate` separate from `RateRange`.
- Keep per-voice randomization, but compute ranges as:
  - delay target in `[MinDelay, MinDelay + DelayRange]`
  - rate magnitude in `[MinRate, MinRate + RateRange]`
- Add `StereoMode` enum with 4 modes:
  - `Free`
  - `Slave`
  - `AntiSlave`
  - `Half`
- Implement channel coupling behavior according to the Charsiesis text description.

### 2) Collision/update behavior (high priority)
- Add `UpdateOnCollision` toggle:
  - OFF: current bounce behavior (invert direction at boundaries).
  - ON: when a voice hits min/max delay, choose a new random signed rate (not just direction flip).
- Keep behavior deterministic per sample block and avoid denorm/NaN issues.

### 3) Rate-update quantization and units (high priority)
- Add `UpdateUnit` and `UpdateQuantize`.
- Support both free-time and musical timing workflows.

Implement as:
- `UpdateUnit` enum at least:
  - `Milliseconds`
  - `Seconds`
  - `NoteSyncHost`
  - `NoteSyncManualBPM`
- `UpdateQuantize` behavior:
  - For ms/sec units: snap update interval to integer unit steps.
  - For note-sync units: snap to a note-division table.
- Add note-division table for musical interval quantization, e.g.:
  - `1/64`, `1/32`, `1/16`, `1/8`, `1/4`, `1/2`, `1/1`
  - dotted and triplet variants for at least the common divisions.
- Host-sync mode must read host tempo when available.
- Manual-BPM mode must use an explicit BPM parameter fallback.
- If host tempo is unavailable, safely fallback to manual BPM or 120 BPM.

### 4) Feedback section
- Add `Feedback` amount.
- Add `Rotation` (stereo vector rotation in feedback loop).
- Add `FollowLevel` mode switch (multi-state, not just boolean):
  - `Off`
  - `Normal`
  - `FastDecay`
  - `SlowDecay`
- `FollowLevel` should prevent runaway feedback while preserving character.

### 5) Filter/envelope section
- Replace the simplified wet LP control with proper filter controls:
  - `Lowpass` cutoff
  - `Highpass` cutoff
  - `LPRes`
  - `HPRes`
- Filters should be in/around the wet-feedback path as in Charsiesis behavior.
- Add envelope follower and modulation amounts:
  - `EnvLP`
  - `EnvHP`
  - `EnvDelay`
- Use smoothing to prevent zipper noise.

### 6) Mix/gain section
- Keep `Mix` (dry/wet).
- Add `Pregain` and `Postgain`.
- Decide migration strategy for existing `Gain`:
  - Either map old `Gain` to `Postgain`, or keep compatibility layer for old presets.

### 7) LFO section (non-sequencer only)
- Add:
  - `LFORate`
  - `LFOUnit`
  - `LFOQuant`
  - `LFOShape`
  - `LFOLP`, `LFOHP`, `LFODelay`
- Provide LFO shape options and user-shape support (user curve editor can be basic first).
- LFO rate unit/quantization should mirror the update-rate timing model.

## UI Requirements
- Add controls for all non-sequencer actions listed above.
- Keep sequencer controls absent for this scope.
- Preserve readable value display and tooltips.
- Keep current dots display, and extend visualization only where useful.

## Architecture/Code Quality Requirements
- Keep allocations out of the real-time path.
- Add parameter smoothing where needed (cutoff, gain, feedback, modulation amounts).
- Keep behavior sample-rate aware.
- Keep stereo-safe behavior across mono/stereo host channel configurations.

## Acceptance Tests
1. `UpdateOnCollision = Off` produces boundary bounce behavior only.
2. `UpdateOnCollision = On` produces randomized rates at boundary collisions.
3. `UpdateQuantize = On` snaps update interval to whole units in ms/sec modes.
4. Note-sync update mode at 120 BPM yields expected intervals (e.g., quarter note = 500 ms).
5. `StereoMode` modes produce distinct left/right correlation patterns.
6. Feedback with `FollowLevel` does not run away at high feedback settings.
7. Sequencer actions (`Seq*`) are untouched and not required.

## Out Of Scope
- Sequencer step engine and sequencer UI (`SeqStep`, `SeqQuant`, `SeqUnit`, `SeqLP/HP/Delay`, `Seq0..Seq7`).

## Suggested Implementation Order
1. Delay core parity (`MinDelay`, `MinRate`, `StereoMode`).
2. Collision/update logic (`UpdateOnCollision`, `UpdateUnit`, `UpdateQuantize`, BPM/note sync).
3. Feedback section (`Feedback`, `Rotation`, `FollowLevel`).
4. Filter/envelope section.
5. Mix pre/post gain.
6. LFO section.
7. UI polish and preset migration checks.
