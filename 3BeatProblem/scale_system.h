#pragma once

#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <cmath>

namespace threebeat {

enum class ScaleType : int {
  kMajor = 0,
  kMinor,
  kDorian,
  kPhrygian,
  kLydian,
  kMixolydian,
  kHarmonicMinor,
  kMelodicMinor,
  kPentatonicMajor,
  kPentatonicMinor,
  kWholeTone,
  kChromatic,
  kCount
};

enum class Mood : int {
  kDark = 0,
  kHappy,
  kSad,
  kBright,
  kTense,
  kDreamy,
  kRandom,
  kChordal,
  kCount
};

inline const std::vector<int>& GetScaleIntervals(ScaleType type) {
  static const std::vector<int> kMajor        = {0, 2, 4, 5, 7, 9, 11};
  static const std::vector<int> kMinor       = {0, 2, 3, 5, 7, 8, 10};
  static const std::vector<int> kDorian      = {0, 2, 3, 5, 7, 9, 10};
  static const std::vector<int> kPhrygian    = {0, 1, 3, 5, 7, 8, 10};
  static const std::vector<int> kLydian     = {0, 2, 4, 6, 7, 9, 11};
  static const std::vector<int> kMixolydian = {0, 2, 4, 5, 7, 9, 10};
  static const std::vector<int> kHarmonicMin = {0, 2, 3, 5, 7, 8, 11};
  static const std::vector<int> kMelodicMin = {0, 2, 3, 5, 7, 9, 11};
  static const std::vector<int> kPentMajor  = {0, 2, 4, 7, 9};
  static const std::vector<int> kPentMinor  = {0, 3, 5, 7, 10};
  static const std::vector<int> kWholeTone = {0, 2, 4, 6, 8, 10};
  static const std::vector<int> kChromatic  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  switch (type) {
    case ScaleType::kMajor:           return kMajor;
    case ScaleType::kMinor:        return kMinor;
    case ScaleType::kDorian:       return kDorian;
    case ScaleType::kPhrygian:     return kPhrygian;
    case ScaleType::kLydian:       return kLydian;
    case ScaleType::kMixolydian:    return kMixolydian;
    case ScaleType::kHarmonicMinor: return kHarmonicMin;
    case ScaleType::kMelodicMinor:  return kMelodicMin;
    case ScaleType::kPentatonicMajor: return kPentMajor;
    case ScaleType::kPentatonicMinor: return kPentMinor;
    case ScaleType::kWholeTone:    return kWholeTone;
    case ScaleType::kChromatic:   return kChromatic;
    default:                      return kMajor;
  }
}

inline const std::vector<int>& GetMoodDegrees(Mood mood) {
  static const std::vector<int> kDark = {0, 2, 4};
  static const std::vector<int> kHappy = {0, 3, 4};
  static const std::vector<int> kSad = {0, 1, 4};
  static const std::vector<int> kBright = {3, 4, 5};
  static const std::vector<int> kTense = {0, 1, 3};
  static const std::vector<int> kDreamy = {0, 3, 5};
  static const std::vector<int> kRandom = {};
  static const std::vector<int> kChordal = {0, 2, 4, 5};

  switch (mood) {
    case Mood::kDark:    return kDark;
    case Mood::kHappy:   return kHappy;
    case Mood::kSad:     return kSad;
    case Mood::kBright: return kBright;
    case Mood::kTense:  return kTense;
    case Mood::kDreamy: return kDreamy;
    case Mood::kRandom: return kRandom;
    case Mood::kChordal: return kChordal;
    default:           return kDark;
  }
}

inline int GetScaleSize(ScaleType type) {
  return static_cast<int>(GetScaleIntervals(type).size());
}

inline const char* GetScaleName(ScaleType type) {
  switch (type) {
    case ScaleType::kMajor:            return "Major";
    case ScaleType::kMinor:           return "Minor";
    case ScaleType::kDorian:          return "Dorian";
    case ScaleType::kPhrygian:        return "Phrygian";
    case ScaleType::kLydian:          return "Lydian";
    case ScaleType::kMixolydian:         return "Mixolydian";
    case ScaleType::kHarmonicMinor:  return "Harmonic Minor";
    case ScaleType::kMelodicMinor:  return "Melodic Minor";
    case ScaleType::kPentatonicMajor: return "Pentatonic Major";
    case ScaleType::kPentatonicMinor: return "Pentatonic Minor";
    case ScaleType::kWholeTone:     return "Whole Tone";
    case ScaleType::kChromatic:      return "Chromatic";
    default:                       return "Unknown";
  }
}

inline const char* GetMoodName(Mood mood) {
  switch (mood) {
    case Mood::kDark:    return "Dark";
    case Mood::kHappy:   return "Happy";
    case Mood::kSad:    return "Sad";
    case Mood::kBright:   return "Bright";
    case Mood::kTense:    return "Tense";
    case Mood::kDreamy:   return "Dreamy";
    case Mood::kRandom:  return "Random";
    case Mood::kChordal:  return "Chordal";
    default:           return "Unknown";
  }
}

inline int GetScaleTypeCount() {
  return 12;
}

inline int GetMoodCount() {
  return 8;
}

inline int GenerateScaleNote(
    int rootNote,
    ScaleType scale,
    Mood mood,
    int octaveLow,
    int octaveHigh,
    float position,
    int& lastNoteIndex,
    std::mt19937& rng) {

  const std::vector<int>& scaleIntervals = GetScaleIntervals(scale);
  int scaleSize = static_cast<int>(scaleIntervals.size());

  if (scaleSize == 0) return -1;

  const std::vector<int>& moodDegrees = GetMoodDegrees(mood);

  std::vector<int> usableDegrees;
  usableDegrees.reserve(scaleSize);

  if (mood == Mood::kRandom) {
    for (int i = 0; i < scaleSize; i++) {
      usableDegrees.push_back(i);
    }
  } else {
    for (int degree : moodDegrees) {
      if (degree >= 0 && degree < scaleSize) {
        usableDegrees.push_back(degree);
      }
    }
  }

  if (usableDegrees.empty()) {
    usableDegrees.push_back(0);
  }

  size_t numUsable = usableDegrees.size();
  size_t idx = static_cast<size_t>(position * static_cast<float>(numUsable)) % numUsable;

  if (static_cast<int>(idx) == lastNoteIndex && numUsable > 1) {
    idx = (idx + 1) % numUsable;
  }

  int degreeIndex = usableDegrees[idx];
  double interval = static_cast<double>(scaleIntervals[degreeIndex]);

  int octaveRange = octaveHigh - octaveLow + 1;
  if (octaveRange < 1) octaveRange = 1;

  float octavePos = position * static_cast<float>(octaveRange) * 3.7f;
  int octaveOffset = static_cast<int>(octavePos) % octaveRange;
  if (octaveOffset < 0) octaveOffset = 0;

  int octave = octaveLow + octaveOffset;
  if (octave < 0) octave = 0;
  if (octave > 8) octave = 8;

  int note = rootNote + static_cast<int>(interval) + (octave * 12);
  if (note < 0) note = 0;
  if (note > 127) note = 127;

  lastNoteIndex = static_cast<int>(idx);

  return note;
}

} // namespace threebeat