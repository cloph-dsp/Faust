# Spectral Blurring Effect

## Overview
The canonical Spectral Blur implementation in this repository is now a C++ processor built around a true overlap-add STFT pipeline.

- Source files: `BLUR/Spectral/spectralblur.h` and `BLUR/Spectral/spectralblur.cpp`
- Status of the Faust version: retired after repeated prototypes produced distortion, ringing, or frequency smearing without the intended temporal suspension

The target sound is the same one described in the SoundMagic manual: spectral change should move more slowly than the source so the output feels suspended, glassy, and ghostly rather than saturated or resonant.

## Algorithm
This implementation uses a conventional short-time Fourier transform with overlap-add resynthesis.

1. Audio is buffered into overlapping frames using a square-root Hann window and a hop size of `fft_size / 4`.
2. Each frame is transformed into the frequency domain with a radix-2 FFT.
3. For bins inside the selected blur range, the processor low-pass filters frame-to-frame spectral change:
   - magnitude is smoothed over time
   - phase advance is smoothed over time
4. The smoothed spectrum is resynthesized with phase-vocoder style phase accumulation.
5. The frame is returned to the time domain with an inverse FFT and overlap-added back into the output stream.

This is the important difference from the discarded Faust attempts: the new processor is actually smoothing spectral evolution from one analysis frame to the next inside a real STFT resynthesis path.

## Implementation Notes
- `blur_amount` still acts as both effect intensity and wet/dry balance, matching the existing control schema.
- `fft_size` now controls the real analysis window size, overlap, latency, and blur character.
- `randomize_phases` adds small, smoothed phase offsets per bin so the result can detach from the source without collapsing into obvious metallic artifacts.
- Bins outside the selected blur range are passed through unblurred inside the wet path so low or high detail can be preserved.
- The dry path is delayed to match the STFT latency before wet/dry mixing, which avoids combing when partial blur is used.

## Sound Character
When tuned correctly, the effect should behave like a spectral afterimage rather than a distortion stage.

- Low blur settings preserve attack and articulation while slightly slowing spectral change.
- Mid blur settings create a floating overlap between past and present frames.
- High blur settings hold harmonics and noise as a suspended spectral cloud.
- Larger FFT sizes produce slower, heavier, more time-dilated blur.
- Smaller FFT sizes stay closer to the source and react faster.

## Public Controls

### Primary
- **blur_amount**: range [0, 100] %
  - 0%: dry only
  - 50%: moderate spectral persistence
  - 100%: maximum blur and full wet mix

### Modulation
- **variance_type**: enum [none, random, lfo, link amplitude, link inverted amplitude]
- **blur_variance**: range [0, 100] %
- **lfo_rate**: range [0.1, 50] Hz

These modulate the effective blur depth from frame to frame without changing the public wet/dry mapping.

### Spectral Range
- **lo_bin_cutoff**: range [0, 100] %
- **hi_bin_cutoff**: range [0, 100] %

These define which positive-frequency bins are blurred.

### Options
- **randomize_phases**: boolean
- **fft_size**: enum [512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]

`fft_size` is now literal. It changes the real STFT frame size and therefore the processor latency.

### Output
- **gain**: range [-40, 40] dB

## Integration
The C++ API is intentionally small.

1. Create `spectralblur::Processor`
2. Call `prepare(sampleRate, maximumBlockSize, numChannels)`
3. Call `setParameters(...)`
4. Process audio with `processBlock(...)`

The processor reports a latency of `fft_size` samples through `getLatencySamples()`.

For quick plugin-side auditioning, there is also a plain iPlug2 test wrapper in `BLUR/Spectral/SpectralBlurTest.h` and `BLUR/Spectral/SpectralBlurTest.cpp`.

## Why The Faust Version Was Dropped
- The earlier filter-bank approximation sounded like resonance or saturation.
- The later Faust FFT experiments still did not create enough temporal suspension.
- A real overlap-add implementation was the correct abstraction for this effect, so the canonical source moved to C++.