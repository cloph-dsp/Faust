/* ------------------------------------------------------------
name: "Freeze95"
version: "1.1"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -i -light -nvi -ec -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2
------------------------------------------------------------ */

#ifndef  __Freeze95DSP_H__
#define  __Freeze95DSP_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Freeze95DSP
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float Freeze95DSP_faustpower2_f(float value) {
	return value * value;
}

class Freeze95DSP final : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	int iVec0[2];
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fVec1[2];
	float fConst2;
	float fSlow1;
	float fRec2[2];
	int iRec5[2];
	int iVec2[2];
	FAUSTFLOAT fHslider1;
	float fSlow2;
	float fVec3[2];
	int iVec4[2];
	int iRec6[2];
	float fRec4[2];
	FAUSTFLOAT fCheckbox0;
	float fSlow3;
	float fRec7[2];
	float fConst3;
	int iRec9[2];
	float fRec8[2];
	int iRec11[2];
	float fRec10[2];
	float fConst4;
	int iRec14[2];
	float fRec13[2];
	float fConst5;
	float fRec12[2];
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fRec16[3];
	float fConst20;
	float fRec15[3];
	float fConst21;
	float fRec18[3];
	float fRec17[3];
	float fSlow4;
	float fRec20[2];
	int iRec23[2];
	float fConst22;
	int IOTA0;
	float fVec5[262144];
	float fRec26[2];
	float fSlow5;
	float fSlow6;
	float fSlow7;
	int iSlow8;
	float fSlow9;
	float fSlow10;
	int iRec28[2];
	float fRec27[2];
	int iRec29[2];
	int iRec30[2];
	float fConst23;
	float fRec31[2];
	int iVec6[2];
	float fConst24;
	float fRec32[2];
	float fConst25;
	float fSlow11;
	int iRec36[2];
	float fRec35[2];
	float fRec34[2];
	int iConst26;
	FAUSTFLOAT fCheckbox1;
	int iSlow12;
	float fRec38[2];
	float fRec37[2];
	float fRec39[2];
	float fSlow13;
	float fSlow14;
	FAUSTFLOAT fHslider2;
	float fSlow15;
	float fSlow16;
	float fSlow17;
	float fSlow18;
	float fSlow19;
	float fSlow20;
	float fSlow21;
	float fSlow22;
	float fSlow23;
	float fSlow24;
	float fSlow25;
	float fSlow26;
	float fSlow27;
	float fRec33[2];
	float fConst27;
	float fSlow28;
	float fSlow29;
	int iRec40[2];
	int iSlow30;
	float fRec41[2];
	float fRec42[2];
	float fRec43[2];
	float fRec44[2];
	int iRec47[2];
	float fRec46[2];
	float fRec45[2];
	int iRec50[2];
	float fRec49[2];
	float fRec48[2];
	float fRec25[3];
	float fRec24[3];
	float fRec22[2];
	float fVec7[2];
	float fRec21[2];
	int iRec53[2];
	float fRec56[2];
	float fVec8[2];
	float fRec55[2];
	float fRec54[2];
	float fRec52[2];
	float fRec51[2];
	float fRec57[3];
	float fVec9[2];
	float fRec19[2];
	float fConst28;
	float fConst29;
	int iRec60[2];
	float fRec59[2];
	float fVec10[2];
	float fRec58[2];
	int iRec63[2];
	float fRec66[2];
	float fVec11[2];
	float fRec65[2];
	float fRec64[2];
	float fRec62[2];
	float fRec61[2];
	float fRec67[3];
	float fConst30;
	float fConst31;
	int iRec70[2];
	float fRec69[2];
	float fVec12[2];
	float fRec68[2];
	int iRec73[2];
	float fRec76[2];
	float fVec13[2];
	float fRec75[2];
	float fRec74[2];
	float fRec72[2];
	float fRec71[2];
	float fRec77[3];
	float fVec14[2048];
	float fVec15[2048];
	float fVec16[4096];
	float fVec17[4096];
	float fVec18[512];
	int iConst32;
	float fRec3[2];
	float fVec19[2];
	float fRec1[2];
	float fRec0[2];
	float fVec20[512];
	float fRec80[2];
	float fVec21[2];
	float fRec79[2];
	float fRec78[2];
	
 public:
	Freeze95DSP() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/peakhold:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -i -light -nvi -ec -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("description", "Band-split quantum stutter/freeze with host sync, layered special modes, and the newer lo-fi control behavior.");
		m->declare("filename", "Freeze95DSP.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "Freeze95");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.5.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "1.1");
	}

	static constexpr int getStaticNumInputs() {
		return 2;
	}
	static constexpr int getStaticNumOutputs() {
		return 2;
	}
	int getNumInputs() {
		return 2;
	}
	int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::exp(-1.0f / std::max<float>(1.0f, fConst0 * 8.0f / 1e+03f));
		fConst2 = 1.0f - fConst1;
		fConst3 = fConst0 * 0.0025f;
		fConst4 = std::exp(-1.0f / std::max<float>(1.0f, fConst0 * 6e+02f / 1e+03f));
		fConst5 = 1.0f - fConst4;
		fConst6 = 1.0f / std::tan(6911.504f / fConst0);
		fConst7 = fConst6 * fConst6;
		fConst8 = 1.4142135f * fConst6;
		fConst9 = 1.0f + fConst8 + fConst7;
		fConst10 = fConst7 / fConst9;
		fConst11 = (1.0f - fConst8 + fConst7) / fConst9;
		fConst12 = 2.0f * (1.0f - fConst7) / fConst9;
		fConst13 = 1.0f / std::tan(691.1504f / fConst0);
		fConst14 = fConst13 * fConst13;
		fConst15 = 1.4142135f * fConst13;
		fConst16 = 1.0f + fConst15 + fConst14;
		fConst17 = fConst14 / fConst16;
		fConst18 = (1.0f - fConst15 + fConst14) / fConst16;
		fConst19 = 2.0f * (1.0f - fConst14) / fConst16;
		fConst20 = -2.0f * fConst14 / fConst16;
		fConst21 = -2.0f * fConst7 / fConst9;
		fConst22 = 2e+01f * fConst0 / 1e+03f;
		fConst23 = 5.0f * fConst0 / 1e+03f;
		fConst24 = std::exp(-1.0f / std::max<float>(1.0f, 1.4e+02f * fConst0 / 1e+03f));
		fConst25 = 1.0f / fConst0;
		iConst26 = std::max<int>(256, int(fConst0 * 1.25f));
		fConst27 = float(std::max<int>(32, int(fConst0 * 0.0015f)));
		fConst28 = 1.0f / fConst9;
		fConst29 = 2.0f / fConst9;
		fConst30 = 1.0f / fConst16;
		fConst31 = 2.0f / fConst16;
		iConst32 = std::min<int>(4096, std::max<int>(0, std::max<int>(1, int(fConst0 * 1.5f / 1e+03f))));
	}
	
	void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.55f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.2e+02f);
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec5[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iVec2[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iVec4[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec6[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec9[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec11[l12] = 0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			iRec14[l14] = 0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec12[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec20[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			iRec23[l22] = 0;
		}
		IOTA0 = 0;
		for (int l23 = 0; l23 < 262144; l23 = l23 + 1) {
			fVec5[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec26[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			iRec28[l25] = 0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec27[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec29[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iRec30[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec31[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			iVec6[l30] = 0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec32[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			iRec36[l32] = 0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec35[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec34[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec38[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec37[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec39[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec33[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			iRec40[l39] = 0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec41[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec42[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec43[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec44[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			iRec47[l44] = 0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec46[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec45[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			iRec50[l47] = 0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec49[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec48[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec25[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec24[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec22[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fVec7[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec21[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			iRec53[l55] = 0;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec56[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec8[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec55[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec54[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec52[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec51[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec57[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fVec9[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec19[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			iRec60[l65] = 0;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec59[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fVec10[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec58[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			iRec63[l69] = 0;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec66[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fVec11[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec65[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec64[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec62[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec61[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec67[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			iRec70[l77] = 0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec69[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fVec12[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec68[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			iRec73[l81] = 0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec76[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fVec13[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec75[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec74[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec72[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec71[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fRec77[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2048; l89 = l89 + 1) {
			fVec14[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2048; l90 = l90 + 1) {
			fVec15[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 4096; l91 = l91 + 1) {
			fVec16[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 4096; l92 = l92 + 1) {
			fVec17[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 512; l93 = l93 + 1) {
			fVec18[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec3[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fVec19[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec1[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec0[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 512; l98 = l98 + 1) {
			fVec20[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec80[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fVec21[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec79[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec78[l102] = 0.0f;
		}
	}
	
	void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	Freeze95DSP* clone() {
		return new Freeze95DSP();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Freeze95");
		ui_interface->declare(&fCheckbox0, "tooltip", "Power on/off");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Chaos", &fHslider0, FAUSTFLOAT(0.55f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Lo-Fi", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fCheckbox1, "tooltip", "Quantize stutter to host BPM");
		ui_interface->addCheckButton("Sync BPM", &fCheckbox1);
		ui_interface->addHorizontalSlider("transport/bpm", &fHslider2, FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(3e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = float(fHslider0);
		fSlow1 = fConst2 * fSlow0;
		fSlow2 = float(fHslider1);
		fSlow3 = fConst2 * (1.0f - float(fCheckbox0));
		fSlow4 = fConst2 * fSlow2;
		fSlow5 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - fSlow0 / 0.12f));
		fSlow6 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - fSlow0 / 0.2f));
		fSlow7 = std::max<float>(fSlow5, fSlow6);
		iSlow8 = fSlow7 > 0.05f;
		fSlow9 = 1.0f - 0.9f * fSlow6;
		fSlow10 = 1.0f - 0.9f * fSlow5;
		fSlow11 = 1.0f + 0.22f * fSlow6;
		iSlow12 = float(fCheckbox1) > 0.0f;
		fSlow13 = 0.7f * fSlow6;
		fSlow14 = 1.0f - fSlow6;
		fSlow15 = std::max<float>(2e+01f, float(fHslider2)) / 6e+01f;
		fSlow16 = fSlow15 * 0.5f;
		fSlow17 = fSlow15 * 0.25f;
		fSlow18 = (fSlow17 + fSlow16) * 0.5f;
		fSlow19 = (fSlow16 + fSlow15) * 0.5f;
		fSlow20 = fSlow15 * 2.0f;
		fSlow21 = (fSlow15 + fSlow20) * 0.5f;
		fSlow22 = fSlow15 * 3.0f;
		fSlow23 = (fSlow20 + fSlow22) * 0.5f;
		fSlow24 = fSlow15 * 4.0f;
		fSlow25 = (fSlow22 + fSlow24) * 0.5f;
		fSlow26 = fSlow15 * 8.0f;
		fSlow27 = (fSlow24 + fSlow26) * 0.5f;
		fSlow28 = 1.0f - 0.5f * fSlow6;
		fSlow29 = 1.0f - 0.88f * fSlow6;
		iSlow30 = (fSlow7 > 0.02f) | (fSlow0 < 0.12f);
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fVec1[0] = fSlow0;
			float fTemp0 = fSlow1 + fConst1 * fRec2[1];
			fRec2[0] = ((*reinterpret_cast<int*>(&fTemp0) & 2139095040) ? fTemp0 : 0.0f);
			float fTemp1 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, (fRec2[0] - 0.58f) / 0.42f)), 1.8f);
			float fTemp2 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, (fRec2[0] - 0.18f) / 0.62f)), 1.35f);
			float fTemp3 = std::pow(fRec2[0], 1.1f);
			float fTemp4 = std::min<float>(1.0f, 0.64f * fTemp3 + 0.26f * fTemp2 + 0.1f * fTemp1);
			float fTemp5 = 1.0f / std::tan(6.2831855f * std::max<float>(5.2e+03f, 1.65e+04f - 3.5e+03f * fTemp4) * 0.5f / fConst0);
			float fTemp6 = 1.0f + fTemp5;
			float fTemp7 = 1.0f / fTemp6;
			float fTemp8 = 1.0f - fTemp4;
			float fTemp9 = 0.08f * fTemp8;
			float fTemp10 = std::max<float>(1e-06f, fTemp9);
			float fTemp11 = 0.98f - fTemp9;
			float fTemp12 = std::exp(-1.0f / std::max<float>(1.0f, (8.0f + 12.0f * fTemp8) * fConst0 / 1e+03f));
			float fTemp13 = 0.04f * fTemp8;
			float fTemp14 = 0.94f + fTemp13;
			float fTemp15 = float(input0[i0]);
			float fTemp16 = 0.02f + fRec2[0] * 0.06f;
			iRec5[0] = (iRec5[1] + 1) % std::max<int>(1, int(std::max<float>(6e+01f, 1.2e+04f - fTemp4 * 1.198e+04f) * fConst0 / 1e+03f));
			int iTemp17 = std::fabs(fSlow0 - fVec1[1]) > 0.004f;
			iVec2[0] = iTemp17;
			fVec3[0] = fSlow2;
			int iTemp18 = std::fabs(fSlow2 - fVec3[1]) > 0.004f;
			iVec4[0] = iTemp18;
			int iTemp19 = int(std::max<float>(float(iRec5[0] == 0), std::max<float>(std::max<float>(0.0f, float(iTemp17 - iVec2[1])), std::max<float>(0.0f, float(iTemp18 - iVec4[1])))));
			iRec6[0] = iRec6[1] * 1103515245 + 444;
			float fTemp20 = float((iRec6[0] + 45678) % 2147483647) / 2.1474836e+09f;
			float fTemp21 = ((iTemp19) ? fTemp20 : fRec4[1]);
			fRec4[0] = ((*reinterpret_cast<int*>(&fTemp21) & 2139095040) ? fTemp21 : 0.0f);
			float fTemp22 = fSlow3 + fConst1 * fRec7[1];
			fRec7[0] = ((*reinterpret_cast<int*>(&fTemp22) & 2139095040) ? fTemp22 : 0.0f);
			float fTemp23 = float(std::max<int>(0, std::min<int>(1023, int(fConst3 * fRec7[0] * (1.0f - 0.7f * fTemp8)))));
			int iTemp24 = int(fTemp23 * fRec4[0]);
			iRec9[0] = iRec9[1] * 1103515245 + 222;
			float fTemp25 = float((iRec9[0] + 23456) % 2147483647) / 2.1474836e+09f;
			float fTemp26 = ((iTemp19) ? fTemp25 : fRec8[1]);
			fRec8[0] = ((*reinterpret_cast<int*>(&fTemp26) & 2139095040) ? fTemp26 : 0.0f);
			int iTemp27 = int(fTemp23 * fRec8[0]);
			iRec11[0] = iRec11[1] * 1103515245 + 111;
			float fTemp28 = ((iTemp19) ? float((iRec11[0] + 12345) % 2147483647) / 2.1474836e+09f : fRec10[1]);
			fRec10[0] = ((*reinterpret_cast<int*>(&fTemp28) & 2139095040) ? fTemp28 : 0.0f);
			float fTemp29 = float((fRec10[0] < 0.05f) & (((iTemp27 != iTemp24) | (iTemp27 > 0)) | (iTemp24 > 0)));
			float fTemp30 = fTemp29 * fTemp16;
			float fTemp31 = std::max<float>(0.0f, fRec2[0] - 0.32f) / 0.68f * fRec7[0];
			iRec14[0] = (iRec14[1] + 1) % std::max<int>(1, int((5e+03f + (1.0f - fTemp3) * 1.5e+04f) * fConst0 / 1e+03f));
			int iTemp32 = iRec14[0] == 0;
			float fTemp33 = ((iTemp32) ? fTemp20 : fRec13[1]);
			fRec13[0] = ((*reinterpret_cast<int*>(&fTemp33) & 2139095040) ? fTemp33 : 0.0f);
			float fTemp34 = fConst5 * (fRec13[0] - 0.5f) * 0.9f + fConst4 * fRec12[1];
			fRec12[0] = ((*reinterpret_cast<int*>(&fTemp34) & 2139095040) ? fTemp34 : 0.0f);
			float fTemp35 = std::min<float>(1.0f, 0.58f * fTemp2 + 0.42f * fTemp1);
			float fTemp36 = float(input1[i0]);
			float fTemp37 = fTemp36 - (fRec16[1] * fConst19 + fRec16[2] * fConst18);
			fRec16[0] = ((*reinterpret_cast<int*>(&fTemp37) & 2139095040) ? fTemp37 : 0.0f);
			float fTemp38 = fRec16[0] * fConst17 + fRec16[1] * fConst20 + fRec16[2] * fConst17 - (fRec15[1] * fConst12 + fRec15[2] * fConst11);
			fRec15[0] = ((*reinterpret_cast<int*>(&fTemp38) & 2139095040) ? fTemp38 : 0.0f);
			float fTemp39 = fTemp15 - (fRec18[1] * fConst19 + fRec18[2] * fConst18);
			fRec18[0] = ((*reinterpret_cast<int*>(&fTemp39) & 2139095040) ? fTemp39 : 0.0f);
			float fTemp40 = fRec18[0] * fConst17 + fRec18[1] * fConst20 + fRec18[2] * fConst17 - (fRec17[1] * fConst12 + fRec17[2] * fConst11);
			fRec17[0] = ((*reinterpret_cast<int*>(&fTemp40) & 2139095040) ? fTemp40 : 0.0f);
			float fTemp41 = 0.5f * (fRec17[0] * fConst10 + fRec17[1] * fConst21 + fRec17[2] * fConst10 - (fRec15[0] * fConst10 + fRec15[1] * fConst21 + fRec15[2] * fConst10)) * (0.18f + 0.16f * fTemp35);
			float fTemp42 = fSlow4 + fConst1 * fRec20[1];
			fRec20[0] = ((*reinterpret_cast<int*>(&fTemp42) & 2139095040) ? fTemp42 : 0.0f);
			float fTemp43 = 1.0f / std::tan(6.2831855f * std::max<float>(2.6e+03f, 9.8e+03f - 5.2e+03f * std::pow(fRec20[0], 1.35f) - 1.2e+03f * fTemp4) * 0.5f / fConst0);
			float fTemp44 = 1.0f + fTemp43;
			float fTemp45 = 1.0f / fTemp44;
			float fTemp46 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, fRec20[0] * 0.92f + 0.04f * fTemp2)));
			float fTemp47 = fTemp46 * fTemp46 * (3.0f - 2.0f * fTemp46);
			float fTemp48 = fTemp46 * (0.55f + 0.45f * fTemp47);
			float fTemp49 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, (fTemp46 - 0.38f) / 0.62f)), 1.6f);
			float fTemp50 = std::max<float>(9e+01f, 1.5e+04f - fTemp46 * 1.32e+04f);
			float fTemp51 = 1.0f / std::tan(6.2831855f * fTemp50 * 0.5f / fConst0);
			float fTemp52 = 1.0f + fTemp51;
			float fTemp53 = 1.0f / fTemp52;
			float fTemp54 = 1.0f / std::pow(2.0f, std::max<float>(5.0f, 16.0f - fTemp46 * 1e+01f));
			iRec23[0] = (iRec23[1] + iVec0[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - fTemp46 * 2.16e+04f)));
			int iTemp55 = iRec23[0] == 0;
			float fTemp56 = fTemp15 + fTemp36;
			fVec5[IOTA0 & 262143] = fTemp56;
			float fTemp57 = 0.5f * fTemp56;
			float fTemp58 = std::fabs(fTemp57);
			float fTemp59 = std::max<float>(fRec26[1] * fConst22, fTemp58);
			fRec26[0] = ((*reinterpret_cast<int*>(&fTemp59) & 2139095040) ? fTemp59 : 0.0f);
			float fTemp60 = std::min<float>(1.0f, fRec26[0] / 0.25f);
			iRec28[0] = iRec28[1] * 1103515245 + 555;
			float fTemp61 = ((iTemp19) ? float((iRec28[0] + 56789) % 2147483647) / 2.1474836e+09f : fRec27[1]);
			fRec27[0] = ((*reinterpret_cast<int*>(&fTemp61) & 2139095040) ? fTemp61 : 0.0f);
			int iTemp62 = std::max<int>(1, int((12.0f + fTemp35 * 2.5e+02f) * fConst0 / 1e+03f));
			float fTemp63 = std::min<float>(0.49f, float(std::max<int>(1, int(fConst0 * (2.2f - fTemp4 * 1.6f) / 1e+03f))) / float(iTemp62));
			iRec29[0] = (iRec29[1] + iVec0[1]) % iTemp62;
			float fTemp64 = float(iRec29[0]);
			float fTemp65 = 0.5f - fTemp63;
			float fTemp66 = ((int(((iSlow8) ? 0.0f : float(fRec27[0] < (fTemp35 * 0.75f * (0.35f + 0.65f * fTemp4) * (1.0f + 0.8f * fTemp35) * fSlow10 * fSlow9))))) ? ((fTemp64 < fTemp63) ? 1.0f - fTemp64 / fTemp63 : ((fTemp64 < fTemp65) ? 0.0f : ((fTemp64 < 0.5f) ? (fTemp64 - fTemp65) / fTemp63 : 1.0f))) : 1.0f);
			iRec30[0] = iRec30[1] * 1103515245 + 12345;
			float fTemp67 = float(iRec30[0]) / 2.1474836e+09f;
			float fTemp68 = std::max<float>(fRec31[1] * fConst23, fTemp58);
			fRec31[0] = ((*reinterpret_cast<int*>(&fTemp68) & 2139095040) ? fTemp68 : 0.0f);
			float fTemp69 = std::min<float>(1.0f, fRec31[0] / 0.3f);
			int iTemp70 = fTemp69 > 0.65f;
			iVec6[0] = iTemp70;
			float fTemp71 = fRec32[1] * fConst24 + std::max<float>(0.0f, float(iTemp70 - iVec6[1]));
			fRec32[0] = ((*reinterpret_cast<int*>(&fTemp71) & 2139095040) ? fTemp71 : 0.0f);
			float fTemp72 = std::min<float>(0.22f, fRec32[0] * (0.28f + 0.4f * fTemp69) * (0.6f + 0.4f * fTemp8)) * fRec7[0];
			float fTemp73 = 0.65f * fTemp8;
			int iTemp74 = 1 - iVec0[1];
			iRec36[0] = iRec36[1] * 1103515245 + 333;
			float fTemp75 = float((iRec36[0] + 34567) % 2147483647) / 2.1474836e+09f;
			float fTemp76 = ((iTemp32) ? fTemp75 : fRec35[1]);
			fRec35[0] = ((*reinterpret_cast<int*>(&fTemp76) & 2139095040) ? fTemp76 : 0.0f);
			float fTemp77 = fRec34[1] * 0.982f + (fRec35[0] - 0.5f) * 0.05f;
			fRec34[0] = ((*reinterpret_cast<int*>(&fTemp77) & 2139095040) ? fTemp77 : 0.0f);
			float fTemp78 = 0.15f * (1.0f - fTemp69);
			float fTemp79 = fRec10[0] * (1.0f - fTemp78) + fTemp78 * 0.3f;
			float fTemp80 = 1.0f + 0.25f * fTemp8;
			float fTemp81 = (187.5f - fRec2[0] * 137.5f) * fTemp80;
			int iTemp82 = std::max<int>(1, std::min<int>(int((fTemp81 + ((375.0f - fRec2[0] * 275.0f) * fTemp80 - fTemp81) * fTemp79 * fTemp79) * (1.0f + std::min<float>(0.14f, std::max<float>(-0.14f, fRec34[0])) * (0.26f + 0.18f * fTemp2)) * fSlow11 * 1.02f * fConst0 / 1e+03f), iConst26));
			float fTemp83 = float(iTemp82);
			float fTemp84 = ((iTemp32) ? fTemp25 : fRec38[1]);
			fRec38[0] = ((*reinterpret_cast<int*>(&fTemp84) & 2139095040) ? fTemp84 : 0.0f);
			float fTemp85 = fRec37[1] * 0.982f + (fRec38[0] - 0.5f) * 0.05f;
			fRec37[0] = ((*reinterpret_cast<int*>(&fTemp85) & 2139095040) ? fTemp85 : 0.0f);
			float fTemp86 = std::min<float>(0.14f, std::max<float>(-0.14f, fRec37[0]));
			float fTemp87 = ((iTemp19) ? fTemp75 : fRec39[1]);
			fRec39[0] = ((*reinterpret_cast<int*>(&fTemp87) & 2139095040) ? fTemp87 : 0.0f);
			float fTemp88 = (0.35f + fTemp4 * 2.6f) * fSlow14 + fSlow13;
			float fTemp89 = ((fTemp88 + (18.0f + 1.1e+02f * fTemp4 - fTemp88) * std::pow(fRec39[0], 0.82f)) * (1.0f - 0.58f * fTemp8) + 0.4756f * fTemp8) * (1.0f + fTemp86 * (0.28f + 0.22f * fTemp2));
			float fTemp90 = ((iSlow12) ? ((fTemp89 < fSlow18) ? fSlow17 : ((fTemp89 < fSlow19) ? fSlow16 : ((fTemp89 < fSlow21) ? fSlow15 : ((fTemp89 < fSlow23) ? fSlow20 : ((fTemp89 < fSlow25) ? fSlow22 : ((fTemp89 < fSlow27) ? fSlow24 : fSlow26)))))) : fTemp89);
			float fTemp91 = ((iTemp74) ? 0.0f : fRec33[1] + std::min<float>(5e+02f, fTemp90 / std::max<float>(fConst25, fTemp83 / fConst0)) / fConst0);
			float fTemp92 = fTemp91 - std::floor(fTemp91);
			fRec33[0] = ((*reinterpret_cast<int*>(&fTemp92) & 2139095040) ? fTemp92 : 0.0f);
			float fTemp93 = 1.0f - fRec33[0];
			int iTemp94 = std::max<int>(16, int(fConst27 * (0.6f + 0.8f * fRec8[0])));
			int iTemp95 = std::max<int>(1, iTemp82);
			int iTemp96 = int(fRec33[0] * fTemp83) % iTemp95;
			float fTemp97 = float(iTemp96 % iTemp94) / float(iTemp94);
			float fTemp98 = std::max<float>(0.05f, ((fRec4[0] < 0.33f) ? std::sin(fTemp97 * 3.1415927f) : ((fRec4[0] < 0.66f) ? 1.0f - std::fabs(2.0f * fTemp97 - 1.0f) : std::pow(fTemp97, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp97)))));
			float fTemp99 = std::min<float>(0.9f, fTemp73);
			int iTemp100 = int((fRec27[0] - 0.5f + fTemp86 * 0.22f) * ((0.7f + fTemp4 * 2.0f) * (0.4f + 0.6f * fTemp4) + fTemp4 * 12.0f * (0.35f + 0.65f * fTemp2) * fSlow29) * fSlow28);
			iRec40[0] = (iRec40[1] + iVec0[1]) % iConst26;
			int iTemp101 = iRec40[0] - iTemp82;
			float fTemp102 = 0.5f * fVec5[(IOTA0 - std::min<int>(iConst26, std::max<int>(0, (iTemp101 + int(float((iTemp96 + iTemp100 + iTemp82) % iTemp95) * (1.0f - fTemp99) + float(iTemp82 - 1) * fTemp99) % iTemp95 + iConst26) % iConst26))) & 262143] * fTemp98 * (0.92f + 0.08f * std::pow(std::max<float>(0.0f, fTemp93), 1.8f) * (0.35f + fTemp73));
			float fTemp103 = fTemp4 * fTemp4 * (3.0f - 2.0f * fTemp4);
			float fTemp104 = 0.1f * std::max<float>(0.05f, 0.92f + -0.4f * fTemp103 + fTemp13) + 0.9f * fRec41[1];
			fRec41[0] = ((*reinterpret_cast<int*>(&fTemp104) & 2139095040) ? fTemp104 : 0.0f);
			float fTemp105 = std::min<float>(1.0f, std::max<float>(0.0f, fRec4[0] + (fTemp60 - 0.5f) * 0.1f));
			float fTemp106 = 0.025f + 0.17f * fTemp103;
			float fTemp107 = 0.05f + 0.22f * fTemp103;
			float fTemp108 = std::max<float>(0.02f, 1.0f - fRec41[0]) / std::max<float>(0.001f, fTemp107 + fTemp106 + 0.015f + 0.12f * fTemp103);
			float fTemp109 = 0.1f * fTemp107 * fTemp108 + 0.9f * fRec42[1];
			fRec42[0] = ((*reinterpret_cast<int*>(&fTemp109) & 2139095040) ? fTemp109 : 0.0f);
			float fTemp110 = fRec41[0] + fRec42[0];
			float fTemp111 = 0.1f * fTemp106 * fTemp108 + 0.9f * fRec43[1];
			fRec43[0] = ((*reinterpret_cast<int*>(&fTemp111) & 2139095040) ? fTemp111 : 0.0f);
			int iTemp112 = ((iSlow8) ? 0 : ((fTemp105 < (fRec41[0] - 0.01f)) ? 0 : ((fTemp105 < (fTemp110 - 0.01f)) ? 1 : ((fTemp105 < (fTemp110 + fRec43[0] - 0.01f)) ? 2 : 3))));
			int iTemp113 = iTemp112 == 0;
			float fTemp114 = std::pow(fRec20[0], 1.5f);
			float fTemp115 = ((iTemp74) ? 0.0f : fRec44[1] + std::max<float>(0.35f, fTemp90 * (0.52f + fTemp4) * (1.0f - 0.3f * fTemp114)) / fConst0);
			float fTemp116 = fTemp115 - std::floor(fTemp115);
			fRec44[0] = ((*reinterpret_cast<int*>(&fTemp116) & 2139095040) ? fTemp116 : 0.0f);
			float fTemp117 = 1.0f / std::tan(6.2831855f * std::max<float>(1.4e+03f, 7.6e+03f - 3.8e+03f * fTemp114 - 1.8e+03f * fTemp4) * 0.5f / fConst0);
			float fTemp118 = 1.0f + fTemp117;
			float fTemp119 = 1.0f / fTemp118;
			iRec47[0] = (iRec47[1] + 1) % std::max<int>(1, int(6.0f + 28.0f * fTemp4));
			float fTemp120 = 0.3f + 0.32f * fTemp4;
			float fTemp121 = 0.5f * fVec5[(IOTA0 - std::min<int>(iConst26, std::max<int>(0, (iTemp101 + (int(fTemp93 * fTemp83) % iTemp95 + iTemp100 + iTemp82) % iTemp95 + iConst26) % iConst26))) & 262143] * fTemp98;
			float fTemp122 = ((iRec47[0] == 0) ? fTemp102 * (1.0f - fTemp120) + fTemp121 * fTemp120 : fRec46[1]);
			fRec46[0] = ((*reinterpret_cast<int*>(&fTemp122) & 2139095040) ? fTemp122 : 0.0f);
			float fTemp123 = fRec45[1] * -((1.0f - fTemp117) / fTemp118) + fRec46[0] * fTemp119 + fRec46[1] * fTemp119;
			fRec45[0] = ((*reinterpret_cast<int*>(&fTemp123) & 2139095040) ? fTemp123 : 0.0f);
			float fTemp124 = 1.0f / std::tan(6.2831855f * (4e+02f + fTemp8 * 1.8e+03f) * 0.5f / fConst0);
			float fTemp125 = 1.0f + fTemp124;
			float fTemp126 = 1.0f / fTemp125;
			iRec50[0] = (iRec50[1] + 1) % std::max<int>(1, int(2.0f + fTemp4 * 1e+01f));
			float fTemp127 = ((iRec50[0] == 0) ? fTemp102 : fRec49[1]);
			fRec49[0] = ((*reinterpret_cast<int*>(&fTemp127) & 2139095040) ? fTemp127 : 0.0f);
			float fTemp128 = fRec48[1] * -((1.0f - fTemp124) / fTemp125) + fRec49[0] * fTemp126 + fRec49[1] * fTemp126;
			fRec48[0] = ((*reinterpret_cast<int*>(&fTemp128) & 2139095040) ? fTemp128 : 0.0f);
			float fTemp129 = ((((iSlow30) ? fTemp102 : ((iTemp113) ? fTemp102 : ((iTemp112 == 1) ? fTemp121 : ((iTemp112 == 2) ? fRec48[0] * (0.92f + fTemp9) : fRec45[0] * std::sin(fRec44[0] * 3.1415927f) * 0.82f))) * ((iTemp113) ? 1.0f : std::pow(1e+01f, (-0.5f - fTemp4 * 2.5f) / 2e+01f)) * (1.0f + 0.18f * fTemp35)) * (1.0f - fTemp72) + (fTemp57 * 0.65f + fTemp102 * 0.35f) * fTemp72) * fTemp66 + fTemp67 * 0.003f * fRec2[0] * fRec2[0] * (0.25f + 0.75f * fTemp4) * (1.0f - fTemp66) * (1.0f - std::min<float>(0.8f, fTemp60 * 0.8f))) * 0.98f - (fRec25[1] * fConst19 + fRec25[2] * fConst18);
			fRec25[0] = ((*reinterpret_cast<int*>(&fTemp129) & 2139095040) ? fTemp129 : 0.0f);
			float fTemp130 = fRec25[0] * fConst17 + fRec25[1] * fConst20 + fRec25[2] * fConst17 - (fRec24[1] * fConst12 + fRec24[2] * fConst11);
			fRec24[0] = ((*reinterpret_cast<int*>(&fTemp130) & 2139095040) ? fTemp130 : 0.0f);
			float fTemp131 = std::max<float>(-1.0f, std::min<float>(1.0f, (fRec24[0] * fConst10 + fRec24[1] * fConst21 + fRec24[2] * fConst10) * 0.95f));
			float fTemp132 = 1.0f + fTemp46 * 4.6f;
			float fTemp133 = fTemp131 * fTemp132 / (1.0f + (fTemp132 - 1.0f) * std::fabs(fTemp131));
			float fTemp134 = ((iTemp55) ? fTemp133 : fRec22[1]);
			fRec22[0] = ((*reinterpret_cast<int*>(&fTemp134) & 2139095040) ? fTemp134 : 0.0f);
			float fTemp135 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec22[0] / fTemp54 + 0.5f) * fTemp54));
			fVec7[0] = fTemp135;
			float fTemp136 = (1.0f - fTemp51) / fTemp52;
			float fTemp137 = fRec21[1] * -fTemp136 + fTemp135 * fTemp53 + fVec7[1] * fTemp53;
			fRec21[0] = ((*reinterpret_cast<int*>(&fTemp137) & 2139095040) ? fTemp137 : 0.0f);
			float fTemp138 = fTemp46 * (0.4f + 0.6f * fTemp47);
			int iTemp139 = std::max<int>(1, int(1.0f + fTemp46 * 7.0f));
			float fTemp140 = 1.0f / std::tan(6.2831855f * std::max<float>(1.6e+02f, fTemp50 / (0.95f + 0.5f * float(iTemp139))) * 0.5f / fConst0);
			float fTemp141 = 1.0f + fTemp140;
			float fTemp142 = 1.0f / fTemp141;
			iRec53[0] = (iRec53[1] + 1) % iTemp139;
			float fTemp143 = 1.0f / std::tan(6.2831855f * (3.2e+03f + fTemp46 * 1.4e+03f) * 0.5f / fConst0);
			float fTemp144 = 1.0f + fTemp143;
			float fTemp145 = 0.35f + 0.65f * fTemp46;
			float fTemp146 = fTemp67 - fTemp67;
			float fTemp147 = fTemp146 * fTemp54 * (0.2f + 0.3f * fTemp46);
			float fTemp148 = 1.3f + 2.9f * fTemp46 + 0.75f * fTemp47;
			float fTemp149 = fTemp133 / (1.0f + fTemp148 * std::fabs(fTemp133));
			float fTemp150 = ((iTemp55) ? fTemp149 : fRec56[1]);
			fRec56[0] = ((*reinterpret_cast<int*>(&fTemp150) & 2139095040) ? fTemp150 : 0.0f);
			float fTemp151 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec56[0] + fTemp147) / fTemp54 + 0.5f) * fTemp54));
			float fTemp152 = 1.0f - fTemp145;
			float fTemp153 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp149 + fTemp147) / fTemp54 + 0.5f) * fTemp54));
			float fTemp154 = ((std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp133 + fTemp147) / fTemp54 + 0.5f) * fTemp54)) * fTemp152 + fTemp153 / std::max<float>(0.05f, 1.0f - fTemp148 * std::min<float>(0.999f, std::fabs(fTemp153))) * fTemp145) * (1.0f - fTemp47) + (std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec22[0] + fTemp147) / fTemp54 + 0.5f) * fTemp54)) * fTemp152 + fTemp151 / std::max<float>(0.05f, 1.0f - fTemp148 * std::min<float>(0.999f, std::fabs(fTemp151))) * fTemp145) * fTemp47) * (1.0f + 0.55f * fTemp46);
			fVec8[0] = fTemp154;
			float fTemp155 = fRec55[1] * -fTemp136 + fTemp154 * fTemp53 + fVec8[1] * fTemp53;
			fRec55[0] = ((*reinterpret_cast<int*>(&fTemp155) & 2139095040) ? fTemp155 : 0.0f);
			float fTemp156 = fRec54[1] * -((1.0f - fTemp143) / fTemp144) + fRec55[0] * (fTemp143 / fTemp144) + fRec55[1] * (-fTemp143 / fTemp144);
			fRec54[0] = ((*reinterpret_cast<int*>(&fTemp156) & 2139095040) ? fTemp156 : 0.0f);
			float fTemp157 = fRec55[0] + fRec54[0] * (std::pow(1e+01f, (fTemp46 * 2.2f + (1.0f - std::min<float>(1.0f, fTemp46 * 2.5f)) * 0.35f) / 2e+01f) - 1.0f);
			float fTemp158 = ((iRec53[0] == 0) ? fTemp157 : fRec52[1]);
			fRec52[0] = ((*reinterpret_cast<int*>(&fTemp158) & 2139095040) ? fTemp158 : 0.0f);
			float fTemp159 = fRec51[1] * -((1.0f - fTemp140) / fTemp141) + fRec52[0] * fTemp142 + fRec52[1] * fTemp142;
			fRec51[0] = ((*reinterpret_cast<int*>(&fTemp159) & 2139095040) ? fTemp159 : 0.0f);
			float fTemp160 = 0.1f + 0.55f * fTemp47;
			float fTemp161 = 1.0f / std::tan(6.2831855f * std::max<float>(1.8e+03f, 1.35e+04f - fTemp46 * 8.2e+03f) * 0.5f / fConst0);
			float fTemp162 = fTemp161 * fTemp161;
			float fTemp163 = 1.4142135f * fTemp161;
			float fTemp164 = 1.0f + fTemp163 + fTemp162;
			float fTemp165 = 1.0f / fTemp164;
			float fTemp166 = fTemp157 - (fRec57[1] * (2.0f * (1.0f - fTemp162) / fTemp164) + fRec57[2] * ((1.0f - fTemp163 + fTemp162) / fTemp164));
			fRec57[0] = ((*reinterpret_cast<int*>(&fTemp166) & 2139095040) ? fTemp166 : 0.0f);
			float fTemp167 = fTemp131 * (1.0f - fTemp48) + (((fTemp133 * (1.0f - fTemp46) + (fTemp157 * (1.0f - fTemp160) + (fRec57[0] * fTemp165 + fRec57[1] * (2.0f / fTemp164) + fRec57[2] * fTemp165) * fTemp160) * fTemp46) * (1.0f - fTemp138) + fRec51[0] * fTemp138) * (1.0f - fTemp49) + fRec21[0] * fTemp49) * fTemp48;
			fVec9[0] = fTemp167;
			float fTemp168 = fRec19[1] * -((1.0f - fTemp43) / fTemp44) + fTemp167 * fTemp45 + fVec9[1] * fTemp45;
			fRec19[0] = ((*reinterpret_cast<int*>(&fTemp168) & 2139095040) ? fTemp168 : 0.0f);
			float fTemp169 = 0.5f * (fRec17[0] * fConst28 + fRec17[1] * fConst29 + fRec17[2] * fConst28 - (fRec15[0] * fConst28 + fRec15[1] * fConst29 + fRec15[2] * fConst28)) * (0.18f + 0.16f * fTemp2);
			float fTemp170 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, fRec20[0] * 0.95f + 0.05f * fTemp2)));
			float fTemp171 = fTemp170 * fTemp170 * (3.0f - 2.0f * fTemp170);
			float fTemp172 = fTemp170 * (0.55f + 0.45f * fTemp171);
			float fTemp173 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, (fTemp170 - 0.38f) / 0.62f)), 1.6f);
			float fTemp174 = std::max<float>(9e+01f, 1.5e+04f - fTemp170 * 1.32e+04f);
			float fTemp175 = 1.0f / std::tan(6.2831855f * fTemp174 * 0.5f / fConst0);
			float fTemp176 = 1.0f + fTemp175;
			float fTemp177 = 1.0f / fTemp176;
			float fTemp178 = 1.0f / std::pow(2.0f, std::max<float>(5.0f, 16.0f - fTemp170 * 1e+01f));
			iRec60[0] = (iRec60[1] + iVec0[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - fTemp170 * 2.16e+04f)));
			int iTemp179 = iRec60[0] == 0;
			float fTemp180 = std::max<float>(-1.0f, std::min<float>(1.0f, fRec24[0] * fConst28 + fRec24[1] * fConst29 + fRec24[2] * fConst28));
			float fTemp181 = 1.0f + fTemp170 * 4.6f;
			float fTemp182 = fTemp180 * fTemp181 / (1.0f + (fTemp181 - 1.0f) * std::fabs(fTemp180));
			float fTemp183 = ((iTemp179) ? fTemp182 : fRec59[1]);
			fRec59[0] = ((*reinterpret_cast<int*>(&fTemp183) & 2139095040) ? fTemp183 : 0.0f);
			float fTemp184 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec59[0] / fTemp178 + 0.5f) * fTemp178));
			fVec10[0] = fTemp184;
			float fTemp185 = (1.0f - fTemp175) / fTemp176;
			float fTemp186 = fRec58[1] * -fTemp185 + fTemp184 * fTemp177 + fVec10[1] * fTemp177;
			fRec58[0] = ((*reinterpret_cast<int*>(&fTemp186) & 2139095040) ? fTemp186 : 0.0f);
			float fTemp187 = fTemp170 * (0.4f + 0.6f * fTemp171);
			int iTemp188 = std::max<int>(1, int(1.0f + fTemp170 * 7.0f));
			float fTemp189 = 1.0f / std::tan(6.2831855f * std::max<float>(1.6e+02f, fTemp174 / (0.95f + 0.5f * float(iTemp188))) * 0.5f / fConst0);
			float fTemp190 = 1.0f + fTemp189;
			float fTemp191 = 1.0f / fTemp190;
			iRec63[0] = (iRec63[1] + 1) % iTemp188;
			float fTemp192 = 1.0f / std::tan(6.2831855f * (3.2e+03f + fTemp170 * 1.4e+03f) * 0.5f / fConst0);
			float fTemp193 = 1.0f + fTemp192;
			float fTemp194 = 0.35f + 0.65f * fTemp170;
			float fTemp195 = fTemp146 * fTemp178 * (0.2f + 0.3f * fTemp170);
			float fTemp196 = 1.3f + 2.9f * fTemp170 + 0.75f * fTemp171;
			float fTemp197 = fTemp182 / (1.0f + fTemp196 * std::fabs(fTemp182));
			float fTemp198 = ((iTemp179) ? fTemp197 : fRec66[1]);
			fRec66[0] = ((*reinterpret_cast<int*>(&fTemp198) & 2139095040) ? fTemp198 : 0.0f);
			float fTemp199 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec66[0] + fTemp195) / fTemp178 + 0.5f) * fTemp178));
			float fTemp200 = 1.0f - fTemp194;
			float fTemp201 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp197 + fTemp195) / fTemp178 + 0.5f) * fTemp178));
			float fTemp202 = ((std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp182 + fTemp195) / fTemp178 + 0.5f) * fTemp178)) * fTemp200 + fTemp201 / std::max<float>(0.05f, 1.0f - fTemp196 * std::min<float>(0.999f, std::fabs(fTemp201))) * fTemp194) * (1.0f - fTemp171) + (std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec59[0] + fTemp195) / fTemp178 + 0.5f) * fTemp178)) * fTemp200 + fTemp199 / std::max<float>(0.05f, 1.0f - fTemp196 * std::min<float>(0.999f, std::fabs(fTemp199))) * fTemp194) * fTemp171) * (1.0f + 0.55f * fTemp170);
			fVec11[0] = fTemp202;
			float fTemp203 = fRec65[1] * -fTemp185 + fTemp202 * fTemp177 + fVec11[1] * fTemp177;
			fRec65[0] = ((*reinterpret_cast<int*>(&fTemp203) & 2139095040) ? fTemp203 : 0.0f);
			float fTemp204 = fRec64[1] * -((1.0f - fTemp192) / fTemp193) + fRec65[0] * (fTemp192 / fTemp193) + fRec65[1] * (-fTemp192 / fTemp193);
			fRec64[0] = ((*reinterpret_cast<int*>(&fTemp204) & 2139095040) ? fTemp204 : 0.0f);
			float fTemp205 = fRec65[0] + fRec64[0] * (std::pow(1e+01f, (fTemp170 * 2.2f + (1.0f - std::min<float>(1.0f, fTemp170 * 2.5f)) * 0.35f) / 2e+01f) - 1.0f);
			float fTemp206 = ((iRec63[0] == 0) ? fTemp205 : fRec62[1]);
			fRec62[0] = ((*reinterpret_cast<int*>(&fTemp206) & 2139095040) ? fTemp206 : 0.0f);
			float fTemp207 = fRec61[1] * -((1.0f - fTemp189) / fTemp190) + fRec62[0] * fTemp191 + fRec62[1] * fTemp191;
			fRec61[0] = ((*reinterpret_cast<int*>(&fTemp207) & 2139095040) ? fTemp207 : 0.0f);
			float fTemp208 = 0.1f + 0.55f * fTemp171;
			float fTemp209 = 1.0f / std::tan(6.2831855f * std::max<float>(1.8e+03f, 1.35e+04f - fTemp170 * 8.2e+03f) * 0.5f / fConst0);
			float fTemp210 = fTemp209 * fTemp209;
			float fTemp211 = 1.4142135f * fTemp209;
			float fTemp212 = 1.0f + fTemp211 + fTemp210;
			float fTemp213 = 1.0f / fTemp212;
			float fTemp214 = fTemp205 - (fRec67[1] * (2.0f * (1.0f - fTemp210) / fTemp212) + fRec67[2] * ((1.0f - fTemp211 + fTemp210) / fTemp212));
			fRec67[0] = ((*reinterpret_cast<int*>(&fTemp214) & 2139095040) ? fTemp214 : 0.0f);
			float fTemp215 = fTemp180 * (1.0f - fTemp172) + (((fTemp182 * (1.0f - fTemp170) + (fTemp205 * (1.0f - fTemp208) + (fRec67[0] * fTemp213 + fRec67[1] * (2.0f / fTemp212) + fRec67[2] * fTemp213) * fTemp208) * fTemp170) * (1.0f - fTemp187) + fRec61[0] * fTemp187) * (1.0f - fTemp173) + fRec58[0] * fTemp173) * fTemp172;
			float fTemp216 = 0.5f * (fRec18[0] * fConst30 + fRec18[1] * fConst31 + fRec18[2] * fConst30 - (fRec16[0] * fConst30 + fRec16[1] * fConst31 + fRec16[2] * fConst30)) * (0.06f + 0.08f * fTemp3);
			float fTemp217 = std::min<float>(1.0f, std::max<float>(0.0f, fRec20[0] * 0.35f));
			float fTemp218 = fTemp217 * fTemp217 * (3.0f - 2.0f * fTemp217);
			float fTemp219 = fTemp217 * (0.55f + 0.45f * fTemp218);
			float fTemp220 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, (fTemp217 - 0.38f) / 0.62f)), 1.6f);
			float fTemp221 = std::max<float>(9e+01f, 1.5e+04f - fTemp217 * 1.32e+04f);
			float fTemp222 = 1.0f / std::tan(6.2831855f * fTemp221 * 0.5f / fConst0);
			float fTemp223 = 1.0f + fTemp222;
			float fTemp224 = 1.0f / fTemp223;
			float fTemp225 = 1.0f / std::pow(2.0f, std::max<float>(5.0f, 16.0f - fTemp217 * 1e+01f));
			iRec70[0] = (iRec70[1] + iVec0[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - fTemp217 * 2.16e+04f)));
			int iTemp226 = iRec70[0] == 0;
			float fTemp227 = std::max<float>(-1.0f, std::min<float>(1.0f, (fRec25[0] * fConst30 + fRec25[1] * fConst31 + fRec25[2] * fConst30) * 0.98f));
			float fTemp228 = 1.0f + fTemp217 * 4.6f;
			float fTemp229 = fTemp227 * fTemp228 / (1.0f + (fTemp228 - 1.0f) * std::fabs(fTemp227));
			float fTemp230 = ((iTemp226) ? fTemp229 : fRec69[1]);
			fRec69[0] = ((*reinterpret_cast<int*>(&fTemp230) & 2139095040) ? fTemp230 : 0.0f);
			float fTemp231 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec69[0] / fTemp225 + 0.5f) * fTemp225));
			fVec12[0] = fTemp231;
			float fTemp232 = (1.0f - fTemp222) / fTemp223;
			float fTemp233 = fRec68[1] * -fTemp232 + fTemp231 * fTemp224 + fVec12[1] * fTemp224;
			fRec68[0] = ((*reinterpret_cast<int*>(&fTemp233) & 2139095040) ? fTemp233 : 0.0f);
			float fTemp234 = fTemp217 * (0.4f + 0.6f * fTemp218);
			int iTemp235 = std::max<int>(1, int(1.0f + fTemp217 * 7.0f));
			float fTemp236 = 1.0f / std::tan(6.2831855f * std::max<float>(1.6e+02f, fTemp221 / (0.95f + 0.5f * float(iTemp235))) * 0.5f / fConst0);
			float fTemp237 = 1.0f + fTemp236;
			float fTemp238 = 1.0f / fTemp237;
			iRec73[0] = (iRec73[1] + 1) % iTemp235;
			float fTemp239 = 1.0f / std::tan(6.2831855f * (3.2e+03f + fTemp217 * 1.4e+03f) * 0.5f / fConst0);
			float fTemp240 = 1.0f + fTemp239;
			float fTemp241 = 0.35f + 0.65f * fTemp217;
			float fTemp242 = fTemp146 * fTemp225 * (0.2f + 0.3f * fTemp217);
			float fTemp243 = 1.3f + 2.9f * fTemp217 + 0.75f * fTemp218;
			float fTemp244 = fTemp229 / (1.0f + fTemp243 * std::fabs(fTemp229));
			float fTemp245 = ((iTemp226) ? fTemp244 : fRec76[1]);
			fRec76[0] = ((*reinterpret_cast<int*>(&fTemp245) & 2139095040) ? fTemp245 : 0.0f);
			float fTemp246 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec76[0] + fTemp242) / fTemp225 + 0.5f) * fTemp225));
			float fTemp247 = 1.0f - fTemp241;
			float fTemp248 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp244 + fTemp242) / fTemp225 + 0.5f) * fTemp225));
			float fTemp249 = ((std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fTemp229 + fTemp242) / fTemp225 + 0.5f) * fTemp225)) * fTemp247 + fTemp248 / std::max<float>(0.05f, 1.0f - fTemp243 * std::min<float>(0.999f, std::fabs(fTemp248))) * fTemp241) * (1.0f - fTemp218) + (std::min<float>(1.0f, std::max<float>(-1.0f, std::floor((fRec69[0] + fTemp242) / fTemp225 + 0.5f) * fTemp225)) * fTemp247 + fTemp246 / std::max<float>(0.05f, 1.0f - fTemp243 * std::min<float>(0.999f, std::fabs(fTemp246))) * fTemp241) * fTemp218) * (1.0f + 0.55f * fTemp217);
			fVec13[0] = fTemp249;
			float fTemp250 = fRec75[1] * -fTemp232 + fTemp249 * fTemp224 + fVec13[1] * fTemp224;
			fRec75[0] = ((*reinterpret_cast<int*>(&fTemp250) & 2139095040) ? fTemp250 : 0.0f);
			float fTemp251 = fRec74[1] * -((1.0f - fTemp239) / fTemp240) + fRec75[0] * (fTemp239 / fTemp240) + fRec75[1] * (-fTemp239 / fTemp240);
			fRec74[0] = ((*reinterpret_cast<int*>(&fTemp251) & 2139095040) ? fTemp251 : 0.0f);
			float fTemp252 = fRec75[0] + fRec74[0] * (std::pow(1e+01f, (fTemp217 * 2.2f + (1.0f - std::min<float>(1.0f, fTemp217 * 2.5f)) * 0.35f) / 2e+01f) - 1.0f);
			float fTemp253 = ((iRec73[0] == 0) ? fTemp252 : fRec72[1]);
			fRec72[0] = ((*reinterpret_cast<int*>(&fTemp253) & 2139095040) ? fTemp253 : 0.0f);
			float fTemp254 = fRec71[1] * -((1.0f - fTemp236) / fTemp237) + fRec72[0] * fTemp238 + fRec72[1] * fTemp238;
			fRec71[0] = ((*reinterpret_cast<int*>(&fTemp254) & 2139095040) ? fTemp254 : 0.0f);
			float fTemp255 = 0.1f + 0.55f * fTemp218;
			float fTemp256 = 1.0f / std::tan(6.2831855f * std::max<float>(1.8e+03f, 1.35e+04f - fTemp217 * 8.2e+03f) * 0.5f / fConst0);
			float fTemp257 = fTemp256 * fTemp256;
			float fTemp258 = 1.4142135f * fTemp256;
			float fTemp259 = 1.0f + fTemp258 + fTemp257;
			float fTemp260 = 1.0f / fTemp259;
			float fTemp261 = fTemp252 - (fRec77[1] * (2.0f * (1.0f - fTemp257) / fTemp259) + fRec77[2] * ((1.0f - fTemp258 + fTemp257) / fTemp259));
			fRec77[0] = ((*reinterpret_cast<int*>(&fTemp261) & 2139095040) ? fTemp261 : 0.0f);
			float fTemp262 = fTemp227 * (1.0f - fTemp219) + (((fTemp229 * (1.0f - fTemp217) + (fTemp252 * (1.0f - fTemp255) + (fRec77[0] * fTemp260 + fRec77[1] * (2.0f / fTemp259) + fRec77[2] * fTemp260) * fTemp255) * fTemp217) * (1.0f - fTemp234) + fRec71[0] * fTemp234) * (1.0f - fTemp220) + fRec68[0] * fTemp220) * fTemp219;
			fVec14[IOTA0 & 2047] = fTemp262 + fTemp216 + fTemp215 + fTemp169 + fRec19[0] + fTemp41;
			float fTemp263 = fVec14[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp27))) & 2047];
			float fTemp264 = 1.0f - fTemp30;
			fVec15[IOTA0 & 2047] = fTemp262 - fTemp216 + (fTemp215 - fTemp169) + (fRec19[0] - fTemp41);
			float fTemp265 = fVec15[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp24))) & 2047];
			fVec16[IOTA0 & 4095] = fTemp265 * fTemp264 + fTemp263 * fTemp29 * fTemp16;
			int iTemp266 = std::max<int>(0, int(fConst0 * fTemp31 * (0.4f + 5.0f * fTemp4) * std::fabs(fRec12[0]) / 1e+03f));
			float fTemp267 = fVec16[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec12[0] < 0.0f) ? iTemp266 : 0)))) & 4095];
			fVec17[IOTA0 & 4095] = fTemp263 * fTemp264 + fTemp265 * fTemp29 * fTemp16;
			float fTemp268 = fVec17[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec12[0] >= 0.0f) ? iTemp266 : 0)))) & 4095];
			float fTemp269 = 0.5f * (fTemp268 + fTemp267);
			float fTemp270 = (0.5f * (fTemp268 - fTemp267) + fTemp269 * fRec12[0] * fTemp31 * (0.06f + 0.16f * fTemp4)) * (1.0f - fTemp30 * 1.2f);
			fVec18[IOTA0 & 511] = (fTemp15 + fRec7[0] * (fTemp269 + fTemp270 - fTemp15)) * fTemp14;
			float fTemp271 = fVec18[(IOTA0 - iConst32) & 511];
			float fTemp272 = std::max<float>(fRec3[1] * fTemp12, std::fabs(fTemp271));
			fRec3[0] = ((*reinterpret_cast<int*>(&fTemp272) & 2139095040) ? fTemp272 : 0.0f);
			float fTemp273 = std::min<float>(1.0f, std::max<float>(0.0f, fRec3[0] - fTemp11) / fTemp10);
			float fTemp274 = fTemp271 * (1.0f - fTemp273 + 0.98f / std::max<float>(0.98f, fRec3[0]) * fTemp273);
			float fTemp275 = fTemp274 * (27.0f + fTemp274 * fTemp274) / (27.0f + 9.0f * fTemp274 * fTemp274);
			fVec19[0] = fTemp275;
			float fTemp276 = (1.0f - fTemp5) / fTemp6;
			float fTemp277 = fRec1[1] * -fTemp276 + fTemp275 * fTemp7 + fVec19[1] * fTemp7;
			fRec1[0] = ((*reinterpret_cast<int*>(&fTemp277) & 2139095040) ? fTemp277 : 0.0f);
			float fTemp278 = fRec0[1] * 0.995f + (fRec1[0] - fRec1[1]);
			fRec0[0] = ((*reinterpret_cast<int*>(&fTemp278) & 2139095040) ? fTemp278 : 0.0f);
			output0[i0] = FAUSTFLOAT(fRec0[0]);
			fVec20[IOTA0 & 511] = (fTemp36 + fRec7[0] * (fTemp269 - fTemp270 - fTemp36)) * fTemp14;
			float fTemp279 = fVec20[(IOTA0 - iConst32) & 511];
			float fTemp280 = std::max<float>(fRec80[1] * fTemp12, std::fabs(fTemp279));
			fRec80[0] = ((*reinterpret_cast<int*>(&fTemp280) & 2139095040) ? fTemp280 : 0.0f);
			float fTemp281 = std::min<float>(1.0f, std::max<float>(0.0f, fRec80[0] - fTemp11) / fTemp10);
			float fTemp282 = fTemp279 * (1.0f - fTemp281 + 0.98f / std::max<float>(0.98f, fRec80[0]) * fTemp281);
			float fTemp283 = fTemp282 * (27.0f + fTemp282 * fTemp282) / (27.0f + 9.0f * fTemp282 * fTemp282);
			fVec21[0] = fTemp283;
			float fTemp284 = fRec79[1] * -fTemp276 + fTemp283 * fTemp7 + fVec21[1] * fTemp7;
			fRec79[0] = ((*reinterpret_cast<int*>(&fTemp284) & 2139095040) ? fTemp284 : 0.0f);
			float fTemp285 = fRec78[1] * 0.995f + (fRec79[0] - fRec79[1]);
			fRec78[0] = ((*reinterpret_cast<int*>(&fTemp285) & 2139095040) ? fTemp285 : 0.0f);
			output1[i0] = FAUSTFLOAT(fRec78[0]);
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			fRec2[1] = fRec2[0];
			iRec5[1] = iRec5[0];
			iVec2[1] = iVec2[0];
			fVec3[1] = fVec3[0];
			iVec4[1] = iVec4[0];
			iRec6[1] = iRec6[0];
			fRec4[1] = fRec4[0];
			fRec7[1] = fRec7[0];
			iRec9[1] = iRec9[0];
			fRec8[1] = fRec8[0];
			iRec11[1] = iRec11[0];
			fRec10[1] = fRec10[0];
			iRec14[1] = iRec14[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			iRec23[1] = iRec23[0];
			IOTA0 = IOTA0 + 1;
			fRec26[1] = fRec26[0];
			iRec28[1] = iRec28[0];
			fRec27[1] = fRec27[0];
			iRec29[1] = iRec29[0];
			iRec30[1] = iRec30[0];
			fRec31[1] = fRec31[0];
			iVec6[1] = iVec6[0];
			fRec32[1] = fRec32[0];
			iRec36[1] = iRec36[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec39[1] = fRec39[0];
			fRec33[1] = fRec33[0];
			iRec40[1] = iRec40[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			iRec47[1] = iRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			iRec50[1] = iRec50[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fVec7[1] = fVec7[0];
			fRec21[1] = fRec21[0];
			iRec53[1] = iRec53[0];
			fRec56[1] = fRec56[0];
			fVec8[1] = fVec8[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fVec9[1] = fVec9[0];
			fRec19[1] = fRec19[0];
			iRec60[1] = iRec60[0];
			fRec59[1] = fRec59[0];
			fVec10[1] = fVec10[0];
			fRec58[1] = fRec58[0];
			iRec63[1] = iRec63[0];
			fRec66[1] = fRec66[0];
			fVec11[1] = fVec11[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			iRec70[1] = iRec70[0];
			fRec69[1] = fRec69[0];
			fVec12[1] = fVec12[0];
			fRec68[1] = fRec68[0];
			iRec73[1] = iRec73[0];
			fRec76[1] = fRec76[0];
			fVec13[1] = fVec13[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec3[1] = fRec3[0];
			fVec19[1] = fVec19[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec80[1] = fRec80[0];
			fVec21[1] = fVec21[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
		}
	}

};

#endif
