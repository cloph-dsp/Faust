// sanity_check.cpp — verify DSP produces sane output on real audio
// ponytail: minimal probe, no framework, asserts via printf
// Match plugin config (Grungr.h defines FAUSTFLOAT double before include)
#define FAUSTFLOAT double
#include "faust/dsp/dsp.h"
#include "faust/gui/meta.h"
#include "faust/gui/MapUI.h"
#include "../DSP/GrungrFaustDSP.h"
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstring>

// ponytail: MSVC doesn't define M_PI without _USE_MATH_DEFINES; literal is shorter.
static const double SANITY_PI = 3.14159265358979323846;

static double maxAbs(const double* p, int n) {
    double m = 0.0;
    for (int i = 0; i < n; i++) m = std::max(m, std::fabs(p[i]));
    return m;
}

static void runCase(const char* label, bool bypass, double inputLevel, bool sine) {
    constexpr int SR = 48000;
    constexpr int N = 4096;
    GrungrFaustDSP dsp;
    dsp.init(SR);
    MapUI ui;
    dsp.buildUserInterface(&ui);
    ui.setParamValue("Bypass", bypass ? 1.0f : 0.0f);

    std::vector<double> inL(N), inR(N);
    for (int i = 0; i < N; i++) {
        double v = sine ? (inputLevel * std::sin(2.0 * SANITY_PI * 220.0 * i / SR)) : inputLevel;
        inL[i] = inR[i] = v;
    }
    std::vector<double> outL(N, 0.0), outR(N, 0.0);
    double* in[2]  = { inL.data(), inR.data() };
    double* out[2] = { outL.data(), outR.data() };
    dsp.compute(N, in, out);

    int nanCount = 0;
    for (int i = 0; i < N; i++) {
        if (!std::isfinite(outL[i])) nanCount++;
        if (!std::isfinite(outR[i])) nanCount++;
    }
    std::printf("[%s] bypass=%d %s in=%.3f -> maxAbsL=%.6f maxAbsR=%.6f nanCount=%d\n",
        label, bypass, sine?"sine220":"dc", inputLevel,
        maxAbs(outL.data(), N), maxAbs(outR.data(), N), nanCount);
    std::printf("  L[2000..2008]: ");
    for (int i = 2000; i < 2008; i++) std::printf("%.5f ", outL[i]);
    std::printf("\n");
}

int main() {
    // ponytail: sine 220Hz @ 0.5 is the smallest probe that exercises the
    // nonlinear stages without being killed by DC blockers.
    runCase("WET sine 0.5", false, 0.5, true);
    runCase("DRY sine 0.5", true,  0.5, true);
    runCase("WET sine 0.05",false, 0.05, true);
    runCase("WET dc 0.5",  false, 0.5, false);
    return 0;
}
