# Faust Optimization Reference

## CPU Optimization

### Reduce Sample-Rate Work
Move expensive calculations out of the audio loop:

```
// Bad: exp() calculated per sample
freq = hslider("freq", 440, 20, 2000, 1);
process = os.osc(freq + exp(control));

// Good: exp() calculated at control rate, then smoothed
control = hslider("control", 0, 0, 10, 0.1);
freq = (control : exp) : si.smoo;
process = os.osc(freq);
```

### Use Tables for Expensive Functions
Instead of computing sin/cos/exp per sample, use lookup tables:

```
// Use rdtable for waveform synthesis
sine = 2*ma.PI*float(phase)/float(size) : sin;
phase = (+(1)) ~ %(size);
wave = rdtable(size, sine, phase);
```

### Delay Line Optimization
Use `-mcd` (max compile-time delay) to reduce memory:
```
// Compile with: -mcd 1024
process = no.noise : de.delay(1024, 500);
```

### Filter Order Trade-offs
Higher order = more CPU but better response:
```
// 1-pole: cheap, slow rolloff
process = fi.lowpass(1, 1000);

// 8-pole: expensive, steep rolloff  
process = fi.lowpass(8, 1000);
```

## Memory Optimization

### Delay Line Size
Always specify maximum delay:
```
// Good: max 1000 samples
process = no.noise : de.delay(1000, 500);

// Bad: unbounded delay
process = no.noise : @(someVariable);
```

### Table Size
Use minimum necessary table sizes:
```
// Good: 4096 entries
wave = rdtable(4096, init, phase);

// Excessive: 1M entries
wave = rdtable(1000000, init, phase);
```

## Anti-Aliasing

### Oversampling
Use `-os` flag for oversampling:
```
// Compile: faust -os mydsp.dsp
```

### Placement of Distortion
Put nonlinearity AFTER oversampling:
```
// Good: oversample -> distort -> downsample
process = _ : si.interpolate(2) : ef.cubicnl : si.decimate(2);

// Bad: distort -> oversample (aliasing)
process = _ : ef.cubicnl : si.interpolate(2);
```

### Filter Before Decimation
Always lowpass before downsampling:
```
ups = 4;
process = _ : si.interpolate(ups) : distortion : fi.lowpass(8, SR/(2*ups)) : si.decimate(ups);
```

## Real-Time Safety

### No Allocations in Audio Path
Faust code is inherently real-time safe, but avoid:
- Large stack allocations
- Dynamic memory in component code

### Avoid Division in Sample Loop
Use multiplication instead:
```
// Slow
process = _ / 2;

// Fast
process = _ * 0.5;
```

### Use Fixed-Point When Possible
For embedded targets, use `-fx` flag:
```
// Compile: faust -fx mydsp.dsp
```

## Compiler Options

### Precision
- `-single` - Single precision (fastest)
- `-double` - Double precision (default)
- `-quad` - Quad precision (slowest, most accurate)
- `-fx` - Fixed-point (for embedded)

### Output
- `-lang cpp` - C++ (default)
- `-lang rust` - Rust
- `-lang llvm` - LLVM IR
- `-lang llvm-vec` - LLVM with vectorization
- `-svg` - Generate block diagram SVG

### Optimization
- `-opt` - Enable optimizations
- `-dirtree` - Share delay lines
- `-mcd n` - Max compile-time delay
- `-os n` - Oversampling factor

### Vectorization
- `-vec` - Enable vectorization
- `-vs` - Vector size (e.g., 4, 8)

## Common Optimizations

### Smoothing Placement
Put expensive transforms BEFORE smoothing:
```
// Good: exp calculated at control rate
freq = hslider("freq", 440, 20, 2000, 1);
mapped = (freq/2000) : exp : si.smoo;

// Bad: exp calculated per sample
freq = hslider("freq", 440, 20, 2000, 1);
mapped = (freq : si.smoo) : exp;
```

### Pre-calculate Constants
Move constants outside of process:
```
SR = ma.SR;  // Sample rate
PI = ma.PI;  // Pi

process = os.osc(2*PI*440/SR);  // Pre-calculated
```

### Use Par for Parallel Processing
Let compiler optimize parallel paths:
```
// Good: compiler sees parallel opportunity
process = no.noise : fi.lowpass(3, 500), fi.highpass(3, 500);

// May not optimize as well
left = no.noise : fi.lowpass(3, 500);
right = no.noise : fi.highpass(3, 500);
```

### Pattern Matching for Conditionals
Use pattern matching instead of select2 when possible:
```
// Less efficient
process = select2(condition, iftrue, iffalse);

// More efficient (may compile to branch)
process = condition * iftrue + (1-condition) * iffalse;
```

## Debugging Performance

### Check Generated Code
Use `-dump` to see generated code:
```
faust -dump mydsp.dsp
```

### Profile in Target
Measure actual CPU in your host/plugin environment.

### Block Diagram
Generate SVG to visualize signal flow:
```
faust -svg mydsp.dsp
```

## Example: Optimized Distortion

```
import("stdfaust.lib");

drive = hslider("drive", 0.5, 0, 1, 0.01) : si.smoo;
tone = hslider("tone", 0.5, 0, 1, 0.01) : si.smoo;
gain = hslider("gain [unit:dB]", 0, -20, 20, 0.1) : ba.db2linear : si.smoo;

// Pre-calculate filter cutoff
cutoff = 2000 + tone * 6000;

// Oversample, process, downsample would go here
// For this example, simple chain:
process = _ 
  : ef.cubicnl(drive, 0.5)  // distortion
  : fi.lowpass(4, cutoff)   // tone control  
  : *(gain);                // output gain
```