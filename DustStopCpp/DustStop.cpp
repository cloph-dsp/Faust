#include "DustStop.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <algorithm>
#include <cmath>

namespace
{
  constexpr float kPi = 3.14159265358979323846f;

  template <typename T>
  T Clamp(T val, T lo, T hi)
  {
    return std::min(hi, std::max(lo, val));
  }

  float SmootherCoeff(double sr, double timeMs)
  {
    return static_cast<float>(1.0 - std::exp(-1.0 / (0.001 * std::max(0.1, timeMs) * sr)));
  }

  float FlushDenorm(float x)
  {
    return (std::abs(x) < 1.0e-18f) ? 1.0e-18f : x;
  }
}

DustStop::DustStop(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kTrigger)->InitBool("Trigger", false, "Trigger", 0,
    "", "Tap to stop tape/reboot. Check=start/restart.");

  GetParam(kTimeDiv)->InitEnum("Time", 2,
    {"1/4", "1/2", "1/1", "2/1", "4/1", "8/1"});

  GetParam(kDust)->InitPercentage("Dust", 50.0);
  GetParam(kDamping)->InitPercentage("Damping", 50.0);

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]()
  {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* graphics)
  {
    const char* uiFont = nullptr;

    if (graphics->LoadFont("DustStop-Font", "Segoe UI", ETextStyle::Normal)
        || graphics->LoadFont("DustStop-Font", "Arial", ETextStyle::Normal)
        || graphics->LoadFont("DustStop-Font", "Verdana", ETextStyle::Normal))
    {
      uiFont = "DustStop-Font";
    }

    graphics->AttachPanelBackground(IColor(255, 14, 15, 18));
    graphics->EnableMouseOver(true);
    graphics->EnableTooltips(true);
    graphics->AttachBubbleControl();
    graphics->AssignParamNameToolTips();

    const IVStyle style = DEFAULT_STYLE
      .WithColor(kBG, IColor(255, 24, 27, 33))
      .WithColor(kFG, IColor(255, 92, 99, 112))
      .WithColor(kPR, IColor(255, 219, 123, 73))
      .WithColor(kFR, IColor(255, 72, 80, 90))
      .WithLabelText(IText(14.f, IColor(255, 158, 164, 171), uiFont))
      .WithValueText(IText(16.f, IColor(255, 243, 239, 232), uiFont));

    const IRECT b = graphics->GetBounds().GetPadded(-20.f);

    graphics->AttachControl(new ITextControl(b.GetFromTop(48.f),
      "DUSTSTOP", IText(28.f, IColor(255, 219, 123, 73), uiFont)));

    const IRECT toggleRect(b.L, 100.f, b.L + 160.f, 150.f);
    graphics->AttachControl(new IVToggleControl(toggleRect, kTrigger, "TRIGGER",
      DEFAULT_STYLE.WithColor(kFG, IColor(255, 72, 80, 90))
        .WithLabelText(IText(14.f, IColor(255, 158, 164, 171), uiFont))
        .WithValueText(IText(16.f, IColor(255, 243, 239, 232), uiFont))));

    const float knobSize = 96.f;
    const float knobY = 160.f;
    const float gap = 24.f;
    const float totalW = knobSize * 3.f + gap * 2.f;
    const float left = b.MW() - totalW * 0.5f;

    IRECT kR(left, knobY, left + knobSize, knobY + knobSize);
    graphics->AttachControl(new IVKnobControl(kR, kDust, "Dust", style));

    kR.Translate(knobSize + gap, 0.f);
    graphics->AttachControl(new IVKnobControl(kR, kDamping, "Damping", style));

    kR.Translate(knobSize + gap, 0.f);
    graphics->AttachControl(new IVKnobControl(kR, kTimeDiv, "Time", style));
  };
#endif

#if IPLUG_DSP
  OnReset();
#endif
}

#if IPLUG_DSP

void DustStop::OnReset()
{
  mSampleRate = std::max(1.0, GetSampleRate());
  mElapsed = 0;
  mPhase = Phase::kIdle;
  mWritePos = 0;
  mLastTrigger = false;
  mBootElapsed = 0;

  for (auto& buf : mDelayBuf)
    buf.assign(kBufSize, 0.f);

  for (int ci = 0; ci < kNumCombs; ++ci)
    for (int ch = 0; ch < kMaxChannels; ++ch)
      std::fill_n(mCombs[ci][ch], kCombBufSize, 0.f);

  for (int ci = 0; ci < kNumCombs; ++ci)
    mCombIOTAs[ci] = 0;

  mNoiseSmL = mNoiseSmR = 0.f;
  mFastNoiseL = mFastNoiseR = 0.f;
  mSmDust = 0.5f;
  mSmDamping = 0.5f;
  mDustSt = mDampingSt = 0.f;
  mRng1 = 111; mRng2 = 222; mRng3 = 333;

  mHpL = mHpPrevL = mHpR = mHpPrevR = 0.f;
  mSubPhase = 0.f;

  mDurSamps = 0;
  mBootDurSamps = 0;
  mTailSamps = static_cast<int>(mSampleRate * 3.0);
  mTotalSamps = 0;
}

uint32_t DustStop::XorShift32(uint32_t& state)
{
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

float DustStop::FastNoiseLP(uint32_t& rng, float& state, float alpha)
{
  float raw = static_cast<float>(XorShift32(rng) & 0x00FFFFFFu) / 16777215.0f;
  state = raw * alpha + state * (1.0f - alpha);
  return state;
}

float DustStop::SlowNoiseLP(uint32_t& rng, float& state, float alpha)
{
  float raw = static_cast<float>(XorShift32(rng) & 0x00FFFFFFu) / 16777215.0f;
  state = raw * alpha + state * (1.0f - alpha);
  return state;
}

float DustStop::OnePole(float in, float& st, float alpha)
{
  st = in * alpha + st * (1.0f - alpha);
  return FlushDenorm(st);
}

void DustStop::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (nFrames <= 0 || !outputs)
    return;

  const int nCh = std::min(kMaxChannels,
    std::max(1, std::max(NInChansConnected(), NOutChansConnected())));

  double hostBPM = GetTempo();
  double bpm = (hostBPM > 0.0) ? hostBPM : 120.0;

  const int timeIdx = static_cast<int>(GetParam(kTimeDiv)->Value());
  const double beats = kTimeVals[Clamp(timeIdx, 0, 5)];
  const double durSecs = (60.0 / bpm) * beats;

  mDurSamps = std::max(1, static_cast<int>(durSecs * mSampleRate));
  mTailSamps = static_cast<int>(mSampleRate * 3.0);
  mTotalSamps = mDurSamps + mTailSamps;

  bool trigger = GetParam(kTrigger)->Value() >= 0.5;
  float dustTarget = static_cast<float>(GetParam(kDust)->Value() * 0.01);
  float dampTarget = static_cast<float>(GetParam(kDamping)->Value() * 0.01);

  float dustAlpha = SmootherCoeff(mSampleRate, 12.0);
  float dampAlpha = SmootherCoeff(mSampleRate, 12.0);
  float noiseAlphaSlow = SmootherCoeff(mSampleRate, 25.0);
  float noiseAlphaFast = 0.01f;

  for (int s = 0; s < nFrames; ++s)
  {
    mSmDust = OnePole(dustTarget, mDustSt, dustAlpha);
    mSmDamping = OnePole(dampTarget, mDampingSt, dampAlpha);

    // --- Trigger edge detection ---
    bool rising = trigger && !mLastTrigger;
    bool falling = !trigger && mLastTrigger;
    mLastTrigger = trigger;

    if (rising)
    {
      // Any rising edge restarts the effect from scratch
      mPhase = Phase::kTapeStop;
      mElapsed = 0;
      mBootElapsed = 0;
    }
    else if (falling)
    {
      // Falling edge during effect or done → enter reboot
      if (mPhase != Phase::kIdle && mPhase != Phase::kReboot)
      {
        mPhase = Phase::kReboot;
        mBootElapsed = 0;
        mBootDurSamps = mDurSamps; // reboot takes same duration as tape stop
      }
    }

    // --- Write inputs to delay buffer (always, even during reboot) ---
    float dry[kMaxChannels]{};
    for (int c = 0; c < nCh; ++c)
    {
      float in = (inputs && inputs[c]) ? static_cast<float>(inputs[c][s]) : 0.f;
      dry[c] = Clamp(in, -1.f, 1.f);
      mDelayBuf[static_cast<size_t>(c)][mWritePos & kBufMask] = dry[c];
    }
    if (nCh == 1)
      mDelayBuf[1][mWritePos & kBufMask] = dry[0];

    mWritePos = (mWritePos + 1) & kBufMask;

    // --- Compute effect output ---
    float effectOut[kMaxChannels] = {0.f, 0.f};
    float prog = 0.f;
    float dprog = 0.f;

    if (mPhase == Phase::kTapeStop || mPhase == Phase::kDustTail)
    {
      prog = Clamp(static_cast<float>(mElapsed) / static_cast<float>(mDurSamps), 0.f, 1.f);
      dprog = Clamp(static_cast<float>(mElapsed - mDurSamps) / static_cast<float>(mTailSamps), 0.f, 1.f);

      // --- Tape delay read ---
      int maxDelay = static_cast<int>(static_cast<float>(mDurSamps) * 0.666667f);
      int delaySamps = (mPhase == Phase::kDustTail)
        ? maxDelay
        : std::max(1, static_cast<int>(std::pow(prog, 1.5f) * static_cast<float>(mDurSamps) * 0.666667f));

      uint32_t rd = (mWritePos - static_cast<uint32_t>(delaySamps) + kBufSize) & kBufMask;

      // --- Noise for comb excitation and grain ---
      float noiseSlowL = SlowNoiseLP(mRng2, mNoiseSmL, noiseAlphaSlow) * 2.0f - 1.0f;
      float noiseSlowR = SlowNoiseLP(mRng2, mNoiseSmR, noiseAlphaSlow) * 2.0f - 1.0f;
      float noiseFastL = FastNoiseLP(mRng3, mFastNoiseL, noiseAlphaFast);
      float noiseFastR = FastNoiseLP(mRng3, mFastNoiseR, noiseAlphaFast);

      // --- Read from delay buffer ---
      float bufOut[kMaxChannels];
      for (int c = 0; c < nCh; ++c)
        bufOut[c] = mDelayBuf[static_cast<size_t>(c)][rd];
      if (nCh == 1) bufOut[1] = bufOut[0];

      // --- Harmonic comb bank ---
      // 5 parallel combs at D4/A4/D5/G5/D6 form a rich "million pieces" texture
      float noiseMix = dprog * mSmDust * 0.5f;

      // Dust scales all feedbacks; higher combs ring more aggressively
      float dustBoost = mSmDust * 0.3f + 0.5f; // 0.5 → 0.8
      float fbDecay = (mPhase == Phase::kDustTail)
        ? (1.0f - dprog * 0.5f) : 1.0f;

      float chimeAccum[kMaxChannels] = {0.f, 0.f};

      for (int ci = 0; ci < kNumCombs; ++ci)
      {
        float fb = kCombFBBase[ci] * dustBoost * fbDecay;
        int iota = mCombIOTAs[ci];
        int writeIdx = iota & kCombMask;
        int readIdx = (iota - kCombDelays[ci] - 1) & kCombMask;

        for (int c = 0; c < nCh; ++c)
        {
          float noiseExciter = (c == 0) ? noiseSlowL : noiseSlowR;
          float exciter = bufOut[c] + noiseMix * noiseExciter;
          mCombs[ci][c][writeIdx] = exciter + fb * mCombs[ci][c][readIdx];
          chimeAccum[c] += mCombs[ci][c][writeIdx];
        }
        mCombIOTAs[ci]++;
      }

      // Normalize comb sum
      float invNum = 1.0f / static_cast<float>(kNumCombs);
      for (int c = 0; c < nCh; ++c)
        effectOut[c] = chimeAccum[c] * invNum;

      // --- Granular amplitude modulation ---
      float grainModL = (mPhase == Phase::kDustTail)
        ? noiseFastL * 0.6f + mSmDust * 0.4f : 1.0f;
      float grainModR = (mPhase == Phase::kDustTail)
        ? noiseFastR * 0.6f + mSmDust * 0.4f : 1.0f;

      // --- Envelope ---
      float baseFade = 1.0f - prog * 0.3f;
      float darkening = 1.0f - prog * mSmDamping * 0.35f;
      float dustFade = 1.0f - std::pow(dprog, 1.5f);
      if (dustFade < 0.f) dustFade = 0.f;

      float envL = baseFade * darkening * dustFade * grainModL;
      float envR = baseFade * darkening * dustFade * grainModR;

      effectOut[0] *= envL;
      effectOut[1] *= envR;

      // --- Sub oscillator ---
      mSubPhase += 45.0f / static_cast<float>(mSampleRate);
      if (mSubPhase >= 1.0f) mSubPhase -= 1.0f;
      float subSig = std::sin(2.0f * kPi * mSubPhase)
        * prog * (1.0f - mSmDamping * 0.7f) * 0.08f;
      effectOut[0] += subSig;
      effectOut[1] += subSig;

      // --- Advance state ---
      mElapsed++;
      if (mPhase == Phase::kTapeStop && mElapsed >= mDurSamps)
        mPhase = Phase::kDustTail;
      if (mElapsed >= mTotalSamps)
        mPhase = Phase::kDone;
    }
    else if (mPhase == Phase::kReboot)
    {
      // Reboot: effect output continues but fades, dry crossfades in
      // The comb keeps ringing (feedback naturally decays) while we
      // crossfade the output to dry over ~1.5 seconds
      mBootElapsed++;
      float bootProg = Clamp(static_cast<float>(mBootElapsed) / static_cast<float>(mBootDurSamps), 0.f, 1.f);

      // Cosine S-curve: starts at 1 (full effect), ends at 0 (full dry)
      float bootFade = 0.5f * (1.0f + std::cos(bootProg * kPi));

      // During reboot, continue the comb processing so it rings naturally
      // but with progressively less feedback
      prog = Clamp(static_cast<float>(mElapsed) / static_cast<float>(mDurSamps), 0.f, 1.f);
      dprog = Clamp(static_cast<float>(mElapsed - mDurSamps) / static_cast<float>(mTailSamps), 0.f, 1.f);

      int maxDelay = static_cast<int>(static_cast<float>(mDurSamps) * 0.666667f);
      int delaySamps = (mElapsed >= mDurSamps)
        ? maxDelay
        : std::max(1, static_cast<int>(std::pow(prog, 1.5f) * static_cast<float>(mDurSamps) * 0.666667f));

      uint32_t rd = (mWritePos - static_cast<uint32_t>(delaySamps) + kBufSize) & kBufMask;

      float noiseSlowL = SlowNoiseLP(mRng2, mNoiseSmL, noiseAlphaSlow) * 2.0f - 1.0f;
      float noiseSlowR = SlowNoiseLP(mRng2, mNoiseSmR, noiseAlphaSlow) * 2.0f - 1.0f;
      float noiseFastL = FastNoiseLP(mRng3, mFastNoiseL, noiseAlphaFast);
      float noiseFastR = FastNoiseLP(mRng3, mFastNoiseR, noiseAlphaFast);

      float bufOut[kMaxChannels];
      for (int c = 0; c < nCh; ++c)
        bufOut[c] = mDelayBuf[static_cast<size_t>(c)][rd];
      if (nCh == 1) bufOut[1] = bufOut[0];

      float noiseMix = dprog * mSmDust * 0.5f;
      float dustBoost = mSmDust * 0.3f + 0.5f;
      float fbDecay = (dprog > 0.f) ? (1.0f - dprog * 0.5f) : 1.0f;

      // During reboot, reduce comb feedback by bootFade so the ring dies
      float bootFbScale = 0.3f + 0.7f * bootFade; // 1.0 → 0.3 over boot

      float chimeAccum[kMaxChannels] = {0.f, 0.f};
      for (int ci = 0; ci < kNumCombs; ++ci)
      {
        float fb = kCombFBBase[ci] * dustBoost * fbDecay * bootFbScale;
        int iota = mCombIOTAs[ci];
        int writeIdx = iota & kCombMask;
        int readIdx = (iota - kCombDelays[ci] - 1) & kCombMask;

        for (int c = 0; c < nCh; ++c)
        {
          float noiseExciter = (c == 0) ? noiseSlowL : noiseSlowR;
          float exciter = bufOut[c] + noiseMix * noiseExciter;
          mCombs[ci][c][writeIdx] = exciter + fb * mCombs[ci][c][readIdx];
          chimeAccum[c] += mCombs[ci][c][writeIdx];
        }
        mCombIOTAs[ci]++;
      }

      float invNum = 1.0f / static_cast<float>(kNumCombs);
      for (int c = 0; c < nCh; ++c)
        effectOut[c] = chimeAccum[c] * invNum;

      float grainModL = (dprog > 0.f) ? noiseFastL * 0.6f + mSmDust * 0.4f : 1.0f;
      float grainModR = (dprog > 0.f) ? noiseFastR * 0.6f + mSmDust * 0.4f : 1.0f;

      float baseFade = 1.0f - prog * 0.3f;
      float darkening = 1.0f - prog * mSmDamping * 0.35f;
      float dustFade = 1.0f - std::pow(dprog, 1.5f);
      if (dustFade < 0.f) dustFade = 0.f;

      float envL = baseFade * darkening * dustFade * grainModL;
      float envR = baseFade * darkening * dustFade * grainModR;

      effectOut[0] *= envL * bootFade;
      effectOut[1] *= envR * bootFade;

      mSubPhase += 45.0f / static_cast<float>(mSampleRate);
      if (mSubPhase >= 1.0f) mSubPhase -= 1.0f;
      float subSig = std::sin(2.0f * kPi * mSubPhase)
        * prog * (1.0f - mSmDamping * 0.7f) * 0.08f * bootFade;
      effectOut[0] += subSig;
      effectOut[1] += subSig;

      // Crossfade to dry
      for (int c = 0; c < nCh; ++c)
        effectOut[c] += dry[c] * (1.0f - bootFade);

      mElapsed++;

      if (bootProg >= 1.0f)
        mPhase = Phase::kIdle;
    }

    // --- Output routing ---
    if (mPhase == Phase::kIdle)
    {
      // Dry passthrough
      outputs[0][s] = static_cast<sample>(Clamp(dry[0], -1.f, 1.f));
      outputs[1][s] = static_cast<sample>(Clamp(dry[1], -1.f, 1.f));
    }
    else if (mPhase == Phase::kDone)
    {
      // Silence after completion
      outputs[0][s] = static_cast<sample>(0);
      outputs[1][s] = static_cast<sample>(0);
    }
    else
    {
      // kTapeStop, kDustTail, kReboot → processed output through DC blocker
      float hpOutL = effectOut[0] - mHpPrevL + 0.9995f * mHpL;
      mHpPrevL = effectOut[0];
      mHpL = hpOutL;
      outputs[0][s] = static_cast<sample>(Clamp(hpOutL * 0.95f, -1.f, 1.f));

      float hpOutR = effectOut[1] - mHpPrevR + 0.9995f * mHpR;
      mHpPrevR = effectOut[1];
      mHpR = hpOutR;
      outputs[1][s] = static_cast<sample>(Clamp(hpOutR * 0.95f, -1.f, 1.f));
    }
  }
}

#endif
