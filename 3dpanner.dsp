import("stdfaust.lib");

//==========================================================================
// 1. USER CONTROLS (smoothed)
//==========================================================================
az      = hslider("Azimuth [deg]",   0.0, -90.0,  90.0, 0.1) : si.smoo;
el      = hslider("Elevation [deg]", 0.0, -45.0,  90.0, 0.1) : si.smoo;
dist    = hslider("Distance [m]",    1.0,  0.2,  15.0, 0.1) : si.smoo;
room    = hslider("RoomSize",        0.6,  0.1,  0.99, 0.01) : si.smoo;
mix     = hslider("DryWet",          0.5,  0.0,  1.0, 0.01) : si.smoo;
revWet  = hslider("ReverbWet",       0.5,  0.0,  1.0, 0.01) : si.smoo; // new

//==========================================================================
// 2. CONSTANTS & HELPERS
//==========================================================================
PI         = ma.PI;
FDELAYBUF  = 4096;
MAX_ITD    = ma.SR * 0.0008;

deg2rad(d) = d * PI / 180.0;
clamp(x,a,b) = min(max(x,a),b);

//==========================================================================
// 3. ENHANCED PHYSICAL CUES
//==========================================================================

// Multi-stage frequency-dependent air absorption (ISO 9613-1 inspired)
airAbs(d) = fi.lowpass(1, 20000.0 / (1.0 + d * 0.15)) : 
            fi.lowpass(1, 20000.0 / (1.0 + d * 0.25)) :
            fi.lowpass(1, 20000.0 / (1.0 + d * 0.35));

// Enhanced head shadow approximation (two-stage)
headSh(p) = fi.lowpass(1, 20000.0 - abs(p) * 14000.0) : 
            fi.lowpass(1, 18000.0 - abs(p) * 12000.0);

// Improved elevation HRTF (adds more perceptual realism)
elevHRTF(e) = fi.peak_eq_cq(-12.0, 8000.0 + e * 60.0, 5.0) : 
               fi.peak_eq_cq(-6.0, 6000.0 + e * 40.0, 3.0);

// Natural stereo reverb using Freeverb (adjusted parameters)
simpleReverb = re.stereo_freeverb(room, 0.5, 0.8, 0.0);

// Distance attenuation (inverse square law)
distAtt(d) = 1.0 / sqrt(1.0 + d * d * 0.5);

// Distance-based reverb factor (reduced intensity)
revFac(d) = clamp(0.02 + 0.5 * (1.0 - exp(-d / 3.0)), 0.0, 0.5);

// Simple pinna notch filter for elevation cues (refined notch)
notch(x, freq, Q) = x - fi.resonbp(freq, Q)(x);
pinnaNotch(e, x) = notch(x, 7500.0 + e * 70.0, 0.8);

// Precedence effect (Haas effect): add a short delayed copy to reinforce direction
precedence(in, ms) = in + (in : @(int(ma.SR * ms / 1000.0)) * 0.5);

// Elevation gain (smoother and more perceptually accurate)
elevGain(e) = sqrt(0.5 + 0.5 * cos(deg2rad(e * 0.8)));

// Equal-power panning with elevation
panPos(a) = sin(deg2rad(a));
gainL(a, e) = sqrt(cos((panPos(a) + 1.0) * PI / 4.0)) * elevGain(e);
gainR(a, e) = sqrt(sin((panPos(a) + 1.0) * PI / 4.0)) * elevGain(e);

//==========================================================================
// 4. ADVANCED PANNING & LOCALIZATION (ITD, ILD, PINNA, PRECEDENCE)
//==========================================================================

// Woodworth's formula for ITD (head radius ~8.75cm)
headRadius = 0.0875;
speedOfSound = 343.0;
woodworthITD(a) = (headRadius / speedOfSound) * select2(a >= 0, deg2rad(a) - sin(deg2rad(a)), deg2rad(a) + sin(deg2rad(a)));
delayL(a) = MAX_ITD * (0.5 + 0.5 * clamp(woodworthITD(a) / 0.0008, -1, 1));
delayR(a) = MAX_ITD * (0.5 - 0.5 * clamp(woodworthITD(a) / 0.0008, -1, 1));

// Frequency-dependent ILD (shadowing increases with frequency)
ildFreq(f, a) = 1.0 - 0.4 * abs(sin(deg2rad(a))) * min(f/8000.0, 1.0);
ildL(a) = fi.lowpass(1, 4000.0) : *(ildFreq(4000, a));
ildR(a) = fi.lowpass(1, 4000.0) : *(ildFreq(4000, -a));

// Simple pinna notch filter for elevation cues (notch moves with elevation)
notch(x, freq, Q) = x - fi.resonbp(freq, Q)(x);
pinnaNotch(e, x) = notch(x, 8000.0 + e * 60.0, 0.7);

// Precedence effect (Haas effect): add a short delayed copy to reinforce direction
precedence(in, ms) = in + (in : @(int(ma.SR * ms / 1000.0)) * 0.5);

// Elevation gain (smoother transition)
elevGain(e) = sqrt(0.5 + 0.5 * cos(deg2rad(e)));

// Equal-power panning with elevation
panPos(a) = sin(deg2rad(a));
gainL(a, e) = sqrt(cos((panPos(a) + 1.0) * PI / 4.0)) * elevGain(e);
gainR(a, e) = sqrt(sin((panPos(a) + 1.0) * PI / 4.0)) * elevGain(e);

//==========================================================================
// 5. SPATIALIZER (ADVANCED)
//==========================================================================
spatializer(inL, inR, a, e, d) = outL, outR
with {
    mono   = (inL + inR) * 0.5;
    hrtf   = mono : elevHRTF(e);
    hrtf_pinna = pinnaNotch(e, hrtf);
    dAir   = hrtf_pinna : airAbs(d);
    p      = panPos(a);

    // Direct sound with advanced cues
    dL0    = dAir : de.fdelay(FDELAYBUF, delayL(a)) : headSh(p) : ildL(a) * gainL(a, e);
    dR0    = dAir : de.fdelay(FDELAYBUF, delayR(a)) : headSh(-p) : ildR(a) * gainR(a, e);
    // Precedence effect (short delay, 1.1ms)
    dL     = precedence(dL0, 1.1);
    dR     = precedence(dR0, 1.1);

    // Spatialized reverb with decorrelation
    // Apply elevation filtering to reverb for more perceptual realism
    revInput = pinnaNotch(e, hrtf); // elevation cues for reverb
    revSig = revInput, revInput : simpleReverb;
    revSigL = revSig : select2(0);
    revSigR = revSig : select2(1);
    // Optionally, modulate reverb wetness with elevation (higher elevation, less reverb)
    elevRevWet = revWet * (0.8 - 0.4 * (clamp(e, -45, 90) / 90.0));
    revL   = revSigL : de.fdelay(FDELAYBUF, delayL(a) * 0.7) : headSh(p * 0.6) * gainL(a, e);
    revR   = revSigR : de.fdelay(FDELAYBUF, delayR(a) * 0.7 + 127) : headSh(-p * 0.6) * gainR(a, e);

    att    = distAtt(d);
    fac    = revFac(d);

    // Always binaural output (for headphones)
    outL   = (dL + revL * fac * elevRevWet) * att;
    outR   = (dR + revR * fac * elevRevWet) * att;
};

//==========================================================================
// 6. FINAL MIX (equal-power crossfade, binaural option)
//==========================================================================
process(inL, inR) = outL, outR
with {
    wet   = spatializer(inL, inR, az, el, dist);
    wetL  = wet : select2(0);
    wetR  = wet : select2(1);
    // Equal-power dry/wet mix
    dryGain = sqrt(1.0 - mix);
    wetGain = sqrt(mix);
    dry = (inL + inR) * 0.5;
    outL  = dry * dryGain + wetL * wetGain;
    outR  = dry * dryGain + wetR * wetGain;
};