import { exportMidi } from "./midiexport.js";

const NOTES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
const OCTAVES = [2, 3, 4, 5, 6];
const OSC_TYPES = ["sine", "triangle", "square", "sawtooth", "fatsine", "fattriangle", "fatsquare", "fatsawtooth", "amsine"];
const NOISE_COLORS = ["white", "pink", "brown", "grey", "red", "orange", "blue", "violet", "purple"];
const PRESET_KEY = "three-beat-presets-v1";
const PRESET_LS_KEY = "three-beat-presets-local-v1";

let storeRef;
let analyzersRef = [];
let transportApi;
let cachedPresetNames = [];
let rafId = null;
let presetAdapter = null;
let audioEnabledRef = true;
let livePlayheads = {};

const dom = {};

async function initPresetAdapter() {
  try {
    const module = await import("https://cdn.jsdelivr.net/npm/idb-keyval@6/+esm");
    return {
      get: async (key) => module.get(key),
      set: async (key, value) => module.set(key, value)
    };
  } catch (_error) {
    return {
      get: async (key) => {
        try {
          const raw = localStorage.getItem(`${PRESET_LS_KEY}:${key}`);
          return raw ? JSON.parse(raw) : null;
        } catch (_e) {
          return null;
        }
      },
      set: async (key, value) => {
        try {
          localStorage.setItem(`${PRESET_LS_KEY}:${key}`, JSON.stringify(value));
        } catch (_e) {
          // ignore
        }
      }
    };
  }
}

function getState() {
  return storeRef.getState();
}

function toast(message, type = "info") {
  const node = document.createElement("div");
  node.className = `toast ${type}`;
  node.textContent = message;
  dom.toastHost.appendChild(node);
  requestAnimationFrame(() => node.classList.add("show"));
  setTimeout(() => {
    node.classList.remove("show");
    setTimeout(() => node.remove(), 200);
  }, 1800);
}

function buildNoteOptions(selected) {
  return OCTAVES.flatMap((octave) => NOTES.map((note) => `${note}${octave}`))
    .map((note) => `<option value="${note}" ${selected === note ? "selected" : ""}>${note}</option>`)
    .join("");
}

function buildSoundOptions(selected) {
  const oscOptions = OSC_TYPES.map((sound) => `<option value="${sound}" ${selected === sound ? "selected" : ""}>${sound}</option>`).join("");
  const noiseOptions = NOISE_COLORS.map((sound) => `<option value="${sound}" ${selected === sound ? "selected" : ""}>${sound}</option>`).join("");
  return `<optgroup label="Osc">${oscOptions}</optgroup><optgroup label="Noise">${noiseOptions}</optgroup>`;
}

function dotPos(step, divisions, radiusPercent = 40) {
  const angle = (step / divisions) * Math.PI * 2 - Math.PI / 2;
  return {
    x: 50 + radiusPercent * Math.cos(angle),
    y: 50 + radiusPercent * Math.sin(angle)
  };
}

function accentName(level) {
  if (level === 2) return "Hard";
  if (level === 1) return "Mid";
  return "Soft";
}

function laneToolbar(lane, isActiveLane) {
  return `
    <div class="lane-toolbar">
      <button class="lane-name ${isActiveLane ? "active" : ""}" data-action="select-lane" data-lane="${lane.id}">${lane.name}</button>
      <div class="lane-tools">
        <button data-action="lane-mute" data-lane="${lane.id}" aria-pressed="${lane.mute}">M</button>
        <button data-action="lane-solo" data-lane="${lane.id}" aria-pressed="${lane.solo}">S</button>
        <button data-action="lane-copy" data-lane="${lane.id}">Copy</button>
        <button data-action="lane-paste" data-lane="${lane.id}">Paste</button>
        <button data-action="lane-clear" data-lane="${lane.id}">Clear</button>
        <button data-action="lane-remove" data-lane="${lane.id}">Remove</button>
      </div>
    </div>
  `;
}

function laneAdvancedTools(lane) {
  return `
    <div class="lane-workflow" role="group" aria-label="${lane.name} workflow">
      <button data-action="lane-fill" data-value="2" data-lane="${lane.id}">Fill 2</button>
      <button data-action="lane-fill" data-value="3" data-lane="${lane.id}">Fill 3</button>
      <button data-action="lane-fill" data-value="4" data-lane="${lane.id}">Fill 4</button>
      <button data-action="lane-rotate" data-value="left" data-lane="${lane.id}">Rotate L</button>
      <button data-action="lane-rotate" data-value="right" data-lane="${lane.id}">Rotate R</button>
      <button data-action="lane-invert" data-lane="${lane.id}">Invert</button>
    </div>
  `;
}

function renderLanes(state) {
  dom.laneContainer.innerHTML = state.lanes.map((lane, laneIndex) => {
    const isActiveLane = state.editor.activeLaneId === lane.id;
    const totalSteps = lane.divisions * state.transport.barMultiplier;
    const activeStep = state.transport.currentStep % lane.divisions;
    return `
      <article class="lane-card ${isActiveLane ? "selected" : ""}" data-lane-card="${lane.id}">
        ${laneToolbar(lane, isActiveLane)}
        <div class="lane-controls">
          <label>Sound
            <select data-action="lane-sound" data-lane="${lane.id}">
              ${buildSoundOptions(lane.sound)}
            </select>
          </label>
          <label>Note
            <select data-action="lane-note" data-lane="${lane.id}" ${NOISE_COLORS.includes(lane.sound) ? "disabled" : ""}>
              ${buildNoteOptions(lane.note)}
            </select>
          </label>
          <label>Div
            <input data-action="lane-divisions" data-lane="${lane.id}" type="number" min="1" max="16" value="${lane.divisions}">
          </label>
          <label>Vol
            <input data-action="lane-volume" data-lane="${lane.id}" type="range" min="0" max="100" value="${lane.volume}">
          </label>
        </div>
        ${laneAdvancedTools(lane)}
        <div class="lane-visuals">
          <div class="circle-wrap" data-lane-circle="${lane.id}">
            <div class="circle-ring ring-1"></div>
            <div class="circle-ring ring-2"></div>
            <div class="circle-ring ring-3"></div>
            <canvas id="analyser-${laneIndex}" class="analyser-canvas" width="220" height="220"></canvas>
            ${Array.from({ length: lane.divisions }, (_, step) => {
              const pos = dotPos(step, lane.divisions);
              const active = lane.pattern[step];
              const accent = lane.velocityPattern[step] ?? 1;
              return `
                <button
                  class="circle-step ${active ? "on" : ""} ${step === activeStep && state.transport.isPlaying ? "playhead" : ""} accent-${accent}"
                  data-action="step-toggle"
                  data-lane="${lane.id}"
                  data-step="${step}"
                  style="left:${pos.x}%;top:${pos.y}%"
                  title="Step ${step + 1} | ${active ? "On" : "Off"} | ${accentName(accent)} accent"
                  aria-label="Lane ${lane.name} step ${step + 1}"
                ></button>
              `;
            }).join("")}
          </div>
          <div class="grid-wrap" role="grid" aria-label="${lane.name} step grid">
            ${Array.from({ length: totalSteps }, (_, step) => {
              const sourceStep = step % lane.divisions;
              const active = lane.pattern[sourceStep];
              const accent = lane.velocityPattern[sourceStep] ?? 1;
              const isPlayhead = state.transport.isPlaying && sourceStep === activeStep;
              return `
                <button
                  role="gridcell"
                  class="grid-step ${active ? "on" : ""} ${isPlayhead ? "playhead" : ""} accent-${accent}"
                  data-action="step-toggle"
                  data-lane="${lane.id}"
                  data-step="${sourceStep}"
                  data-transport-step="${step}"
                  aria-label="Step ${step + 1}"
                >
                  <span>${step + 1}</span>
                </button>
              `;
            }).join("")}
          </div>
        </div>
      </article>
    `;
  }).join("");
}

function updateTopControls(state) {
  const activeLaneStep = livePlayheads[state.editor.activeLaneId];
  const shownStep = Number.isInteger(activeLaneStep)
    ? activeLaneStep + 1
    : (state.transport.currentStep + 1);
  dom.playToggle.textContent = state.transport.isPlaying ? "Stop" : "Play";
  dom.playToggle.setAttribute("aria-pressed", String(state.transport.isPlaying));
  dom.bpmInput.value = state.transport.bpm;
  dom.barsInput.value = state.transport.barMultiplier;
  dom.swingInput.value = state.transport.swing;
  dom.swingValue.textContent = `${state.transport.swing}%`;
  dom.mutateAmount.value = state.ui.mutateAmount;
  dom.statusPanel.textContent = `step:${shownStep} | loop:${state.transport.barMultiplier} bars | active lanes:${state.lanes.filter((l) => !l.mute).length} | cpu-safe:${window.matchMedia("(prefers-reduced-motion: reduce)").matches ? "on" : "off"}`;
  dom.undoButton.disabled = !storeRef.canUndo();
  dom.redoButton.disabled = !storeRef.canRedo();
}

function renderAll(state) {
  updateTopControls(state);
  renderLanes(state);
  bindLaneEvents();
}

function updatePlayheadClasses(state) {
  state.lanes.forEach((lane) => {
    const current = state.transport.currentStep % lane.divisions;
    dom.laneContainer.querySelectorAll(`.circle-step[data-lane="${lane.id}"]`).forEach((node) => {
      const step = parseInt(node.dataset.step, 10);
      node.classList.toggle("playhead", state.transport.isPlaying && step === current);
    });
    dom.laneContainer.querySelectorAll(`.grid-step[data-lane="${lane.id}"]`).forEach((node) => {
      const sourceStep = parseInt(node.dataset.step, 10);
      node.classList.toggle("playhead", state.transport.isPlaying && sourceStep === current);
    });
  });
}

function updateSingleLanePlayhead(laneId, step) {
  livePlayheads[laneId] = step;
  dom.laneContainer.querySelectorAll(`.circle-step[data-lane="${laneId}"]`).forEach((node) => {
    const nodeStep = parseInt(node.dataset.step, 10);
    node.classList.toggle("playhead", nodeStep === step);
  });
  dom.laneContainer.querySelectorAll(`.grid-step[data-lane="${laneId}"]`).forEach((node) => {
    const nodeStep = parseInt(node.dataset.step, 10);
    node.classList.toggle("playhead", nodeStep === step);
  });
}

function laneById(laneId) {
  return getState().lanes.find((lane) => lane.id === laneId);
}

function bindLaneEvents() {
  dom.laneContainer.querySelectorAll("[data-action]").forEach((node) => {
    node.addEventListener("click", async (event) => {
      const action = event.currentTarget.dataset.action;
      const laneId = event.currentTarget.dataset.lane;
      const lane = laneById(laneId);
      if (!lane && action.startsWith("lane")) return;

      switch (action) {
        case "select-lane":
          storeRef.dispatch({ type: "EDITOR_SET", payload: { activeLaneId: laneId }, meta: { transient: true } });
          return;
        case "lane-mute":
          storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId, changes: { mute: !lane.mute } } });
          return;
        case "lane-solo":
          storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId, changes: { solo: !lane.solo } } });
          return;
        case "lane-copy":
          storeRef.dispatch({ type: "LANE_COPY", payload: { laneId } });
          toast(`${lane.name} copied.`, "success");
          return;
        case "lane-paste":
          storeRef.dispatch({ type: "LANE_PASTE", payload: { laneId } });
          return;
        case "lane-clear":
          storeRef.dispatch({ type: "PATTERN_CLEAR", payload: { laneId } });
          return;
        case "lane-remove":
          storeRef.dispatch({ type: "LANE_REMOVE", payload: { laneId } });
          return;
        case "lane-fill":
          storeRef.dispatch({ type: "PATTERN_FILL", payload: { laneId, every: parseInt(event.currentTarget.dataset.value, 10) } });
          return;
        case "lane-rotate":
          storeRef.dispatch({ type: "PATTERN_ROTATE", payload: { laneId, direction: event.currentTarget.dataset.value } });
          return;
        case "lane-invert":
          storeRef.dispatch({ type: "PATTERN_INVERT", payload: { laneId } });
          return;
        case "step-toggle":
          storeRef.dispatch({ type: "STEP_TOGGLE", payload: { laneId, step: parseInt(event.currentTarget.dataset.step, 10) } });
          return;
        default:
      }
    });

    if (node.dataset.action === "step-toggle") {
      node.addEventListener("contextmenu", (event) => {
        event.preventDefault();
        storeRef.dispatch({
          type: "STEP_VELOCITY_CYCLE",
          payload: {
            laneId: event.currentTarget.dataset.lane,
            step: parseInt(event.currentTarget.dataset.step, 10)
          }
        });
      });
    }
  });

  dom.laneContainer.querySelectorAll('select[data-action="lane-sound"]').forEach((input) => {
    input.addEventListener("change", (event) => {
      storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId: event.target.dataset.lane, changes: { sound: event.target.value } } });
    });
  });
  dom.laneContainer.querySelectorAll('select[data-action="lane-note"]').forEach((input) => {
    input.addEventListener("change", (event) => {
      storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId: event.target.dataset.lane, changes: { note: event.target.value } } });
    });
  });
  dom.laneContainer.querySelectorAll('input[data-action="lane-divisions"]').forEach((input) => {
    input.addEventListener("change", (event) => {
      storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId: event.target.dataset.lane, changes: { divisions: parseInt(event.target.value, 10) } } });
    });
  });
  dom.laneContainer.querySelectorAll('input[data-action="lane-volume"]').forEach((input) => {
    input.addEventListener("input", (event) => {
      storeRef.dispatch({ type: "LANE_UPDATE", payload: { laneId: event.target.dataset.lane, changes: { volume: parseInt(event.target.value, 10) } } });
    });
  });
}

async function getPresetMap() {
  if (!presetAdapter) presetAdapter = await initPresetAdapter();
  return (await presetAdapter.get(PRESET_KEY)) || {};
}

async function refreshPresetSelect() {
  const map = await getPresetMap();
  cachedPresetNames = Object.keys(map).sort((a, b) => a.localeCompare(b));
  dom.presetSelect.innerHTML = cachedPresetNames.length
    ? cachedPresetNames.map((name) => `<option value="${name}">${name}</option>`).join("")
    : '<option value="">No presets</option>';
}

async function savePresetByName(name, session) {
  const trimmed = name.trim();
  if (!trimmed) {
    toast("Preset name required.", "error");
    return;
  }
  const map = await getPresetMap();
  map[trimmed] = session;
  await presetAdapter.set(PRESET_KEY, map);
  await refreshPresetSelect();
  dom.presetSelect.value = trimmed;
  toast(`Preset "${trimmed}" saved.`, "success");
}

async function loadSelectedPreset() {
  const key = dom.presetSelect.value;
  if (!key) return;
  const map = await getPresetMap();
  const session = map[key];
  if (!session) return;
  storeRef.dispatch({ type: "PRESET_LOAD", payload: { session } });
  toast(`Preset "${key}" loaded.`, "success");
}

async function deleteSelectedPreset() {
  const key = dom.presetSelect.value;
  if (!key) return;
  const map = await getPresetMap();
  if (!map[key]) return;
  delete map[key];
  await presetAdapter.set(PRESET_KEY, map);
  await refreshPresetSelect();
  toast(`Preset "${key}" deleted.`, "success");
}

function exportSessionJson() {
  const blob = new Blob([JSON.stringify(getState(), null, 2)], { type: "application/json" });
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = "3beat-session.json";
  document.body.appendChild(anchor);
  anchor.click();
  document.body.removeChild(anchor);
  URL.revokeObjectURL(url);
}

function importSessionFile(file) {
  if (!file) return;
  const reader = new FileReader();
  reader.onload = () => {
    try {
      const data = JSON.parse(String(reader.result || "{}"));
      if (!data.transport || !Array.isArray(data.lanes)) {
        toast("Invalid session format.", "error");
        return;
      }
      storeRef.dispatch({ type: "SESSION_IMPORT", payload: { session: data } });
      toast("Session imported.", "success");
    } catch (_error) {
      toast("Failed to parse session JSON.", "error");
    }
  };
  reader.readAsText(file);
}

function runMutateOnActiveLane() {
  const state = getState();
  storeRef.dispatch({
    type: "PATTERN_RANDOMIZE",
    payload: { laneId: state.editor.activeLaneId, intensity: state.ui.mutateAmount }
  });
}

function registerTopLevelEvents() {
  dom.playToggle.addEventListener("click", async () => {
    if (!audioEnabledRef) {
      toast("Audio engine unavailable. Open via a local web server and ensure CDN access.", "error");
      return;
    }
    const state = getState();
    if (state.transport.isPlaying) {
      storeRef.dispatch({ type: "TRANSPORT_SET", payload: { isPlaying: false, currentStep: 0 } });
      return;
    }
    storeRef.dispatch({ type: "TRANSPORT_SET", payload: { isPlaying: true } });
  });

  dom.panicButton.addEventListener("click", () => {
    transportApi.panic();
    storeRef.dispatch({ type: "TRANSPORT_SET", payload: { isPlaying: false, currentStep: 0 } });
    toast("All voices stopped.", "info");
  });

  dom.bpmInput.addEventListener("input", (event) => {
    storeRef.dispatch({ type: "TRANSPORT_SET", payload: { bpm: parseInt(event.target.value, 10) } });
  });
  dom.barsInput.addEventListener("change", (event) => {
    storeRef.dispatch({ type: "TRANSPORT_SET", payload: { barMultiplier: parseInt(event.target.value, 10), currentStep: 0 } });
  });
  dom.swingInput.addEventListener("input", (event) => {
    storeRef.dispatch({ type: "TRANSPORT_SET", payload: { swing: parseInt(event.target.value, 10) } });
  });
  dom.mutateAmount.addEventListener("input", (event) => {
    storeRef.dispatch({ type: "UI_SET", payload: { mutateAmount: parseInt(event.target.value, 10) }, meta: { transient: true } });
  });

  dom.mutateActiveLane.addEventListener("click", runMutateOnActiveLane);
  dom.addLane.addEventListener("click", () => {
    storeRef.dispatch({ type: "LANE_ADD", payload: {} });
  });
  dom.exportMidi.addEventListener("click", () => {
    const result = exportMidi(getState());
    toast(result.message, result.success ? "success" : "error");
  });

  dom.undoButton.addEventListener("click", () => storeRef.dispatch({ type: "HISTORY_UNDO" }));
  dom.redoButton.addEventListener("click", () => storeRef.dispatch({ type: "HISTORY_REDO" }));

  dom.shortcutButton.addEventListener("click", () => {
    const next = !dom.shortcutPanel.classList.contains("hidden");
    dom.shortcutPanel.classList.toggle("hidden");
    dom.shortcutPanel.setAttribute("aria-hidden", String(next));
  });

  dom.savePreset.addEventListener("click", () => savePresetByName(dom.presetName.value, getState()));
  dom.loadPreset.addEventListener("click", loadSelectedPreset);
  dom.deletePreset.addEventListener("click", deleteSelectedPreset);
  dom.exportSession.addEventListener("click", exportSessionJson);
  dom.importSession.addEventListener("change", (event) => {
    importSessionFile(event.target.files?.[0]);
    event.target.value = "";
  });
}

function setActiveLaneByNumber(index) {
  const state = getState();
  const lane = state.lanes[index];
  if (!lane) return;
  storeRef.dispatch({ type: "EDITOR_SET", payload: { activeLaneId: lane.id }, meta: { transient: true } });
}

function toggleCurrentEditorStep() {
  const state = getState();
  const lane = state.lanes.find((item) => item.id === state.editor.activeLaneId);
  if (!lane) return;
  const step = state.editor.activeStep % lane.divisions;
  storeRef.dispatch({ type: "STEP_TOGGLE", payload: { laneId: lane.id, step } });
}

function cycleCurrentEditorAccent() {
  const state = getState();
  const lane = state.lanes.find((item) => item.id === state.editor.activeLaneId);
  if (!lane) return;
  const step = state.editor.activeStep % lane.divisions;
  storeRef.dispatch({ type: "STEP_VELOCITY_CYCLE", payload: { laneId: lane.id, step } });
}

function moveEditorStep(delta) {
  const state = getState();
  const lane = state.lanes.find((item) => item.id === state.editor.activeLaneId);
  if (!lane) return;
  const nextStep = (state.editor.activeStep + delta + lane.divisions) % lane.divisions;
  storeRef.dispatch({ type: "EDITOR_SET", payload: { activeStep: nextStep }, meta: { transient: true } });
}

function registerHotkeys() {
  if (!window.hotkeys) return;
  hotkeys.filter = (event) => {
    const tag = event.target?.tagName?.toLowerCase();
    return !["input", "textarea", "select"].includes(tag);
  };
  hotkeys("space", async (event) => {
    event.preventDefault();
    dom.playToggle.click();
  });
  hotkeys("ctrl+z,command+z", (event) => {
    event.preventDefault();
    storeRef.dispatch({ type: "HISTORY_UNDO" });
  });
  hotkeys("ctrl+shift+z,command+shift+z", (event) => {
    event.preventDefault();
    storeRef.dispatch({ type: "HISTORY_REDO" });
  });
  hotkeys("r", (event) => {
    event.preventDefault();
    runMutateOnActiveLane();
  });
  hotkeys("c", (event) => {
    event.preventDefault();
    storeRef.dispatch({ type: "LANE_COPY", payload: { laneId: getState().editor.activeLaneId } });
  });
  hotkeys("v", (event) => {
    event.preventDefault();
    storeRef.dispatch({ type: "LANE_PASTE", payload: { laneId: getState().editor.activeLaneId } });
  });
  hotkeys("?", (event) => {
    event.preventDefault();
    dom.shortcutButton.click();
  });
  hotkeys("1,2,3,4,5,6,7,8,9", (event) => {
    event.preventDefault();
    const idx = parseInt(event.key, 10) - 1;
    setActiveLaneByNumber(idx);
  });
  hotkeys("left", (event) => { event.preventDefault(); moveEditorStep(-1); });
  hotkeys("right", (event) => { event.preventDefault(); moveEditorStep(1); });
  hotkeys("enter", (event) => { event.preventDefault(); toggleCurrentEditorStep(); });
  hotkeys("a", (event) => { event.preventDefault(); cycleCurrentEditorAccent(); });
}

function visualFrame() {
  const state = getState();
  if (state.transport.isPlaying) {
    analyzersRef.forEach((analyzer, idx) => {
      const canvas = document.getElementById(`analyser-${idx}`);
      if (!canvas || !analyzer) return;
      const ctx = canvas.getContext("2d");
      if (!ctx) return;
      let values = null;
      try {
        values = analyzer.getValue();
      } catch (_error) {
        return;
      }
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.save();
      ctx.beginPath();
      ctx.arc(canvas.width / 2, canvas.height / 2, canvas.width / 2 - 8, 0, Math.PI * 2);
      ctx.clip();
      const width = canvas.width / values.length;
      for (let i = 0; i < values.length; i += 1) {
        const normalized = Math.max(0, (values[i] + 120) / 120);
        const bar = normalized * canvas.height * 0.75;
        const hue = 190 - normalized * 130;
        ctx.fillStyle = `hsl(${hue}, 90%, 58%)`;
        ctx.fillRect(i * width, canvas.height - bar, width * 0.8, bar);
      }
      ctx.restore();
    });
  }
  rafId = requestAnimationFrame(visualFrame);
}

function cacheDom() {
  dom.playToggle = document.getElementById("playToggle");
  dom.panicButton = document.getElementById("panicButton");
  dom.bpmInput = document.getElementById("bpmInput");
  dom.barsInput = document.getElementById("barsInput");
  dom.swingInput = document.getElementById("swingInput");
  dom.swingValue = document.getElementById("swingValue");
  dom.statusPanel = document.getElementById("statusPanel");
  dom.laneContainer = document.getElementById("laneContainer");
  dom.addLane = document.getElementById("addLane");
  dom.mutateAmount = document.getElementById("mutateAmount");
  dom.mutateActiveLane = document.getElementById("mutateActiveLane");
  dom.exportMidi = document.getElementById("exportMidi");
  dom.undoButton = document.getElementById("undoButton");
  dom.redoButton = document.getElementById("redoButton");
  dom.shortcutButton = document.getElementById("shortcutButton");
  dom.shortcutPanel = document.getElementById("shortcutPanel");
  dom.presetName = document.getElementById("presetName");
  dom.savePreset = document.getElementById("savePreset");
  dom.presetSelect = document.getElementById("presetSelect");
  dom.loadPreset = document.getElementById("loadPreset");
  dom.deletePreset = document.getElementById("deletePreset");
  dom.exportSession = document.getElementById("exportSession");
  dom.importSession = document.getElementById("importSession");
  dom.toastHost = document.getElementById("toastHost");
}

export function initUI({ store, analyzers, transport, setStepListener, audioEnabled = true }) {
  storeRef = store;
  analyzersRef = analyzers || [];
  transportApi = transport;
  audioEnabledRef = audioEnabled;
  cacheDom();

  renderAll(getState());
  registerTopLevelEvents();
  registerHotkeys();
  refreshPresetSelect();

  if (!audioEnabledRef) {
    dom.playToggle.disabled = true;
    dom.panicButton.disabled = true;
    toast("Audio disabled: Tone.js not available. UI is still editable.", "error");
  }

  setStepListener(({ step, laneId }) => {
    updateSingleLanePlayhead(laneId, step);
    updateTopControls(getState());
  });

  storeRef.subscribe((_prev, next, action) => {
    if (action?.meta?.transient && action.type === "TRANSPORT_SET") {
      updateTopControls(next);
      updatePlayheadClasses(next);
      return;
    }
    if (!next.transport.isPlaying) {
      livePlayheads = {};
    }
    renderAll(next);
  });

  if (rafId) cancelAnimationFrame(rafId);
  rafId = requestAnimationFrame(visualFrame);
}
