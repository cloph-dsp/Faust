import("stdfaust.lib");

// Quantum Stutter Randomizer: Chaotic Buffer Underrun Emulator
// Based on stutter_simple.dsp: randomized segment capture with aggressive stutter repetition
// Mostly continuous stutter with occasional special effects layered on top
declare name "Quantum Stutter Randomizer";
declare author "Generated";
declare version "5.1";
declare description "Chaotic, band-split stutter/freeze with host sync, lo-fi, and underrun-style gaps.";

// Glide helpers: fast for performance knobs, slow for structural gain-type changes
fast_glide_ms = 8.0;   // quick response, click-free
slow_glide_ms = 80.0;  // gentle response for trims
smooth_ms(ms) = si.smooth(exp(-1.0 / max(1.0, ma.SR * ms / 1000.0)));

// Checkbox defaults to 0 in Faust; invert so effect starts engaged (1) until user clicks bypass
bypass_raw = checkbox("Bypass[tooltip: Toggle quantum stutter effect on/off]");
stutter_on = 1.0 - bypass_raw;
chaos_raw = hslider("Chaos[style:knob]", 0.5, 0, 1, 0.01);
chaos = chaos_raw : smooth_ms(fast_glide_ms);  // smoothed for audio curves; keep raw for instant gating/edges
lofi_knob_raw = hslider("Lo-Fi[style:knob]", 0.5, 0, 1, 0.01);
lofi_knob = lofi_knob_raw : smooth_ms(fast_glide_ms); // 0 = clean, 1 = max lo-fi
bpm_host = hslider("transport/bpm", 120, 20, 300, 0.1);  // host-provided BPM (fallback to 120 if unavailable)
sync_on = checkbox("Sync BPM[tooltip: Quantize stutter to host BPM grid]");
input_trim_db = hslider("Input Trim[style:knob][unit:dB]", 0, -12, 12, 0.1);
output_trim_db = hslider("Output Trim[style:knob][unit:dB]", 0, -12, 12, 0.1);
input_gain = ba.db2linear(input_trim_db) : smooth_ms(slow_glide_ms);
output_gain = ba.db2linear(output_trim_db) : smooth_ms(slow_glide_ms);
bypass_env = stutter_on : smooth_ms(fast_glide_ms); // smoothed bypass to avoid zippering
// Nonlinear chaos shaping: more controlled at low, much wilder near 1
chaos_sharp = pow(chaos, 1.6);
chaos_wild = pow(chaos, 2.2);
fidelity = 1.0 - chaos_sharp;  // 1 when calm -> pull stutter closer to source feel
lofi_amt = lofi_knob * (0.25 + 0.75 * chaos_sharp);  // auto-cleaner when chaos is calm
bpm_safe = ba.if(bpm_host > 1.0, bpm_host, 120.0);
calm_mode = max(0.0, 1.0 - chaos_raw / 0.1);  // instant calm detection from raw knob
simple_mode = max(0.0, 1.0 - chaos_raw / 0.2);  // instant simple detection from raw knob
calm_force = max(calm_mode, simple_mode);

// === RANDOM SOURCES (per-band independent LCG generators) ===
// Base set (legacy/global)
rnd1 = (+(111) ~ *(1103515245)) : +(12345) : % (2147483647) : /(2147483647.0);
rnd2 = (+(222) ~ *(1103515245)) : +(23456) : % (2147483647) : /(2147483647.0);
rnd3 = (+(333) ~ *(1103515245)) : +(34567) : % (2147483647) : /(2147483647.0);
rnd4 = (+(444) ~ *(1103515245)) : +(45678) : % (2147483647) : /(2147483647.0);
rnd5 = (+(555) ~ *(1103515245)) : +(56789) : % (2147483647) : /(2147483647.0);

// Low band seeds
rnd1L = (+(1011) ~ *(1103515245)) : +(13245) : % (2147483647) : /(2147483647.0);
rnd2L = (+(1022) ~ *(1103515245)) : +(14356) : % (2147483647) : /(2147483647.0);
rnd3L = (+(1033) ~ *(1103515245)) : +(15467) : % (2147483647) : /(2147483647.0);
rnd4L = (+(1044) ~ *(1103515245)) : +(16578) : % (2147483647) : /(2147483647.0);
rnd5L = (+(1055) ~ *(1103515245)) : +(17689) : % (2147483647) : /(2147483647.0);

// Mid band seeds
rnd1M = (+(2011) ~ *(1103515245)) : +(21245) : % (2147483647) : /(2147483647.0);
rnd2M = (+(2022) ~ *(1103515245)) : +(22356) : % (2147483647) : /(2147483647.0);
rnd3M = (+(2033) ~ *(1103515245)) : +(23467) : % (2147483647) : /(2147483647.0);
rnd4M = (+(2044) ~ *(1103515245)) : +(24578) : % (2147483647) : /(2147483647.0);
rnd5M = (+(2055) ~ *(1103515245)) : +(25689) : % (2147483647) : /(2147483647.0);

// High band seeds
rnd1H = (+(3011) ~ *(1103515245)) : +(31245) : % (2147483647) : /(2147483647.0);
rnd2H = (+(3022) ~ *(1103515245)) : +(32356) : % (2147483647) : /(2147483647.0);
rnd3H = (+(3033) ~ *(1103515245)) : +(33467) : % (2147483647) : /(2147483647.0);
rnd4H = (+(3044) ~ *(1103515245)) : +(34578) : % (2147483647) : /(2147483647.0);
rnd5H = (+(3055) ~ *(1103515245)) : +(35689) : % (2147483647) : /(2147483647.0);

// === UPDATE TRIGGER (triggers new stutter segments) ===
// Chaos knob controls update frequency: slow = ~60ms when calm (quickly reflects control moves), fast = 20ms
update_time_ms = max(60, 12000 - chaos_sharp * 11980);  // lower floor to prevent staleness
update_rate = max(1, int(update_time_ms * ma.SR / 1000));  // Convert to samples, avoid zero
counter = (+(1) : %(update_rate)) ~ _;

// Force an immediate refresh when chaos is moved (edge-detect on raw chaos)
chaos_prev = chaos_raw';
chaos_delta = abs(chaos_raw - chaos_prev);
chaos_moved = chaos_delta > 0.004;  // small threshold to ignore jitter
chaos_edge = max(0.0, chaos_moved - chaos_moved');  // rising edge impulse when knob moves

// Also refresh immediately when Lo-Fi is moved so S&H updates right away
lofi_prev = lofi_knob_raw';
lofi_delta = abs(lofi_knob_raw - lofi_prev);
lofi_moved = lofi_delta > 0.004;
lofi_edge = max(0.0, lofi_moved - lofi_moved');

trig = max(counter == 0, max(chaos_edge, lofi_edge));

// === SAMPLED RANDOM VALUES ===
r1 = ba.sAndH(trig, rnd1);  // Capture size selection
r2 = ba.sAndH(trig, rnd2);  // Hold duration + variance
r3 = ba.sAndH(trig, rnd3);  // Repeat rate (stutter speed)
r4 = ba.sAndH(trig, rnd4);  // Special effects mode
r5 = ba.sAndH(trig, rnd5);  // Extra variance

// Band-specific sampled randoms
r1_low = ba.sAndH(trig, rnd1L);
r2_low = ba.sAndH(trig, rnd2L);
r3_low = ba.sAndH(trig, rnd3L);
r4_low = ba.sAndH(trig, rnd4L);
r5_low = ba.sAndH(trig, rnd5L);

r1_mid = ba.sAndH(trig, rnd1M);
r2_mid = ba.sAndH(trig, rnd2M);
r3_mid = ba.sAndH(trig, rnd3M);
r4_mid = ba.sAndH(trig, rnd4M);
r5_mid = ba.sAndH(trig, rnd5M);

r1_high = ba.sAndH(trig, rnd1H);
r2_high = ba.sAndH(trig, rnd2H);
r3_high = ba.sAndH(trig, rnd3H);
r4_high = ba.sAndH(trig, rnd4H);
r5_high = ba.sAndH(trig, rnd5H);

// === SLOW RANDOM WALKS (5-20s) ===
walk_time_ms = 5000 + (1.0 - chaos) * 15000;  // 5s (chaos high) to 20s (chaos low)
walk_rate = max(1, int(walk_time_ms * ma.SR / 1000));
walk_counter = (+(1) : %(walk_rate)) ~ _;
walk_trig = (walk_counter == 0);
walk_step_repeat = (ba.sAndH(walk_trig, rnd2) - 0.5) * 0.08;  // small steps
walk_step_capture = (ba.sAndH(walk_trig, rnd3) - 0.5) * 0.08;
walk_repeat = walk_step_repeat : (+ ~ *(0.97)) : max(-0.25) : min(0.25);
walk_capture = walk_step_capture : (+ ~ *(0.97)) : max(-0.25) : min(0.25);

// Band-specific slow walks (reuse same walk period but separate random directions)
walk_step_repeat_low = (ba.sAndH(walk_trig, rnd2L) - 0.5) * 0.08;
walk_step_capture_low = (ba.sAndH(walk_trig, rnd3L) - 0.5) * 0.08;
walk_repeat_low = walk_step_repeat_low : (+ ~ *(0.97)) : max(-0.25) : min(0.25);
walk_capture_low = walk_step_capture_low : (+ ~ *(0.97)) : max(-0.25) : min(0.25);

walk_step_repeat_mid = (ba.sAndH(walk_trig, rnd2M) - 0.5) * 0.08;
walk_step_capture_mid = (ba.sAndH(walk_trig, rnd3M) - 0.5) * 0.08;
walk_repeat_mid = walk_step_repeat_mid : (+ ~ *(0.97)) : max(-0.25) : min(0.25);
walk_capture_mid = walk_step_capture_mid : (+ ~ *(0.97)) : max(-0.25) : min(0.25);

walk_step_repeat_high = (ba.sAndH(walk_trig, rnd2H) - 0.5) * 0.08;
walk_step_capture_high = (ba.sAndH(walk_trig, rnd3H) - 0.5) * 0.08;
walk_repeat_high = walk_step_repeat_high : (+ ~ *(0.97)) : max(-0.25) : min(0.25);
walk_capture_high = walk_step_capture_high : (+ ~ *(0.97)) : max(-0.25) : min(0.25);

// === BUFFER SETUP ===
buf_seconds = 6.5;  // covers longest granular freezes at max chaos
buf_sz = max(256, int(ma.SR * buf_seconds));  // Sample-rate independent buffer

// === REPEAT RATE (stutter frequency - how fast segment repeats) ===
// Variable from ~0.35 Hz (2.8s) to 24 Hz - still wide but avoids ultra-slow looping when calm
// Chaos knob: higher = faster repetition (glitchy), lower = slower but still responsive
// In the bottom 20%, steer toward a simple repeater feel (~0.7 Hz floor)
min_repeat_hz = (0.35 + chaos_sharp * 2.6) * (1.0 - simple_mode) + 0.7 * simple_mode;
max_repeat_hz = 18.0 + 110.0 * chaos_sharp;  // tighter low end, much faster top end

// Host BPM sync grid (quantizes repeat rate to musical divisions when enabled)
beat_hz = bpm_safe / 60.0;
g1 = beat_hz * 0.25;   // 1/4 beat
g2 = beat_hz * 0.333;  // triplet-ish (1/3 beat)
g3 = beat_hz * 0.5;    // 1/2 beat
g4 = beat_hz * 1.0;    // 1 beat
g5 = beat_hz * 2.0;    // 2 beats
g6 = beat_hz * 4.0;    // 4 beats
g7 = beat_hz * 8.0;    // 8 beats (machine-gun)
m12 = (g1 + g2) * 0.5;
m23 = (g2 + g3) * 0.5;
m34 = (g3 + g4) * 0.5;
m45 = (g4 + g5) * 0.5;
m56 = (g5 + g6) * 0.5;
m67 = (g6 + g7) * 0.5;
sync_quantize(freq) = ba.if(freq < m12, g1,
                     ba.if(freq < m23, g2,
                     ba.if(freq < m34, g3,
                     ba.if(freq < m45, g4,
                     ba.if(freq < m56, g5,
                     ba.if(freq < m67, g6, g7))))));

repeat_rate_base = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3;
repeat_rate_nom = repeat_rate_base * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;  // pull toward near-natural rate when calm
repeat_rate_free = repeat_rate_nom * (1.0 + walk_repeat);
repeat_rate = ba.if(sync_on > 0, sync_quantize(repeat_rate_free), repeat_rate_free);

// Band-specific repeat rates with subtle scaling
repeat_rate_base_low = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_low;
repeat_rate_nom_low = repeat_rate_base_low * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_low_free = repeat_rate_nom_low * (1.0 + walk_repeat_low) * 0.82;  // slightly slower in lows
repeat_rate_low = ba.if(sync_on > 0, sync_quantize(repeat_rate_low_free), repeat_rate_low_free);

repeat_rate_base_mid = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_mid;
repeat_rate_nom_mid = repeat_rate_base_mid * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_mid_free = repeat_rate_nom_mid * (1.0 + walk_repeat_mid) * 1.00;
repeat_rate_mid = ba.if(sync_on > 0, sync_quantize(repeat_rate_mid_free), repeat_rate_mid_free);

repeat_rate_base_high = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_high;
repeat_rate_nom_high = repeat_rate_base_high * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_high_free = repeat_rate_nom_high * (1.0 + walk_repeat_high) * 1.22;  // faster in highs
repeat_rate_high = ba.if(sync_on > 0, sync_quantize(repeat_rate_high_free), repeat_rate_high_free);

// === HOLD DURATION (how long current stutter segment is active) ===
// Variable from ~120ms to ~8 seconds - still roomy but refreshes faster when calm
// Chaos knob: higher = shorter holds (glitchier), lower = longer holds (more stutter-y)
min_hold_ms = 240 - chaos_sharp * 140;    // ~240ms (calm) down to ~100ms (wild)
max_hold_ms = 8000 - chaos_sharp * 6000;  // ~8s (calm) to ~2s (wild)
hold_ms_raw = min_hold_ms + (max_hold_ms - min_hold_ms) * pow(r2, 0.6);  // Bias shorter
hold_ms = hold_ms_raw * (0.30 + 0.70 * chaos_sharp) * (1.0 + 0.35 * simple_mode);  // longer chunks when calm-simple
hold_updates = max(1, int(hold_ms / update_time_ms));  // Convert to number of update triggers

// === STUTTER SKIP COUNTER ===
skip_counter = select2(trig, skip_counter - 1, hold_updates) : max(0) ~ _;
should_capture_new = trig & (skip_counter == 0);

// === GAP/DROPOUT LOGIC (buffer underrun simulation) ===
// Low chaos: 1% gap probability (barely any silence)
// High chaos: 50% gap probability (aggressive dropouts)
gap_probability = (chaos_wild * 0.75 * (0.35 + 0.65 * chaos_sharp) * (1.0 + 0.8 * chaos_wild)) * (1.0 - 0.9 * calm_mode) * (1.0 - 0.9 * simple_mode);  // near-zero when calm, boosted when wild
gap_decision = ba.sAndH(trig, rnd5);  // Random value sampled at each trigger
will_gap = ba.if(calm_force > 0.05, 0, gap_decision < gap_probability);
gap_time_ms = 12 + chaos_wild * 250;  // 12ms to ~262ms gaps at extremes
gap_period = max(1, int(gap_time_ms * ma.SR / 1000));
gap_phase = ba.period(gap_period);  // Sawtooth phase for gap
gap_fade_ms = 2.2 - chaos_sharp * 1.6;  // slightly longer fade at calm, tighter at wild
gap_fade_samps = max(1, int(ma.SR * gap_fade_ms / 1000.0));
gap_fade_frac = min(0.49, float(gap_fade_samps) / float(gap_period));
gap_env_open = ba.if(gap_phase < gap_fade_frac, 1.0 - gap_phase / gap_fade_frac,
               ba.if(gap_phase < 0.5 - gap_fade_frac, 0.0,
               ba.if(gap_phase < 0.5, (gap_phase - (0.5 - gap_fade_frac)) / gap_fade_frac, 1.0)));
gap_env = ba.if(will_gap, gap_env_open, 1.0);
gap_noise = (no.noise * 0.003 * chaos * chaos) * (0.25 + 0.75 * chaos_sharp);  // quieter when calm
is_in_gap = will_gap & (gap_env <= 0.001);

// === WRITE POINTER ===
write_ptr = ba.period(buf_sz);

// === STUTTER PROCESSOR (core stutter_simple.dsp logic) ===
stutter_processor(x, r1p, r2p, r3p, r4p, r5p, walk_repeat_p, walk_capture_p, repeat_rate_p, capture_scale_p) = out
with {
    // Cached SR-scaled constants (fixed) to reduce per-sample math
    sr_ms_5 = 5.0 * ma.SR / 1000.0;
    sr_ms_20 = 20.0 * ma.SR / 1000.0;
    fade_base_const = int(ma.SR * 0.0015);

    // === TRANSIENT DETECTION (gentle bias toward capturing transients) ===
    // Simple envelope follower to detect transients on input signal
    transient_strength = ba.peakhold(sr_ms_5, abs(x));  // 5ms attack peakhold
    transient_norm = min(1.0, transient_strength / 0.3);  // Normalized 0-1
    
    // === CAPTURE SIZE (segment to repeat) - with gentle transient biasing ===
    // At low chaos: large segments for smooth stutter (187.5-375ms) - 25% longer for meditative feel
    // At high chaos: smaller segments for glitchier effect (50-100ms)
    // Transient biasing: gently pushes toward capturing transients (15% max influence)
    min_capture_ms = (187.5 - chaos * 137.5) * (1.0 + 0.25 * fidelity);   // longer windows when calm for fidelity
    max_capture_ms = (375 - chaos * 275) * (1.0 + 0.25 * fidelity);  // bigger slices preserve phrase feel when calm
    
    // Gentle transient influence: blend r1 with transient signal
    // transient_norm = 0 (quiet) -> favor smaller segments (r1 pulled up slightly)
    // transient_norm = 1 (loud transient) -> favor larger segments (r1 pulled down slightly)
    transient_bias = 0.15 * (1.0 - transient_norm);  // 0-15% influence, inverted (0 quiet, 1 loud)
    r1_biased = r1p * (1.0 - transient_bias) + transient_bias * 0.3;  // Blend with slight bias toward small
    
    capture_ms_nom = min_capture_ms + (max_capture_ms - min_capture_ms) * r1_biased * r1_biased;  // Use biased r1, squared bias toward small
    capture_ms = capture_ms_nom * (1.0 + walk_capture_p) * (1.0 + 0.4 * simple_mode) * capture_scale_p;
    capture_sz = int(capture_ms * ma.SR / 1000.0);
    safe_capture = max(1, min(capture_sz, buf_sz));
    
    // Main stutter: fast repeating phase through captured segment with slight positional jitter
    capture_sec = max(1.0 / ma.SR, float(safe_capture) / ma.SR);
    max_repeat_cycles = 500.0;  // avoid runaway playback rates
    repeat_cycles = min(max_repeat_cycles, repeat_rate_p / capture_sec);
    stutter_phase = os.phasor(1, repeat_cycles);
    stutter_pos = int(stutter_phase * safe_capture) % max(1, safe_capture);
    jitter_base = (1 + chaos_sharp * 3) * (0.35 + 0.65 * chaos_sharp);   // extremely steady when calm
    jitter_extra = (chaos_sharp * 22) * (0.35 + 0.65 * chaos_sharp);  // even more jitter headroom when wild
    // When simple_mode is active, heavily tame extra jitter and overall span to reduce flutter on low chaos
    jitter_extra_tamed = jitter_extra * (1.0 - 0.85 * simple_mode);  // kill most of the extra jitter when simple
    jitter_span = (jitter_base + jitter_extra_tamed) * (1.0 - 0.35 * simple_mode);  // overall span also shrinks when simple
    jitter_walk = walk_repeat_p * 0.5;  // slow drift component
    jitter_offset = int((r5p - 0.5 + jitter_walk) * jitter_span);
    stutter_pos_j_base = (stutter_pos + jitter_offset + safe_capture) % max(1, safe_capture);
    recency = min(0.9, 0.65 * fidelity);  // pull read position toward freshest audio when calm
    stutter_pos_j = int(stutter_pos_j_base * (1.0 - recency) + (safe_capture - 1) * recency) % max(1, safe_capture);
    
    // Read pointer: always reading from circular buffer at stutter position
    read_idx = (write_ptr - safe_capture + stutter_pos_j + buf_sz) % buf_sz;
    
    // Delay line (continuously reads/writes input)
    delayed_raw = de.delay(buf_sz, read_idx, x);
    
    // Crossfade envelope (smooth out repeats, eliminate clicks) with stochastic windowing
    fade_base = max(32, fade_base_const);  // ~1.5ms base
    fade_rand = int(fade_base * (0.6 + 0.8 * r2p));  // 0.6x to 1.4x base
    fade_samples = max(16, fade_rand);
    fade_phase = (stutter_pos % fade_samples) / float(fade_samples);
    fade_curve_sel = r4p;
    fade_env_hann = sin(fade_phase * ma.PI);
    fade_env_tri = 1.0 - abs(2.0 * fade_phase - 1.0);
    fade_env_exp = pow(fade_phase, 0.35) * (1.0 - pow(fade_phase, 2.0));
    fade_env_raw = ba.if(fade_curve_sel < 0.33, fade_env_hann,
                   ba.if(fade_curve_sel < 0.66, fade_env_tri, fade_env_exp));
    fade_env = max(0.05, fade_env_raw);
    
    delayed = delayed_raw * fade_env;
    
    // === SPECIAL EFFECTS (controlled by chaos knob) ===
    // Smoothly rebalance probabilities so chaos moves feel even and predictable
    smoothstep01(u) = u * u * (3.0 - 2.0 * u);  // eased 0-1 curve for gentler transitions
    prob_slew(u) = u : si.smooth(0.9);  // capped smoothing so rapid chaos moves do not over-lag

    chaos_effect = smoothstep01(chaos);  // eased chaos for probability mapping
    effect_selector_raw = r4p;
    energy_env = ba.peakhold(sr_ms_20, abs(x));
    energy_norm = min(1.0, energy_env / 0.25);
    effect_selector = min(1.0, max(0.0, effect_selector_raw + (energy_norm - 0.5) * 0.2));  // push energetic input toward stronger effects

    // Base shares (normal → reverse → slow-mo → granular) smoothly morph with chaos
    normal_base = 0.90;   // ~90% normal when calm
    normal_wild = 0.45;   // ~45% normal when fully chaotic
    normal_prob_raw = normal_base + (normal_wild - normal_base) * chaos_effect;
    normal_prob = prob_slew(max(0.05, normal_prob_raw + 0.04 * fidelity));  // lean slightly more normal when calm

    reverse_raw = 0.05 + 0.25 * chaos_effect;   // 5→30%
    slow_raw    = 0.03 + 0.22 * chaos_effect;   // 3→25%
    granular_raw= 0.02 + 0.18 * chaos_effect;   // 2→20%
    raw_sum = reverse_raw + slow_raw + granular_raw;

    remaining_prob = max(0.02, 1.0 - normal_prob);  // always leave room for other modes
    raw_scale = remaining_prob / max(1e-3, raw_sum);
    reverse_prob = prob_slew(reverse_raw * raw_scale);
    slow_prob = prob_slew(slow_raw * raw_scale);
    granular_prob = max(0.0, 1.0 - (normal_prob + reverse_prob + slow_prob));

    reverse_threshold = normal_prob + reverse_prob;
    slowmo_threshold = reverse_threshold + slow_prob;
    granular_threshold = min(1.0, slowmo_threshold + granular_prob);

    effect_mode_raw = ba.if(effect_selector < normal_prob, 0,
                     ba.if(effect_selector < reverse_threshold, 1,
                     ba.if(effect_selector < slowmo_threshold, 2, 3)));

    // Tiny hysteresis margins to prevent chatter near thresholds
    eps = 0.01;
    norm_bound = normal_prob - eps;
    rev_bound = reverse_threshold - eps;
    slow_bound = slowmo_threshold - eps;
    effect_mode = ba.if(calm_force > 0.05, 0,
                 ba.if(effect_selector < norm_bound, 0,
                 ba.if(effect_selector < rev_bound, 1,
                 ba.if(effect_selector < slow_bound, 2, 3))));

    // Safety gain riding for non-normal modes (reverse/slowmo/granular)
    burst_trim_db = -0.5 - chaos_sharp * 2.5;  // keep more level at high chaos
    burst_trim = ba.db2linear(burst_trim_db);
    burst_gain = ba.if(effect_mode == 0, 1.0, burst_trim);
    
    // Reverse: invert phase
    stutter_phase_reverse = os.phasor(1, repeat_cycles);
    stutter_pos_reverse = int((1.0 - stutter_phase_reverse) * safe_capture) % max(1, safe_capture);
    stutter_pos_reverse_j = (stutter_pos_reverse + jitter_offset + safe_capture) % max(1, safe_capture);
    read_idx_reverse = (write_ptr - safe_capture + stutter_pos_reverse_j + buf_sz) % buf_sz;
    delayed_reverse = de.delay(buf_sz, read_idx_reverse, x) * fade_env;
    
    // Slow-mo: MASSIVELY slowed down repetition (1000x slower!)
    slow_repeat = repeat_rate_p / 1000.0;  // 1000x slower
    slow_cycles = min(max_repeat_cycles, slow_repeat / capture_sec);
    stutter_phase_slow = os.phasor(1, slow_cycles);
    stutter_pos_slow = int(stutter_phase_slow * safe_capture) % max(1, safe_capture);
    stutter_pos_slow_j = (stutter_pos_slow + jitter_offset + safe_capture) % max(1, safe_capture);
    read_idx_slow = (write_ptr - safe_capture + stutter_pos_slow_j + buf_sz) % buf_sz;
    delayed_slow = de.delay(buf_sz, read_idx_slow, x) * fade_env;
    
    // Granular Freeze: Time-stretched granular glitch (rare, crystalline artifacts)
    // Single-voice granular with sane, sample-rate-aware phasing
    granular_grain_ms = 26 + chaos_sharp * 90;  // 26-116ms grains, more stretch when wild
    granular_grain_samps = max(32, int(ma.SR * granular_grain_ms / 1000.0));
    granular_rate_hz = ma.SR / float(granular_grain_samps);
    
    // === GRANULAR (lite, single voice, no spray) ===
    granular_phase_1 = os.phasor(1, granular_rate_hz);
    grain_pos_1 = int(granular_phase_1 * safe_capture) % max(1, safe_capture);
    read_idx_granular_1 = (write_ptr - safe_capture + grain_pos_1 + buf_sz) % buf_sz;
    grain_sample_1 = de.delay(buf_sz, read_idx_granular_1, x);

    // Granular envelope: smooth the grain edges with Hann window
    grain_env_phase = granular_phase_1;
    grain_window = sin(grain_env_phase * ma.PI);  // Hann-like window

    // Single-voice granular (lighter for IDE/stack)
    delayed_granular = grain_sample_1 * grain_window * 0.8;
    
    // Select effect output
    effect_out_raw = ba.if(effect_mode == 0, delayed,
                    ba.if(effect_mode == 1, delayed_reverse,
                    ba.if(effect_mode == 2, delayed_slow, delayed_granular)));
    chaos_makeup = 1.0 + 0.18 * chaos_wild;  // stronger level lift when fully chaotic
    effect_out = effect_out_raw * burst_gain * chaos_makeup;
    // Extra guard: if chaos is very low, force normal regardless of any lingering selector state
    effect_out_guard = ba.if((calm_force > 0.02) | (chaos_raw < 0.12), delayed, effect_out);

    // Transient-respecting dry/ghost injection to preserve sharp hits
    transient_thresh = 0.65;
    transient_hit = transient_norm > transient_thresh;
    transient_edge = max(0.0, transient_hit - transient_hit');  // rising edge only
    inj_rel_ms = 140;
    inj_rel_coeff = exp(-1.0 / max(1.0, (inj_rel_ms * ma.SR / 1000.0)));
    inj_env = transient_edge : (+ ~ *(inj_rel_coeff));  // decaying burst envelope
    inj_mix_raw = min(0.22, inj_env * (0.28 + 0.4 * transient_norm) * (0.6 + 0.4 * fidelity));
    inj_mix = inj_mix_raw * bypass_env;  // respect bypass smoothing
    ghost = x * 0.65 + delayed * 0.35;  // mostly dry, slight stutter color to avoid jump
    effect_pre_gap = effect_out_guard * (1.0 - inj_mix) + ghost * inj_mix;
    
    // Apply gap/dropout gate
    // Duck gap noise when input energy is high to keep dropouts intentional
    gap_noise_duck = 1.0 - min(0.8, energy_norm * 0.8);
    out = effect_pre_gap * gap_env + gap_noise * (1.0 - gap_env) * gap_noise_duck;
};

// === COMPACTION HELPERS (generic curve) ===
comp_drive_amt(lofi) = 1.5 + 3.5 * lofi; // stronger at higher lo-fi
comp_curve(u, drive) = u / (1.0 + drive * abs(u));
comp_expand(v, drive) = v / max(0.05, (1.0 - drive * min(0.999, abs(v))));

// === LO-FI DEGRADATION (gentle bitcrushing) ===
lofi(x, amt) = result
with {
    x0 = max(-1.0, min(1.0, x));

    // Cheaper nonlinear companding always on (keeps low-level detail without exp/log/pow)
    drive = comp_drive_amt(amt);
    comp_in = comp_curve(x0, drive);

    // Bit depth: 16 (clean) to 8 (max lo-fi)
    bits = 16 - amt * 8;  // 16-bit to 8-bit
    step = 1.0 / pow(2, bits);
    dither_amp = step * 0.5 * amt;  // scale with amount
    tpdf_lin = (no.noise - no.noise) * dither_amp;
    tpdf_cmp = (no.noise - no.noise) * dither_amp;
    q_lin = floor((x0 + tpdf_lin) / step + 0.5) * step : max(-1.0) : min(1.0);
    q_cmp = floor((comp_in + tpdf_cmp) / step + 0.5) * step : max(-1.0) : min(1.0);
    decomp = comp_expand(q_cmp, drive);
    comp_strength = amt;
    crush_raw = q_lin * (1.0 - comp_strength) + decomp * comp_strength;
    // Makeup to counter level loss at high lo-fi
    makeup = 1.0 + 0.6 * amt;
    crush = crush_raw * makeup;
    // Lowpass: 16kHz (clean) to 2kHz (max lo-fi)
    lp_freq = 16000 - amt * 14000; // 16kHz to 2kHz
    bit_out = fi.lowpass(1, lp_freq, crush);

    // Post-EQ tilt: restore presence after heavy lowpass (up to ~+6 dB @ highs)
    tilt_freq = 4000 + amt * 2000;  // 4-6 kHz pivot
    tilt_boost_db = amt * 6.0;      // ~1 dB/oct over ~6 octaves
    bright_boost_db = (1.0 - min(1.0, amt * 3.0)) * 1.5;  // small lift when lofi is near clean
    tilt_boost = ba.db2linear(tilt_boost_db + bright_boost_db) - 1.0;
    tilt_high = fi.highpass(1, tilt_freq, bit_out);
    bit_tilt = bit_out + tilt_high * tilt_boost;

    // Bit blend
    bit_mix = amt;
    bit_blend = x * (1.0 - bit_mix) + bit_tilt * bit_mix;

    // Sample-rate reduction (downsample/hold) 1x -> 1/8x
    rate_div = max(1, int(1 + amt * 7));
    rate_counter = (+(1) : %(rate_div)) ~ _;
    rate_trig = (rate_counter == 0);
    rate_hold = ba.sAndH(rate_trig, bit_tilt);
    rate_lp_freq = max(200, lp_freq / rate_div);  // tame images
    rate_lp = fi.lowpass(1, rate_lp_freq, rate_hold);

    // Rate blend
    rate_mix = amt;
    result = bit_blend * (1.0 - rate_mix) + rate_lp * rate_mix;
};

// Gentle soft saturation to prevent unexpected overs while keeping transients intact
soft_sat(x) = x * (27 + x * x) / (27 + 9 * x * x);

// Fast, cheap lookahead limiter (1-2ms) to avoid flat-tops on chaos spikes
limiter(x) = limited
with {
    ceiling = 0.98;
    knee_width = 0.08 * fidelity;  // softer knee when calm to reduce pumping
    knee_start = ceiling - knee_width;
    lookahead_ms = 1.5;
    look_samps = max(1, int(ma.SR * lookahead_ms / 1000.0));
    rel_ms_base = 8.0;
    rel_ms = rel_ms_base + 12.0 * fidelity;  // longer release when calm
    rel_coeff = exp(-1.0 / max(1.0, (rel_ms * ma.SR / 1000.0)));
    x_look = de.delay(4096, look_samps, x);
    env = abs(x_look) : (max ~ *(rel_coeff));
    over = max(0.0, env - knee_start);
    blend = min(1.0, over / max(1e-6, knee_width));
    hard_gain = ceiling / max(ceiling, env);
    gain = 1.0 * (1.0 - blend) + hard_gain * blend;
    limited = x_look * gain;
};

// === STEREO CROSS-BLEND LOGIC ===
// Very subtle: 5% chance of cross-blending, 2-8% blend amount
stereo_cross_probability = 0.05;  // 5% chance
stereo_decision = ba.sAndH(trig, rnd1);  // Use rnd1 for cross-blend decision
will_cross_blend = stereo_decision < stereo_cross_probability;
stereo_blend_amt = 0.02 + chaos * 0.06;  // 2% to 8% blend (very subtle)

// === MAIN STEREO PROCESSING ===
process(left_in, right_in) = (out_l, out_r)
with {
    // Band crossover points and scalars
    xover_lo = 220;
    xover_hi = 2200;
    capture_scale_low = 1.12;
    capture_scale_mid = 1.00;
    capture_scale_high = 0.86;
    band_trim_low = 0.98;
    band_trim_mid = 1.00;
    band_trim_high = 0.95;

    // Input trims
    in_l = left_in * input_gain;
    in_r = right_in * input_gain;

    // 3-way soft crossover per channel (lighter order to reduce graph size)
    low_l   = in_l : fi.lowpass(2, xover_lo);
    high1_l = in_l : fi.highpass(2, xover_lo);
    mid_l   = high1_l : fi.lowpass(2, xover_hi);
    high_l  = high1_l : fi.highpass(2, xover_hi);

    low_r   = in_r : fi.lowpass(2, xover_lo);
    high1_r = in_r : fi.highpass(2, xover_lo);
    mid_r   = high1_r : fi.lowpass(2, xover_hi);
    high_r  = high1_r : fi.highpass(2, xover_hi);

    // Banded stutter per channel with independent seeds and scalars
    low_stutter_l = stutter_processor(low_l, r1_low, r2_low, r3_low, r4_low, r5_low, walk_repeat_low, walk_capture_low, repeat_rate_low, capture_scale_low) * band_trim_low;
    mid_stutter_l = stutter_processor(mid_l, r1_mid, r2_mid, r3_mid, r4_mid, r5_mid, walk_repeat_mid, walk_capture_mid, repeat_rate_mid, capture_scale_mid) * band_trim_mid;
    high_stutter_l= stutter_processor(high_l, r1_high, r2_high, r3_high, r4_high, r5_high, walk_repeat_high, walk_capture_high, repeat_rate_high, capture_scale_high) * band_trim_high;

    low_stutter_r = stutter_processor(low_r, r1_low, r2_low, r3_low, r4_low, r5_low, walk_repeat_low, walk_capture_low, repeat_rate_low, capture_scale_low) * band_trim_low;
    mid_stutter_r = stutter_processor(mid_r, r1_mid, r2_mid, r3_mid, r4_mid, r5_mid, walk_repeat_mid, walk_capture_mid, repeat_rate_mid, capture_scale_mid) * band_trim_mid;
    high_stutter_r= stutter_processor(high_r, r1_high, r2_high, r3_high, r4_high, r5_high, walk_repeat_high, walk_capture_high, repeat_rate_high, capture_scale_high) * band_trim_high;

    // Band-aware lo-fi: keep lows cleaner, lean harder on highs
    lofi_low_amt = min(1.0, lofi_amt * 0.55 + 0.10 * chaos_sharp);
    lofi_mid_amt = lofi_amt;
    lofi_high_amt = min(1.0, lofi_amt * 1.25);

    low_proc_l = low_stutter_l;  // preserve weight and punch in lows
    mid_proc_l = lofi(mid_stutter_l, lofi_mid_amt);
    high_proc_l = lofi(high_stutter_l, lofi_high_amt);

    low_proc_r = low_stutter_r;
    mid_proc_r = lofi(mid_stutter_r, lofi_mid_amt);
    high_proc_r = lofi(high_stutter_r, lofi_high_amt);

    left_stutter = low_proc_l + mid_proc_l + high_proc_l;
    right_stutter = low_proc_r + mid_proc_r + high_proc_r;

    // Stereo decorrelation: tiny per-channel random offset, scaled by mix/bypass
    decor_scale = bypass_env * (1.0 - 0.7 * fidelity);  // reduce smear when calm
    decor_max_samps = max(0, int(ma.SR * 0.0025 * decor_scale));  // up to ~2.5ms at full mix
    decor_l_samps = int(decor_max_samps * r2);
    decor_r_samps = int(decor_max_samps * r4);
    left_stutter_d = de.delay(256, decor_l_samps, left_stutter);
    right_stutter_d = de.delay(256, decor_r_samps, right_stutter);
    decor_ok = (decor_l_samps != decor_r_samps) | (decor_l_samps > 0) | (decor_r_samps > 0);
    cross_gate = will_cross_blend & decor_ok;
    
    // Very slight cross-blend when triggered
    blend_l = left_stutter_d * (1.0 - cross_gate * stereo_blend_amt) + right_stutter_d * cross_gate * stereo_blend_amt;
    blend_r = right_stutter_d * (1.0 - cross_gate * stereo_blend_amt) + left_stutter_d * cross_gate * stereo_blend_amt;

    // Slow random pan / lateral delay (lightweight gate based on chaos only)
    pan_chaos_gate = max(0.0, chaos - 0.32) / 0.68;  // 0 at calm, 1 near max chaos
    pan_gate = pan_chaos_gate * bypass_env;
    pan_target = (ba.sAndH(walk_trig, rnd4) - 0.5) * 0.9;  // wide but slow target
    pan_state = pan_target : smooth_ms(600);  // slow drift between targets
    pan_depth = pan_gate * (0.06 + 0.16 * chaos_sharp);  // amount of mid->side injection
    pan_delay_ms = pan_gate * (0.4 + 5.0 * chaos_sharp) * abs(pan_state);  // lateral delay grows with chaos
    pan_delay_samps = max(0, int(ma.SR * pan_delay_ms / 1000.0));
    delay_l_samps = ba.if(pan_state >= 0, pan_delay_samps, 0);
    delay_r_samps = ba.if(pan_state < 0, pan_delay_samps, 0);
    blend_l_dpan = de.delay(2048, delay_l_samps, blend_l);
    blend_r_dpan = de.delay(2048, delay_r_samps, blend_r);
    mid_pan = 0.5 * (blend_l_dpan + blend_r_dpan);
    side_pan = 0.5 * (blend_l_dpan - blend_r_dpan) + mid_pan * pan_state * pan_depth;

    // Mono-compat guard: gently rein in side when cross-blend fires to keep phantom center stable
    side_shaper = 1.0 - cross_gate * stereo_blend_amt * 1.2;  // reduce side a bit when cross-blending
    guard_l = mid_pan + side_pan * side_shaper;
    guard_r = mid_pan - side_pan * side_shaper;
    
    // Smoothed on/off (no clicks)
    enabled_l = in_l + bypass_env * (guard_l - in_l);
    enabled_r = in_r + bypass_env * (guard_r - in_r);
    
    // Output trim, soft saturation, and DC blocking
    out_l = enabled_l * output_gain : limiter : soft_sat : fi.dcblocker;
    out_r = enabled_r * output_gain : limiter : soft_sat : fi.dcblocker;
};

