# Faust Composition Operators Reference

## Overview

Faust is a block-diagram language at heart. Five composition operators control how signals flow between components.

## Composition Operators

### Parallel Composition: `,`

Duplicates the input to multiple processors. Each processor runs independently.

```
A , B    // A and B receive the same input, outputs are combined
```

**Example**: Stereo effect from mono input
```
process = no.noise : filter, filter;
```

### Sequential Composition: `:`

Connects output of left to input of right. Number of outputs must match number of inputs.

```
A : B    // A's output goes to B's input
```

**Example**: Filtered noise
```
process = no.noise : fi.lowpass(3, 1000);
```

### Split Composition: `<:`

Distributes outputs to inputs. Right side must have multiple of left side's outputs.

```
A <: B, C    // A's output split to B and C
```

**Example**: Mono to stereo
```
process = no.noise <: _, _;
```

### Merge Composition: `:>`

Combines outputs into fewer inputs. Left side outputs must be multiple of right side inputs.

```
A :> B    // A's outputs merged into B (summed)
```

**Example**: Mixing oscillators
```
process = os.osc(220), os.osc(440) :> _;
```

### Recursive Composition: `~`

Creates feedback loops. Implicit 1-sample delay on each feedback path.

```
A ~ B    // A's outputs feed back to B's inputs
```

**Example**: One-pole filter
```
process = 0.5 + ~ * 0.5;
```

## Priority

| Priority | Operator | Associativity |
|----------|----------|---------------|
| 4 | `~` | left |
| 3 | `,` | right |
| 2 | `:` | right |
| 1 | `<:`, `:>` | right |

## Practical Examples

### Effect Chain (Sequential)
```
process = no.noise : fi.lowpass(3, 1000) : fi.highpass(3, 200);
```

### Parallel Filters
```
process = no.noise : fi.lowpass(3, 500), fi.highpass(3, 1000);
```

### Split and Merge (Mid-Side)
```
process = _ <: (+, -) :> (_, _);  // mid-side encoding
```

### Feedback Delay
```
delay = @(1000);
feedback = 0.5;
process = _ : delay : +~*(feedback);
```

### Recursive Oscillator
```
osc = 0.0 ~ +~;
process = osc;
```

## Pattern Matching in Composition

The `case` construct enables pattern matching:

```
duplicate(1, x) = x;
duplicate(n, x) = x, duplicate(n-1, x);
```

This creates parallel compositions dynamically.