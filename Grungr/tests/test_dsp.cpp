#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cmath>
#include <vector>

#include "faust/dsp/dsp.h"
#include "faust/gui/meta.h"
#include "faust/gui/MapUI.h"
#include "../DSP/GrungrFaustDSP.h"

TEST_CASE("DSP initializes without error") {
    GrungrFaustDSP dsp;
    CHECK_NOTHROW(dsp.init(44100));
    CHECK(dsp.getNumInputs() == 2);
    CHECK(dsp.getNumOutputs() == 2);
}

TEST_CASE("DSP processes silence without NaN/Inf") {
    GrungrFaustDSP dsp;
    dsp.init(44100);
    
    FAUSTFLOAT inputs[2][64] = {};
    FAUSTFLOAT outputs[2][64] = {};
    FAUSTFLOAT* in[2] = {inputs[0], inputs[1]};
    FAUSTFLOAT* out[2] = {outputs[0], outputs[1]};
    dsp.compute(64, in, out);
    
    for (int i = 0; i < 64; i++) {
        CHECK(std::isfinite(out[0][i]));
        CHECK(std::isfinite(out[1][i]));
    }
}

TEST_CASE("DSP defaults match config.h defaults") {
    GrungrFaustDSP dsp;
    dsp.init(44100);
    MapUI ui;
    dsp.buildUserInterface(&ui);
    
    CHECK(ui.getParamValue("Grunge") == doctest::Approx(0.70f));
    CHECK(ui.getParamValue("Butt") == doctest::Approx(0.62f));
    CHECK(ui.getParamValue("Face") == doctest::Approx(0.56f));
    CHECK(ui.getParamValue("Loud") == doctest::Approx(0.72f));
}

TEST_CASE("Parameter mapping works through MapUI") {
    GrungrFaustDSP dsp;
    dsp.init(44100);
    MapUI ui;
    dsp.buildUserInterface(&ui);

    ui.setParamValue("Grunge", 0.5f);
    CHECK(ui.getParamValue("Grunge") == doctest::Approx(0.5f));

    ui.setParamValue("RAW Voicing", 1.0f);
    CHECK(ui.getParamValue("RAW Voicing") == doctest::Approx(1.0f));

    ui.setParamValue("Bypass", 1.0f);
    CHECK(ui.getParamValue("Bypass") == doctest::Approx(1.0f));
}

TEST_CASE("DSP handles various block sizes without crash") {
    GrungrFaustDSP dsp;
    dsp.init(44100);
    
    int blockSizes[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    for (int i = 0; i < 10; i++) {
        int n = blockSizes[i];
        std::vector<FAUSTFLOAT> inL(n, static_cast<FAUSTFLOAT>(0.5)), inR(n, static_cast<FAUSTFLOAT>(0.5));
        std::vector<FAUSTFLOAT> outL(n, static_cast<FAUSTFLOAT>(0.0)), outR(n, static_cast<FAUSTFLOAT>(0.0));
        FAUSTFLOAT* in[2] = {inL.data(), inR.data()};
        FAUSTFLOAT* out[2] = {outL.data(), outR.data()};
        CHECK_NOTHROW(dsp.compute(n, in, out));
        for (int s = 0; s < n; s++) {
            CHECK(std::isfinite(out[0][s]));
        }
    }
}
