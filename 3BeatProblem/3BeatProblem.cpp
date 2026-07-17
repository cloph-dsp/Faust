#include "3BeatProblem.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <cmath>
#include <cstring>
#include <cctype>

// MUSIC-THEORY DATABASE
// ScaleType: see 3BeatProblem.h (struct defined there).
// kScales[80] - canonical + exotic scales
const ScaleType kScales[80] = {
  // 0-24: Common scales / modes
    { "Chromatic", 12, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } },
    { "Major", 7, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Minor", 7, { 0, 2, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Harmonic Minor", 7, { 0, 2, 3, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Melodic Minor", 7, { 0, 2, 3, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Pentatonic Major", 5, { 0, 2, 4, 7, 9, 0, 0, 0, 0, 0, 0, 0 } },
    { "Pentatonic Minor", 5, { 0, 3, 5, 7, 10, 0, 0, 0, 0, 0, 0, 0 } },
    { "Blues", 6, { 0, 3, 5, 6, 7, 10, 0, 0, 0, 0, 0, 0 } },
    { "Dorian", 7, { 0, 2, 3, 5, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Phrygian", 7, { 0, 1, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Lydian", 7, { 0, 2, 4, 6, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Mixolydian", 7, { 0, 2, 4, 5, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Locrian", 7, { 0, 1, 3, 5, 6, 8, 10, 0, 0, 0, 0, 0 } },
    { "Ionian", 7, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Aeolian", 7, { 0, 2, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Phrygian Dominant", 7, { 0, 1, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Lydian Dominant", 7, { 0, 2, 4, 6, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Altered", 7, { 0, 1, 3, 4, 6, 8, 10, 0, 0, 0, 0, 0 } },
    { "Hungarian Minor", 7, { 0, 2, 3, 6, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Hungarian Major", 7, { 0, 3, 4, 6, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Neapolitan Major", 7, { 0, 1, 3, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Neapolitan Minor", 7, { 0, 1, 3, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Spanish", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Arabic", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Jewish", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
  // 25-79: Exotic / jazz / world scales
    { "Bebop Dominant", 8, { 0, 2, 4, 5, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Bebop Major", 8, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Bebop Minor", 8, { 0, 2, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Bebop Harmonic Minor", 8, { 0, 2, 3, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Bebop Melodic Minor", 8, { 0, 2, 3, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Major Blues", 6, { 0, 2, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0 } },
    { "Minor Blues", 6, { 0, 3, 5, 6, 7, 10, 0, 0, 0, 0, 0, 0 } },
    { "Double Harmonic Major", 7, { 0, 3, 4, 6, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Double Harmonic Minor", 7, { 0, 2, 3, 6, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Persian", 7, { 0, 1, 3, 4, 6, 7, 10, 0, 0, 0, 0, 0 } },
    { "Indian Raga", 7, { 0, 1, 3, 6, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Todi", 7, { 0, 1, 3, 6, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Kafi", 7, { 0, 2, 3, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Khamaj", 7, { 0, 2, 4, 5, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Bhairav", 7, { 0, 1, 3, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Asavari", 7, { 0, 2, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Marwa", 7, { 0, 1, 4, 5, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Purvi", 7, { 0, 1, 4, 5, 6, 8, 10, 0, 0, 0, 0, 0 } },
    { "Todi (Hindustani)", 7, { 0, 1, 3, 6, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Kir", 6, { 0, 1, 4, 5, 7, 8, 0, 0, 0, 0, 0, 0 } },
    { "Hir", 6, { 0, 2, 4, 5, 7, 8, 0, 0, 0, 0, 0, 0 } },
    { "Hirajoshi", 5, { 0, 2, 3, 7, 9, 0, 0, 0, 0, 0, 0, 0 } },
    { "Iwato", 5, { 0, 1, 4, 5, 8, 0, 0, 0, 0, 0, 0, 0 } },
    { "Kumoi", 5, { 0, 2, 3, 7, 9, 0, 0, 0, 0, 0, 0, 0 } },
    { "Pelog", 5, { 0, 1, 4, 7, 8, 0, 0, 0, 0, 0, 0, 0 } },
    { "Slendro", 5, { 0, 2, 4, 7, 9, 0, 0, 0, 0, 0, 0, 0 } },
    { "Egyptian", 5, { 0, 2, 3, 7, 10, 0, 0, 0, 0, 0, 0, 0 } },
    { "Romanian", 7, { 0, 2, 3, 6, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Ukrainian Dorian", 7, { 0, 2, 3, 6, 7, 9, 10, 0, 0, 0, 0, 0 } },
    { "Scottish", 6, { 0, 2, 4, 5, 7, 10, 0, 0, 0, 0, 0, 0 } },
    { "Balkan", 7, { 0, 1, 3, 5, 6, 8, 10, 0, 0, 0, 0, 0 } },
    { "Klezmer", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Hijaz", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Saba", 6, { 0, 1, 3, 5, 6, 8, 0, 0, 0, 0, 0, 0 } },
    { "Nahawand", 7, { 0, 2, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Rast", 7, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Bayati", 7, { 0, 1, 3, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Sikah", 6, { 0, 1, 4, 5, 7, 11, 0, 0, 0, 0, 0, 0 } },
    { "Huzam", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Nawa Athar", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Athar Kurd", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Hijazkar", 7, { 0, 1, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Suznak", 7, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Uzzal", 7, { 0, 2, 4, 6, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Iraqi", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Mouraskh", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Sultani Yakah", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Yakah", 7, { 0, 2, 4, 5, 7, 8, 10, 0, 0, 0, 0, 0 } },
    { "Mahur", 7, { 0, 2, 4, 5, 7, 9, 11, 0, 0, 0, 0, 0 } },
    { "Husseini", 7, { 0, 2, 3, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Awj Iraqi", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Awj", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Nawa", 7, { 0, 2, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Hijaz Sharqi", 7, { 0, 1, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
    { "Suzidil", 7, { 0, 2, 4, 5, 7, 8, 11, 0, 0, 0, 0, 0 } },
};

// kChordTypes[100] - common + extended + pad chords
const ChordType kChordTypes[100] = {
  // 0-34: Common chord types
    { "",  3, { 0, 4, 7, 0, 0, 0, 0, 0 } },
    { "m", 3, { 0, 3, 7, 0, 0, 0, 0, 0 } },
    { "dim", 3, { 0, 3, 6, 0, 0, 0, 0, 0 } },
    { "aug", 3, { 0, 4, 8, 0, 0, 0, 0, 0 } },
    { "sus4", 3, { 0, 5, 7, 0, 0, 0, 0, 0 } },
    { "sus2", 3, { 0, 2, 7, 0, 0, 0, 0, 0 } },
    { "5", 2, { 0, 7, 0, 0, 0, 0, 0, 0 } },
    { "6", 4, { 0, 4, 7, 9, 0, 0, 0, 0 } },
    { "m6", 4, { 0, 3, 7, 9, 0, 0, 0, 0 } },
    { "7", 4, { 0, 4, 7, 10, 0, 0, 0, 0 } },
    { "maj7", 4, { 0, 4, 7, 11, 0, 0, 0, 0 } },
    { "m7", 4, { 0, 3, 7, 10, 0, 0, 0, 0 } },
    { "dim7", 4, { 0, 3, 6, 9, 0, 0, 0, 0 } },
    { "m7b5", 4, { 0, 3, 6, 10, 0, 0, 0, 0 } },
    { "7sus4", 4, { 0, 5, 7, 10, 0, 0, 0, 0 } },
    { "7b9", 5, { 0, 4, 7, 10, 13, 0, 0, 0 } },
    { "7#9", 5, { 0, 4, 7, 10, 15, 0, 0, 0 } },
    { "7#11", 5, { 0, 4, 7, 10, 18, 0, 0, 0 } },
    { "7b13", 5, { 0, 4, 7, 10, 20, 0, 0, 0 } },
    { "7alt", 5, { 0, 4, 8, 10, 13, 0, 0, 0 } },
    { "maj9", 5, { 0, 4, 7, 11, 14, 0, 0, 0 } },
    { "m9", 5, { 0, 3, 7, 10, 14, 0, 0, 0 } },
    { "9", 5, { 0, 4, 7, 10, 14, 0, 0, 0 } },
    { "maj9#5", 5, { 0, 4, 8, 11, 14, 0, 0, 0 } },
    { "m(maj7)", 4, { 0, 3, 7, 11, 0, 0, 0, 0 } },
    { "7#5", 4, { 0, 4, 8, 10, 0, 0, 0, 0 } },
    { "7b5", 4, { 0, 4, 6, 10, 0, 0, 0, 0 } },
    { "m6/9", 5, { 0, 3, 7, 9, 14, 0, 0, 0 } },
    { "6/9", 5, { 0, 4, 7, 9, 14, 0, 0, 0 } },
    { "11", 7, { 0, 4, 7, 10, 14, 17, 19, 0 } },
    { "maj11", 7, { 0, 4, 7, 11, 14, 17, 19, 0 } },
    { "m11", 7, { 0, 3, 7, 10, 14, 17, 19, 0 } },
    { "add9", 4, { 0, 4, 7, 14, 0, 0, 0, 0 } },
    { "m(add9)", 4, { 0, 3, 7, 14, 0, 0, 0, 0 } },
    { "9sus4", 5, { 0, 5, 7, 10, 14, 0, 0, 0 } },
  // 35-49: Extended / altered
    { "13", 6, { 0, 4, 7, 10, 2, 9, 0, 0 } },
    { "maj13", 6, { 0, 4, 7, 11, 14, 21, 0, 0 } },
    { "m13", 6, { 0, 3, 7, 10, 14, 21, 0, 0 } },
    { "7#13", 6, { 0, 4, 7, 10, 9, 21, 0, 0 } },
    { "add11", 5, { 0, 4, 7, 5, 10, 0, 0, 0 } },
    { "addb9", 4, { 0, 4, 7, 1, 0, 0, 0, 0 } },
    { "add#9", 4, { 0, 4, 7, 3, 0, 0, 0, 0 } },
    { "add#11", 5, { 0, 4, 7, 6, 9, 0, 0, 0 } },
    { "7#5#9", 5, { 0, 4, 8, 10, 15, 0, 0, 0 } },
    { "7#5b9", 5, { 0, 4, 8, 10, 1, 0, 0, 0 } },
    { "7b5b9", 5, { 0, 4, 6, 10, 1, 0, 0, 0 } },
    { "7b5#9", 5, { 0, 4, 6, 10, 15, 0, 0, 0 } },
    { "9#11", 6, { 0, 4, 7, 10, 14, 18, 0, 0 } },
    { "9b13", 6, { 0, 4, 7, 10, 14, 20, 0, 0 } },
    { "11b9", 7, { 0, 4, 7, 10, 13, 17, 19, 0 } },
  // 50-64: Added-tone / suspended
    { "add4", 4, { 0, 4, 5, 7, 0, 0, 0, 0 } },
    { "add2", 4, { 0, 2, 4, 7, 0, 0, 0, 0 } },
    { "sus2sus4", 3, { 0, 2, 5, 0, 0, 0, 0, 0 } },
    { "q", 3, { 0, 3, 10, 0, 0, 0, 0, 0 } },
    { "pad1", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad2", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad3", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad4", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad5", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad6", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad7", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad8", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad9", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad10", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad11", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
  // 65-99: Pad placeholders
    { "pad12", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad13", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad14", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad15", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad16", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad17", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad18", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad19", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad20", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad21", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad22", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad23", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad24", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad25", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad26", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad27", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad28", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad29", 3, { 3, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad30", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad31", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad32", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad33", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad34", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad35", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad36", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad37", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad38", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad39", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad40", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad41", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad42", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad43", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad44", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad45", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
    { "pad46", 3, { 0, 0, 0, 0, 0, 0, 0, 0 } },
};

static constexpr const char* kNoteRoots[12] = {
  "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"
};

int ThreeBeatProblem::ParseChordIdStatic(const char* s) {
  if (!s) return -1;
  while (*s == ' ' || *s == '\t') s++;
  if (!*s) return -1;
  int root = -1;
  char r0 = *s++;
  char r1 = (r0 >= 'a' && r0 <= 'g') ? (r0 - 'a' + 'A') : r0;
  switch (r1) {
    case 'C': root = 0; break;
    case 'D': root = 2; break;
    case 'E': root = 4; break;
    case 'F': root = 5; break;
    case 'G': root = 7; break;
    case 'A': root = 9; break;
    case 'B': root = 11; break;
    default: return -1;
  }
  if (*s == '#') { root = (root + 1) % 12; s++; }
  else if (*s == 'b' || *s == '-') { root = (root + 11) % 12; s++; }
  char rest[32];
  int i = 0;
  while (*s && *s != ' ' && i < 31) rest[i++] = *s++;
  rest[i] = 0;
  int type = -1;
  if (rest[0] == 0) {
    type = 0;
  } else {
    for (int j = 0; j < 100; j++) {
      const char* a = rest, *b = kChordTypes[j].name;
      bool match = true;
      while (*a && *b) {
        if (tolower(*a) != tolower(*b)) { match = false; break; }
        a++; b++;
      }
      if (match && !*a && !*b) { type = j; break; }
    }
  }
  if (type < 0) return -1;
  return root * 100 + type;
}

int ThreeBeatProblem::ParseScaleIdxStatic(const char* s) {
  if (!s) return -1;
  while (*s == ' ' || *s == '\t') s++;
  if (!*s) return -1;
  for (int j = 0; j < 80; j++) {
    const char* a = s, *b = kScales[j].name;
    bool match = true;
    while (*a && *b) {
      if (tolower(*a) != tolower(*b)) { match = false; break; }
      a++; b++;
    }
    if (match && !*a && !*b) return j;
  }
  return -1;
}

void ThreeBeatProblem::FormatChordIdStatic(int chordId, char* buf, int bufSize) {
  if (!buf || bufSize <= 0) return;
  int root = (chordId / 100) % 12;
  int type = chordId % 100;
  if (root < 0) root = 0;
  if (root > 11) root = 11;
  if (type < 0) type = 0;
  if (type >= 100) type = 0;
  snprintf(buf, bufSize, "%s%s", kNoteRoots[root], kChordTypes[type].name);
}

ThreeBeatProblem::ThreeBeatProblem(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, 1)) {
  // Init atomics (std::atomic has no copy, so can't use array initializer)
  for (int c = 0; c < kNumCircles; c++)
    mCurrentStepUIs[c].store(-1);

  // Per-circle params: Steps, Chord, Scale, OctMin, OctMax, PlayProb, Solo, Mute, FillGap, 16 step bools
  static const int chordDefaults[3] = {0, 700, 900}; // C/G/A maj7 per circle
  for (int c = 0; c < kNumCircles; c++) {
    char name[24];
    snprintf(name, sizeof(name), "C%d Steps", c + 1);
    GetParam(StepsParam(c))->InitInt(name, 8, 1, kMaxSteps);
    snprintf(name, sizeof(name), "C%d Chord", c + 1);
    GetParam(ChordParam(c))->InitInt(name, chordDefaults[c], 0, 1199); // root*100+type; 0=C maj7, 700=G maj7, 900=A maj7
    snprintf(name, sizeof(name), "C%d Scale", c + 1);
    GetParam(ScaleParam(c))->InitInt(name, 2, 0, 79);   // Ionian
    snprintf(name, sizeof(name), "C%d OctMin", c + 1);
    GetParam(OctMinParam(c))->InitInt(name, 4, 0, 9);
    snprintf(name, sizeof(name), "C%d OctMax", c + 1);
    GetParam(OctMaxParam(c))->InitInt(name, 5, 0, 9);
    snprintf(name, sizeof(name), "C%d PlayProb", c + 1);
    GetParam(PlayProbParam(c))->InitInt(name, 80, 0, 100);
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
    if (graphics->LoadFont("UI-Font", VOLUNMO_FN)
        || graphics->LoadFont("UI-Font", "Segoe UI", ETextStyle::Normal)
        || graphics->LoadFont("UI-Font", "Arial", ETextStyle::Normal)
        || graphics->LoadFont("UI-Font", "Verdana", ETextStyle::Normal))
      uiFont = "UI-Font";
    if (!uiFont) uiFont = "Arial"; // ponytail: null-guard so IText() never gets nullptr font name

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

    // ===== Side panel (LEFT, 240px wide) =====
    const float PANEL_W = 240.0f;
    const float PANEL_X0 = 0.0f;
    const float PANEL_X1 = PANEL_W;
    const float PANEL_Y0 = 96.0f;
    const float PANEL_Y1 = (float)PLUG_HEIGHT;

    graphics->AttachControl(new IPanelControl(IRECT(PANEL_X0, PANEL_Y0, PANEL_X1, PANEL_Y1),
                                              IColor(255, 20, 26, 30)));

    const float SECTION_H = (PANEL_Y1 - PANEL_Y0) / 3.0f;

    for (int c = 0; c < kNumCircles; c++) {
      const float secY0 = PANEL_Y0 + c * SECTION_H;

      char header[16];
      snprintf(header, sizeof(header), "CIRCLE %d", c + 1);
      graphics->AttachControl(new IVLabelControl(
        IRECT(PANEL_X0, secY0, PANEL_X1, secY0 + 24),
        header,
        DEFAULT_STYLE.WithValueText(IText(16.f, accent, uiFont, EAlign::Center, EVAlign::Middle))
                      .WithDrawFrame(false).WithDrawShadows(false)));

      mChordSelectors[c] = new ChordDisplayControl(
        IRECT(PANEL_X0 + 8, secY0 + 32, PANEL_X1 - 8, secY0 + 64),
        c, uiFont);
      graphics->AttachControl(mChordSelectors[c]);

      mScaleSelectors[c] = new ScaleDisplayControl(
        IRECT(PANEL_X0 + 8, secY0 + 68, PANEL_X1 - 8, secY0 + 100),
        c, uiFont);
      graphics->AttachControl(mScaleSelectors[c]);

      graphics->AttachControl(new IVKnobControl(
        IRECT(PANEL_X0 + 8, secY0 + 104, PANEL_X0 + 68, secY0 + 164),
        OctMinParam(c), "OctMin", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(PANEL_X0 + 8 + 64, secY0 + 104, PANEL_X0 + 132, secY0 + 164),
        OctMaxParam(c), "OctMax", kStyle));
      graphics->AttachControl(new IVKnobControl(
        IRECT(PANEL_X0 + 8 + 128, secY0 + 104, PANEL_X0 + 196, secY0 + 164),
        PlayProbParam(c), "Prob", kStyle));
    }

    // ===== Per-circle columns (shifted right by panel width) =====
    const float mainX0Offset = PANEL_W + 20.0f;

    for (int c = 0; c < kNumCircles; c++) {
      const float x0 = mainX0Offset + c * colW;
      const float cx = x0 + colW / 2;

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
      mFills[c] = new FillControl(fillR, c, [this, c]() { this->FillCircle(c); }, uiFont);
      graphics->AttachControl(mFills[c]);

      // Toolbar Row 1: transform ops
      const float row1Y = fillY + fillH + 8;
      const float row1H = 32.0f;
      const float r1Gap = 6.0f;
      const float r1W = 50.0f, r1Inv = 50.0f;
      const float r1TotalW = 2 * r1W + r1Inv + 2 * r1Gap;
      const float r1X0 = cx - r1TotalW / 2;
      float rx1 = r1X0;
      mRotatesL[c] = new RotateControl(IRECT(rx1, row1Y, rx1 + r1W, row1Y + row1H), c, -1,
        [this, c]() { this->RotateCircle(c, -1); }, uiFont);
      rx1 += r1W + r1Gap;
      mRotatesR[c] = new RotateControl(IRECT(rx1, row1Y, rx1 + r1W, row1Y + row1H), c, +1,
        [this, c]() { this->RotateCircle(c, +1); }, uiFont);
      rx1 += r1W + r1Gap;
      mInverts[c] = new InvertControl(IRECT(rx1, row1Y, rx1 + r1Inv, row1Y + row1H), c,
        [this, c]() { this->InvertCircle(c); }, uiFont);
      graphics->AttachControl(mRotatesL[c]);
      graphics->AttachControl(mRotatesR[c]);
      graphics->AttachControl(mInverts[c]);

      // Toolbar Row 2: destructive/edit ops
      const float row2Y = row1Y + row1H + 4;
      const float row2H = 32.0f;
      const float r2Gap = 6.0f;
      const float r2Clr = 60.0f, r2Undo = 32.0f, r2Redo = 32.0f;
      const float r2TotalW = r2Clr + r2Undo + r2Redo + 2 * r2Gap;
      const float r2X0 = cx - r2TotalW / 2;
      float rx2 = r2X0;
      mClears[c] = new ClearControl(IRECT(rx2, row2Y, rx2 + r2Clr, row2Y + row2H), c,
        [this, c]() { this->ClearCircle(c); }, uiFont);
      rx2 += r2Clr + r2Gap;
      mUndos[c] = new UndoControl(IRECT(rx2, row2Y, rx2 + r2Undo, row2Y + row2H), c,
        [this, c]() { this->UndoCircle(c); }, uiFont);
      rx2 += r2Undo + r2Gap;
      mRedos[c] = new RedoControl(IRECT(rx2, row2Y, rx2 + r2Redo, row2Y + row2H), c,
        [this, c]() { this->RedoCircle(c); }, uiFont);
      graphics->AttachControl(mClears[c]);
      graphics->AttachControl(mUndos[c]);
      graphics->AttachControl(mRedos[c]);
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
    mCachedChordId[c] = GetParam(ChordParam(c))->Int();
    mCachedScaleId[c] = GetParam(ScaleParam(c))->Int();
    mCachedOctMin[c] = std::clamp(GetParam(OctMinParam(c))->Int(), 0, 9);
    mCachedOctMax[c] = std::clamp(GetParam(OctMaxParam(c))->Int(), 0, 9);
    mCachedPlayProb[c] = std::clamp(GetParam(PlayProbParam(c))->Int(), 0, 100);
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
          // PlayProb gating: skip step probabilistically
          if (mCachedPlayProb[c] < 100 && (RandXor() % 100u) >= (uint32_t)mCachedPlayProb[c]) {
            // step skipped — advance step counter, update UI, continue
            mCurrentSteps[c] = curStep;
            if (curStep % 2 == 1) {
              if (ppq >= firedPpq) mCurrentStepUIs[c].store(curStep);
            } else {
              const double evenBoundary = basePpq;
              if (ppq >= evenBoundary) mCurrentStepUIs[c].store(curStep);
            }
            prevSteps[c] = curStep;
            continue;
          }

          // Pick the note ONCE per step — reuse for voice + MIDI
          int chosenNote = PickRandomNote(c);

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
          mVoices[slot].mNote = chosenNote;
          mVoices[slot].mPhase = 0.0;
          mVoices[slot].mSamplesLeft = noteSamples[c];
          mVoices[slot].mTotalSamples = noteSamples[c];

          // Mutation: randomize velocity (uses thread-safe RNG, not rand())
          int velocity = 100;
          if (mCachedMutation > 0) {
            double range = (double(RandXor()) / (double)0xFFFFFFFFu * 2.0 - 1.0) * (mCachedMutation / 100.0) * 35.0;
            velocity = std::clamp((int)(100.0 + range), 1, 127);
          }
          IMidiMsg msgOn;
          msgOn.MakeNoteOnMsg(chosenNote, velocity, i);
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
  // Seed audio-thread PRNG from sample pos so pattern is non-repeating
  double sp = GetSamplePos();
  mRngState = (uint32_t)(sp * 1000.0) ^ 0x9E3779B9u;
  if (mRngState == 0) mRngState = 0xCAFEF00Du;
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

void ThreeBeatProblem::ClearCircle(int c) {
  bool arr[kMaxSteps] = {0};
  PushHistory(c);
  for (int s = 0; s < kMaxSteps; s++) {
    BeginInformHostOfParamChangeFromUI(StepParam(c, s));
    GetParam(StepParam(c, s))->Set(0.0);
    EndInformHostOfParamChangeFromUI(StepParam(c, s));
  }
  if (mCircles[c]) mCircles[c]->SetPattern(arr);
}

void ThreeBeatProblem::InvertCircle(int c) {
  bool arr[kMaxSteps];
  mCircles[c]->GetPattern(arr);
  PushHistoryExplicit(c, arr);
  for (int s = 0; s < kMaxSteps; s++) arr[s] = !arr[s];
  for (int s = 0; s < kMaxSteps; s++) {
    BeginInformHostOfParamChangeFromUI(StepParam(c, s));
    GetParam(StepParam(c, s))->Set(arr[s] ? 1.0 : 0.0);
    EndInformHostOfParamChangeFromUI(StepParam(c, s));
  }
  if (mCircles[c]) mCircles[c]->SetPattern(arr);
}

void ThreeBeatProblem::UndoCircle(int c) {
  if (mHistoryCount[c] == 0) return;
  if (mCircles[c]) {
    bool arr[kMaxSteps];
    mCircles[c]->GetPattern(arr);
    // Push current state to redo stack
    int redoIdx = mRedoStackHead[c];
    for (int s = 0; s < kMaxSteps; s++) mRedoStack[c][redoIdx][s] = arr[s];
    mRedoStackHead[c] = (mRedoStackHead[c] + 1) % kHistorySize;
    mRedoCount[c] = std::min(mRedoCount[c] + 1, kHistorySize - 1);
    // Pop from history
    mHistoryHead[c] = (mHistoryHead[c] - 1 + kHistorySize) % kHistorySize;
    mHistoryCount[c]--;
    // Restore state
    bool prev[kMaxSteps];
    for (int s = 0; s < kMaxSteps; s++) prev[s] = mHistory[c][mHistoryHead[c]][s];
    for (int s = 0; s < kMaxSteps; s++) {
      BeginInformHostOfParamChangeFromUI(StepParam(c, s));
      GetParam(StepParam(c, s))->Set(prev[s] ? 1.0 : 0.0);
      EndInformHostOfParamChangeFromUI(StepParam(c, s));
    }
    mCircles[c]->SetPattern(prev);
  }
}

void ThreeBeatProblem::RedoCircle(int c) {
  if (mRedoCount[c] == 0) return;
  int redoIdx = (mRedoStackHead[c] - mRedoCount[c] + kHistorySize) % kHistorySize;
  bool redoState[kMaxSteps];
  for (int s = 0; s < kMaxSteps; s++) redoState[s] = mRedoStack[c][redoIdx][s];
  for (int s = 0; s < kMaxSteps; s++) {
    BeginInformHostOfParamChangeFromUI(StepParam(c, s));
    GetParam(StepParam(c, s))->Set(redoState[s] ? 1.0 : 0.0);
    EndInformHostOfParamChangeFromUI(StepParam(c, s));
  }
  if (mCircles[c]) mCircles[c]->SetPattern(redoState);
  mRedoCount[c]--;
}

void ThreeBeatProblem::PushHistory(int c) {
  if (mCircles[c]) {
    bool arr[kMaxSteps];
    mCircles[c]->GetPattern(arr);
    PushHistoryExplicit(c, arr);
  }
}

void ThreeBeatProblem::PushHistoryExplicit(int c, const bool pre[kMaxSteps]) {
  for (int s = 0; s < kMaxSteps; s++) mHistory[c][mHistoryHead[c]][s] = pre[s];
  mHistoryHead[c] = (mHistoryHead[c] + 1) % kHistorySize;
  if (mHistoryCount[c] < kHistorySize) mHistoryCount[c]++;
  // Clear redo on new edit
  mRedoCount[c] = 0;
  mRedoStackHead[c] = mHistoryHead[c];
}

int ThreeBeatProblem::PickRandomNote(int c) {
  int chordId = mCachedChordId[c];
  int scaleId = mCachedScaleId[c];
  int octMin = mCachedOctMin[c];
  int octMax = mCachedOctMax[c];

  int root = (chordId / 100) % 12;
  int type = chordId % 100;
  if (root < 0 || root > 11) root = 0;
  if (type < 0 || type >= 100) type = 0;
  if (scaleId < 0 || scaleId >= 80) scaleId = 0;

  const ScaleType& sc = kScales[scaleId];
  const ChordType& ct = kChordTypes[type];

  // Build pool: notes within [octMin..octMax] semitones that are in scale
  int pool[128];
  int poolSize = 0;
  for (int oct = octMin; oct <= octMax; oct++) {
    int base = oct * 12;
    // Add chord tones
    for (int i = 0; i < ct.n && poolSize < 128; i++) {
      int n = base + root + ct.intervals[i];
      if (n >= 0 && n < 128) { pool[poolSize++] = n; }
    }
    // Add scale tones
    for (int i = 0; i < sc.n && poolSize < 128; i++) {
      int n = base + sc.intervals[i];
      if (n >= 0 && n < 128) {
        bool found = false;
        for (int j = 0; j < poolSize; j++) if (pool[j] == n) { found = true; break; }
        if (!found) pool[poolSize++] = n;
      }
    }
  }

  if (poolSize == 0) return 60; // fallback middle C
  return pool[RandXor() % (uint32_t)poolSize];
}
#endif
