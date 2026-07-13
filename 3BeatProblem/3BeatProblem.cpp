#include "3BeatProblem.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <cmath>
#include <cstring>

ThreeBeatProblem::ThreeBeatProblem(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, 1)) {
  // Init atomics (std::atomic has no copy, so can't use array initializer)
  for (int c = 0; c < kNumCircles; c++)
    mCurrentStepUIs[c].store(-1);

  // Per-circle params: Steps, Note, Solo, Mute, FillGap, 16 step bools
  const int defaultNotes[kNumCircles] = {60, 64, 67}; // C4, E4, G4
  for (int c = 0; c < kNumCircles; c++) {
    char name[24];
    snprintf(name, sizeof(name), "C%d Steps", c + 1);
    GetParam(StepsParam(c))->InitInt(name, 8, 1, kMaxSteps);
    snprintf(name, sizeof(name), "C%d Note", c + 1);
    GetParam(NoteParam(c))->InitInt(name, defaultNotes[c], 0, 127);
    snprintf(name, sizeof(name), "C%d Solo", c + 1);
    GetParam(SoloParam(c))->InitBool(name, false);
    snprintf(name, sizeof(name), "C%d Mute", c + 1);
    GetParam(MuteParam(c))->InitBool(name, false);
    snprintf(name, sizeof(name), "C%d Fill Gap", c + 1);
    GetParam(FillGapParam(c))->InitInt(name, 1, 1, 5);
    for (int s = 0; s < kMaxSteps; s++) {
      snprintf(name, sizeof(name), "C%d S%d", c + 1, s + 1);
      GetParam(StepParam(c, s))->InitBool(name, false);
    }
  }

  // Global params: Swing, Mutation
  GetParam(kParamSwing)->InitInt("Swing", 0, 0, 100);
  GetParam(kParamMutation)->InitInt("Mutation", 0, 0, 100);

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS,
                        GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* graphics) {
    const IColor bg(255, 15, 20, 23);
    const IColor panel(255, 30, 45, 51);
    const IColor line(255, 54, 81, 95);
    const IColor accent(255, 41, 211, 178);
    const IColor amber(255, 255, 179, 71);
    const IColor textMain(255, 233, 242, 244);
    const IColor textMuted(255, 139, 163, 173);

    const char* uiFont = nullptr;
    if (graphics->LoadFont("UI-Font", "Segoe UI", ETextStyle::Normal)
        || graphics->LoadFont("UI-Font", "Arial", ETextStyle::Normal)
        || graphics->LoadFont("UI-Font", "Verdana", ETextStyle::Normal))
      uiFont = "UI-Font";

    const IVStyle kStyle = DEFAULT_STYLE
      .WithColor(kBG, panel)
      .WithColor(kFG, line)
      .WithColor(kPR, accent)
      .WithColor(kFR, line)
      .WithColor(kHL, accent)
      .WithColor(kX1, textMain)
      .WithLabelText(IText(22.f, textMuted, uiFont, EAlign::Center, EVAlign::Top))
      .WithValueText(IText(24.f, textMain, uiFont, EAlign::Center, EVAlign::Bottom));

    graphics->AttachPanelBackground(bg);

    // Top bar: Swing + Mutation sliders (right-aligned)
    graphics->AttachControl(new IVSliderControl(
      IRECT(PLUG_WIDTH - 340, 18, PLUG_WIDTH - 180, 54),
      kParamSwing, "Swing", kStyle));
    graphics->AttachControl(new IVSliderControl(
      IRECT(PLUG_WIDTH - 170, 18, PLUG_WIDTH - 10, 54),
      kParamMutation, "Mutation", kStyle));

    // Title + tagline
    graphics->AttachControl(new IVLabelControl(
      IRECT(20, 12, 600, 52), "THREE BEAT PROBLEM",
      DEFAULT_STYLE.WithValueText(IText(26.f, accent, uiFont, EAlign::Near, EVAlign::Middle))
                    .WithDrawFrame(false).WithDrawShadows(false)));
    graphics->AttachControl(new IVLabelControl(
      IRECT(20, 52, 800, 84), "host tempo  *  3 independent circles  *  click dots to toggle",
      DEFAULT_STYLE.WithValueText(IText(18.f, textMuted, uiFont, EAlign::Near, EVAlign::Middle))
                    .WithDrawFrame(false).WithDrawShadows(false)));

    // 3 columns: each has a circle on top + Steps/Note knobs below
    const float colW = (PLUG_WIDTH - 40) / kNumCircles;
    const float titleH = 96.0f;
    const float knobSize = 92.0f;
    const float knobGap = 16.0f;

    for (int c = 0; c < kNumCircles; c++) {
      const float x0 = 20 + c * colW;
      const float x1 = x0 + colW - 20;
      const float cx = (x0 + x1) / 2;

      // Circle: fill available width, square aspect
      const float circleD = std::min(colW - 40, 340.0f);
      const float circleY0 = titleH;
      IRECT circleRect(cx - circleD / 2, circleY0, cx + circleD / 2, circleY0 + circleD);

      mCircles[c] = new CircleSteps(circleRect, accent, line, amber, accent);
      mCircles[c]->SetSteps(GetParam(StepsParam(c))->Int());

      // Sync circle pattern from cached params
      bool arr[kMaxSteps];
      for (int s = 0; s < kMaxSteps; s++)
        arr[s] = GetParam(StepParam(c, s))->Value() > 0.5;
      mCircles[c]->SetPattern(arr);

      // Lambda captures circle index c for param sync
      mCircles[c]->SetParamChangeFn([this, c]() {
        bool pat[kMaxSteps];
        mCircles[c]->GetPattern(pat);
        for (int s = 0; s < kMaxSteps; s++) {
          BeginInformHostOfParamChangeFromUI(StepParam(c, s));
          GetParam(StepParam(c, s))->Set(pat[s] ? 1.0 : 0.0);
          EndInformHostOfParamChangeFromUI(StepParam(c, s));
        }
      });
      graphics->AttachControl(mCircles[c]);

      // Two knobs below the circle, centered
      const float knobsY0 = circleY0 + circleD + 16;
      const float knobsW = 2 * knobSize + knobGap;
      const float kx0 = cx - knobsW / 2;
      graphics->AttachControl(new IVKnobControl(
        IRECT(kx0, knobsY0, kx0 + knobSize, knobsY0 + knobSize),
        StepsParam(c), "Steps", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(kx0 + knobSize + knobGap, knobsY0, kx0 + 2 * knobSize + knobGap, knobsY0 + knobSize),
        NoteParam(c), "Note", kStyle));

      // Solo / Mute toggle buttons below the knobs
      const float btnY = knobsY0 + knobSize + 16;
      const float btnW = 60.0f;
      const float btnH = 44.0f;
      const float btnGap = 8.0f;
      const float btnsTotalW = 2 * btnW + btnGap;
      const float bx0 = cx - btnsTotalW / 2;
      graphics->AttachControl(new IVSwitchControl(
        IRECT(bx0, btnY, bx0 + btnW, btnY + btnH),
        SoloParam(c), "S", kStyle.WithValueText(IText(22.f, accent, uiFont, EAlign::Center, EVAlign::Middle))));
      graphics->AttachControl(new IVSwitchControl(
        IRECT(bx0 + btnW + btnGap, btnY, bx0 + 2 * btnW + btnGap, btnY + btnH),
        MuteParam(c), "M", kStyle.WithValueText(IText(22.f, accent, uiFont, EAlign::Center, EVAlign::Middle))));

      // Fill bar (5 gap cells + Fill button) below S/M buttons
      const float fillY = btnY + btnH + 16;
      const float fillH = 36;
      const float fillW = bx0 + 2 * btnW + btnGap - bx0;
      IRECT fillR(bx0, fillY, bx0 + fillW, fillY + fillH);
      mFills[c] = new FillControl(fillR, c, [this, c]() { this->FillCircle(c); });
      graphics->AttachControl(mFills[c]);

      // Rotate L/R buttons below fill bar
      const float rotY = fillY + fillH + 8;
      const float rotW = 60.0f;
      const float rotH = 44.0f;
      const float rotGap = 8.0f;
      const float rotTotalW = 2 * rotW + rotGap;
      const float rotX0 = cx - rotTotalW / 2;
      mRotatesL[c] = new RotateControl(IRECT(rotX0, rotY, rotX0 + rotW, rotY + rotH), c, -1,
        [this, c]() { this->RotateCircle(c, -1); });
      mRotatesR[c] = new RotateControl(IRECT(rotX0 + rotW + rotGap, rotY, rotX0 + 2 * rotW + rotGap, rotY + rotH), c, +1,
        [this, c]() { this->RotateCircle(c, +1); });
      graphics->AttachControl(mRotatesL[c]);
      graphics->AttachControl(mRotatesR[c]);
    }
  };
#endif
}

void ThreeBeatProblem::OnParamChange(int paramIdx) {
#if IPLUG_EDITOR
  // Handle global params first
  if (paramIdx == kParamSwing) {
    mCachedSwing = std::clamp(GetParam(paramIdx)->Int(), 0, 100);
    return;
  }
  if (paramIdx == kParamMutation) {
    mCachedMutation = std::clamp(GetParam(paramIdx)->Int(), 0, 100);
    return;
  }

  if (paramIdx < kParamCircle0Steps || paramIdx >= kNumParams) return;

  int c = (paramIdx - kParamCircle0Steps) / kParamsPerCircle;
  int t = (paramIdx - kParamCircle0Steps) % kParamsPerCircle;
  if (c < 0 || c >= kNumCircles || !mCircles[c]) return;

  if (t == 0) {
    // Steps changed
    mCircles[c]->SetSteps(std::max(1, std::min(kMaxSteps, GetParam(paramIdx)->Int())));
  } else if (t == 20) {
    // FillGap changed — cache it
    mCachedFillGap[c] = std::clamp(GetParam(paramIdx)->Int(), 1, 5);
  } else if (t == 18 || t == 19) {
    // Solo or Mute changed — no UI update needed for circle pattern
  } else if (t >= 2) {
    // Step bool changed — refresh entire pattern for this circle
    bool arr[kMaxSteps];
    for (int s = 0; s < kMaxSteps; s++)
      arr[s] = GetParam(StepParam(c, s))->Value() > 0.5;
    mCircles[c]->SetPattern(arr);
  }
  // t == 1 (Note) — no UI update needed, circle doesn't display note value
#endif
}

void ThreeBeatProblem::OnIdle() {
#if IPLUG_EDITOR
  if (!GetUI()) return;
  for (int c = 0; c < kNumCircles; c++) {
    if (mCircles[c])
      mCircles[c]->SetPlayhead(mCurrentStepUIs[c].load());
  }
#endif
}

#if IPLUG_DSP
void ThreeBeatProblem::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  const int nOut = NOutChansConnected();

  // Zero outputs first (host may give garbage buffers)
  for (int ch = 0; ch < nOut; ch++) {
    if (outputs[ch]) memset(outputs[ch], 0, nFrames * sizeof(sample));
  }

  // Cache all circle params
  for (int c = 0; c < kNumCircles; c++) {
    mCachedSteps[c] = std::clamp(GetParam(StepsParam(c))->Int(), 1, kMaxSteps);
    mCachedNote[c] = std::clamp(GetParam(NoteParam(c))->Int(), 0, 127);
    for (int s = 0; s < kMaxSteps; s++)
      mCachedPatterns[c][s] = GetParam(StepParam(c, s))->Value() > 0.5;
  }

  // Standard DAW solo/mute: if any circle is soloed, only soloed circles play
  bool anySoloed = false;
  for (int cc = 0; cc < kNumCircles; cc++)
    if (GetParam(SoloParam(cc))->Value() > 0.5) { anySoloed = true; break; }
  bool circleActive[kNumCircles];
  for (int c = 0; c < kNumCircles; c++) {
    bool muted  = GetParam(MuteParam(c))->Value() > 0.5;
    bool soloed = GetParam(SoloParam(c))->Value() > 0.5;
    circleActive[c] = !muted && (!anySoloed || soloed);
  }

  const bool transport = mTimeInfo.mTransportIsRunning;
  const double tempo = mTimeInfo.mTempo;

  if (!transport || tempo <= 0.0) {
    // Transport stopped: silence all voices, clear all highlights
    for (int v = 0; v < kMaxVoices; v++) {
      if (mVoices[v].mActive) {
        IMidiMsg msg;
        msg.MakeNoteOffMsg(mVoices[v].mNote, 0);
        SendMidiMsg(msg);
      }
      mVoices[v].mActive = false;
    }
    for (int c = 0; c < kNumCircles; c++) {
      mCurrentSteps[c] = -1;
      mCurrentStepUIs[c].store(-1);
    }
    return;
  }

  const double sr = GetSampleRate();
  const double ppqPerSample = tempo / (60.0 * sr);
  double ppq = mTimeInfo.mPPQPos;

  // Per-circle step tracking state
  double stepPpq[kNumCircles];
  int noteSamples[kNumCircles];
  int prevSteps[kNumCircles];
  for (int c = 0; c < kNumCircles; c++) {
    stepPpq[c] = 4.0 / double(mCachedSteps[c]);
    const double samplesPerBeat = sr * 60.0 / tempo;
    const double stepDur = (samplesPerBeat * 4.0) / double(mCachedSteps[c]);
    noteSamples[c] = std::max(1, (int)(stepDur * 0.95));
    prevSteps[c] = mCurrentSteps[c];
  }

  for (int i = 0; i < nFrames; i++) {
    // Check each circle for step transitions (swing-aware for odd steps)
    for (int c = 0; c < kNumCircles; c++) {
      const double baseStep = std::floor(ppq / stepPpq[c]);
      int curStep = (int)(baseStep) % mCachedSteps[c];
      if (curStep < 0) curStep += mCachedSteps[c];

      // Swing: odd steps fire at grid+swingOffset, even steps on grid
      const double swingOffset = (mCachedSwing / 100.0) * stepPpq[c] * 0.5;
      const double basePpq = baseStep * stepPpq[c];
      const double oddStepBase = basePpq + stepPpq[c];
      const double firedPpq = oddStepBase - swingOffset;

      bool shouldFire = false;
      if (curStep % 2 == 0) {
        shouldFire = (curStep != prevSteps[c]);
      } else {
        shouldFire = (curStep != prevSteps[c]) && (ppq >= firedPpq);
      }

      if (shouldFire) {
        if (circleActive[c] && mCachedPatterns[c][curStep]) {
          // Find a free voice slot
          int slot = -1;
          for (int v = 0; v < kMaxVoices; v++) {
            if (!mVoices[v].mActive) { slot = v; break; }
          }
          if (slot < 0) {
            // Steal oldest — send note-off first to prevent stuck MIDI
            slot = 0;
            IMidiMsg msgOff;
            msgOff.MakeNoteOffMsg(mVoices[slot].mNote, i);
            SendMidiMsg(msgOff);
          }
          mVoices[slot].mActive = true;
          mVoices[slot].mNote = mCachedNote[c];
          mVoices[slot].mPhase = 0.0;
          mVoices[slot].mSamplesLeft = noteSamples[c];
          mVoices[slot].mTotalSamples = noteSamples[c];

          // Mutation: randomize velocity
          int velocity = 100;
          if (mCachedMutation > 0) {
            double range = (double(rand()) / RAND_MAX * 2.0 - 1.0) * (mCachedMutation / 100.0) * 35.0;
            velocity = std::clamp((int)(100.0 + range), 1, 127);
          }
          IMidiMsg msgOn;
          msgOn.MakeNoteOnMsg(mCachedNote[c], velocity, i);
          SendMidiMsg(msgOn);
        }
        mCurrentSteps[c] = curStep;
        // UI playhead: odd steps show at swing-delayed time, even on grid
        if (circleActive[c]) {
          if (curStep % 2 == 1) {
            if (ppq >= firedPpq) mCurrentStepUIs[c].store(curStep);
          } else {
            const double evenBoundary = basePpq;
            if (ppq >= evenBoundary) mCurrentStepUIs[c].store(curStep);
          }
        } else {
          mCurrentStepUIs[c].store(-1); // dim out inactive circles
        }
        prevSteps[c] = curStep;
      }
    }

    // Render voices ONCE per sample, sum to mono mix
    double mix = 0.0;
    for (int v = 0; v < kMaxVoices; v++) {
      if (!mVoices[v].mActive) continue;
      Voice& vc = mVoices[v];
      const double freq = 440.0 * std::pow(2.0, (double(vc.mNote) - 69.0) / 12.0);
      const double w = 2.0 * 3.141592653589793 * freq / sr;
      const double env = double(vc.mSamplesLeft) / double(vc.mTotalSamples);
      mix += std::sin(vc.mPhase) * env * 0.3;
      vc.mPhase += w;
      if (vc.mPhase > 6.283185307179586) vc.mPhase -= 6.283185307179586;
      if (--vc.mSamplesLeft <= 0) {
        vc.mActive = false;
        IMidiMsg msgOff;
        msgOff.MakeNoteOffMsg(vc.mNote, i);
        SendMidiMsg(msgOff);
      }
    }

    // Write same mix to both output channels
    for (int ch = 0; ch < nOut && ch < 2; ch++) {
      if (outputs[ch]) outputs[ch][i] = (sample)mix;
    }

    ppq += ppqPerSample;
  }
}

void ThreeBeatProblem::OnReset() {
  for (int c = 0; c < kNumCircles; c++) {
    mCurrentSteps[c] = -1;
    mCurrentStepUIs[c].store(-1);
  }
  for (int v = 0; v < kMaxVoices; v++) {
    mVoices[v].mActive = false;
    mVoices[v].mNote = 60;
    mVoices[v].mPhase = 0.0;
    mVoices[v].mSamplesLeft = 0;
    mVoices[v].mTotalSamples = 1;
  }
}

void ThreeBeatProblem::FillCircle(int c) {
  const int gapVals[5] = {1, 2, 3, 4, 8};
  int gapIdx = mCachedFillGap[c] - 1;
  int gap = gapVals[gapIdx >= 0 && gapIdx < 5 ? gapIdx : 0];
  bool arr[kMaxSteps];
  for (int s = 0; s < kMaxSteps; s++)
    arr[s] = (s % gap == 0);
  for (int s = 0; s < kMaxSteps; s++) {
    BeginInformHostOfParamChangeFromUI(StepParam(c, s));
    GetParam(StepParam(c, s))->Set(arr[s] ? 1.0 : 0.0);
    EndInformHostOfParamChangeFromUI(StepParam(c, s));
  }
  if (mCircles[c]) mCircles[c]->SetPattern(arr);
}

void ThreeBeatProblem::RotateCircle(int c, int direction) {
  bool arr[kMaxSteps];
  mCircles[c]->GetPattern(arr);
  if (direction < 0) {
    bool first = arr[0];
    for (int s = 0; s < kMaxSteps - 1; s++) arr[s] = arr[s + 1];
    arr[kMaxSteps - 1] = first;
  } else {
    bool last = arr[kMaxSteps - 1];
    for (int s = kMaxSteps - 1; s > 0; s--) arr[s] = arr[s - 1];
    arr[0] = last;
  }
  for (int s = 0; s < kMaxSteps; s++) {
    BeginInformHostOfParamChangeFromUI(StepParam(c, s));
    GetParam(StepParam(c, s))->Set(arr[s] ? 1.0 : 0.0);
    EndInformHostOfParamChangeFromUI(StepParam(c, s));
  }
  if (mCircles[c]) mCircles[c]->SetPattern(arr);
}
#endif
