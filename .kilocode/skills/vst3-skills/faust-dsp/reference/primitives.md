# Faust Primitives Reference

## Numbers

### Integer
```
42, -7, 0
```

### Float
```
3.14, -0.5, 1e-3, 2.5e6
```

### Constants
```
ma.SR    // Sample rate (44100, 48000, etc.)
ma.PI    // 3.14159...
ma.E     // 2.71828...
```

## Arithmetic Primitives

| Primitive | Infix | Description |
|-----------|-------|-------------|
| `+(x)` | `+` | Addition |
| `-(x)` | `-` | Subtraction |
| `*(x)` | `*` | Multiplication |
| `/(x)` | `/` | Division |
| `%(x)` | `%` | Modulo |
| `^(x)` | `^` | Power |

## Comparison Primitives

| Primitive | Infix | Output |
|-----------|-------|--------|
| `<(x)` | `<` | 1 if <, else 0 |
| `<=(x)` | `<=` | 1 if <=, else 0 |
| `==(x)` | `==` | 1 if ==, else 0 |
| `!=(x)` | `!=` | 1 if !=, else 0 |
| `>=(x)` | `>=` | 1 if >=, else 0 |
| `>(x)` | `>` | 1 if >, else 0 |

## Bitwise Primitives

| Primitive | Infix | Description |
|-----------|-------|-------------|
| `&(x)` | `&` | Bitwise AND |
| `|(x)` | `|` | Bitwise OR |
| `xor(x)` | `xor` | Bitwise XOR |
| `<<(x)` | `<<` | Left shift |
| `>>(x)` | `>>` | Right shift |

## Math Primitives (math.h equivalent)

| Primitive | Description |
|-----------|-------------|
| `abs(x)` | Absolute value |
| `acos(x)` | Arc cosine |
| `asin(x)` | Arc sine |
| `atan(x)` | Arc tangent |
| `atan2(y, x)` | Arc tangent 2 |
| `cos(x)` | Cosine |
| `exp(x)` | Exponential |
| `floor(x)` | Floor |
| `log(x)` | Natural log |
| `log10(x)` | Base 10 log |
| `max(a, b)` | Maximum |
| `min(a, b)` | Minimum |
| `pow(x, y)` | Power |
| `remainder(x, y)` | Remainder |
| `rint(x)` | Round to nearest |
| `round(x)` | Round |
| `sin(x)` | Sine |
| `sqrt(x)` | Square root |
| `tan(x)` | Tangent |
| `tanh(x)` | Hyperbolic tangent |

## Delay Primitives

### mem (Memory)
One sample delay, equivalent to `'`:
```
mem = _';
```

### ' (Quote - One Sample Delay)
Delays signal by 1 sample:
```
x'  // Same as @(1)
```

### @ (At - Variable Delay)
Delays signal by n samples:
```
x@(100)  // 100 sample delay
x@(n)    // Variable delay (must be bounded)
```

## Table Primitives

### rdtable (Read Table)
```
rdtable(size, init, phase)
```
- size: table size
- init: initialization function
- phase: read index (0 to size-1)

**Example**: Simple sine lookup
```
size = 4096;
sine(i) = sin(2*ma.PI*i/size);
phase = +(0.5)~%(size);
process = rdtable(size, sine, phase);
```

### rwtable (Read-Write Table)
```
rwtable(size, init, write, read)
```
- size: table size
- init: initialization function
- write: write index and value
- read: read index

**Example**: Simple looper
```
size = 48000;
writePos = button("rec") : +~%(size);
readPos = hslider("speed", 1, 0.1, 2, 0.01) : *(readPos) : int;
process = rwtable(size, 0.0, writePos, readPos);
```

## Selector Primitives

### select2
Two-way conditional:
```
select2(condition, ifTrue, ifFalse)
// Equivalent to: condition * ifTrue + (1-condition) * ifFalse
```

**Example**:
```
bypass = checkbox("bypass");
input = no.noise;
output = dm.zita_light;
process = select2(bypass, output, input);
```

### select3
Three-way conditional:
```
select3(cond, if1, if2, if3)
```

## Identity and Cut

### _ (Identity)
Passes signal through unchanged:
```
process = _ * 0.5;  // Gain
process = _ : fi.lowpass(3, 1000);  // Filter
```

### ! (Cut)
Terminates signal (discard):
```
process = no.noise : (_, !);  // Only right output
```

## Route Primitive

### route
Connect inputs to outputs:
```
route(Ni, No, (i1,o1, i2,o2, ...))
```

**Example**: Swap channels
```
process = route(2, 2, (1,2, 2,1));
```

## Waveform Primitive

Create lookup table as literal:
```
wave = waveform{0, 0.5, 1, 0.5, 0, -0.5, -1, -0.5};
```

## Foreign Functions

Declare external C functions:
```
asinh = ffunction(float asinhf|asinh|asinhl|asinfx(float), <math.h>, "");
```

Format: `ffunction(returnType name|altName1|...|altNameN(paramType), <include>, <extra>)`

## Environment Primitives

### with
Local definitions:
```
process = f : g with { f = os.osc(440); }
```

### letrec
Mutual recursion:
```
counter = v letrec { 'v = v + 1; }
```

### environment
Named environment:
```
const = environment { pi = 3.14; e = 2.71; };
```

### library
Load from file:
```
fl = library("filters.lib");
process = fl.lowpass(1000);
```

### component
Import DSP file:
```
reverb = component("freeverb.dsp");
process = no.noise : reverb;
```

### . (Access)
Access environment member:
```
math = library("maths.lib");
pi = math.PI;
```

## Iteration Primitives

### par
Parallel duplication:
```
par(i, n, expr)
```

**Example**: 4 oscillators
```
process = par(i, 4, os.osc(220*(i+1)));
```

### seq
Sequential duplication:
```
seq(i, n, expr)
```

### sum
Summation:
```
sum(i, n, expr)
```

### prod
Product:
```
prod(i, n, expr)
```

## UI Primitives

See UI reference for detailed documentation.

## Pattern Matching

### case
Match multiple patterns:
```
foo = case {
  0 => 1;
  1 => 2;
  _ => 3;
};
```

### Pattern in Function Definition
```
duplicate(1, x) = x;
duplicate(n, x) = x, duplicate(n-1, x);
```

## Type System

Faust is implicitly typed. The compiler infers:
- Sample rate: `ma.SR`
- Integer vs float based on usage
- Input/output counts

## Implicit Inputs

Functions can have implicit inputs when not all parameters provided:
```
fi.lowpass(3, 1000)  // Has implicit input
os.osc(440)          // Has no implicit input
```

Use `_` to make implicit inputs explicit:
```
process = _ : fi.lowpass(3, 1000);
```