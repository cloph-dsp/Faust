// =============================================================================
// TunerDSP.dsp - Minimal audio pass-through for the Tuner VST.
//
// Pitch detection, RMS metering, and all analysis live in C++ inside Tuner.cpp.
// This .dsp just passes audio through unchanged -- a tuner is a transparent
// analysis utility and must not alter the signal.
//
// IMPORTANT: iPlug2 allocates output buffers sized to PLUG_CHANNEL_IO (2 in
// our case).  If we declared extra process() outputs (e.g. an RMS sidechain),
// Faust would write past the end of outputs[] and corrupt host memory.  We
// therefore keep process() strictly 2-in/2-out and do all analysis in C++.
// =============================================================================

declare name "TunerDSP";
declare version "1.0";
declare description "Transparent stereo pass-through for iPlug2 VST3 tuner host";
declare author "CLOPH";
declare license "MIT";

import("stdfaust.lib");

// Pure pass-through.  Both channels pass through with gain 1.0 and zero phase
// shift -- no DC blocker, no pre-emphasis, no level meter on the audio side.
// All analysis lives in Tuner.cpp's Detector class.
process(inL, inR) = (inL, inR);