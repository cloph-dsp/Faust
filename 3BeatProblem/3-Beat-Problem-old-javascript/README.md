# 3 Beat Workstation

Single-page polyrhythm workstation built with vanilla JS + Tone.js.

## Features

- Multi-lane sequencer with **add/remove lanes** (up to 12).
- Hybrid editing per lane:
  - Circular step editor
  - Compact step grid editor
- Transport controls:
  - Play/Stop
  - BPM
  - Bars (loop length)
  - Swing
  - Panic (kill all voices)
- Lane tools:
  - Mute / Solo
  - Copy / Paste
  - Clear
  - Fill (2/3/4)
  - Invert
  - Rotate left/right
- Humanization and workflow:
  - Step accent cycling (soft/mid/hard)
  - Mutate lane
  - Undo / Redo
- Persistence/export:
  - Autosaved session in `localStorage`
  - Presets (IndexedDB with localStorage fallback)
  - Session JSON import/export
  - MIDI export

## Quick Start

Run from a local web server (not `file://`):

```bash
python -m http.server 8000
```

Open:

```text
http://localhost:8000
```

## Controls

### Mouse / UI

- Click steps to toggle on/off.
- Right-click a step to cycle accent level.
- Use lane toolbar buttons for lane-level operations.

### Keyboard

- `Space`: Play / Stop
- `Ctrl/Cmd + Z`: Undo
- `Ctrl/Cmd + Shift + Z`: Redo
- `R`: Mutate active lane
- `C`: Copy active lane
- `V`: Paste to active lane
- `1-9`: Select lane by index
- `Left/Right`: Move active step
- `Enter`: Toggle active step
- `A`: Cycle active step accent
- `?`: Toggle shortcuts panel

## Project Structure

- `index.html`: App shell and controls.
- `styles.css`: UI styling, responsive behavior, reduced-motion support.
- `app.js`: Bootstrap, store/audio/ui wiring.
- `statemanager.js`: Reducer store, action handling, undo/redo, persistence.
- `audioengine.js`: Tone.js scheduling, lane audio graph, playback sync.
- `uicontroller.js`: Rendering, interaction handlers, shortcuts, presets, import/export.
- `midiexport.js`: MIDI file generation and download.

## Notes

- `GET /favicon.ico 404` in local server logs is harmless unless you add a favicon file.
- If audio is blocked, ensure:
  - You started from a web server
  - Network access to CDN scripts is available
  - You interact with the page before playback (browser audio policy)

## License

Add your preferred license in `LICENSE`.
