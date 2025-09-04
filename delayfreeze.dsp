declare filename "DelayFreeze.dsp";
declare name "DelayFreeze";
import("stdfaust.lib");

// ----- Delay Controls -----
delayFreeze = checkbox("t:[1]Freeze/v:[0]Main/[0]Freeze [tooltip: Pause the delay line]");
delayFeedback = hslider("t:[0]Delay/v:[2]Feedback/[0]Delay Feedback [tooltip: Adjust the amount of feedback] [style:knob]", 0.6, 0, 1, 0.01);
delayLengthMs = hslider("t:[0]Delay/v:[0]Time/[2]Delay Length [unit:ms] [tooltip: Set delay length in milliseconds]", 500, 10, 5000, 1);
delaySync = checkbox("t:[0]Delay/v:[0]Time/[0]Delay Tempo Sync [tooltip: Sync delay time to project tempo]");
delayBeatDiv = hslider("t:[0]Delay/v:[0]Time/[1]Delay Beat Division [tooltip: Set delay length in beats][style:menu{'1/32':0.03125; '1/16':0.0625; '1/8':0.125; '1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8; '16/1':16}]", 1, 0.03125, 16, 0.0001);

// ----- Freeze Controls -----
freezeLengthMs = hslider("t:[1]Freeze/v:[1]Time/[2]Freeze Length [unit:ms] [tooltip: Set freeze length in milliseconds]", 1000, 50, 60000, 1);
freezeSync = checkbox("t:[1]Freeze/v:[1]Time/[0]Freeze Tempo Sync [tooltip: Sync freeze time to project tempo]");
freezeBeatDiv = hslider("t:[1]Freeze/v:[1]Time/[1]Freeze Beat Division [tooltip: Set freeze length in beats][style:menu{'1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8; '16/1':16; '32/1':32}]", 1, 0.25, 32, 0.01);
freezeUseOwnTime = checkbox("t:[1]Freeze/v:[0]Main/[1]Use Freeze Time (override delay time) [tooltip: When enabled, engaging Freeze switches the delay time to the Freeze Time]");
freezeFeedback = hslider("t:[1]Freeze/v:[2]Hold/[0]Freeze Feedback [tooltip: Feedback while frozen; 1.0 is infinite hold]", 0.999, 0.9, 1.0, 0.001);
freezeMuteInput = checkbox("t:[1]Freeze/v:[0]Main/[2]Mute Input While Frozen [tooltip: If enabled, input is muted during Freeze; disable for overdub]") : int;

// ----- Shared Controls -----
bpm = nentry("t:[2]General/v:[0]Timing/[0]BPM [tooltip: Set project tempo for sync]", 120, 40, 300, 1);
timeGlideMs = hslider("t:[2]General/v:[0]Timing/[1]Time Glide [unit:ms] [tooltip: Smoothing time for delay-time changes to avoid clicks]", 30, 0, 200, 1);
filterFreq = hslider("t:[2]General/v:[1]Tone & Mix/[0]Filter Frequency [unit:Hz] [tooltip: Cutoff for low-pass in feedback path] [style:knob]", 5000, 20, 20000, 1);
wetDry = hslider("t:[2]General/v:[1]Tone & Mix/[1]Wet/Dry Mix [tooltip: Blend dry and wet signal] [style:knob]", 0.5, 0, 1, 0.01);
delayPan = hslider("t:[0]Delay/v:[3]Pan/[0]Delay Pan [tooltip: Pan the delayed signal] [style:knob]", 0, -1, 1, 0.01);

// ----- Time helpers -----
ms2samp(x) = x * ma.SR / 1000.0;
beats2ms(bpm, beats) = (60.0 / bpm) * beats * 1000.0;

// ----- Time calculations (ms -> samples) -----
delayTimeMs = ba.if(delaySync, beats2ms(bpm, delayBeatDiv), delayLengthMs);
freezeTimeMs = ba.if(freezeSync, beats2ms(bpm, freezeBeatDiv), freezeLengthMs);

// When frozen, by default keep the current delay time to preserve the buffer and avoid dropouts.
// If 'Use Freeze Time' is enabled, switch to the freeze time.
useFreezeTime = (delayFreeze * freezeUseOwnTime);
targetTimeMs = ba.if(useFreezeTime, freezeTimeMs, delayTimeMs);

// Max delay and clamped sample delay
maxdelay = ma.SR * 20; // Max ~20 seconds (lower memory footprint)

// Smooth time changes to avoid clicks when switching times
targetSamples = ms2samp(targetTimeMs);
timePole = ba.tau2pole(timeGlideMs/1000.0);
smoothedSamples = targetSamples : si.smooth(timePole);
currentSamples = max(1.0, min(smoothedSamples, maxdelay - 1));

// ----- Core blocks -----
lpf = fi.lowpass(3, filterFreq);
dc_blocker = fi.highpass(1, 20);
feedback_gain = ba.if(delayFreeze, freezeFeedback, delayFeedback);

// Per-channel delay engine with freeze
input_signal = ba.if(delayFreeze * freezeMuteInput, 0, _);
delay_freeze_line = input_signal : (+ ~ (de.fdelay(maxdelay, currentSamples) : lpf : dc_blocker : *(feedback_gain)));

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
  dry = _;
  wet = delay_freeze_line;
};
