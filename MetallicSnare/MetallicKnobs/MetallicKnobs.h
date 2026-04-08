#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "IPlug_include_in_plug_hdr.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <atomic>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Denormal protection
#ifndef DENORMAL_GUARD
#define DENORMAL_GUARD 1e-15f
#endif

using namespace iplug;
using namespace igraphics;

// Faster and deterministic RNG for IPlug2 DSP
class WhiteNoise {
public:
  WhiteNoise(uint32_t seed = 0x12345678) : mState(seed == 0 ? 0x1 : seed) {}
  
  float Process() {
    mState ^= mState << 13;
    mState ^= mState >> 17;
    mState ^= mState << 5;
    return (static_cast<float>(static_cast<int32_t>(mState)) / 2147483648.0f);
  }
private:
  uint32_t mState;
};

// Generates a 0.5ms DC "thump" followed by noise burst
class SophieImpactPulse {
public:
  void Prepare(double sampleRate) {
    mSampleRate = sampleRate;
    mDcLength = static_cast<int>(0.0005 * sampleRate);
    mCounter = 0;
    mActive = false;
  }
  void Trigger() {
    mCounter = 0;
    mActive = true;
  }
  float Process() {
    if (!mActive) return 0.0f;
    float out = 0.0f;
    if (mCounter < mDcLength) {
      out = 1.0f; // 0.5ms DC Offset for "resident" impact
    } else {
      float burstDecay = 1.0f - std::min(1.0f, (float)(mCounter - mDcLength) / (0.05f * (float)mSampleRate));
      out = mNoise.Process() * burstDecay;
      if (burstDecay <= 0.0f) mActive = false;
    }
    mCounter++;
    return out;
  }
private:
  WhiteNoise mNoise;
  int mDcLength = 0;
  int mCounter = 0;
  double mSampleRate = 44100.0;
  bool mActive = false;
};

// 15ms HPF noise burst for "shattered glass" transients
class NoiseSnap {
public:
  void Prepare(double sampleRate) {
    mSampleRate = sampleRate;
    mSnapLength = static_cast<int>(0.015 * sampleRate);
    mCounter = 0;
    mActive = false;
  }
  void Trigger() {
    mCounter = 0;
    mActive = true;
    mZ1 = 0.0f;
  }
  float Process() {
    if (!mActive) return 0.0f;
    float raw = mNoise.Process();
    // Simple 1-pole HPF (fixed at ~8kHz)
    float hpf = raw - mZ1;
    mZ1 = raw * 0.5f + mZ1 * 0.5f;
    
    float env = 1.0f - (static_cast<float>(mCounter) / static_cast<float>(mSnapLength));
    mCounter++;
    if (mCounter >= mSnapLength) mActive = false;
    return hpf * env;
  }
private:
  WhiteNoise mNoise;
  float mZ1 = 0.0f;
  int mSnapLength = 0, mCounter = 0;
  double mSampleRate = 44100.0;
  bool mActive = false;
};

// Asymmetric Sigmoid Shaper for brutal harmonic density
struct AsymmetricSigmoid {
  static float Process(float in, float drive) {
    float x = in * drive;
    if (x > 0.0f) return std::tanh(x);
    // Exponential "soft-bottom" for asymmetry
    return (std::exp(x) - 1.0f) * 0.5f;
  }
};

// Multi-stage Wavefolder + Aliasing Degrader for "SHATTER"
class ShatterProcessor {
public:
  float Process(float in, float shatter, float envelope) {
    if (shatter <= 0.001f) return in;
    
    // 1. Sine Fold (Buchla-style)
    // Scale the fold drive by the envelope to maintain dynamics
    float foldDrive = 1.0f + (shatter * envelope * 6.0f);
    float out = std::sin(in * 3.14159f * foldDrive);
    
    // 2. Rectification & Bias
    if (shatter > 0.5f) {
      float rectAmt = (shatter - 0.5f) * 2.0f * envelope;
      float rect = std::abs(out);
      out = out * (1.0f - rectAmt) + rect * rectAmt;
    }
    
    // 3. Bit-crush simulation (quantization noise)
    // Dynamic bitcrushing: only crushes at higher amplitudes
    float bitShatter = shatter * envelope;
    float levels = std::pow(2.0f, 16.0f - bitShatter * 12.0f);
    out = std::round(out * levels) / levels;
    
    return out * envelope; // Apply envelope again to ensure total silence during decay
  }
};

// Diffusion chain APF using prime/metallic delay times
class MetallicAPF {
public:
  void Prepare(double sampleRate, float delayMs) {
    int delaySamps = std::max(1, static_cast<int>(delayMs * 0.001 * sampleRate));
    mBuffer.assign(delaySamps, 0.0f);
    mIdx = 0;
  }
  float Process(float in, float g = 0.7f) {
    if (mBuffer.empty()) return in;
    float delayed = mBuffer[mIdx];
    float feedforward = in + (-g * delayed);
    mBuffer[mIdx] = delayed + (g * feedforward);
    if (++mIdx >= mBuffer.size()) mIdx = 0;
    return feedforward;
  }
private:
  std::vector<float> mBuffer;
  size_t mIdx = 0;
};

// ZDF SVF with Tanh Saturation in Feedback Loop
class NonlinearSVF {
public:
  void Prepare(double sampleRate) { mFs = sampleRate; Reset(); }
  void Reset() { s1 = s2 = 0.0f; }
  float Process(float in, float cutoffHz, float Q, float drive = 1.0f) {
    float g = std::tan(static_cast<float>(M_PI) * cutoffHz / static_cast<float>(mFs));
    float k = 1.0f / Q;
    float a1 = 1.0f / (1.0f + g * (g + k));
    float a2 = g * a1;
    float a3 = g * a2;
    float v0 = in;
    float v3 = v0 - s2;
    float v1 = a1 * s1 + a2 * v3;
    float v2 = s2 + a2 * s1 + a3 * v3;
    s1 = 2.0f * v1 - s1;
    s2 = 2.0f * v2 - s2;
    return std::tanh(v2 * drive); 
  }
private:
  float s1 = 0, s2 = 0, mFs = 44100.0f;
};

enum EParams
{
  kShatter = 0,
  kTension,
  kPang,
  kGrit,
  kPunch,
  kWeight,
  kShatterArm,
  kMidiLearn,
  kNumParams
};

class MetallicKnobs final : public Plugin
{
public:
  MetallicKnobs(const InstanceInfo& info);

  void SetHoverParam(int idx) { mHoverParam.store(idx); }
  int GetHoverParam() const { return mHoverParam.load(); }

  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;

private:
  double mSampleRate = 44100.0;
  std::atomic<float> mPeak{0.0f};
  std::atomic<int> mHoverParam{-1};
  
  static constexpr int NUM_MODES = 8;
  const double MODE_RATIOS[NUM_MODES] = {1.0, 1.90, 2.75, 4.02, 6.13, 9.02, 12.57, 17.24};
  const double SHELL_MODES[2] = { 1300.0, 3000.0 };
  
  double mPhases[NUM_MODES] = {};
  double mShellPhases[2] = {};
  double mAmplitudes[NUM_MODES] = {};
  double mShellAmplitudes[2] = {};
  
  SophieImpactPulse mImpact;
  NoiseSnap mSnap;
  ShatterProcessor mShatter;
  MetallicAPF mDiffuser[3];
  NonlinearSVF mFilter;
  
  double mEnvBody = 0.0;
  double mPitchEnv = 0.0;
  
  double mFundamentalFreq = 200.0;
  double mVelocity = 1.0;
};
