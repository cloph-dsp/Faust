declare name "Stereo Phaser (Amplitude-Dependent)";
declare author "CLOPH";
declare description "Self-contained stereo phaser with amplitude-dependent modulation";
import("stdfaust.lib");

//==============================================================================
// UI Controls
//==============================================================================
rate = hslider("h:[1]Controls/[0]Rate [unit:Hz][style:knob]", 0.5, 0.01, 5.0, 0.01) : si.smoo;
depth = hslider("h:[1]Controls/[1]Depth [style:knob]", 1.0, 0.0, 2.0, 0.01) : si.smoo;
feedback_gain = hslider("h:[1]Controls/[2]Feedback [style:knob]", 0.0, -0.95, 0.95, 0.01) : si.smoo;
stages_sel = nentry("h:[1]Controls/[3]Stages[style:menu{'4 Stages':0;'6 Stages':1;'8 Stages':2}]", 1, 0, 2, 1) : int;

width = hslider("h:[2]Tone/[0]Width [unit:Hz][style:knob]", 50, 1, 2000, 1) : si.smoo;
frqmin = hslider("h:[2]Tone/[1]Min Freq [unit:Hz][style:knob]", 400, 20, 4000, 1) : si.smoo;
fratio = hslider("h:[2]Tone/[2]Freq Ratio [style:knob]", 1.5, 1.01, 3.0, 0.01) : si.smoo;

// Amplitude tracking controls
env_attack = hslider("h:[2]Tone/[3]Env Attack [unit:ms][style:knob]", 10, 1, 100, 1) : si.smoo;
env_release = hslider("h:[2]Tone/[4]Env Release [unit:ms][style:knob]", 100, 10, 1000, 10) : si.smoo;
env_sensitivity = hslider("h:[2]Tone/[5]Env Sensitivity [style:knob]", 1.0, 0.0, 2.0, 0.01) : si.smoo;
env_inverse = checkbox("h:[2]Tone/[6]Inverse Mode");

mix = hslider("h:[3]Mix/[0]Wet/Dry [style:knob]", 0.5, 0.0, 1.0, 0.01) : si.smoo;
bypass_switch = checkbox("h:[3]Mix/[1]Bypass");

//==============================================================================
// Envelope follower for amplitude tracking
//==============================================================================
envelope_follower(att, rel) = abs : si.smooth(ba.tau2pole(att/1000)) : si.smooth(ba.tau2pole(rel/1000));

//==============================================================================
// Simple allpass filter implementation - EXACT from working version
//==============================================================================
allpass_filter(delay_samples, feedback) = (+ <: de.fdelay(4096, delay_samples), *(feedback)) ~ *(-feedback) : mem, _ : +;

//==============================================================================
// Single phaser stage with frequency-based control
//==============================================================================
phaser_stage_freq(lfo_val, stage_num) = allpass_filter(delay_time, 0.7)
with {
    // Use width, frqmin, and fratio for frequency-based modulation
    base_freq = frqmin * pow(fratio, stage_num);
    modulated_freq = base_freq + (lfo_val * width);
    clamped_freq = max(20, min(20000, modulated_freq));
    
    // Convert frequency to delay time (samples)
    delay_time = ma.SR / (4.0 * clamped_freq);
};

//==============================================================================
// Single phaser stage - time-based (original)
//==============================================================================
phaser_stage(lfo_val) = allpass_filter(delay_time, 0.7)
with {
    center_delay = 1.0;
    depth_delay = 0.8;
    delay_time = (center_delay + lfo_val * depth_delay) * ma.SR / 1000.0;
};

//==============================================================================
// Mono phaser chains with feedback added
//==============================================================================
phaser4_chain(lfo_val, fb) = (+ : seq(i, 4, phaser_stage_freq(lfo_val, i))) ~ *(fb);
phaser6_chain(lfo_val, fb) = (+ : seq(i, 6, phaser_stage_freq(lfo_val, i))) ~ *(fb);
phaser8_chain(lfo_val, fb) = (+ : seq(i, 8, phaser_stage_freq(lfo_val, i))) ~ *(fb);

//==============================================================================
// Stereo phaser processing with amplitude modulation
//==============================================================================
// Base LFO generators
lfo_left = os.osc(rate) * depth;
lfo_right = os.osc(rate + 0.01) * depth;

// Mixing gains
dry_gain = 1 - depth/2;
wet_gain = depth/2;

// Process channel helper
process_channel(base_lfo, input) = final_out
with {
    // Extract envelope and modulate LFO depth
    env = input : envelope_follower(env_attack, env_release) : *(env_sensitivity);
    
    // Apply inverse mode if enabled (1.0 - env instead of env)
    env_mod = select2(env_inverse, env, 1.0 - env);
    
    // Modulate LFO: blend from 30% to 100% based on envelope
    modulated_lfo = base_lfo * (0.3 + env_mod * 0.7);
    
    // Apply phaser with feedback - select the right chain
    wet = ba.if(stages_sel == 0,
            phaser4_chain(modulated_lfo, feedback_gain, input),
            ba.if(stages_sel == 1,
                phaser6_chain(modulated_lfo, feedback_gain, input),
                phaser8_chain(modulated_lfo, feedback_gain, input)
            )
        );
    
    // Mix wet and dry
    mixed = (wet * wet_gain) + (input * dry_gain);
    
    // Bypass
    bypassed = select2(bypass_switch, mixed, input);
    
    // Final wet/dry mix
    final_out = (bypassed * mix) + (input * (1 - mix));
};

// Process with amplitude dependency
process = _, _ : process_channel(lfo_left), process_channel(lfo_right);