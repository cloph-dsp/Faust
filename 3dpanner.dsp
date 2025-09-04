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
// 3. PHYSICAL CUES & ENHANCED REVERB
//==========================================================================
airAbs(d)      = fi.lowpass(2, 20000.0 / (1.0 + d*0.5));
headSh(p)      = fi.lowpass(1, 20000.0 - abs(p) * 14000.0);
elevHRTF(e)    = fi.peak_eq_cq(-12.0, 8000.0 + e * 50.0, 5.0);

// Enhanced reverb with early reflections and late reverb
earlyRefl(sz) = _ <: 
    @(220) * 0.3,   // ~5ms at 44.1kHz
    @(529) * 0.25,  // ~12ms at 44.1kHz
    @(838) * 0.2,   // ~19ms at 44.1kHz
    @(1147) * 0.15  // ~26ms at 44.1kHz
    :> _;

lateReverb(sz,dmp) = re.stereo_freeverb(sz, dmp, 1.0, 0.0) : select2(0);

// Frequency-dependent damping based on room characteristics
roomDamping(sz) = 0.3 + (1.0 - sz) * 0.4; // smaller rooms = more damping

// Enhanced reverb processor
enhancedRev(sz) = _ <: (earlyRefl(sz), (lateReverb(sz, roomDamping(sz)) * 0.7)) :> _ : 
    fi.lowpass(2, 12000.0 - sz * 4000.0); // room size affects brightness

distAtt(d)     = 1.0 / (1.0 + d*d);
revFac(d)      = clamp(0.05 + 0.55 * (1.0 - exp(-d/2.0)), 0.0, 0.6);

//==========================================================================
// 4. PANNING (ITD + ILD)
//==========================================================================
panPos(a)      = sin(deg2rad(a));
delayL(a)      = 0.5 * MAX_ITD * (panPos(a) + 1.0);
delayR(a)      = 0.5 * MAX_ITD * (1.0 - panPos(a));
elevGain(e)    = 0.5 + 0.5 * cos(deg2rad(e)); // smooth elevation gain (1.0 at 0°, 0.5 at 90°)
gainL(a,e)     = cos((panPos(a) + 1.0) * PI / 4.0) * elevGain(e);
gainR(a,e)     = sin((panPos(a) + 1.0) * PI / 4.0) * elevGain(e);

//==========================================================================
// 5. SPATIALIZER (stereo output)
//==========================================================================
spatializer(inL, inR, a, e, d, sz) = wetL, wetR
with {
    mono   = (inL + inR) * 0.5;
    hrtf   = mono : elevHRTF(e);
    dAir   = hrtf : airAbs(d);
    p      = panPos(a);

    // direct sound
    dL     = dAir : de.fdelay(FDELAYBUF, delayL(a)) : headSh(p) * gainL(a,e);
    dR     = dAir : de.fdelay(FDELAYBUF, delayR(a)) : headSh(-p) * gainR(a,e);

    // enhanced spatialized reverb
    revSig = hrtf : enhancedRev(sz); // use enhanced reverb
    revL   = revSig : de.fdelay(FDELAYBUF, delayL(a)) : headSh(p) * gainL(a,e);
    revR   = revSig : de.fdelay(FDELAYBUF, delayR(a)) : headSh(-p) * gainR(a,e);

    // combine with distance attenuation and user‑controlled wetness
    att    = distAtt(d);
    fac    = revFac(d);
    wetL   = (dL + revL * fac * revWet) * att;
    wetR   = (dR + revR * fac * revWet) * att;
};

//==========================================================================
// 6. FINAL MIX
//==========================================================================
process(inL, inR) = outL, outR
with {
    wet   = spatializer(inL, inR, az, el, dist, room);
    wetL  = wet : select2(0);
    wetR  = wet : select2(1);
    outL  = inL * (1.0 - mix) + wetL * mix;
    outR  = inR * (1.0 - mix) + wetR * mix;
};