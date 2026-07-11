// Grungr
// Report-driven Faust model centered on the stock JFET input, cascaded 4558
// clipping stages, asymmetric final clipping, and the active scooped tone stack.

import("stdfaust.lib");

declare name "Grungr";
declare author "cloph";
declare version "2.7";

//============================================================================
// UI Controls
//============================================================================

grunge_knob = hslider("[1]Grunge[style:knob]", 0.70, 0, 1, 0.01) : si.smoo;
butt_knob = hslider("[2]Butt[style:knob]", 0.62, 0, 1, 0.01) : si.smoo;
face_knob = hslider("[3]Face[style:knob]", 0.56, 0, 1, 0.01) : si.smoo;
loud_knob = hslider("[4]Loud[style:knob]", 0.72, 0, 1, 0.01) : si.smoo;
voicing_raw = hslider("[5]RAW Voicing[style:knob]", 0, 0, 2, 1);
bypass_switch = checkbox("[6]Bypass[style:switch]");

mode_select = int(voicing_raw + 0.5) : min(2) : max(0);
raw_mode = (mode_select == 0);
enhanced_mode = (mode_select == 1);
bass_mode = (mode_select == 2);
mod_active = enhanced_mode : si.smoo;
bass_active = bass_mode : si.smoo;

//============================================================================
// Helpers and Report-Derived Constants
//============================================================================

lininterp(a, b, t) = a + t * (b - a);
db2lin(db) = ba.db2linear(db);
audio_pot(x, curve) = pow(max(x, ma.EPSILON), curve);
clip01(x) = min(1.0, max(0.0, x));
dynamic_sag(amount, min_gain) = _
    <: _, (abs : si.smoo : *(amount / depth_limit) : clip01 : *(depth_limit) : *(-1.0) : +(1.0))
    : *
with {
    depth_limit = max(1.0 - min_gain, ma.EPSILON);
};

// NAM captures show the stock pedal's drive control compressing and darkening
// toward the top of the sweep rather than just getting louder.
grunge_drive = clip01(1.32 * grunge_knob - 0.32 * grunge_knob * grunge_knob);
grunge_edge = clip01((grunge_knob - 0.82) / 0.18);
butt_taper = audio_pot(butt_knob, 0.82);
face_taper = audio_pot(face_knob, 0.88);
loud_taper = audio_pot(loud_knob, 1.65);
enhanced_drive_t = clip01((grunge_knob - 0.55) / 0.45);
bass_drive_t = clip01((grunge_knob - 0.40) / 0.60);
drive_tone_t = clip01((grunge_knob - 0.35) / 0.65);
enhanced_edge_t = clip01((grunge_knob - 0.70) / 0.30);
bass_edge_t = clip01((grunge_knob - 0.65) / 0.35);
voicing_blend = mod_active * enhanced_drive_t;
bass_voicing_blend = bass_active * bass_drive_t;
stock_blend = 1.0 - (voicing_blend + bass_voicing_blend);
drive_floor = 0.07 * (1.0 - grunge_drive);
stock_drive_lift = stock_blend * (0.22 * (1.0 - grunge_drive));
enhanced_drive_lift = voicing_blend * (0.10 * (1.0 - grunge_drive) + 0.05 * grunge_edge);
bass_drive_lift = bass_voicing_blend * (0.08 * (1.0 - grunge_drive) + 0.03 * bass_edge_t);
drive_core = clip01(grunge_drive + drive_floor + stock_drive_lift + enhanced_drive_lift + bass_drive_lift);

instrument_trim = 0.20;
stock_low_hz = 120.0;
stock_high_hz = 3000.0;
stock_mid_hz = 620.0;
stock_mid_q = 0.95;
stock_mid_scoop_db = -5.6;
stock_input_hpf_hz = 1.6;
stock_input_lp_hz = 15000.0;
stock_output_lp_hz = 14600.0;
stock_output_trim = 0.86;
face_air = 0.50 + 0.50 * face_taper;

// Enhanced mode is allowed to depart from stock voicing, but raw mode must
// stay pinned to the measured pedal path when the switch is off.
enhanced_tight_t = mod_active * drive_tone_t;
enhanced_stage_relief_db = voicing_blend * (0.90 + 1.70 * grunge_drive);
enhanced_late_gain_db = voicing_blend * (0.70 + 1.80 * grunge_edge);
enhanced_clip_relief = voicing_blend * (0.020 + 0.015 * (1.0 - grunge_drive));
enhanced_bias_shift = -voicing_blend * (0.004 + 0.006 * enhanced_edge_t);
enhanced_input_hpf_hz = enhanced_tight_t * (12.0 + 30.0 * grunge_drive);
enhanced_stage2_hpf_boost_hz = enhanced_tight_t * (16.0 + 24.0 * grunge_drive);
enhanced_stage3_hpf_boost_hz = voicing_blend * (10.0 + 18.0 * grunge_drive);
enhanced_stage1_lp_trim_hz = voicing_blend * (280.0 + 820.0 * grunge_edge);
enhanced_stage2_lp_trim_hz = voicing_blend * (220.0 + 620.0 * grunge_edge);
enhanced_stage3_lp_trim_hz = enhanced_tight_t * face_air * (420.0 + 960.0 * grunge_edge);
enhanced_presence_trim_db = enhanced_tight_t * face_air * (0.20 + 0.40 * (1.0 - grunge_drive));
enhanced_fizz_trim_db = -enhanced_tight_t * face_air * (0.90 + 1.80 * grunge_edge);
enhanced_output_makeup_db = enhanced_tight_t * (0.35 + 0.45 * grunge_drive - 0.25 * grunge_edge);

// MOD voicing refinements — modern, polished, professional character.
// Pre-emphasis / de-emphasis around clipping reduces the harsh harmonics that
// land in the 3–5 kHz band; an HF intermodulation notch catches the sum/diff
// products that build up with complex chord work; a steeper sub-bass HPF
// keeps the low end defined instead of loose. All gated on voicing_blend so
// RAW mode stays bit-for-bit unchanged.
mod_preemph_db = voicing_blend * (1.3 + 0.5 * (1.0 - grunge_drive));
mod_deemph_db = -mod_preemph_db;
mod_preemph_hz = 3500.0;
mod_hf_im_notch_hz = 4500.0;
mod_hf_im_notch_q = 1.5;
mod_hf_im_notch_db = -voicing_blend * (1.8 + 0.6 * grunge_edge);
mod_sub_hpf_hz = max(20.0, mod_active * (40.0 + 12.0 * grunge_drive));

// Bass mode is optimized for bass guitar: lower gain staging for hot inputs,
// minimal HPF to preserve low fundamentals, higher clip thresholds for
// headroom, and more compression for sustain.
bass_stage_relief_db = bass_voicing_blend * (1.20 + 2.00 * grunge_drive);
bass_late_gain_db = bass_voicing_blend * (0.50 + 1.50 * bass_edge_t);
bass_clip_relief = bass_voicing_blend * (0.025 + 0.020 * (1.0 - grunge_drive));
bass_bias_shift = -bass_voicing_blend * (0.003 + 0.005 * bass_edge_t);
bass_stage2_hpf_boost_hz = bass_voicing_blend * (8.0 + 12.0 * grunge_drive);
bass_stage3_hpf_boost_hz = bass_voicing_blend * (6.0 + 10.0 * grunge_drive);
bass_stage1_lp_trim_hz = bass_voicing_blend * (200.0 + 600.0 * bass_edge_t);
bass_stage2_lp_trim_hz = bass_voicing_blend * (160.0 + 460.0 * bass_edge_t);
bass_stage3_lp_trim_hz = bass_voicing_blend * face_air * (320.0 + 720.0 * bass_edge_t);
bass_mid_scoop_trim_db = bass_voicing_blend * 3.0;
bass_fizz_shelf_hz = 4200.0 + (2500.0 - 4200.0) * bass_active;
bass_fizz_trim_db = -bass_voicing_blend * face_air * (0.70 + 1.40 * bass_edge_t);
bass_output_makeup_db = bass_voicing_blend * (0.50 + 0.55 * grunge_drive - 0.20 * bass_edge_t);
bass_power_sag = bass_voicing_blend * (0.024 + 0.050 * grunge_drive * grunge_drive);
bass_stage_sag_floor_offset = bass_voicing_blend * (-0.08);
bass_output_sag_floor_offset = bass_voicing_blend * (-0.06);

// The report calls out the 4558's 1.0 V/us slew rate as a meaningful part of
// the pedal character. A fixed-step ramp is a stable way to emulate that limit.
opamp_slew = ba.ramp(ma.SR / 1000000.0);

stage1_gain_db = 18.0 + 19.0 * drive_core - enhanced_stage_relief_db - bass_stage_relief_db;
stage2_gain_db = 12.0 + 14.0 * drive_core - 0.65 * (enhanced_stage_relief_db + bass_stage_relief_db);
stage3_gain_db = 6.0 + 6.0 * drive_core + enhanced_late_gain_db + bass_late_gain_db;

stage1_clip_threshold = 0.76 - 0.10 * drive_core + enhanced_clip_relief + bass_clip_relief;
stage2_clip_threshold = 0.74 - 0.12 * drive_core + 0.85 * (enhanced_clip_relief + bass_clip_relief);
stage2_clip_bias = -0.010 - 0.018 * drive_core + enhanced_bias_shift + bass_bias_shift;
stage3_pos_threshold = 1.10 - 0.16 * drive_core;
stage3_neg_threshold = 0.74 - 0.10 * drive_core;
stage3_led_pos_threshold = 1.62 - 0.12 * drive_core;
stage3_led_neg_threshold = 1.08 - 0.08 * drive_core;
stage3_led_mix = voicing_blend * enhanced_edge_t * (0.08 + 0.34 * grunge_edge);

stage1_lp_hz = 10800.0 - 2000.0 * drive_core - enhanced_stage1_lp_trim_hz - bass_stage1_lp_trim_hz;
stage2_hpf_hz = 80.0 + 25.0 * drive_core + 12.0 * voicing_blend + enhanced_stage2_hpf_boost_hz + bass_stage2_hpf_boost_hz;
stage2_lp_hz = 8400.0 - 1600.0 * drive_core - enhanced_stage2_lp_trim_hz - bass_stage2_lp_trim_hz;
stage3_hpf_hz = 110.0 + 20.0 * drive_core + enhanced_stage3_hpf_boost_hz + bass_stage3_hpf_boost_hz;
stage3_lp_hz = 11200.0 - 1800.0 * drive_core - 900.0 * grunge_edge - enhanced_stage3_lp_trim_hz - bass_stage3_lp_trim_hz;
tone_mid_scoop_db = stock_mid_scoop_db + drive_tone_t * (0.95 - 0.55 * grunge_drive) + bass_mid_scoop_trim_db;
presence_peak_hz = lininterp(1450.0, 2400.0, voicing_blend);
presence_peak_q = 0.72;
presence_peak_db = drive_tone_t * face_air * (0.70 - 0.30 * grunge_drive) + enhanced_presence_trim_db;
output_lp_hz = stock_output_lp_hz - drive_tone_t * face_air * (900.0 * grunge_drive + 650.0 * grunge_edge) - enhanced_tight_t * face_air * (350.0 + 850.0 * grunge_edge);
im_notch_db = -3.0 * voicing_blend * enhanced_edge_t;
power_sag = voicing_blend * (0.016 + 0.040 * grunge_drive * grunge_drive) + bass_power_sag;
stage_sag_floor = 0.76 + 0.04 * stock_blend + bass_stage_sag_floor_offset;
output_sag_floor = 0.84 + 0.03 * stock_blend + bass_output_sag_floor_offset;

low_shelf_db = lininterp(-9.0, 15.0, butt_taper);
high_shelf_db = lininterp(-8.0, 11.0, face_taper);
output_gain = loud_taper * db2lin(0.95 - 1.10 * grunge_edge * grunge_edge + 2.1 * stock_blend * (drive_core - grunge_drive) - 0.55 * voicing_blend * enhanced_edge_t + enhanced_output_makeup_db + bass_output_makeup_db);

centered_soft_clip(threshold, drive, bias) =
    +(bias)
    : *(drive / threshold)
    : aa.arctan2
    : *(2.0 * threshold / ma.PI)
    : +(-dc)
with {
    dc = (2.0 * threshold / ma.PI) * atan((bias * drive) / threshold);
};

diode_half(v, threshold) = v / threshold : aa.hardclip2 : *(threshold);
asym_ground_sample(x, pos_threshold, neg_threshold) =
    select2(x > 0, diode_half(x, neg_threshold), diode_half(x, pos_threshold));

asym_ground_clip(pos_threshold, neg_threshold) = \(x).(asym_ground_sample(x, pos_threshold, neg_threshold))
with {
    pos_half(v) = v / pos_threshold : aa.hardclip2 : *(pos_threshold);
    neg_half(v) = v / neg_threshold : aa.hardclip2 : *(neg_threshold);
};

hybrid_ground_clip = \(x).(
    (1.0 - stage3_led_mix) * asym_ground_sample(x, stage3_pos_threshold, stage3_neg_threshold)
    + stage3_led_mix * asym_ground_sample(x, stage3_led_pos_threshold, stage3_led_neg_threshold)
);

//============================================================================
// Signal Path
//============================================================================

// In mod mode, attenuate loud transients before the input clipper to prevent
// splat from hard pick attacks. Fast peak follower (1 ms / 50 ms); engages
// only when signal envelope exceeds ~-8 dBFS, then ramps up to 0.7 (-3 dB)
// reduction. RAW mode: voicing_blend=0 -> depth=0, signal unchanged.
mod_transient_attenuate = _
    <: _, (an.amp_follower_ud(0.001, 0.05) : -(0.4) : clip01 : *(voicing_blend * 0.7) : *(-1.0) : +(1.0))
    : *;

// In mod mode, blend a small amount of tanh saturation in parallel with the
// stage2 output for subtle even-harmonic richness (tube-style warmth). RAW
// mode: voicing_blend=0 -> warmth contribution is 0, signal unchanged.
mod_tanh_warmth = _
    <: _, (*(0.4) : ma.tanh : *(0.05 * voicing_blend))
    : +;

// In mod mode, engage a gentle lookahead limiter then a noise gate to control
// peaks and suppress hiss/hum during silent passages. Limiter: 5 ms lookahead,
// ceiling -0.5 dBFS (linear 0.944), 1 ms attack, 10 ms hold, 50 ms release.
// Gate: hold 10 ms, release 80 ms, threshold -50 dB, 2:1 expansion. RAW mode
// bypassed entirely. These are mono (applied per channel via the outer par).
mod_limiter = ba.bypass1(1.0 - mod_active,
    co.limiter_lad_mono(0.005, 0.944, 0.001, 0.01, 0.05));

mod_noise_gate = ba.bypass1(1.0 - mod_active,
    co.expander_N_chan(2.0, -50.0, -30.0, 0.005, 0.01, 0.08, 6.0, 0, 0.5, _, 1024, 1));

// In mod mode, apply a soft-knee makeup curve at output: signum(x) * |x|^0.97
// which gently boosts small signals and slightly attenuates large ones
// (mastering-style leveling curve). signum preserves stereo polarity.
pow097(x) = x ^ 0.97;
mod_makeup_curve = ba.bypass1(1.0 - mod_active,
    \(x).(ma.signum(x) * pow097(abs(x))));

//----------------------------------------------------------------------------
// Bass mode polish (gated on bass_active / bass_voicing_blend; transparent
// when mode_select != 2).
//----------------------------------------------------------------------------

// T3 "Polished" — LF transient control pre-saturation. Sidechain HP'd at 35 Hz
// so the envelope follower only sees sub-content; engagement above -6 dBFS into
// up to -3 dB of attenuation. RAW / non-bass: voicing=0 -> gain=1, unchanged.
bass_lf_transient_control = _
    <: _, (fi.highpass(2, 35.0)
              : an.amp_follower_ud(0.005, 0.08)
              : -(0.5)
              : clip01
              : *(bass_voicing_blend * 0.5)
              : *(-1.0)
              : +(1.0))
    : *;

// T1 "Round" — parallel even-harmonic tanh in stage2 next to mod_tanh_warmth.
// tanh(x*1.6) scaled by 0.20 * bass_voicing_blend. RAW: weight=0, no contribution.
bass_harmonic_warmth = _
    <: _, (*(1.6)
              : ma.tanh
              : *(0.20 * bass_voicing_blend))
    : +;

// T1 "Round" — LF-only even-harmonic shaper. abs+LPF+tanh generates DC + 2nd
// harmonic content concentrated below 150 Hz (octave-down feel). Mixed at 0.18
// on bass_voicing_blend. Complementary to bass_harmonic_warmth above.
bass_sub_shaper = _
    <: _, ((abs
              : *(1.4)
              : ma.tanh
              : fi.lowpass(2, 150.0)
              : *(0.18 * bass_voicing_blend)))
    : +;

// T2 "Bass-ready" — EQ foundation: +1.5 dB at 55 Hz, Q=0.7 for LF weight.
bass_sub_shelf = fi.peak_eq_cq(bass_voicing_blend * 1.5, 55.0, 0.7);

// T2 "Bass-ready" — EQ foundation: -2.0 dB at 250 Hz, Q=1.0 to clear muddiness
// before saturation. Complements the existing 320 Hz mud-band notch (mod path).
bass_lomid_dip = fi.peak_eq_cq(-bass_voicing_blend * 2.0, 250.0, 1.0);

// T2 "Bass-ready" — top-end restraint. Modulates the LP cutoff 18 kHz -> 6 kHz
// on bass_voicing_blend; gated on bass_active so raw / mod paths untouched.
bass_air_cut = ba.bypass1(1.0 - bass_active,
    fi.lowpass(2, 18000.0 - 12000.0 * bass_voicing_blend));

// T3 "Polished" — lookahead limiter, slower than mod's for "round" level hold.
// 10 ms lookahead, -0.5 dBFS ceiling (linear 0.944), 5/20/80 ms A/H/R. Mono.
bass_lookahead_limiter = ba.bypass1(1.0 - bass_active,
    co.limiter_lad_mono(0.010, 0.944, 0.005, 0.02, 0.08));

// T3 "Polished" — subsonic expander, kills sub-bass rumble between notes. 4:1
// expansion (vs mod's 2:1), deeper threshold (-55 dB), longer release (120 ms).
bass_subsonic_gate = ba.bypass1(1.0 - bass_active,
    co.expander_N_chan(4.0, -55.0, -35.0, 0.005, 0.02, 0.12, 6.0, 0, 0.5, _, 1024, 1));

// T1 "Round" — mastering-style leveling curve at chain end. Reuses pow097 above;
// mono per channel. Sits after bass_subsonic_gate so the gate's silence is
// preserved by the curve.
bass_makeup_curve = ba.bypass1(1.0 - bass_active,
    \(x).(ma.signum(x) * pow097(abs(x))));

input_buffer = _
    : *(instrument_trim)
    : fi.highpass(1, stock_input_hpf_hz + enhanced_input_hpf_hz)
    : fi.highpass(2, mod_sub_hpf_hz)
    : fi.lowpass(1, stock_input_lp_hz)
    : mod_transient_attenuate
    : bass_lf_transient_control
    : centered_soft_clip(1.8, 0.40 + 0.07 * grunge_drive - 0.03 * enhanced_tight_t, 0.01);

first_opamp_stage = _
    : fi.highpass(1, 35)
    : *(db2lin(stage1_gain_db))
    : opamp_slew
    : centered_soft_clip(stage1_clip_threshold, (1.00 + 0.05 * grunge_drive) * (1.0 - 0.18 * voicing_blend), 0.014)
    : fi.dcblocker
    : fi.lowpass(2, stage1_lp_hz);

second_opamp_stage = _
    : fi.highpass(1, stage2_hpf_hz)
    : fi.lowpass(1, stage2_lp_hz)
    : *(db2lin(stage2_gain_db))
    : opamp_slew
    : centered_soft_clip(stage2_clip_threshold, (1.10 + 0.10 * grunge_drive) * (1.0 - 0.16 * voicing_blend), stage2_clip_bias)
    : fi.dcblocker
    : fi.lowpass(2, stage2_lp_hz - 300.0)
    : mod_tanh_warmth
    : bass_harmonic_warmth
    : bass_sub_shaper;

third_opamp_stage = _
    : fi.highpass(1, stage3_hpf_hz)
    : *(db2lin(stage3_gain_db))
    : opamp_slew
    : dynamic_sag(power_sag, stage_sag_floor)
    : fi.high_shelf(mod_preemph_db, mod_preemph_hz)
    : fi.highshelf(3, mod_preemph_db, mod_preemph_hz * 1.2)
    : hybrid_ground_clip
    : fi.high_shelf(mod_deemph_db, mod_preemph_hz)
    : fi.highshelf(3, mod_deemph_db, mod_preemph_hz * 1.2)
    : fi.lowpass(2, stage3_lp_hz)
    : fi.dcblocker;

tone_stack = _
    : fi.low_shelf(low_shelf_db, stock_low_hz)
    : fi.high_shelf(high_shelf_db, stock_high_hz)
    : fi.peak_eq_cq(tone_mid_scoop_db, stock_mid_hz, stock_mid_q)
    : fi.peak_eq_cq(presence_peak_db, presence_peak_hz, presence_peak_q)
    : fi.peak_eq_cq(im_notch_db, 55.0, 2.5)
    : fi.peak_eq_cq(-voicing_blend * 2.5, 320.0, 1.1)
    : bass_sub_shelf
    : bass_lomid_dip;

output_buffer = _
    : tone_stack
    : *(stock_output_trim)
    : fi.highpass(1, 30)
    : fi.high_shelf(enhanced_fizz_trim_db + bass_fizz_trim_db, bass_fizz_shelf_hz)
    : fi.peak_eq_cq(mod_hf_im_notch_db, mod_hf_im_notch_hz, mod_hf_im_notch_q)
    : fi.lowpass(2, output_lp_hz)
    : fi.lowpass(2, output_lp_hz * (1.0 - 0.10 * voicing_blend))
    : bass_air_cut
    : *(output_gain)
    : opamp_slew
    : dynamic_sag(0.5 * power_sag, output_sag_floor)
    : centered_soft_clip(1.12, 0.64 + 0.09 * grunge_drive + 0.12 * voicing_blend + 0.04 * voicing_blend * enhanced_edge_t, 0.0)
    : mod_limiter
    : mod_noise_gate
    : mod_makeup_curve
    : bass_lookahead_limiter
    : bass_subsonic_gate
    : bass_makeup_curve;

grunge_process = _
    : input_buffer
    : first_opamp_stage
    : second_opamp_stage
    : third_opamp_stage
    : output_buffer;

//============================================================================
// Main Process
//============================================================================

process = par(i, 2, ba.bypass1(bypass_switch, grunge_process));
process_stereo = process;