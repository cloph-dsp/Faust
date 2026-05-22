declare name "Granul8or";
declare author "CLOPH";
declare description "Granular Synthesis Engine";
import("stdfaust.lib");
granulator = * <: stereo_grain_engine : (_,_)
with {
    // MAIN CONTROLS
    speed = hgroup("[1]Main Controls", hslider("[1]Speed[style:knob]", 1, 0.01, 20, 0.01));
    position = hgroup("[1]Main Controls", hslider("[2]Position[style:knob]", 0.5, 0, 1, 0.01));
    grainSize = hgroup("[1]Main Controls", hslider("[3]Grain Size[style:knob]", 0.1, 0.001, 1, 0.001));
    freeze = hgroup("[1]Main Controls", checkbox("[4]Freeze"));
    smooth_mode = hgroup("[1]Main Controls", checkbox("[5]Smooth Mode"));
    smooth_factor = hgroup("[1]Main Controls", hslider("[6]Smooth Factor[scale:exp][style:knob]", 0.95, 0.95, 0.999, 0.0001));
    
    // MODULATE
    chaos = hgroup("[2]Modulation", hslider("[1]Chaos[style:knob]", 0, 0, 1, 0.01));
    jitter = hgroup("[2]Modulation", hslider("[2]Time Jitter[style:knob]", 0, 0, 1, 0.01));
    lfoRate = hgroup("[2]Modulation", hslider("[3]LFO Rate[style:knob]", 0.1, 0.01, 10, 0.01));
    lfoDepth = hgroup("[2]Modulation", hslider("[4]LFO Depth[style:knob]", 0, 0, 1, 0.01));
    envelope_type = hgroup("[2]Modulation", hslider("[5]Envelope Type[style:knob]", 0, 0, 3, 1));
    
    // FX
    spread = hgroup("[3]Effects", hslider("[1]Stereo Spread[style:knob]", 0, 0, 1, 0.01));
    pitch = hgroup("[3]Effects", hslider("[2]Pitch Shift[style:knob]", 0, -12, 12, 0.1));
    resonance = hgroup("[3]Effects", hslider("[3]Resonance[style:knob]", 0, 0, 0.9, 0.01));
    bitcrush = hgroup("[3]Effects", hslider("[4]Bit Crush[style:knob]", 0.01, 0.01, 1, 0.01));
    
    // Buffer
    bufferSize = 96000;
    
    // Soothing
    adaptive_smooth(x) = select2(smooth_mode, x, si.smooth(smooth_factor, x));
    
    // Random gen
    noise1 = no.noise : *(chaos) : adaptive_smooth;
    noise2 = no.noise : *(jitter) : adaptive_smooth;
    
    // LFO
    lfo = os.osc(lfoRate) : *(lfoDepth) : adaptive_smooth;
    
    // Pitch shift
    pitchRatio = ba.semi2ratio(pitch) : adaptive_smooth;
    
    // Phaser
    randomizedSpeed = speed * (1 + (noise2 : adaptive_smooth)) * pitchRatio;
    basePhase = os.phasor(1, freeze * 0 + (1 - freeze) * randomizedSpeed/ma.SR);
    
    // Position
    randomizedPosition = position + (noise1 : adaptive_smooth) + lfo : max(0) : min(1);
    phase = select2(freeze, basePhase, randomizedPosition);
    
    // Grain envelope
    grain_env = (
        en.ar(0.001, grainSize, 1.0) * (1-envelope_type)/4 +
        (os.osc(1.0/grainSize) : +(1) * 0.5) * (2-envelope_type)/4 +
        en.asr(0.1*grainSize, 0.5, 0.4*grainSize, 1.0) * (3-envelope_type)/4 +
        (no.noise : fi.lowpass(1, 1.0/grainSize)) * (4-envelope_type)/4
    ) : adaptive_smooth;
    
    // Resonant filter
    resonator(x) = fi.resonlp(freq_mod, resonance * 5 + 0.7, 1.0, x)
    with {
        freq_mod = 200 + 4000 * (lfo + 0.5) : adaptive_smooth;
    };
    
    // Calculate buffer indices
    readPos = ba.period(bufferSize) * phase;
    smoothedPos = readPos : adaptive_smooth;
    index = int(smoothedPos);
    frac = smoothedPos - index;
    
    // Stereo grain engine
    stereo_grain_engine = * <: (left_channel, right_channel)
    with {
        left_channel = process_channel(0);
        right_channel = process_channel(spread);
        
        process_channel(offset) = (
            de.fdelay(bufferSize, index + (offset * 100)), 
            de.fdelay(bufferSize, index + 1 + (offset * 100)) 
            : interpolate
        ) * grain_env : resonator : quantize;
        
        interpolate(a, b) = a * (1 - frac) + b * frac;
        
        // Bit crushing
        quantize(x) = floor(x * steps) / steps
        with {
            steps = pow(2, (1.01 - bitcrush) * 16) : adaptive_smooth;
        };
    };
};
process = granulator;
