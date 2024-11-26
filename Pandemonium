import("stdfaust.lib");

declare name "Pandemonium";
declare author "CLOPH";
declare license "BSD";
declare description "A stereo feedback delay with various panning modes";

// Main controls
g = hslider("v:[0]Pan/[0]Left-Right[tooltip:Panning of the original signal and delayed panning.]", 0.5, 0, 1, 0.01) : si.smoo;
fb = hslider("v:[1]Delay/[1]Feedback", 10, 0, 100, 1) / 100 - 0.05 : si.smoo;

// Tempo and sync
synconoff = checkbox("v:[2]Tempo/[2]Sync[tooltip:Delay BPM Synchronization.]");
delaytime = 
(
    hslider("v:[2]Tempo/[0]Rate Time [unit:/s]", 1.75, 0.1, 5, 0.05) * ma.SR,
    ba.tempo(hslider("v:[2]Tempo/[5]BPMs", 120, 50, 300, 1)) / rate
    : select2(synconoff)
);
rate = (ma.np2(hslider("v:[2]Tempo/[6]Rate @ BPMs[unit:]", 1, 1, 11, 1)) :> _) / 4;

// Panning type
pant = nentry("v:[0]Pan/[1]Pan Type[tooltip:Type 0: follows once. Type 1: Follows with ping-pong. Type 2: Reverse follow. Type 3: Reverse ping-pong. Type 4: Randomized.]", 0, 0, 4, 1) : round;

// Smoothing
smooth_pant = pant : si.smooth(ba.tau2pole(0.5));
smooth_selector(x, y, a) = it.interpolate_linear(a, x, y);

// Random panning
randomtimeslider = hslider("v:[0]Pan/[5]Type 3 Time[unit:s]", 0.4, 0.1, 1, 0.01) : si.smoo;
randomtime = nentry("v:[0]Pan/[3]Random Type", 0, 0, 2, 1) : round;

// Filter
lp = hslider("v:[1]Delay/t:[3]Filter/[1]Low-Pass[unit:Hz]", 20000, 100, 20000, 1) : si.smoo;
hp = hslider("v:[1]Delay/t:[3]Filter/[0]High-Pass[unit:Hz]", 20, 20, 5000, 1) : si.smoo;

// Mix
bypass = checkbox("v:[4]Mix/[2]Bypass");
mix = hslider("v:[4]Mix/[0]Dry/Wet", 0.5, 0, 1, 0.01) : si.smoo; 
gain = hslider("v:[4]Mix/[1]Gain", 1, 0, 1.25, 0.01) : si.smoo;

// Panning and delay functions
myselectortype(pant) = *(pant==0), *(pant==1), *(pant==2), *(pant==3), *(pant==4) :> _;
binarywave = os.lf_squarewavepos(ma.SR / delaytime);
pingpong(panning) = binarywave - panning : abs;

noisediv = no.noise : abs : ba.slidingRMSp(50, 200);
myselector3(typeselect) = *(typeselect==0), *(typeselect==1), *(typeselect==2) :> _;
randomsec = randomtimeslider, 
            (delaytime / ma.SR / 4 : abs), 
            (noisediv)
        : myselector3(randomtime);
random = no.noise : abs <: ba.downSample(1 / randomsec);

// Panning and delay functions
panningtype(panning) = 
    smooth_selector(
        *(panning @(delaytime)),
        smooth_selector(
            (+~panning @(delaytime)),
            smooth_selector(
                (+~(1-panning) @(delaytime)),
                smooth_selector(
                    *(pingpong(panning) @(delaytime)),
                    *(random @(delaytime)),
                    smooth_pant - 3
                ),
                smooth_pant - 2
            ),
            smooth_pant - 1
        ),
        smooth_pant
    );

panningmode(panning) = 
    smooth_selector(
        1,
        smooth_selector(
            1*(panning),
            smooth_selector(
                1*(1-panning),
                smooth_selector(
                    (1 * (pingpong(panning))),
                    1,
                    smooth_pant - 3
                ),
                smooth_pant - 2
            ),
            smooth_pant - 1
        ),
        smooth_pant
    );

// Filters
filters = _ <: fi.lowpassLR4(lp) : fi.highpassLR4(hp);

// Main delay function
feeddelay(panning) =
    (_ * panningmode(panning)) @(delaytime) : 
        (+~de.delay(delaytime, delaytime) * fb) 
            <: panningtype(panning)
        : filters :
    _ * mix;

// Main process
process = ba.bypass2(
    bypass, 
    (_,_ <: (
        (dry*(1-g), dry*g, 
         feeddelay(1-g), feeddelay(g)) :> _,_
    )) : (*(gain), *(gain))
)
with {
    dry = *(1-mix);
};
