#pragma once

#include "scale_system.h"
#include <array>
#include <vector>
#include <cstdint>
#include <cmath>
#include <random>
#include <algorithm>

namespace threebeat {

static constexpr int kMaxSteps = 16;
static constexpr int kNumSequencers = 3;
static constexpr int kMaxVoices = 8;

enum class SequencerMode : int { kMelody = 0, kDrum = 1 };

struct StepPattern {
  bool active[kMaxSteps] = {false};
  uint8_t velocity[kMaxSteps] = {1};
  int numSteps = 8;

  inline void Clear() {
    for (int i = 0; i < kMaxSteps; i++) {
      active[i] = false;
      velocity[i] = 1;
    }
  }

  inline void Fill(int every) {
    for (int i = 0; i < numSteps; i++) {
      active[i] = (i % every == 0);
    }
  }

  inline void Invert() {
    for (int i = 0; i < numSteps; i++) {
      active[i] = !active[i];
    }
  }

  inline void Rotate(bool right) {
    if (right) {
      bool t = active[numSteps - 1];
      for (int i = numSteps - 1; i > 0; i--) {
        active[i] = active[i - 1];
      }
      active[0] = t;
    } else {
      bool t = active[0];
      for (int i = 0; i < numSteps - 1; i++) {
        active[i] = active[i + 1];
      }
      active[numSteps - 1] = t;
    }
  }

  inline void Mutate(int intensity, std::mt19937& rng) {
    float p = intensity / 100.0f;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int i = 0; i < numSteps; i++) {
      if (dist(rng) < p) {
        active[i] = !active[i];
      }
    }
  }

  inline void CopyFrom(const StepPattern& other) {
    for (int i = 0; i < kMaxSteps; i++) {
      active[i] = other.active[i];
      velocity[i] = other.velocity[i];
    }
    numSteps = other.numSteps;
  }
};

struct DrumSequencer {
  StepPattern pattern;
  int midiNote = 36;
  bool mute = false;
  bool solo = false;
  float phase = 0.0f;
  int currentStep = 0;
  float rate = 0.25f;
};

struct Voice {
  int rootNote = -1;
  int velocity = 100;
  bool active = false;

  StepPattern patterns[kNumSequencers];
  Mood moods[kNumSequencers] = {Mood::kDark, Mood::kHappy, Mood::kBright};
  float phases[kNumSequencers] = {};
  int currentSteps[kNumSequencers] = {};
  float rates[kNumSequencers] = {0.25f, 0.25f, 0.25f};
  int lastNoteIndices[kNumSequencers] = {-1, -1, -1};


};

struct MidiEvent {
  enum Type { kNoteOn, kNoteOff };
  Type type;
  int note;
  int velocity;
  int sampleOffset;
  int channel;
};

class VoiceAllocator {
public:
  Voice* NoteOn(int note, int velocity) {
    int existingIdx = FindVoiceForNote(note);
    if (existingIdx >= 0) {
      voices_[existingIdx].velocity = velocity;
      return &voices_[existingIdx];
    }

    int freeIdx = FindFreeVoice();
    if (freeIdx < 0) {
      freeIdx = nextVoiceIndex_;
    }

    Voice* v = &voices_[freeIdx];
    v->rootNote = note;
    v->velocity = velocity;
    v->active = true;

    for (int i = 0; i < kNumSequencers; i++) {
      v->patterns[i].Clear();
      v->phases[i] = 0.0f;
      v->currentSteps[i] = 0;
      v->lastNoteIndices[i] = -1;
    }

    nextVoiceIndex_ = (nextVoiceIndex_ + 1) % kMaxVoices;
    return v;
  }

  void NoteOff(int note) {
    int idx = FindVoiceForNote(note);
    if (idx >= 0) {
      voices_[idx].rootNote = -1;
      voices_[idx].active = false;
    }
  }

  void AllNotesOff() {
    for (int i = 0; i < kMaxVoices; i++) {
      voices_[i].rootNote = -1;
      voices_[i].active = false;
    }
  }

  Voice* GetVoice(int index) { return &voices_[index]; }
  const Voice* GetVoice(int index) const { return &voices_[index]; }

  int GetActiveVoiceCount() const {
    int count = 0;
    for (int i = 0; i < kMaxVoices; i++) {
      if (voices_[i].active) count++;
    }
    return count;
  }

  bool IsNoteHeld(int note) const {
    return FindVoiceForNote(note) >= 0;
  }

private:
  std::array<Voice, kMaxVoices> voices_{};
  int nextVoiceIndex_ = 0;

  int FindFreeVoice() const {
    for (int i = 0; i < kMaxVoices; i++) {
      if (!voices_[i].active || voices_[i].rootNote < 0) {
        return i;
      }
    }
    return -1;
  }

  int FindVoiceForNote(int note) const {
    for (int i = 0; i < kMaxVoices; i++) {
      if (voices_[i].active && voices_[i].rootNote == note) {
        return i;
      }
    }
    return -1;
  }
};

class SequencerEngine {
public:
  SequencerEngine()
      : rng_(42) {
    for (int v = 0; v < kMaxVoices; v++) {
      for (int s = 0; s < kNumSequencers; s++) {
        voiceAllocator_.GetVoice(v)->patterns[s].Fill(4);
      }
    }
    for (int d = 0; d < kNumSequencers; d++) {
      drumSeqs_[d].pattern.Fill(4);
    }
  }

  void SetMode(SequencerMode mode) { mode_ = mode; }
  SequencerMode GetMode() const { return mode_; }

  VoiceAllocator& GetVoiceAllocator() { return voiceAllocator_; }
  ScaleType GetScaleType() const { return scaleType_; }
  void SetScaleType(ScaleType t) { scaleType_ = t; }
  int GetScaleRoot() const { return scaleRoot_; }
  void SetScaleRoot(int r) { scaleRoot_ = r; }
  int GetOctaveLow() const { return octaveLow_; }
  void SetOctaveLow(int o) { octaveLow_ = o; }
  int GetOctaveHigh() const { return octaveHigh_; }
  void SetOctaveHigh(int o) { octaveHigh_ = o; }

  DrumSequencer& GetDrumSequencer(int i) { return drumSeqs_[i]; }
  const DrumSequencer& GetDrumSequencer(int i) const { return drumSeqs_[i]; }

  StepPattern& GetMelodyPattern(int voiceIdx, int seqIdx) {
    return voiceAllocator_.GetVoice(voiceIdx)->patterns[seqIdx];
  }

  StepPattern& GetDrumPattern(int seqIdx) { return drumSeqs_[seqIdx].pattern; }

  std::vector<MidiEvent> ProcessBlock(
      double sampleRate, int numSamples, double tempo,
      bool transportRunning, int velocity, int velRandom,
      int probability, int noteDuration) {

    std::vector<MidiEvent> events;
    events.reserve(numSamples * 3);

    double samplesPerBeat = sampleRate * 60.0 / tempo;

    for (int s = 0; s < numSamples; s++) {
      sampleCounter_++;

      if (!transportRunning) continue;

      if (mode_ == SequencerMode::kMelody) {
        for (int v = 0; v < kMaxVoices; v++) {
          Voice* voice = voiceAllocator_.GetVoice(v);
          if (!voice->active || voice->rootNote < 0) continue;

          for (int seq = 0; seq < kNumSequencers; seq++) {
            float phaseStep = RateToPhaseStep(voice->rates[seq], samplesPerBeat);
            voice->phases[seq] += phaseStep;

            if (voice->phases[seq] >= 1.0f) {
              voice->phases[seq] -= 1.0f;
              voice->currentSteps[seq]++;

              int step = voice->currentSteps[seq] % voice->patterns[seq].numSteps;

              if (voice->patterns[seq].active[step] && ShouldTrigger(probability)) {
                int note = GenerateScaleNote(
                    voice->rootNote, scaleType_, voice->moods[seq],
                    octaveLow_, octaveHigh_, voice->phases[seq],
                    voice->lastNoteIndices[seq], rng_);

                if (note >= 0 && note <= 127) {
                  int vel = CalcVelocity(velocity, voice->patterns[seq].velocity[step], velRandom);

                  MidiEvent ev;
                  ev.type = MidiEvent::kNoteOn;
                  ev.note = note;
                  ev.velocity = vel;
                  ev.sampleOffset = s;
                  ev.channel = v;
                  events.push_back(ev);

                  double stepDurSamples = samplesPerBeat * 0.25;
                  double durSamples = stepDurSamples * (noteDuration / 100.0);
                  int offOffset = s + static_cast<int>(durSamples);
                  if (offOffset < numSamples) {
                    MidiEvent off;
                    off.type = MidiEvent::kNoteOff;
                    off.note = note;
                    off.velocity = 0;
                    off.sampleOffset = offOffset;
                    off.channel = v;
                    events.push_back(off);
                  } else {
                    MidiEvent off;
                    off.type = MidiEvent::kNoteOff;
                    off.note = note;
                    off.velocity = 0;
                    off.sampleOffset = numSamples - 1;
                    off.channel = v;
                    events.push_back(off);
                  }
                }
              }
            }
          }
        }
      } else {
        bool hasSolo = false;
        for (int i = 0; i < kNumSequencers; i++) {
          if (drumSeqs_[i].solo) {
            hasSolo = true;
            break;
          }
        }

        for (int seq = 0; seq < kNumSequencers; seq++) {
          auto& ds = drumSeqs_[seq];
          if (ds.mute) continue;
          if (hasSolo && !ds.solo) continue;

          float phaseStep = RateToPhaseStep(ds.rate, samplesPerBeat);
          ds.phase += phaseStep;

          if (ds.phase >= 1.0f) {
            ds.phase -= 1.0f;
            ds.currentStep++;

            int step = ds.currentStep % ds.pattern.numSteps;

            if (ds.pattern.active[step] && ShouldTrigger(probability)) {
              int vel = CalcVelocity(velocity, ds.pattern.velocity[step], velRandom);

              MidiEvent ev;
              ev.type = MidiEvent::kNoteOn;
              ev.note = ds.midiNote;
              ev.velocity = vel;
              ev.sampleOffset = s;
              ev.channel = seq + 8;
              events.push_back(ev);

              double stepDur = samplesPerBeat * 0.25;
              double dur = stepDur * (noteDuration / 100.0);
              int offOff = s + static_cast<int>(dur);
              if (offOff < numSamples) {
                MidiEvent off;
                off.type = MidiEvent::kNoteOff;
                off.note = ds.midiNote;
                off.velocity = 0;
                off.sampleOffset = offOff;
                off.channel = seq + 8;
                events.push_back(off);
              }
            }
          }
        }
      }
    }

    return events;
  }

  void DrumClear(int si) { drumSeqs_[si].pattern.Clear(); }
  void DrumFill(int si, int every) { drumSeqs_[si].pattern.Fill(every); }
  void DrumInvert(int si) { drumSeqs_[si].pattern.Invert(); }
  void DrumRotate(int si, bool right) { drumSeqs_[si].pattern.Rotate(right); }
  void DrumMutate(int si, int intensity) { drumSeqs_[si].pattern.Mutate(intensity, rng_); }
  void DrumCopyPattern(int from, int to) { drumSeqs_[to].pattern.CopyFrom(drumSeqs_[from].pattern); }

  void ResetAll() {
    sampleCounter_ = 0.0;
    voiceAllocator_.AllNotesOff();
    for (int i = 0; i < kNumSequencers; i++) {
      drumSeqs_[i].phase = 0.0f;
      drumSeqs_[i].currentStep = 0;
    }
    for (int v = 0; v < kMaxVoices; v++) {
      for (int s = 0; s < kNumSequencers; s++) {
        Voice* voice = voiceAllocator_.GetVoice(v);
        voice->phases[s] = 0.0f;
        voice->currentSteps[s] = 0;
      }
    }
  }

  void Panic(std::vector<MidiEvent>& outEvents) {
    voiceAllocator_.AllNotesOff();
    for (int i = 0; i < kMaxVoices; i++) {
      if (voiceAllocator_.GetVoice(i)->rootNote >= 0) {
        MidiEvent off;
        off.type = MidiEvent::kNoteOff;
        off.note = voiceAllocator_.GetVoice(i)->rootNote;
        off.velocity = 0;
        off.sampleOffset = 0;
        off.channel = i;
        outEvents.push_back(off);
      }
    }
  }

  struct State {
    SequencerMode mode = SequencerMode::kMelody;
    int scaleType = 1;
    int scaleRoot = 0;
    int octaveLow = 3, octaveHigh = 5;

    struct DrumState {
      StepPattern pat;
      int note = 36;
      bool mute = false, solo = false;
  float rate = 1.0f;
    };
    DrumState drums[kNumSequencers];

    struct VoiceState {
      int root = -1, vel = 100;
      bool act = false;
      StepPattern pats[kNumSequencers];
      int moods[kNumSequencers];
      float rates[kNumSequencers];
    };
    VoiceState voices[kMaxVoices];
  };

  State GetState() const {
    State s;
    s.mode = mode_;
    s.scaleType = static_cast<int>(scaleType_);
    s.scaleRoot = scaleRoot_;
    s.octaveLow = octaveLow_;
    s.octaveHigh = octaveHigh_;

    for (int i = 0; i < kNumSequencers; i++) {
      s.drums[i].pat.CopyFrom(drumSeqs_[i].pattern);
      s.drums[i].note = drumSeqs_[i].midiNote;
      s.drums[i].mute = drumSeqs_[i].mute;
      s.drums[i].solo = drumSeqs_[i].solo;
      s.drums[i].rate = drumSeqs_[i].rate;
    }

    for (int v = 0; v < kMaxVoices; v++) {
      const Voice* voice = voiceAllocator_.GetVoice(v);
      s.voices[v].root = voice->rootNote;
      s.voices[v].vel = voice->velocity;
      s.voices[v].act = voice->active;
      for (int seq = 0; seq < kNumSequencers; seq++) {
        s.voices[v].pats[seq].CopyFrom(voice->patterns[seq]);
        s.voices[v].moods[seq] = static_cast<int>(voice->moods[seq]);
        s.voices[v].rates[seq] = voice->rates[seq];
      }
    }

    return s;
  }

  void SetState(const State& s) {
    mode_ = s.mode;
    scaleType_ = static_cast<ScaleType>(s.scaleType);
    scaleRoot_ = s.scaleRoot;
    octaveLow_ = s.octaveLow;
    octaveHigh_ = s.octaveHigh;

    for (int i = 0; i < kNumSequencers; i++) {
      drumSeqs_[i].pattern.CopyFrom(s.drums[i].pat);
      drumSeqs_[i].midiNote = s.drums[i].note;
      drumSeqs_[i].mute = s.drums[i].mute;
      drumSeqs_[i].solo = s.drums[i].solo;
      drumSeqs_[i].rate = s.drums[i].rate;
    }

    voiceAllocator_.AllNotesOff();
    for (int v = 0; v < kMaxVoices; v++) {
      Voice* voice = voiceAllocator_.GetVoice(v);
      voice->rootNote = s.voices[v].root;
      voice->velocity = s.voices[v].vel;
      voice->active = s.voices[v].act;
      for (int seq = 0; seq < kNumSequencers; seq++) {
        voice->patterns[seq].CopyFrom(s.voices[v].pats[seq]);
        voice->moods[seq] = static_cast<Mood>(s.voices[v].moods[seq]);
        voice->rates[seq] = s.voices[v].rates[seq];
      }
    }
  }

private:
  SequencerMode mode_ = SequencerMode::kDrum;
  VoiceAllocator voiceAllocator_;
  ScaleType scaleType_ = ScaleType::kMinor;
  int scaleRoot_ = 0, octaveLow_ = 3, octaveHigh_ = 5;
  std::array<DrumSequencer, kNumSequencers> drumSeqs_{};
  double sampleCounter_ = 0.0;
  std::mt19937 rng_;
  std::vector<MidiEvent> pendingNoteOffs_{};

  inline float RateToPhaseStep(float rate, double samplesPerBeat) const {
    return static_cast<float>(rate / samplesPerBeat);
  }

  inline int CalcVelocity(int base, int accent, int random) {
    static const int accentMap[3] = {64, 88, 112};
    int accentVel = accentMap[std::min(accent, 2)];
    float randAmount = (random / 100.0f) * 16.0f;
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    int delta = static_cast<int>(dist(rng_) * randAmount);
    int result = accentVel + delta;
    if (result < 1) result = 1;
    if (result > 127) result = 127;
    return result;
  }

  inline bool ShouldTrigger(int probability) {
    if (probability >= 100) return true;
    std::uniform_int_distribution<int> dist(0, 99);
    return dist(rng_) < probability;
  }
};

} // namespace threebeat