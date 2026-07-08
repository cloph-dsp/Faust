import { createStore } from "./statemanager.js";
import {
  initAudio,
  startTransport,
  stopTransport,
  syncFromState,
  destroyAudio,
  getAnalyzerNodes,
  setStepListener,
  panic
} from "./audioengine.js";
import { initUI } from "./uicontroller.js";

const store = createStore();

function createSilentTransport() {
  return {
    startTransport: async () => {},
    stopTransport: () => {},
    panic: () => {}
  };
}

document.addEventListener("DOMContentLoaded", () => {
  let analyzers = [];
  let transport = createSilentTransport();
  let stepListenerSetter = () => {};
  let audioEnabled = false;

  try {
    if (typeof Tone === "undefined") {
      throw new Error("Tone.js was not loaded.");
    }
    initAudio(store);
    analyzers = getAnalyzerNodes();
    transport = { startTransport, stopTransport, panic };
    stepListenerSetter = setStepListener;
    audioEnabled = true;
  } catch (error) {
    console.error("Audio engine disabled:", error);
  }

  initUI({
    store,
    analyzers,
    transport,
    setStepListener: stepListenerSetter,
    audioEnabled
  });

  store.subscribe((prev, next, action) => {
    try {
      syncFromState(prev, next, action);
    } catch (error) {
      console.error("Audio sync failed:", error);
    }
  });
});

window.addEventListener("beforeunload", () => {
  destroyAudio();
});
