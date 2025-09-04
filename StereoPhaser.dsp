declare name "Stereo Phaser";
declare author "CLOPH";
declare description "Four-stage stereo phaser with feedback and LFO modulation";
import("stdfaust.lib");
import("filters.lib");

// UI
rateHz      = hslider("h:[1]Main/[1]Rate [unit:Hz][style:knob]", 0.5, 0.01, 5, 0.01) : si.smoo;
depth       = hslider("h:[1]Main/[2]Depth [unit:0..1][style:knob]", 0.6, 0, 1, 0.01) : si.smoo;
feedback    = hslider("h:[1]Main/[3]Feedback [unit:0..1][style:knob]", 0.5, 0, 0.95, 0.01) : si.smoo;
centerHz    = hslider("h:[2]Tone/[1]Center Freq [unit:Hz][style:knob]", 800, 100, 4000, 1) : si.smoo;
spreadOct   = hslider("h:[2]Tone/[2]Spread [unit:oct][style:knob]", 2, 0, 4, 0.1) : si.smoo;
mix         = hslider("h:[3]Mix/[1]Wet/Dry [style:knob]", 0.5, 0, 1, 0.01) : si.smoo;
bypass      = checkbox("h:[3]Mix/[2]Bypass");
phaseOffDeg = hslider("h:[1]Main/[4]Stereo Phase Offset [unit:deg][style:knob]", 90, 0, 180, 1) : si.smoo;

// Helpers
limitFreq(f) = max(20, min(f, ma.SR*0.45));
modLFO(f)    = os.osc(f) ; // -1..1
toInt(x)     = int(x + 0.5);

// Compute modulation factor around 1.0 on a log scale (octaves)
modFactor(lfo) = pow(2.0, lfo * depth * spreadOct);

// Allpass comb stages from filters.lib (floating delay)
apGain = 0.7; // internal stage feedback (0..1)
targetMax = toInt(ma.SR * 0.05); // ~50ms
maxD = ma.np2(targetMax); // power-of-two as required by allpass_fcomb
freq2delSamp(f) = ma.SR / limitFreq(f);
clampDelSamp(d) = max(1.0, min(d, maxD - 2));
apStage(del) = fi.allpass_fcomb(maxD, clampDelSamp(del), apGain);

// 4-stage chain with modulated delays
phaserChain(lfo) = apStage(freq2delSamp(centerHz*0.5*modFactor(lfo)))
                 : apStage(freq2delSamp(centerHz*1.0*modFactor(lfo)))
                 : apStage(freq2delSamp(centerHz*2.0*modFactor(lfo)))
                 : apStage(freq2delSamp(centerHz*4.0*modFactor(lfo)));

// Feedback phaser topology: y = x + feedback * phaser(y)
phaserFB(lfo) = + ~ ( phaserChain(lfo) : *(feedback) );

// Stereo LFOs with phase offset
phaseOffSamp = (phaseOffDeg/360.0) * (ma.SR / max(0.01, rateHz));
lfoL = modLFO(rateHz);
lfoR = lfoL : de.fdelay(ma.SR, phaseOffSamp);

// Stereo process with proper wet/dry mixing
process = ba.bypass2(
  bypass,
  (
    // branch input to wet and dry paths
    _,_ <: (phaserFB(lfoL), phaserFB(lfoR)) , _,_
    : _*(mix), _*(mix), _*(1-mix), _*(1-mix)
    : +, +
  )
);
