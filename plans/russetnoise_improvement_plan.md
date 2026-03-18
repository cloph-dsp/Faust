# RussetNoise DSP Improvement Plan

## Problem Analysis

### Issue 1: Insufficient Intensity at 100% Amount
The current implementation at 100% Amount only applies subtle spectral corrections based on the input signal. The output doesn't truly resemble pure noise (white/pink/brown/russet) with the expected spectral richness and amplitude.

### Issue 2: Auto Makeup Gain Deficiency
At 50% Amount, there's a noticeable volume dip because spectral corrections involve both cuts and boosts that don't balance out. The static makeup gain curve doesn't compensate for the actual gain reduction occurring in the spectral processing.

## Solution Design

### Fix 1: Enhanced Noise Transformation (Lines ~695-710)

**Current approach:**
```cpp
// Blends correction toward target spectrum shape
const float targetBlend = Clip((amountDrive - 0.5f) / 0.5f, 0.f, 1.f);
smoothedCorrectionDb[bandIndex] = Lerp(smoothedCorrectionDb[bandIndex], targetOnlyShape, targetBlend);
```

**New approach:**
```cpp
// At high amounts, progressively replace input with synthesized target noise
// This creates true noise character, not just spectral correction
const float noiseBlend = SmoothStep(0.4f, 1.0f, amountDrive); // 40% to 100%
if (noiseBlend > 0.f) {
    // Calculate what the output would be if it were pure target noise
    const float noiseLevelDb = targetSpectrumDb[bandIndex];
    const float inputLevelDb = currentSpectrumDb[bandIndex];
    
    // Blend toward pure noise level, not just correction
    const float targetOutputDb = Lerp(inputLevelDb, noiseLevelDb, noiseBlend);
    smoothedCorrectionDb[bandIndex] = targetOutputDb - inputLevelDb;
}
```

**Key changes:**
- Start earlier (40% instead of 50%)
- Instead of blending current corrections toward target shape, we directly calculate what the spectrum should be at each frequency
- This creates authentic noise character with the correct spectral envelope

### Fix 2: Dynamic Makeup Gain Compensation

**Current approach:**
```cpp
float ComputeMakeupGain(float amount) {
    float makeupDb = std::pow(amount, 1.2f) * 12.0f;
    return DbToLinear(makeupDb);
}
```

**New approach:**
Calculate actual RMS compensation in ProcessHop:

```cpp
void ProcessHop(int channelCount) {
    ...
    // Calculate actual gain being applied by the spectral corrections
    double appliedGainSum = 0.0;
    int gainCount = 0;
    for (int binIndex = 0; binIndex <= kFFTSize / 2; ++binIndex) {
        if (binIndex > 0) {
            float linearGain = DbToLinear(smoothedCorrectionDb[...]);
            appliedGainSum += linearGain * linearGain;
            gainCount++;
        }
    }
    float avgLinearGain = sqrt(appliedGainSum / gainCount);
    
    // Compensation = inverse of average gain, but ensure minimum 1.0 (no cut)
    float makeupGain = 1.0f / std::max(0.1f, avgLinearGain);
    makeupGain = std::max(1.0f, makeupGain); // Never reduce below unity
    
    // Additional compensation for amount scaling
    makeupGain *= (1.0f + amount * 0.5f);
    ...
}
```

**Key changes:**
- Calculate actual power ratio between input and processed signal
- Ensure makeup gain never goes below 1.0 (unity)
- Scale makeup based on measured gain reduction to maintain consistent loudness

### Implementation Order

1. Fix noise transformation intensity (ProcessHop)
2. Fix auto makeup gain calculation
3. Rebuild and verify
