// Amorph Custom UI - Simple Tuner (YIN Precision)
// WINDOW SIZE: 240x130


class SimpleTunerView extends HTMLElement {
    constructor (patchConnection) {
        super();
        this.pc = patchConnection;
        this._pitch = 0;
        this._clarity = 0;
        this._cents = 0;
        this._animFrame = null;
        this._needlePos = 50;
        this._glow = 0;
        this._smoothAmount = 0.5;
        this._smoothHandler = null;
        this._knobCleanup = null;
        this.innerHTML = this.getHTML();
    }
    connectedCallback () {
        this._pitchHandler = v => { this._pitch = v; };
        this._clarityHandler = v => { this._clarity = v; };
        this._centsHandler = v => { this._cents = v; };
        this.pc.addEndpointListener('pitchOut', this._pitchHandler);
        this.pc.addEndpointListener('clarityOut', this._clarityHandler);
        this.pc.addEndpointListener('centsOut', this._centsHandler);
        this._smoothHandler = v => { this._smoothAmount = v; if (this._knobFill) this._knobFill.style.height = Math.round(v * 100) + '%'; };
        this.pc.addParameterListener('param1', this._smoothHandler);
        this.pc.sendEventOrValue('param1', this._smoothAmount);
        this._initSmoothKnob();
        this.startAnimationLoop();
    }
    disconnectedCallback () {
        this.pc.removeEndpointListener('pitchOut', this._pitchHandler);
        this.pc.removeEndpointListener('clarityOut', this._clarityHandler);
        this.pc.removeEndpointListener('centsOut', this._centsHandler);
        if (this._smoothHandler) this.pc.removeParameterListener('param1', this._smoothHandler);
        if (this._knobCleanup) this._knobCleanup();
        if (this._animFrame) cancelAnimationFrame(this._animFrame);
    }
    _freqToNote (freq) {
        if (freq <= 0) return { name: '\u2014', full: '\u2014' };
        const note = 12 * Math.log2(freq / 440) + 69;
        const midi = Math.round(note);
        const names = ['C','C\u266f','D','D\u266f','E','F','F\u266f','G','G\u266f','A','A\u266f','B'];
        const octave = Math.floor(midi / 12) - 1;
        return { name: names[((midi % 12) + 12) % 12], full: names[((midi % 12) + 12) % 12] + octave };
    }
    _initSmoothKnob () {
        const el = this.querySelector('.tiny-knob');
        if (!el) return;
        this._knobFill = el.querySelector('.knob-fill');
        let dragging = false, startY = 0, startVal = 0;
        const onDown = e => {
            dragging = true;
            startY = e.clientY;
            startVal = this._smoothAmount;
            el.setPointerCapture(e.pointerId);
        };
        const onMove = e => {
            if (!dragging) return;
            const v = Math.max(0, Math.min(1, startVal + (startY - e.clientY) * 0.005));
            this._smoothAmount = v;
            if (this._knobFill) this._knobFill.style.height = Math.round(v * 100) + '%';
            this.pc.sendEventOrValue('param1', v);
        };
        const onUp = e => {
            dragging = false;
            el.releasePointerCapture(e.pointerId);
        };
        el.addEventListener('pointerdown', onDown);
        el.addEventListener('pointermove', onMove);
        el.addEventListener('pointerup', onUp);
        this._knobCleanup = () => {
            el.removeEventListener('pointerdown', onDown);
            el.removeEventListener('pointermove', onMove);
            el.removeEventListener('pointerup', onUp);
        };
    }
    startAnimationLoop () {
        const loop = () => {
            const freq = this._pitch;
            const cents = this._cents;
            const clarity = this._clarity;
            const hasSignal = clarity > 0.25 && freq > 0;

            const noteEl   = this.querySelector('.note');
            const octEl    = this.querySelector('.octave');
            const centsEl  = this.querySelector('.cents');
            const freqEl   = this.querySelector('.freq');
            const needleEl = this.querySelector('.needle');
            const meterEl  = this.querySelector('.meter');
            const inTune   = this.querySelector('.in-tune-zone');

            const ac = Math.abs(cents);
            const targetPos = hasSignal
                ? Math.max(0, Math.min(100, 50 + (cents / 50) * 50))
                : 50;
            const dist = Math.abs(targetPos - this._needlePos);
            const baseRate = dist > 15 ? 0.30 : dist > 5 ? 0.12 : dist > 1.5 ? 0.05 : 0.015;
            const smoothMul = this._smoothAmount < 0.01 ? 100 : 0.1 + (1 - this._smoothAmount) * 1.8;
            const rate = Math.min(1, baseRate * smoothMul);
            this._needlePos += (targetPos - this._needlePos) * rate;
            needleEl.style.left = this._needlePos.toFixed(2) + '%';

            const targetGlow = hasSignal && ac <= 2 ? 1 : 0;
            this._glow += (targetGlow - this._glow) * 0.12;
            meterEl.style.setProperty('--glow', this._glow.toFixed(3));
            inTune.style.opacity = (this._glow * 0.8).toFixed(3);

            if (hasSignal) {
                const info = this._freqToNote(freq);
                noteEl.textContent = info.name;
                octEl.textContent = info.full.length > 1 ? info.full.slice(-1) : '';
                const sign = cents > 0 ? '+' : '';
                centsEl.textContent = sign + cents.toFixed(1) + '\u00a2';
                freqEl.textContent = freq.toFixed(2) + ' Hz';
                const color = ac <= 1 ? '#00ff66' : ac <= 3 ? '#66ff00' : ac <= 7 ? '#ccff00' : ac <= 15 ? '#ffcc00' : ac <= 30 ? '#ff6600' : '#ff2222';
                needleEl.style.background = color;
                needleEl.style.boxShadow = ac <= 2
                    ? '0 0 6px ' + color
                    : 'none';
                centsEl.style.color = color;
                noteEl.style.color = ac <= 2 ? '#00ff66' : '#e8e8e8';
            } else {
                noteEl.textContent = '\u2014';
                octEl.textContent = '';
                centsEl.textContent = '\u2014\u00a2';
                freqEl.textContent = '\u2014\u2014\u2014 Hz';
                needleEl.style.background = '#2a2a2a';
                needleEl.style.boxShadow = 'none';
                centsEl.style.color = '#2a2a2a';
                noteEl.style.color = '#2a2a2a';
            }

            this._animFrame = requestAnimationFrame(loop);
        };
        this._animFrame = requestAnimationFrame(loop);
    }
    getHTML () {
        return [
'<style>',
'  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }',
'  :host { display: block; width: 100%; height: 100%; overflow: hidden;',
'          background: #050505; font-family: "SF Mono", "Consolas", "Courier New", monospace;',
'          user-select: none; -webkit-user-select: none; }',
'  .panel { display: flex; flex-direction: column; align-items: center;',
'           justify-content: center; height: 100%; padding: 6px 8px; gap: 3px;',
'           position: relative;',
'           background: radial-gradient(ellipse at 50% 40%, #0a0a0a 0%, #050505 70%); }',
'',
'  .note-row { display: flex; align-items: baseline; gap: 4px; }',
'  .note { font-size: 36px; font-weight: 800; color: #e8e8e8; line-height: 0.9;',
'          font-variant-numeric: tabular-nums; letter-spacing: -0.5px;',
'          transition: color 0.1s; min-width: 30px; text-align: right; }',
'  .octave { font-size: 14px; font-weight: 600; color: #666; line-height: 1;',
'            font-variant-numeric: tabular-nums; min-width: 10px; }',
'  .cents { font-size: 14px; font-weight: 700; min-width: 52px; text-align: left;',
'           font-variant-numeric: tabular-nums; transition: color 0.1s;',
'           margin-left: 8px; }',
'  .freq { font-size: 10px; color: #444; font-variant-numeric: tabular-nums;',
'          letter-spacing: 0.3px; }',
'',
'  .meter { width: 100%; height: 16px; position: relative; --glow: 0;',
'           transition: box-shadow 0.15s; }',
'  .track { width: 100%; height: 100%; position: relative;',
'           border-radius: 1px; }',
'  .line { position: absolute; left: 0; right: 0; top: 50%; height: 1px;',
'          background: #1a1a1a; transform: translateY(-50%); }',
'  .tick { position: absolute; top: 3px; bottom: 3px; width: 1px; background: #2a2a2a; }',
'  .tick.mid { background: #333; }',
'  .tick.center { background: #555; width: 2px; top: 0; bottom: 0; }',
'',
'  .in-tune-zone { position: absolute; left: 46%; width: 8%; top: 0; bottom: 0;',
'                  background: linear-gradient(180deg, transparent, rgba(0,255,102,0.06), transparent);',
'                  opacity: 0; pointer-events: none; }',
'',
'  .needle { position: absolute; top: 0; bottom: 0; width: 2px; background: #f00;',
'            transform: translateX(-50%); }',
'',
'  .logo { position: absolute; bottom: 3px; right: 6px; width: 48px; opacity: 0.12;',
'          pointer-events: none; }',
'  .knob-area { position: absolute; bottom: 3px; left: 6px; display: flex;',
'               align-items: center; gap: 3px; pointer-events: auto; }',
'  .tiny-knob { width: 6px; height: 20px; background: #111; border: 1px solid #333;',
'               border-radius: 1px; cursor: ns-resize; position: relative; touch-action: none; }',
'  .knob-fill { position: absolute; bottom: 0; left: 0; right: 0; background: #555;',
'               border-radius: 0 0 1px 1px; }',
'  .knob-label { font-size: 7px; color: #444; letter-spacing: 0.3px; user-select: none; }',
'</style>',
'<div class="panel">',
'  <div class="note-row">',
'    <span class="note">\u2014</span>',
'    <span class="octave"></span>',
'    <span class="cents">\u2014\u00a2</span>',
'  </div>',
'  <div class="meter">',
'    <div class="track">',
'      <div class="in-tune-zone"></div>',
'      <div class="line"></div>',
'      <div class="tick" style="left:0%;"></div>',
'      <div class="tick mid" style="left:25%;"></div>',
'      <div class="tick center" style="left:50%;"></div>',
'      <div class="tick mid" style="left:75%;"></div>',
'      <div class="tick" style="left:100%;"></div>',
'      <div class="needle" style="left:50%;background:#2a2a2a;"></div>',
'    </div>',
'  </div>',
'  <span class="freq">\u2014\u2014\u2014 Hz</span>',
'  <div class="knob-area">',
'    <div class="tiny-knob"><div class="knob-fill" style="height:50%"></div></div>',
'    <span class="knob-label">SM</span>',
'  </div>',
'  <div class="logo"><svg xmlns="http://www.w3.org/2000/svg" fill="#fcfdfc" viewBox="294.15 84.3 1461.67 569"><path d="M1256.13 198.447a162.18 162.18 0 01111.06 41.283c33.89 29.926 55.78 75.595 58.56 120.675a169.07 169.07 0 01-42.1 122.893c-50.59 57.167-132.05 76.066-200.5 39.404-15.1-8.086-26.95-17.493-39.72-28.796-.59 12.339-.16 28.004-.17 40.594l.63 118.797-55.71-.062-.12-448.985c18.1-.316 37.29.053 55.46.099l-.07 42.286c29.09-29.84 71.33-46.236 112.68-48.188m6.38 288.673c64.62-2.577 114.91-57.047 112.34-121.666s-57.04-114.917-121.66-112.349-114.93 57.041-112.35 121.666c2.57 64.625 57.05 114.927 121.67 112.349M1255 280.111c3.41-.123 7.34-.042 10.34 1.775a10.93 10.93 0 014.98 7.055c2.55 12.398 1.44 55.562 1.6 70.089l-.27 86.773c-.21 9.076-3.39 10.913-10.81 14.449a23.2 23.2 0 01-5.87-.824c-3.08-.825-6.69-2.391-8.26-5.36-2.55-4.794-2.54-131.681-2.15-146.395.16-6.235.01-14.392 2.05-20.264 1.5-4.282 4.56-5.55 8.39-7.298m-46.14 42.005c3.64-.258 13.61-.274 15.06 4.074 8.28 24.903 4.65 55.827 3.91 82.009-.14 4.743-3.94 7.389-7.34 9.895-11.69.293-17.66.132-18.82-13.584-1.96-22.952-1.71-46.404-.49-69.401.39-7.404 2.38-8.553 7.68-12.993m90.84.059c2.44.226 4.88.561 7.2 1.37 3.1 1.079 5.49 3.643 6.81 6.606 4.56 10.262 3.59 68.837-.78 79.625-1.79 4.431-3.98 6.668-8.34 8.556-2.96-.113-6.36-.401-8.99-1.841-3.31-1.814-4.78-4.881-5.78-8.367-3.86-13.392-4.01-69.533 2.59-81.042 1.83-3.183 3.83-3.984 7.29-4.907m-134.55 29.941c25.58-2.604 25.67 25.422 11.17 36.498-23.32 2.978-25.12-26.266-11.17-36.498m175.64-.108c22.95-.577 24.83 24.254 8.58 36.261-6.44-.249-12.1-1.659-16.07-7.186a17.87 17.87 0 01-3.02-13.511c1.19-7.024 4.97-11.599 10.51-15.564M885.807 198.677c94.5-5.669 175.763 66.221 181.653 160.708s-65.81 175.914-160.281 182.029c-94.789 6.135-176.549-65.855-182.46-160.658s66.272-176.392 161.088-182.079m25.722 296.202c69.124-8.41 118.261-71.379 109.621-140.475s-71.771-118.026-140.838-109.155c-68.742 8.83-117.39 71.6-108.79 140.371s71.207 117.629 140.007 109.259m-19.795-233.811c9.721-.434 11.492 1.36 17.518 8.309 2.222 30.261.911 71.452.897 102.609-.012 27.858 1.618 72.076-.81 98.871-3.444 4.025-4.847 5.16-9.058 8.397a97 97 0 01-3.402-.336c-13.202-1.557-14.455-9.009-14.627-20.8l-.111-172.252c.095-12.978-1.354-17.024 9.593-24.798m43.631 43.107c2.739-.209 5.525-.212 8.245.221 3.128.497 6.208 2.06 7.995 4.741 6.818 10.231 4.241 102.409 1.428 117.567-.954 5.139-2.142 7.534-6.565 10.586a29.5 29.5 0 01-4.19.289c-4.201-.035-9.333-1.763-12.202-4.881-5.617-6.103-3.562-103.771-2.647-119.855 3.027-4.387 3.803-5.289 7.936-8.668m-88.531-.197q.524.006 1.047.024c9.555.273 17.097 2.433 17.4 13.204 1.025 36.471 1.701 76.117.019 112.356-2.785 3.29-4.844 4.866-8.139 7.569-1.835.105-3.661.147-5.495-.001-4.227-.34-7.931-1.485-10.679-4.903-4.289-5.334-4.002-103.546-2.572-116.076.665-5.825 4.168-8.712 8.419-12.173m134.396 39.843c25.51-2.261 24.39 41.627 9.341 53.838-20.699 2.564-20.184-21.835-18.718-35.761.893-8.484 2.626-12.848 9.377-18.077m-178.992.086c20.418-1.167 19.503 21.749 18.346 36.431-.614 7.802-3.578 12.187-9.759 16.737-20.279 2.73-19.704-21.365-18.59-35.185.649-8.055 3.606-13.164 10.003-17.983M1446.11 85.313l55.45.016.57 157.559c29.58-28.244 61.67-44.612 103.6-44.737A149.08 149.08 0 011712 242.53a145.1 145.1 0 0137.35 63.41c8.05 29.349 6.04 79.101 6.12 111.138l.32 120.672c-17.75.376-36.77.035-54.61.044l-.38-192.982c-1.79-26.145-8.11-45.456-26.51-64.953-51.72-54.957-155.28-29.362-169.75 46.607-4.73 24.835-3.1 53.036-3.11 78.527l.32 132.724c-18.65.359-37.3.11-55.93-.746 1.61-20.675.89-45.293.76-66.346l-.44-106.29.46-167.107c.13-36.613.99-75.588-.49-111.915m-984.365 113.11c19.066-2.024 44.574 4.167 62.294 11.627 48.615 20.465 91.842 65.274 100.73 118.622l-54.086-.111c-5.799-15.115-11.841-27.009-22.733-39.313a108.9 108.9 0 00-76.034-36.837c-30.815-1.854-60.272 6.62-83.543 27.294a118.25 118.25 0 00-39.47 80.937c-6.135 108.833 113.046 168.035 194.1 95.08 15.008-15.295 22.757-28.909 27.522-49.751 17.75-.28 37.759-.58 55.392.369-3.402 27.333-17.805 55.321-35.65 76.166a169.96 169.96 0 01-115.294 58.215 163.92 163.92 0 01-119.537-40.237 175.05 175.05 0 01-60.807-119.496c-3.249-43.665 11.225-89.315 39.941-122.473 34.749-40.125 75.306-56.331 127.175-60.092M645.508 84.326c17.854.525 36.439.298 54.366.307a22331 22331 0 01-.036 347.834c-.054 36.149-2.421 55.853 39.264 55.561-.228 9.76 1.204 43.492-.256 49.94-28.012.415-49.097.366-70.521-20.44-24.736-24.024-23.286-53.386-23.31-84.754l.488-69.465-.375-154.246zm973.532 195.841c6.23-.695 12.51-.107 16.32 5.564 1.35 2.013 2.3 3.719 2.36 6.216l.28 37.515-.47 76.367-.81 44.485c-.17 3.979-4.79 7.21-7.87 9.386-22.49 2.048-18.75-16.258-18.7-32.634l.43-140.047c2.32-2.277 5.83-4.82 8.46-6.852m-1159.454-.081c15.223-1.392 17.912 5.199 18.144 18.707l.425 148.169c.008 5.685-4.281 9.244-8.142 12.709-8.279 1.181-17.606-1.046-18.191-11.013-.993-16.93-.68-34.227-.771-51.228q-.36-54.365.145-108.728c3.13-4.34 4.212-5.243 8.39-8.616m42.177 42.146c14.31-.458 20.637 5.142 19.989 20.147-.808 18.719 5.107 61.443-7.471 75.282-4.29.36-16.089.83-17.282-5.026-5.211-25.579-3.323-54.796-2.73-80.987.096-4.256 4.446-7.197 7.494-9.416m-85.868-.04c23.318-3.003 19.648 21.056 19.924 38.396.241 15.123 3.786 48.804-8.233 57.33-2.408.188-4.893.423-7.307.302-3.427-.172-6.826-1.604-9.043-4.299-5.149-6.258-4.039-69.966-2.922-80.791.561-5.44 3.616-7.739 7.581-10.938m1159.065-.093c2.61-.223 5.24-.188 7.84.105 2.39.28 6.24 1.804 7.09 3.618 6.3 13.46 4.79 71.829 3.29 85.149-2.9 3.165-4.77 4.537-8.1 7.095-4.5.209-14.48-.313-15.7-5.972-5.42-25.224-3.7-54.357-2.49-80.204.23-4.821 4.53-7.454 8.07-9.791m87.9 28.011a45.14 45.14 0 013.31.078c19.58 1.289 20.28 27.197 5.71 39.389-14.26-.462-20.42-7.735-19.25-23.352.52-6.985 4.63-12.047 10.23-16.115m-131.08.93l.31-.008c22.7-.441 24.05 25.328 9.98 38.334-25.24-1.379-27.37-26.775-10.29-38.326m-1157.334.963c20.519-3.363 26.916 27.548 6.278 37.187-.944-.021-1.788-.061-2.727-.194a17.4 17.4 0 01-11.453-6.921c-7.75-10.652-1.784-23.183 7.902-30.072m172.509-.089c21.301.623 24.185 22.327 8.99 35.642-22.129 1.543-24.041-22.372-8.99-35.642m56.319 3.65a14.91 14.91 0 017.066 28.957 14.908 14.908 0 01-18.131-18.686 14.91 14.91 0 0111.065-10.271"/></svg></div>',
'</div>'
        ].join('\n');
    }
}

export default function createPatchView (patchConnection) {
    const name = 'simple-tuner-view';
    if (!window.customElements.get(name))
        window.customElements.define(name, class extends SimpleTunerView {
            constructor () { super(patchConnection); }
        });
    return new (window.customElements.get(name))();
}