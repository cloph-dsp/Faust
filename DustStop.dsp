declare name "DustStop";
declare version "5.1";
declare author "CLOPH";
declare description "Tape stop with granular chime disintegration - sound slows to a stop then shatters into bell-like chime particles.";

import("stdfaust.lib");

clamp01(x) = min(1.0, max(0.0, x));
beats2ms(bpm, beats) = (60.0 / bpm) * beats * 1000.0;

trigger_raw = checkbox("Trigger[tooltip: Hold=effect active, Release=immediate dry]");
time_div = hslider("Time[style:menu{'1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8}]", 1, 0.25, 8, 0.01);
dust_raw = hslider("Dust[style:knob]", 0.5, 0, 1, 0.01);
damping_raw = hslider("Damping[style:knob]", 0.5, 0, 1, 0.01);
bpm_host = hslider("transport/bpm", 120, 20, 300, 0.1);

bpm_safe = max(20.0, bpm_host);
dust = dust_raw : si.smoo;
damping = damping_raw : si.smoo;

dur_samps = max(1, int(beats2ms(bpm_safe, time_div) * ma.SR / 1000.0));
tail_samps = int(ma.SR * 3.0);
total_samps = dur_samps + tail_samps;

// === STATE MACHINE ===
// trigger_raw directly (no trig_latch alias — avoids compiler misassignment)
// Checked=active, Unchecked=immediate dry. Re-check = restart.
samp = ba.period(4194304);
trig_edge = trigger_raw > trigger_raw';

trig_time = ba.sAndH(trig_edge, float(samp));
elapsed_off = float(samp) - trig_time;
elapsed = ba.if(elapsed_off < 0.0, elapsed_off + 4194304.0, elapsed_off);
elapsed_i = int(elapsed);

// elapsed_cap: 0 when trigger unchecked, capped at total_samps
elapsed_cap = ba.if(trigger_raw < 0.5, 0,
    ba.if(elapsed_i > total_samps, total_samps, elapsed_i));

prog = clamp01(float(elapsed_cap) / float(dur_samps));
dprog = clamp01(float(elapsed_cap - dur_samps) / float(tail_samps));

// Active flag (nested ba.if to avoid & compiler bug)
active = ba.if(trigger_raw > 0.5,
    ba.if(elapsed_cap < total_samps, 1.0, 0.0),
    0.0);

buf_sz = 262144;

// === AUDIO CHAIN ===
// Highpass output to remove DC, scale 0.95 avoid digital full-scale
// chime_out passes through comb filter; master_env = 0 after completion = silence
chan(x) = fi.highpass(1, 30, chime_out * master_env + sub_sig) * 0.95
with {
    // Three independent noise generators for: position drift, grain modulation, sub
    rnd1 = (+(111) ~ *(1103515245)) : +(12345) : %(2147483647) : /(2147483647.0);
    rnd2 = (+(222) ~ *(1103515245)) : +(23456) : %(2147483647) : /(2147483647.0);
    rnd3 = (+(333) ~ *(1103515245)) : +(34567) : %(2147483647) : /(2147483647.0);

    wr = ba.period(buf_sz);

    // === CLEAN TAPE STOP DELAY ===
    // delay = prog^1.5 * dur_samps * 2/3
    // d(delay)/dn = sqrt(prog) → pitch = 1 - sqrt(prog)
    //   prog=0: pitch=1 (normal), prog=1: pitch=0 (stopped)
    // NEVER reverses because d(delay)/dn <= 1 always
    // min=1 ensures seamless idle→active transition (no read position jump)
    tape_delay = max(1, int(pow(prog, 1.5) * float(dur_samps) * 2.0 / 3.0));

    // === FROZEN TAIL (dust phase) ===
    // Read position freezes at max tape delay during dust tail
    // This gives the ring buffer a stable segment for the comb to resonate on
    tail_delay = int(float(dur_samps) * 2.0 / 3.0);
    drift = int((rnd1 : si.smoo) * float(buf_sz) * 0.02 * dprog * dust);

    total_delay = ba.if(dprog > 0.0, tail_delay, tape_delay);
    active_rd = (wr - total_delay + buf_sz) % buf_sz;
    idle_rd = (wr - 1 + buf_sz) % buf_sz;
    rd = ba.if(active > 0.0, active_rd, idle_rd);

    // x appears EXACTLY ONCE in chan(x)
    buf_out = rwtable(buf_sz, 0.0, (wr, x, rd));

    // === CHIME RESONANCE (DUST TAIL ONLY) ===
    // Feedback comb: exciter : + ~ *(fb) @(delay)
    //   Creates pitched resonance at SR/(delay+1) Hz
    //   @(delay) INSIDE the ~ loop → feedback is per-period, not per-sample
    //   Ring decays as fb^periods → with fb=0.98, ring lasts ~570ms (-60dB)
    //   This is the CORRECT comb topology for a chime-like ring
    // During tape stop (dprog=0): chime_fb = 0 → no feedback, acts as a delay (transparent)
    // During dust tail (dprog>0): chime_fb ramps up → pitched "chime" ring
    //   chime_fb decays during tail so chime naturally fades
    // Noise exciter: tape-stretched buffer has no energy at 604 Hz → comb can't ring
    //   Adding filtered noise provides broadband excitation at chime frequencies
    chime_delay = 73;              // SR/(73+1) ≈ 596 Hz at 44.1kHz (D5 — bell-like pitch)
    chime_noise = (rnd2 : si.smoo) * 2.0 - 1.0;  // bipolar noise for comb excitation
    chime_exciter = buf_out + chime_noise * dprog * dust * 0.5;
    chime_fb = ba.if(dprog > 0.0, 0.98 * clamp01(1.0 - dprog * 0.3), 0.0);
    chime_out = chime_exciter : + ~ *(chime_fb) @(chime_delay);

    // === GRANULAR TEXTURE (dust tail) ===
    // Fast noise amplitude modulation for percussive "grain" feel
    // Faster noise (si.smooth coefficient 0.01 vs si.smoo's 0.001) creates
    // rapid amplitude changes → individual "strikes" on the comb resonance
    // Deeper modulation (0.4-1.0) makes grains more distinct
    grain_fast = si.smooth(0.01, rnd3);  // ~2ms time constant → fast modulation
    grain_mod = ba.if(dprog > 0.0, grain_fast * 0.6 + 0.4 * dust, 1.0);

    // === ENVELOPE ===
    base_fade = 1.0 - prog * 0.3;         // gentle volume drop (only -3dB at end of tape stop)
    darkening = 1.0 - prog * damping * 0.35; // lowpass-like darkening via damping
    dust_fade = clamp01(1.0 - pow(dprog, 1.5)); // smooth fade during dust tail

    effect_env = base_fade * darkening * dust_fade * grain_mod;

    // master_env: 3-way — passthrough / effect / silence
    // trig_latch=0: passthrough (untriggered → immediate dry)
    // trig_latch=1, active=1: effect
    // trig_latch=1, active=0: silence (completed)
    master_env = ba.if(trigger_raw < 0.5, 1.0, ba.if(active > 0.0, effect_env, 0.0));

    // Dark sub-bass oscillator adds weight during tape stop
    sub_sig = os.osc(45.0) * prog * (1.0 - damping * 0.7) * 0.08;
};

process = chan(_), chan(_);
