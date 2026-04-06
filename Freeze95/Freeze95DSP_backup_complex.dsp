import("stdfaust.lib");

// Freeze95 - compact stutter DSP for iPlug2 integration
// Derived from Quantum Stutter Randomizer; simplified for fast faust compilation.
declare name "Freeze95";
declare version "1.0";

// -- PARAMETERS ---------------------------------------------------------------
bypass_raw  = checkbox("Bypass[tooltip: Power on/off]");
chaos_raw   = hslider("Chaos[style:knob]",  0.55, 0.0, 1.0, 0.01);
lofi_raw    = hslider("Lo-Fi[style:knob]",  0.5, 0.0, 1.0, 0.01);
bpm_host    = hslider("transport/bpm",      120, 20,  300, 0.1);
sync_on     = checkbox("Sync BPM[tooltip: Quantize stutter to host BPM]");

// -- SMOOTHING HELPERS --------------------------------------------------------
glide(ms) = si.smooth(exp(-1.0 / max(1.0, ma.SR * ms / 1000.0)));
clamp01(x) = min(1.0, max(0.0, x));
wrapf(value, size) = value - size * floor(value / size);
delay_lerp(maxDelay, delayF, x) = out
with {
	delay0 = int(floor(delayF));
	delay1 = (delay0 + 1) % maxDelay;
	frac   = delayF - float(delay0);
	tap0   = de.delay(maxDelay, delay0, x);
	tap1   = de.delay(maxDelay, delay1, x);
	out    = tap0 + (tap1 - tap0) * frac;
};

stutter_on = (1.0 - bypass_raw) : glide(8.0);
chaos      = chaos_raw           : glide(8.0);
lofi       = lofi_raw            : glide(8.0);

// -- DERIVED CHAOS CURVES -----------------------------------------------------
chaos_focus   = pow(chaos, 1.1);
chaos_texture = pow(clamp01((chaos - 0.18) / 0.62), 1.35);
chaos_damage  = pow(clamp01((chaos - 0.58) / 0.42), 1.8);
chaos_sharp   = min(1.0, 0.72 * chaos_focus + 0.28 * chaos_texture);
fidelity      = 1.0 - chaos_sharp;
calm_mode     = clamp01(1.0 - chaos_raw / 0.12);

// -- BUFFER -------------------------------------------------------------------
buf_ms  = 4000.0;
buf_sz  = int(ma.SR * buf_ms / 1000.0);
write_ptr = ba.period(buf_sz);

// -- RANDOM GENERATORS (5 independent LCGs) ----------------------------------
rnd1 = (+(111) ~ *(1103515245)) : +(12345) : %(2147483647) : /(2147483647.0);
rnd2 = (+(222) ~ *(1103515245)) : +(23456) : %(2147483647) : /(2147483647.0);
rnd3 = (+(333) ~ *(1103515245)) : +(34567) : %(2147483647) : /(2147483647.0);
rnd4 = (+(444) ~ *(1103515245)) : +(45678) : %(2147483647) : /(2147483647.0);
rnd5 = (+(555) ~ *(1103515245)) : +(56789) : %(2147483647) : /(2147483647.0);

// -- UPDATE TRIGGER -----------------------------------------------------------
update_ms   = 160.0 + pow(1.0 - chaos_focus, 2.0) * 3040.0;
update_rate = max(1, int(update_ms * ma.SR / 1000.0));
counter     = (+(1) : %(update_rate)) ~ _;
chaos_prev  = chaos_raw';
chaos_delta = abs(chaos_raw - chaos_prev);
chaos_moved = chaos_delta > 0.004;
chaos_edge  = max(0.0, chaos_moved - chaos_moved');
trig        = max(counter == 0, chaos_edge);

// -- SAMPLED RANDOM VALUES ----------------------------------------------------
r1 = ba.sAndH(trig, rnd1);
r2 = ba.sAndH(trig, rnd2);
r3 = ba.sAndH(trig, rnd3);
r4 = ba.sAndH(trig, rnd4);
r5 = ba.sAndH(trig, rnd5);

// -- HOLD / SKIP COUNTER ------------------------------------------------------
min_hold_ms  = 100.0 + fidelity * 140.0;
max_hold_ms  = 2000.0 + fidelity * 6000.0;
hold_ms      = min_hold_ms + (max_hold_ms - min_hold_ms) * pow(r2, 0.6);
hold_updates = max(1, int(hold_ms / update_ms));
skip_ctr     = select2(trig, skip_ctr - 1, hold_updates) : max(0) ~ _;

// -- REPEAT RATE --------------------------------------------------------------
bpm_safe = max(20.0, bpm_host);
beat_hz  = bpm_safe / 60.0;
g1 = beat_hz * 0.25;
g2 = beat_hz * 0.5;
g3 = beat_hz * 1.0;
g4 = beat_hz * 2.0;
g5 = beat_hz * 3.0;
g6 = beat_hz * 4.0;
g7 = beat_hz * 8.0;
m12 = (g1+g2)*0.5; m23 = (g2+g3)*0.5; m34 = (g3+g4)*0.5;
m45 = (g4+g5)*0.5; m56 = (g5+g6)*0.5; m67 = (g6+g7)*0.5;
sync_q(f) = ba.if(f<m12, g1,
			 ba.if(f<m23, g2,
			 ba.if(f<m34, g3,
			 ba.if(f<m45, g4,
			 ba.if(f<m56, g5,
			 ba.if(f<m67, g6, g7))))));

min_hz          = 0.35 + chaos_sharp * 2.6;
max_hz          = 10.0 + chaos_focus * 20.0 + chaos_texture * 34.0 + chaos_damage * 54.0;
repeat_rate_nom = min_hz + (max_hz - min_hz) * r3;
repeat_rate_raw = repeat_rate_nom * (1.0 - 0.45 * fidelity) + 0.3825 * fidelity;
repeat_rate     = ba.if(sync_on > 0.0, sync_q(repeat_rate_raw), repeat_rate_raw);

// -- CAPTURE SIZE -------------------------------------------------------------
min_cap_ms  = 50.0 + fidelity * 137.0;
max_cap_ms  = 100.0 + fidelity * 275.0;
capture_ms  = min_cap_ms + (max_cap_ms - min_cap_ms) * r1 * r1;
capture_sz  = max(1, min(int(capture_ms * ma.SR / 1000.0), buf_sz));
cap_sec     = max(1.0 / ma.SR, float(capture_sz) / ma.SR);
rep_cycles  = min(500.0, repeat_rate / cap_sec);

// -- STUTTER PHASOR AND POSITIONS ---------------------------------------------
capture_f     = float(max(1, capture_sz));
stutter_ph    = os.phasor(1, rep_cycles);
stutter_pos_f = stutter_ph * capture_f;

jitter_span = (0.35 + chaos_texture * 9.0 + chaos_damage * 8.0) * (1.0 - 0.9 * calm_mode);
jitter_off  = int((r4 - 0.5) * jitter_span);
read_pos_f  = wrapf(stutter_pos_f + float(jitter_off) + capture_f, capture_f);
read_idx_f  = wrapf(float(write_ptr) - capture_f + read_pos_f + float(buf_sz), float(buf_sz));

// Reverse playhead (same rate, inverted phase)
rev_pos_f  = wrapf((1.0 - stutter_ph) * capture_f, capture_f);
rev_idx_f  = wrapf(float(write_ptr) - capture_f + rev_pos_f + float(buf_sz), float(buf_sz));

// -- CROSSFADE ENVELOPE -------------------------------------------------------
fade_samps = max(16, int(ma.SR * 0.0015));
fade_len_f = max(1.0, min(float(fade_samps), capture_f * 0.5));
edge_dist  = min(read_pos_f, capture_f - read_pos_f);
fade_env   = max(0.05, sin(min(1.0, edge_dist / fade_len_f) * (ma.PI * 0.5)));

// -- PER-CHANNEL STUTTER CORE -------------------------------------------------
stutter_ch(x) = out
with {
	d_fwd = delay_lerp(buf_sz, read_idx_f, x) * fade_env;
	d_rev = delay_lerp(buf_sz, rev_idx_f,  x) * fade_env;
	use_rev = (r4 > (0.94 - chaos_texture * 0.16 - chaos_damage * 0.20)) & (chaos_texture > 0.05) & (calm_mode < 0.05);
	out = ba.if(use_rev, d_rev, d_fwd);
};

// -- GAP / DROPOUT GATE -------------------------------------------------------
gap_prob     = chaos_damage * 0.38 * (1.0 - 0.9 * calm_mode);
will_gap     = (r5 < gap_prob) & (calm_mode < 0.05);
gap_per      = max(1, int(ma.SR * (0.03 + chaos_damage * 0.19)));
gap_ph_i     = ba.period(gap_per);
gap_ph_f     = float(gap_ph_i) / float(max(1, gap_per));
gap_frac     = min(0.49, 0.002 * ma.SR / float(max(1, gap_per)));

// Envelope: ramps up from 0, stays silent in first half, ramps back up in second half
gap_env_raw  = ba.if(gap_ph_f < gap_frac,
				   gap_ph_f / max(1e-6, gap_frac),
				   ba.if(gap_ph_f < 0.5 - gap_frac,
					   0.0,
					   ba.if(gap_ph_f < 0.5,
						   (gap_ph_f - (0.5 - gap_frac)) / max(1e-6, gap_frac),
						   1.0)));
gap_env      = ba.if(will_gap, gap_env_raw, 1.0);

// -- LO-FI PROCESSING ---------------------------------------------------------
lofi_drive  = 1.0 + lofi * 6.0;
lofi_bits   = max(5.0, 16.0 - lofi * 10.0);
lofi_step   = 1.0 / pow(2.0, lofi_bits);
lofi_lp_hz  = max(90.0, 15000.0 - lofi * 13200.0);
lofi_sr_hz  = max(450.0, 22050.0 - lofi * 21600.0);
lofi_period = max(1, int(ma.SR / lofi_sr_hz));
lofi_trig   = (ba.period(lofi_period) == 0);
lofi_shape(x) = (x * lofi_drive) / (1.0 + (lofi_drive - 1.0) * abs(x));
lofi_hold(x)  = ba.sAndH(lofi_trig, x);
lofi_crush(x) = floor(max(-1.0, min(1.0, x)) / lofi_step + 0.5) * lofi_step;
lofi_chain(x) = x : lofi_shape : lofi_hold : lofi_crush : fi.lowpass(1, lofi_lp_hz);

// -- SOFT LIMITER -------------------------------------------------------------
soft_lim(x) = x * (27.0 + x*x) / (27.0 + 9.0 * x*x);

// -- MAIN STEREO PROCESS ------------------------------------------------------
process(L, R) = (outL, outR)
with {
	bypass_s = stutter_on;
	wet_l    = stutter_ch(L) * gap_env : lofi_chain : soft_lim;
	wet_r    = stutter_ch(R) * gap_env : lofi_chain : soft_lim;
	outL     = L + bypass_s * (wet_l - L);
	outR     = R + bypass_s * (wet_r - R);
};
 
