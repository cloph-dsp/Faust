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
	
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fVec1[2];
	float fConst2;
	float fSlow1;
	float fRec2[2];
	float fConst3;
	FAUSTFLOAT fCheckbox0;
	float fSlow2;
	float fRec3[2];
	float fConst4;
	int iRec5[2];
	int iVec2[2];
	FAUSTFLOAT fHslider1;
	float fSlow3;
	float fVec3[2];
	int iVec4[2];
	int iRec6[2];
	float fRec4[2];
	int iRec8[2];
	float fRec7[2];
	int iRec10[2];
	float fRec9[2];
	float fSlow4;
	float fRec11[2];
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fRec13[3];
	int IOTA0;
	float fVec5[2097152];
	float fConst13;
	float fConst14;
	float fRec12[2];
	float fSlow5;
	float fSlow6;
	float fSlow7;
	int iSlow8;
	float fSlow9;
	int iRec15[2];
	float fRec14[2];
	int iRec16[2];
	int iRec17[2];
	int iConst15;
	int iRec20[2];
	int iRec21[2];
	float fRec19[2];
	float fRec18[2];
	float fConst16;
	float fRec22[2];
	int iRec24[2];
	float fRec23[2];
	float fSlow10;
	float fConst17;
	float fSlow11;
	int iRec27[2];
	float fRec26[2];
	float fRec25[2];
	int iRec29[2];
	float fRec28[2];
	float fSlow12;
	float fSlow13;
	float fConst18;
	FAUSTFLOAT fCheckbox1;
	int iSlow14;
	float fSlow15;
	float fSlow16;
	float fRec31[2];
	FAUSTFLOAT fHslider2;
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
	float fSlow28;
	float fSlow29;
	float fSlow30;
	float fRec30[2];
	int iRec32[2];
	int iRec34[2];
	float fRec33[2];
	float fRec35[2];
	int iConst19;
	float fConst20;
	int iVec6[2];
	float fConst21;
	float fRec36[2];
	int iSlow31;
	float fRec37[2];
	float fRec38[2];
	float fRec39[2];
	float fRec40[2];
	float fConst22;
	float fRec41[2];
	float fConst23;
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fRec44[3];
	float fConst36;
	float fRec43[3];
	float fVec7[2097152];
	float fRec42[2];
	int iRec47[2];
	float fRec46[2];
	float fRec45[2];
	float fRec48[2];
	int iRec50[2];
	float fRec49[2];
	float fSlow32;
	int iRec53[2];
	float fRec52[2];
	float fRec51[2];
	int iRec55[2];
	float fRec54[2];
	float fRec57[2];
	float fRec56[2];
	int iRec59[2];
	float fRec58[2];
	float fRec60[2];
	int iVec8[2];
	float fRec61[2];
	float fConst37;
	float fRec62[2];
	float fConst38;
	int iRec65[2];
	int iRec69[2];
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec71[3];
	float fVec9[2097152];
	float fRec70[2];
	int iRec74[2];
	float fRec73[2];
	float fRec72[2];
	float fRec75[2];
	int iRec77[2];
	float fRec76[2];
	float fConst45;
	float fSlow33;
	int iRec80[2];
	float fRec79[2];
	float fRec78[2];
	int iRec82[2];
	float fRec81[2];
	float fRec84[2];
	float fRec83[2];
	int iRec86[2];
	float fRec85[2];
	float fRec87[2];
	int iVec10[2];
	float fRec88[2];
	float fConst46;
	float fRec89[2];
	float fRec68[2];
	float fRec90[2];
	float fVec11[2];
	float fRec67[2];
	float fRec66[2];
	float fRec64[2];
	float fRec63[2];
	int iRec93[2];
	int iRec97[2];
	float fRec96[2];
	float fRec98[2];
	float fVec12[2];
	float fRec95[2];
	float fRec94[2];
	float fRec92[2];
	float fRec91[2];
	int iRec101[2];
	int iRec105[2];
	float fRec104[2];
	float fRec106[2];
	float fVec13[2];
	float fRec103[2];
	float fRec102[2];
	float fRec100[2];
	float fRec99[2];
	float fVec14[2048];
	float fRec108[3];
	float fVec15[2097152];
	float fRec107[2];
	float fRec109[2];
	float fRec110[2];
	int iVec16[2];
	float fRec111[2];
	float fRec112[2];
	float fRec115[3];
	float fRec114[3];
	float fVec17[2097152];
	float fRec113[2];
	float fRec116[2];
	float fRec117[2];
	int iVec18[2];
	float fRec118[2];
	float fRec119[2];
	float fRec126[3];
	float fVec19[2097152];
	float fRec125[2];
	float fRec127[2];
	float fRec128[2];
	int iVec20[2];
	float fRec129[2];
	float fRec130[2];
	float fRec124[2];
	float fRec131[2];
	float fVec21[2];
	float fRec123[2];
	float fRec122[2];
	float fRec121[2];
	float fRec120[2];
	float fRec136[2];
	float fRec137[2];
	float fVec22[2];
	float fRec135[2];
	float fRec134[2];
	float fRec133[2];
	float fRec132[2];
	float fRec142[2];
	float fRec143[2];
	float fVec23[2];
	float fRec141[2];
	float fRec140[2];
	float fRec139[2];
	float fRec138[2];
	float fVec24[2048];
	float fVec25[4096];
	float fConst47;
	float fRec145[2];
	float fConst48;
	float fRec144[2];
	float fConst49;
	float fVec26[4096];
	float fVec27[512];
	int iConst50;
	float fRec1[2];
	float fVec28[2];
	float fRec0[2];
	float fVec29[512];
	float fRec147[2];
	float fVec30[2];
	float fRec146[2];
	
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
		fConst1 = std::exp(-(1.0f / std::max<float>(1.0f, 0.008f * fConst0)));
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.001f * fConst0;
		fConst4 = 0.0025f * fConst0;
		fConst5 = 0.02f * fConst0;
		fConst6 = std::tan(4665.265f / fConst0);
		fConst7 = Freeze95DSP_faustpower2_f(fConst6);
		fConst8 = 2.0f * (1.0f - 1.0f / fConst7);
		fConst9 = 1.0f / fConst6;
		fConst10 = (fConst9 + -1.4142135f) / fConst6 + 1.0f;
		fConst11 = (fConst9 + 1.4142135f) / fConst6 + 1.0f;
		fConst12 = 1.0f / fConst11;
		fConst13 = fConst7 * fConst11;
		fConst14 = 1.0f / fConst13;
		iConst15 = std::max<int>(256, int(6.5f * fConst0));
		fConst16 = 0.005f * fConst0;
		fConst17 = 0.00086f * fConst0;
		fConst18 = 1.0f / fConst0;
		iConst19 = int(0.0015f * fConst0);
		fConst20 = float(std::max<int>(32, iConst19));
		fConst21 = std::exp(-(1.0f / std::max<float>(1.0f, 0.14f * fConst0)));
		fConst22 = 0.8f / fConst13;
		fConst23 = std::tan(7948.2295f / fConst0);
		fConst24 = 2.0f * (1.0f - 1.0f / Freeze95DSP_faustpower2_f(fConst23));
		fConst25 = 1.0f / fConst23;
		fConst26 = (fConst25 + -1.4142135f) / fConst23 + 1.0f;
		fConst27 = (fConst25 + 1.4142135f) / fConst23 + 1.0f;
		fConst28 = 1.0f / fConst27;
		fConst29 = std::tan(587.47784f / fConst0);
		fConst30 = Freeze95DSP_faustpower2_f(fConst29);
		fConst31 = 2.0f * (1.0f - 1.0f / fConst30);
		fConst32 = 1.0f / fConst29;
		fConst33 = (fConst32 + -1.4142135f) / fConst29 + 1.0f;
		fConst34 = (fConst32 + 1.4142135f) / fConst29 + 1.0f;
		fConst35 = 1.0f / fConst34;
		fConst36 = 1.0f / (fConst30 * fConst34);
		fConst37 = 0.8f / fConst27;
		fConst38 = 3.1415927f / fConst0;
		fConst39 = std::tan(1140.3982f / fConst0);
		fConst40 = 2.0f * (1.0f - 1.0f / Freeze95DSP_faustpower2_f(fConst39));
		fConst41 = 1.0f / fConst39;
		fConst42 = (fConst41 + -1.4142135f) / fConst39 + 1.0f;
		fConst43 = (fConst41 + 1.4142135f) / fConst39 + 1.0f;
		fConst44 = 1.0f / fConst43;
		fConst45 = 0.00112f * fConst0;
		fConst46 = 0.8f / fConst43;
		fConst47 = std::exp(-(1.0f / std::max<float>(1.0f, 0.6f * fConst0)));
		fConst48 = 0.9f * (1.0f - fConst47);
		fConst49 = 0.0014705883f * fConst0;
		iConst50 = std::min<int>(4096, std::max<int>(0, std::max<int>(1, iConst19)));
	}
	
	void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.55f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
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
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec5[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iVec2[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iVec4[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec6[l8] = 0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec4[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec8[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec10[l12] = 0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec9[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		IOTA0 = 0;
		for (int l16 = 0; l16 < 2097152; l16 = l16 + 1) {
			fVec5[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec12[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			iRec15[l18] = 0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec14[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			iRec16[l20] = 0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			iRec17[l21] = 0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			iRec20[l22] = 0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			iRec21[l23] = 0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec19[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec18[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec22[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec24[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec23[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			iRec27[l29] = 0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec25[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			iRec29[l32] = 0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec28[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec31[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec30[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			iRec32[l36] = 0;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			iRec34[l37] = 0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec33[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec35[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			iVec6[l40] = 0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec36[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec37[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec38[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec39[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec40[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec41[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec44[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec43[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2097152; l49 = l49 + 1) {
			fVec7[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec42[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			iRec47[l51] = 0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec46[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec45[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec48[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			iRec50[l55] = 0;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec49[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			iRec53[l57] = 0;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec52[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec51[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			iRec55[l60] = 0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec54[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec57[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec56[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			iRec59[l64] = 0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec58[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec60[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			iVec8[l67] = 0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec61[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec62[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			iRec65[l70] = 0;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			iRec69[l71] = 0;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec71[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2097152; l73 = l73 + 1) {
			fVec9[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec70[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			iRec74[l75] = 0;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec73[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec72[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec75[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			iRec77[l79] = 0;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec76[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			iRec80[l81] = 0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec79[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			iRec82[l84] = 0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec81[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec84[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec83[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			iRec86[l88] = 0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec85[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec87[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			iVec10[l91] = 0;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec88[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec89[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec68[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec90[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fVec11[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec67[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec66[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec64[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec63[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			iRec93[l101] = 0;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			iRec97[l102] = 0;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec96[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec98[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fVec12[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec95[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec94[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec92[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec91[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			iRec101[l110] = 0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			iRec105[l111] = 0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec104[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec106[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fVec13[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec103[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec102[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec100[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec99[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2048; l119 = l119 + 1) {
			fVec14[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 3; l120 = l120 + 1) {
			fRec108[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2097152; l121 = l121 + 1) {
			fVec15[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec107[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec109[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec110[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			iVec16[l125] = 0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec111[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec112[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 3; l128 = l128 + 1) {
			fRec115[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 3; l129 = l129 + 1) {
			fRec114[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 2097152; l130 = l130 + 1) {
			fVec17[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec113[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec116[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec117[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			iVec18[l134] = 0;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec118[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec119[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 3; l137 = l137 + 1) {
			fRec126[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2097152; l138 = l138 + 1) {
			fVec19[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec125[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec127[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec128[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
			iVec20[l142] = 0;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec129[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec130[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec124[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec131[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fVec21[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec123[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec122[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fRec121[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec120[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec136[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fRec137[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fVec22[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec135[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec134[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec133[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec132[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2; l159 = l159 + 1) {
			fRec142[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec143[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fVec23[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fRec141[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec140[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec139[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec138[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 2048; l166 = l166 + 1) {
			fVec24[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 4096; l167 = l167 + 1) {
			fVec25[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec145[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec144[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 4096; l170 = l170 + 1) {
			fVec26[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 512; l171 = l171 + 1) {
			fVec27[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec1[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fVec28[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec0[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 512; l175 = l175 + 1) {
			fVec29[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec147[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fVec30[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 2; l178 = l178 + 1) {
			fRec146[l178] = 0.0f;
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
		fSlow2 = fConst2 * (1.0f - float(fCheckbox0));
		fSlow3 = float(fHslider1);
		fSlow4 = fConst2 * fSlow3;
		fSlow5 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 8.333333f * fSlow0));
		fSlow6 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 5.0f * fSlow0));
		fSlow7 = std::max<float>(fSlow5, fSlow6);
		iSlow8 = fSlow7 > 0.05f;
		fSlow9 = 0.75f * (1.0f - 0.9f * fSlow5) * (1.0f - 0.9f * fSlow6);
		fSlow10 = 0.4f * fSlow6 + 1.0f;
		fSlow11 = fConst17 * fSlow10;
		fSlow12 = 22.0f * (1.0f - 0.85f * fSlow6);
		fSlow13 = 1.0f - 0.35f * fSlow6;
		iSlow14 = float(fCheckbox1) > 0.0f;
		fSlow15 = 0.7f * fSlow6;
		fSlow16 = 1.0f - fSlow6;
		fSlow17 = std::max<float>(2e+01f, float(fHslider2));
		fSlow18 = 0.004858333f * fSlow17;
		fSlow19 = 0.006941667f * fSlow17;
		fSlow20 = 0.0125f * fSlow17;
		fSlow21 = 0.025f * fSlow17;
		fSlow22 = 0.05f * fSlow17;
		fSlow23 = 0.1f * fSlow17;
		fSlow24 = 0.13333334f * fSlow17;
		fSlow25 = 0.06666667f * fSlow17;
		fSlow26 = 0.033333335f * fSlow17;
		fSlow27 = 0.016666668f * fSlow17;
		fSlow28 = 0.008333334f * fSlow17;
		fSlow29 = 0.00555f * fSlow17;
		fSlow30 = 0.004166667f * fSlow17;
		iSlow31 = (fSlow7 > 0.02f) | (fSlow0 < 0.12f);
		fSlow32 = fConst3 * fSlow10;
		fSlow33 = fConst45 * fSlow10;
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
			float fTemp1 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, 2.3809524f * (fRec2[0] + -0.58f))), 1.8f);
			float fTemp2 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, 1.6129032f * (fRec2[0] + -0.18f))), 1.35f);
			float fTemp3 = std::pow(fRec2[0], 1.1f);
			float fTemp4 = std::min<float>(1.0f, 0.64f * fTemp3 + 0.26f * fTemp2 + 0.1f * fTemp1);
			float fTemp5 = 1.0f - fTemp4;
			float fTemp6 = std::exp(-(1.0f / std::max<float>(1.0f, fConst3 * (12.0f * fTemp5 + 8.0f))));
			float fTemp7 = 0.04f * fTemp5;
			float fTemp8 = fTemp7 + 0.94f;
			float fTemp9 = float(input0[i0]);
			float fTemp10 = 0.06f * fRec2[0] + 0.02f;
			float fTemp11 = fSlow2 + fConst1 * fRec3[1];
			fRec3[0] = ((*reinterpret_cast<int*>(&fTemp11) & 2139095040) ? fTemp11 : 0.0f);
			float fTemp12 = float(std::max<int>(0, std::min<int>(1023, int(fConst4 * fRec3[0] * (1.0f - 0.7f * fTemp5)))));
			iRec5[0] = (iRec5[1] + 1) % std::max<int>(1, int(fConst3 * std::max<float>(6e+01f, 1.2e+04f - 1.198e+04f * fTemp4)));
			int iTemp13 = std::fabs(fSlow0 - fVec1[1]) > 0.004f;
			iVec2[0] = iTemp13;
			fVec3[0] = fSlow3;
			int iTemp14 = std::fabs(fSlow3 - fVec3[1]) > 0.004f;
			iVec4[0] = iTemp14;
			int iTemp15 = int(std::max<float>(float(iRec5[0] == 0), std::max<float>(std::max<float>(0.0f, float(iTemp13 - iVec2[1])), std::max<float>(0.0f, float(iTemp14 - iVec4[1])))));
			iRec6[0] = 1103515245 * iRec6[1] + 444;
			float fTemp16 = 4.656613e-10f * float((iRec6[0] + 45678) % 2147483647);
			float fTemp17 = ((iTemp15) ? fTemp16 : fRec4[1]);
			fRec4[0] = ((*reinterpret_cast<int*>(&fTemp17) & 2139095040) ? fTemp17 : 0.0f);
			int iTemp18 = int(fRec4[0] * fTemp12);
			iRec8[0] = 1103515245 * iRec8[1] + 222;
			float fTemp19 = ((iTemp15) ? 4.656613e-10f * float((iRec8[0] + 23456) % 2147483647) : fRec7[1]);
			fRec7[0] = ((*reinterpret_cast<int*>(&fTemp19) & 2139095040) ? fTemp19 : 0.0f);
			int iTemp20 = int(fRec7[0] * fTemp12);
			iRec10[0] = 1103515245 * iRec10[1] + 111;
			float fTemp21 = ((iTemp15) ? 4.656613e-10f * float((iRec10[0] + 12345) % 2147483647) : fRec9[1]);
			fRec9[0] = ((*reinterpret_cast<int*>(&fTemp21) & 2139095040) ? fTemp21 : 0.0f);
			float fTemp22 = float((fRec9[0] < 0.05f) & (((iTemp20 != iTemp18) | (iTemp20 > 0)) | (iTemp18 > 0)));
			float fTemp23 = fTemp22 * fTemp10;
			float fTemp24 = fSlow4 + fConst1 * fRec11[1];
			fRec11[0] = ((*reinterpret_cast<int*>(&fTemp24) & 2139095040) ? fTemp24 : 0.0f);
			float fTemp25 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, 1.2f * fRec11[0])));
			float fTemp26 = 3.0f - 2.0f * fTemp25;
			float fTemp27 = Freeze95DSP_faustpower2_f(fTemp25) * fTemp26;
			float fTemp28 = fTemp25 * (0.45f * fTemp27 + 0.55f);
			float fTemp29 = 1.0f - fTemp28;
			float fTemp30 = fTemp9 - fConst12 * (fConst10 * fRec13[2] + fConst8 * fRec13[1]);
			fRec13[0] = ((*reinterpret_cast<int*>(&fTemp30) & 2139095040) ? fTemp30 : 0.0f);
			float fTemp31 = fRec13[2] + (fRec13[0] - 2.0f * fRec13[1]);
			fVec5[IOTA0 & 2097151] = fTemp31;
			float fTemp32 = std::fabs(fConst14 * fTemp31);
			float fTemp33 = std::max<float>(fConst5 * fRec12[1], fTemp32);
			fRec12[0] = ((*reinterpret_cast<int*>(&fTemp33) & 2139095040) ? fTemp33 : 0.0f);
			float fTemp34 = std::min<float>(1.0f, 4.0f * fRec12[0]);
			float fTemp35 = std::min<float>(1.0f, 0.58f * fTemp2 + 0.42f * fTemp1);
			float fTemp36 = 0.65f * fTemp4 + 0.35f;
			iRec15[0] = 1103515245 * iRec15[1] + 555;
			float fTemp37 = ((iTemp15) ? 4.656613e-10f * float((iRec15[0] + 56789) % 2147483647) : fRec14[1]);
			fRec14[0] = ((*reinterpret_cast<int*>(&fTemp37) & 2139095040) ? fTemp37 : 0.0f);
			int iTemp38 = std::max<int>(1, int(fConst3 * (2.5e+02f * fTemp35 + 12.0f)));
			float fTemp39 = std::min<float>(0.49f, float(std::max<int>(1, int(fConst3 * (2.2f - 1.6f * fTemp4)))) / float(iTemp38));
			iRec16[0] = (iVec0[1] + iRec16[1]) % iTemp38;
			float fTemp40 = float(iRec16[0]);
			float fTemp41 = ((int(((iSlow8) ? 0.0f : float(fRec14[0] < (fSlow9 * fTemp36 * fTemp35 * (0.8f * fTemp35 + 1.0f)))))) ? ((fTemp40 < fTemp39) ? 1.0f - fTemp40 / fTemp39 : ((fTemp40 < (0.5f - fTemp39)) ? 0.0f : ((fTemp40 < 0.5f) ? (fTemp40 + fTemp39 + -0.5f) / fTemp39 : 1.0f))) : 1.0f);
			iRec17[0] = 1103515245 * iRec17[1] + 12345;
			float fTemp42 = Freeze95DSP_faustpower2_f(fRec2[0]) * float(iRec17[0]) * (0.75f * fTemp4 + 0.25f) * (1.0f - fTemp41);
			iRec20[0] = (iRec20[1] + 1) % std::max<int>(1, int(fConst3 * (1.5e+04f * (1.0f - fTemp3) + 5e+03f)));
			int iTemp43 = iRec20[0] == 0;
			iRec21[0] = 1103515245 * iRec21[1] + 3033;
			float fTemp44 = 4.656613e-10f * float((iRec21[0] + 33467) % 2147483647);
			float fTemp45 = ((iTemp43) ? fTemp44 : fRec19[1]);
			fRec19[0] = ((*reinterpret_cast<int*>(&fTemp45) & 2139095040) ? fTemp45 : 0.0f);
			float fTemp46 = 0.97f * fRec18[1] + 0.08f * (fRec19[0] + -0.5f);
			fRec18[0] = ((*reinterpret_cast<int*>(&fTemp46) & 2139095040) ? fTemp46 : 0.0f);
			float fTemp47 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec18[0])) + 1.0f;
			float fTemp48 = std::max<float>(fConst16 * fRec22[1], fTemp32);
			fRec22[0] = ((*reinterpret_cast<int*>(&fTemp48) & 2139095040) ? fTemp48 : 0.0f);
			float fTemp49 = std::min<float>(1.0f, 3.3333333f * fRec22[0]);
			float fTemp50 = 1.0f - fTemp49;
			iRec24[0] = 1103515245 * iRec24[1] + 3011;
			float fTemp51 = ((iTemp15) ? 4.656613e-10f * float((iRec24[0] + 31245) % 2147483647) : fRec23[1]);
			fRec23[0] = ((*reinterpret_cast<int*>(&fTemp51) & 2139095040) ? fTemp51 : 0.0f);
			float fTemp52 = 187.5f - 137.5f * fRec2[0];
			float fTemp53 = fTemp52 - (375.0f - 275.0f * fRec2[0]);
			float fTemp54 = 0.25f * fTemp5 + 1.0f;
			int iTemp55 = std::max<int>(1, std::min<int>(int(fSlow11 * fTemp54 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec23[0] * (1.0f - 0.15f * fTemp50) + 0.045f * fTemp50)) * fTemp47), iConst15));
			int iTemp56 = std::max<int>(1, iTemp55);
			float fTemp57 = 0.65f * fTemp5;
			float fTemp58 = std::min<float>(0.9f, fTemp57);
			iRec27[0] = 1103515245 * iRec27[1] + 3022;
			float fTemp59 = 4.656613e-10f * float((iRec27[0] + 32356) % 2147483647);
			float fTemp60 = ((iTemp43) ? fTemp59 : fRec26[1]);
			fRec26[0] = ((*reinterpret_cast<int*>(&fTemp60) & 2139095040) ? fTemp60 : 0.0f);
			float fTemp61 = 0.97f * fRec25[1] + 0.08f * (fRec26[0] + -0.5f);
			fRec25[0] = ((*reinterpret_cast<int*>(&fTemp61) & 2139095040) ? fTemp61 : 0.0f);
			float fTemp62 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec25[0]));
			iRec29[0] = 1103515245 * iRec29[1] + 3055;
			float fTemp63 = ((iTemp15) ? 4.656613e-10f * float((iRec29[0] + 35689) % 2147483647) : fRec28[1]);
			fRec28[0] = ((*reinterpret_cast<int*>(&fTemp63) & 2139095040) ? fTemp63 : 0.0f);
			float fTemp64 = 3.0f * fTemp4 + 1.0f + fSlow12 * fTemp4;
			float fTemp65 = fTemp36 * fTemp64;
			int iTemp66 = int(fSlow13 * fTemp65 * (fRec28[0] - 0.5f * (1.0f - fTemp62)));
			int iTemp67 = iTemp55 + iTemp66;
			float fTemp68 = float(iTemp55);
			int iTemp69 = 1 - iVec0[1];
			float fTemp70 = fSlow16 * (2.6f * fTemp4 + 0.35f);
			float fTemp71 = 1.1e+02f * fTemp4 + (18.0f - fTemp70) - fSlow15;
			float fTemp72 = ((iTemp15) ? fTemp44 : fRec31[1]);
			fRec31[0] = ((*reinterpret_cast<int*>(&fTemp72) & 2139095040) ? fTemp72 : 0.0f);
			float fTemp73 = 1.0f - fTemp57;
			float fTemp74 = 0.585f * fTemp5;
			float fTemp75 = 1.22f * (fTemp74 + fTemp73 * (fSlow15 + fTemp70 + fRec31[0] * fTemp71)) * (fTemp62 + 1.0f);
			float fTemp76 = ((iSlow14) ? ((fTemp75 < fSlow18) ? fSlow30 : ((fTemp75 < fSlow19) ? fSlow29 : ((fTemp75 < fSlow20) ? fSlow28 : ((fTemp75 < fSlow21) ? fSlow27 : ((fTemp75 < fSlow22) ? fSlow26 : ((fTemp75 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp75);
			float fTemp77 = fTemp76 / std::max<float>(fConst18, fConst18 * fTemp68);
			float fTemp78 = ((iTemp69) ? 0.0f : fRec30[1] + fConst18 * std::min<float>(5e+02f, fTemp77));
			float fTemp79 = fTemp78 - std::floor(fTemp78);
			fRec30[0] = ((*reinterpret_cast<int*>(&fTemp79) & 2139095040) ? fTemp79 : 0.0f);
			int iTemp80 = int(fRec30[0] * fTemp68) % iTemp56;
			float fTemp81 = 1.0f - fTemp58;
			iRec32[0] = (iVec0[1] + iRec32[1]) % iConst15;
			iRec34[0] = 1103515245 * iRec34[1] + 3044;
			float fTemp82 = ((iTemp15) ? 4.656613e-10f * float((iRec34[0] + 34578) % 2147483647) : fRec33[1]);
			fRec33[0] = ((*reinterpret_cast<int*>(&fTemp82) & 2139095040) ? fTemp82 : 0.0f);
			int iTemp83 = fRec33[0] < 0.33f;
			int iTemp84 = fRec33[0] < 0.66f;
			float fTemp85 = ((iTemp15) ? fTemp59 : fRec35[1]);
			fRec35[0] = ((*reinterpret_cast<int*>(&fTemp85) & 2139095040) ? fTemp85 : 0.0f);
			int iTemp86 = std::max<int>(16, int(fConst20 * (0.8f * fRec35[0] + 0.6f)));
			float fTemp87 = float(iTemp86);
			float fTemp88 = float(iTemp80 % iTemp86) / fTemp87;
			float fTemp89 = std::max<float>(0.05f, ((iTemp83) ? std::sin(3.1415927f * fTemp88) : ((iTemp84) ? 1.0f - std::fabs(2.0f * fTemp88 + -1.0f) : std::pow(fTemp88, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp88)))));
			float fTemp90 = fTemp89 * fVec5[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fTemp81 * float((iTemp80 + iTemp67) % iTemp56) + fTemp58 * float(iTemp55 + -1)) % iTemp56 - iTemp55)) % iConst15))) & 2097151];
			float fTemp91 = 0.4f * fTemp5 + 0.6f;
			int iTemp92 = fTemp49 > 0.65f;
			iVec6[0] = iTemp92;
			float fTemp93 = fConst21 * fRec36[1] + std::max<float>(0.0f, float(iTemp92 - iVec6[1]));
			fRec36[0] = ((*reinterpret_cast<int*>(&fTemp93) & 2139095040) ? fTemp93 : 0.0f);
			float fTemp94 = fRec3[0] * std::min<float>(0.22f, fRec36[0] * fTemp91 * (0.4f * fTemp49 + 0.28f));
			float fTemp95 = Freeze95DSP_faustpower2_f(fTemp4) * (3.0f - 2.0f * fTemp4);
			float fTemp96 = 0.1f * std::max<float>(0.05f, fTemp7 + (0.9f - 0.45f * fTemp95)) + 0.9f * fRec37[1];
			fRec37[0] = ((*reinterpret_cast<int*>(&fTemp96) & 2139095040) ? fTemp96 : 0.0f);
			float fTemp97 = fRec37[0] + -0.01f;
			float fTemp98 = std::min<float>(1.0f, std::max<float>(0.0f, fRec33[0] + 0.2f * (fTemp34 + -0.5f)));
			float fTemp99 = std::max<float>(0.001f, 0.65f * fTemp95 + 0.1f);
			float fTemp100 = std::max<float>(0.02f, 1.0f - fRec37[0]);
			float fTemp101 = 0.1f * (fTemp100 * (0.25f * fTemp95 + 0.05f) / fTemp99) + 0.9f * fRec38[1];
			fRec38[0] = ((*reinterpret_cast<int*>(&fTemp101) & 2139095040) ? fTemp101 : 0.0f);
			float fTemp102 = fRec37[0] + fRec38[0];
			float fTemp103 = fTemp102 + -0.01f;
			float fTemp104 = 0.1f * (fTemp100 * (0.22f * fTemp95 + 0.03f) / fTemp99) + 0.9f * fRec39[1];
			fRec39[0] = ((*reinterpret_cast<int*>(&fTemp104) & 2139095040) ? fTemp104 : 0.0f);
			float fTemp105 = fTemp102 + fRec39[0] + -0.01f;
			int iTemp106 = ((iSlow8) ? 0 : ((fTemp98 < fTemp97) ? 0 : ((fTemp98 < fTemp103) ? 1 : ((fTemp98 < fTemp105) ? 2 : 3))));
			int iTemp107 = iTemp106 == 0;
			float fTemp108 = std::pow(1e+01f, 0.05f * (-0.5f - 2.5f * fTemp4));
			float fTemp109 = ((iTemp69) ? 0.0f : fRec40[1] + 1.0f / float(std::max<int>(32, int(fConst3 * (9e+01f * fTemp4 + 26.0f)))));
			float fTemp110 = fTemp109 - std::floor(fTemp109);
			fRec40[0] = ((*reinterpret_cast<int*>(&fTemp110) & 2139095040) ? fTemp110 : 0.0f);
			float fTemp111 = std::sin(3.1415927f * fRec40[0]);
			float fTemp112 = ((iTemp69) ? 0.0f : fRec41[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp77));
			float fTemp113 = fTemp112 - std::floor(fTemp112);
			fRec41[0] = ((*reinterpret_cast<int*>(&fTemp113) & 2139095040) ? fTemp113 : 0.0f);
			float fTemp114 = fConst14 * fTemp90;
			float fTemp115 = 0.18f * fTemp35 + 1.0f;
			float fTemp116 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.82f * (fTemp41 * (((iSlow31) ? fTemp114 : fTemp115 * ((iTemp107) ? fTemp114 : ((iTemp106 == 1) ? fConst14 * fTemp89 * fVec5[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fTemp68 * (1.0f - fRec30[0])) % iTemp56 + iTemp67) % iTemp56 - iTemp55)) % iConst15))) & 2097151] : ((iTemp106 == 2) ? fConst14 * fVec5[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp66 + iTemp55 + int(fRec41[0] * fTemp68) % iTemp56) % iTemp56 - iTemp55)) % iConst15))) & 2097151] * fTemp89 : fConst22 * fTemp111 * fVec5[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp68) % iTemp56 - iTemp55)) % iConst15))) & 2097151]))) * ((iTemp107) ? 1.0f : fTemp108)) * (1.0f - fTemp94) + fConst14 * fTemp94 * (0.65f * fTemp31 + 0.35f * fTemp90)) + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp34)))));
			float fTemp117 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, 0.95f * fRec11[0] + 0.05f * fTemp2)));
			float fTemp118 = 3.0f - 2.0f * fTemp117;
			float fTemp119 = Freeze95DSP_faustpower2_f(fTemp117) * fTemp118;
			float fTemp120 = fTemp117 * (0.45f * fTemp119 + 0.55f);
			float fTemp121 = 1.0f - fTemp120;
			float fTemp122 = fTemp9 - fConst35 * (fConst33 * fRec44[2] + fConst31 * fRec44[1]);
			fRec44[0] = ((*reinterpret_cast<int*>(&fTemp122) & 2139095040) ? fTemp122 : 0.0f);
			float fTemp123 = fConst36 * (fRec44[2] + (fRec44[0] - 2.0f * fRec44[1])) - fConst28 * (fConst26 * fRec43[2] + fConst24 * fRec43[1]);
			fRec43[0] = ((*reinterpret_cast<int*>(&fTemp123) & 2139095040) ? fTemp123 : 0.0f);
			float fTemp124 = fRec43[2] + fRec43[0] + 2.0f * fRec43[1];
			fVec7[IOTA0 & 2097151] = fTemp124;
			float fTemp125 = std::fabs(fConst28 * fTemp124);
			float fTemp126 = std::max<float>(fConst5 * fRec42[1], fTemp125);
			fRec42[0] = ((*reinterpret_cast<int*>(&fTemp126) & 2139095040) ? fTemp126 : 0.0f);
			float fTemp127 = std::min<float>(1.0f, 4.0f * fRec42[0]);
			iRec47[0] = 1103515245 * iRec47[1] + 2033;
			float fTemp128 = 4.656613e-10f * float((iRec47[0] + 23467) % 2147483647);
			float fTemp129 = ((iTemp43) ? fTemp128 : fRec46[1]);
			fRec46[0] = ((*reinterpret_cast<int*>(&fTemp129) & 2139095040) ? fTemp129 : 0.0f);
			float fTemp130 = 0.97f * fRec45[1] + 0.08f * (fRec46[0] + -0.5f);
			fRec45[0] = ((*reinterpret_cast<int*>(&fTemp130) & 2139095040) ? fTemp130 : 0.0f);
			float fTemp131 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec45[0])) + 1.0f;
			float fTemp132 = std::max<float>(fConst16 * fRec48[1], fTemp125);
			fRec48[0] = ((*reinterpret_cast<int*>(&fTemp132) & 2139095040) ? fTemp132 : 0.0f);
			float fTemp133 = std::min<float>(1.0f, 3.3333333f * fRec48[0]);
			float fTemp134 = 1.0f - fTemp133;
			iRec50[0] = 1103515245 * iRec50[1] + 2011;
			float fTemp135 = ((iTemp15) ? 4.656613e-10f * float((iRec50[0] + 21245) % 2147483647) : fRec49[1]);
			fRec49[0] = ((*reinterpret_cast<int*>(&fTemp135) & 2139095040) ? fTemp135 : 0.0f);
			int iTemp136 = std::max<int>(1, std::min<int>(int(fSlow32 * fTemp54 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec49[0] * (1.0f - 0.15f * fTemp134) + 0.045f * fTemp134)) * fTemp131), iConst15));
			int iTemp137 = std::max<int>(1, iTemp136);
			iRec53[0] = 1103515245 * iRec53[1] + 2022;
			float fTemp138 = 4.656613e-10f * float((iRec53[0] + 22356) % 2147483647);
			float fTemp139 = ((iTemp43) ? fTemp138 : fRec52[1]);
			fRec52[0] = ((*reinterpret_cast<int*>(&fTemp139) & 2139095040) ? fTemp139 : 0.0f);
			float fTemp140 = 0.97f * fRec51[1] + 0.08f * (fRec52[0] + -0.5f);
			fRec51[0] = ((*reinterpret_cast<int*>(&fTemp140) & 2139095040) ? fTemp140 : 0.0f);
			float fTemp141 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec51[0]));
			iRec55[0] = 1103515245 * iRec55[1] + 2055;
			float fTemp142 = ((iTemp15) ? 4.656613e-10f * float((iRec55[0] + 25689) % 2147483647) : fRec54[1]);
			fRec54[0] = ((*reinterpret_cast<int*>(&fTemp142) & 2139095040) ? fTemp142 : 0.0f);
			int iTemp143 = int(fSlow13 * fTemp65 * (fRec54[0] - 0.5f * (1.0f - fTemp141)));
			int iTemp144 = iTemp136 + iTemp143;
			float fTemp145 = float(iTemp136);
			float fTemp146 = ((iTemp15) ? fTemp128 : fRec57[1]);
			fRec57[0] = ((*reinterpret_cast<int*>(&fTemp146) & 2139095040) ? fTemp146 : 0.0f);
			float fTemp147 = (fTemp74 + fTemp73 * (fSlow15 + fTemp70 + fRec57[0] * fTemp71)) * (fTemp141 + 1.0f);
			float fTemp148 = ((iSlow14) ? ((fTemp147 < fSlow18) ? fSlow30 : ((fTemp147 < fSlow19) ? fSlow29 : ((fTemp147 < fSlow20) ? fSlow28 : ((fTemp147 < fSlow21) ? fSlow27 : ((fTemp147 < fSlow22) ? fSlow26 : ((fTemp147 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp147);
			float fTemp149 = fTemp148 / std::max<float>(fConst18, fConst18 * fTemp145);
			float fTemp150 = ((iTemp69) ? 0.0f : fRec56[1] + fConst18 * std::min<float>(5e+02f, fTemp149));
			float fTemp151 = fTemp150 - std::floor(fTemp150);
			fRec56[0] = ((*reinterpret_cast<int*>(&fTemp151) & 2139095040) ? fTemp151 : 0.0f);
			int iTemp152 = int(fRec56[0] * fTemp145) % iTemp137;
			iRec59[0] = 1103515245 * iRec59[1] + 2044;
			float fTemp153 = ((iTemp15) ? 4.656613e-10f * float((iRec59[0] + 24578) % 2147483647) : fRec58[1]);
			fRec58[0] = ((*reinterpret_cast<int*>(&fTemp153) & 2139095040) ? fTemp153 : 0.0f);
			int iTemp154 = fRec58[0] < 0.33f;
			int iTemp155 = fRec58[0] < 0.66f;
			float fTemp156 = ((iTemp15) ? fTemp138 : fRec60[1]);
			fRec60[0] = ((*reinterpret_cast<int*>(&fTemp156) & 2139095040) ? fTemp156 : 0.0f);
			int iTemp157 = std::max<int>(16, int(fConst20 * (0.8f * fRec60[0] + 0.6f)));
			float fTemp158 = float(iTemp157);
			float fTemp159 = float(iTemp152 % iTemp157) / fTemp158;
			float fTemp160 = std::max<float>(0.05f, ((iTemp154) ? std::sin(3.1415927f * fTemp159) : ((iTemp155) ? 1.0f - std::fabs(2.0f * fTemp159 + -1.0f) : std::pow(fTemp159, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp159)))));
			float fTemp161 = fTemp160 * fVec7[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fTemp81 * float((iTemp152 + iTemp144) % iTemp137) + fTemp58 * float(iTemp136 + -1)) % iTemp137 - iTemp136)) % iConst15))) & 2097151];
			int iTemp162 = fTemp133 > 0.65f;
			iVec8[0] = iTemp162;
			float fTemp163 = fConst21 * fRec61[1] + std::max<float>(0.0f, float(iTemp162 - iVec8[1]));
			fRec61[0] = ((*reinterpret_cast<int*>(&fTemp163) & 2139095040) ? fTemp163 : 0.0f);
			float fTemp164 = fRec3[0] * std::min<float>(0.22f, fRec61[0] * fTemp91 * (0.4f * fTemp133 + 0.28f));
			float fTemp165 = std::min<float>(1.0f, std::max<float>(0.0f, fRec58[0] + 0.2f * (fTemp127 + -0.5f)));
			int iTemp166 = ((iSlow8) ? 0 : ((fTemp165 < fTemp97) ? 0 : ((fTemp165 < fTemp103) ? 1 : ((fTemp165 < fTemp105) ? 2 : 3))));
			int iTemp167 = iTemp166 == 0;
			float fTemp168 = ((iTemp69) ? 0.0f : fRec62[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp149));
			float fTemp169 = fTemp168 - std::floor(fTemp168);
			fRec62[0] = ((*reinterpret_cast<int*>(&fTemp169) & 2139095040) ? fTemp169 : 0.0f);
			float fTemp170 = fConst28 * fTemp161;
			float fTemp171 = std::max<float>(-1.0f, std::min<float>(1.0f, 1.02f * (fTemp41 * (((iSlow31) ? fTemp170 : fTemp115 * ((iTemp167) ? fTemp170 : ((iTemp166 == 1) ? fConst28 * fTemp160 * fVec7[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fTemp145 * (1.0f - fRec56[0])) % iTemp137 + iTemp144) % iTemp137 - iTemp136)) % iConst15))) & 2097151] : ((iTemp166 == 2) ? fConst28 * fVec7[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp143 + iTemp136 + int(fRec62[0] * fTemp145) % iTemp137) % iTemp137 - iTemp136)) % iConst15))) & 2097151] * fTemp160 : fConst37 * fTemp111 * fVec7[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp145) % iTemp137 - iTemp136)) % iConst15))) & 2097151]))) * ((iTemp167) ? 1.0f : fTemp108)) * (1.0f - fTemp164) + fConst28 * fTemp164 * (0.65f * fTemp124 + 0.35f * fTemp161)) + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp127)))));
			float fTemp172 = std::min<float>(1.0f, std::max<float>(0.0f, 0.35f * fRec11[0]));
			float fTemp173 = 3.0f - 2.0f * fTemp172;
			float fTemp174 = Freeze95DSP_faustpower2_f(fTemp172) * fTemp173;
			float fTemp175 = 0.6f * fTemp174 + 0.4f;
			int iTemp176 = std::max<int>(1, int(7.0f * fTemp172 + 1.0f));
			float fTemp177 = std::max<float>(1.4e+02f, 1.6e+04f - 1.4e+04f * fTemp172);
			float fTemp178 = 1.0f / std::tan(fConst38 * std::max<float>(1.8e+02f, fTemp177 / (0.35f * float(iTemp176) + 0.8f)));
			float fTemp179 = fTemp178 + 1.0f;
			iRec65[0] = (iRec65[1] + 1) % iTemp176;
			int iTemp180 = iRec65[0] == 0;
			float fTemp181 = 6.0f * fTemp172;
			float fTemp182 = std::pow(1e+01f, 0.05f * (fTemp181 + 1.5f * (1.0f - std::min<float>(1.0f, 3.0f * fTemp172)))) + -1.0f;
			float fTemp183 = std::tan(fConst38 * (2e+03f * fTemp172 + 4e+03f));
			float fTemp184 = 1.0f / fTemp183;
			float fTemp185 = fTemp184 + 1.0f;
			float fTemp186 = 1.0f / std::tan(fConst38 * fTemp177);
			float fTemp187 = fTemp186 + 1.0f;
			float fTemp188 = 0.55f * fTemp172 + 1.0f;
			float fTemp189 = std::pow(2.0f, std::max<float>(5.0f, 16.0f - 1e+01f * fTemp172));
			iRec69[0] = (iVec0[1] + iRec69[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - 2.16e+04f * fTemp172)));
			int iTemp190 = iRec69[0] == 0;
			float fTemp191 = fTemp9 - fConst44 * (fConst42 * fRec71[2] + fConst40 * fRec71[1]);
			fRec71[0] = ((*reinterpret_cast<int*>(&fTemp191) & 2139095040) ? fTemp191 : 0.0f);
			float fTemp192 = fRec71[2] + fRec71[0] + 2.0f * fRec71[1];
			fVec9[IOTA0 & 2097151] = fTemp192;
			float fTemp193 = std::fabs(fConst44 * fTemp192);
			float fTemp194 = std::max<float>(fConst5 * fRec70[1], fTemp193);
			fRec70[0] = ((*reinterpret_cast<int*>(&fTemp194) & 2139095040) ? fTemp194 : 0.0f);
			float fTemp195 = std::min<float>(1.0f, 4.0f * fRec70[0]);
			iRec74[0] = 1103515245 * iRec74[1] + 1033;
			float fTemp196 = 4.656613e-10f * float((iRec74[0] + 15467) % 2147483647);
			float fTemp197 = ((iTemp43) ? fTemp196 : fRec73[1]);
			fRec73[0] = ((*reinterpret_cast<int*>(&fTemp197) & 2139095040) ? fTemp197 : 0.0f);
			float fTemp198 = 0.97f * fRec72[1] + 0.08f * (fRec73[0] + -0.5f);
			fRec72[0] = ((*reinterpret_cast<int*>(&fTemp198) & 2139095040) ? fTemp198 : 0.0f);
			float fTemp199 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec72[0])) + 1.0f;
			float fTemp200 = std::max<float>(fConst16 * fRec75[1], fTemp193);
			fRec75[0] = ((*reinterpret_cast<int*>(&fTemp200) & 2139095040) ? fTemp200 : 0.0f);
			float fTemp201 = std::min<float>(1.0f, 3.3333333f * fRec75[0]);
			float fTemp202 = 1.0f - fTemp201;
			iRec77[0] = 1103515245 * iRec77[1] + 1011;
			float fTemp203 = ((iTemp15) ? 4.656613e-10f * float((iRec77[0] + 13245) % 2147483647) : fRec76[1]);
			fRec76[0] = ((*reinterpret_cast<int*>(&fTemp203) & 2139095040) ? fTemp203 : 0.0f);
			int iTemp204 = std::max<int>(1, std::min<int>(int(fSlow33 * fTemp54 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec76[0] * (1.0f - 0.15f * fTemp202) + 0.045f * fTemp202)) * fTemp199), iConst15));
			int iTemp205 = std::max<int>(1, iTemp204);
			iRec80[0] = 1103515245 * iRec80[1] + 1022;
			float fTemp206 = 4.656613e-10f * float((iRec80[0] + 14356) % 2147483647);
			float fTemp207 = ((iTemp43) ? fTemp206 : fRec79[1]);
			fRec79[0] = ((*reinterpret_cast<int*>(&fTemp207) & 2139095040) ? fTemp207 : 0.0f);
			float fTemp208 = 0.97f * fRec78[1] + 0.08f * (fRec79[0] + -0.5f);
			fRec78[0] = ((*reinterpret_cast<int*>(&fTemp208) & 2139095040) ? fTemp208 : 0.0f);
			float fTemp209 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec78[0]));
			iRec82[0] = 1103515245 * iRec82[1] + 1055;
			float fTemp210 = ((iTemp15) ? 4.656613e-10f * float((iRec82[0] + 17689) % 2147483647) : fRec81[1]);
			fRec81[0] = ((*reinterpret_cast<int*>(&fTemp210) & 2139095040) ? fTemp210 : 0.0f);
			int iTemp211 = int(fSlow13 * (fRec81[0] - 0.5f * (1.0f - fTemp209)) * fTemp36 * fTemp64);
			int iTemp212 = iTemp204 + iTemp211;
			float fTemp213 = float(iTemp204);
			float fTemp214 = ((iTemp15) ? fTemp196 : fRec84[1]);
			fRec84[0] = ((*reinterpret_cast<int*>(&fTemp214) & 2139095040) ? fTemp214 : 0.0f);
			float fTemp215 = 0.82f * ((fSlow15 + fTemp70 + fRec84[0] * fTemp71) * fTemp73 + fTemp74) * (fTemp209 + 1.0f);
			float fTemp216 = ((iSlow14) ? ((fTemp215 < fSlow18) ? fSlow30 : ((fTemp215 < fSlow19) ? fSlow29 : ((fTemp215 < fSlow20) ? fSlow28 : ((fTemp215 < fSlow21) ? fSlow27 : ((fTemp215 < fSlow22) ? fSlow26 : ((fTemp215 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp215);
			float fTemp217 = fTemp216 / std::max<float>(fConst18, fConst18 * fTemp213);
			float fTemp218 = ((iTemp69) ? 0.0f : fRec83[1] + fConst18 * std::min<float>(5e+02f, fTemp217));
			float fTemp219 = fTemp218 - std::floor(fTemp218);
			fRec83[0] = ((*reinterpret_cast<int*>(&fTemp219) & 2139095040) ? fTemp219 : 0.0f);
			int iTemp220 = int(fRec83[0] * fTemp213) % iTemp205;
			iRec86[0] = 1103515245 * iRec86[1] + 1044;
			float fTemp221 = ((iTemp15) ? 4.656613e-10f * float((iRec86[0] + 16578) % 2147483647) : fRec85[1]);
			fRec85[0] = ((*reinterpret_cast<int*>(&fTemp221) & 2139095040) ? fTemp221 : 0.0f);
			int iTemp222 = fRec85[0] < 0.33f;
			int iTemp223 = fRec85[0] < 0.66f;
			float fTemp224 = ((iTemp15) ? fTemp206 : fRec87[1]);
			fRec87[0] = ((*reinterpret_cast<int*>(&fTemp224) & 2139095040) ? fTemp224 : 0.0f);
			int iTemp225 = std::max<int>(16, int(fConst20 * (0.8f * fRec87[0] + 0.6f)));
			float fTemp226 = float(iTemp225);
			float fTemp227 = float(iTemp220 % iTemp225) / fTemp226;
			float fTemp228 = std::max<float>(0.05f, ((iTemp222) ? std::sin(3.1415927f * fTemp227) : ((iTemp223) ? 1.0f - std::fabs(2.0f * fTemp227 + -1.0f) : std::pow(fTemp227, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp227)))));
			float fTemp229 = fTemp228 * fVec9[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(float((iTemp220 + iTemp212) % iTemp205) * fTemp81 + fTemp58 * float(iTemp204 + -1)) % iTemp205 - iTemp204)) % iConst15))) & 2097151];
			int iTemp230 = fTemp201 > 0.65f;
			iVec10[0] = iTemp230;
			float fTemp231 = fConst21 * fRec88[1] + std::max<float>(0.0f, float(iTemp230 - iVec10[1]));
			fRec88[0] = ((*reinterpret_cast<int*>(&fTemp231) & 2139095040) ? fTemp231 : 0.0f);
			float fTemp232 = fRec3[0] * std::min<float>(0.22f, fRec88[0] * (0.4f * fTemp201 + 0.28f) * fTemp91);
			float fTemp233 = std::min<float>(1.0f, std::max<float>(0.0f, fRec85[0] + 0.2f * (fTemp195 + -0.5f)));
			int iTemp234 = ((iSlow8) ? 0 : ((fTemp233 < fTemp97) ? 0 : ((fTemp233 < fTemp103) ? 1 : ((fTemp233 < fTemp105) ? 2 : 3))));
			int iTemp235 = iTemp234 == 0;
			float fTemp236 = ((iTemp69) ? 0.0f : fRec89[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp217));
			float fTemp237 = fTemp236 - std::floor(fTemp236);
			fRec89[0] = ((*reinterpret_cast<int*>(&fTemp237) & 2139095040) ? fTemp237 : 0.0f);
			float fTemp238 = fConst44 * fTemp229;
			float fTemp239 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.88f * ((((iSlow31) ? fTemp238 : ((iTemp235) ? fTemp238 : ((iTemp234 == 1) ? fConst44 * fTemp228 * fVec9[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fTemp213 * (1.0f - fRec83[0])) % iTemp205 + iTemp212) % iTemp205 - iTemp204)) % iConst15))) & 2097151] : ((iTemp234 == 2) ? fConst44 * fVec9[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp211 + iTemp204 + int(fRec89[0] * fTemp213) % iTemp205) % iTemp205 - iTemp204)) % iConst15))) & 2097151] * fTemp228 : fConst46 * fVec9[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp213) % iTemp205 - iTemp204)) % iConst15))) & 2097151] * fTemp111))) * ((iTemp235) ? 1.0f : fTemp108) * fTemp115) * (1.0f - fTemp232) + fConst44 * fTemp232 * (0.65f * fTemp192 + 0.35f * fTemp229)) * fTemp41 + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp195)))));
			float fTemp240 = 6.0f * fTemp172 * std::fabs(fTemp239) + 1.0f;
			float fTemp241 = fTemp181 + 1.0f;
			float fTemp242 = fTemp239 * fTemp241;
			float fTemp243 = fTemp242 / fTemp240;
			float fTemp244 = fTemp172 * (0.75f * fTemp172 * fTemp173 + 3.5f) + 1.5f;
			float fTemp245 = fTemp240 * (fTemp244 * std::fabs(fTemp243) + 1.0f);
			float fTemp246 = ((iTemp190) ? fTemp242 / fTemp245 : fRec68[1]);
			fRec68[0] = ((*reinterpret_cast<int*>(&fTemp246) & 2139095040) ? fTemp246 : 0.0f);
			float fTemp247 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec68[0] * fTemp189 + 0.5f) / fTemp189));
			float fTemp248 = 0.65f * fTemp172 + 0.35f;
			float fTemp249 = ((iTemp190) ? fTemp243 : fRec90[1]);
			fRec90[0] = ((*reinterpret_cast<int*>(&fTemp249) & 2139095040) ? fTemp249 : 0.0f);
			float fTemp250 = 1.0f - fTemp172;
			float fTemp251 = 1.0f - fTemp174;
			float fTemp252 = fTemp242 * fTemp189;
			float fTemp253 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp252 / fTemp245 + 0.5f) / fTemp189));
			float fTemp254 = ((0.65f * fTemp250 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp252 / fTemp240 + 0.5f) / fTemp189)) + fTemp248 * fTemp253 / std::max<float>(0.05f, 1.0f - fTemp244 * std::min<float>(0.999f, std::fabs(fTemp253)))) * fTemp251 + fTemp174 * (0.65f * fTemp250 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec90[0] * fTemp189 + 0.5f) / fTemp189)) + fTemp248 * fTemp247 / std::max<float>(0.05f, 1.0f - fTemp244 * std::min<float>(0.999f, std::fabs(fTemp247))))) * fTemp188;
			fVec11[0] = fTemp254;
			float fTemp255 = 1.0f - fTemp186;
			float fTemp256 = (fRec67[1] * fTemp255 - (fTemp254 + fVec11[1])) / fTemp187;
			fRec67[0] = ((*reinterpret_cast<int*>(&-fTemp256) & 2139095040) ? -fTemp256 : 0.0f);
			float fTemp257 = 1.0f - fTemp184;
			float fTemp258 = (fRec66[1] * fTemp257 - (fRec67[0] - fRec67[1]) / fTemp183) / fTemp185;
			fRec66[0] = ((*reinterpret_cast<int*>(&-fTemp258) & 2139095040) ? -fTemp258 : 0.0f);
			float fTemp259 = fRec67[0] + fRec66[0] * fTemp182;
			float fTemp260 = ((iTemp180) ? fTemp259 : fRec64[1]);
			fRec64[0] = ((*reinterpret_cast<int*>(&fTemp260) & 2139095040) ? fTemp260 : 0.0f);
			float fTemp261 = 1.0f - fTemp178;
			float fTemp262 = (fRec63[1] * fTemp261 - (fRec64[0] + fRec64[1])) / fTemp179;
			fRec63[0] = ((*reinterpret_cast<int*>(&-fTemp262) & 2139095040) ? -fTemp262 : 0.0f);
			float fTemp263 = 1.0f - fTemp172 * fTemp175;
			float fTemp264 = fTemp172 * (0.45f * fTemp174 + 0.55f);
			float fTemp265 = 1.0f - fTemp264;
			float fTemp266 = 0.6f * fTemp119 + 0.4f;
			int iTemp267 = std::max<int>(1, int(7.0f * fTemp117 + 1.0f));
			float fTemp268 = std::max<float>(1.4e+02f, 1.6e+04f - 1.4e+04f * fTemp117);
			float fTemp269 = 1.0f / std::tan(fConst38 * std::max<float>(1.8e+02f, fTemp268 / (0.35f * float(iTemp267) + 0.8f)));
			float fTemp270 = fTemp269 + 1.0f;
			iRec93[0] = (iRec93[1] + 1) % iTemp267;
			int iTemp271 = iRec93[0] == 0;
			float fTemp272 = 6.0f * fTemp117;
			float fTemp273 = std::pow(1e+01f, 0.05f * (fTemp272 + 1.5f * (1.0f - std::min<float>(1.0f, 3.0f * fTemp117)))) + -1.0f;
			float fTemp274 = std::tan(fConst38 * (2e+03f * fTemp117 + 4e+03f));
			float fTemp275 = 1.0f / fTemp274;
			float fTemp276 = fTemp275 + 1.0f;
			float fTemp277 = 1.0f / std::tan(fConst38 * fTemp268);
			float fTemp278 = fTemp277 + 1.0f;
			float fTemp279 = 0.55f * fTemp117 + 1.0f;
			float fTemp280 = std::pow(2.0f, std::max<float>(5.0f, 16.0f - 1e+01f * fTemp117));
			iRec97[0] = (iVec0[1] + iRec97[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - 2.16e+04f * fTemp117)));
			int iTemp281 = iRec97[0] == 0;
			float fTemp282 = 6.0f * fTemp117 * std::fabs(fTemp171) + 1.0f;
			float fTemp283 = fTemp272 + 1.0f;
			float fTemp284 = fTemp171 * fTemp283;
			float fTemp285 = fTemp284 / fTemp282;
			float fTemp286 = fTemp117 * (0.75f * fTemp117 * fTemp118 + 3.5f) + 1.5f;
			float fTemp287 = fTemp282 * (fTemp286 * std::fabs(fTemp285) + 1.0f);
			float fTemp288 = ((iTemp281) ? fTemp284 / fTemp287 : fRec96[1]);
			fRec96[0] = ((*reinterpret_cast<int*>(&fTemp288) & 2139095040) ? fTemp288 : 0.0f);
			float fTemp289 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec96[0] * fTemp280 + 0.5f) / fTemp280));
			float fTemp290 = 0.65f * fTemp117 + 0.35f;
			float fTemp291 = ((iTemp281) ? fTemp285 : fRec98[1]);
			fRec98[0] = ((*reinterpret_cast<int*>(&fTemp291) & 2139095040) ? fTemp291 : 0.0f);
			float fTemp292 = 1.0f - fTemp117;
			float fTemp293 = 1.0f - fTemp119;
			float fTemp294 = fTemp284 * fTemp280;
			float fTemp295 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp294 / fTemp287 + 0.5f) / fTemp280));
			float fTemp296 = ((0.65f * fTemp292 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp294 / fTemp282 + 0.5f) / fTemp280)) + fTemp290 * fTemp295 / std::max<float>(0.05f, 1.0f - fTemp286 * std::min<float>(0.999f, std::fabs(fTemp295)))) * fTemp293 + fTemp119 * (0.65f * fTemp292 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec98[0] * fTemp280 + 0.5f) / fTemp280)) + fTemp290 * fTemp289 / std::max<float>(0.05f, 1.0f - fTemp286 * std::min<float>(0.999f, std::fabs(fTemp289))))) * fTemp279;
			fVec12[0] = fTemp296;
			float fTemp297 = 1.0f - fTemp277;
			float fTemp298 = (fRec95[1] * fTemp297 - (fTemp296 + fVec12[1])) / fTemp278;
			fRec95[0] = ((*reinterpret_cast<int*>(&-fTemp298) & 2139095040) ? -fTemp298 : 0.0f);
			float fTemp299 = 1.0f - fTemp275;
			float fTemp300 = (fRec94[1] * fTemp299 - (fRec95[0] - fRec95[1]) / fTemp274) / fTemp276;
			fRec94[0] = ((*reinterpret_cast<int*>(&-fTemp300) & 2139095040) ? -fTemp300 : 0.0f);
			float fTemp301 = fRec95[0] + fRec94[0] * fTemp273;
			float fTemp302 = ((iTemp271) ? fTemp301 : fRec92[1]);
			fRec92[0] = ((*reinterpret_cast<int*>(&fTemp302) & 2139095040) ? fTemp302 : 0.0f);
			float fTemp303 = 1.0f - fTemp269;
			float fTemp304 = (fRec91[1] * fTemp303 - (fRec92[0] + fRec92[1])) / fTemp270;
			fRec91[0] = ((*reinterpret_cast<int*>(&-fTemp304) & 2139095040) ? -fTemp304 : 0.0f);
			float fTemp305 = 1.0f - fTemp117 * fTemp266;
			float fTemp306 = 0.6f * fTemp27 + 0.4f;
			int iTemp307 = std::max<int>(1, int(7.0f * fTemp25 + 1.0f));
			float fTemp308 = std::max<float>(1.4e+02f, 1.6e+04f - 1.4e+04f * fTemp25);
			float fTemp309 = 1.0f / std::tan(fConst38 * std::max<float>(1.8e+02f, fTemp308 / (0.35f * float(iTemp307) + 0.8f)));
			float fTemp310 = fTemp309 + 1.0f;
			iRec101[0] = (iRec101[1] + 1) % iTemp307;
			int iTemp311 = iRec101[0] == 0;
			float fTemp312 = 6.0f * fTemp25;
			float fTemp313 = std::pow(1e+01f, 0.05f * (fTemp312 + 1.5f * (1.0f - std::min<float>(1.0f, 3.0f * fTemp25)))) + -1.0f;
			float fTemp314 = std::tan(fConst38 * (2e+03f * fTemp25 + 4e+03f));
			float fTemp315 = 1.0f / fTemp314;
			float fTemp316 = fTemp315 + 1.0f;
			float fTemp317 = 1.0f / std::tan(fConst38 * fTemp308);
			float fTemp318 = fTemp317 + 1.0f;
			float fTemp319 = 0.55f * fTemp25 + 1.0f;
			float fTemp320 = std::pow(2.0f, std::max<float>(5.0f, 16.0f - 1e+01f * fTemp25));
			iRec105[0] = (iVec0[1] + iRec105[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - 2.16e+04f * fTemp25)));
			int iTemp321 = iRec105[0] == 0;
			float fTemp322 = 6.0f * fTemp25 * std::fabs(fTemp116) + 1.0f;
			float fTemp323 = fTemp312 + 1.0f;
			float fTemp324 = fTemp116 * fTemp323;
			float fTemp325 = fTemp324 / fTemp322;
			float fTemp326 = fTemp25 * (0.75f * fTemp25 * fTemp26 + 3.5f) + 1.5f;
			float fTemp327 = fTemp322 * (fTemp326 * std::fabs(fTemp325) + 1.0f);
			float fTemp328 = ((iTemp321) ? fTemp324 / fTemp327 : fRec104[1]);
			fRec104[0] = ((*reinterpret_cast<int*>(&fTemp328) & 2139095040) ? fTemp328 : 0.0f);
			float fTemp329 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec104[0] * fTemp320 + 0.5f) / fTemp320));
			float fTemp330 = 0.65f * fTemp25 + 0.35f;
			float fTemp331 = ((iTemp321) ? fTemp325 : fRec106[1]);
			fRec106[0] = ((*reinterpret_cast<int*>(&fTemp331) & 2139095040) ? fTemp331 : 0.0f);
			float fTemp332 = 1.0f - fTemp25;
			float fTemp333 = 1.0f - fTemp27;
			float fTemp334 = fTemp324 * fTemp320;
			float fTemp335 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp334 / fTemp327 + 0.5f) / fTemp320));
			float fTemp336 = ((0.65f * fTemp332 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp334 / fTemp322 + 0.5f) / fTemp320)) + fTemp330 * fTemp335 / std::max<float>(0.05f, 1.0f - fTemp326 * std::min<float>(0.999f, std::fabs(fTemp335)))) * fTemp333 + fTemp27 * (0.65f * fTemp332 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec106[0] * fTemp320 + 0.5f) / fTemp320)) + fTemp330 * fTemp329 / std::max<float>(0.05f, 1.0f - fTemp326 * std::min<float>(0.999f, std::fabs(fTemp329))))) * fTemp319;
			fVec13[0] = fTemp336;
			float fTemp337 = 1.0f - fTemp317;
			float fTemp338 = (fRec103[1] * fTemp337 - (fTemp336 + fVec13[1])) / fTemp318;
			fRec103[0] = ((*reinterpret_cast<int*>(&-fTemp338) & 2139095040) ? -fTemp338 : 0.0f);
			float fTemp339 = 1.0f - fTemp315;
			float fTemp340 = (fRec102[1] * fTemp339 - (fRec103[0] - fRec103[1]) / fTemp314) / fTemp316;
			fRec102[0] = ((*reinterpret_cast<int*>(&-fTemp340) & 2139095040) ? -fTemp340 : 0.0f);
			float fTemp341 = fRec103[0] + fRec102[0] * fTemp313;
			float fTemp342 = ((iTemp311) ? fTemp341 : fRec100[1]);
			fRec100[0] = ((*reinterpret_cast<int*>(&fTemp342) & 2139095040) ? fTemp342 : 0.0f);
			float fTemp343 = 1.0f - fTemp309;
			float fTemp344 = (fRec99[1] * fTemp343 - (fRec100[0] + fRec100[1])) / fTemp310;
			fRec99[0] = ((*reinterpret_cast<int*>(&-fTemp344) & 2139095040) ? -fTemp344 : 0.0f);
			float fTemp345 = 1.0f - fTemp25 * fTemp306;
			fVec14[IOTA0 & 2047] = fTemp28 * ((fTemp324 * fTemp332 / fTemp322 + fTemp25 * fTemp341) * fTemp345 + fRec99[0] * fTemp25 * fTemp306) + fTemp120 * ((fTemp284 * fTemp292 / fTemp282 + fTemp117 * fTemp301) * fTemp305 + fRec91[0] * fTemp117 * fTemp266) + fTemp239 * fTemp265 + fTemp264 * ((fTemp242 * fTemp250 / fTemp240 + fTemp172 * fTemp259) * fTemp263 + fRec63[0] * fTemp172 * fTemp175) + fTemp171 * fTemp121 + fTemp116 * fTemp29;
			float fTemp346 = fVec14[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp20))) & 2047];
			float fTemp347 = float(input1[i0]);
			float fTemp348 = fTemp347 - fConst12 * (fConst10 * fRec108[2] + fConst8 * fRec108[1]);
			fRec108[0] = ((*reinterpret_cast<int*>(&fTemp348) & 2139095040) ? fTemp348 : 0.0f);
			float fTemp349 = fRec108[2] + (fRec108[0] - 2.0f * fRec108[1]);
			fVec15[IOTA0 & 2097151] = fTemp349;
			float fTemp350 = std::fabs(fConst14 * fTemp349);
			float fTemp351 = std::max<float>(fConst5 * fRec107[1], fTemp350);
			fRec107[0] = ((*reinterpret_cast<int*>(&fTemp351) & 2139095040) ? fTemp351 : 0.0f);
			float fTemp352 = std::min<float>(1.0f, 4.0f * fRec107[0]);
			float fTemp353 = std::max<float>(fConst16 * fRec109[1], fTemp350);
			fRec109[0] = ((*reinterpret_cast<int*>(&fTemp353) & 2139095040) ? fTemp353 : 0.0f);
			float fTemp354 = std::min<float>(1.0f, 3.3333333f * fRec109[0]);
			float fTemp355 = 1.0f - fTemp354;
			int iTemp356 = std::max<int>(1, std::min<int>(int(fSlow11 * fTemp54 * fTemp47 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec23[0] * (1.0f - 0.15f * fTemp355) + 0.045f * fTemp355))), iConst15));
			int iTemp357 = std::max<int>(1, iTemp356);
			float fTemp358 = float(iTemp356);
			float fTemp359 = fTemp76 / std::max<float>(fConst18, fConst18 * fTemp358);
			float fTemp360 = ((iTemp69) ? 0.0f : fRec110[1] + fConst18 * std::min<float>(5e+02f, fTemp359));
			float fTemp361 = fTemp360 - std::floor(fTemp360);
			fRec110[0] = ((*reinterpret_cast<int*>(&fTemp361) & 2139095040) ? fTemp361 : 0.0f);
			int iTemp362 = int(fRec110[0] * fTemp358) % iTemp357;
			int iTemp363 = iTemp66 + iTemp356;
			float fTemp364 = float(iTemp362 % iTemp86) / fTemp87;
			float fTemp365 = std::max<float>(0.05f, ((iTemp83) ? std::sin(3.1415927f * fTemp364) : ((iTemp84) ? 1.0f - std::fabs(2.0f * fTemp364 + -1.0f) : std::pow(fTemp364, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp364)))));
			float fTemp366 = fTemp365 * fVec15[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fTemp81 * float((iTemp363 + iTemp362) % iTemp357) + fTemp58 * float(iTemp356 + -1)) % iTemp357 - iTemp356)) % iConst15))) & 2097151];
			int iTemp367 = fTemp354 > 0.65f;
			iVec16[0] = iTemp367;
			float fTemp368 = fConst21 * fRec111[1] + std::max<float>(0.0f, float(iTemp367 - iVec16[1]));
			fRec111[0] = ((*reinterpret_cast<int*>(&fTemp368) & 2139095040) ? fTemp368 : 0.0f);
			float fTemp369 = fRec3[0] * std::min<float>(0.22f, fRec111[0] * fTemp91 * (0.4f * fTemp354 + 0.28f));
			float fTemp370 = std::min<float>(1.0f, std::max<float>(0.0f, fRec33[0] + 0.2f * (fTemp352 + -0.5f)));
			int iTemp371 = ((iSlow8) ? 0 : ((fTemp370 < fTemp97) ? 0 : ((fTemp370 < fTemp103) ? 1 : ((fTemp370 < fTemp105) ? 2 : 3))));
			int iTemp372 = iTemp371 == 0;
			float fTemp373 = ((iTemp69) ? 0.0f : fRec112[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp359));
			float fTemp374 = fTemp373 - std::floor(fTemp373);
			fRec112[0] = ((*reinterpret_cast<int*>(&fTemp374) & 2139095040) ? fTemp374 : 0.0f);
			float fTemp375 = fConst14 * fTemp366;
			float fTemp376 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.82f * (fTemp41 * (((iSlow31) ? fTemp375 : fTemp115 * ((iTemp372) ? fTemp375 : ((iTemp371 == 1) ? fConst14 * fTemp365 * fVec15[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp363 + int(fTemp358 * (1.0f - fRec110[0])) % iTemp357) % iTemp357 - iTemp356)) % iConst15))) & 2097151] : ((iTemp371 == 2) ? fConst14 * fVec15[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fRec112[0] * fTemp358) % iTemp357 + iTemp363) % iTemp357 - iTemp356)) % iConst15))) & 2097151] * fTemp365 : fConst22 * fTemp111 * fVec15[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp358) % iTemp357 - iTemp356)) % iConst15))) & 2097151]))) * ((iTemp372) ? 1.0f : fTemp108)) * (1.0f - fTemp369) + fConst14 * fTemp369 * (0.65f * fTemp349 + 0.35f * fTemp366)) + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp352)))));
			float fTemp377 = fTemp347 - fConst35 * (fConst33 * fRec115[2] + fConst31 * fRec115[1]);
			fRec115[0] = ((*reinterpret_cast<int*>(&fTemp377) & 2139095040) ? fTemp377 : 0.0f);
			float fTemp378 = fConst36 * (fRec115[2] + (fRec115[0] - 2.0f * fRec115[1])) - fConst28 * (fConst26 * fRec114[2] + fConst24 * fRec114[1]);
			fRec114[0] = ((*reinterpret_cast<int*>(&fTemp378) & 2139095040) ? fTemp378 : 0.0f);
			float fTemp379 = fRec114[2] + fRec114[0] + 2.0f * fRec114[1];
			fVec17[IOTA0 & 2097151] = fTemp379;
			float fTemp380 = std::fabs(fConst28 * fTemp379);
			float fTemp381 = std::max<float>(fConst5 * fRec113[1], fTemp380);
			fRec113[0] = ((*reinterpret_cast<int*>(&fTemp381) & 2139095040) ? fTemp381 : 0.0f);
			float fTemp382 = std::min<float>(1.0f, 4.0f * fRec113[0]);
			float fTemp383 = std::max<float>(fConst16 * fRec116[1], fTemp380);
			fRec116[0] = ((*reinterpret_cast<int*>(&fTemp383) & 2139095040) ? fTemp383 : 0.0f);
			float fTemp384 = std::min<float>(1.0f, 3.3333333f * fRec116[0]);
			float fTemp385 = 1.0f - fTemp384;
			int iTemp386 = std::max<int>(1, std::min<int>(int(fSlow32 * fTemp54 * fTemp131 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec49[0] * (1.0f - 0.15f * fTemp385) + 0.045f * fTemp385))), iConst15));
			int iTemp387 = std::max<int>(1, iTemp386);
			float fTemp388 = float(iTemp386);
			float fTemp389 = fTemp148 / std::max<float>(fConst18, fConst18 * fTemp388);
			float fTemp390 = ((iTemp69) ? 0.0f : fRec117[1] + fConst18 * std::min<float>(5e+02f, fTemp389));
			float fTemp391 = fTemp390 - std::floor(fTemp390);
			fRec117[0] = ((*reinterpret_cast<int*>(&fTemp391) & 2139095040) ? fTemp391 : 0.0f);
			int iTemp392 = int(fRec117[0] * fTemp388) % iTemp387;
			int iTemp393 = iTemp143 + iTemp386;
			float fTemp394 = float(iTemp392 % iTemp157) / fTemp158;
			float fTemp395 = std::max<float>(0.05f, ((iTemp154) ? std::sin(3.1415927f * fTemp394) : ((iTemp155) ? 1.0f - std::fabs(2.0f * fTemp394 + -1.0f) : std::pow(fTemp394, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp394)))));
			float fTemp396 = fTemp395 * fVec17[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fTemp81 * float((iTemp393 + iTemp392) % iTemp387) + fTemp58 * float(iTemp386 + -1)) % iTemp387 - iTemp386)) % iConst15))) & 2097151];
			int iTemp397 = fTemp384 > 0.65f;
			iVec18[0] = iTemp397;
			float fTemp398 = fConst21 * fRec118[1] + std::max<float>(0.0f, float(iTemp397 - iVec18[1]));
			fRec118[0] = ((*reinterpret_cast<int*>(&fTemp398) & 2139095040) ? fTemp398 : 0.0f);
			float fTemp399 = fRec3[0] * std::min<float>(0.22f, fRec118[0] * fTemp91 * (0.4f * fTemp384 + 0.28f));
			float fTemp400 = std::min<float>(1.0f, std::max<float>(0.0f, fRec58[0] + 0.2f * (fTemp382 + -0.5f)));
			int iTemp401 = ((iSlow8) ? 0 : ((fTemp400 < fTemp97) ? 0 : ((fTemp400 < fTemp103) ? 1 : ((fTemp400 < fTemp105) ? 2 : 3))));
			int iTemp402 = iTemp401 == 0;
			float fTemp403 = ((iTemp69) ? 0.0f : fRec119[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp389));
			float fTemp404 = fTemp403 - std::floor(fTemp403);
			fRec119[0] = ((*reinterpret_cast<int*>(&fTemp404) & 2139095040) ? fTemp404 : 0.0f);
			float fTemp405 = fConst28 * fTemp396;
			float fTemp406 = std::max<float>(-1.0f, std::min<float>(1.0f, 1.02f * (fTemp41 * (((iSlow31) ? fTemp405 : fTemp115 * ((iTemp402) ? fTemp405 : ((iTemp401 == 1) ? fConst28 * fTemp395 * fVec17[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp393 + int(fTemp388 * (1.0f - fRec117[0])) % iTemp387) % iTemp387 - iTemp386)) % iConst15))) & 2097151] : ((iTemp401 == 2) ? fConst28 * fVec17[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fRec119[0] * fTemp388) % iTemp387 + iTemp393) % iTemp387 - iTemp386)) % iConst15))) & 2097151] * fTemp395 : fConst37 * fTemp111 * fVec17[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp388) % iTemp387 - iTemp386)) % iConst15))) & 2097151]))) * ((iTemp402) ? 1.0f : fTemp108)) * (1.0f - fTemp399) + fConst28 * fTemp399 * (0.65f * fTemp379 + 0.35f * fTemp396)) + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp382)))));
			float fTemp407 = fTemp347 - fConst44 * (fConst42 * fRec126[2] + fConst40 * fRec126[1]);
			fRec126[0] = ((*reinterpret_cast<int*>(&fTemp407) & 2139095040) ? fTemp407 : 0.0f);
			float fTemp408 = fRec126[2] + fRec126[0] + 2.0f * fRec126[1];
			fVec19[IOTA0 & 2097151] = fTemp408;
			float fTemp409 = std::fabs(fConst44 * fTemp408);
			float fTemp410 = std::max<float>(fConst5 * fRec125[1], fTemp409);
			fRec125[0] = ((*reinterpret_cast<int*>(&fTemp410) & 2139095040) ? fTemp410 : 0.0f);
			float fTemp411 = std::min<float>(1.0f, 4.0f * fRec125[0]);
			float fTemp412 = std::max<float>(fConst16 * fRec127[1], fTemp409);
			fRec127[0] = ((*reinterpret_cast<int*>(&fTemp412) & 2139095040) ? fTemp412 : 0.0f);
			float fTemp413 = std::min<float>(1.0f, 3.3333333f * fRec127[0]);
			float fTemp414 = 1.0f - fTemp413;
			int iTemp415 = std::max<int>(1, std::min<int>(int(fSlow33 * fTemp54 * fTemp199 * (fTemp52 - fTemp53 * Freeze95DSP_faustpower2_f(fRec76[0] * (1.0f - 0.15f * fTemp414) + 0.045f * fTemp414))), iConst15));
			int iTemp416 = std::max<int>(1, iTemp415);
			float fTemp417 = float(iTemp415);
			float fTemp418 = fTemp216 / std::max<float>(fConst18, fConst18 * fTemp417);
			float fTemp419 = ((iTemp69) ? 0.0f : fRec128[1] + fConst18 * std::min<float>(5e+02f, fTemp418));
			float fTemp420 = fTemp419 - std::floor(fTemp419);
			fRec128[0] = ((*reinterpret_cast<int*>(&fTemp420) & 2139095040) ? fTemp420 : 0.0f);
			int iTemp421 = int(fRec128[0] * fTemp417) % iTemp416;
			int iTemp422 = iTemp211 + iTemp415;
			float fTemp423 = float(iTemp421 % iTemp225) / fTemp226;
			float fTemp424 = std::max<float>(0.05f, ((iTemp222) ? std::sin(3.1415927f * fTemp423) : ((iTemp223) ? 1.0f - std::fabs(2.0f * fTemp423 + -1.0f) : std::pow(fTemp423, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp423)))));
			float fTemp425 = fTemp424 * fVec19[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fTemp81 * float((iTemp422 + iTemp421) % iTemp416) + fTemp58 * float(iTemp415 + -1)) % iTemp416 - iTemp415)) % iConst15))) & 2097151];
			int iTemp426 = fTemp413 > 0.65f;
			iVec20[0] = iTemp426;
			float fTemp427 = fConst21 * fRec129[1] + std::max<float>(0.0f, float(iTemp426 - iVec20[1]));
			fRec129[0] = ((*reinterpret_cast<int*>(&fTemp427) & 2139095040) ? fTemp427 : 0.0f);
			float fTemp428 = fRec3[0] * std::min<float>(0.22f, fRec129[0] * fTemp91 * (0.4f * fTemp413 + 0.28f));
			float fTemp429 = std::min<float>(1.0f, std::max<float>(0.0f, fRec85[0] + 0.2f * (fTemp411 + -0.5f)));
			int iTemp430 = ((iSlow8) ? 0 : ((fTemp429 < fTemp97) ? 0 : ((fTemp429 < fTemp103) ? 1 : ((fTemp429 < fTemp105) ? 2 : 3))));
			int iTemp431 = iTemp430 == 0;
			float fTemp432 = ((iTemp69) ? 0.0f : fRec130[1] + fConst18 * std::min<float>(5e+02f, 0.001f * fTemp418));
			float fTemp433 = fTemp432 - std::floor(fTemp432);
			fRec130[0] = ((*reinterpret_cast<int*>(&fTemp433) & 2139095040) ? fTemp433 : 0.0f);
			float fTemp434 = fConst44 * fTemp425;
			float fTemp435 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.88f * (fTemp41 * (((iSlow31) ? fTemp434 : fTemp115 * ((iTemp431) ? fTemp434 : ((iTemp430 == 1) ? fConst44 * fTemp424 * fVec19[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (iTemp422 + int(fTemp417 * (1.0f - fRec128[0])) % iTemp416) % iTemp416 - iTemp415)) % iConst15))) & 2097151] : ((iTemp430 == 2) ? fConst44 * fVec19[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + (int(fRec130[0] * fTemp417) % iTemp416 + iTemp422) % iTemp416 - iTemp415)) % iConst15))) & 2097151] * fTemp424 : fConst46 * fTemp111 * fVec19[(IOTA0 - std::min<int>(iConst15, std::max<int>(0, (iConst15 + (iRec32[0] + int(fRec40[0] * fTemp417) % iTemp416 - iTemp415)) % iConst15))) & 2097151]))) * ((iTemp431) ? 1.0f : fTemp108)) * (1.0f - fTemp428) + fConst44 * fTemp428 * (0.65f * fTemp408 + 0.35f * fTemp425)) + 1.3969839e-12f * fTemp42 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp411)))));
			float fTemp436 = 6.0f * fTemp172 * std::fabs(fTemp435) + 1.0f;
			float fTemp437 = fTemp241 * fTemp435;
			float fTemp438 = fTemp437 / fTemp436;
			float fTemp439 = fTemp436 * (fTemp244 * std::fabs(fTemp438) + 1.0f);
			float fTemp440 = ((iTemp190) ? fTemp437 / fTemp439 : fRec124[1]);
			fRec124[0] = ((*reinterpret_cast<int*>(&fTemp440) & 2139095040) ? fTemp440 : 0.0f);
			float fTemp441 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec124[0] * fTemp189 + 0.5f) / fTemp189));
			float fTemp442 = ((iTemp190) ? fTemp438 : fRec131[1]);
			fRec131[0] = ((*reinterpret_cast<int*>(&fTemp442) & 2139095040) ? fTemp442 : 0.0f);
			float fTemp443 = fTemp241 * fTemp189 * fTemp435;
			float fTemp444 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp443 / fTemp439 + 0.5f) / fTemp189));
			float fTemp445 = fTemp188 * (fTemp251 * (0.65f * fTemp250 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp443 / fTemp436 + 0.5f) / fTemp189)) + fTemp248 * fTemp444 / std::max<float>(0.05f, 1.0f - fTemp244 * std::min<float>(0.999f, std::fabs(fTemp444)))) + fTemp174 * (0.65f * fTemp250 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec131[0] * fTemp189 + 0.5f) / fTemp189)) + fTemp248 * fTemp441 / std::max<float>(0.05f, 1.0f - fTemp244 * std::min<float>(0.999f, std::fabs(fTemp441)))));
			fVec21[0] = fTemp445;
			float fTemp446 = (fTemp255 * fRec123[1] - (fTemp445 + fVec21[1])) / fTemp187;
			fRec123[0] = ((*reinterpret_cast<int*>(&-fTemp446) & 2139095040) ? -fTemp446 : 0.0f);
			float fTemp447 = (fTemp257 * fRec122[1] - (fRec123[0] - fRec123[1]) / fTemp183) / fTemp185;
			fRec122[0] = ((*reinterpret_cast<int*>(&-fTemp447) & 2139095040) ? -fTemp447 : 0.0f);
			float fTemp448 = fRec123[0] + fRec122[0] * fTemp182;
			float fTemp449 = ((iTemp180) ? fTemp448 : fRec121[1]);
			fRec121[0] = ((*reinterpret_cast<int*>(&fTemp449) & 2139095040) ? fTemp449 : 0.0f);
			float fTemp450 = (fTemp261 * fRec120[1] - (fRec121[0] + fRec121[1])) / fTemp179;
			fRec120[0] = ((*reinterpret_cast<int*>(&-fTemp450) & 2139095040) ? -fTemp450 : 0.0f);
			float fTemp451 = 6.0f * fTemp117 * std::fabs(fTemp406) + 1.0f;
			float fTemp452 = fTemp283 * fTemp406;
			float fTemp453 = fTemp452 / fTemp451;
			float fTemp454 = fTemp451 * (fTemp286 * std::fabs(fTemp453) + 1.0f);
			float fTemp455 = ((iTemp281) ? fTemp452 / fTemp454 : fRec136[1]);
			fRec136[0] = ((*reinterpret_cast<int*>(&fTemp455) & 2139095040) ? fTemp455 : 0.0f);
			float fTemp456 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec136[0] * fTemp280 + 0.5f) / fTemp280));
			float fTemp457 = ((iTemp281) ? fTemp453 : fRec137[1]);
			fRec137[0] = ((*reinterpret_cast<int*>(&fTemp457) & 2139095040) ? fTemp457 : 0.0f);
			float fTemp458 = fTemp283 * fTemp280 * fTemp406;
			float fTemp459 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp458 / fTemp454 + 0.5f) / fTemp280));
			float fTemp460 = fTemp279 * (fTemp293 * (0.65f * fTemp292 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp458 / fTemp451 + 0.5f) / fTemp280)) + fTemp290 * fTemp459 / std::max<float>(0.05f, 1.0f - fTemp286 * std::min<float>(0.999f, std::fabs(fTemp459)))) + fTemp119 * (0.65f * fTemp292 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec137[0] * fTemp280 + 0.5f) / fTemp280)) + fTemp290 * fTemp456 / std::max<float>(0.05f, 1.0f - fTemp286 * std::min<float>(0.999f, std::fabs(fTemp456)))));
			fVec22[0] = fTemp460;
			float fTemp461 = (fTemp297 * fRec135[1] - (fTemp460 + fVec22[1])) / fTemp278;
			fRec135[0] = ((*reinterpret_cast<int*>(&-fTemp461) & 2139095040) ? -fTemp461 : 0.0f);
			float fTemp462 = (fTemp299 * fRec134[1] - (fRec135[0] - fRec135[1]) / fTemp274) / fTemp276;
			fRec134[0] = ((*reinterpret_cast<int*>(&-fTemp462) & 2139095040) ? -fTemp462 : 0.0f);
			float fTemp463 = fRec135[0] + fRec134[0] * fTemp273;
			float fTemp464 = ((iTemp271) ? fTemp463 : fRec133[1]);
			fRec133[0] = ((*reinterpret_cast<int*>(&fTemp464) & 2139095040) ? fTemp464 : 0.0f);
			float fTemp465 = (fTemp303 * fRec132[1] - (fRec133[0] + fRec133[1])) / fTemp270;
			fRec132[0] = ((*reinterpret_cast<int*>(&-fTemp465) & 2139095040) ? -fTemp465 : 0.0f);
			float fTemp466 = 6.0f * fTemp25 * std::fabs(fTemp376) + 1.0f;
			float fTemp467 = fTemp323 * fTemp376;
			float fTemp468 = fTemp467 / fTemp466;
			float fTemp469 = fTemp466 * (fTemp326 * std::fabs(fTemp468) + 1.0f);
			float fTemp470 = ((iTemp321) ? fTemp467 / fTemp469 : fRec142[1]);
			fRec142[0] = ((*reinterpret_cast<int*>(&fTemp470) & 2139095040) ? fTemp470 : 0.0f);
			float fTemp471 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec142[0] * fTemp320 + 0.5f) / fTemp320));
			float fTemp472 = ((iTemp321) ? fTemp468 : fRec143[1]);
			fRec143[0] = ((*reinterpret_cast<int*>(&fTemp472) & 2139095040) ? fTemp472 : 0.0f);
			float fTemp473 = fTemp323 * fTemp320 * fTemp376;
			float fTemp474 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp473 / fTemp469 + 0.5f) / fTemp320));
			float fTemp475 = fTemp319 * (fTemp333 * (0.65f * fTemp332 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp473 / fTemp466 + 0.5f) / fTemp320)) + fTemp330 * fTemp474 / std::max<float>(0.05f, 1.0f - fTemp326 * std::min<float>(0.999f, std::fabs(fTemp474)))) + fTemp27 * (0.65f * fTemp332 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec143[0] * fTemp320 + 0.5f) / fTemp320)) + fTemp330 * fTemp471 / std::max<float>(0.05f, 1.0f - fTemp326 * std::min<float>(0.999f, std::fabs(fTemp471)))));
			fVec23[0] = fTemp475;
			float fTemp476 = (fTemp337 * fRec141[1] - (fTemp475 + fVec23[1])) / fTemp318;
			fRec141[0] = ((*reinterpret_cast<int*>(&-fTemp476) & 2139095040) ? -fTemp476 : 0.0f);
			float fTemp477 = (fTemp339 * fRec140[1] - (fRec141[0] - fRec141[1]) / fTemp314) / fTemp316;
			fRec140[0] = ((*reinterpret_cast<int*>(&-fTemp477) & 2139095040) ? -fTemp477 : 0.0f);
			float fTemp478 = fRec141[0] + fRec140[0] * fTemp313;
			float fTemp479 = ((iTemp311) ? fTemp478 : fRec139[1]);
			fRec139[0] = ((*reinterpret_cast<int*>(&fTemp479) & 2139095040) ? fTemp479 : 0.0f);
			float fTemp480 = (fTemp343 * fRec138[1] - (fRec139[0] + fRec139[1])) / fTemp310;
			fRec138[0] = ((*reinterpret_cast<int*>(&-fTemp480) & 2139095040) ? -fTemp480 : 0.0f);
			fVec24[IOTA0 & 2047] = fTemp28 * (fTemp345 * (fTemp323 * fTemp332 * fTemp376 / fTemp466 + fTemp25 * fTemp478) + fRec138[0] * fTemp25 * fTemp306) + fTemp120 * (fTemp305 * (fTemp283 * fTemp292 * fTemp406 / fTemp451 + fTemp117 * fTemp463) + fRec132[0] * fTemp117 * fTemp266) + fTemp265 * fTemp435 + fTemp264 * (fTemp263 * (fTemp241 * fTemp250 * fTemp435 / fTemp436 + fTemp172 * fTemp448) + fRec120[0] * fTemp172 * fTemp175) + fTemp121 * fTemp406 + fTemp29 * fTemp376;
			float fTemp481 = fVec24[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp18))) & 2047];
			float fTemp482 = 1.0f - fTemp23;
			fVec25[IOTA0 & 4095] = fTemp482 * fTemp481 + fTemp346 * fTemp22 * fTemp10;
			float fTemp483 = ((iTemp43) ? fTemp16 : fRec145[1]);
			fRec145[0] = ((*reinterpret_cast<int*>(&fTemp483) & 2139095040) ? fTemp483 : 0.0f);
			float fTemp484 = fConst48 * (fRec145[0] + -0.5f) + fConst47 * fRec144[1];
			fRec144[0] = ((*reinterpret_cast<int*>(&fTemp484) & 2139095040) ? fTemp484 : 0.0f);
			float fTemp485 = std::max<float>(0.0f, fRec2[0] + -0.32f);
			int iTemp486 = std::max<int>(0, int(fConst49 * fRec3[0] * std::fabs(fRec144[0]) * fTemp485 * (5.0f * fTemp4 + 0.4f)));
			float fTemp487 = fVec25[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec144[0] < 0.0f) ? iTemp486 : 0)))) & 4095];
			fVec26[IOTA0 & 4095] = fTemp346 * fTemp482 + fTemp23 * fTemp481;
			float fTemp488 = fVec26[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec144[0] >= 0.0f) ? iTemp486 : 0)))) & 4095];
			float fTemp489 = fTemp488 + fTemp487;
			float fTemp490 = (0.5f * (fTemp488 - fTemp487) + 0.7352941f * fRec3[0] * fRec144[0] * fTemp485 * fTemp489 * (0.16f * fTemp4 + 0.06f)) * (1.0f - 1.2f * fTemp23);
			float fTemp491 = 0.5f * fTemp489;
			fVec27[IOTA0 & 511] = (fTemp9 + fRec3[0] * (fTemp491 + fTemp490 - fTemp9)) * fTemp8;
			float fTemp492 = fVec27[(IOTA0 - iConst50) & 511];
			float fTemp493 = std::max<float>(fRec1[1] * fTemp6, std::fabs(fTemp492));
			fRec1[0] = ((*reinterpret_cast<int*>(&fTemp493) & 2139095040) ? fTemp493 : 0.0f);
			float fTemp494 = 0.08f * fTemp5;
			float fTemp495 = std::max<float>(1e-06f, fTemp494);
			float fTemp496 = std::min<float>(1.0f, std::max<float>(0.0f, fRec1[0] + fTemp494 + -0.98f) / fTemp495) * (0.98f / std::max<float>(0.98f, fRec1[0]) + -1.0f) + 1.0f;
			float fTemp497 = Freeze95DSP_faustpower2_f(fTemp492) * Freeze95DSP_faustpower2_f(fTemp496);
			float fTemp498 = fTemp492 * fTemp496 * (fTemp497 + 27.0f) / (9.0f * fTemp497 + 27.0f);
			fVec28[0] = fTemp498;
			float fTemp499 = 0.995f * fRec0[1] + fTemp498 - fVec28[1];
			fRec0[0] = ((*reinterpret_cast<int*>(&fTemp499) & 2139095040) ? fTemp499 : 0.0f);
			output0[i0] = FAUSTFLOAT(fRec0[0]);
			fVec29[IOTA0 & 511] = fTemp8 * (fTemp347 + fRec3[0] * (fTemp491 - (fTemp347 + fTemp490)));
			float fTemp500 = fVec29[(IOTA0 - iConst50) & 511];
			float fTemp501 = std::max<float>(fTemp6 * fRec147[1], std::fabs(fTemp500));
			fRec147[0] = ((*reinterpret_cast<int*>(&fTemp501) & 2139095040) ? fTemp501 : 0.0f);
			float fTemp502 = std::min<float>(1.0f, std::max<float>(0.0f, fRec147[0] + fTemp494 + -0.98f) / fTemp495) * (0.98f / std::max<float>(0.98f, fRec147[0]) + -1.0f) + 1.0f;
			float fTemp503 = Freeze95DSP_faustpower2_f(fTemp500) * Freeze95DSP_faustpower2_f(fTemp502);
			float fTemp504 = fTemp500 * fTemp502 * (fTemp503 + 27.0f) / (9.0f * fTemp503 + 27.0f);
			fVec30[0] = fTemp504;
			float fTemp505 = 0.995f * fRec146[1] + fTemp504 - fVec30[1];
			fRec146[0] = ((*reinterpret_cast<int*>(&fTemp505) & 2139095040) ? fTemp505 : 0.0f);
			output1[i0] = FAUSTFLOAT(fRec146[0]);
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			iRec5[1] = iRec5[0];
			iVec2[1] = iVec2[0];
			fVec3[1] = fVec3[0];
			iVec4[1] = iVec4[0];
			iRec6[1] = iRec6[0];
			fRec4[1] = fRec4[0];
			iRec8[1] = iRec8[0];
			fRec7[1] = fRec7[0];
			iRec10[1] = iRec10[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			iRec15[1] = iRec15[0];
			fRec14[1] = fRec14[0];
			iRec16[1] = iRec16[0];
			iRec17[1] = iRec17[0];
			iRec20[1] = iRec20[0];
			iRec21[1] = iRec21[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec22[1] = fRec22[0];
			iRec24[1] = iRec24[0];
			fRec23[1] = fRec23[0];
			iRec27[1] = iRec27[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			iRec29[1] = iRec29[0];
			fRec28[1] = fRec28[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			iRec32[1] = iRec32[0];
			iRec34[1] = iRec34[0];
			fRec33[1] = fRec33[0];
			fRec35[1] = fRec35[0];
			iVec6[1] = iVec6[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			iRec47[1] = iRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec48[1] = fRec48[0];
			iRec50[1] = iRec50[0];
			fRec49[1] = fRec49[0];
			iRec53[1] = iRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			iRec55[1] = iRec55[0];
			fRec54[1] = fRec54[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			iRec59[1] = iRec59[0];
			fRec58[1] = fRec58[0];
			fRec60[1] = fRec60[0];
			iVec8[1] = iVec8[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
			iRec65[1] = iRec65[0];
			iRec69[1] = iRec69[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			iRec74[1] = iRec74[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec75[1] = fRec75[0];
			iRec77[1] = iRec77[0];
			fRec76[1] = fRec76[0];
			iRec80[1] = iRec80[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			iRec82[1] = iRec82[0];
			fRec81[1] = fRec81[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			iRec86[1] = iRec86[0];
			fRec85[1] = fRec85[0];
			fRec87[1] = fRec87[0];
			iVec10[1] = iVec10[0];
			fRec88[1] = fRec88[0];
			fRec89[1] = fRec89[0];
			fRec68[1] = fRec68[0];
			fRec90[1] = fRec90[0];
			fVec11[1] = fVec11[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			iRec93[1] = iRec93[0];
			iRec97[1] = iRec97[0];
			fRec96[1] = fRec96[0];
			fRec98[1] = fRec98[0];
			fVec12[1] = fVec12[0];
			fRec95[1] = fRec95[0];
			fRec94[1] = fRec94[0];
			fRec92[1] = fRec92[0];
			fRec91[1] = fRec91[0];
			iRec101[1] = iRec101[0];
			iRec105[1] = iRec105[0];
			fRec104[1] = fRec104[0];
			fRec106[1] = fRec106[0];
			fVec13[1] = fVec13[0];
			fRec103[1] = fRec103[0];
			fRec102[1] = fRec102[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec108[2] = fRec108[1];
			fRec108[1] = fRec108[0];
			fRec107[1] = fRec107[0];
			fRec109[1] = fRec109[0];
			fRec110[1] = fRec110[0];
			iVec16[1] = iVec16[0];
			fRec111[1] = fRec111[0];
			fRec112[1] = fRec112[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fRec113[1] = fRec113[0];
			fRec116[1] = fRec116[0];
			fRec117[1] = fRec117[0];
			iVec18[1] = iVec18[0];
			fRec118[1] = fRec118[0];
			fRec119[1] = fRec119[0];
			fRec126[2] = fRec126[1];
			fRec126[1] = fRec126[0];
			fRec125[1] = fRec125[0];
			fRec127[1] = fRec127[0];
			fRec128[1] = fRec128[0];
			iVec20[1] = iVec20[0];
			fRec129[1] = fRec129[0];
			fRec130[1] = fRec130[0];
			fRec124[1] = fRec124[0];
			fRec131[1] = fRec131[0];
			fVec21[1] = fVec21[0];
			fRec123[1] = fRec123[0];
			fRec122[1] = fRec122[0];
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			fRec136[1] = fRec136[0];
			fRec137[1] = fRec137[0];
			fVec22[1] = fVec22[0];
			fRec135[1] = fRec135[0];
			fRec134[1] = fRec134[0];
			fRec133[1] = fRec133[0];
			fRec132[1] = fRec132[0];
			fRec142[1] = fRec142[0];
			fRec143[1] = fRec143[0];
			fVec23[1] = fVec23[0];
			fRec141[1] = fRec141[0];
			fRec140[1] = fRec140[0];
			fRec139[1] = fRec139[0];
			fRec138[1] = fRec138[0];
			fRec145[1] = fRec145[0];
			fRec144[1] = fRec144[0];
			fRec1[1] = fRec1[0];
			fVec28[1] = fVec28[0];
			fRec0[1] = fRec0[0];
			fRec147[1] = fRec147[0];
			fVec30[1] = fVec30[0];
			fRec146[1] = fRec146[0];
		}
	}

};

#endif
