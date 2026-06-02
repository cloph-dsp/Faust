const STORAGE_KEY = "three-beat-session-v2";
const HISTORY_LIMIT = 80;
const COLORS_NOISE = ["white", "pink", "brown", "grey", "red", "orange", "blue", "violet", "purple"];
const MAX_LANES = 12;

const defaultLanes = [
  { id: "lane1", name: "Lane 1", divisions: 4, note: "C3", sound: "sine", volume: 70 },
  { id: "lane2", name: "Lane 2", divisions: 3, note: "E4", sound: "triangle", volume: 70 },
  { id: "lane3", name: "Lane 3", divisions: 5, note: "G5", sound: "square", volume: 70 }
].map((lane) => ({
  ...lane,
  mute: false,
  solo: false,
  pattern: new Array(lane.divisions).fill(false),
  velocityPattern: new Array(lane.divisions).fill(1)
}));

const initialPresent = {
  transport: {
    isPlaying: false,
    bpm: 120,
    barMultiplier: 1,
    swing: 0,
    currentStep: 0
  },
  lanes: defaultLanes,
  editor: {
    activeLaneId: "lane1",
    activeStep: 0,
    selection: null,
    clipboard: null
  },
  humanize: {
    velocityRandomization: 0,
    noteDuration: 55
  },
  ui: {
    viewMode: "hybrid",
    advancedOpen: false,
    showShortcuts: false,
    notifications: [],
    mutateAmount: 35
  }
};

const initialState = {
  past: [],
  present: initialPresent,
  future: []
};

function deepClone(value) {
  return JSON.parse(JSON.stringify(value));
}

function clamp(value, min, max) {
  return Math.max(min, Math.min(max, value));
}

function normalizeLane(lane) {
  const divisions = clamp(parseInt(lane.divisions, 10) || 1, 1, 16);
  const pattern = Array.isArray(lane.pattern) ? lane.pattern.slice(0, divisions) : [];
  const velocityPattern = Array.isArray(lane.velocityPattern) ? lane.velocityPattern.slice(0, divisions) : [];
  while (pattern.length < divisions) pattern.push(false);
  while (velocityPattern.length < divisions) velocityPattern.push(1);
  return {
    id: lane.id,
    name: lane.name || lane.id,
    divisions,
    note: lane.note || "C4",
    sound: lane.sound || "sine",
    volume: clamp(parseInt(lane.volume, 10) || 0, 0, 100),
    mute: Boolean(lane.mute),
    solo: Boolean(lane.solo),
    pattern,
    velocityPattern: velocityPattern.map((v) => clamp(parseInt(v, 10) || 0, 0, 2))
  };
}

function normalizeSession(present) {
  const source = present && typeof present === "object" ? present : initialPresent;
  const lanes = Array.isArray(source.lanes) && source.lanes.length ? source.lanes.map(normalizeLane) : deepClone(defaultLanes);
  const requestedActiveLaneId = source.editor?.activeLaneId || lanes[0].id;
  const activeLaneExists = lanes.some((lane) => lane.id === requestedActiveLaneId);
  return {
    transport: {
      isPlaying: Boolean(source.transport?.isPlaying),
      bpm: clamp(parseInt(source.transport?.bpm, 10) || 120, 40, 300),
      barMultiplier: clamp(parseInt(source.transport?.barMultiplier, 10) || 1, 1, 8),
      swing: clamp(parseInt(source.transport?.swing, 10) || 0, 0, 100),
      currentStep: clamp(parseInt(source.transport?.currentStep, 10) || 0, 0, 255)
    },
    lanes,
    editor: {
      activeLaneId: activeLaneExists ? requestedActiveLaneId : lanes[0].id,
      activeStep: clamp(parseInt(source.editor?.activeStep, 10) || 0, 0, 255),
      selection: source.editor?.selection || null,
      clipboard: source.editor?.clipboard || null
    },
    humanize: {
      velocityRandomization: clamp(parseInt(source.humanize?.velocityRandomization, 10) || 0, 0, 100),
      noteDuration: clamp(parseInt(source.humanize?.noteDuration, 10) || 55, 0, 100)
    },
    ui: {
      viewMode: source.ui?.viewMode || "hybrid",
      advancedOpen: Boolean(source.ui?.advancedOpen),
      showShortcuts: Boolean(source.ui?.showShortcuts),
      notifications: Array.isArray(source.ui?.notifications) ? source.ui.notifications.slice(-5) : [],
      mutateAmount: clamp(parseInt(source.ui?.mutateAmount, 10) || 35, 0, 100)
    }
  };
}

function createLaneFromNumber(number) {
  const defaultNotes = ["C3", "E4", "G4", "A4", "D4", "F4"];
  const note = defaultNotes[(number - 1) % defaultNotes.length];
  return {
    id: `lane${number}`,
    name: `Lane ${number}`,
    divisions: 4,
    note,
    sound: "sine",
    volume: 70,
    mute: false,
    solo: false,
    pattern: new Array(4).fill(false),
    velocityPattern: new Array(4).fill(1)
  };
}

function pushHistory(state, nextPresent) {
  const past = state.past.concat([deepClone(state.present)]).slice(-HISTORY_LIMIT);
  return { past, present: nextPresent, future: [] };
}

function updateLaneCollection(present, laneId, updater) {
  let changed = false;
  const lanes = present.lanes.map((lane) => {
    if (lane.id !== laneId) return lane;
    const updated = updater(lane);
    if (updated !== lane) changed = true;
    return updated;
  });
  return changed ? { ...present, lanes } : present;
}

function reducer(state, action) {
  const type = action?.type;
  const payload = action?.payload || {};

  if (type === "HISTORY_UNDO") {
    if (!state.past.length) return state;
    const previous = state.past[state.past.length - 1];
    return {
      past: state.past.slice(0, -1),
      present: deepClone(previous),
      future: [deepClone(state.present), ...state.future].slice(0, HISTORY_LIMIT)
    };
  }

  if (type === "HISTORY_REDO") {
    if (!state.future.length) return state;
    const next = state.future[0];
    return {
      past: state.past.concat([deepClone(state.present)]).slice(-HISTORY_LIMIT),
      present: deepClone(next),
      future: state.future.slice(1)
    };
  }

  let nextPresent = state.present;

  switch (type) {
    case "TRANSPORT_SET":
      nextPresent = {
        ...nextPresent,
        transport: {
          ...nextPresent.transport,
          isPlaying: payload.isPlaying ?? nextPresent.transport.isPlaying,
          bpm: payload.bpm !== undefined ? clamp(parseInt(payload.bpm, 10) || 120, 40, 300) : nextPresent.transport.bpm,
          barMultiplier: payload.barMultiplier !== undefined ? clamp(parseInt(payload.barMultiplier, 10) || 1, 1, 8) : nextPresent.transport.barMultiplier,
          swing: payload.swing !== undefined ? clamp(parseInt(payload.swing, 10) || 0, 0, 100) : nextPresent.transport.swing,
          currentStep: payload.currentStep !== undefined ? clamp(parseInt(payload.currentStep, 10) || 0, 0, 255) : nextPresent.transport.currentStep
        }
      };
      break;
    case "UI_SET":
      nextPresent = {
        ...nextPresent,
        ui: { ...nextPresent.ui, ...payload }
      };
      break;
    case "EDITOR_SET":
      nextPresent = {
        ...nextPresent,
        editor: {
          ...nextPresent.editor,
          ...payload
        }
      };
      break;
    case "LANE_UPDATE": {
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const divisions = payload.changes?.divisions !== undefined
          ? clamp(parseInt(payload.changes.divisions, 10) || lane.divisions, 1, 16)
          : lane.divisions;
        const updated = { ...lane, ...payload.changes, divisions };
        const pattern = lane.pattern.slice(0, divisions);
        const velocityPattern = lane.velocityPattern.slice(0, divisions);
        while (pattern.length < divisions) pattern.push(false);
        while (velocityPattern.length < divisions) velocityPattern.push(1);
        updated.pattern = pattern;
        updated.velocityPattern = velocityPattern;
        updated.volume = clamp(parseInt(updated.volume, 10) || 0, 0, 100);
        return updated;
      });
      break;
    }
    case "LANE_ADD": {
      if (nextPresent.lanes.length >= MAX_LANES) break;
      const maxLaneNumber = nextPresent.lanes.reduce((max, lane) => {
        const n = parseInt(String(lane.id).replace("lane", ""), 10);
        return Number.isFinite(n) ? Math.max(max, n) : max;
      }, 0);
      const nextNumber = Math.max(maxLaneNumber + 1, nextPresent.lanes.length + 1);
      const newLane = createLaneFromNumber(nextNumber);
      nextPresent = {
        ...nextPresent,
        lanes: nextPresent.lanes.concat([newLane]),
        editor: {
          ...nextPresent.editor,
          activeLaneId: newLane.id,
          activeStep: 0
        }
      };
      break;
    }
    case "LANE_REMOVE": {
      if (nextPresent.lanes.length <= 1) break;
      const targetLaneId = payload.laneId;
      const laneExists = nextPresent.lanes.some((lane) => lane.id === targetLaneId);
      if (!laneExists) break;
      const lanes = nextPresent.lanes.filter((lane) => lane.id !== targetLaneId);
      const activeLaneId = nextPresent.editor.activeLaneId === targetLaneId
        ? lanes[0].id
        : nextPresent.editor.activeLaneId;
      nextPresent = {
        ...nextPresent,
        lanes,
        editor: {
          ...nextPresent.editor,
          activeLaneId,
          activeStep: 0
        }
      };
      break;
    }
    case "STEP_TOGGLE": {
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const idx = clamp(parseInt(payload.step, 10) || 0, 0, lane.divisions - 1);
        const pattern = lane.pattern.slice();
        const velocityPattern = lane.velocityPattern.slice();
        pattern[idx] = !pattern[idx];
        if (!pattern[idx]) velocityPattern[idx] = 1;
        return { ...lane, pattern, velocityPattern };
      });
      break;
    }
    case "STEP_SET": {
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const idx = clamp(parseInt(payload.step, 10) || 0, 0, lane.divisions - 1);
        const pattern = lane.pattern.slice();
        const velocityPattern = lane.velocityPattern.slice();
        if (payload.active !== undefined) pattern[idx] = Boolean(payload.active);
        if (payload.velocity !== undefined) velocityPattern[idx] = clamp(parseInt(payload.velocity, 10) || 0, 0, 2);
        if (!pattern[idx]) velocityPattern[idx] = 1;
        return { ...lane, pattern, velocityPattern };
      });
      break;
    }
    case "STEP_VELOCITY_CYCLE": {
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const idx = clamp(parseInt(payload.step, 10) || 0, 0, lane.divisions - 1);
        if (!lane.pattern[idx]) return lane;
        const velocityPattern = lane.velocityPattern.slice();
        velocityPattern[idx] = (velocityPattern[idx] + 1) % 3;
        return { ...lane, velocityPattern };
      });
      break;
    }
    case "LANE_COPY": {
      const lane = nextPresent.lanes.find((item) => item.id === payload.laneId);
      if (lane) {
        nextPresent = {
          ...nextPresent,
          editor: { ...nextPresent.editor, clipboard: deepClone(lane) }
        };
      }
      break;
    }
    case "LANE_PASTE": {
      const clipboard = nextPresent.editor.clipboard;
      if (!clipboard) break;
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const pattern = new Array(lane.divisions).fill(false);
        const velocityPattern = new Array(lane.divisions).fill(1);
        for (let i = 0; i < lane.divisions; i += 1) {
          pattern[i] = Boolean(clipboard.pattern[i % clipboard.pattern.length]);
          velocityPattern[i] = clamp(parseInt(clipboard.velocityPattern[i % clipboard.velocityPattern.length], 10) || 1, 0, 2);
        }
        return { ...lane, pattern, velocityPattern };
      });
      break;
    }
    case "PATTERN_CLEAR":
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => ({
        ...lane,
        pattern: new Array(lane.divisions).fill(false),
        velocityPattern: new Array(lane.divisions).fill(1)
      }));
      break;
    case "PATTERN_FILL":
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const every = clamp(parseInt(payload.every, 10) || 1, 1, lane.divisions);
        const offset = clamp(parseInt(payload.offset, 10) || 0, 0, lane.divisions - 1);
        const pattern = lane.pattern.map((_, i) => ((i - offset + lane.divisions) % every === 0));
        const velocityPattern = lane.velocityPattern.map((_, i) => (pattern[i] ? 1 : 1));
        return { ...lane, pattern, velocityPattern };
      });
      break;
    case "PATTERN_INVERT":
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => ({
        ...lane,
        pattern: lane.pattern.map((p) => !p),
        velocityPattern: lane.pattern.map((p, i) => (p ? 1 : lane.velocityPattern[i]))
      }));
      break;
    case "PATTERN_ROTATE":
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const right = payload.direction === "right";
        const pattern = lane.pattern.slice();
        const velocityPattern = lane.velocityPattern.slice();
        if (right) {
          pattern.unshift(pattern.pop());
          velocityPattern.unshift(velocityPattern.pop());
        } else {
          pattern.push(pattern.shift());
          velocityPattern.push(velocityPattern.shift());
        }
        return { ...lane, pattern, velocityPattern };
      });
      break;
    case "PATTERN_RANDOMIZE":
      nextPresent = updateLaneCollection(nextPresent, payload.laneId, (lane) => {
        const intensity = clamp(parseInt(payload.intensity, 10) || 0, 0, 100) / 100;
        const pattern = lane.pattern.slice();
        const velocityPattern = lane.velocityPattern.slice();
        for (let i = 0; i < lane.divisions; i += 1) {
          if (Math.random() <= intensity) {
            pattern[i] = !pattern[i];
          }
          if (pattern[i]) velocityPattern[i] = clamp(Math.floor(Math.random() * 3), 0, 2);
        }
        return { ...lane, pattern, velocityPattern };
      });
      break;
    case "PRESET_LOAD":
    case "SESSION_IMPORT":
      nextPresent = normalizeSession(payload.session);
      nextPresent.transport.isPlaying = false;
      break;
    case "PRESET_SAVE":
      break;
    default:
      return state;
  }

  if (nextPresent === state.present) return state;

  // Keep transient UI notifications small and short-lived in store snapshots.
  nextPresent = {
    ...nextPresent,
    ui: {
      ...nextPresent.ui,
      notifications: Array.isArray(nextPresent.ui.notifications) ? nextPresent.ui.notifications.slice(-5) : []
    }
  };
  const normalized = normalizeSession(nextPresent);
  if (action?.meta?.transient) {
    return {
      past: state.past,
      present: normalized,
      future: state.future
    };
  }
  return pushHistory(state, normalized);
}

function savePresent(present) {
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(present));
  } catch (_error) {
    // Local storage can fail in private mode; ignore hard failure.
  }
}

function loadPresent() {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) return deepClone(initialPresent);
    return normalizeSession(JSON.parse(raw));
  } catch (_error) {
    return deepClone(initialPresent);
  }
}

export function createStore() {
  let state = {
    past: [],
    present: loadPresent(),
    future: []
  };
  const listeners = new Set();

  function getState() {
    return deepClone(state.present);
  }

  function subscribe(listener) {
    listeners.add(listener);
    return () => listeners.delete(listener);
  }

  function dispatch(action) {
    const previous = state.present;
    const next = reducer(state, action);
    if (next === state) return;
    state = next;
    if (!action?.meta?.transient) {
      savePresent(state.present);
    }
    listeners.forEach((listener) => {
      listener(deepClone(previous), deepClone(state.present), action);
    });
  }

  return {
    getState,
    dispatch,
    subscribe,
    canUndo: () => state.past.length > 0,
    canRedo: () => state.future.length > 0
  };
}

export function getNoiseColors() {
  return COLORS_NOISE.slice();
}

export function getDefaultState() {
  return deepClone(initialPresent);
}
