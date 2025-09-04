import("stdfaust.lib");

// ===============================================
// Side Sculpt: Unified Compression & Stereo Enhancement (1 knob)
// ===============================================

// ---------- Utils
db2lin(db) = pow(10.0, db/20.0);
max2(a,b)  = (a + b + abs(a - b)) / 2.0; // max sem condicionais

// UI (unified single knob)
amt = hslider("Amount [unit:0..1]", 0.5, 0, 1, 0.01) : si.smoo;

// Crossovers (adaptive)
fcLow  = 180.0 + 200.0*amt;   // adaptive low crossover
fcHigh = 6000.0 - 2000.0*amt; // adaptive high crossover

// Filtros (4ª ordem)
lp4(f) = fi.lowpass(4, f);
hp4(f) = fi.highpass(4, f);

// Mid/Side matrix (√2 normalizado)
toMS(L,R) = ((L+R)*0.70710678, (L-R)*0.70710678);
toLR(M,S) = ((M+S)*0.70710678, (M-S)*0.70710678);

// Envelope follower (peak-ish, AR)
arEnv(a, r) = abs : en.ar(a, r) : +(1e-12);

// Mapping do knob
ratioLow  = 1.0 + 40.0*amt;   // 1:1 .. ~41:1 (compressor)
ratioHigh = 1.0 + 20.0*amt;   // 1:1 .. ~21:1 (expander)
thrLow    = db2lin(-30.0 + 30.0*amt);
thrHigh   = db2lin(-42.0 + 30.0*amt);

// Stereo enhancement
delaySamples = 44 + 352 * (1 - amt); // 1ms to 9ms delay for depth

// ---------- Processamento do canal Side
sideProcess(S) = S_low * gL * widthLow + S_mid * widthMid + S_high_enh * gH * widthHigh
with {
  // Split de bandas
  S_low  = S : lp4(fcLow);
  S_high = S : hp4(fcHigh);
  S_mid  = S - S_low - S_high;

  // Harmonic enhancement (psychoacoustic)
  S_high_enh = S_high : ef.cubicnl(0.2 * amt, 0) : *(1 + 0.5*amt) + S_high * (1 - 0.5*amt);

  // Frequency-dependent stereo widening (Ozone-style)
  widthLow  = 1.0;                    // keep lows mono for tight bass
  widthMid  = 1.0 + 2.0*amt;          // moderate widening for mids
  widthHigh = 1.0 + 4.0*amt;          // strong widening for highs

  // Envelopes
  envL = S_low  : arEnv(0.005, 0.100); // attack 5ms, release 100ms
  envH = S_high : arEnv(0.002, 0.040); // attack 2ms, release 40ms

  // Ganhos dinâmicos (branchless)
  overL  = envL / thrLow;
  overH  = envH / thrHigh;
  overLc = max2(1.0, overL);
  overHc = max2(1.0, overH);

  // Compressor (downward) nos graves do Side
  gL = pow(overLc, (1.0/ratioLow) - 1.0);

  // Expander (upward) nos agudos do Side
  gH = pow(overHc, 1.0 - (1.0/ratioHigh));
};

// ---------- Cadeia estéreo completa
process = _,_
  : toMS
  : ( _, de.delay(4410, delaySamples) )  // add depth delay to side
  : ( _, sideProcess )   // Mid clean; Side processed with frequency-dependent widening
  : toLR;