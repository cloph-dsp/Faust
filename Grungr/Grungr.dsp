// Grungr
// Report-driven Faust model centered on the stock JFET input, cascaded 4558
// clipping stages, asymmetric final clipping, and the active scooped tone stack.

import("stdfaust.lib");

declare name "Grungr";\ndeclare author "cloph";
declare version "2.7";

//============================================================================
// UI Controls
//============================================================================

grunge_knob = hslider("[1]Grunge[style:knob]", 0.70, 0, 1, 0.01) : si.smoo;
butt_knob = hslider("[2]Butt[style:knob]", 0.62, 0, 1, 0.01) : si.smoo;
face_knob = hslider("[3]Face[style:knob]", 0.56, 0, 1, 0.01) : si.smoo;
loud_knob = hslider("[4]Loud[style:knob]", 0.72, 0, 1, 0.01) : si.smoo;
enhanced_switch = checkbox("[5]Enhanced[style:switch]") : si.smoo;
bypass_switch = checkbox("[6]Bypass[style:switch]");

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
drive_tone_t = clip01((grunge_knob - 0.35) / 0.65);
enhanced_edge_t = clip01((grunge_knob - 0.70) / 0.30);
enhanced_mode = enhanced_switch;
voicing_blend = enhanced_mode * enhanced_drive_t;
stock_blend = 1.0 - voicing_blend;
drive_floor = 0.07 * (1.0 - grunge_drive);
stock_drive_lift = stock_blend * (0.22 * (1.0 - grunge_drive));
enhanced_drive_lift = voicing_blend * (0.10 * (1.0 - grunge_drive) + 0.05 * grunge_edge);
drive_core = clip01(grunge_drive + drive_floor + stock_drive_lift + enhanced_drive_lift);

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
enhanced_tight_t = enhanced_mode * drive_tone_t;
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

// The report calls out the 4558's 1.0 V/us slew rate as a meaningful part of
// the pedal character. A fixed-step ramp is a stable way to emulate that limit.
opamp_slew = ba.ramp(ma.SR / 1000000.0);

stage1_gain_db = 18.0 + 19.0 * drive_core - enhanced_stage_relief_db;
stage2_gain_db = 12.0 + 14.0 * drive_core - 0.65 * enhanced_stage_relief_db;
stage3_gain_db = 6.0 + 6.0 * drive_core + enhanced_late_gain_db;

stage1_clip_threshold = 0.76 - 0.10 * drive_core + enhanced_clip_relief;
stage2_clip_threshold = 0.74 - 0.12 * drive_core + 0.85 * enhanced_clip_relief;
stage2_clip_bias = -0.010 - 0.018 * drive_core + enhanced_bias_shift;
stage3_pos_threshold = 1.10 - 0.16 * drive_core;
stage3_neg_threshold = 0.74 - 0.10 * drive_core;
stage3_led_pos_threshold = 1.62 - 0.12 * drive_core;
stage3_led_neg_threshold = 1.08 - 0.08 * drive_core;
stage3_led_mix = voicing_blend * enhanced_edge_t * (0.08 + 0.34 * grunge_edge);

stage1_lp_hz = 10800.0 - 2000.0 * drive_core - enhanced_stage1_lp_trim_hz;
stage2_hpf_hz = 80.0 + 25.0 * drive_core + 12.0 * voicing_blend + enhanced_stage2_hpf_boost_hz;
stage2_lp_hz = 8400.0 - 1600.0 * drive_core - enhanced_stage2_lp_trim_hz;
stage3_hpf_hz = 110.0 + 20.0 * drive_core + enhanced_stage3_hpf_boost_hz;
stage3_lp_hz = 11200.0 - 1800.0 * drive_core - 900.0 * grunge_edge - enhanced_stage3_lp_trim_hz;
tone_mid_scoop_db = stock_mid_scoop_db + drive_tone_t * (0.95 - 0.55 * grunge_drive);
presence_peak_hz = 1450.0;
presence_peak_q = 0.72;
presence_peak_db = drive_tone_t * face_air * (0.70 - 0.30 * grunge_drive) + enhanced_presence_trim_db;
output_lp_hz = stock_output_lp_hz - drive_tone_t * face_air * (900.0 * grunge_drive + 650.0 * grunge_edge) - enhanced_tight_t * face_air * (350.0 + 850.0 * grunge_edge);
im_notch_db = -3.0 * voicing_blend * enhanced_edge_t;
power_sag = voicing_blend * (0.016 + 0.040 * grunge_drive * grunge_drive);
stage_sag_floor = 0.76 + 0.04 * stock_blend;
output_sag_floor = 0.84 + 0.03 * stock_blend;

low_shelf_db = lininterp(-9.0, 15.0, butt_taper);
high_shelf_db = lininterp(-8.0, 11.0, face_taper);
output_gain = loud_taper * db2lin(0.95 - 1.10 * grunge_edge * grunge_edge + 2.1 * stock_blend * (drive_core - grunge_drive) - 0.55 * voicing_blend * enhanced_edge_t + enhanced_output_makeup_db);

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

input_buffer = _
    : *(instrument_trim)
    : fi.highpass(1, stock_input_hpf_hz + enhanced_input_hpf_hz)
    : fi.lowpass(1, stock_input_lp_hz)
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
    : fi.lowpass(2, stage2_lp_hz - 300.0);

third_opamp_stage = _
    : fi.highpass(1, stage3_hpf_hz)
    : *(db2lin(stage3_gain_db))
    : opamp_slew
    : dynamic_sag(power_sag, stage_sag_floor)
    : hybrid_ground_clip
    : fi.lowpass(2, stage3_lp_hz)
    : fi.dcblocker;

tone_stack = _
    : fi.low_shelf(low_shelf_db, stock_low_hz)
    : fi.high_shelf(high_shelf_db, stock_high_hz)
    : fi.peak_eq_cq(tone_mid_scoop_db, stock_mid_hz, stock_mid_q)
    : fi.peak_eq_cq(presence_peak_db, presence_peak_hz, presence_peak_q)
    : fi.peak_eq_cq(im_notch_db, 55.0, 2.5);

output_buffer = _
    : tone_stack
    : *(stock_output_trim)
    : fi.highpass(1, 30)
    : fi.high_shelf(enhanced_fizz_trim_db, 4200.0)
    : fi.lowpass(2, output_lp_hz)
    : *(output_gain)
    : opamp_slew
    : dynamic_sag(0.5 * power_sag, output_sag_floor)
    : centered_soft_clip(1.12, 0.64 + 0.09 * grunge_drive + 0.12 * voicing_blend + 0.04 * voicing_blend * enhanced_edge_t, 0.0);

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