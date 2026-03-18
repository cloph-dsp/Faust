import("stdfaust.lib");

// L-R8 Stereo Shifter

// === UTILITY FUNCTIONS (Enhanced Stereo without M/S) ===
// Linkwitz-Riley crossovers for better phase coherence
lr_lp(f) = fi.lowpass(2, f) : fi.lowpass(2, f);
lr_hp(f) = fi.highpass(2, f) : fi.highpass(2, f);

// All-pass filters for phase decorrelation (creates stereo width)
ap_chain(freq, q) = fi.tf2(b0, b1, b2, a1, a2)
with {
    w = 2.0 * ma.PI * freq / ma.SR;
    alpha = sin(w) / (2.0 * q);
    b0 = 1.0 - alpha;
    b1 = -2.0 * cos(w);
    b2 = 1.0 + alpha;
    a1 = -2.0 * cos(w);
    a2 = 1.0 - alpha;
};

// Cascaded all-pass for richer phase relationships
stereo_decorr(x) = x : ap_chain(800, 0.7) : ap_chain(1600, 0.7) : ap_chain(3200, 0.7);

// Fixed delay times (will be calculated properly in process function)
base_delay_l = 20;  // 20ms base delay for left
base_delay_r = 40;  // 40ms base delay for right

// Fixed safe buffer sizes 
max_delay_samps = 2048;  // Fixed 2048 samples (~46ms at 44.1kHz)

// Multiple LFOs for organic stereo movement
mod_lfo1 = os.osc(0.23);    // Primary slow modulation
mod_lfo2 = os.osc(0.37);    // Secondary modulation (different rate)
mod_lfo3 = os.osc(0.13);    // Tertiary very slow modulation

// Frequency-dependent modulation depths
low_mod_depth = 0.015;      // Subtle for low frequencies
mid_mod_depth = 0.025;      // Moderate for mid frequencies  
high_mod_depth = 0.04;      // More pronounced for high frequencies

// Haas effect - subtle timing differences for stereo imaging
haas_delay_l = int(0.5 * ma.SR / 1000);  // 0.5ms delay
haas_delay_r = int(0.8 * ma.SR / 1000);  // 0.8ms delay

// === MAIN CONTROLS ===
mainGroup(x) = hgroup("[0]Main", x);
timingGroup(x) = hgroup("[1]Timing", x);
freqGroup(x) = hgroup("[2]Frequency", x);

bypass = mainGroup(checkbox("[0]Bypass"));

// === TIMING CONTROLS ===
// Order: Sync, Rate, BPM, Free Rate, Crossfade
use_host_sync = timingGroup(checkbox("[0]Sync"));
rate_param    = timingGroup(hslider("[1]Rate", 7, 0, 14, 1));
bpm_param     = timingGroup(hslider("[2]BPM", 120, 1, 300, 1));
free_rate     = timingGroup(hslider("[3]Free Rate", 2, 0.1, 10, 0.1) : si.smoo);
cross_fade    = timingGroup(hslider("[4]Cross", 0, 0, 1, 0.01) : si.smoo);

// === FREQUENCY CONTROLS ===
highpass_freq  = freqGroup(hslider("[0]High Pass", 1000, 20, 1000, 1) : si.smoo);
enhance_cutoff = freqGroup(hslider("[1]Low Pass", 8000, 1000, 20000, 1) : si.smoo);

// Rate multiplier lookup table (matching PD patch select object)
rate_multiplier = ba.selectn(15, int(rate_param),
    0.015625, 0.03125, 0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64, 128, 256
);

// BPM calculation with proper sync handling
effective_bpm = select2(use_host_sync, 120, bpm_param);  // When OFF use default 120, when ON use manual BPM
// When sync is OFF, use free rate, when sync is ON, use BPM + rate
tempo_hz = select2(use_host_sync, free_rate, effective_bpm * rate_multiplier / 60);

// LFO for stereo shifting (triangle wave as in PD)
lfo = os.lf_triangle(tempo_hz);

// Main processing with enhanced stereo imaging
process(l, r) = select2(bypass, out_l, l), select2(bypass, out_r, r)
with {
    // Calculate delay times properly inside process function
    beat_time_ms = select2(use_host_sync, 1000 / free_rate, 60000 / (effective_bpm * rate_multiplier));
    delay_time_l = select2(use_host_sync, base_delay_l, beat_time_ms * 0.05);
    delay_time_r = select2(use_host_sync, base_delay_r, beat_time_ms * 0.1);
    
    // Convert to samples with safety limits
    delay_samps_l = int(delay_time_l * ma.SR / 1000) : max(1) : min(2000);
    delay_samps_r = int(delay_time_r * ma.SR / 1000) : max(1) : min(2000);
    
    // Simple frequency split (remove dynamic modulation for now)
    low_l = l : fi.lowpass(2, highpass_freq);  
    low_r = r : fi.lowpass(2, highpass_freq);
    
    // All high frequencies (above highpass)
    all_high_l = l : fi.highpass(2, highpass_freq);
    all_high_r = r : fi.highpass(2, highpass_freq);
    
    // Apply delays to high frequencies for stereo shifting
    all_high_l_delayed = all_high_l : de.delay(max_delay_samps, delay_samps_l);
    all_high_r_delayed = all_high_r : de.delay(max_delay_samps, delay_samps_r);
    
    // Simple LFO for shifting control with proper crossfade
    raw_lfo = os.lf_triangle(tempo_hz);
    
        // Improved crossfade logic for true stereo swap
        // hard_switch: +1 for swap, -1 for no swap
        hard_switch = select2(raw_lfo >= 0, 1, -1);
        smooth_switch = raw_lfo;
        // Blend between hard and smooth
        shift_amount = (1 - cross_fade) * hard_switch + cross_fade * smooth_switch;
        // At hard_switch=1: swap L/R, at -1: no swap
        all_high_l_hard = select2(hard_switch == 1, all_high_r_delayed, all_high_l_delayed);
        all_high_r_hard = select2(hard_switch == 1, all_high_l_delayed, all_high_r_delayed);
    // For smooth crossfade, use symmetric formula
    // shift_amount in [-1, 1]
    all_high_l_smooth = all_high_l_delayed * ((1 - shift_amount) / 2) + all_high_r_delayed * ((1 + shift_amount) / 2);
    all_high_r_smooth = all_high_r_delayed * ((1 - shift_amount) / 2) + all_high_l_delayed * ((1 + shift_amount) / 2);
        // Blend between hard and smooth
        all_high_l_shifted = (1 - cross_fade) * all_high_l_hard + cross_fade * all_high_l_smooth;
        all_high_r_shifted = (1 - cross_fade) * all_high_r_hard + cross_fade * all_high_r_smooth;
    
    
    // Split shifted result: mid frequencies normal, high frequencies get enhancement
    mid_l_shifted = all_high_l_shifted : fi.lowpass(2, enhance_cutoff);
    mid_r_shifted = all_high_r_shifted : fi.lowpass(2, enhance_cutoff);
    
    // High frequencies (ABOVE enhance_cutoff) get additional enhancement
    high_l_basic = all_high_l_shifted : fi.highpass(2, enhance_cutoff);
    high_r_basic = all_high_r_shifted : fi.highpass(2, enhance_cutoff);
    
    // Simple enhancement: just stronger cross-mixing for high frequencies
    enhanced_shift_l = max(0, shift_amount) * 1.3 : min(1.0);  // 30% enhancement
    enhanced_shift_r = max(0, -shift_amount) * 1.3 : min(1.0);
    
    high_l_enhanced = high_l_basic * (1 - enhanced_shift_r) + high_r_basic * enhanced_shift_r;
    high_r_enhanced = high_r_basic * (1 - enhanced_shift_l) + high_l_basic * enhanced_shift_l;
    
    // Combine all frequency bands
    out_l = low_l + mid_l_shifted + high_l_enhanced;
    out_r = low_r + mid_r_shifted + high_r_enhanced;
};