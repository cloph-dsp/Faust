#include "3BeatProblem.h"

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <cstdio>
#include <cstring>

ThreeBeatProblem::ThreeBeatProblem(const InstanceInfo& info)
    : Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kParamMode)->InitEnum("Mode", 1, {"Melody", "Drum"});
  GetParam(kParamBPM)->InitDouble("BPM", 120.0, 40.0, 300.0, 1.0, "BPM");
  GetParam(kParamSync)->InitBool("Sync", false);
  GetParam(kParamSwing)->InitPercentage("Swing", 0.0);
  GetParam(kParamNoteDuration)->InitPercentage("Duration", 50.0);
  GetParam(kParamVelocity)->InitDouble("Velocity", 100.0, 0.0, 127.0, 1.0);
  GetParam(kParamVelRandom)->InitPercentage("Vel Rand", 0.0);
  GetParam(kParamProbability)->InitPercentage("Probability", 100.0);

  GetParam(kParamScaleType)->InitEnum("Scale Type", 1, {"Major","Minor","Dorian","Phrygian","Lydian","Mixolydian","Harmonic Minor","Melodic Minor","Pent Major","Pent Minor","Whole Tone","Chromatic"});
  GetParam(kParamScaleRoot)->InitEnum("Scale Root", 0, {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"});
  GetParam(kParamOctaveLow)->InitInt("Octave Low", 3, 0, 8);
  GetParam(kParamOctaveHigh)->InitInt("Octave High", 5, 0, 8);

  for (int s = 0; s < 3; s++) {
    int base = kParamS1Rate + (s * 20);
    char name[32];

    snprintf(name, sizeof(name), "S%d Rate", s + 1);
    GetParam(base)->InitEnum(name, 4, {"1/64","1/32","1/16","1/8","1/4","1/2","1x","2x","4x","8x","16x","32x","64x"});

    snprintf(name, sizeof(name), "S%d Steps", s + 1);
    GetParam(base + 1)->InitInt(name, 8, 2, 16);

    snprintf(name, sizeof(name), "S%d Mood", s + 1);
    GetParam(base + 2)->InitEnum(name, s, {"Dark","Happy","Sad","Bright","Tense","Dreamy","Random","Chordal"});

    for (int p = 0; p < 16; p++) {
      snprintf(name, sizeof(name), "S%d Step%d", s + 1, p + 1);
      GetParam(base + 4 + p)->InitBool(name, false);
    }
  }

  for (int s = 0; s < 3; s++) {
    int base = kParamDrumS1Rate + (s * 22);
    char name[32];

    snprintf(name, sizeof(name), "Drum%d Rate", s + 1);
    GetParam(base)->InitEnum(name, 6, {"1/64","1/32","1/16","1/8","1/4","1/2","1x","2x","4x","8x","16x","32x","64x"});

    snprintf(name, sizeof(name), "Drum%d Steps", s + 1);
    GetParam(base + 1)->InitInt(name, 8, 2, 16);

    snprintf(name, sizeof(name), "Drum%d Note", s + 1);
    GetParam(base + 2)->InitInt(name, 36 + (s * 12), 0, 127);

    snprintf(name, sizeof(name), "Drum%d Mute", s + 1);
    GetParam(base + 3)->InitBool(name, false);

    snprintf(name, sizeof(name), "Drum%d Solo", s + 1);
    GetParam(base + 4)->InitBool(name, false);

    for (int p = 0; p < 16; p++) {
      snprintf(name, sizeof(name), "Drum%d St%d", s + 1, p + 1);
      GetParam(base + 6 + p)->InitBool(name, false);
    }
  }

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* graphics) {
    const IColor bg(255, 15, 20, 23);
    const IColor panel(255, 30, 45, 51);
    const IColor line(255, 54, 81, 95);
    const IColor accent(255, 41, 211, 178);
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
      .WithColor(kSH, IColor(60, 0, 0, 0))
      .WithLabelText(IText(11.f, textMuted, uiFont, EAlign::Center, EVAlign::Top))
      .WithValueText(IText(12.f, textMain, uiFont, EAlign::Center, EVAlign::Bottom))
      .WithDrawShadows(true)
      .WithShadowOffset(2);

    const IVStyle kToggleStyle = DEFAULT_STYLE
      .WithColor(kBG, panel)
      .WithColor(kFG, line)
      .WithColor(kPR, accent)
      .WithColor(kFR, line)
      .WithColor(kHL, accent)
      .WithColor(kX1, textMain)
      .WithLabelText(IText(10.f, textMuted, uiFont, EAlign::Center, EVAlign::Top))
      .WithValueText(IText(10.f, accent, uiFont, EAlign::Center, EVAlign::Bottom));

    graphics->AttachPanelBackground(bg);

    graphics->AttachControl(new IVKnobControl(
      IRECT(10, 10, 70, 70), kParamBPM, "BPM", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(80, 10, 140, 70), kParamVelocity, "Vel", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(150, 10, 210, 70), kParamVelRandom, "RndV", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(220, 10, 280, 70), kParamNoteDuration, "Dur", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(290, 10, 350, 70), kParamProbability, "Prob", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(360, 10, 420, 70), kParamSwing, "Swing", kStyle));
    graphics->AttachControl(new IVToggleControl(
      IRECT(440, 20, 490, 60), kParamSync, "Sync", kToggleStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(500, 10, 560, 70), kParamMode, "Mode", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(570, 10, 630, 70), kParamScaleType, "Scale", kStyle));
    graphics->AttachControl(new IVKnobControl(
      IRECT(640, 10, 670, 70), kParamScaleRoot, "Root", kStyle));

    for (int s = 0; s < 3; s++) {
      int sy = 85 + s * 130;
      int rateP = (s == 0) ? kParamS1Rate : (s == 1) ? kParamS2Rate : kParamS3Rate;

      graphics->AttachControl(new IVKnobControl(
        IRECT(20, sy, 80, sy + 60), rateP, "Rate", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(90, sy, 150, sy + 60), rateP + 1, "Steps", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(160, sy, 220, sy + 60), rateP + 2, "Mood", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(230, sy, 290, sy + 60), kParamOctaveLow, "OctLo", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(300, sy, 360, sy + 60), kParamOctaveHigh, "OctHi", kStyle));

      const int patternBase = rateP + 4;
      for (int p = 0; p < 16; p++) {
        int px = 370 + (p % 8) * 38;
        int py = sy + (p / 8) * 22;
        char lbl[8];
        snprintf(lbl, 8, "%d", p + 1);
        graphics->AttachControl(new IVToggleControl(
          IRECT(px, py, px + 34, py + 18), patternBase + p, lbl, kToggleStyle));
      }
    }
  };
#endif
}

void ThreeBeatProblem::ProcessMidiMsg(const IMidiMsg& msg) {
  if (engine_.GetMode() != threebeat::SequencerMode::kMelody) {
    return;
  }

  switch (msg.StatusMsg()) {
    case IMidiMsg::kNoteOn:
      if (msg.Velocity() > 0) {
        engine_.GetVoiceAllocator().NoteOn(msg.NoteNumber(), msg.Velocity());
      } else {
        engine_.GetVoiceAllocator().NoteOff(msg.NoteNumber());
      }
      break;
    case IMidiMsg::kNoteOff:
      engine_.GetVoiceAllocator().NoteOff(msg.NoteNumber());
      break;
    default:
      break;
  }
}

void ThreeBeatProblem::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  double tempo = GetTempo();
  bool transportRunning = mTimeInfo.mTransportIsRunning;

  int syncOn = static_cast<int>(GetParam(kParamSync)->Value());
  if (!syncOn) {
    tempo = GetParam(kParamBPM)->Value();
    transportRunning = true;
  }

  auto events = engine_.ProcessBlock(
      GetSampleRate(), nFrames, tempo, transportRunning,
      cachedVelocity_, cachedVelRandom_,
      cachedProbability_, cachedNoteDuration_);

  for (const auto& ev : events) {
    IMidiMsg midiMsg;
    int ch = ev.channel;
    if (ch < 0 || ch > 15) ch = 0;
    if (ev.type == threebeat::MidiEvent::kNoteOn) {
      midiMsg.MakeNoteOnMsg(ev.note, ev.velocity, ev.sampleOffset, ch);
    } else {
      midiMsg.MakeNoteOffMsg(ev.note, ev.sampleOffset, ch);
    }
    SendMidiMsg(midiMsg);
  }

  memset(outputs[0], 0, nFrames * sizeof(sample));
  memset(outputs[1], 0, nFrames * sizeof(sample));
}

void ThreeBeatProblem::OnReset() {
  engine_.ResetAll();
}

void ThreeBeatProblem::OnParamChange(int paramIdx) {
  double val = GetParam(paramIdx)->Value();

  switch (paramIdx) {
    case kParamMode:
      engine_.SetMode(val < 0.5 ? threebeat::SequencerMode::kMelody : threebeat::SequencerMode::kDrum);
      engine_.ResetAll();
      break;

    case kParamVelocity:
      cachedVelocity_ = static_cast<int>(val);
      break;

    case kParamVelRandom:
      cachedVelRandom_ = static_cast<int>(val);
      break;

    case kParamProbability:
      cachedProbability_ = static_cast<int>(val);
      break;

    case kParamNoteDuration:
      cachedNoteDuration_ = static_cast<int>(val);
      break;

    case kParamScaleType:
      engine_.SetScaleType(static_cast<threebeat::ScaleType>(static_cast<int>(val)));
      break;

    case kParamScaleRoot:
      engine_.SetScaleRoot(static_cast<int>(val));
      break;

    case kParamOctaveLow:
      engine_.SetOctaveLow(static_cast<int>(val));
      break;

    case kParamOctaveHigh:
      engine_.SetOctaveHigh(static_cast<int>(val));
      break;

    default: {
      if (paramIdx >= kParamS1Rate && paramIdx < kParamS1Rate + 20) {
        int seqIdx = 0;
        int local = paramIdx - kParamS1Rate;
        HandleMelodySeqParam(seqIdx, local, val);
      } else if (paramIdx >= kParamS2Rate && paramIdx < kParamS2Rate + 20) {
        int seqIdx = 1;
        int local = paramIdx - kParamS2Rate;
        HandleMelodySeqParam(seqIdx, local, val);
      } else if (paramIdx >= kParamS3Rate && paramIdx < kParamS3Rate + 20) {
        int seqIdx = 2;
        int local = paramIdx - kParamS3Rate;
        HandleMelodySeqParam(seqIdx, local, val);
      } else if (paramIdx >= kParamDrumS1Rate && paramIdx < kParamDrumS1Rate + 22) {
        int seqIdx = 0;
        int local = paramIdx - kParamDrumS1Rate;
        HandleDrumSeqParam(seqIdx, local, val);
      } else if (paramIdx >= kParamDrumS2Rate && paramIdx < kParamDrumS2Rate + 22) {
        int seqIdx = 1;
        int local = paramIdx - kParamDrumS2Rate;
        HandleDrumSeqParam(seqIdx, local, val);
      } else if (paramIdx >= kParamDrumS3Rate && paramIdx < kParamDrumS3Rate + 22) {
        int seqIdx = 2;
        int local = paramIdx - kParamDrumS3Rate;
        HandleDrumSeqParam(seqIdx, local, val);
      }
      break;
    }
  }
}

void ThreeBeatProblem::HandleMelodySeqParam(int seqIdx, int localParam, double value) {
  auto& allocator = engine_.GetVoiceAllocator();

  for (int v = 0; v < threebeat::kMaxVoices; v++) {
    auto* voice = allocator.GetVoice(v);
    if (!voice->active) continue;

    switch (localParam) {
      case 0:
        voice->rates[seqIdx] = RateIdxToFloat(static_cast<int>(value));
        break;
      case 1:
        voice->patterns[seqIdx].numSteps = static_cast<int>(value);
        break;
      case 2:
        voice->moods[seqIdx] = static_cast<threebeat::Mood>(static_cast<int>(value));
        break;
      default: {
        int patternIdx = localParam - 4;
        if (patternIdx >= 0 && patternIdx < threebeat::kMaxSteps) {
          voice->patterns[seqIdx].active[patternIdx] = (value >= 0.5);
        }
        break;
      }
    }
  }
}

void ThreeBeatProblem::HandleDrumSeqParam(int seqIdx, int localParam, double value) {
  auto& drumSeq = engine_.GetDrumSequencer(seqIdx);

  switch (localParam) {
    case 0:
      drumSeq.rate = RateIdxToFloat(static_cast<int>(value));
      break;
    case 1:
      drumSeq.pattern.numSteps = static_cast<int>(value);
      break;
    case 2:
      drumSeq.midiNote = static_cast<int>(value);
      break;
    case 3:
      drumSeq.mute = (value >= 0.5);
      break;
    case 4:
      drumSeq.solo = (value >= 0.5);
      break;
    default: {
      int patternIdx = localParam - 6;
      if (patternIdx >= 0 && patternIdx < threebeat::kMaxSteps) {
        drumSeq.pattern.active[patternIdx] = (value >= 0.5);
      }
      break;
    }
  }
}

void ThreeBeatProblem::SyncAllParamsFromEngine() {
  cachedVelocity_ = static_cast<int>(GetParam(kParamVelocity)->Value());
  cachedVelRandom_ = static_cast<int>(GetParam(kParamVelRandom)->Value());
  cachedProbability_ = static_cast<int>(GetParam(kParamProbability)->Value());
  cachedNoteDuration_ = static_cast<int>(GetParam(kParamNoteDuration)->Value());
}

bool ThreeBeatProblem::SerializeState(IByteChunk& chunk) const {
  auto state = engine_.GetState();
  chunk.Put(&state);
  return true;
}

int ThreeBeatProblem::UnserializeState(const IByteChunk& chunk, int startPos) {
  threebeat::SequencerEngine::State state;
  int pos = chunk.Get(&state, startPos);
  if (pos > startPos) {
    engine_.SetState(state);
    SyncAllParamsFromEngine();
  }
  return pos;
}
