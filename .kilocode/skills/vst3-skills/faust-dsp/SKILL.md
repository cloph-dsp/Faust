---
name: faust-dsp
description: Comprehensive Faust DSP language reference for writing, debugging, and optimizing audio effects and synthesizers. Covers syntax, primitives, libraries, composition, UI, patterns, and expert techniques for any Faust question.
user-invocable: true
argument-hint: Faust DSP topic or question
args:
  - name: topic
    description: Specific Faust topic (syntax, filters, oscillators, reverb, UI, optimization, libraries, etc.) or general question
    required: false
  - name: context
    description: Optional context about what you're building or the problem you're solving
    required: false
---

Provide a complete, detailed answer about Faust DSP language. Draw from comprehensive knowledge including syntax, primitives, libraries, composition operators, UI elements, optimization strategies, and real-world patterns.

## How to Use This Skill

Ask any question about Faust, such as:
- How do I create a specific effect or algorithm?
- What does this syntax mean?
- How do I use a particular library or function?
- How do I optimize for CPU or reduce aliasing?
- How do I implement a specific synthesis technique?
- What are best practices for a particular use case?

Provide thorough code examples and explanations.

## Faust DSP Quick Reference

### Basic Structure

```faust
declare name "MyEffect";
declare version "1.0";
declare author "You";
declare license "MIT";

import("stdfaust.lib");

// Definitions
process = your_dsp_chain;
```

### Core Composition Operators

| Operator | Name | Description |
|----------|------|-------------|
| `,` | Parallel | Duplicate input to multiple processors |
| `:` | Sequential | Connect output to input |
| `<:` | Split | Distribute outputs to more inputs |
| `:>` | Merge | Combine outputs into fewer inputs |
| `~` | Recursive | Create feedback loops |

### Primitives

**Numbers**: `42`, `3.14`, `1e-3`
**Boolean**: `true`, `false`

**Arithmetic**: `+`, `-`, `*`, `/`, `^`, `%`
**Comparison**: `<`, `<=`, `==`, `!=`, `>=`, `>`
**Bitwise**: `&`, `|`, `xor`, `<<`, `>>`

**Delay**: `' `@(n)`
- `'` = 1 sample delay
- `@(n)` = n samples delay (must be bounded, non-negative)

**Tables**: `rdtable(size, init, phase)`, `rwtable(size, init, write, read)`
**Selectors**: `select2(cond, iftrue, iffalse)`, `select3(cond, iftrue, ifmid, iffalse)`

### UI Elements

| Primitive | Description |
|-----------|-------------|
| `button("name")` | On/off button (0 or 1) |
| `checkbox("name")` | Toggle checkbox |
| `hslider("name", default, min, max, step)` | Horizontal slider |
| `vslider("name", default, min, max, step)` | Vertical slider |
| `nentry("name", default, min, max, step)` | Number entry |
| `hgroup("name", x)` | Horizontal group |
| `vgroup("name", x)` | Vertical group |
| `tgroup("name", x)` | Tab group |
| `vbargraph("name", min, max, signal)` | Vertical bargraph (output) |
| `hbargraph("name", min, max, signal)` | Horizontal bargraph (output) |
| `attach(x, y)` | Attach UI to signal path |

### Standard Libraries (via stdfaust.lib)

| Prefix | Library | Description |
|--------|---------|-------------|
| `os` | oscillators.lib | Sine, triangle, saw, square, noise |
| `fi` | filters.lib | Lowpass, highpass, bandpass, EQ, comb |
| `en` | envelopes.lib | ADSR, AR, etc. |
| `no` | noises.lib | White, pink, brown noise |
| `dm` | demos.lib | Ready-made effects |
| `de` | delays.lib | Delay lines |
| `re` | reverbs.lib | Reverb algorithms |
| `ma` | maths.lib | Math functions, SR, PI |
| `ba` | basics.lib | Utilities |
| `si` | signals.lib | Signal processing |
| `co` | compressors.lib | Dynamics processing |
| `pf` | phaflangers.lib | Phaser and flanger |
| `pm` | physmodels.lib | Physical models |
| `ve` | vaeffects.lib | Virtual analog effects |
| `ef` | misceffects.lib | Misc effects |
| `an` | analyzers.lib | Analysis tools |
| `ro` | routes.lib | Routing utilities |

### Iteration Constructs

- `par(i, n, expr)` - Duplicate expr n times in parallel
- `seq(i, n, expr)` - Duplicate expr n times in series
- `sum(i, n, expr)` - Sum n expressions
- `prod(i, n, expr)` - Product of n expressions

### Environment Expressions

- `with { ... }` - Local definitions
- `letrec { ... }` - Mutual recursion
- `environment { ... }` - Named environment
- `library("file.lib")` - Load library file
- `component("file.dsp")` - Import DSP file

### Common Patterns

**Smoothing**: `si.smoo` or `si.smooth(tau2pole(time))`
**Conversion**: `ba.db2linear`, `ba.linear2db`
**Frequency**: `ba.midikey2hz`, `ba.hz2midikey`
**Oscillators**: `os.osc(freq)`, `os.triangle(freq)`, `os.sawtooth(freq)`, `os.square(freq)`
**Filters**: `fi.lowpass(order, freq)`, `fi.highpass(order, freq)`, `fi.bandpass(order, freq)`, `fi.resonlp(freq, q, gain)`

### Compiler Options

- `-lang cpp` - C++ output (default)
- `-lang rust` - Rust output
- `-lang llvm` - LLVM output
- `-lang jax` - JAX export for AI training
- `-single` - Single precision
- `-double` - Double precision
- `-quad` - Quad precision
- `-fx` - Fixed-point (embedded/real-time)
- `-svg` - Generate SVG diagram
- `-os` - Oversampling
- `-mcd n` - Max compile-time delay
- `-mdoc` - Mathematical documentation

## Additional Reference Files

For more detailed information, see:
- `reference/composition.md` - Composition operators
- `reference/ui.md` - UI elements and metadata
- `reference/libraries.md` - Complete library reference
- `reference/patterns.md` - Real-world DSP patterns
- `reference/primitives.md` - All primitives
- `reference/optimization.md` - CPU, memory, anti-aliasing
- `reference/midi-osc.md` - MIDI and OSC support
- `reference/debugging.md` - Errors and debugging
- `reference/advanced.md` - Advanced topics, polyphony, deployment
- `reference/ai-ddsp.md` - AI integration (DDSP, RTNeural)

## Answer Format

Provide:
1. Direct answer to the question
2. Working code examples
3. Explanation of how it works
4. Related concepts or alternatives
5. Links to relevant documentation when helpful