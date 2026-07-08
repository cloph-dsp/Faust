import { getNoiseColors } from "./statemanager.js";

const NOISE_COLORS = getNoiseColors();
const OSC_TYPES = ["sine", "triangle", "square", "sawtooth", "fatsine", "fattriangle", "fatsquare", "fatsawtooth", "amsine"];
const NOISE_FILTER_CONFIG = {
  violet: { type: "highpass", frequency: 2000, rolloff: -12 },
  blue: { type: "highpass", frequency: 500, rolloff: -12 },
  red: { type: "lowpass", frequency: 200, rolloff: -12 },
  grey: { type: "bandpass", frequency: 1000, rolloff: -12 },
  purple: { type: "highpass", frequency: 4000, rolloff: -12 },
  orange: { type: "lowpass", frequency: 5000, rolloff: -12 }
};

let storeRef = null;
let laneEngines = [];
let stepListener = null;
let isReady = false;

function isNoise(sound) {
  return NOISE_COLORS.includes(sound);
}

function isOscillator(sound) {
  return OSC_TYPES.includes(sound);
}

function createSource(sound, gainNode) {
  if (isNoise(sound)) {
    if (NOISE_FILTER_CONFIG[sound]) {
      const source = new Tone.Noise("white");
      const filter = new Tone.Filter(NOISE_FILTER_CONFIG[sound]);
      source.chain(filter, gainNode);
      source._customFilter = filter;
      return source;
    }
    return new Tone.Noise(sound).connect(gainNode);
  }

  const type = isOscillator(sound) ? sound : "sine";
  return new Tone.Synth({
    oscillator: { type },
    envelope: { attack: 0.001, decay: 0.05, sustain: 0.2, release: 0.08 }
  }).connect(gainNode);
}

function buildLaneEngine(lane) {
  const gainNode = new Tone.Gain(lane.volume / 100).toDestination();
  const analyser = new Tone.Analyser("fft", 512);
  gainNode.connect(analyser);
  return {
    id: lane.id,
    sound: lane.sound,
    gainNode,
    analyser,
    source: createSource(lane.sound, gainNode),
    sequence: null,
    currentStep: 0
  };
}

function disposeLaneEngine(engine) {
  if (!engine) return;
  if (engine.sequence) {
    engine.sequence.dispose();
  }
  if (engine.source) {
    try {
      if (engine.source.stop && engine.source.state === "started") engine.source.stop();
    } catch (_error) {}
    engine.source.dispose();
  }
  if (engine.source?._customFilter) {
    engine.source._customFilter.dispose();
  }
  if (engine.gainNode) engine.gainNode.dispose();
  if (engine.analyser) engine.analyser.dispose();
}

function getLaneAudibility(state, laneId) {
  const hasSolo = state.lanes.some((lane) => lane.solo);
  const lane = state.lanes.find((item) => item.id === laneId);
  if (!lane) return false;
  if (hasSolo) return lane.solo && !lane.mute;
  return !lane.mute;
}

function mapVelocity(level, randomizePercent) {
  const base = [0.55, 0.78, 1.0][level] ?? 0.78;
  if (!randomizePercent) return base;
  const amount = (randomizePercent / 100) * 0.35;
  const delta = (Math.random() * 2 - 1) * amount;
  return Math.max(0.05, Math.min(1, base + delta));
}

function getStepDurationSeconds(bpm, divisions) {
  return (60 / bpm) * (4 / divisions);
}

function createSequenceForLane(lane, laneIndex) {
  const state = storeRef.getState();
  const totalSteps = lane.divisions * state.transport.barMultiplier;
  const stepDurationSeconds = getStepDurationSeconds(state.transport.bpm, lane.divisions);

  const sequence = new Tone.Sequence((time, sequenceStep) => {
    const liveState = storeRef.getState();
    const liveLane = liveState.lanes[laneIndex];
    if (!liveLane) return;
    const step = sequenceStep % liveLane.divisions;
    const active = liveLane.pattern[step];
    const engine = laneEngines[laneIndex];
    if (!engine) return;
    engine.currentStep = step;

    Tone.Draw.schedule(() => {
      if (stepListener) {
        stepListener({
          laneId: liveLane.id,
          step,
          totalSteps,
          transportStep: sequenceStep
        });
      }
    }, time);

    if (!active) return;
    if (!getLaneAudibility(liveState, liveLane.id)) return;

    const velocityLevel = liveLane.velocityPattern[step] ?? 1;
    const velocity = mapVelocity(velocityLevel, liveState.humanize.velocityRandomization);
    const durationFactor = 0.1 + (liveState.humanize.noteDuration / 100) * 0.9;
    const duration = Math.max(0.01, stepDurationSeconds * durationFactor);

    if (isNoise(liveLane.sound)) {
      try {
        engine.source.start(time).stop(time + duration);
      } catch (_error) {
        // Noise can throw when overlapping starts happen on rapid edits.
      }
      return;
    }

    try {
      engine.source.triggerAttackRelease(liveLane.note, duration, time, velocity);
    } catch (_error) {
      // Ignore occasional Tone scheduling race from hot edits.
    }
  }, Array.from({ length: totalSteps }, (_, i) => i), stepDurationSeconds);

  return sequence;
}

function rebuildLane(index) {
  const state = storeRef.getState();
  const lane = state.lanes[index];
  const existing = laneEngines[index];
  if (!lane || !existing) return;

  if (existing.sequence) {
    existing.sequence.dispose();
    existing.sequence = null;
  }

  const needsNewSource = existing.sound !== lane.sound;
  if (needsNewSource) {
    if (existing.source) {
      try {
        if (existing.source.stop && existing.source.state === "started") existing.source.stop();
      } catch (_error) {}
      if (existing.source._customFilter) {
        existing.source._customFilter.dispose();
      }
      existing.source.dispose();
    }
    existing.source = createSource(lane.sound, existing.gainNode);
    existing.sound = lane.sound;
  }

  existing.gainNode.gain.value = lane.volume / 100;
  existing.sequence = createSequenceForLane(lane, index);

  if (state.transport.isPlaying && existing.sequence) {
    existing.sequence.start(0);
  }
}

function rebuildAllLanes() {
  const state = storeRef.getState();
  state.lanes.forEach((_lane, index) => rebuildLane(index));
}

function haveLaneStructureChanged(prev, next) {
  if (prev.lanes.length !== next.lanes.length) return true;
  for (let i = 0; i < next.lanes.length; i += 1) {
    if (prev.lanes[i].id !== next.lanes[i].id) return true;
  }
  return false;
}

function rebuildAudioGraph(nextState) {
  const shouldResume = nextState.transport.isPlaying;
  Tone.Transport.stop();
  Tone.Transport.cancel(0);
  laneEngines.forEach(disposeLaneEngine);
  laneEngines = nextState.lanes.map((lane) => buildLaneEngine(lane));
  updateTransport(nextState);
  rebuildAllLanes();
  if (shouldResume) {
    laneEngines.forEach((engine) => {
      if (engine.sequence) engine.sequence.start(0);
    });
    Tone.Transport.start("+0.01");
  }
}

function updateTransport(state) {
  Tone.Transport.bpm.rampTo(state.transport.bpm, 0.05);
  Tone.Transport.swing = state.transport.swing / 100;
  Tone.Transport.loopStart = 0;
  Tone.Transport.loop = true;
  Tone.Transport.loopEnd = `${state.transport.barMultiplier}m`;
}

export function initAudio(store) {
  storeRef = store;
  if (isReady) return getAnalyzerNodes();
  Tone.Transport.stop();
  Tone.Transport.cancel(0);
  Tone.Transport.lookahead = 0.03;
  Tone.Transport.timeSignature = 4;

  const state = storeRef.getState();
  laneEngines = state.lanes.map((lane) => buildLaneEngine(lane));

  updateTransport(state);
  rebuildAllLanes();
  isReady = true;
  return getAnalyzerNodes();
}

export async function startTransport() {
  if (!storeRef || !isReady) return;
  await Tone.start();
  const state = storeRef.getState();
  updateTransport(state);
  rebuildAllLanes();
  Tone.Transport.start("+0.01");
}

export function stopTransport() {
  Tone.Transport.stop();
  Tone.Transport.position = 0;
}

export function panic() {
  Tone.Transport.stop();
  Tone.Transport.cancel(0);
  laneEngines.forEach((engine) => {
    if (!engine?.source) return;
    try {
      if (engine.source.stop && engine.source.state === "started") engine.source.stop();
    } catch (_error) {}
  });
  if (storeRef) {
    rebuildAllLanes();
  }
}

export function syncFromState(prev, next) {
  if (!isReady) return;

  if (prev.transport.isPlaying !== next.transport.isPlaying) {
    if (next.transport.isPlaying) {
      startTransport();
    } else {
      stopTransport();
    }
    return;
  }

  if (haveLaneStructureChanged(prev, next)) {
    rebuildAudioGraph(next);
    return;
  }

  if (
    prev.transport.bpm !== next.transport.bpm ||
    prev.transport.swing !== next.transport.swing
  ) {
    updateTransport(next);
  }

  if (prev.transport.barMultiplier !== next.transport.barMultiplier) {
    updateTransport(next);
    rebuildAllLanes();
    return;
  }

  next.lanes.forEach((lane, index) => {
    const previousLane = prev.lanes[index];
    if (!previousLane) return rebuildLane(index);

    if (
      previousLane.sound !== lane.sound ||
      previousLane.divisions !== lane.divisions ||
      JSON.stringify(previousLane.pattern) !== JSON.stringify(lane.pattern) ||
      JSON.stringify(previousLane.velocityPattern) !== JSON.stringify(lane.velocityPattern) ||
      previousLane.note !== lane.note
    ) {
      rebuildLane(index);
    }

    if (previousLane.volume !== lane.volume) {
      laneEngines[index].gainNode.gain.value = lane.volume / 100;
    }
  });
}

export function destroyAudio() {
  Tone.Transport.stop();
  Tone.Transport.cancel(0);
  laneEngines.forEach(disposeLaneEngine);
  laneEngines = [];
  isReady = false;
}

export function getAnalyzerNodes() {
  return laneEngines.map((engine) => engine.analyser);
}

export function setStepListener(listener) {
  stepListener = listener;
}
