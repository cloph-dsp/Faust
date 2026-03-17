import("stdfaust.lib");

// ===============================================
// SideSculpt v2.0: Stereo Enhancement & Spatial Processing
// ===============================================
// 
// Single-knob stereo enhancer focusing on:
// - Intelligent stereo widening with psychoacoustic enhancement
// - Subtle frequency-dependent spatial processing
// - Natural stereo depth with modulated delays
// - Gentle harmonic enhancement for presence
// - Minimal dynamics processing for transparency
//
// Improvements in v2.0:
// - Focus on stereo enhancement over compression
// - Linkwitz-Riley crossovers for better phase coherence
// - Modulated delay for organic stereo movement
// - Frequency-dependent width processing
// - Transparent operation with minimal artifacts
// ===============================================

// ---------- Utils & Safety
db2lin(db) = pow(10.0, db/20.0);
max2(a,b)  = (a + b + abs(a - b)) / 2.0;    // max without conditionals
min2(a,b)  = (a + b - abs(a - b)) / 2.0;    // min without conditionals
clip(lo,hi) = max2(lo) : min2(hi);          // safe clipping
safediv(x,y) = x / max(abs(y), 1e-12);      // division by zero protection

// UI (unified single knob with musical response)
amt = hslider("Amount [unit:0..1]", 0.5, 0, 1, 0.01) : si.smoo : clip(0.0, 1.0);

// Crossovers (adaptive with musical spacing)
fcLow  = 200.0 * pow(2.0, amt);        // 200Hz to 400Hz (1 octave range)
fcHigh = 4000.0 / pow(1.5, amt);       // 4kHz to ~2.7kHz (smoother transition)

// Enhanced filters with better phase coherence
lp4(f) = fi.lowpass(4, f);
hp4(f) = fi.highpass(4, f);
bp4(fl, fh) = hp4(fl) : lp4(fh);       // bandpass for mid band

// Linkwitz-Riley crossover for better phase alignment
lr_lp(f) = fi.lowpass(2, f) : fi.lowpass(2, f);
lr_hp(f) = fi.highpass(2, f) : fi.highpass(2, f);

// Mid/Side matrix (√2 normalizado)
toMS(L,R) = ((L+R)*0.70710678, (L-R)*0.70710678);
toLR(M,S) = ((M+S)*0.70710678, (M-S)*0.70710678);

// Enhanced envelope followers (simplified for stereo enhancement)
simpleEnv(t) = abs : fi.lowpass(1, 1.0/(2*ma.PI*t));

// Simplified parameter mapping focused on stereo enhancement
widthLow  = 1.0;                         // keep bass centered
widthMid  = 1.0 + 1.2*amt;               // moderate mid widening
widthHigh = 1.0 + 2.0*amt;               // stronger high widening

// Gentle presence boost (optional light compression for fullness)
compRatio = 1.0 + 0.5 * amt;             // very gentle 1:1 to 1.5:1
compThresh = db2lin(-30.0);              // fixed gentle threshold

// Stereo enhancement with natural depth
delaySamples = 3 + 15 * amt;             // 0.07ms to 0.4ms delay for width
modDepth = 0.02 * amt;                   // very subtle modulation
modRate = 1.0;                           // fixed 1Hz modulation rate

// Chorus-like modulation for stereo width
lfo = os.osc(modRate) * modDepth;
delayMod = delaySamples * (1.0 + lfo * 0.1);

// ---------- Stereo Enhancement Processing (minimal compression)
sideProcess(S) = S_low * widthLow + S_mid * widthMid * gM + S_high_enh * widthHigh * gH
with {
  // Improved band splitting with Linkwitz-Riley crossovers
  S_low  = S : lr_lp(fcLow);
  S_high = S : lr_hp(fcHigh);
  S_mid  = S : lr_hp(fcLow) : lr_lp(fcHigh);

  // Subtle harmonic enhancement for presence (very gentle)
  satAmount = 0.01 * amt;
  S_high_enh = S_high * (1 - 0.02*amt) + (S_high : ef.cubicnl(satAmount, 0) : *(0.02*amt));

  // Optional very gentle compression for fullness (minimal processing)
  envM = S_mid : simpleEnv(0.050);     // slow envelope for musicality
  envH = S_high : simpleEnv(0.020);    // faster for highs
  
  // Very gentle gain reduction (barely perceptible)
  overM = (envM + 1e-12) / compThresh;
  overH = (envH + 1e-12) / compThresh;
  
  gM = select2(overM > 1.0 * (1 + amt), 
    pow(overM, (1.0/compRatio) - 1.0), 1.0);  // only compress at higher amounts
    
  gH = select2(overH > 1.0 * (1 + amt), 
    pow(overH, (1.0/compRatio) - 1.0), 1.0);  // only compress at higher amounts
};

// ---------- Clean stereo processing chain
process = _,_
  : (fi.dcblocker, fi.dcblocker)         // DC blocking for safety
  : toMS
  : ( _, de.delay(882, delayMod) )       // short delay buffer (20ms max)
  : ( _, sideProcess )                   // Mid clean; Side stereo enhanced
  : toLR;