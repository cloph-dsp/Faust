// latency_probe.cpp — measure Grungr impulse-response latency
// ponytail: one-shot standalone, no doctest dep, links only the Faust DSP class.
#include "faust/dsp/dsp.h"
#include "faust/gui/meta.h"
#include "faust/gui/MapUI.h"
#include "../DSP/GrungrFaustDSP.h"

#include <cmath>
#include <cstdio>
#include <vector>

int main() {
    constexpr int SR = 48000;
    constexpr int N  = 4096;          // long enough that all ringing is captured
    constexpr float kThreshold = 0.05f; // 5% of peak — leading-edge of impulse response

    GrungrFaustDSP dsp;
    dsp.init(SR);

    // Use defaults — the same signal chain users actually hear.
    // (Grunge=0 silences the signal; we need a real Grunge to see the impulse.)
    MapUI ui;
    dsp.buildUserInterface(&ui);
    ui.setParamValue("Grunge",       0.7f);
    ui.setParamValue("Butt",         0.62f);
    ui.setParamValue("Face",         0.56f);
    ui.setParamValue("Loud",         0.72f);
    ui.setParamValue("RAW Voicing",  0.0f);
    ui.setParamValue("Bypass",       0.0f);

    // Warm up the filter state with a small DC pulse so ADAA branches settle.
    // Then reset state and start the actual measurement from a clean slate.
    {
        std::vector<float> warmL(64, 0.0f), warmR(64, 0.0f);
        float* wIn[2]  = { warmL.data(), warmR.data() };
        float* wOut[2] = { warmL.data(), warmR.data() };  // reuse, dummy
        dsp.compute(64, wIn, wOut);
    }
    dsp.instanceClear();   // back to clean state, so impulse position is known

    std::vector<float> inL(N, 0.0f), inR(N, 0.0f);
    inL[0] = 0.5f; inR[0] = 0.5f;     // half-amplitude impulse at sample 0
    std::vector<float> outL(N, 0.0f), outR(N, 0.0f);
    float* in[2]  = { inL.data(),  inR.data() };
    float* out[2] = { outL.data(), outR.data() };
    dsp.compute(N, in, out);

    // DEBUG: print first 16 samples
    std::printf("First 16 output L samples: ");
    for (int i = 0; i < 16; i++) std::printf("%.5f ", out[0][i]);
    std::printf("\nFirst 16 output R samples: ");
    for (int i = 0; i < 16; i++) std::printf("%.5f ", out[1][i]);
    std::printf("\nMax abs any sample L=%.5f R=%.5f\n",
                [&]{float m=0; for(int i=0;i<N;i++)m=std::max(m,std::fabs(out[0][i]));return m;}(),
                [&]{float m=0; for(int i=0;i<N;i++)m=std::max(m,std::fabs(out[1][i]));return m;}());

    int peakIdxL = 0, peakIdxR = 0;
    float peakL = 0.0f, peakR = 0.0f;
    for (int i = 0; i < N; i++) {
        float aL = std::fabs(out[0][i]);
        float aR = std::fabs(out[1][i]);
        if (aL > peakL) { peakL = aL; peakIdxL = i; }
        if (aR > peakR) { peakR = aR; peakIdxR = i; }
    }

    int leadL = -1, leadR = -1;
    float tL = kThreshold * peakL, tR = kThreshold * peakR;
    for (int i = 0; i < N; i++) {
        if (leadL < 0 && std::fabs(out[0][i]) > tL) leadL = i;
        if (leadR < 0 && std::fabs(out[1][i]) > tR) leadR = i;
        if (leadL >= 0 && leadR >= 0) break;
    }

    std::printf("Sample rate: %d Hz\n", SR);
    std::printf("Peak L @ %d, mag=%.4f   |   Peak R @ %d, mag=%.4f\n",
                peakIdxL, peakL, peakIdxR, peakR);
    std::printf("Leading-edge L @ %d  |  R @ %d  (threshold %.1f%% of peak)\n",
                leadL, leadR, kThreshold * 100.0f);
    std::printf("Recommended PLUG_LATENCY: %d samples (%.3f ms @ 48k)\n",
                peakIdxL, 1000.0f * peakIdxL / SR);
    return 0;
}
