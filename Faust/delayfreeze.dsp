declare filename "DelayFreeze.dsp";
declare name "DelayFreeze";
declare version "2.0";
declare author "Updated for improved timing stability";
import("stdfaust.lib");

// ===== UI CONTROLS =====

// ----- Main Controls (Tab 0) -----
delayFreeze = checkbox("t:[0]Main/h:[0]Freeze Control/[0]Freeze [tooltip: Pause the delay line]");
freezeUseOwnTime = checkbox("t:[0]Main/h:[0]Freeze Control/[1]Use Freeze Time [tooltip: When enabled, engaging Freeze switches to the Freeze Time setting]");
freezeMuteInput = checkbox("t:[0]Main/h:[0]Freeze Control/[2]Mute Input While Frozen [tooltip: If enabled, input is muted during Freeze; disable for overdub]") : int;

wetDry = hslider("t:[0]Main/h:[1]Mix & Output/[0]Wet/Dry Mix [tooltip: Blend dry and wet signal] [style:knob]", 0.5, 0, 1, 0.01);
delayPan = hslider("t:[0]Main/h:[1]Mix & Output/[1]Delay Pan [tooltip: Pan the delayed signal] [style:knob]", 0, -1, 1, 0.01);

// ----- Delay Time (Tab 1) -----
delaySync = checkbox("t:[1]Delay Time/h:[0]Normal Delay/[0]Tempo Sync [tooltip: Sync delay time to project tempo]");
delayLengthMs = hslider("t:[1]Delay Time/h:[0]Normal Delay/[1]Delay Length [unit:ms] [tooltip: Set delay length in milliseconds]", 500, 10, 5000, 1);
delayBeatDiv = hslider("t:[1]Delay Time/h:[0]Normal Delay/[2]Beat Division [tooltip: Set delay length in beats][style:menu{'1/32':0.03125; '1/16':0.0625; '1/8':0.125; '1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8; '16/1':16}]", 1, 0.03125, 16, 0.0001);

freezeSync = checkbox("t:[1]Delay Time/h:[1]Freeze Time/[0]Tempo Sync [tooltip: Sync freeze time to project tempo]");
freezeLengthMs = hslider("t:[1]Delay Time/h:[1]Freeze Time/[1]Freeze Length [unit:ms] [tooltip: Set freeze length in milliseconds]", 1000, 50, 60000, 1);
freezeBeatDiv = hslider("t:[1]Delay Time/h:[1]Freeze Time/[2]Beat Division [tooltip: Set freeze length in beats][style:menu{'1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8; '16/1':16; '32/1':32}]", 1, 0.25, 32, 0.01);

bpm = nentry("t:[1]Delay Time/h:[2]Timing/[0]BPM [tooltip: Set project tempo for sync]", 120, 40, 300, 1);
timeGlideMs = hslider("t:[1]Delay Time/h:[2]Timing/[1]Time Glide [unit:ms] [tooltip: Smoothing time for delay-time changes to avoid clicks]", 30, 0, 200, 1);

// ----- Feedback (Tab 2) -----
delayFeedback = hslider("t:[2]Feedback/h:[0]Normal Mode/[0]Delay Feedback [tooltip: Adjust the amount of feedback] [style:knob]", 0.6, 0, 1, 0.01);
freezeFeedback = hslider("t:[2]Feedback/h:[1]Freeze Mode/[0]Freeze Feedback [tooltip: Feedback while frozen; close to 1.0 for infinite hold] [style:knob]", 0.99999, 0.9, 0.99999, 0.00001);
filterFreq = hslider("t:[2]Feedback/h:[2]Tone/[0]Filter Frequency [unit:Hz] [tooltip: Low-pass filter cutoff in feedback path] [style:knob]", 5000, 20, 20000, 1);

// ----- Time helpers -----
ms2samp(x) = x * ma.SR / 1000.0;
beats2ms(bpm, beats) = (60.0 / bpm) * beats * 1000.0;

// ----- Time calculations (ms -> samples) -----
delayTimeMs = ba.if(delaySync, beats2ms(bpm, delayBeatDiv), delayLengthMs);
freezeTimeMs = ba.if(freezeSync, beats2ms(bpm, freezeBeatDiv), freezeLengthMs);

// Max delay and clamped sample delay
maxdelay = ma.SR * 20; // Max ~20 seconds (lower memory footprint)

// Time selection logic - cleaner approach
selectedTimeMs = ba.if(freezeUseOwnTime, freezeTimeMs, delayTimeMs);
targetSamples = ms2samp(selectedTimeMs);

// Time smoothing with freeze-aware behavior
timePole = ba.tau2pole(timeGlideMs/1000.0);
smoothedTargetSamples = targetSamples : si.smooth(timePole);

// Sample and hold the *actual* delay time at the moment freeze is engaged
freeze_rising_edge = delayFreeze > delayFreeze';
// Hold the current delay buffer length (not the smoothed target)
actualDelaySamples = smoothedTargetSamples; // This is the real delay time in use
heldDelaySamples = actualDelaySamples : ba.sAndH(freeze_rising_edge);
currentSamples = max(1.0, min(ba.if(delayFreeze, heldDelaySamples, smoothedTargetSamples), maxdelay - 1));

// ----- Core blocks -----
lpf = fi.lowpass(3, filterFreq);
dc_blocker = fi.highpass(1, 20);

// Improved freeze-aware delay line
delay_freeze_line = input_signal : delay_with_freeze
with {
    // Input signal with optional muting during freeze
    input_signal = ba.if(delayFreeze * freezeMuteInput, 0, _);
    
    // Freeze-aware delay with proper feedback control
    delay_with_freeze = (+ : de.fdelay(maxdelay, currentSamples)) ~ feedback_path
    with {
        feedback_path = lpf : dc_blocker : feedback_control;
        // Smooth feedback transitions to avoid clicks
        feedback_target = ba.if(delayFreeze, freezeFeedback, delayFeedback);
        feedback_control = *(feedback_target : si.smooth(ba.tau2pole(0.01)));
    };
};

// Mixing the dry and wet signals with panning
mix_and_pan(dry, wet) = (dry * (1 - wetDry) + wet * wetDry) <: panner(delayPan);

// Equal-power panner function (mono in -> stereo out)
panner(pan) = _ <: *(gL), *(gR)
with {
  theta = (pan * 0.5 + 0.5) * ma.PI / 2.0;
  gL = cos(theta);
  gR = sin(theta);
};

// ----- Main processing function -----
process = par(i, 2, voice)
with {
  voice = _ <: (dry, wet) : mix_and_pan;
  dry = _; // Always pass the original input to dry
  // Only mute the input to the delay line when frozen and mute is enabled
  wet = (_ : ba.if(delayFreeze * freezeMuteInput, 0, _)) : delay_freeze_line;
};
