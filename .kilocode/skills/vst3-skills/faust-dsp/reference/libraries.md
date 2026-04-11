# Faust Standard Libraries Reference

## Quick Access via stdfaust.lib

```faust
import("stdfaust.lib");
```

All libraries accessible via prefixes:
- `os.` - oscillators
- `fi.` - filters
- `en.` - envelopes
- `no.` - noises
- `dm.` - demos
- `de.` - delays
- `re.` - reverbs
- `ma.` - maths
- `ba.` - basics
- `si.` - signals
- `co.` - compressors
- `pf.` - phaflangers
- `pm.` - physmodels
- `ve.` - vaeffects
- `ef.` - misceffects
- `an.` - analyzers
- `ro.` - routes

## Oscillators (os)

| Function | Description |
|----------|-------------|
| `os.osc(freq)` | Sine oscillator |
| `os.triangle(freq)` | Triangle wave |
| `os.sawtooth(freq)` | Sawtooth wave |
| `os.square(freq)` | Square wave |
| `os.oscp(freq, phase)` | Phase-controlled sine |
| `os.noise` | White noise (use `no.noise` instead) |
| `os.parabolic(freq)` | Parabolic wave |
| `os.pulse(freq, width)` | Pulse wave |

**Example**:
```
process = os.osc(440);  // 440 Hz sine wave
process = os.sawtooth(220);  // Low saw wave
```

## Filters (fi)

| Function | Description |
|----------|-------------|
| `fi.lowpass(order, freq)` | Butterworth lowpass |
| `fi.highpass(order, freq)` | Butterworth highpass |
| `fi.bandpass(order, freq)` | Butterworth bandpass |
| `fi.resonlp(freq, q, gain)` | Resonant lowpass |
| `fi.resonhp(freq, q, gain)` | Resonant highpass |
| `fi.resonbp(freq, q, gain)` | Resonant bandpass |
| `fi.peak_eq(freq, q, gain)` | Peaking EQ |
| `fi.lowshelf(freq, gain)` | Low shelf |
| `fi.highshelf(freq, gain)` | High shelf |
| `fi.allpass(freq, q)` | Allpass |
| `fi.fb_comb(maxDelay, delay, feedback)` | Feedback comb |
| `fi.ff_comb(maxDelay, delay, feedforward)` | Feedforward comb |

**Example**:
```
process = no.noise : fi.lowpass(3, 1000);
process = os.osc(220) : fi.resonlp(500, 10, 1);
```

## Envelopes (en)

| Function | Description |
|----------|-------------|
| `en.adsr(a, d, s, r, gate)` | ADSR envelope |
| `en.ar(a, d, gate)` | Attack-Release |
| `en.adsr2(a, d, s, r, gate)` | ADSR with different curve |
| `en.ahdsr(a, h, d, s, r, gate)` | AHDSR with hold |
| `en.adsr_retrigger(a, d, s, r, gate, retrig)` | Retriggerable ADSR |

**Example**:
```
gate = button("gate");
env = en.adsr(0.01, 0.1, 0.7, 0.3, gate);
process = os.osc(440) * env;
```

## Noises (no)

| Function | Description |
|----------|-------------|
| `no.noise` | White noise |
| `no.pink_noise` | Pink noise (1/f) |
| `no.brown_noise` | Brown noise (1/f²) |
| `no.lfnoise0(rate)` | Low-freq white noise (sample-and-hold) |
| `no.lfnoise1(rate)` | Low-freq white noise (linear) |
| `no.lfnoise2(rate)` | Low-freq white noise (quadratic) |

**Example**:
```
process = no.noise * 0.1;
process = no.pink_noise;
```

## Delays (de)

| Function | Description |
|----------|-------------|
| `de.delay(max, size)` | Delay line |
| `de.fdelay(max, size)` | Fractional delay |
| `de.fdelay1(max, size)` | 1-sample fractional delay |
| `de.fdelay2(max, size)` | 2-sample fractional delay |
| `de.fdelay3(max, size)` | 3-sample fractional delay |
| `de.fdelay4(max, size)` | 4-sample fractional delay |

**Example**:
```
delay = hslider("delay", 100, 1, 1000, 1);
process = no.noise : de.delay(1000, delay);
```

## Maths (ma)

| Function | Description |
|----------|-------------|
| `ma.SR` | Sample rate (e.g., 44100, 48000) |
| `ma.PI` | Pi (3.14159...) |
| `ma.E` | E (2.71828...) |
| `ma.exp(x)` | Exponential |
| `ma.log(x)` | Natural log |
| `ma.log10(x)` | Base 10 log |
| `ma.pow(x, y)` | Power |
| `ma.sqrt(x)` | Square root |
| `ma.abs(x)` | Absolute value |
| `ma.sin(x)` | Sine |
| `ma.cos(x)` | Cosine |
| `ma.tan(x)` | Tangent |
| `ma.asin(x)` | Arc sine |
| `ma.acos(x)` | Arc cosine |
| `ma.atan(x)` | Arc tangent |
| `ma.atan2(y, x)` | Arc tangent 2 |
| `ma.tanh(x)` | Hyperbolic tangent |
| `ma.floor(x)` | Floor |
| `ma.ceil(x)` | Ceiling |
| `ma.decimal(x)` | Decimal part |
| `ma.int(x)` | Integer part |
| `ma.min(a, b)` | Minimum |
| `ma.max(a, b)` | Maximum |
| `ma.round(x)` | Round |

## Basics (ba)

| Function | Description |
|----------|-------------|
| `ba.db2linear(dB)` | dB to linear |
| `ba.linear2db(lin)` | Linear to dB |
| `ba.midikey2hz(key)` | MIDI note to Hz |
| `ba.hz2midikey(hz)` | Hz to MIDI note |
| `ba.pianokey2hz(key)` | Piano key to Hz |
| `ba.take(n, list)` | Take n elements |
| `ba.selector(index, n)` | Select one of n |
| `ba.count(list)` | Count elements |
| `ba.impulsify(sig)` | Extract impulse |
| `faust.rewrite` | Rewrite pattern |

**Example**:
```
freq = hslider("freq", 60, 0, 127, 1) : ba.midikey2hz;
process = os.osc(freq);
```

## Signals (si)

| Function | Description |
|----------|-------------|
| `si.smoo` | Smooth (first-order) |
| `si.smooth(tau)` | Smooth with time constant |
| `si.bus(n)` | Create n-way bus |
| `si.interpolate(n)` | Linear interpolation |
| `si.delay(n)` | Delay by n samples |

## Demos (dm) - Ready-made Effects

| Function | Description |
|----------|-------------|
| `dm.zita_rev1` | Zita reverb |
| `dm.zita_light` | Light reverb |
| `dm.freeverb_demo` | Freeverb |
| `dm.wah4_demo` | Wah effect |
| `dm.phaser2_demo` | Phaser |
| `dm.compressor_mono` | Compressor |
| `dm.noise_gate` | Noise gate |
| `dm.distortion_demo` | Distortion |

## Reverbs (re)

| Function | Description |
|----------|-------------|
| `re.freeverb` | Freeverb algorithm |
| `re.zita_rev` | Zita reverb |
| `re.schroeder` | Schroeder reverb |

## Compressors (co)

| Function | Description |
|----------|-------------|
| `co.compressor_mono(thresh, ratio, knee, attack, release)` | Mono compressor |
| `co.compressor_stereo` | Stereo compressor |
| `co.limiter_mono` | Limiter |
| `co.dynamic_eq` | Dynamic EQ |

## Physical Models (pm)

| Function | Description |
|----------|-------------|
| `pm.clarinet` | Clarinet model |
| `pm.flute` | Flute model |
| `pm.bowed` | Bowed string |
| `pm.brass` | Brass model |
| `pm.guitar` | Guitar model |
| `pm.piano` | Piano model |
| `pm.djembe` | Djembe drum |
| `pm.marimba` | Marimba |

## VA Effects (ve)

| Function | Description |
|----------|-------------|
| `ve.moog_vcf` | Moog ladder filter |
| `ve.moog_vcf_2b` | 2-pole Moog |
| `ve.tube` | Tube overdrive |
| `ve.preamp` | Preamp |

## Misc Effects (ef)

| Function | Description |
|----------|-------------|
| `ef.cubicnl(in, drive)` | Cubic nonlinearity |
| `ef.cubicnl_nodc(in, drive)` | Cubic without DC |
| `ef.tanh_distortion` | Tanh distortion |
| `ef.stereo_width(amount)` | Stereo width |

## Analyzers (an)

| Function | Description |
|----------|-------------|
| `an.amp_follower(attack, release)` | Amplitude follower |
| `an.amp_follower_fusion(attack, release)` | Fused amplitude follower |
| `an.peak_follower(attack, release)` | Peak follower |
| `an.meter` | RMS meter |

## Routes (ro)

| Function | Description |
|----------|-------------|
| `ro.nested_crossfade(n)` | Nested crossfade |
| `ro.hadamard(n)` | Hadamard matrix |
| `ro.butterfly(n)` | Butterfly network |
| `ro.route(Ni, No, connections)` | Route signals |