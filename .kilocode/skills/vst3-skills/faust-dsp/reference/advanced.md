# Faust Advanced Topics

## Foreign Functions

Call C functions from Faust:

```
// Declare external function
asinh = ffunction(float asinhf|asinh|asinhl|asinfx(float), <math.h>, "");

// Use it
process = no.noise : +(asinh);
```

Format: `ffunction(returnType name|altSingle|altDouble|altQuad|altFixed(paramType), <include>, <library>)`

### Available Types
- `float` - single precision
- `double` - double precision
- `int` - integer

### Example: Custom Math

```
// Sine approximation
mysin = ffunction(float mysin(float), "mysin.c", "");
```

## Variant Precision

Different code for different precision:

```
singleprecision MAX = 3.402823466e+38;
doubleprecision MAX = 1.7976931348623158e+308;

singleprecision import("single.lib");
doubleprecision import("double.lib");
```

## Block Diagram Expression

### Understanding Diagram Semantics

Faust compiles to block diagrams. Use `-svg` to visualize:

```
faust -svg mydsp.dsp
```

### Diagram Metadata

Add metadata to any expression:

```
<diagram>process</diagram>
```

This generates the block diagram in documentation.

## Soundfile Support

### Load Audio Files

```
sound = soundfile("filename.wav", numChannels)[position];
```

**Example**:
```
// Load 2-channel audio, read at position
sample = sound("drum.wav", 1)[phase];
```

### Soundfile Parameters
- filename: path to audio file
- numChannels: number of channels to read
- position: read position (0-1)

## Environment Advanced

### Explicit Substitution

Customize imported components:

```
reverb = component("freeverb.dsp")[room = 0.5;];
```

### Default Parameters

```
filterParams = environment {
  freq = 1000;
  q = 1;
  gain = 1;
};

customFilter(p) = fi.peak_eq(p.freq, p.q, p.gain);

// Override defaults
process = no.noise : customFilter(filterParams[freq = 2000;]);
```

## Recursion Patterns

### Counter
```
counter = 0 ~ +1;
process = counter;
```

### Timer
```
timer = 1.0' ~ (_ + 1.0) : %(SR);
process = timer;
```

### Waveform Reader
```
wave = waveform{0, 0.5, 1, 0.5, 0, -0.5, -1, -0.5};
phase = +(1)~%(8);
process = wave, phase : rdtable(8, _, phase);
```

### Sample and Hold
```
sah = no.lfnoise0(10) : +~_(button("hold"));
```

## Higher-Order Functions

### Map Over List
```
map(f, list) = list : par(i, count, f);
```

### Filter List
```
filter(p, list) = list <: par(i, count, select2(p(i), _, !));
```

### Fold
```
fold(init, f, list) = init, list : par(i, count, f) : sum;
```

## Complex Patterns

### Convolution (Impulse Response)

```
// Load impulse response
ir = waveform{0.1, 0.2, 0.3, ...};
// Use with filter
convolve = _, ir : rdtable(irSize, ir, _);
process = no.noise : par(i, 4, convolve);
```

### Granular Synthesis

```
grainSize = hslider("grain", 100, 10, 1000, 1);
position = no.lfnoise0(5);
grain = sound("sample.wav", 1)[position] : si.fixe(grainSize);
process = no.noise <: par(i, 8, grain);
```

### Karplus-Strong Algorithm

```
freq = hslider("freq", 220, 20, 2000, 1);
damping = hslider("damping", 0.99, 0.9, 0.999, 0.0001);
trigger = button("gate") : ba.impulsify;
delay = int(ma.SR / freq);

process = trigger 
  : +~(@(delay) : fi.lowpass(1, 5000) : *(damping));
```

### Waveshaping

```
transfer = waveform{-1, -0.5, 0, 0.5, 1};
shape = _, transfer : rdtable(256, transfer, _);
process = no.noise : shape;
```

## Polyphonic Synthesis

### Basic Polyphony

Architecture files handle polyphony. In code:

```
freq = hslider("freq [midi:key]", 440, 20, 2000, 1) : ba.midikey2hz;
gain = hslider("gain", 0.5, 0, 1, 0.01);

process = os.osc(freq) * gain;
```

### Voice Allocation

Use `-poly` flag:
```
faust -poly mydsp.dsp
```

### Voice State

Maintain per-voice state:
```
voice(i) = os.osc(freq);
process = par(i, 8, voice(i)) :> _;
```

## Advanced Filter Design

### Custom Biquad

```
biquad(a0, a1, a2, b1, b2) = 
  +(x0) : *(a0) + ~(a1*x0 + a2*x1 - b1*y0 - b2*y1)
with {
  x0 = _;
  x1 = x0';
  y0 = x0';
  y1 = y0';
};

process = no.noise : biquad(1, -2, 1, -1.8, 0.85);
```

### State Variable Filter

```
svf(freq, q) = 
  loop(b0, b1, b2, a1, a2)
with {
  b0 = 0.5*(1 - cos(2*PI*freq/ma.SR));
  b1 = 0;
  b2 = -0.5*(1 - cos(2*PI*freq/ma.SR));
  a1 = -2*cos(2*PI*freq/ma.SR);
  a2 = q - 1;
};

process = no.noise : svf(1000, 5);
```

## FFT and Spectrum

### Spectral Processing

Use with external libraries for FFT-based processing.

### Analyzer Display

```
import("stdfaust.lib");
spectrum = an.meter : si.smoo;
process = no.noise : hbargraph("level", 0, 1, spectrum);
```

## Architecture Files

### Custom Architecture

Create custom `-arch` file for specific targets:
- JUCE
- VST
- Web Audio
- SuperCollider
- Max/MSP
- etc.

### Architecture Options

```
declare options "[midi:on][osc:on]";
```

## Deployment Targets

### Web (WebAudio)

```
faust -web mydsp.dsp
```

### iOS (AudioKit)

```
faust -ios mydsp.dsp
```

### Android (Oboe)

```
faust -android mydsp.dsp
```

### JUCE Plugin

```
faust -juce mydsp.dsp
```

### VCV Rack

```
faust -vcvrack mydsp.dsp
```

## Best Practices

1. Always define process first
2. Use `stdfaust.lib` for library access
3. Smooth all UI inputs
4. Use control grouping for complex UIs
5. Keep delays bounded
6. Use appropriate filter order
7. Test at multiple sample rates
8. Check CPU usage in target

## Performance Tuning

1. Profile in target environment
2. Use `-mcd` to limit delay sizes
3. Consider table lookup for expensive functions
4. Put smoothing before expensive transforms
5. Use appropriate precision (single/double/fixed)

## Testing

1. Generate SVG to verify signal flow
2. Use -dump to inspect generated code
3. Add meters for signal monitoring
4. Test automation for clicks/pops
5. Test extreme parameter values