# BronzeNoise VST3 smoke suite

`build_smoke.ps1` compiles a small VST3 host against the checked-out Steinberg
SDK, loads the built BronzeNoise binary, and verifies:

- factory creation and 64-bit audio processing;
- finite default impulse response with no right-channel cross-talk;
- wet STFT identity/null behavior and latency-aligned dry behavior;
- reported and measured latency for every FFT size from 256 through 16384;
- all target curves and stereo modes at parameter extremes at 96 kHz;
- finite, ceiling-bounded output under stress.

Run it after the Release VST3 build:

```powershell
./BronzeNoise/tests/build_smoke.ps1
```
