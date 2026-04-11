# Faust DDSP and AI Integration Reference

## Overview

Integrating AI and machine learning with Faust involves two main approaches:
1. **Differentiable DSP (DDSP)** - Native differentiation within Faust
2. **External Neural Networks** - RTNeural or similar C++ libraries

## Differentiable Faust (DDSP)

### Concept

Treat Faust DSP code as a differentiable graph, enabling gradient-based optimization of parameters through backpropagation.

### Architecture

The faust-ddsp library provides differentiable primitives:

| Standard Operation | DDSP Equivalent | Description |
|-------------------|----------------|-------------|
| `+` | `diffAdd` | Addition with gradient propagation |
| `*` | `diffMul` | Scalar multiplication with gradients |
| `hslider` | `diffSlider` | Trainable parameter via backpropagation |
| `abs` | `diffReLU` | Non-linearity for neural layers |

### Basic DDSP Structure

```faust
import("stdfaust.lib");
import("ddsp.lib"); // Experimental DDSP library

// Number of variables to optimize
NVARS = 4;

// Model: Differentiable filter controlled by a network
process = diffInput(NVARS) : diffFilter(NVARS) : diffOutput;
```

## Workflow: Training and Deployment

### 1. Prototyping

Write DSP code in Faust normally.

### 2. Training (Off-line)

Export to JAX backend for training:

```bash
faust -lang jax plugin.dsp
```

This generates a Python function that can be trained using audio datasets.

### 3. Export Weights

Save trained weights to `.json` file or constants in a `.lib` file.

### 4. Compile VST

```bash
faust2faustvst plugin.dsp
# or
faust2juce plugin.dsp
```

## Integration with External Neural Networks (RTNeural)

For complex neural networks (MLP, LSTM) processing audio buffers:

1. **Faust** handles synthesis/filtering
2. **RTNeural** (C++ library) runs the neural network
3. Network dynamically controls Faust parameters in each processBlock

### Best Practices

- Avoid dynamic memory allocations in audio loop
- Use Faust's fixed-point arithmetic (`-fx` flag) for real-time stability
- Pre-load neural network weights before audio processing begins
- Use batch processing for inference efficiency

## Practical Example: Parameter Estimator

To make a VST "imitate" analog hardware using AI:

1. Define hardware structure in Faust (oscillators, saturating filters)
2. Transform code into Differentiable Faust
3. Use spectral loss function (available in faust-ddsp) to compare output with target recording
4. Optimize until Faust sounds identical to target

## Compiler Options for AI

```bash
# JAX export for Python training
faust -lang jax mydsp.dsp

# Fixed-point for embedded/real-time
faust -fx mydsp.dsp

# Generate documentation with mathematical semantics
faust -mdoc mydsp.dsp
```

## Key Points

- The Faust compiler (v2.60+) supports native differentiation
- DDSP allows learning filter coefficients, envelope times, and other parameters
- For large AI models, use external libraries like RTNeural
- Always ensure real-time safety: no allocations in audio callback
- Test with `-single` precision for production if latency matters

## Resources

- Differentiable Programming for Audio Synthesis (Grame talks)
- faust-ddsp library (experimental)
- RTNeural C++ library for neural network inference
- JUCE framework for VST hosting