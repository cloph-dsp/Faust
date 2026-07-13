#include "3BeatProblem.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <cmath>
#include <cstring>

ThreeBeatProblem::ThreeBeatProblem(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, 1)) {
  GetParam(kParamSteps)->InitInt("Steps", 8, 1, kMaxSteps);
  GetParam(kParamNote)->InitInt("Note", 60, 0, 127);
  for (int s = 0; s < kMaxSteps; s++) {
    char name[16];
    snprintf(name, sizeof(name), "S%d", s + 1);
    GetParam(kParamStep0 + s)->InitBool(name, false);
  }

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
      .WithLabelText(IText(11.f, textMuted, uiFont, EAlign::Center, EVAlign::Top))
      .WithValueText(IText(12.f, textMain, uiFont, EAlign::Center, EVAlign::Bottom));

    graphics->AttachPanelBackground(bg);

    // Title + tagline
    graphics->AttachControl(new IVLabelControl(
      IRECT(10, 6, 280, 26), "THREE BEAT PROBLEM",
      DEFAULT_STYLE.WithValueText(IText(13.f, accent, uiFont, EAlign::Near, EVAlign::Middle))
                    .WithDrawFrame(false).WithDrawShadows(false)));
    graphics->AttachControl(new IVLabelControl(
      IRECT(10, 26, 390, 42), "host tempo  *  click dots to toggle",
      DEFAULT_STYLE.WithValueText(IText(9.f, textMuted, uiFont, EAlign::Near, EVAlign::Middle))
                    .WithDrawFrame(false).WithDrawShadows(false)));

    // Two knobs right-aligned
    graphics->AttachControl(new IVKnobControl(
      IRECT(305, 6, 345, 46), kParamSteps, "Steps", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(350, 6, PLUG_WIDTH - 8, 46), kParamNote, "Note", kStyle));

    // Center: one circle
    mCircle = new CircleSteps(
      IRECT(20, 52, PLUG_WIDTH - 20, PLUG_WIDTH - 28),
      accent, line, amber, accent);
    mCircle->SetSteps(GetParam(kParamSteps)->Int());
    mCircle->SetPattern(mCachedStepsArr);
    mCircle->SetParamChangeFn([this]() {
      // Sync iPlug params from the circle's pattern so host automation sees clicks
      bool arr[kMaxSteps];
      mCircle->GetPattern(arr);
      for (int s = 0; s < kMaxSteps; s++) {
        BeginInformHostOfParamChangeFromUI(kParamStep0 + s);
        GetParam(kParamStep0 + s)->Set(arr[s] ? 1.0 : 0.0);
        EndInformHostOfParamChangeFromUI(kParamStep0 + s);
      }
    });
    graphics->AttachControl(mCircle);
  };
#endif
}

void ThreeBeatProblem::OnParamChange(int paramIdx) {
#if IPLUG_EDITOR
  if (paramIdx == kParamSteps && mCircle) {
    mCircle->SetSteps(std::max(1, std::min(kMaxSteps, GetParam(kParamSteps)->Int())));
  } else if (paramIdx >= kParamStep0 && paramIdx < kParamStep0 + kMaxSteps && mCircle) {
    bool arr[kMaxSteps];
    for (int s = 0; s < kMaxSteps; s++) {
      arr[s] = GetParam(kParamStep0 + s)->Value() > 0.5;
    }
    mCircle->SetPattern(arr);
  }
#endif
}

void ThreeBeatProblem::OnIdle() {
#if IPLUG_EDITOR
  if (GetUI() && mCircle) {
    mCircle->SetPlayhead(mCurrentStepUI.load());
  }
#endif
}

#if IPLUG_DSP
void ThreeBeatProblem::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  const int nOut = NOutChansConnected();

  // Always zero outputs first (defensive: host may give us garbage buffers)
  for (int ch = 0; ch < nOut; ch++) {
    if (outputs[ch]) memset(outputs[ch], 0, nFrames * sizeof(sample));
  }

  // Cache params
  mCachedSteps = std::clamp(GetParam(kParamSteps)->Int(), 1, kMaxSteps);
  mCachedNote = std::clamp(GetParam(kParamNote)->Int(), 0, 127);
  for (int s = 0; s < kMaxSteps; s++) {
    mCachedStepsArr[s] = GetParam(kParamStep0 + s)->Value() > 0.5;
  }

  const bool transport = mTimeInfo.mTransportIsRunning;
  const double tempo = mTimeInfo.mTempo;

  if (!transport || tempo <= 0.0) {
    // Transport stopped: silence voices, clear highlight, mark step invalid so step 0 retriggers on next play
    for (int v = 0; v < kMaxVoices; v++) {
      if (mVoices[v].mActive) {
        IMidiMsg msg;
        msg.MakeNoteOffMsg(mVoices[v].mNote, 0);
        SendMidiMsg(msg);
      }
      mVoices[v].mActive = false;
      mVoices[v].mEnv = 0.0;
    }
    mCurrentStep = -1;
    mCurrentStepUI.store(-1);
    return;
  }

  const double sr = GetSampleRate();
  // Step duration derived from host tempo. 1 bar = 4 quarters; stepDur = 4 quarters / numSteps.
  const double samplesPerBeat = sr * 60.0 / tempo;
  const double stepDur = (samplesPerBeat * 4.0) / double(mCachedSteps);
  const int noteSamples = std::max(1, (int)(stepDur * 0.95));

  // PPQ-driven step lookup (sample-accurate transport sync, no drift)
  const double stepPpq = 4.0 / double(mCachedSteps);   // quarter notes per step
  const double ppqPerSample = tempo / (60.0 * sr);
  double ppq = mTimeInfo.mPPQPos;

  int prevStep = mCurrentStep;

  for (int i = 0; i < nFrames; i++) {
    // Compute current step from PPQ (negative-safe modulo)
    int curStep = (int)std::floor(ppq / stepPpq) % mCachedSteps;
    if (curStep < 0) curStep += mCachedSteps;

    // Step transition -> trigger voice if active
    if (curStep != prevStep) {
      if (mCachedStepsArr[curStep]) {
        int slot = -1;
        for (int v = 0; v < kMaxVoices; v++) {
          if (!mVoices[v].mActive) { slot = v; break; }
        }
        if (slot < 0) slot = 0;
        mVoices[slot].mActive = true;
        mVoices[slot].mNote = mCachedNote;
        mVoices[slot].mPhase = 0.0;
        mVoices[slot].mEnv = 1.0;
        mVoices[slot].mSamplesLeft = noteSamples;
        mVoices[slot].mTotalSamples = noteSamples;
        // Also send MIDI note-on to host (for routing to other instruments)
        IMidiMsg msgOn;
        msgOn.MakeNoteOnMsg(mCachedNote, 100, i);
        SendMidiMsg(msgOn);
      }
      mCurrentStep = curStep;
      mCurrentStepUI.store(curStep);
      prevStep = curStep;
    }

    // Render voices ONCE per sample (NOT per channel), sum to mono mix
    double mix = 0.0;
    for (int v = 0; v < kMaxVoices; v++) {
      if (!mVoices[v].mActive) continue;
      Voice& vc = mVoices[v];
      const double freq = 440.0 * std::pow(2.0, (double(vc.mNote) - 69.0) / 12.0);
      const double w = 2.0 * 3.141592653589793 * freq / sr;
      // Linear decay envelope: 1.0 at trigger -> 0.0 at noteSamples
      const double env = double(vc.mSamplesLeft) / double(vc.mTotalSamples);
      mix += std::sin(vc.mPhase) * env * 0.3;
      vc.mPhase += w;
      if (vc.mPhase > 6.283185307179586) vc.mPhase -= 6.283185307179586;
      if (--vc.mSamplesLeft <= 0) {
        vc.mActive = false;
        // Send MIDI note-off when voice expires
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
  mCurrentStep = -1;
  mCurrentStepUI.store(-1);
  for (int v = 0; v < kMaxVoices; v++) {
    mVoices[v].mActive = false;
    mVoices[v].mNote = 60;
    mVoices[v].mPhase = 0.0;
    mVoices[v].mEnv = 0.0;
    mVoices[v].mSamplesLeft = 0;
    mVoices[v].mTotalSamples = 1;
  }
}
#endif