const MIDI_NOTE_MAP = {
  C: 60, "C#": 61, D: 62, "D#": 63, E: 64, F: 65,
  "F#": 66, G: 67, "G#": 68, A: 69, "A#": 70, B: 71
};

function noteToMidiNumber(note) {
  if (!note || note.length < 2) return 60;
  const noteName = note.slice(0, -1);
  const octave = parseInt(note.slice(-1), 10);
  const base = MIDI_NOTE_MAP[noteName];
  if (base === undefined || Number.isNaN(octave)) return 60;
  return base + (octave - 4) * 12;
}

function writeVarLength(view, offset, value) {
  let v = Math.max(0, Math.min(0x0fffffff, value));
  const bytes = [];
  do {
    bytes.push(v & 0x7f);
    v >>= 7;
  } while (v > 0);
  for (let i = bytes.length - 1; i >= 0; i -= 1) {
    let out = bytes[i];
    if (i > 0) out |= 0x80;
    view.setUint8(offset.value++, out);
  }
}

function createMidiEvents(state) {
  const ticksPerBeat = 480;
  const events = [{
    tick: 0,
    meta: true,
    type: "setTempo",
    microsecondsPerBeat: Math.floor(60000000 / state.transport.bpm)
  }];

  state.lanes.forEach((lane, laneIndex) => {
    const totalSteps = lane.divisions * state.transport.barMultiplier;
    const ticksPerStep = Math.floor((ticksPerBeat * 4) / lane.divisions);
    const channel = laneIndex;

    for (let step = 0; step < totalSteps; step += 1) {
      const sourceStep = step % lane.divisions;
      if (!lane.pattern[sourceStep]) continue;

      const velocityLevel = lane.velocityPattern[sourceStep] ?? 1;
      const deterministicVelocity = [64, 88, 112][velocityLevel] ?? 88;
      const velocity = state.humanize.velocityRandomization > 0
        ? Math.max(1, Math.min(127, deterministicVelocity + Math.floor((Math.random() * 2 - 1) * 18)))
        : deterministicVelocity;

      const startTick = step * ticksPerStep;
      const durationFactor = 0.1 + (state.humanize.noteDuration / 100) * 0.9;
      const durationTicks = Math.max(1, Math.floor(ticksPerStep * durationFactor));
      const endTick = startTick + durationTicks;

      events.push({
        tick: startTick,
        channel,
        type: "noteOn",
        noteNumber: noteToMidiNumber(lane.note),
        velocity
      });
      events.push({
        tick: endTick,
        channel,
        type: "noteOff",
        noteNumber: noteToMidiNumber(lane.note),
        velocity: 0
      });
    }
  });

  // Resolve ties: noteOff should occur before noteOn at identical ticks.
  events.sort((a, b) => {
    if (a.tick !== b.tick) return a.tick - b.tick;
    const aOrder = a.meta ? 0 : (a.type === "noteOff" ? 1 : 2);
    const bOrder = b.meta ? 0 : (b.type === "noteOff" ? 1 : 2);
    return aOrder - bOrder;
  });
  return { events, ticksPerBeat };
}

function toMidiBlob(state) {
  const { events, ticksPerBeat } = createMidiEvents(state);
  const estimatedSize = 2048 + events.length * 12;
  const buffer = new ArrayBuffer(estimatedSize);
  const view = new DataView(buffer);
  const offset = { value: 0 };

  const writeString = (value) => {
    for (let i = 0; i < value.length; i += 1) view.setUint8(offset.value + i, value.charCodeAt(i));
    offset.value += value.length;
  };
  const writeInt16 = (value) => { view.setInt16(offset.value, value, false); offset.value += 2; };
  const writeInt32 = (value) => { view.setInt32(offset.value, value, false); offset.value += 4; };

  writeString("MThd");
  writeInt32(6);
  writeInt16(0);
  writeInt16(1);
  writeInt16(ticksPerBeat);

  writeString("MTrk");
  const trackLengthOffset = offset.value;
  writeInt32(0);

  let previousTick = 0;
  events.forEach((event) => {
    const delta = event.tick - previousTick;
    previousTick = event.tick;
    writeVarLength(view, offset, delta);
    if (event.meta) {
      view.setUint8(offset.value++, 0xff);
      view.setUint8(offset.value++, 0x51);
      view.setUint8(offset.value++, 0x03);
      view.setUint8(offset.value++, (event.microsecondsPerBeat >> 16) & 0xff);
      view.setUint8(offset.value++, (event.microsecondsPerBeat >> 8) & 0xff);
      view.setUint8(offset.value++, event.microsecondsPerBeat & 0xff);
      return;
    }
    const status = event.type === "noteOn" ? 0x90 : 0x80;
    view.setUint8(offset.value++, status | event.channel);
    view.setUint8(offset.value++, event.noteNumber & 0x7f);
    view.setUint8(offset.value++, event.velocity & 0x7f);
  });

  writeVarLength(view, offset, 0);
  view.setUint8(offset.value++, 0xff);
  view.setUint8(offset.value++, 0x2f);
  view.setUint8(offset.value++, 0x00);

  const trackLength = offset.value - trackLengthOffset - 4;
  view.setInt32(trackLengthOffset, trackLength, false);
  return new Blob([new Uint8Array(buffer.slice(0, offset.value))], { type: "audio/midi" });
}

export function exportMidi(state) {
  try {
    const blob = toMidiBlob(state);
    const url = URL.createObjectURL(blob);
    const anchor = document.createElement("a");
    anchor.href = url;
    anchor.download = "3beat-workstation.mid";
    document.body.appendChild(anchor);
    anchor.click();
    document.body.removeChild(anchor);
    URL.revokeObjectURL(url);
    return { success: true, message: "MIDI exported." };
  } catch (_error) {
    return { success: false, message: "MIDI export failed." };
  }
}
