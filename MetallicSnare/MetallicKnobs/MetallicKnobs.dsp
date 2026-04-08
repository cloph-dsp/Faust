import("stdfaust.lib");

// --- MACRO INPUTS ---
clonk  = hslider("CLONK", 200, 50, 4000, 0.1) : si.smoo;      // Fundamental Frequency
shatter = hslider("SHATTER", 0, 0, 10, 0.01) : si.smoo;       // FM Index / Brittle Attack
tension = hslider("TENSION", 1, 0.5, 4, 0.01) : si.smoo;      // Inharmonicity factor
vacuum  = hslider("VACUUM", 0, 0, 1, 0.001) : si.smoo;        // Pitch Collapse / Squeak
pang    = hslider("PANG", 0.1, 0.01, 2, 0.01) : si.smoo;      // Waveguide Feedback
grit    = hslider("GRIT", 0, 0, 1, 0.01) : si.smoo;           // Wavefolding/Decimator

gate    = button("gate");

// --- EXCITER: FM PULSE ---
// Quick modulation burst to "strike" the resonators
exciter = pulse_exciter * 0.5
with {
    mod_freq = clonk * 4.5;
    mod_index = shatter * 5000;
    env = en.ar(0.001, 0.005, gate);
    carrier = os.oscsin(clonk + os.oscsin(mod_freq) * mod_index);
    pulse_exciter = carrier * env;
};

// --- MODAL BANK: 8 RESONATORS ---
// Inharmonic ratios derived from industrial metal clangs
modal_bank(f0, t, d) = par(i, 8, resonator(i)) :> _
with {
    ratios(i) = ba.selectn(8, i, 1.0, 1.52, 2.73, 3.14, 4.21, 5.89, 7.21, 8.54);
    // Apply tension to stretch partials
    freq(i) = f0 * ratios(i) * (1 + t * i * 0.1);
    // Apply vacuum sweep: rapid downward pitch shift
    swept_freq(i) = freq(i) * (1.0 - (vacuum * en.ar(0.0, 0.1, gate)));
    
    // Bandpass resonator (Modal)
    resonator(i) = fi.resonbp(swept_freq(i), 50 + (100 * (1-vacuum)), 1.0);
};

// --- POST-PROCESSING: PANG & GRIT ---
// Waveguide-style pang feedback
pang_fx = + ~ (de.delay(10000, clonk : hertz2samples) * (pang * 0.95) : fi.lowpass(1, 15000))
with {
    hertz2samples(h) = (ma.SR / h) : ma.MIN(10000);
};

// Wavefolder grit
grit_fx(x) = x : ba.if(grit > 0, ef.cubicnl(x * (1 + grit * 4), 0.5), x);

process = exciter : modal_bank(clonk, tension, pang) : pang_fx : grit_fx : _ * 0.7;

