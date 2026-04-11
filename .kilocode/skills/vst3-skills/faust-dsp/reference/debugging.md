# Faust Errors and Debugging Reference

## Common Syntax Errors

### Mismatched Inputs/Outputs

**Error**: "X has Y inputs but Z expected"

**Cause**: Composition operator expects matching counts.

```
// Error: os.osc(440) has 0 inputs, fi.lowpass needs 1
process = os.osc(440) : fi.lowpass;

// Fix: os.osc has no implicit input
process = _ : os.osc(440) : fi.lowpass;
```

### Unbounded Delay

**Error**: "Delay must be bounded and non-negative"

**Cause**: Using expression that could be negative.

```
// Error
process = no.noise : @(slider - 10);

// Fix: ensure non-negative
delay = hslider("delay", 10, 1, 100, 1);
process = no.noise : @(delay);
```

### Division by Zero

**Error**: "Division by zero"

**Cause**: Division with constant zero.

```
// Error
process = _ / 0;

// Fix
process = _ / 0.001;
```

### Missing Process

**Error**: "The 'process' definition is missing"

**Cause**: No process defined.

```
// Error: just definitions
freq = 440;

// Fix: add process
freq = 440;
process = os.osc(freq);
```

## Debug Techniques

### Generate SVG Diagram

```
faust -svg mydsp.dsp
```

This produces a visual block diagram of your DSP.

### Dump Intermediate Code

```
faust -dump mydsp.dsp
```

Shows the compilation stages and final C++ code.

### Check Input/Output Counts

```
inputs(process)
outputs(process)
```

Use in Faust code to check.

### Print Values

Use bargraphs for debugging:

```
debug = hslider("debug", 0, -1, 1, 0.01);
process = os.osc(440) : hbargraph("out", -1, 1, debug);
```

## Warning Messages

### Implicit Promotion

"X will be promoted to float"

Usually fine, but may indicate type confusion.

### Unused Definition

"Definition Y is not used in process"

You defined something not connected to process.

### Redefinition

"Definition X redefined"

You defined the same name twice.

## Error Messages by Category

### Composition Errors

- "inputs of A must match outputs of B"
- "number of outputs of A must be multiple of inputs of B"

### Type Errors

- "X expects Y but got Z"
- "cannot apply X to Y inputs"

### Delay Errors

- "delay value must be constant"
- "delay is not bounded"

### Table Errors

- "table size must be > 0"
- "index out of range"

## Debugging Workflow

1. Check SVG output for signal flow
2. Use -dump to see generated code
3. Add hbargraph to inspect signals
4. Simplify and isolate problem
5. Check inputs/outputs with functions

## Example: Debugging Filter Chain

Problem: No sound output

```
process = os.osc(440) : fi.lowpass : fi.highpass;
```

Debug steps:

1. Check outputs exist: `outputs(process)` = 1 - OK
2. Comment out filters one by one
3. Check SVG diagram
4. Add output meter

```
process = os.osc(440) : fi.lowpass : hbargraph("out", -1, 1, _);
```

5. Found issue: filter cutoff too low, add slider

```
cutoff = hslider("cutoff", 1000, 20, 20000, 1);
process = os.osc(440) : fi.lowpass(3, cutoff);
```