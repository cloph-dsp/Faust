declare name "Stereo Phaser";
declare author "CLOPH";
declare description "Self-contained stereo phaser based on standard phaser topology";
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

mix = hslider("h:[3]Mix/[0]Wet/Dry [style:knob]", 0.5, 0.0, 1.0, 0.01) : si.smoo;
bypass_switch = checkbox("h:[3]Mix/[1]Bypass");

//==============================================================================
// Simple allpass filter implementation
//==============================================================================
allpass_filter(delay_samples, feedback) = (+ <: de.fdelay(4096, delay_samples), *(feedback)) ~*(-feedback) : mem, _ : +;

//==============================================================================
// Single phaser stage
//==============================================================================
phaser_stage(lfo_val) = allpass_filter(delay_time, 0.7)
with {
    center_delay = 1.0; // center delay in milliseconds
    depth_delay = 0.8;  // depth of modulation in milliseconds
    delay_time = (center_delay + lfo_val * depth_delay) * ma.SR / 1000.0;
};

//==============================================================================
// Mono phaser chains with fixed stages (compile-time constants)
//==============================================================================
phaser4_chain(lfo_val) = seq(i, 4, phaser_stage(lfo_val));
phaser6_chain(lfo_val) = seq(i, 6, phaser_stage(lfo_val));
phaser8_chain(lfo_val) = seq(i, 8, phaser_stage(lfo_val));

//==============================================================================
// Stereo phaser processing
//==============================================================================
// Generate quadrature LFOs for stereo effect
lfo_left = os.osc(rate) * depth;
lfo_right = os.osc(rate + 0.01) * depth; // Slightly detuned for wider stereo image

// Select phaser chain based on stages selection
selected_chain(lfo_val) = ba.selectn(3, stages_sel, 
    phaser4_chain(lfo_val), 
    phaser6_chain(lfo_val), 
    phaser8_chain(lfo_val)
);

// Mixing gains
dry_gain = 1 - depth/2;
wet_gain = depth/2;

// Left channel processing
left_channel = selected_chain(lfo_left);

// Right channel processing  
right_channel = selected_chain(lfo_right);

// Manual bypass and mix implementation to avoid naming conflicts
process = _,_ : 
    (_ <: left_channel*wet_gain + _*dry_gain, _ : select2(bypass_switch) : _ <: *(mix), *(1-mix) : +),
    (_ <: right_channel*wet_gain + _*dry_gain, _ : select2(bypass_switch) : _ <: *(mix), *(1-mix) : +);
