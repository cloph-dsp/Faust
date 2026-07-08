# 3 Beat Problem — Implementation Plan

## Product
Polyphonic algorithmic MIDI generator VST3 plugin with dual mode: Melody (scale-based) + Drum (pattern-based).

## Architecture
- **Framework**: iPlug2 (matching existing DataBend/Freeze95 patterns)
- **Type**: MIDI Effect (PLUG_TYPE=2, PLUG_CHANNEL_IO="0-0")
- **Window**: 680×480
- **Modes**: Melody (8-voice polyphonic) | Drum (3 fixed sequencers)

## Wave 1: Project Scaffold & Build System (5 tasks)

| # | Task | Description |
|---|------|-------------|
| 1.1 | Directory structure | Create 3BeatProblem/ with projects/, scripts/, resources/ |
| 1.2 | config.h | PLUG_TYPE=2, PLUG_CHANNEL_IO="0-0", MIDI_IN=1, MIDI_OUT=1, W=680, H=480 |
| 1.3 | vcxproj | Based on DataBend vcxproj pattern |
| 1.4 | Build scripts | prebuild/postbuild-win.bat (copy + adapt from DataBend) |
| 1.5 | Resources | Placeholder .ico + main.rc |

## Wave 2: Core DSP Engine (11 tasks)

| # | Task | Description |
|---|------|-------------|
| 2.1 | scale_system.h | 12 scale definitions as semitone interval arrays |
| 2.2 | Mood mappings | Dark→{0,2,4}, Happy→{0,4,6}, Sad→{0,3,4}, Bright→{4,5,6}, Tense→{1,3,5}, Dreamy→{0,4,6}, Random, Chordal→{0,2,4,6} |
| 2.3 | SequencerMode enum | Mode-switching logic |
| 2.4 | Voice struct | Melody: rootNote, velocity, seq[3] array |
| 2.5 | DrumSequencer struct | Fixed note, velocity pattern, mute/solo state |
| 2.6 | Voice allocation | 8-voice pool for melody mode |
| 2.7 | Step trigger | Phase accumulation, probability gate (both modes) |
| 2.8 | Note generation | Melody: scale→MIDI; Drum: fixed note + velocity |
| 2.9 | Drum operations | Clear, Fill(2/3/4), Invert, Rotate(L/R), Mutate |
| 2.10 | Parameter handlers | Dual-mode param processing |
| 2.11 | State serialization | Mode-aware SerializeState/UnserializeState |

## Wave 3: Plugin Shell (9 tasks)

| # | Task | Description |
|---|------|-------------|
| 3.1 | 3BeatProblem.h | Mode-aware class inheriting iplug::Plugin |
| 3.2 | ProcessMidiMsg() | Melody: spawn voices; Drum: ignore |
| 3.3 | ProcessBlock() | Mode-aware sequencer advancement + SendMidiMsg() |
| 3.4 | Global params | Mode, BPM, Sync, Swing, Transport, Velocity |
| 3.5 | Melody params | Rate, Steps, Mood, Octave ranges (×3 seqs) |
| 3.6 | Drum params | MIDI Note, Mute, Solo, Velocity Pattern (×3 seqs) |
| 3.7 | Mode switch | Reset voices/sequencers on mode change |
| 3.8 | State chunks | Preset save/load with mode preservation |
| 3.9 | IGraphics setup | UI initialization |

## Wave 4: UI Implementation (11 tasks)

| # | Task | Description |
|---|------|-------------|
| 4.1 | Layout design | Mode toggle in top bar, 3 sequencer columns |
| 4.2 | Circular sequencer | Custom IControl for ring display (both modes) |
| 4.3 | Playhead rendering | Rotating indicator synced to step |
| 4.4 | Step editing | Click toggle, right-click accent (drum mode) |
| 4.5 | Mode toggle | Melody/Drum dropdown |
| 4.6 | Melody controls | Mood dropdown, Octave knobs per sequencer |
| 4.7 | Drum controls | MIDI Note picker, Mute/Solo buttons per sequencer |
| 4.8 | Drum operations | Clear, Fill, Invert, Rotate, Mutate buttons |
| 4.9 | Transport | Play/Stop/Panic buttons |
| 4.10 | Global controls | BPM, Swing, Sync, Velocity |
| 4.11 | SVG assets | UI element graphics |

## Wave 5: Verification (5 tasks)

| # | Task | Description |
|---|------|-------------|
| 5.1 | Wire components | Connect DSP + UI + Plugin |
| 5.2 | Debug build | Compile + fix LSP diagnostics |
| 5.3 | Release build | Compile optimized |
| 5.4 | Verify checks | Parameter metadata, chunk serialization |
| 5.5 | DAW test | Reaper/Studio One load test |

## Parameters (Total: ~30)

### Global (shared)
- Mode: Melody/Drum (enum)
- Sync: On/Off
- BPM: 40-300
- Swing: 0-100%
- Duration: 10-100%
- Velocity: 0-127
- Transport: Play/Stop (not a VST3 param)

### Melody Mode (×3 sequencers)
- Rate: 1/64 to 4x
- Steps: 2-16
- Pattern: 16 bools
- Mood: enum (8 options)
- Scale Root: 0-11
- Scale Type: enum (12 scales)
- Octave Low: 0-8
- Octave High: 0-8
- Vel Random: 0-100%
- Probability: 0-100%

### Drum Mode (×3 sequencers)
- Rate: 1/64 to 4x
- Steps: 2-16
- Pattern: 16 bools
- Velocity Pattern: 0-2 (×16)
- MIDI Note: 0-127
- Mute: bool
- Solo: bool
