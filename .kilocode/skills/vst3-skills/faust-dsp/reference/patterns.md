# Faust Patterns and Techniques

## Oscillators

### Basic Sine Oscillator
```
process = os.osc(440);
```

### Multi-oscillator
```
freq = hslider("freq", 440, 20, 2000, 1);
process = os.osc(freq), os.osc(freq*2), os.osc(freq*3);
```

### FM Synthesis
```
carrier = hslider("carrier", 220, 20, 1000, 1);
mod = hslider("modulator", 110, 20, 1000, 1);
index = hslider("index", 100, 0, 1000, 1);
process = os.osc(carrier + os.osc(mod)*index);
```

### Additive Synthesis
```
fundamental = hslider("freq", 110, 20, 880, 1);
process = sum(i, 8, os.osc(fundamental*(i+1)) / (i+1));
```

## Filters

### Lowpass Filter
```
cutoff = hslider("cutoff", 1000, 20, 20000, 1);
order = hslider("order", 1, 1, 8, 1);
process = no.noise : fi.lowpass(order, cutoff);
```

### Resonant Filter (LP)
```
process = os.osc(220) : fi.resonlp(500, 10, 1);
```

### Parametric EQ
```
freq = hslider("freq", 1000, 20, 20000, 1);
gain = hslider("gain", 0, -24, 24, 0.1);
q = hslider("q", 1, 0.1, 20, 0.1);
process = no.noise : fi.peak_eq(freq, q, gain);
```

## Delays

### Basic Delay
```
delay = hslider("delay", 100, 1, 1000, 1);
process = no.noise : de.delay(1000, delay);
```

### Feedback Delay
```
delay = hslider("delay", 200, 1, 1000, 1);
feedback = hslider("feedback", 0.5, 0, 0.99, 0.01);
process = _ : de.delay(1000, delay) : +~*(feedback);
```

### Ping-Pong Delay
```
delayL = hslider("delayL", 100, 1, 500, 1);
delayR = hslider("delayR", 150, 1, 500, 1);
feedback = hslider("feedback", 0.4, 0, 0.9, 0.01);
process = _ <: 
  de.delay(500, delayL) : +~*(feedback),
  de.delay(500, delayR) : +~*(feedback)
  :> _,_;
```

### Chorusing
```
speed = hslider("speed", 1, 0.1, 10, 0.1);
depth = hslider("depth", 5, 0, 20, 0.1);
lfo = os.osc(speed) * depth;
process = _ : de.fdelay(500, lfo + 10);
```

## Envelopes

### ADSR Envelope
```
gate = button("gate");
attack = hslider("attack", 0.01, 0.001, 1, 0.001);
decay = hslider("decay", 0.1, 0.001, 1, 0.001);
sustain = hslider("sustain", 0.7, 0, 1, 0.01);
release = hslider("release", 0.3, 0.001, 2, 0.001);
env = en.adsr(attack, decay, sustain, release, gate);
process = os.osc(440) * env;
```

### Simple AR Envelope
```
gate = button("gate");
attack = hslider("attack", 0.01, 0.001, 0.5, 0.001);
release = hslider("release", 0.2, 0.001, 2, 0.001);
env = en.ar(attack, release, gate);
process = os.osc(440) * env;
```

## Dynamics

### Simple Compressor
```
thresh = hslider("threshold", -20, -60, 0, 0.1);
ratio = hslider("ratio", 4, 1, 20, 0.1);
attack = hslider("attack", 0.01, 0.001, 0.5, 0.001);
release = hslider("release", 0.1, 0.001, 1, 0.001);
gain = hslider("gain", 0, -20, 20, 0.1);
process = no.noise : co.compressor_mono(thresh, ratio, 0.5, attack, release) : *(gain : ba.db2linear);
```

### Limiter
```
process = no.noise : co.limiter_mono : *(1.5);
```

### Noise Gate
```
process = no.noise : dm.noise_gate;
```

## Reverb

### Using Demo Reverb
```
process = no.noise <: dm.zita_light;
```

### Freeverb
```
process = no.noise <: dm.freeverb_demo;
```

### Custom Reverb (Algorithmic)
```
import("stdfaust.lib");
room = hslider("room", 0.5, 0, 1, 0.01);
process = no.noise : re.freeverb(room, 0.5);
```

## Effects Chain

### Guitar Stomp Chain
```
drive = hslider("drive", 0.5, 0, 1, 0.01);
tone = hslider("tone", 0.5, 0, 1, 0.01);
gain = hslider("gain", 0.5, 0, 1, 0.01);
process = _ : ef.cubicnl(drive, 0.5) : fi.lowpass(3, 2000 + tone*8000) : *(gain);
```

### Multi-FX Chain
```
import("stdfaust.lib");
dist = hslider("drive", 0, 0, 1, 0.01);
filter = hslider("filter", 1000, 100, 10000, 10);
wah = hslider("wah", 0, 0, 1, 0.01);
mix = hslider("mix", 0.5, 0, 1, 0.01);
wet = _ : ef.cubicnl(dist, 0.2) : fi.lowpass(4, filter) : dm.wah4_demo(wah);
dry = _;
process = dry, wet :> *(1-mix), *(mix);
```

## Noise Types

### White Noise
```
process = no.noise * 0.1;
```

### Pink Noise
```
process = no.pink_noise * 0.1;
```

### Brown Noise
```
process = no.brown_noise * 0.1;
```

### Crackle/Impulse Noise
```
process = no.lfnoise0(5);
```

## Physical Modeling

### Karplus-Strong String
```
freq = hslider("freq", 220, 20, 2000, 1);
damping = hslider("damping", 0.99, 0.9, 0.999, 0.0001);
gate = button("gate");
trigger = gate : ba.impulsify;
length = ma.SR / freq;
process = trigger : +~(@(int(length)) : fi.lowpass(1, 5000) : *(damping));
```

### Drum Synth
```
import("stdfaust.lib");
pitch = hslider("pitch", 100, 20, 500, 1);
decay = hslider("decay", 0.5, 0.01, 2, 0.01);
gate = button("gate");
process = gate : pm.djembe(pitch, decay, 0.5, 1);
```

## Stereo Processing

### Mid-Side Encoding/Decoding
```
// Encode
process = _ <: (+, -);
// Decode  
process = _ <: (+, -) :> (_,_);
```

### Stereo Width
```
width = hslider("width", 1, 0, 2, 0.01);
process = _ <: ef.stereo_width(width) :> _,_;
```

### Dual Mono
```
left = hslider("left", 1000, 20, 20000, 1);
right = hslider("right", 1000, 20, 20000, 1);
process = no.noise : fi.lowpass(3, left), fi.lowpass(3, right);
```

## Modulation

### LFO to Parameter
```
lfo = os.osc(0.5);  // Slow modulation
freq = 1000 + lfo * 500;  // Modulated frequency
process = no.noise : fi.lowpass(3, freq);
```

### Vibrato
```
freq = hslider("freq", 440, 20, 2000, 1);
vibRate = hslider("vibRate", 5, 0.1, 20, 0.1);
vibDepth = hslider("vibDepth", 10, 0, 50, 0.1);
process = os.osc(freq + os.osc(vibRate)*vibDepth);
```

### Tremolo
```
freq = hslider("freq", 440, 20, 2000, 1);
tremRate = hslider("tremRate", 5, 0.1, 20, 0.1);
tremDepth = hslider("tremDepth", 0.5, 0, 1, 0.01);
process = os.osc(freq) * (1 - os.osc(tremRate)*tremDepth);
```

## Utility

### Smooth Parameter Changes
```
freq = hslider("freq", 440, 20, 2000, 1) : si.smoo;
process = os.osc(freq);
```

### dB Conversion
```
gain = hslider("gain [unit:dB]", 0, -60, 12, 0.1) : ba.db2linear;
process = os.osc(440) * gain;
```

### MIDI to Frequency
```
key = hslider("key", 60, 0, 127, 1);
freq = key : ba.midikey2hz;
process = os.osc(freq);
```

### Clickless On/Off
```
gate = button("gate") : si.smoo;
process = os.osc(440) * gate;
```

## Advanced Patterns

### Phaser
```
import("stdfaust.lib");
stages = hslider("stages", 2, 1, 8, 1);
freq = hslider("freq", 1000, 100, 5000, 10);
depth = hslider("depth", 1, 0, 1, 0.01);
rate = hslider("rate", 1, 0.1, 10, 0.1);
process = _ : pf.phaser2_mono(stages, 0, freq, 50, depth, freq, rate, depth, 0.3, 0);
```

### Flanger
```
import("stdfaust.lib");
delay = hslider("delay", 5, 0, 20, 0.1);
depth = hslider("depth", 5, 0, 10, 0.1);
feedback = hslider("feedback", 0.5, 0, 0.9, 0.01);
rate = hslider("rate", 1, 0.1, 10, 0.1);
process = _ : pf.flanger_mono(delay, depth, feedback, rate);
```

### Ring Modulator
```
import("stdfaust.lib");
modFreq = hslider("modFreq", 100, 10, 1000, 1);
process = os.osc(440) * os.osc(modFreq);
```

### Waveshaper
```
import("stdfaust.lib");
drive = hslider("drive", 0, 0, 1, 0.01);
process = _ : ef.cubicnl(drive, 0.5);
```