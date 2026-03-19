import("stdfaust.lib");

// Freeze95 - full-bandwidth Quantum Stutter core with the newer lo-fi behavior.
declare name "Freeze95";
declare version "1.1";
declare description "Band-split quantum stutter/freeze with host sync, layered special modes, and the newer lo-fi control behavior.";

fast_glide_ms = 8.0;
smooth_ms(ms) = si.smooth(exp(-1.0 / max(1.0, ma.SR * ms / 1000.0)));
clamp01(x) = min(1.0, max(0.0, x));

bypass_raw = checkbox("Bypass[tooltip: Power on/off]");
chaos_raw = hslider("Chaos[style:knob]", 0.55, 0.0, 1.0, 0.01);
lofi_raw = hslider("Lo-Fi[style:knob]", 0.5, 0.0, 1.0, 0.01);
bpm_host = hslider("transport/bpm", 120, 20, 300, 0.1);
sync_on = checkbox("Sync BPM[tooltip: Quantize stutter to host BPM]");

stutter_on = 1.0 - bypass_raw;
bypass_env = stutter_on : smooth_ms(fast_glide_ms);
chaos = chaos_raw : smooth_ms(fast_glide_ms);
lofi_knob = lofi_raw : smooth_ms(fast_glide_ms);

chaos_focus = pow(chaos, 1.1);
chaos_texture = pow(clamp01((chaos - 0.18) / 0.62), 1.35);
chaos_damage = pow(clamp01((chaos - 0.58) / 0.42), 1.8);
chaos_sharp = min(1.0, 0.64 * chaos_focus + 0.26 * chaos_texture + 0.10 * chaos_damage);
chaos_wild = min(1.0, 0.58 * chaos_texture + 0.42 * chaos_damage);
fidelity = 1.0 - chaos_sharp;
calm_mode = clamp01(1.0 - chaos_raw / 0.12);
simple_mode = clamp01(1.0 - chaos_raw / 0.2);
calm_force = max(calm_mode, simple_mode);
lofi_amt = lofi_knob;
bpm_safe = max(20.0, bpm_host);
output_gain = 0.94 + 0.04 * fidelity;

// Base set
rnd1 = (+(111) ~ *(1103515245)) : +(12345) : %(2147483647) : /(2147483647.0);
rnd2 = (+(222) ~ *(1103515245)) : +(23456) : %(2147483647) : /(2147483647.0);
rnd3 = (+(333) ~ *(1103515245)) : +(34567) : %(2147483647) : /(2147483647.0);
rnd4 = (+(444) ~ *(1103515245)) : +(45678) : %(2147483647) : /(2147483647.0);
rnd5 = (+(555) ~ *(1103515245)) : +(56789) : %(2147483647) : /(2147483647.0);

// Low band seeds
rnd1L = (+(1011) ~ *(1103515245)) : +(13245) : %(2147483647) : /(2147483647.0);
rnd2L = (+(1022) ~ *(1103515245)) : +(14356) : %(2147483647) : /(2147483647.0);
rnd3L = (+(1033) ~ *(1103515245)) : +(15467) : %(2147483647) : /(2147483647.0);
rnd4L = (+(1044) ~ *(1103515245)) : +(16578) : %(2147483647) : /(2147483647.0);
rnd5L = (+(1055) ~ *(1103515245)) : +(17689) : %(2147483647) : /(2147483647.0);

// Mid band seeds
rnd1M = (+(2011) ~ *(1103515245)) : +(21245) : %(2147483647) : /(2147483647.0);
rnd2M = (+(2022) ~ *(1103515245)) : +(22356) : %(2147483647) : /(2147483647.0);
rnd3M = (+(2033) ~ *(1103515245)) : +(23467) : %(2147483647) : /(2147483647.0);
rnd4M = (+(2044) ~ *(1103515245)) : +(24578) : %(2147483647) : /(2147483647.0);
rnd5M = (+(2055) ~ *(1103515245)) : +(25689) : %(2147483647) : /(2147483647.0);

// High band seeds
rnd1H = (+(3011) ~ *(1103515245)) : +(31245) : %(2147483647) : /(2147483647.0);
rnd2H = (+(3022) ~ *(1103515245)) : +(32356) : %(2147483647) : /(2147483647.0);
rnd3H = (+(3033) ~ *(1103515245)) : +(33467) : %(2147483647) : /(2147483647.0);
rnd4H = (+(3044) ~ *(1103515245)) : +(34578) : %(2147483647) : /(2147483647.0);
rnd5H = (+(3055) ~ *(1103515245)) : +(35689) : %(2147483647) : /(2147483647.0);

update_time_ms = max(60.0, 12000.0 - chaos_sharp * 11980.0);
update_rate = max(1, int(update_time_ms * ma.SR / 1000.0));
counter = (+(1) : %(update_rate)) ~ _;

chaos_prev = chaos_raw';
chaos_delta = abs(chaos_raw - chaos_prev);
chaos_moved = chaos_delta > 0.004;
chaos_edge = max(0.0, chaos_moved - chaos_moved');

lofi_prev = lofi_raw';
lofi_delta = abs(lofi_raw - lofi_prev);
lofi_moved = lofi_delta > 0.004;
lofi_edge = max(0.0, lofi_moved - lofi_moved');

trig = max(counter == 0, max(chaos_edge, lofi_edge));

r1 = ba.sAndH(trig, rnd1);
r2 = ba.sAndH(trig, rnd2);
r3 = ba.sAndH(trig, rnd3);
r4 = ba.sAndH(trig, rnd4);
r5 = ba.sAndH(trig, rnd5);

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

walk_time_ms = 5000.0 + (1.0 - chaos_focus) * 15000.0;
walk_rate = max(1, int(walk_time_ms * ma.SR / 1000.0));
walk_counter = (+(1) : %(walk_rate)) ~ _;
walk_trig = (walk_counter == 0);

walk_step_repeat = (ba.sAndH(walk_trig, rnd2) - 0.5) * 0.08;
walk_step_capture = (ba.sAndH(walk_trig, rnd3) - 0.5) * 0.08;
walk_repeat = walk_step_repeat : (+ ~ *(0.97)) : max(-0.25) : min(0.25);
walk_capture = walk_step_capture : (+ ~ *(0.97)) : max(-0.25) : min(0.25);

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

buf_seconds = 6.5;
buf_sz = max(256, int(ma.SR * buf_seconds));
write_ptr = ba.period(buf_sz);

min_repeat_hz = (0.35 + chaos_sharp * 2.6) * (1.0 - simple_mode) + 0.7 * simple_mode;
max_repeat_hz = 18.0 + 110.0 * chaos_sharp;

beat_hz = bpm_safe / 60.0;
g1 = beat_hz * 0.25;
g2 = beat_hz * 0.333;
g3 = beat_hz * 0.5;
g4 = beat_hz * 1.0;
g5 = beat_hz * 2.0;
g6 = beat_hz * 4.0;
g7 = beat_hz * 8.0;
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
repeat_rate_nom = repeat_rate_base * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_free = repeat_rate_nom * (1.0 + walk_repeat);
repeat_rate = ba.if(sync_on > 0.0, sync_quantize(repeat_rate_free), repeat_rate_free);

repeat_rate_base_low = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_low;
repeat_rate_nom_low = repeat_rate_base_low * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_low_free = repeat_rate_nom_low * (1.0 + walk_repeat_low) * 0.82;
repeat_rate_low = ba.if(sync_on > 0.0, sync_quantize(repeat_rate_low_free), repeat_rate_low_free);

repeat_rate_base_mid = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_mid;
repeat_rate_nom_mid = repeat_rate_base_mid * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_mid_free = repeat_rate_nom_mid * (1.0 + walk_repeat_mid);
repeat_rate_mid = ba.if(sync_on > 0.0, sync_quantize(repeat_rate_mid_free), repeat_rate_mid_free);

repeat_rate_base_high = min_repeat_hz + (max_repeat_hz - min_repeat_hz) * r3_high;
repeat_rate_nom_high = repeat_rate_base_high * (1.0 - 0.65 * fidelity) + 0.9 * 0.65 * fidelity;
repeat_rate_high_free = repeat_rate_nom_high * (1.0 + walk_repeat_high) * 1.22;
repeat_rate_high = ba.if(sync_on > 0.0, sync_quantize(repeat_rate_high_free), repeat_rate_high_free);

min_hold_ms = 240.0 - chaos_sharp * 140.0;
max_hold_ms = 8000.0 - chaos_sharp * 6000.0;
hold_ms_raw = min_hold_ms + (max_hold_ms - min_hold_ms) * pow(r2, 0.6);
hold_ms = hold_ms_raw * (0.30 + 0.70 * chaos_sharp) * (1.0 + 0.35 * simple_mode);
hold_updates = max(1, int(hold_ms / update_time_ms));

skip_counter = select2(trig, skip_counter - 1, hold_updates) : max(0) ~ _;

gap_probability = (chaos_wild * 0.75 * (0.35 + 0.65 * chaos_sharp) * (1.0 + 0.8 * chaos_wild)) *
									(1.0 - 0.9 * calm_mode) * (1.0 - 0.9 * simple_mode);
gap_decision = ba.sAndH(trig, rnd5);
will_gap = ba.if(calm_force > 0.05, 0.0, gap_decision < gap_probability);
gap_time_ms = 12.0 + chaos_wild * 250.0;
gap_period = max(1, int(gap_time_ms * ma.SR / 1000.0));
gap_phase = ba.period(gap_period);
gap_fade_ms = 2.2 - chaos_sharp * 1.6;
gap_fade_samps = max(1, int(ma.SR * gap_fade_ms / 1000.0));
gap_fade_frac = min(0.49, float(gap_fade_samps) / float(gap_period));
gap_env_open = ba.if(gap_phase < gap_fade_frac, 1.0 - gap_phase / gap_fade_frac,
							 ba.if(gap_phase < 0.5 - gap_fade_frac, 0.0,
							 ba.if(gap_phase < 0.5, (gap_phase - (0.5 - gap_fade_frac)) / gap_fade_frac, 1.0)));
gap_env = ba.if(will_gap, gap_env_open, 1.0);
gap_noise = (no.noise * 0.003 * chaos * chaos) * (0.25 + 0.75 * chaos_sharp);

stutter_processor(x, r1p, r2p, r3p, r4p, r5p, walk_repeat_p, walk_capture_p, repeat_rate_p, capture_scale_p) = out
with {
	sr_ms_5 = 5.0 * ma.SR / 1000.0;
	sr_ms_20 = 20.0 * ma.SR / 1000.0;
	fade_base_const = int(ma.SR * 0.0015);

	transient_strength = ba.peakhold(sr_ms_5, abs(x));
	transient_norm = min(1.0, transient_strength / 0.3);

	min_capture_ms = (187.5 - chaos * 137.5) * (1.0 + 0.25 * fidelity);
	max_capture_ms = (375.0 - chaos * 275.0) * (1.0 + 0.25 * fidelity);
	transient_bias = 0.15 * (1.0 - transient_norm);
	r1_biased = r1p * (1.0 - transient_bias) + transient_bias * 0.3;
	capture_ms_nom = min_capture_ms + (max_capture_ms - min_capture_ms) * r1_biased * r1_biased;
	capture_ms = capture_ms_nom * (1.0 + walk_capture_p) * (1.0 + 0.4 * simple_mode) * capture_scale_p;
	capture_sz = int(capture_ms * ma.SR / 1000.0);
	safe_capture = max(1, min(capture_sz, buf_sz));

	capture_sec = max(1.0 / ma.SR, float(safe_capture) / ma.SR);
	max_repeat_cycles = 500.0;
	repeat_cycles = min(max_repeat_cycles, repeat_rate_p / capture_sec);
	stutter_phase = os.phasor(1, repeat_cycles);
	stutter_pos = int(stutter_phase * safe_capture) % max(1, safe_capture);

	jitter_base = (1.0 + chaos_sharp * 3.0) * (0.35 + 0.65 * chaos_sharp);
	jitter_extra = (chaos_sharp * 22.0) * (0.35 + 0.65 * chaos_sharp);
	jitter_extra_tamed = jitter_extra * (1.0 - 0.85 * simple_mode);
	jitter_span = (jitter_base + jitter_extra_tamed) * (1.0 - 0.35 * simple_mode);
	jitter_walk = walk_repeat_p * 0.5;
	jitter_offset = int((r5p - 0.5 + jitter_walk) * jitter_span);
	stutter_pos_j_base = (stutter_pos + jitter_offset + safe_capture) % max(1, safe_capture);
	recency = min(0.9, 0.65 * fidelity);
	stutter_pos_j = int(stutter_pos_j_base * (1.0 - recency) + (safe_capture - 1) * recency) % max(1, safe_capture);
	read_idx = (write_ptr - safe_capture + stutter_pos_j + buf_sz) % buf_sz;
	delayed_raw = de.delay(buf_sz, read_idx, x);

	fade_base = max(32, fade_base_const);
	fade_rand = int(fade_base * (0.6 + 0.8 * r2p));
	fade_samples = max(16, fade_rand);
	fade_phase = (stutter_pos % fade_samples) / float(fade_samples);
	fade_env_hann = sin(fade_phase * ma.PI);
	fade_env_tri = 1.0 - abs(2.0 * fade_phase - 1.0);
	fade_env_exp = pow(fade_phase, 0.35) * (1.0 - pow(fade_phase, 2.0));
	fade_env_raw = ba.if(r4p < 0.33, fade_env_hann,
								 ba.if(r4p < 0.66, fade_env_tri, fade_env_exp));
	fade_env = max(0.05, fade_env_raw);
	delayed = delayed_raw * fade_env;

	smoothstep01(u) = u * u * (3.0 - 2.0 * u);
	prob_slew(u) = u : si.smooth(0.9);

	chaos_effect = smoothstep01(chaos_sharp);
	effect_selector_raw = r4p;
	energy_env = ba.peakhold(sr_ms_20, abs(x));
	energy_norm = min(1.0, energy_env / 0.25);
	effect_selector = min(1.0, max(0.0, effect_selector_raw + (energy_norm - 0.5) * 0.2));

	normal_prob_raw = 0.90 + (0.45 - 0.90) * chaos_effect;
	normal_prob = prob_slew(max(0.05, normal_prob_raw + 0.04 * fidelity));
	reverse_raw = 0.05 + 0.25 * chaos_effect;
	slow_raw = 0.03 + 0.22 * chaos_effect;
	granular_raw = 0.02 + 0.18 * chaos_effect;
	raw_sum = reverse_raw + slow_raw + granular_raw;
	remaining_prob = max(0.02, 1.0 - normal_prob);
	raw_scale = remaining_prob / max(1e-3, raw_sum);
	reverse_prob = prob_slew(reverse_raw * raw_scale);
	slow_prob = prob_slew(slow_raw * raw_scale);
	granular_prob = max(0.0, 1.0 - (normal_prob + reverse_prob + slow_prob));

	reverse_threshold = normal_prob + reverse_prob;
	slowmo_threshold = reverse_threshold + slow_prob;
	effect_mode = ba.if(calm_force > 0.05, 0,
								ba.if(effect_selector < normal_prob - 0.01, 0,
								ba.if(effect_selector < reverse_threshold - 0.01, 1,
								ba.if(effect_selector < slowmo_threshold - 0.01, 2, 3))));

	burst_trim_db = -0.5 - chaos_sharp * 2.5;
	burst_trim = ba.db2linear(burst_trim_db);
	burst_gain = ba.if(effect_mode == 0, 1.0, burst_trim);

	stutter_phase_reverse = os.phasor(1, repeat_cycles);
	stutter_pos_reverse = int((1.0 - stutter_phase_reverse) * safe_capture) % max(1, safe_capture);
	stutter_pos_reverse_j = (stutter_pos_reverse + jitter_offset + safe_capture) % max(1, safe_capture);
	read_idx_reverse = (write_ptr - safe_capture + stutter_pos_reverse_j + buf_sz) % buf_sz;
	delayed_reverse = de.delay(buf_sz, read_idx_reverse, x) * fade_env;

	slow_repeat = repeat_rate_p / 1000.0;
	slow_cycles = min(max_repeat_cycles, slow_repeat / capture_sec);
	stutter_phase_slow = os.phasor(1, slow_cycles);
	stutter_pos_slow = int(stutter_phase_slow * safe_capture) % max(1, safe_capture);
	stutter_pos_slow_j = (stutter_pos_slow + jitter_offset + safe_capture) % max(1, safe_capture);
	read_idx_slow = (write_ptr - safe_capture + stutter_pos_slow_j + buf_sz) % buf_sz;
	delayed_slow = de.delay(buf_sz, read_idx_slow, x) * fade_env;

	granular_grain_ms = 26.0 + chaos_sharp * 90.0;
	granular_grain_samps = max(32, int(ma.SR * granular_grain_ms / 1000.0));
	granular_rate_hz = ma.SR / float(granular_grain_samps);
	granular_phase_1 = os.phasor(1, granular_rate_hz);
	grain_pos_1 = int(granular_phase_1 * safe_capture) % max(1, safe_capture);
	read_idx_granular_1 = (write_ptr - safe_capture + grain_pos_1 + buf_sz) % buf_sz;
	grain_sample_1 = de.delay(buf_sz, read_idx_granular_1, x);
	grain_window = sin(granular_phase_1 * ma.PI);
	delayed_granular = grain_sample_1 * grain_window * 0.8;

	effect_out_raw = ba.if(effect_mode == 0, delayed,
									 ba.if(effect_mode == 1, delayed_reverse,
									 ba.if(effect_mode == 2, delayed_slow, delayed_granular)));
	chaos_makeup = 1.0 + 0.18 * chaos_wild;
	effect_out = effect_out_raw * burst_gain * chaos_makeup;
	effect_out_guard = ba.if((calm_force > 0.02) | (chaos_raw < 0.12), delayed, effect_out);

	transient_hit = transient_norm > 0.65;
	transient_edge = max(0.0, transient_hit - transient_hit');
	inj_rel_coeff = exp(-1.0 / max(1.0, (140.0 * ma.SR / 1000.0)));
	inj_env = transient_edge : (+ ~ *(inj_rel_coeff));
	inj_mix_raw = min(0.22, inj_env * (0.28 + 0.4 * transient_norm) * (0.6 + 0.4 * fidelity));
	inj_mix = inj_mix_raw * bypass_env;
	ghost = x * 0.65 + delayed * 0.35;
	effect_pre_gap = effect_out_guard * (1.0 - inj_mix) + ghost * inj_mix;

	gap_noise_duck = 1.0 - min(0.8, energy_norm * 0.8);
	out = effect_pre_gap * gap_env + gap_noise * (1.0 - gap_env) * gap_noise_duck;
};

comp_drive_amt(amt) = 1.5 + 3.5 * amt;
comp_curve(u, drive) = u / (1.0 + drive * abs(u));
comp_expand(v, drive) = v / max(0.05, (1.0 - drive * min(0.999, abs(v))));
quantize_step(u, step) = floor(u / step + 0.5) * step : max(-1.0) : min(1.0);

lofi(x, amt) = result
with {
	amount = clamp01(amt);
	lofi_curve = amount * amount * (3.0 - 2.0 * amount);
	x0 = max(-1.0, min(1.0, x));
	pre_drive = 1.0 + amount * 6.0;
	pre_shaped = (x0 * pre_drive) / (1.0 + (pre_drive - 1.0) * abs(x0));

	comp_drive = comp_drive_amt(amount) + 0.75 * lofi_curve;
	comp_instant = comp_curve(pre_shaped, comp_drive);

	bits = max(5.0, 16.0 - amount * 10.0);
	step = 1.0 / pow(2.0, bits);
	dither_amp = step * (0.20 + 0.30 * amount);

	lp_hz = max(140.0, 16000.0 - amount * 14000.0);
	sr_hz = max(450.0, 22050.0 - amount * 21600.0);
	sr_period = max(1, int(ma.SR / sr_hz));
	sr_trig = (ba.period(sr_period) == 0);
	held_shaped = ba.sAndH(sr_trig, pre_shaped);
	held_comp = ba.sAndH(sr_trig, comp_instant);

	q_lin_instant = quantize_step(pre_shaped + (no.noise - no.noise) * dither_amp, step);
	q_lin_held = quantize_step(held_shaped + (no.noise - no.noise) * dither_amp, step);
	q_cmp_instant = quantize_step(comp_instant + (no.noise - no.noise) * dither_amp, step);
	q_cmp_held = quantize_step(held_comp + (no.noise - no.noise) * dither_amp, step);

	decomp_instant = comp_expand(q_cmp_instant, comp_drive);
	decomp_held = comp_expand(q_cmp_held, comp_drive);
	comp_strength = 0.35 + 0.65 * amount;
	crush_instant = q_lin_instant * (1.0 - comp_strength) + decomp_instant * comp_strength;
	crush_held = q_lin_held * (1.0 - comp_strength) + decomp_held * comp_strength;
	hold_mix = lofi_curve;
	crush_core = crush_instant * (1.0 - hold_mix) + crush_held * hold_mix;

	makeup = 1.0 + 0.55 * amount;
	bit_out = fi.lowpass(1, lp_hz, crush_core * makeup);

	tilt_freq = 4000.0 + amount * 2000.0;
	tilt_boost_db = amount * 6.0 + (1.0 - min(1.0, amount * 3.0)) * 1.5;
	tilt_boost = ba.db2linear(tilt_boost_db) - 1.0;
	tilt_high = fi.highpass(1, tilt_freq, bit_out);
	bit_tilt = bit_out + tilt_high * tilt_boost;

	bit_mix = amount;
	bit_blend = pre_shaped * (1.0 - bit_mix) + bit_tilt * bit_mix;

	rate_div = max(1, int(1 + amount * 7));
	rate_counter = (+(1) : %(rate_div)) ~ _;
	rate_trig = (rate_counter == 0);
	rate_hold = ba.sAndH(rate_trig, bit_tilt);
	rate_lp_hz = max(180.0, lp_hz / (0.8 + 0.35 * float(rate_div)));
	rate_lp = fi.lowpass(1, rate_lp_hz, rate_hold);

	rate_mix = amount * (0.40 + 0.60 * hold_mix);
	hybrid_wet = bit_blend * (1.0 - rate_mix) + rate_lp * rate_mix;
	body_mix = amount * (0.55 + 0.45 * lofi_curve);
	result = x0 * (1.0 - body_mix) + hybrid_wet * body_mix;
};

soft_sat(x) = x * (27.0 + x * x) / (27.0 + 9.0 * x * x);

limiter(x) = limited
with {
	ceiling = 0.98;
	knee_width = 0.08 * fidelity;
	knee_start = ceiling - knee_width;
	look_samps = max(1, int(ma.SR * 1.5 / 1000.0));
	rel_ms = 8.0 + 12.0 * fidelity;
	rel_coeff = exp(-1.0 / max(1.0, (rel_ms * ma.SR / 1000.0)));
	x_look = de.delay(4096, look_samps, x);
	env = abs(x_look) : (max ~ *(rel_coeff));
	over = max(0.0, env - knee_start);
	blend = min(1.0, over / max(1e-6, knee_width));
	hard_gain = ceiling / max(ceiling, env);
	gain = 1.0 * (1.0 - blend) + hard_gain * blend;
	limited = x_look * gain;
};

stereo_cross_probability = 0.05;
stereo_decision = ba.sAndH(trig, rnd1);
will_cross_blend = stereo_decision < stereo_cross_probability;
stereo_blend_amt = 0.02 + chaos * 0.06;

process(left_in, right_in) = (out_l, out_r)
with {
	xover_lo = 220.0;
	xover_hi = 2200.0;
	capture_scale_low = 1.12;
	capture_scale_mid = 1.00;
	capture_scale_high = 0.86;
	band_trim_low = 0.98;
	band_trim_mid = 1.00;
	band_trim_high = 0.95;

	in_l = left_in;
	in_r = right_in;

	low_l = in_l : fi.lowpass(2, xover_lo);
	high1_l = in_l : fi.highpass(2, xover_lo);
	mid_l = high1_l : fi.lowpass(2, xover_hi);
	high_l = high1_l : fi.highpass(2, xover_hi);

	low_r = in_r : fi.lowpass(2, xover_lo);
	high1_r = in_r : fi.highpass(2, xover_lo);
	mid_r = high1_r : fi.lowpass(2, xover_hi);
	high_r = high1_r : fi.highpass(2, xover_hi);

	low_stutter_l = stutter_processor(low_l, r1_low, r2_low, r3_low, r4_low, r5_low, walk_repeat_low, walk_capture_low, repeat_rate_low, capture_scale_low) * band_trim_low;
	mid_stutter_l = stutter_processor(mid_l, r1_mid, r2_mid, r3_mid, r4_mid, r5_mid, walk_repeat_mid, walk_capture_mid, repeat_rate_mid, capture_scale_mid) * band_trim_mid;
	high_stutter_l = stutter_processor(high_l, r1_high, r2_high, r3_high, r4_high, r5_high, walk_repeat_high, walk_capture_high, repeat_rate_high, capture_scale_high) * band_trim_high;

	low_stutter_r = stutter_processor(low_r, r1_low, r2_low, r3_low, r4_low, r5_low, walk_repeat_low, walk_capture_low, repeat_rate_low, capture_scale_low) * band_trim_low;
	mid_stutter_r = stutter_processor(mid_r, r1_mid, r2_mid, r3_mid, r4_mid, r5_mid, walk_repeat_mid, walk_capture_mid, repeat_rate_mid, capture_scale_mid) * band_trim_mid;
	high_stutter_r = stutter_processor(high_r, r1_high, r2_high, r3_high, r4_high, r5_high, walk_repeat_high, walk_capture_high, repeat_rate_high, capture_scale_high) * band_trim_high;

	lofi_low_amt = lofi_amt * 0.35;
	lofi_mid_amt = min(1.0, lofi_amt * 0.95 + 0.05 * chaos_texture);
	lofi_high_amt = min(1.0, lofi_amt * 1.20);

	low_proc_l = lofi(low_stutter_l, lofi_low_amt);
	mid_proc_l = lofi(mid_stutter_l, lofi_mid_amt);
	high_proc_l = lofi(high_stutter_l, lofi_high_amt);

	low_proc_r = lofi(low_stutter_r, lofi_low_amt);
	mid_proc_r = lofi(mid_stutter_r, lofi_mid_amt);
	high_proc_r = lofi(high_stutter_r, lofi_high_amt);

	left_stutter = low_proc_l + mid_proc_l + high_proc_l;
	right_stutter = low_proc_r + mid_proc_r + high_proc_r;

	decor_scale = bypass_env * (1.0 - 0.7 * fidelity);
	decor_max_samps = max(0, min(1023, int(ma.SR * 0.0025 * decor_scale)));
	decor_l_samps = int(decor_max_samps * r2);
	decor_r_samps = int(decor_max_samps * r4);
	left_stutter_d = de.delay(1024, decor_l_samps, left_stutter);
	right_stutter_d = de.delay(1024, decor_r_samps, right_stutter);
	decor_ok = (decor_l_samps != decor_r_samps) | (decor_l_samps > 0) | (decor_r_samps > 0);
	cross_gate = will_cross_blend & decor_ok;

	blend_l = left_stutter_d * (1.0 - cross_gate * stereo_blend_amt) + right_stutter_d * cross_gate * stereo_blend_amt;
	blend_r = right_stutter_d * (1.0 - cross_gate * stereo_blend_amt) + left_stutter_d * cross_gate * stereo_blend_amt;

	pan_chaos_gate = max(0.0, chaos - 0.32) / 0.68;
	pan_gate = pan_chaos_gate * bypass_env;
	pan_target = (ba.sAndH(walk_trig, rnd4) - 0.5) * 0.9;
	pan_state = pan_target : smooth_ms(600.0);
	pan_depth = pan_gate * (0.06 + 0.16 * chaos_sharp);
	pan_delay_ms = pan_gate * (0.4 + 5.0 * chaos_sharp) * abs(pan_state);
	pan_delay_samps = max(0, int(ma.SR * pan_delay_ms / 1000.0));
	delay_l_samps = ba.if(pan_state >= 0.0, pan_delay_samps, 0);
	delay_r_samps = ba.if(pan_state < 0.0, pan_delay_samps, 0);
	blend_l_dpan = de.delay(2048, delay_l_samps, blend_l);
	blend_r_dpan = de.delay(2048, delay_r_samps, blend_r);
	mid_pan = 0.5 * (blend_l_dpan + blend_r_dpan);
	side_pan = 0.5 * (blend_l_dpan - blend_r_dpan) + mid_pan * pan_state * pan_depth;

	side_shaper = 1.0 - cross_gate * stereo_blend_amt * 1.2;
	guard_l = mid_pan + side_pan * side_shaper;
	guard_r = mid_pan - side_pan * side_shaper;

	enabled_l = in_l + bypass_env * (guard_l - in_l);
	enabled_r = in_r + bypass_env * (guard_r - in_r);

	out_l = enabled_l * output_gain : limiter : soft_sat : fi.dcblocker;
	out_r = enabled_r * output_gain : limiter : soft_sat : fi.dcblocker;
};
 
