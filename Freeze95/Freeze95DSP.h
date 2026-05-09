/* ------------------------------------------------------------
name: "Freeze95"
version: "1.1"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2
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
	float fConst4;
	FAUSTFLOAT fCheckbox0;
	float fConst5;
	float fSlow2;
	float fRec3[2];
	float fConst6;
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
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec17[2];
	float fConst11;
	float fRec18[2];
	float fConst12;
	float fRec13[2];
	float fRec14[2];
	float fConst13;
	float fConst14;
	float fConst15;
	float fRec21[2];
	float fConst16;
	float fRec22[2];
	float fConst17;
	float fRec25[2];
	float fRec26[2];
	int IOTA0;
	float fVec5[2097152];
	float fRec12[2];
	float fSlow5;
	float fSlow6;
	float fSlow7;
	int iSlow8;
	float fSlow9;
	int iRec30[2];
	float fRec29[2];
	int iRec31[2];
	int iRec32[2];
	int iConst18;
	int iRec35[2];
	int iRec36[2];
	float fRec34[2];
	float fRec33[2];
	float fConst19;
	float fRec37[2];
	int iRec39[2];
	float fRec38[2];
	float fSlow10;
	float fConst20;
	float fSlow11;
	int iRec42[2];
	float fRec41[2];
	float fRec40[2];
	int iRec44[2];
	float fRec43[2];
	float fSlow12;
	float fSlow13;
	float fConst21;
	FAUSTFLOAT fCheckbox1;
	int iSlow14;
	float fRec46[2];
	float fSlow15;
	float fSlow16;
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
	float fRec45[2];
	int iRec47[2];
	int iRec49[2];
	float fRec48[2];
	float fRec50[2];
	int iConst22;
	float fConst23;
	int iVec6[2];
	float fConst24;
	float fRec51[2];
	int iSlow31;
	float fRec53[2];
	float fRec54[2];
	float fRec55[2];
	float fRec52[2];
	int iVec7[2];
	float fRec56[2];
	float fRec57[2];
	float fConst25;
	float fConst26;
	float fRec58[2];
	float fRec60[2];
	float fRec61[2];
	float fRec62[2097152];
	float fRec59[2];
	float fRec63[2];
	int iRec65[2];
	float fRec64[2];
	int iRec68[2];
	float fRec67[2];
	float fRec66[2];
	float fSlow32;
	int iRec72[2];
	float fRec71[2];
	float fRec70[2];
	float fRec73[2];
	float fRec69[2];
	float fSlow33;
	int iRec75[2];
	float fRec74[2];
	int iRec77[2];
	float fRec76[2];
	float fRec78[2];
	int iVec8[2];
	float fRec79[2];
	float fRec80[2];
	int iVec9[2];
	float fRec81[2];
	float fRec82[2];
	float fConst27;
	int iRec85[2];
	int iRec89[2];
	float fRec94[2];
	float fRec95[2];
	float fRec91[2];
	float fRec92[2];
	float fVec10[2097152];
	float fRec90[2];
	int iRec99[2];
	float fRec98[2];
	float fRec97[2];
	float fRec100[2];
	int iRec102[2];
	float fRec101[2];
	float fConst28;
	float fSlow34;
	int iRec105[2];
	float fRec104[2];
	float fRec103[2];
	int iRec107[2];
	float fRec106[2];
	float fRec109[2];
	float fRec108[2];
	int iRec111[2];
	float fRec110[2];
	float fRec112[2];
	int iVec11[2];
	float fRec113[2];
	float fRec114[2];
	int iVec12[2];
	float fRec115[2];
	float fRec116[2];
	float fRec88[2];
	float fRec117[2];
	float fVec13[2];
	float fRec87[2];
	float fRec86[2];
	float fRec84[2];
	float fRec83[2];
	int iRec120[2];
	int iRec124[2];
	float fRec123[2];
	float fRec125[2];
	float fVec14[2];
	float fRec122[2];
	float fRec121[2];
	float fRec119[2];
	float fRec118[2];
	int iRec128[2];
	int iRec132[2];
	float fRec131[2];
	float fRec133[2];
	float fVec15[2];
	float fRec130[2];
	float fRec129[2];
	float fRec127[2];
	float fRec126[2];
	float fVec16[2048];
	float fRec139[2];
	float fRec140[2];
	float fRec135[2];
	float fRec136[2];
	float fRec143[2];
	float fRec144[2];
	float fRec147[2];
	float fRec148[2];
	float fVec17[2097152];
	float fRec134[2];
	float fRec151[2];
	float fRec152[2];
	int iVec18[2];
	float fRec153[2];
	float fRec154[2];
	int iVec19[2];
	float fRec155[2];
	float fRec156[2];
	float fRec158[2];
	float fRec159[2];
	float fRec160[2097152];
	float fRec157[2];
	float fRec161[2];
	float fRec162[2];
	int iVec20[2];
	float fRec163[2];
	float fRec164[2];
	int iVec21[2];
	float fRec165[2];
	float fRec166[2];
	float fRec176[2];
	float fRec177[2];
	float fRec173[2];
	float fRec174[2];
	float fVec22[2097152];
	float fRec172[2];
	float fRec179[2];
	float fRec180[2];
	int iVec23[2];
	float fRec181[2];
	float fRec182[2];
	int iVec24[2];
	float fRec183[2];
	float fRec184[2];
	float fRec171[2];
	float fRec185[2];
	float fVec25[2];
	float fRec170[2];
	float fRec169[2];
	float fRec168[2];
	float fRec167[2];
	float fRec190[2];
	float fRec191[2];
	float fVec26[2];
	float fRec189[2];
	float fRec188[2];
	float fRec187[2];
	float fRec186[2];
	float fRec196[2];
	float fRec197[2];
	float fVec27[2];
	float fRec195[2];
	float fRec194[2];
	float fRec193[2];
	float fRec192[2];
	float fVec28[2048];
	float fVec29[4096];
	float fConst29;
	float fRec199[2];
	float fConst30;
	float fRec198[2];
	float fConst31;
	float fVec30[4096];
	float fVec31[512];
	int iConst32;
	float fRec1[2];
	float fVec32[2];
	float fRec0[2];
	float fVec33[512];
	float fRec201[2];
	float fVec34[2];
	float fRec200[2];
	
 public:
	Freeze95DSP() {
	}
	
	Freeze95DSP(const Freeze95DSP&) = default;
	
	virtual ~Freeze95DSP() = default;
	
	Freeze95DSP& operator=(const Freeze95DSP&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/peakhold:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("description", "Band-split quantum stutter/freeze with host sync, layered special modes, and the newer lo-fi control behavior.");
		m->declare("filename", "Freeze95DSP.dsp");
		m->declare("filters.lib/crossover2LR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/crossover2LR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/crossover2LR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/crossover3LR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/crossover3LR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/crossover3LR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpassLR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/highpassLR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/highpassLR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpassLR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/lowpassLR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/lowpassLR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/svf:author", "Oleg Nesterov");
		m->declare("filters.lib/svf:copyright", "Copyright (C) 2020 Oleg Nesterov <oleg@redhat.com>");
		m->declare("filters.lib/svf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "Freeze95");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.5.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.7.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = std::exp(-(1.0f / std::max<float>(1.0f, 0.035f * fConst0)));
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.001f * fConst0;
		fConst4 = std::exp(-(1.0f / std::max<float>(1.0f, 0.008f * fConst0)));
		fConst5 = 1.0f - fConst4;
		fConst6 = 0.0025f * fConst0;
		fConst7 = 0.02f * fConst0;
		fConst8 = std::tan(691.1504f / fConst0);
		fConst9 = fConst8 * (fConst8 + 1.4142135f) + 1.0f;
		fConst10 = 2.0f / fConst9;
		fConst11 = fConst8 / fConst9;
		fConst12 = 1.0f / fConst9;
		fConst13 = std::tan(6911.504f / fConst0);
		fConst14 = fConst13 * (fConst13 + 1.4142135f) + 1.0f;
		fConst15 = 2.0f / fConst14;
		fConst16 = fConst13 / fConst14;
		fConst17 = 1.0f / fConst14;
		iConst18 = std::max<int>(256, static_cast<int>(6.5f * fConst0));
		fConst19 = 0.005f * fConst0;
		fConst20 = 0.00086f * fConst0;
		fConst21 = 1.0f / fConst0;
		iConst22 = static_cast<int>(0.0015f * fConst0);
		fConst23 = static_cast<float>(std::max<int>(32, iConst22));
		fConst24 = std::exp(-(1.0f / std::max<float>(1.0f, 0.14f * fConst0)));
		fConst25 = std::exp(-(1.0f / std::max<float>(1.0f, fConst19)));
		fConst26 = 1.0f - fConst25;
		fConst27 = 3.1415927f / fConst0;
		fConst28 = 0.00112f * fConst0;
		fConst29 = std::exp(-(1.0f / std::max<float>(1.0f, 0.6f * fConst0)));
		fConst30 = 0.9f * (1.0f - fConst29);
		fConst31 = 0.0014705883f * fConst0;
		iConst32 = std::min<int>(4096, std::max<int>(0, std::max<int>(1, iConst22)));
	}
	
	void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.55f);
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fCheckbox1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(1.2e+02f);
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
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec18[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec13[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec22[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec25[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec26[l22] = 0.0f;
		}
		IOTA0 = 0;
		for (int l23 = 0; l23 < 2097152; l23 = l23 + 1) {
			fVec5[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec12[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			iRec30[l25] = 0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec29[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec31[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iRec32[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			iRec35[l29] = 0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			iRec36[l30] = 0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec34[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec33[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec37[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			iRec39[l34] = 0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec38[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			iRec42[l36] = 0;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec41[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec40[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			iRec44[l39] = 0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec43[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec46[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec45[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			iRec47[l43] = 0;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			iRec49[l44] = 0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec48[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec50[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			iVec6[l47] = 0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec51[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec53[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec54[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec55[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec52[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			iVec7[l53] = 0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec56[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec57[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec58[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec60[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec61[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2097152; l59 = l59 + 1) {
			fRec62[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec59[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			iRec65[l62] = 0;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec64[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			iRec68[l64] = 0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec67[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec66[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			iRec72[l67] = 0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec71[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec70[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec73[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec69[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			iRec75[l72] = 0;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec74[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			iRec77[l74] = 0;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec76[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec78[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			iVec8[l77] = 0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec79[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec80[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			iVec9[l80] = 0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec81[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec82[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			iRec85[l83] = 0;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			iRec89[l84] = 0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec94[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec95[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec91[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec92[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2097152; l89 = l89 + 1) {
			fVec10[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec90[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			iRec99[l91] = 0;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec98[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec97[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec100[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			iRec102[l95] = 0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec101[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			iRec105[l97] = 0;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec104[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec103[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			iRec107[l100] = 0;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec106[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec109[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec108[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			iRec111[l104] = 0;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec110[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec112[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			iVec11[l107] = 0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec113[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec114[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			iVec12[l110] = 0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec115[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec116[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec88[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fRec117[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fVec13[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec87[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec86[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec84[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec83[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			iRec120[l120] = 0;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			iRec124[l121] = 0;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec123[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec125[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fVec14[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec122[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec121[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec119[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec118[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			iRec128[l129] = 0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			iRec132[l130] = 0;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec131[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec133[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fVec15[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec130[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec129[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec127[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec126[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2048; l138 = l138 + 1) {
			fVec16[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec139[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec140[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec135[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
			fRec136[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec143[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec144[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec147[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec148[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 2097152; l147 = l147 + 1) {
			fVec17[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec134[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec151[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fRec152[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			iVec18[l151] = 0;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec153[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fRec154[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			iVec19[l154] = 0;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec155[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec156[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec158[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec159[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2097152; l159 = l159 + 1) {
			fRec160[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec157[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec161[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fRec162[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			iVec20[l163] = 0;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec163[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec164[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 2; l166 = l166 + 1) {
			iVec21[l166] = 0;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fRec165[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec166[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec176[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
			fRec177[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fRec173[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec174[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2097152; l173 = l173 + 1) {
			fVec22[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec172[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
			fRec179[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec180[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			iVec23[l177] = 0;
		}
		for (int l178 = 0; l178 < 2; l178 = l178 + 1) {
			fRec181[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fRec182[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			iVec24[l180] = 0;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec183[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
			fRec184[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
			fRec171[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec185[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fVec25[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fRec170[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 2; l187 = l187 + 1) {
			fRec169[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
			fRec168[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec167[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 2; l190 = l190 + 1) {
			fRec190[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
			fRec191[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fVec26[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec189[l193] = 0.0f;
		}
		for (int l194 = 0; l194 < 2; l194 = l194 + 1) {
			fRec188[l194] = 0.0f;
		}
		for (int l195 = 0; l195 < 2; l195 = l195 + 1) {
			fRec187[l195] = 0.0f;
		}
		for (int l196 = 0; l196 < 2; l196 = l196 + 1) {
			fRec186[l196] = 0.0f;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec196[l197] = 0.0f;
		}
		for (int l198 = 0; l198 < 2; l198 = l198 + 1) {
			fRec197[l198] = 0.0f;
		}
		for (int l199 = 0; l199 < 2; l199 = l199 + 1) {
			fVec27[l199] = 0.0f;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec195[l200] = 0.0f;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec194[l201] = 0.0f;
		}
		for (int l202 = 0; l202 < 2; l202 = l202 + 1) {
			fRec193[l202] = 0.0f;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fRec192[l203] = 0.0f;
		}
		for (int l204 = 0; l204 < 2048; l204 = l204 + 1) {
			fVec28[l204] = 0.0f;
		}
		for (int l205 = 0; l205 < 4096; l205 = l205 + 1) {
			fVec29[l205] = 0.0f;
		}
		for (int l206 = 0; l206 < 2; l206 = l206 + 1) {
			fRec199[l206] = 0.0f;
		}
		for (int l207 = 0; l207 < 2; l207 = l207 + 1) {
			fRec198[l207] = 0.0f;
		}
		for (int l208 = 0; l208 < 4096; l208 = l208 + 1) {
			fVec30[l208] = 0.0f;
		}
		for (int l209 = 0; l209 < 512; l209 = l209 + 1) {
			fVec31[l209] = 0.0f;
		}
		for (int l210 = 0; l210 < 2; l210 = l210 + 1) {
			fRec1[l210] = 0.0f;
		}
		for (int l211 = 0; l211 < 2; l211 = l211 + 1) {
			fVec32[l211] = 0.0f;
		}
		for (int l212 = 0; l212 < 2; l212 = l212 + 1) {
			fRec0[l212] = 0.0f;
		}
		for (int l213 = 0; l213 < 512; l213 = l213 + 1) {
			fVec33[l213] = 0.0f;
		}
		for (int l214 = 0; l214 < 2; l214 = l214 + 1) {
			fRec201[l214] = 0.0f;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fVec34[l215] = 0.0f;
		}
		for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
			fRec200[l216] = 0.0f;
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
		return new Freeze95DSP(*this);
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
		fSlow0 = static_cast<float>(fHslider0);
		fSlow1 = fConst2 * fSlow0;
		fSlow2 = fConst5 * (1.0f - static_cast<float>(fCheckbox0));
		fSlow3 = static_cast<float>(fHslider1);
		fSlow4 = fConst2 * fSlow3;
		fSlow5 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 8.333333f * fSlow0));
		fSlow6 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 5.0f * fSlow0));
		fSlow7 = std::max<float>(fSlow5, fSlow6);
		iSlow8 = fSlow7 > 0.05f;
		fSlow9 = 0.75f * (1.0f - 0.9f * fSlow5) * (1.0f - 0.9f * fSlow6);
		fSlow10 = 0.4f * fSlow6 + 1.0f;
		fSlow11 = fConst20 * fSlow10;
		fSlow12 = 22.0f * (1.0f - 0.85f * fSlow6);
		fSlow13 = 1.0f - 0.35f * fSlow6;
		iSlow14 = static_cast<float>(fCheckbox1) > 0.0f;
		fSlow15 = 0.7f * fSlow6;
		fSlow16 = 1.0f - fSlow6;
		fSlow17 = std::max<float>(2e+01f, static_cast<float>(fHslider2));
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
		fSlow33 = fSlow12 + 3.0f;
		fSlow34 = fConst28 * fSlow10;
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
			float fTemp1 = std::pow(fRec2[0], 1.3f);
			float fTemp2 = 1.0f - fTemp1;
			float fTemp3 = std::exp(-(1.0f / std::max<float>(1.0f, fConst3 * (12.0f * fTemp2 + 8.0f))));
			float fTemp4 = 0.04f * fTemp2;
			float fTemp5 = fTemp4 + 0.94f;
			float fTemp6 = 0.06f * fRec2[0] + 0.02f;
			float fTemp7 = fSlow2 + fConst4 * fRec3[1];
			fRec3[0] = ((*reinterpret_cast<int*>(&fTemp7) & 2139095040) ? fTemp7 : 0.0f);
			float fTemp8 = static_cast<float>(std::max<int>(0, std::min<int>(1023, static_cast<int>(fConst6 * fRec3[0] * (1.0f - 0.7f * fTemp2)))));
			iRec5[0] = (iRec5[1] + 1) % std::max<int>(1, static_cast<int>(fConst3 * std::max<float>(6e+01f, 9e+03f - 8.9e+03f * fTemp1)));
			int iTemp9 = std::fabs(fSlow0 - fVec1[1]) > 0.004f;
			iVec2[0] = iTemp9;
			fVec3[0] = fSlow3;
			int iTemp10 = std::fabs(fSlow3 - fVec3[1]) > 0.004f;
			iVec4[0] = iTemp10;
			int iTemp11 = std::max<int>(iRec5[0] == 0, std::max<int>(std::abs(iTemp9 - iVec2[1]), std::abs(iTemp10 - iVec4[1])));
			iRec6[0] = 1103515245 * iRec6[1] + 444;
			float fTemp12 = 4.656613e-10f * static_cast<float>((iRec6[0] + 45678) % 2147483647);
			float fTemp13 = ((iTemp11) ? fTemp12 : fRec4[1]);
			fRec4[0] = ((*reinterpret_cast<int*>(&fTemp13) & 2139095040) ? fTemp13 : 0.0f);
			int iTemp14 = static_cast<int>(fRec4[0] * fTemp8);
			iRec8[0] = 1103515245 * iRec8[1] + 222;
			float fTemp15 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec8[0] + 23456) % 2147483647) : fRec7[1]);
			fRec7[0] = ((*reinterpret_cast<int*>(&fTemp15) & 2139095040) ? fTemp15 : 0.0f);
			int iTemp16 = static_cast<int>(fRec7[0] * fTemp8);
			iRec10[0] = 1103515245 * iRec10[1] + 111;
			float fTemp17 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec10[0] + 12345) % 2147483647) : fRec9[1]);
			fRec9[0] = ((*reinterpret_cast<int*>(&fTemp17) & 2139095040) ? fTemp17 : 0.0f);
			float fTemp18 = static_cast<float>((fRec9[0] < 0.05f) & (((iTemp16 != iTemp14) | (iTemp16 > 0)) | (iTemp14 > 0)));
			float fTemp19 = fTemp18 * fTemp6;
			float fTemp20 = fSlow4 + fConst1 * fRec11[1];
			fRec11[0] = ((*reinterpret_cast<int*>(&fTemp20) & 2139095040) ? fTemp20 : 0.0f);
			float fTemp21 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, 1.2f * fRec11[0])));
			float fTemp22 = 3.0f - 2.0f * fTemp21;
			float fTemp23 = Freeze95DSP_faustpower2_f(fTemp21) * fTemp22;
			float fTemp24 = fTemp21 * (0.45f * fTemp23 + 0.55f);
			float fTemp25 = 1.0f - fTemp24;
			float fTemp26 = static_cast<float>(input0[i0]);
			float fTemp27 = fRec17[1] + fConst8 * (fTemp26 - fRec18[1]);
			float fTemp28 = fConst10 * fTemp27 - fRec17[1];
			fRec17[0] = ((*reinterpret_cast<int*>(&fTemp28) & 2139095040) ? fTemp28 : 0.0f);
			float fTemp29 = fRec18[1] + fConst11 * fTemp27;
			float fTemp30 = 2.0f * fTemp29 - fRec18[1];
			fRec18[0] = ((*reinterpret_cast<int*>(&fTemp30) & 2139095040) ? fTemp30 : 0.0f);
			float fTemp31 = fConst12 * fTemp27;
			float fRec19 = ((*reinterpret_cast<int*>(&fTemp31) & 2139095040) ? fTemp31 : 0.0f);
			float fRec20 = ((*reinterpret_cast<int*>(&fTemp29) & 2139095040) ? fTemp29 : 0.0f);
			float fTemp32 = fRec20 + 1.4142135f * fRec19;
			float fTemp33 = fRec13[1] + fConst8 * (fTemp26 - (fTemp32 + fRec14[1]));
			float fTemp34 = fConst10 * fTemp33 - fRec13[1];
			fRec13[0] = ((*reinterpret_cast<int*>(&fTemp34) & 2139095040) ? fTemp34 : 0.0f);
			float fTemp35 = fRec14[1] + fConst11 * fTemp33;
			float fTemp36 = 2.0f * fTemp35 - fRec14[1];
			fRec14[0] = ((*reinterpret_cast<int*>(&fTemp36) & 2139095040) ? fTemp36 : 0.0f);
			float fTemp37 = fConst12 * fTemp33;
			float fRec15 = ((*reinterpret_cast<int*>(&fTemp37) & 2139095040) ? fTemp37 : 0.0f);
			float fRec16 = ((*reinterpret_cast<int*>(&fTemp35) & 2139095040) ? fTemp35 : 0.0f);
			float fTemp38 = fRec20 + fRec16;
			float fTemp39 = fRec19 + fRec15;
			float fTemp40 = 1.4142135f * fTemp39 + fTemp38;
			float fTemp41 = fRec21[1] + fConst13 * (fTemp26 - (fTemp40 + fRec22[1]));
			float fTemp42 = fConst15 * fTemp41 - fRec21[1];
			fRec21[0] = ((*reinterpret_cast<int*>(&fTemp42) & 2139095040) ? fTemp42 : 0.0f);
			float fTemp43 = fRec22[1] + fConst16 * fTemp41;
			float fTemp44 = 2.0f * fTemp43 - fRec22[1];
			fRec22[0] = ((*reinterpret_cast<int*>(&fTemp44) & 2139095040) ? fTemp44 : 0.0f);
			float fTemp45 = fConst17 * fTemp41;
			float fRec23 = ((*reinterpret_cast<int*>(&fTemp45) & 2139095040) ? fTemp45 : 0.0f);
			float fRec24 = ((*reinterpret_cast<int*>(&fTemp43) & 2139095040) ? fTemp43 : 0.0f);
			float fTemp46 = fRec24 + fTemp38;
			float fTemp47 = fRec23 + fTemp39;
			float fTemp48 = 1.4142135f * fTemp47 + fTemp46;
			float fTemp49 = fRec25[1] + fConst13 * (fTemp26 - (fTemp48 + fRec26[1]));
			float fTemp50 = fConst15 * fTemp49 - fRec25[1];
			fRec25[0] = ((*reinterpret_cast<int*>(&fTemp50) & 2139095040) ? fTemp50 : 0.0f);
			float fTemp51 = fRec26[1] + fConst16 * fTemp49;
			float fTemp52 = 2.0f * fTemp51 - fRec26[1];
			fRec26[0] = ((*reinterpret_cast<int*>(&fTemp52) & 2139095040) ? fTemp52 : 0.0f);
			float fTemp53 = fConst17 * fTemp49;
			float fRec27 = ((*reinterpret_cast<int*>(&fTemp53) & 2139095040) ? fTemp53 : 0.0f);
			float fRec28 = ((*reinterpret_cast<int*>(&fTemp51) & 2139095040) ? fTemp51 : 0.0f);
			float fTemp54 = fTemp26 - (1.4142135f * (fRec27 + fTemp47) + fRec28 + fTemp46);
			fVec5[IOTA0 & 2097151] = fTemp54;
			float fTemp55 = std::fabs(fTemp54);
			float fTemp56 = std::max<float>(fConst7 * fRec12[1], fTemp55);
			fRec12[0] = ((*reinterpret_cast<int*>(&fTemp56) & 2139095040) ? fTemp56 : 0.0f);
			float fTemp57 = std::min<float>(1.0f, 4.0f * fRec12[0]);
			float fTemp58 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, fRec2[0] + -0.3f)), 1.5f);
			float fTemp59 = 0.65f * fTemp1 + 0.35f;
			iRec30[0] = 1103515245 * iRec30[1] + 555;
			float fTemp60 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec30[0] + 56789) % 2147483647) : fRec29[1]);
			fRec29[0] = ((*reinterpret_cast<int*>(&fTemp60) & 2139095040) ? fTemp60 : 0.0f);
			int iTemp61 = std::max<int>(1, static_cast<int>(fConst3 * (2.5e+02f * fTemp58 + 12.0f)));
			float fTemp62 = std::min<float>(0.49f, static_cast<float>(std::max<int>(1, static_cast<int>(fConst3 * (2.2f - 1.6f * fTemp1)))) / static_cast<float>(iTemp61));
			iRec31[0] = (iVec0[1] + iRec31[1]) % iTemp61;
			float fTemp63 = static_cast<float>(iRec31[0]);
			float fTemp64 = ((static_cast<int>(((iSlow8) ? 0.0f : static_cast<float>(fRec29[0] < (fSlow9 * fTemp59 * fTemp58 * (0.8f * fTemp58 + 1.0f)))))) ? ((fTemp63 < fTemp62) ? 1.0f - fTemp63 / fTemp62 : ((fTemp63 < (0.5f - fTemp62)) ? 0.0f : ((fTemp63 < 0.5f) ? (fTemp63 + fTemp62 + -0.5f) / fTemp62 : 1.0f))) : 1.0f);
			iRec32[0] = 1103515245 * iRec32[1] + 12345;
			float fTemp65 = Freeze95DSP_faustpower2_f(fRec2[0]) * static_cast<float>(iRec32[0]) * (0.75f * fTemp1 + 0.25f) * (1.0f - fTemp64);
			iRec35[0] = (iRec35[1] + 1) % std::max<int>(1, static_cast<int>(fConst3 * (1.5e+04f * fTemp2 + 5e+03f)));
			int iTemp66 = iRec35[0] == 0;
			iRec36[0] = 1103515245 * iRec36[1] + 3033;
			float fTemp67 = 4.656613e-10f * static_cast<float>((iRec36[0] + 33467) % 2147483647);
			float fTemp68 = ((iTemp66) ? fTemp67 : fRec34[1]);
			fRec34[0] = ((*reinterpret_cast<int*>(&fTemp68) & 2139095040) ? fTemp68 : 0.0f);
			float fTemp69 = 0.97f * fRec33[1] + 0.08f * (fRec34[0] + -0.5f);
			fRec33[0] = ((*reinterpret_cast<int*>(&fTemp69) & 2139095040) ? fTemp69 : 0.0f);
			float fTemp70 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec33[0])) + 1.0f;
			float fTemp71 = std::max<float>(fConst19 * fRec37[1], fTemp55);
			fRec37[0] = ((*reinterpret_cast<int*>(&fTemp71) & 2139095040) ? fTemp71 : 0.0f);
			float fTemp72 = std::min<float>(1.0f, 3.3333333f * fRec37[0]);
			float fTemp73 = 1.0f - fTemp72;
			iRec39[0] = 1103515245 * iRec39[1] + 3011;
			float fTemp74 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec39[0] + 31245) % 2147483647) : fRec38[1]);
			fRec38[0] = ((*reinterpret_cast<int*>(&fTemp74) & 2139095040) ? fTemp74 : 0.0f);
			float fTemp75 = 137.5f * fRec2[0];
			float fTemp76 = 187.5f - fTemp75;
			float fTemp77 = fTemp76 - (375.0f - 275.0f * fRec2[0]);
			float fTemp78 = 0.25f * fTemp2 + 1.0f;
			int iTemp79 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow11 * fTemp78 * (fTemp76 - fTemp77 * Freeze95DSP_faustpower2_f(fRec38[0] * (1.0f - 0.15f * fTemp73) + 0.045f * fTemp73)) * fTemp70), iConst18));
			int iTemp80 = std::max<int>(1, iTemp79);
			float fTemp81 = 0.65f * fTemp2;
			float fTemp82 = std::min<float>(0.9f, fTemp81);
			iRec42[0] = 1103515245 * iRec42[1] + 3022;
			float fTemp83 = 4.656613e-10f * static_cast<float>((iRec42[0] + 32356) % 2147483647);
			float fTemp84 = ((iTemp66) ? fTemp83 : fRec41[1]);
			fRec41[0] = ((*reinterpret_cast<int*>(&fTemp84) & 2139095040) ? fTemp84 : 0.0f);
			float fTemp85 = 0.97f * fRec40[1] + 0.08f * (fRec41[0] + -0.5f);
			fRec40[0] = ((*reinterpret_cast<int*>(&fTemp85) & 2139095040) ? fTemp85 : 0.0f);
			float fTemp86 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec40[0]));
			iRec44[0] = 1103515245 * iRec44[1] + 3055;
			float fTemp87 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec44[0] + 35689) % 2147483647) : fRec43[1]);
			fRec43[0] = ((*reinterpret_cast<int*>(&fTemp87) & 2139095040) ? fTemp87 : 0.0f);
			float fTemp88 = 3.0f * fTemp1 + 1.0f + fSlow12 * fTemp1;
			int iTemp89 = static_cast<int>(fSlow13 * fTemp59 * fTemp88 * (fRec43[0] - 0.5f * (1.0f - fTemp86)));
			int iTemp90 = iTemp79 + iTemp89;
			float fTemp91 = static_cast<float>(iTemp79);
			int iTemp92 = 1 - iVec0[1];
			float fTemp93 = ((iTemp11) ? fTemp67 : fRec46[1]);
			fRec46[0] = ((*reinterpret_cast<int*>(&fTemp93) & 2139095040) ? fTemp93 : 0.0f);
			float fTemp94 = 3.0f * fTemp2 + 1.0f;
			float fTemp95 = fSlow16 * (2.6f * fTemp1 + 0.35f);
			float fTemp96 = 116.5f * fTemp1 + (1.5f - fTemp95) - fSlow15;
			float fTemp97 = 1.0f - fTemp81;
			float fTemp98 = 0.585f * fTemp2;
			float fTemp99 = 1.22f * (fTemp98 + fTemp97 * (fSlow15 + fTemp95 + fTemp96 * std::pow(fRec46[0], fTemp94))) * (fTemp86 + 1.0f);
			float fTemp100 = ((iSlow14) ? ((fTemp99 < fSlow18) ? fSlow30 : ((fTemp99 < fSlow19) ? fSlow29 : ((fTemp99 < fSlow20) ? fSlow28 : ((fTemp99 < fSlow21) ? fSlow27 : ((fTemp99 < fSlow22) ? fSlow26 : ((fTemp99 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp99);
			float fTemp101 = fTemp100 / std::max<float>(fConst21, fConst21 * fTemp91);
			float fTemp102 = ((iTemp92) ? 0.0f : fRec45[1] + fConst21 * std::min<float>(5e+02f, fTemp101));
			float fTemp103 = fTemp102 - std::floor(fTemp102);
			fRec45[0] = ((*reinterpret_cast<int*>(&fTemp103) & 2139095040) ? fTemp103 : 0.0f);
			int iTemp104 = static_cast<int>(fRec45[0] * fTemp91) % iTemp80;
			float fTemp105 = 1.0f - fTemp82;
			iRec47[0] = (iVec0[1] + iRec47[1]) % iConst18;
			iRec49[0] = 1103515245 * iRec49[1] + 3044;
			float fTemp106 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec49[0] + 34578) % 2147483647) : fRec48[1]);
			fRec48[0] = ((*reinterpret_cast<int*>(&fTemp106) & 2139095040) ? fTemp106 : 0.0f);
			int iTemp107 = fRec48[0] < 0.33f;
			int iTemp108 = fRec48[0] < 0.66f;
			float fTemp109 = ((iTemp11) ? fTemp83 : fRec50[1]);
			fRec50[0] = ((*reinterpret_cast<int*>(&fTemp109) & 2139095040) ? fTemp109 : 0.0f);
			int iTemp110 = std::max<int>(16, static_cast<int>(fConst23 * (0.8f * fRec50[0] + 0.6f)));
			float fTemp111 = static_cast<float>(iTemp110);
			float fTemp112 = static_cast<float>(iTemp104 % iTemp110) / fTemp111;
			float fTemp113 = std::max<float>(0.05f, ((iTemp107) ? std::sin(3.1415927f * fTemp112) : ((iTemp108) ? 1.0f - std::fabs(2.0f * fTemp112 + -1.0f) : std::pow(fTemp112, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp112)))));
			float fTemp114 = fTemp113 * fVec5[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp104 + iTemp90) % iTemp80) + fTemp82 * static_cast<float>(iTemp79 + -1)) % iTemp80 - iTemp79)) % iConst18))) & 2097151];
			float fTemp115 = 0.4f * fTemp2 + 0.6f;
			int iTemp116 = fTemp72 > 0.65f;
			iVec6[0] = iTemp116;
			float fTemp117 = fConst24 * fRec51[1] + std::max<float>(0.0f, static_cast<float>(iTemp116 - iVec6[1]));
			fRec51[0] = ((*reinterpret_cast<int*>(&fTemp117) & 2139095040) ? fTemp117 : 0.0f);
			float fTemp118 = fRec3[0] * std::min<float>(0.35f, fRec51[0] * fTemp115 * (0.4f * fTemp72 + 0.28f));
			float fTemp119 = Freeze95DSP_faustpower2_f(fTemp1) * (3.0f - 2.0f * fTemp1);
			float fTemp120 = 0.1f * std::max<float>(0.05f, fTemp4 + (0.95f - 0.5f * std::pow(fTemp119, 1.5f))) + 0.9f * fRec53[1];
			fRec53[0] = ((*reinterpret_cast<int*>(&fTemp120) & 2139095040) ? fTemp120 : 0.0f);
			float fTemp121 = fRec53[0] + -0.01f;
			float fTemp122 = std::min<float>(1.0f, std::max<float>(0.0f, fRec48[0] + 0.2f * (fTemp57 + -0.5f)));
			float fTemp123 = std::max<float>(0.001f, 0.65f * fTemp119 + 0.1f);
			float fTemp124 = std::max<float>(0.02f, 1.0f - fRec53[0]);
			float fTemp125 = 0.1f * (fTemp124 * (0.25f * fTemp119 + 0.05f) / fTemp123) + 0.9f * fRec54[1];
			fRec54[0] = ((*reinterpret_cast<int*>(&fTemp125) & 2139095040) ? fTemp125 : 0.0f);
			float fTemp126 = fRec53[0] + fRec54[0];
			float fTemp127 = fTemp126 + -0.01f;
			float fTemp128 = 0.1f * (fTemp124 * (0.22f * fTemp119 + 0.03f) / fTemp123) + 0.9f * fRec55[1];
			fRec55[0] = ((*reinterpret_cast<int*>(&fTemp128) & 2139095040) ? fTemp128 : 0.0f);
			float fTemp129 = fTemp126 + fRec55[0] + -0.01f;
			float fTemp130 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp122 < fTemp121) ? 0 : ((fTemp122 < fTemp127) ? 1 : ((fTemp122 < fTemp129) ? 2 : 3))))) + 0.999f * fRec52[1];
			fRec52[0] = ((*reinterpret_cast<int*>(&fTemp130) & 2139095040) ? fTemp130 : 0.0f);
			int iTemp131 = static_cast<int>(fRec52[0]);
			iVec7[0] = iTemp131;
			int iTemp132 = iTemp131 == 0;
			float fTemp133 = std::pow(1e+01f, 0.05f * (-0.5f - 2.5f * fTemp1));
			float fTemp134 = ((iTemp92) ? 0.0f : fRec56[1] + 1.0f / static_cast<float>(std::max<int>(32, static_cast<int>(fConst3 * (9e+01f * fTemp1 + 26.0f)))));
			float fTemp135 = fTemp134 - std::floor(fTemp134);
			fRec56[0] = ((*reinterpret_cast<int*>(&fTemp135) & 2139095040) ? fTemp135 : 0.0f);
			float fTemp136 = std::sin(3.1415927f * fRec56[0]);
			float fTemp137 = 0.8f * fTemp136 * fVec5[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp91) % iTemp80 - iTemp79)) % iConst18))) & 2097151];
			float fTemp138 = ((iTemp92) ? 0.0f : fRec57[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp101));
			float fTemp139 = fTemp138 - std::floor(fTemp138);
			fRec57[0] = ((*reinterpret_cast<int*>(&fTemp139) & 2139095040) ? fTemp139 : 0.0f);
			float fTemp140 = fVec5[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp89 + iTemp79 + static_cast<int>(fRec57[0] * fTemp91) % iTemp80) % iTemp80 - iTemp79)) % iConst18))) & 2097151] * fTemp113;
			float fTemp141 = fTemp113 * fVec5[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fTemp91 * (1.0f - fRec45[0])) % iTemp80 + iTemp90) % iTemp80 - iTemp79)) % iConst18))) & 2097151];
			float fTemp142 = fConst26 * static_cast<float>(std::abs(iTemp131 - iVec7[1]) > 0) + fConst25 * fRec58[1];
			fRec58[0] = ((*reinterpret_cast<int*>(&fTemp142) & 2139095040) ? fTemp142 : 0.0f);
			float fTemp143 = 0.18f * fTemp58 + 1.0f;
			float fTemp144 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.95f * (fTemp64 * (((iSlow31) ? fTemp114 : fTemp143 * ((1.0f - fRec58[0]) * ((iVec7[1] == 0) ? fTemp114 : ((iVec7[1] == 1) ? fTemp141 : ((iVec7[1] == 2) ? fTemp140 : fTemp137))) + fRec58[0] * ((iTemp132) ? fTemp114 : ((iTemp131 == 1) ? fTemp141 : ((iTemp131 == 2) ? fTemp140 : fTemp137)))) * ((iTemp132) ? 1.0f : fTemp133)) * (1.0f - fTemp118) + fTemp118 * (0.35f * fTemp54 + 0.65f * fTemp114)) + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp57)))));
			float fTemp145 = std::min<float>(1.0f, std::max<float>(0.0f, std::min<float>(1.0f, 0.05f * fTemp1 + 0.95f * fRec11[0])));
			float fTemp146 = 3.0f - 2.0f * fTemp145;
			float fTemp147 = Freeze95DSP_faustpower2_f(fTemp145) * fTemp146;
			float fTemp148 = fTemp145 * (0.45f * fTemp147 + 0.55f);
			float fTemp149 = 1.0f - fTemp148;
			float fTemp150 = fRec60[1] + fConst13 * (fRec24 - fRec61[1]);
			float fTemp151 = fConst15 * fTemp150 - fRec60[1];
			fRec60[0] = ((*reinterpret_cast<int*>(&fTemp151) & 2139095040) ? fTemp151 : 0.0f);
			float fTemp152 = fRec61[1] + fConst16 * fTemp150;
			float fTemp153 = 2.0f * fTemp152 - fRec61[1];
			fRec61[0] = ((*reinterpret_cast<int*>(&fTemp153) & 2139095040) ? fTemp153 : 0.0f);
			fRec62[IOTA0 & 2097151] = ((*reinterpret_cast<int*>(&fTemp152) & 2139095040) ? fTemp152 : 0.0f);
			float fTemp154 = fRec62[IOTA0 & 2097151];
			float fTemp155 = std::fabs(fTemp154);
			float fTemp156 = std::max<float>(fConst7 * fRec59[1], fTemp155);
			fRec59[0] = ((*reinterpret_cast<int*>(&fTemp156) & 2139095040) ? fTemp156 : 0.0f);
			float fTemp157 = std::min<float>(1.0f, 4.0f * fRec59[0]);
			float fTemp158 = fTemp75 + -187.5f;
			float fTemp159 = std::max<float>(fConst19 * fRec63[1], fTemp155);
			fRec63[0] = ((*reinterpret_cast<int*>(&fTemp159) & 2139095040) ? fTemp159 : 0.0f);
			float fTemp160 = std::min<float>(1.0f, 3.3333333f * fRec63[0]);
			float fTemp161 = 1.0f - fTemp160;
			iRec65[0] = 1103515245 * iRec65[1] + 2011;
			float fTemp162 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec65[0] + 21245) % 2147483647) : fRec64[1]);
			fRec64[0] = ((*reinterpret_cast<int*>(&fTemp162) & 2139095040) ? fTemp162 : 0.0f);
			float fTemp163 = Freeze95DSP_faustpower2_f(fRec64[0] * (1.0f - 0.15f * fTemp161) + 0.045f * fTemp161);
			iRec68[0] = 1103515245 * iRec68[1] + 2033;
			float fTemp164 = 4.656613e-10f * static_cast<float>((iRec68[0] + 23467) % 2147483647);
			float fTemp165 = ((iTemp66) ? fTemp164 : fRec67[1]);
			fRec67[0] = ((*reinterpret_cast<int*>(&fTemp165) & 2139095040) ? fTemp165 : 0.0f);
			float fTemp166 = 0.97f * fRec66[1] + 0.08f * (fRec67[0] + -0.5f);
			fRec66[0] = ((*reinterpret_cast<int*>(&fTemp166) & 2139095040) ? fTemp166 : 0.0f);
			float fTemp167 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec66[0])) + 1.0f;
			float fTemp168 = fTemp78 * fTemp167;
			int iTemp169 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow32 * fTemp168 * (187.5f - (fTemp75 + fTemp163 * fTemp158))), iConst18));
			int iTemp170 = std::max<int>(1, iTemp169);
			float fTemp171 = static_cast<float>(iTemp169);
			int iTemp172 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow32 * fTemp78 * (fTemp76 - fTemp77 * fTemp163) * fTemp167), iConst18));
			float fTemp173 = static_cast<float>(iTemp172);
			iRec72[0] = 1103515245 * iRec72[1] + 2022;
			float fTemp174 = 4.656613e-10f * static_cast<float>((iRec72[0] + 22356) % 2147483647);
			float fTemp175 = ((iTemp66) ? fTemp174 : fRec71[1]);
			fRec71[0] = ((*reinterpret_cast<int*>(&fTemp175) & 2139095040) ? fTemp175 : 0.0f);
			float fTemp176 = 0.97f * fRec70[1] + 0.08f * (fRec71[0] + -0.5f);
			fRec70[0] = ((*reinterpret_cast<int*>(&fTemp176) & 2139095040) ? fTemp176 : 0.0f);
			float fTemp177 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec70[0]));
			float fTemp178 = ((iTemp11) ? fTemp164 : fRec73[1]);
			fRec73[0] = ((*reinterpret_cast<int*>(&fTemp178) & 2139095040) ? fTemp178 : 0.0f);
			float fTemp179 = (fTemp98 + fTemp97 * (fSlow15 + fTemp95 + fTemp96 * std::pow(fRec73[0], fTemp94))) * (fTemp177 + 1.0f);
			float fTemp180 = ((iSlow14) ? ((fTemp179 < fSlow18) ? fSlow30 : ((fTemp179 < fSlow19) ? fSlow29 : ((fTemp179 < fSlow20) ? fSlow28 : ((fTemp179 < fSlow21) ? fSlow27 : ((fTemp179 < fSlow22) ? fSlow26 : ((fTemp179 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp179);
			float fTemp181 = fTemp180 / std::max<float>(fConst21, fConst21 * fTemp173);
			float fTemp182 = ((iTemp92) ? 0.0f : fRec69[1] + fConst21 * std::min<float>(5e+02f, fTemp181));
			float fTemp183 = fTemp182 - std::floor(fTemp182);
			fRec69[0] = ((*reinterpret_cast<int*>(&fTemp183) & 2139095040) ? fTemp183 : 0.0f);
			iRec75[0] = 1103515245 * iRec75[1] + 2055;
			float fTemp184 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec75[0] + 25689) % 2147483647) : fRec74[1]);
			fRec74[0] = ((*reinterpret_cast<int*>(&fTemp184) & 2139095040) ? fTemp184 : 0.0f);
			int iTemp185 = static_cast<int>(fSlow13 * fTemp59 * (fRec74[0] - 0.5f * (1.0f - fTemp177)) * (fSlow33 * fTemp1 + 1.0f));
			int iTemp186 = iTemp169 + iTemp185;
			iRec77[0] = 1103515245 * iRec77[1] + 2044;
			float fTemp187 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec77[0] + 24578) % 2147483647) : fRec76[1]);
			fRec76[0] = ((*reinterpret_cast<int*>(&fTemp187) & 2139095040) ? fTemp187 : 0.0f);
			int iTemp188 = fRec76[0] < 0.33f;
			int iTemp189 = fRec76[0] < 0.66f;
			float fTemp190 = ((iTemp11) ? fTemp174 : fRec78[1]);
			fRec78[0] = ((*reinterpret_cast<int*>(&fTemp190) & 2139095040) ? fTemp190 : 0.0f);
			int iTemp191 = std::max<int>(16, static_cast<int>(fConst23 * (0.8f * fRec78[0] + 0.6f)));
			float fTemp192 = static_cast<float>(iTemp191);
			float fTemp193 = static_cast<float>(static_cast<int>(fRec69[0] * fTemp173) % std::max<int>(1, iTemp172) % iTemp191) / fTemp192;
			float fTemp194 = std::max<float>(0.05f, ((iTemp188) ? std::sin(3.1415927f * fTemp193) : ((iTemp189) ? 1.0f - std::fabs(2.0f * fTemp193 + -1.0f) : std::pow(fTemp193, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp193)))));
			float fTemp195 = fTemp194 * fRec62[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp186 + static_cast<int>(fRec69[0] * fTemp171) % iTemp170) % iTemp170) + fTemp82 * static_cast<float>(iTemp169 + -1)) % iTemp170 - iTemp169)) % iConst18))) & 2097151];
			int iTemp196 = fTemp160 > 0.65f;
			iVec8[0] = iTemp196;
			float fTemp197 = fConst24 * fRec79[1] + std::max<float>(0.0f, static_cast<float>(iTemp196 - iVec8[1]));
			fRec79[0] = ((*reinterpret_cast<int*>(&fTemp197) & 2139095040) ? fTemp197 : 0.0f);
			float fTemp198 = fRec3[0] * std::min<float>(0.35f, fRec79[0] * fTemp115 * (0.4f * fTemp160 + 0.28f));
			float fTemp199 = std::min<float>(1.0f, std::max<float>(0.0f, fRec76[0] + 0.2f * (fTemp157 + -0.5f)));
			float fTemp200 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp199 < fTemp121) ? 0 : ((fTemp199 < fTemp127) ? 1 : ((fTemp199 < fTemp129) ? 2 : 3))))) + 0.999f * fRec80[1];
			fRec80[0] = ((*reinterpret_cast<int*>(&fTemp200) & 2139095040) ? fTemp200 : 0.0f);
			int iTemp201 = static_cast<int>(fRec80[0]);
			iVec9[0] = iTemp201;
			int iTemp202 = iTemp201 == 0;
			float fTemp203 = 0.8f * fTemp136 * fRec62[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp171) % iTemp170 - iTemp169)) % iConst18))) & 2097151];
			float fTemp204 = ((iTemp92) ? 0.0f : fRec81[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp181));
			float fTemp205 = fTemp204 - std::floor(fTemp204);
			fRec81[0] = ((*reinterpret_cast<int*>(&fTemp205) & 2139095040) ? fTemp205 : 0.0f);
			float fTemp206 = fRec62[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fRec81[0] * fTemp171) % iTemp170 + iTemp186) % iTemp170 - iTemp169)) % iConst18))) & 2097151] * fTemp194;
			float fTemp207 = fTemp194 * fRec62[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp186 + static_cast<int>(fTemp171 * (1.0f - fRec69[0])) % iTemp170) % iTemp170 - iTemp169)) % iConst18))) & 2097151];
			float fTemp208 = fConst26 * static_cast<float>(std::abs(iTemp201 - iVec9[1]) > 0) + fConst25 * fRec82[1];
			fRec82[0] = ((*reinterpret_cast<int*>(&fTemp208) & 2139095040) ? fTemp208 : 0.0f);
			float fTemp209 = std::max<float>(-1.0f, std::min<float>(1.0f, 1.02f * (fTemp64 * (((iSlow31) ? fTemp195 : fTemp143 * ((1.0f - fRec82[0]) * ((iVec9[1] == 0) ? fTemp195 : ((iVec9[1] == 1) ? fTemp207 : ((iVec9[1] == 2) ? fTemp206 : fTemp203))) + fRec82[0] * ((iTemp202) ? fTemp195 : ((iTemp201 == 1) ? fTemp207 : ((iTemp201 == 2) ? fTemp206 : fTemp203)))) * ((iTemp202) ? 1.0f : fTemp133)) * (1.0f - fTemp198) + fTemp198 * (0.35f * fTemp154 + 0.65f * fTemp195)) + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp157)))));
			float fTemp210 = std::min<float>(1.0f, std::max<float>(0.0f, 0.35f * fRec11[0]));
			float fTemp211 = 3.0f - 2.0f * fTemp210;
			float fTemp212 = Freeze95DSP_faustpower2_f(fTemp210) * fTemp211;
			float fTemp213 = 0.6f * fTemp212 + 0.4f;
			float fTemp214 = 3.0f * fTemp210;
			int iTemp215 = std::max<int>(1, static_cast<int>(fTemp214 + 1.0f));
			float fTemp216 = std::max<float>(3e+03f, 2.205e+04f - 1.905e+04f * fTemp210);
			float fTemp217 = std::max<float>(0.45f * fTemp216, 2e+04f - 1.6e+04f * fTemp210);
			float fTemp218 = 1.0f / std::tan(fConst27 * std::max<float>(1.8e+02f, fTemp217 / (0.35f * static_cast<float>(iTemp215) + 0.8f)));
			float fTemp219 = fTemp218 + 1.0f;
			iRec85[0] = (iRec85[1] + 1) % iTemp215;
			int iTemp220 = iRec85[0] == 0;
			float fTemp221 = 6.0f * fTemp210;
			float fTemp222 = std::pow(1e+01f, 0.05f * (fTemp221 + 1.5f * (1.0f - std::min<float>(1.0f, fTemp214)))) + -1.0f;
			float fTemp223 = std::tan(fConst27 * (2e+03f * fTemp210 + 4e+03f));
			float fTemp224 = 1.0f / fTemp223;
			float fTemp225 = fTemp224 + 1.0f;
			float fTemp226 = 1.0f / std::tan(fConst27 * fTemp217);
			float fTemp227 = fTemp226 + 1.0f;
			float fTemp228 = 0.55f * fTemp210 + 1.0f;
			float fTemp229 = std::pow(2.0f, std::max<float>(8.0f, 16.0f - 8.0f * fTemp210));
			iRec89[0] = (iVec0[1] + iRec89[1]) % std::max<int>(1, static_cast<int>(fConst0 / fTemp216));
			int iTemp230 = iRec89[0] == 0;
			float fTemp231 = fRec94[1] + fConst8 * (fRec20 - fRec95[1]);
			float fTemp232 = fConst10 * fTemp231 - fRec94[1];
			fRec94[0] = ((*reinterpret_cast<int*>(&fTemp232) & 2139095040) ? fTemp232 : 0.0f);
			float fTemp233 = fRec95[1] + fConst11 * fTemp231;
			float fTemp234 = 2.0f * fTemp233 - fRec95[1];
			fRec95[0] = ((*reinterpret_cast<int*>(&fTemp234) & 2139095040) ? fTemp234 : 0.0f);
			float fRec96 = ((*reinterpret_cast<int*>(&fTemp233) & 2139095040) ? fTemp233 : 0.0f);
			float fTemp235 = fRec91[1] + fConst13 * (fRec96 - fRec92[1]);
			float fTemp236 = fConst15 * fTemp235 - fRec91[1];
			fRec91[0] = ((*reinterpret_cast<int*>(&fTemp236) & 2139095040) ? fTemp236 : 0.0f);
			float fTemp237 = fRec92[1] + fConst16 * fTemp235;
			float fTemp238 = 2.0f * fTemp237 - fRec92[1];
			fRec92[0] = ((*reinterpret_cast<int*>(&fTemp238) & 2139095040) ? fTemp238 : 0.0f);
			float fTemp239 = fConst17 * fTemp235;
			float fRec93 = ((*reinterpret_cast<int*>(&fTemp239) & 2139095040) ? fTemp239 : 0.0f);
			float fTemp240 = fRec96 - 2.828427f * fRec93;
			fVec10[IOTA0 & 2097151] = fTemp240;
			float fTemp241 = std::fabs(fTemp240);
			float fTemp242 = std::max<float>(fConst7 * fRec90[1], fTemp241);
			fRec90[0] = ((*reinterpret_cast<int*>(&fTemp242) & 2139095040) ? fTemp242 : 0.0f);
			float fTemp243 = std::min<float>(1.0f, 4.0f * fRec90[0]);
			iRec99[0] = 1103515245 * iRec99[1] + 1033;
			float fTemp244 = 4.656613e-10f * static_cast<float>((iRec99[0] + 15467) % 2147483647);
			float fTemp245 = ((iTemp66) ? fTemp244 : fRec98[1]);
			fRec98[0] = ((*reinterpret_cast<int*>(&fTemp245) & 2139095040) ? fTemp245 : 0.0f);
			float fTemp246 = 0.97f * fRec97[1] + 0.08f * (fRec98[0] + -0.5f);
			fRec97[0] = ((*reinterpret_cast<int*>(&fTemp246) & 2139095040) ? fTemp246 : 0.0f);
			float fTemp247 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec97[0])) + 1.0f;
			float fTemp248 = std::max<float>(fConst19 * fRec100[1], fTemp241);
			fRec100[0] = ((*reinterpret_cast<int*>(&fTemp248) & 2139095040) ? fTemp248 : 0.0f);
			float fTemp249 = std::min<float>(1.0f, 3.3333333f * fRec100[0]);
			float fTemp250 = 1.0f - fTemp249;
			iRec102[0] = 1103515245 * iRec102[1] + 1011;
			float fTemp251 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec102[0] + 13245) % 2147483647) : fRec101[1]);
			fRec101[0] = ((*reinterpret_cast<int*>(&fTemp251) & 2139095040) ? fTemp251 : 0.0f);
			int iTemp252 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow34 * fTemp78 * (fTemp76 - fTemp77 * Freeze95DSP_faustpower2_f(fRec101[0] * (1.0f - 0.15f * fTemp250) + 0.045f * fTemp250)) * fTemp247), iConst18));
			int iTemp253 = std::max<int>(1, iTemp252);
			iRec105[0] = 1103515245 * iRec105[1] + 1022;
			float fTemp254 = 4.656613e-10f * static_cast<float>((iRec105[0] + 14356) % 2147483647);
			float fTemp255 = ((iTemp66) ? fTemp254 : fRec104[1]);
			fRec104[0] = ((*reinterpret_cast<int*>(&fTemp255) & 2139095040) ? fTemp255 : 0.0f);
			float fTemp256 = 0.97f * fRec103[1] + 0.08f * (fRec104[0] + -0.5f);
			fRec103[0] = ((*reinterpret_cast<int*>(&fTemp256) & 2139095040) ? fTemp256 : 0.0f);
			float fTemp257 = std::min<float>(0.25f, std::max<float>(-0.25f, fRec103[0]));
			iRec107[0] = 1103515245 * iRec107[1] + 1055;
			float fTemp258 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec107[0] + 17689) % 2147483647) : fRec106[1]);
			fRec106[0] = ((*reinterpret_cast<int*>(&fTemp258) & 2139095040) ? fTemp258 : 0.0f);
			int iTemp259 = static_cast<int>(fSlow13 * (fRec106[0] - 0.5f * (1.0f - fTemp257)) * fTemp59 * fTemp88);
			int iTemp260 = iTemp252 + iTemp259;
			float fTemp261 = static_cast<float>(iTemp252);
			float fTemp262 = ((iTemp11) ? fTemp244 : fRec109[1]);
			fRec109[0] = ((*reinterpret_cast<int*>(&fTemp262) & 2139095040) ? fTemp262 : 0.0f);
			float fTemp263 = 0.82f * (fTemp98 + fTemp97 * (fSlow15 + fTemp95 + fTemp96 * std::pow(fRec109[0], fTemp94))) * (fTemp257 + 1.0f);
			float fTemp264 = ((iSlow14) ? ((fTemp263 < fSlow18) ? fSlow30 : ((fTemp263 < fSlow19) ? fSlow29 : ((fTemp263 < fSlow20) ? fSlow28 : ((fTemp263 < fSlow21) ? fSlow27 : ((fTemp263 < fSlow22) ? fSlow26 : ((fTemp263 < fSlow23) ? fSlow25 : fSlow24)))))) : fTemp263);
			float fTemp265 = fTemp264 / std::max<float>(fConst21, fConst21 * fTemp261);
			float fTemp266 = ((iTemp92) ? 0.0f : fRec108[1] + fConst21 * std::min<float>(5e+02f, fTemp265));
			float fTemp267 = fTemp266 - std::floor(fTemp266);
			fRec108[0] = ((*reinterpret_cast<int*>(&fTemp267) & 2139095040) ? fTemp267 : 0.0f);
			int iTemp268 = static_cast<int>(fRec108[0] * fTemp261) % iTemp253;
			iRec111[0] = 1103515245 * iRec111[1] + 1044;
			float fTemp269 = ((iTemp11) ? 4.656613e-10f * static_cast<float>((iRec111[0] + 16578) % 2147483647) : fRec110[1]);
			fRec110[0] = ((*reinterpret_cast<int*>(&fTemp269) & 2139095040) ? fTemp269 : 0.0f);
			int iTemp270 = fRec110[0] < 0.33f;
			int iTemp271 = fRec110[0] < 0.66f;
			float fTemp272 = ((iTemp11) ? fTemp254 : fRec112[1]);
			fRec112[0] = ((*reinterpret_cast<int*>(&fTemp272) & 2139095040) ? fTemp272 : 0.0f);
			int iTemp273 = std::max<int>(16, static_cast<int>(fConst23 * (0.8f * fRec112[0] + 0.6f)));
			float fTemp274 = static_cast<float>(iTemp273);
			float fTemp275 = static_cast<float>(iTemp268 % iTemp273) / fTemp274;
			float fTemp276 = std::max<float>(0.05f, ((iTemp270) ? std::sin(3.1415927f * fTemp275) : ((iTemp271) ? 1.0f - std::fabs(2.0f * fTemp275 + -1.0f) : std::pow(fTemp275, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp275)))));
			float fTemp277 = fTemp276 * fVec10[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp268 + iTemp260) % iTemp253) + fTemp82 * static_cast<float>(iTemp252 + -1)) % iTemp253 - iTemp252)) % iConst18))) & 2097151];
			int iTemp278 = fTemp249 > 0.65f;
			iVec11[0] = iTemp278;
			float fTemp279 = fConst24 * fRec113[1] + std::max<float>(0.0f, static_cast<float>(iTemp278 - iVec11[1]));
			fRec113[0] = ((*reinterpret_cast<int*>(&fTemp279) & 2139095040) ? fTemp279 : 0.0f);
			float fTemp280 = fRec3[0] * std::min<float>(0.35f, fRec113[0] * (0.4f * fTemp249 + 0.28f) * fTemp115);
			float fTemp281 = std::min<float>(1.0f, std::max<float>(0.0f, fRec110[0] + 0.2f * (fTemp243 + -0.5f)));
			float fTemp282 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp281 < fTemp121) ? 0 : ((fTemp281 < fTemp127) ? 1 : ((fTemp281 < fTemp129) ? 2 : 3))))) + 0.999f * fRec114[1];
			fRec114[0] = ((*reinterpret_cast<int*>(&fTemp282) & 2139095040) ? fTemp282 : 0.0f);
			int iTemp283 = static_cast<int>(fRec114[0]);
			iVec12[0] = iTemp283;
			int iTemp284 = iTemp283 == 0;
			float fTemp285 = 0.8f * fVec10[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp261) % iTemp253 - iTemp252)) % iConst18))) & 2097151] * fTemp136;
			float fTemp286 = ((iTemp92) ? 0.0f : fRec115[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp265));
			float fTemp287 = fTemp286 - std::floor(fTemp286);
			fRec115[0] = ((*reinterpret_cast<int*>(&fTemp287) & 2139095040) ? fTemp287 : 0.0f);
			float fTemp288 = fVec10[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp259 + iTemp252 + static_cast<int>(fRec115[0] * fTemp261) % iTemp253) % iTemp253 - iTemp252)) % iConst18))) & 2097151] * fTemp276;
			float fTemp289 = fTemp276 * fVec10[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fTemp261 * (1.0f - fRec108[0])) % iTemp253 + iTemp260) % iTemp253 - iTemp252)) % iConst18))) & 2097151];
			float fTemp290 = fConst26 * static_cast<float>(std::abs(iTemp283 - iVec12[1]) > 0) + fConst25 * fRec116[1];
			fRec116[0] = ((*reinterpret_cast<int*>(&fTemp290) & 2139095040) ? fTemp290 : 0.0f);
			float fTemp291 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.98f * ((((iSlow31) ? fTemp277 : ((1.0f - fRec116[0]) * ((iVec12[1] == 0) ? fTemp277 : ((iVec12[1] == 1) ? fTemp289 : ((iVec12[1] == 2) ? fTemp288 : fTemp285))) + fRec116[0] * ((iTemp284) ? fTemp277 : ((iTemp283 == 1) ? fTemp289 : ((iTemp283 == 2) ? fTemp288 : fTemp285)))) * ((iTemp284) ? 1.0f : fTemp133) * fTemp143) * (1.0f - fTemp280) + fTemp280 * (0.35f * fTemp240 + 0.65f * fTemp277)) * fTemp64 + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp243)))));
			float fTemp292 = 6.0f * fTemp210 * std::fabs(fTemp291) + 1.0f;
			float fTemp293 = fTemp221 + 1.0f;
			float fTemp294 = fTemp291 * fTemp293;
			float fTemp295 = fTemp294 / fTemp292;
			float fTemp296 = fTemp210 * (0.75f * fTemp210 * fTemp211 + 3.5f) + 1.5f;
			float fTemp297 = fTemp292 * (fTemp296 * std::fabs(fTemp295) + 1.0f);
			float fTemp298 = ((iTemp230) ? fTemp294 / fTemp297 : fRec88[1]);
			fRec88[0] = ((*reinterpret_cast<int*>(&fTemp298) & 2139095040) ? fTemp298 : 0.0f);
			float fTemp299 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec88[0] * fTemp229 + 0.5f) / fTemp229));
			float fTemp300 = 0.65f * fTemp210 + 0.35f;
			float fTemp301 = ((iTemp230) ? fTemp295 : fRec117[1]);
			fRec117[0] = ((*reinterpret_cast<int*>(&fTemp301) & 2139095040) ? fTemp301 : 0.0f);
			float fTemp302 = 1.0f - fTemp210;
			float fTemp303 = 1.0f - fTemp212;
			float fTemp304 = fTemp294 * fTemp229;
			float fTemp305 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp304 / fTemp297 + 0.5f) / fTemp229));
			float fTemp306 = ((0.65f * fTemp302 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp304 / fTemp292 + 0.5f) / fTemp229)) + fTemp300 * fTemp305 / std::max<float>(0.05f, 1.0f - fTemp296 * std::min<float>(0.999f, std::fabs(fTemp305)))) * fTemp303 + fTemp212 * (0.65f * fTemp302 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec117[0] * fTemp229 + 0.5f) / fTemp229)) + fTemp300 * fTemp299 / std::max<float>(0.05f, 1.0f - fTemp296 * std::min<float>(0.999f, std::fabs(fTemp299))))) * fTemp228;
			fVec13[0] = fTemp306;
			float fTemp307 = 1.0f - fTemp226;
			float fTemp308 = (fRec87[1] * fTemp307 - (fTemp306 + fVec13[1])) / fTemp227;
			float n0 = -fTemp308; 			fRec87[0] = ((*reinterpret_cast<int*>(&n0) & 2139095040) ? n0 : 0.0f);
			float fTemp309 = 1.0f - fTemp224;
			float fTemp310 = (fRec86[1] * fTemp309 - (fRec87[0] - fRec87[1]) / fTemp223) / fTemp225;
			float n1 = -fTemp310; 			fRec86[0] = ((*reinterpret_cast<int*>(&n1) & 2139095040) ? n1 : 0.0f);
			float fTemp311 = fRec87[0] + fRec86[0] * fTemp222;
			float fTemp312 = ((iTemp220) ? fTemp311 : fRec84[1]);
			fRec84[0] = ((*reinterpret_cast<int*>(&fTemp312) & 2139095040) ? fTemp312 : 0.0f);
			float fTemp313 = 1.0f - fTemp218;
			float fTemp314 = (fRec83[1] * fTemp313 - (fRec84[0] + fRec84[1])) / fTemp219;
			float n2 = -fTemp314; 			fRec83[0] = ((*reinterpret_cast<int*>(&n2) & 2139095040) ? n2 : 0.0f);
			float fTemp315 = 1.0f - fTemp210 * fTemp213;
			float fTemp316 = fTemp210 * (0.45f * fTemp212 + 0.55f);
			float fTemp317 = 1.0f - fTemp316;
			float fTemp318 = 0.6f * fTemp147 + 0.4f;
			float fTemp319 = 3.0f * fTemp145;
			int iTemp320 = std::max<int>(1, static_cast<int>(fTemp319 + 1.0f));
			float fTemp321 = std::max<float>(3e+03f, 2.205e+04f - 1.905e+04f * fTemp145);
			float fTemp322 = std::max<float>(0.45f * fTemp321, 2e+04f - 1.6e+04f * fTemp145);
			float fTemp323 = 1.0f / std::tan(fConst27 * std::max<float>(1.8e+02f, fTemp322 / (0.35f * static_cast<float>(iTemp320) + 0.8f)));
			float fTemp324 = fTemp323 + 1.0f;
			iRec120[0] = (iRec120[1] + 1) % iTemp320;
			int iTemp325 = iRec120[0] == 0;
			float fTemp326 = 6.0f * fTemp145;
			float fTemp327 = std::pow(1e+01f, 0.05f * (fTemp326 + 1.5f * (1.0f - std::min<float>(1.0f, fTemp319)))) + -1.0f;
			float fTemp328 = std::tan(fConst27 * (2e+03f * fTemp145 + 4e+03f));
			float fTemp329 = 1.0f / fTemp328;
			float fTemp330 = fTemp329 + 1.0f;
			float fTemp331 = 1.0f / std::tan(fConst27 * fTemp322);
			float fTemp332 = fTemp331 + 1.0f;
			float fTemp333 = 0.55f * fTemp145 + 1.0f;
			float fTemp334 = std::pow(2.0f, std::max<float>(8.0f, 16.0f - 8.0f * fTemp145));
			iRec124[0] = (iVec0[1] + iRec124[1]) % std::max<int>(1, static_cast<int>(fConst0 / fTemp321));
			int iTemp335 = iRec124[0] == 0;
			float fTemp336 = 6.0f * fTemp145 * std::fabs(fTemp209) + 1.0f;
			float fTemp337 = fTemp326 + 1.0f;
			float fTemp338 = fTemp209 * fTemp337;
			float fTemp339 = fTemp338 / fTemp336;
			float fTemp340 = fTemp145 * (0.75f * fTemp145 * fTemp146 + 3.5f) + 1.5f;
			float fTemp341 = fTemp336 * (fTemp340 * std::fabs(fTemp339) + 1.0f);
			float fTemp342 = ((iTemp335) ? fTemp338 / fTemp341 : fRec123[1]);
			fRec123[0] = ((*reinterpret_cast<int*>(&fTemp342) & 2139095040) ? fTemp342 : 0.0f);
			float fTemp343 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec123[0] * fTemp334 + 0.5f) / fTemp334));
			float fTemp344 = 0.65f * fTemp145 + 0.35f;
			float fTemp345 = ((iTemp335) ? fTemp339 : fRec125[1]);
			fRec125[0] = ((*reinterpret_cast<int*>(&fTemp345) & 2139095040) ? fTemp345 : 0.0f);
			float fTemp346 = 1.0f - fTemp145;
			float fTemp347 = 1.0f - fTemp147;
			float fTemp348 = fTemp338 * fTemp334;
			float fTemp349 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp348 / fTemp341 + 0.5f) / fTemp334));
			float fTemp350 = ((0.65f * fTemp346 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp348 / fTemp336 + 0.5f) / fTemp334)) + fTemp344 * fTemp349 / std::max<float>(0.05f, 1.0f - fTemp340 * std::min<float>(0.999f, std::fabs(fTemp349)))) * fTemp347 + fTemp147 * (0.65f * fTemp346 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec125[0] * fTemp334 + 0.5f) / fTemp334)) + fTemp344 * fTemp343 / std::max<float>(0.05f, 1.0f - fTemp340 * std::min<float>(0.999f, std::fabs(fTemp343))))) * fTemp333;
			fVec14[0] = fTemp350;
			float fTemp351 = 1.0f - fTemp331;
			float fTemp352 = (fRec122[1] * fTemp351 - (fTemp350 + fVec14[1])) / fTemp332;
			float n3 = -fTemp352; 			fRec122[0] = ((*reinterpret_cast<int*>(&n3) & 2139095040) ? n3 : 0.0f);
			float fTemp353 = 1.0f - fTemp329;
			float fTemp354 = (fRec121[1] * fTemp353 - (fRec122[0] - fRec122[1]) / fTemp328) / fTemp330;
			float n4 = -fTemp354; 			fRec121[0] = ((*reinterpret_cast<int*>(&n4) & 2139095040) ? n4 : 0.0f);
			float fTemp355 = fRec122[0] + fRec121[0] * fTemp327;
			float fTemp356 = ((iTemp325) ? fTemp355 : fRec119[1]);
			fRec119[0] = ((*reinterpret_cast<int*>(&fTemp356) & 2139095040) ? fTemp356 : 0.0f);
			float fTemp357 = 1.0f - fTemp323;
			float fTemp358 = (fRec118[1] * fTemp357 - (fRec119[0] + fRec119[1])) / fTemp324;
			float n5 = -fTemp358; 			fRec118[0] = ((*reinterpret_cast<int*>(&n5) & 2139095040) ? n5 : 0.0f);
			float fTemp359 = 1.0f - fTemp145 * fTemp318;
			float fTemp360 = 0.6f * fTemp23 + 0.4f;
			float fTemp361 = 3.0f * fTemp21;
			int iTemp362 = std::max<int>(1, static_cast<int>(fTemp361 + 1.0f));
			float fTemp363 = std::max<float>(3e+03f, 2.205e+04f - 1.905e+04f * fTemp21);
			float fTemp364 = std::max<float>(0.45f * fTemp363, 2e+04f - 1.6e+04f * fTemp21);
			float fTemp365 = 1.0f / std::tan(fConst27 * std::max<float>(1.8e+02f, fTemp364 / (0.35f * static_cast<float>(iTemp362) + 0.8f)));
			float fTemp366 = fTemp365 + 1.0f;
			iRec128[0] = (iRec128[1] + 1) % iTemp362;
			int iTemp367 = iRec128[0] == 0;
			float fTemp368 = 6.0f * fTemp21;
			float fTemp369 = std::pow(1e+01f, 0.05f * (fTemp368 + 1.5f * (1.0f - std::min<float>(1.0f, fTemp361)))) + -1.0f;
			float fTemp370 = std::tan(fConst27 * (2e+03f * fTemp21 + 4e+03f));
			float fTemp371 = 1.0f / fTemp370;
			float fTemp372 = fTemp371 + 1.0f;
			float fTemp373 = 1.0f / std::tan(fConst27 * fTemp364);
			float fTemp374 = fTemp373 + 1.0f;
			float fTemp375 = 0.55f * fTemp21 + 1.0f;
			float fTemp376 = std::pow(2.0f, std::max<float>(8.0f, 16.0f - 8.0f * fTemp21));
			iRec132[0] = (iVec0[1] + iRec132[1]) % std::max<int>(1, static_cast<int>(fConst0 / fTemp363));
			int iTemp377 = iRec132[0] == 0;
			float fTemp378 = 6.0f * fTemp21 * std::fabs(fTemp144) + 1.0f;
			float fTemp379 = fTemp368 + 1.0f;
			float fTemp380 = fTemp144 * fTemp379;
			float fTemp381 = fTemp380 / fTemp378;
			float fTemp382 = fTemp21 * (0.75f * fTemp21 * fTemp22 + 3.5f) + 1.5f;
			float fTemp383 = fTemp378 * (fTemp382 * std::fabs(fTemp381) + 1.0f);
			float fTemp384 = ((iTemp377) ? fTemp380 / fTemp383 : fRec131[1]);
			fRec131[0] = ((*reinterpret_cast<int*>(&fTemp384) & 2139095040) ? fTemp384 : 0.0f);
			float fTemp385 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec131[0] * fTemp376 + 0.5f) / fTemp376));
			float fTemp386 = 0.65f * fTemp21 + 0.35f;
			float fTemp387 = ((iTemp377) ? fTemp381 : fRec133[1]);
			fRec133[0] = ((*reinterpret_cast<int*>(&fTemp387) & 2139095040) ? fTemp387 : 0.0f);
			float fTemp388 = 1.0f - fTemp21;
			float fTemp389 = 1.0f - fTemp23;
			float fTemp390 = fTemp380 * fTemp376;
			float fTemp391 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp390 / fTemp383 + 0.5f) / fTemp376));
			float fTemp392 = ((0.65f * fTemp388 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp390 / fTemp378 + 0.5f) / fTemp376)) + fTemp386 * fTemp391 / std::max<float>(0.05f, 1.0f - fTemp382 * std::min<float>(0.999f, std::fabs(fTemp391)))) * fTemp389 + fTemp23 * (0.65f * fTemp388 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec133[0] * fTemp376 + 0.5f) / fTemp376)) + fTemp386 * fTemp385 / std::max<float>(0.05f, 1.0f - fTemp382 * std::min<float>(0.999f, std::fabs(fTemp385))))) * fTemp375;
			fVec15[0] = fTemp392;
			float fTemp393 = 1.0f - fTemp373;
			float fTemp394 = (fRec130[1] * fTemp393 - (fTemp392 + fVec15[1])) / fTemp374;
			float n6 = -fTemp394; 			fRec130[0] = ((*reinterpret_cast<int*>(&n6) & 2139095040) ? n6 : 0.0f);
			float fTemp395 = 1.0f - fTemp371;
			float fTemp396 = (fRec129[1] * fTemp395 - (fRec130[0] - fRec130[1]) / fTemp370) / fTemp372;
			float n7 = -fTemp396; 			fRec129[0] = ((*reinterpret_cast<int*>(&n7) & 2139095040) ? n7 : 0.0f);
			float fTemp397 = fRec130[0] + fRec129[0] * fTemp369;
			float fTemp398 = ((iTemp367) ? fTemp397 : fRec127[1]);
			fRec127[0] = ((*reinterpret_cast<int*>(&fTemp398) & 2139095040) ? fTemp398 : 0.0f);
			float fTemp399 = 1.0f - fTemp365;
			float fTemp400 = (fRec126[1] * fTemp399 - (fRec127[0] + fRec127[1])) / fTemp366;
			float n8 = -fTemp400; 			fRec126[0] = ((*reinterpret_cast<int*>(&n8) & 2139095040) ? n8 : 0.0f);
			float fTemp401 = 1.0f - fTemp21 * fTemp360;
			float fTemp402 = 1.0f - fRec3[0];
			float fTemp403 = fTemp26 * fTemp402;
			fVec16[IOTA0 & 2047] = fTemp403 + fRec3[0] * (fTemp24 * ((fTemp380 * fTemp388 / fTemp378 + fTemp21 * fTemp397) * fTemp401 + fRec126[0] * fTemp21 * fTemp360) + fTemp148 * ((fTemp338 * fTemp346 / fTemp336 + fTemp145 * fTemp355) * fTemp359 + fRec118[0] * fTemp145 * fTemp318) + fTemp291 * fTemp317 + fTemp316 * ((fTemp294 * fTemp302 / fTemp292 + fTemp210 * fTemp311) * fTemp315 + fRec83[0] * fTemp210 * fTemp213) + fTemp209 * fTemp149 + fTemp144 * fTemp25);
			float fTemp404 = fVec16[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp16))) & 2047];
			float fTemp405 = static_cast<float>(input1[i0]);
			float fTemp406 = fRec139[1] + fConst8 * (fTemp405 - fRec140[1]);
			float fTemp407 = fConst10 * fTemp406 - fRec139[1];
			fRec139[0] = ((*reinterpret_cast<int*>(&fTemp407) & 2139095040) ? fTemp407 : 0.0f);
			float fTemp408 = fRec140[1] + fConst11 * fTemp406;
			float fTemp409 = 2.0f * fTemp408 - fRec140[1];
			fRec140[0] = ((*reinterpret_cast<int*>(&fTemp409) & 2139095040) ? fTemp409 : 0.0f);
			float fTemp410 = fConst12 * fTemp406;
			float fRec141 = ((*reinterpret_cast<int*>(&fTemp410) & 2139095040) ? fTemp410 : 0.0f);
			float fRec142 = ((*reinterpret_cast<int*>(&fTemp408) & 2139095040) ? fTemp408 : 0.0f);
			float fTemp411 = fRec142 + 1.4142135f * fRec141;
			float fTemp412 = fRec135[1] + fConst8 * (fTemp405 - (fTemp411 + fRec136[1]));
			float fTemp413 = fConst10 * fTemp412 - fRec135[1];
			fRec135[0] = ((*reinterpret_cast<int*>(&fTemp413) & 2139095040) ? fTemp413 : 0.0f);
			float fTemp414 = fRec136[1] + fConst11 * fTemp412;
			float fTemp415 = 2.0f * fTemp414 - fRec136[1];
			fRec136[0] = ((*reinterpret_cast<int*>(&fTemp415) & 2139095040) ? fTemp415 : 0.0f);
			float fTemp416 = fConst12 * fTemp412;
			float fRec137 = ((*reinterpret_cast<int*>(&fTemp416) & 2139095040) ? fTemp416 : 0.0f);
			float fRec138 = ((*reinterpret_cast<int*>(&fTemp414) & 2139095040) ? fTemp414 : 0.0f);
			float fTemp417 = fRec142 + fRec138;
			float fTemp418 = fRec141 + fRec137;
			float fTemp419 = 1.4142135f * fTemp418 + fTemp417;
			float fTemp420 = fRec143[1] + fConst13 * (fTemp405 - (fTemp419 + fRec144[1]));
			float fTemp421 = fConst15 * fTemp420 - fRec143[1];
			fRec143[0] = ((*reinterpret_cast<int*>(&fTemp421) & 2139095040) ? fTemp421 : 0.0f);
			float fTemp422 = fRec144[1] + fConst16 * fTemp420;
			float fTemp423 = 2.0f * fTemp422 - fRec144[1];
			fRec144[0] = ((*reinterpret_cast<int*>(&fTemp423) & 2139095040) ? fTemp423 : 0.0f);
			float fTemp424 = fConst17 * fTemp420;
			float fRec145 = ((*reinterpret_cast<int*>(&fTemp424) & 2139095040) ? fTemp424 : 0.0f);
			float fRec146 = ((*reinterpret_cast<int*>(&fTemp422) & 2139095040) ? fTemp422 : 0.0f);
			float fTemp425 = fRec146 + fTemp417;
			float fTemp426 = fRec145 + fTemp418;
			float fTemp427 = 1.4142135f * fTemp426 + fTemp425;
			float fTemp428 = fRec147[1] + fConst13 * (fTemp405 - (fTemp427 + fRec148[1]));
			float fTemp429 = fConst15 * fTemp428 - fRec147[1];
			fRec147[0] = ((*reinterpret_cast<int*>(&fTemp429) & 2139095040) ? fTemp429 : 0.0f);
			float fTemp430 = fRec148[1] + fConst16 * fTemp428;
			float fTemp431 = 2.0f * fTemp430 - fRec148[1];
			fRec148[0] = ((*reinterpret_cast<int*>(&fTemp431) & 2139095040) ? fTemp431 : 0.0f);
			float fTemp432 = fConst17 * fTemp428;
			float fRec149 = ((*reinterpret_cast<int*>(&fTemp432) & 2139095040) ? fTemp432 : 0.0f);
			float fRec150 = ((*reinterpret_cast<int*>(&fTemp430) & 2139095040) ? fTemp430 : 0.0f);
			float fTemp433 = fTemp405 - (1.4142135f * (fRec149 + fTemp426) + fRec150 + fTemp425);
			fVec17[IOTA0 & 2097151] = fTemp433;
			float fTemp434 = std::fabs(fTemp433);
			float fTemp435 = std::max<float>(fConst7 * fRec134[1], fTemp434);
			fRec134[0] = ((*reinterpret_cast<int*>(&fTemp435) & 2139095040) ? fTemp435 : 0.0f);
			float fTemp436 = std::min<float>(1.0f, 4.0f * fRec134[0]);
			float fTemp437 = std::max<float>(fConst19 * fRec151[1], fTemp434);
			fRec151[0] = ((*reinterpret_cast<int*>(&fTemp437) & 2139095040) ? fTemp437 : 0.0f);
			float fTemp438 = std::min<float>(1.0f, 3.3333333f * fRec151[0]);
			float fTemp439 = 1.0f - fTemp438;
			int iTemp440 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow11 * fTemp78 * fTemp70 * (fTemp76 - fTemp77 * Freeze95DSP_faustpower2_f(fRec38[0] * (1.0f - 0.15f * fTemp439) + 0.045f * fTemp439))), iConst18));
			int iTemp441 = std::max<int>(1, iTemp440);
			float fTemp442 = static_cast<float>(iTemp440);
			float fTemp443 = fTemp100 / std::max<float>(fConst21, fConst21 * fTemp442);
			float fTemp444 = ((iTemp92) ? 0.0f : fRec152[1] + fConst21 * std::min<float>(5e+02f, fTemp443));
			float fTemp445 = fTemp444 - std::floor(fTemp444);
			fRec152[0] = ((*reinterpret_cast<int*>(&fTemp445) & 2139095040) ? fTemp445 : 0.0f);
			int iTemp446 = static_cast<int>(fRec152[0] * fTemp442) % iTemp441;
			int iTemp447 = iTemp89 + iTemp440;
			float fTemp448 = static_cast<float>(iTemp446 % iTemp110) / fTemp111;
			float fTemp449 = std::max<float>(0.05f, ((iTemp107) ? std::sin(3.1415927f * fTemp448) : ((iTemp108) ? 1.0f - std::fabs(2.0f * fTemp448 + -1.0f) : std::pow(fTemp448, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp448)))));
			float fTemp450 = fTemp449 * fVec17[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp447 + iTemp446) % iTemp441) + fTemp82 * static_cast<float>(iTemp440 + -1)) % iTemp441 - iTemp440)) % iConst18))) & 2097151];
			int iTemp451 = fTemp438 > 0.65f;
			iVec18[0] = iTemp451;
			float fTemp452 = fConst24 * fRec153[1] + std::max<float>(0.0f, static_cast<float>(iTemp451 - iVec18[1]));
			fRec153[0] = ((*reinterpret_cast<int*>(&fTemp452) & 2139095040) ? fTemp452 : 0.0f);
			float fTemp453 = fRec3[0] * std::min<float>(0.35f, fRec153[0] * fTemp115 * (0.4f * fTemp438 + 0.28f));
			float fTemp454 = std::min<float>(1.0f, std::max<float>(0.0f, fRec48[0] + 0.2f * (fTemp436 + -0.5f)));
			float fTemp455 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp454 < fTemp121) ? 0 : ((fTemp454 < fTemp127) ? 1 : ((fTemp454 < fTemp129) ? 2 : 3))))) + 0.999f * fRec154[1];
			fRec154[0] = ((*reinterpret_cast<int*>(&fTemp455) & 2139095040) ? fTemp455 : 0.0f);
			int iTemp456 = static_cast<int>(fRec154[0]);
			iVec19[0] = iTemp456;
			int iTemp457 = iTemp456 == 0;
			float fTemp458 = 0.8f * fTemp136 * fVec17[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp442) % iTemp441 - iTemp440)) % iConst18))) & 2097151];
			float fTemp459 = ((iTemp92) ? 0.0f : fRec155[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp443));
			float fTemp460 = fTemp459 - std::floor(fTemp459);
			fRec155[0] = ((*reinterpret_cast<int*>(&fTemp460) & 2139095040) ? fTemp460 : 0.0f);
			float fTemp461 = fVec17[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fRec155[0] * fTemp442) % iTemp441 + iTemp447) % iTemp441 - iTemp440)) % iConst18))) & 2097151] * fTemp449;
			float fTemp462 = fTemp449 * fVec17[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp447 + static_cast<int>(fTemp442 * (1.0f - fRec152[0])) % iTemp441) % iTemp441 - iTemp440)) % iConst18))) & 2097151];
			float fTemp463 = fConst26 * static_cast<float>(std::abs(iTemp456 - iVec19[1]) > 0) + fConst25 * fRec156[1];
			fRec156[0] = ((*reinterpret_cast<int*>(&fTemp463) & 2139095040) ? fTemp463 : 0.0f);
			float fTemp464 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.95f * (fTemp64 * (((iSlow31) ? fTemp450 : fTemp143 * ((1.0f - fRec156[0]) * ((iVec19[1] == 0) ? fTemp450 : ((iVec19[1] == 1) ? fTemp462 : ((iVec19[1] == 2) ? fTemp461 : fTemp458))) + fRec156[0] * ((iTemp457) ? fTemp450 : ((iTemp456 == 1) ? fTemp462 : ((iTemp456 == 2) ? fTemp461 : fTemp458)))) * ((iTemp457) ? 1.0f : fTemp133)) * (1.0f - fTemp453) + fTemp453 * (0.35f * fTemp433 + 0.65f * fTemp450)) + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp436)))));
			float fTemp465 = fRec158[1] + fConst13 * (fRec146 - fRec159[1]);
			float fTemp466 = fConst15 * fTemp465 - fRec158[1];
			fRec158[0] = ((*reinterpret_cast<int*>(&fTemp466) & 2139095040) ? fTemp466 : 0.0f);
			float fTemp467 = fRec159[1] + fConst16 * fTemp465;
			float fTemp468 = 2.0f * fTemp467 - fRec159[1];
			fRec159[0] = ((*reinterpret_cast<int*>(&fTemp468) & 2139095040) ? fTemp468 : 0.0f);
			fRec160[IOTA0 & 2097151] = ((*reinterpret_cast<int*>(&fTemp467) & 2139095040) ? fTemp467 : 0.0f);
			float fTemp469 = fRec160[IOTA0 & 2097151];
			float fTemp470 = std::fabs(fTemp469);
			float fTemp471 = std::max<float>(fConst7 * fRec157[1], fTemp470);
			fRec157[0] = ((*reinterpret_cast<int*>(&fTemp471) & 2139095040) ? fTemp471 : 0.0f);
			float fTemp472 = std::min<float>(1.0f, 4.0f * fRec157[0]);
			float fTemp473 = std::max<float>(fConst19 * fRec161[1], fTemp470);
			fRec161[0] = ((*reinterpret_cast<int*>(&fTemp473) & 2139095040) ? fTemp473 : 0.0f);
			float fTemp474 = std::min<float>(1.0f, 3.3333333f * fRec161[0]);
			float fTemp475 = 1.0f - fTemp474;
			float fTemp476 = Freeze95DSP_faustpower2_f(fRec64[0] * (1.0f - 0.15f * fTemp475) + 0.045f * fTemp475);
			int iTemp477 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow32 * fTemp168 * (187.5f - (fTemp75 + fTemp158 * fTemp476))), iConst18));
			int iTemp478 = std::max<int>(1, iTemp477);
			float fTemp479 = static_cast<float>(iTemp477);
			int iTemp480 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow32 * fTemp168 * (fTemp76 - fTemp77 * fTemp476)), iConst18));
			float fTemp481 = static_cast<float>(iTemp480);
			float fTemp482 = fTemp180 / std::max<float>(fConst21, fConst21 * fTemp481);
			float fTemp483 = ((iTemp92) ? 0.0f : fRec162[1] + fConst21 * std::min<float>(5e+02f, fTemp482));
			float fTemp484 = fTemp483 - std::floor(fTemp483);
			fRec162[0] = ((*reinterpret_cast<int*>(&fTemp484) & 2139095040) ? fTemp484 : 0.0f);
			int iTemp485 = iTemp185 + iTemp477;
			float fTemp486 = static_cast<float>(static_cast<int>(fRec162[0] * fTemp481) % std::max<int>(1, iTemp480) % iTemp191) / fTemp192;
			float fTemp487 = std::max<float>(0.05f, ((iTemp188) ? std::sin(3.1415927f * fTemp486) : ((iTemp189) ? 1.0f - std::fabs(2.0f * fTemp486 + -1.0f) : std::pow(fTemp486, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp486)))));
			float fTemp488 = fTemp487 * fRec160[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp485 + static_cast<int>(fRec162[0] * fTemp479) % iTemp478) % iTemp478) + fTemp82 * static_cast<float>(iTemp477 + -1)) % iTemp478 - iTemp477)) % iConst18))) & 2097151];
			int iTemp489 = fTemp474 > 0.65f;
			iVec20[0] = iTemp489;
			float fTemp490 = fConst24 * fRec163[1] + std::max<float>(0.0f, static_cast<float>(iTemp489 - iVec20[1]));
			fRec163[0] = ((*reinterpret_cast<int*>(&fTemp490) & 2139095040) ? fTemp490 : 0.0f);
			float fTemp491 = fRec3[0] * std::min<float>(0.35f, fRec163[0] * fTemp115 * (0.4f * fTemp474 + 0.28f));
			float fTemp492 = std::min<float>(1.0f, std::max<float>(0.0f, fRec76[0] + 0.2f * (fTemp472 + -0.5f)));
			float fTemp493 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp492 < fTemp121) ? 0 : ((fTemp492 < fTemp127) ? 1 : ((fTemp492 < fTemp129) ? 2 : 3))))) + 0.999f * fRec164[1];
			fRec164[0] = ((*reinterpret_cast<int*>(&fTemp493) & 2139095040) ? fTemp493 : 0.0f);
			int iTemp494 = static_cast<int>(fRec164[0]);
			iVec21[0] = iTemp494;
			int iTemp495 = iTemp494 == 0;
			float fTemp496 = 0.8f * fTemp136 * fRec160[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp479) % iTemp478 - iTemp477)) % iConst18))) & 2097151];
			float fTemp497 = ((iTemp92) ? 0.0f : fRec165[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp482));
			float fTemp498 = fTemp497 - std::floor(fTemp497);
			fRec165[0] = ((*reinterpret_cast<int*>(&fTemp498) & 2139095040) ? fTemp498 : 0.0f);
			float fTemp499 = fRec160[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fRec165[0] * fTemp479) % iTemp478 + iTemp485) % iTemp478 - iTemp477)) % iConst18))) & 2097151] * fTemp487;
			float fTemp500 = fTemp487 * fRec160[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp485 + static_cast<int>(fTemp479 * (1.0f - fRec162[0])) % iTemp478) % iTemp478 - iTemp477)) % iConst18))) & 2097151];
			float fTemp501 = fConst26 * static_cast<float>(std::abs(iTemp494 - iVec21[1]) > 0) + fConst25 * fRec166[1];
			fRec166[0] = ((*reinterpret_cast<int*>(&fTemp501) & 2139095040) ? fTemp501 : 0.0f);
			float fTemp502 = std::max<float>(-1.0f, std::min<float>(1.0f, 1.02f * (fTemp64 * (((iSlow31) ? fTemp488 : fTemp143 * ((1.0f - fRec166[0]) * ((iVec21[1] == 0) ? fTemp488 : ((iVec21[1] == 1) ? fTemp500 : ((iVec21[1] == 2) ? fTemp499 : fTemp496))) + fRec166[0] * ((iTemp495) ? fTemp488 : ((iTemp494 == 1) ? fTemp500 : ((iTemp494 == 2) ? fTemp499 : fTemp496)))) * ((iTemp495) ? 1.0f : fTemp133)) * (1.0f - fTemp491) + fTemp491 * (0.35f * fTemp469 + 0.65f * fTemp488)) + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp472)))));
			float fTemp503 = fRec176[1] + fConst8 * (fRec142 - fRec177[1]);
			float fTemp504 = fConst10 * fTemp503 - fRec176[1];
			fRec176[0] = ((*reinterpret_cast<int*>(&fTemp504) & 2139095040) ? fTemp504 : 0.0f);
			float fTemp505 = fRec177[1] + fConst11 * fTemp503;
			float fTemp506 = 2.0f * fTemp505 - fRec177[1];
			fRec177[0] = ((*reinterpret_cast<int*>(&fTemp506) & 2139095040) ? fTemp506 : 0.0f);
			float fRec178 = ((*reinterpret_cast<int*>(&fTemp505) & 2139095040) ? fTemp505 : 0.0f);
			float fTemp507 = fRec173[1] + fConst13 * (fRec178 - fRec174[1]);
			float fTemp508 = fConst15 * fTemp507 - fRec173[1];
			fRec173[0] = ((*reinterpret_cast<int*>(&fTemp508) & 2139095040) ? fTemp508 : 0.0f);
			float fTemp509 = fRec174[1] + fConst16 * fTemp507;
			float fTemp510 = 2.0f * fTemp509 - fRec174[1];
			fRec174[0] = ((*reinterpret_cast<int*>(&fTemp510) & 2139095040) ? fTemp510 : 0.0f);
			float fTemp511 = fConst17 * fTemp507;
			float fRec175 = ((*reinterpret_cast<int*>(&fTemp511) & 2139095040) ? fTemp511 : 0.0f);
			float fTemp512 = fRec178 - 2.828427f * fRec175;
			fVec22[IOTA0 & 2097151] = fTemp512;
			float fTemp513 = std::fabs(fTemp512);
			float fTemp514 = std::max<float>(fConst7 * fRec172[1], fTemp513);
			fRec172[0] = ((*reinterpret_cast<int*>(&fTemp514) & 2139095040) ? fTemp514 : 0.0f);
			float fTemp515 = std::min<float>(1.0f, 4.0f * fRec172[0]);
			float fTemp516 = std::max<float>(fConst19 * fRec179[1], fTemp513);
			fRec179[0] = ((*reinterpret_cast<int*>(&fTemp516) & 2139095040) ? fTemp516 : 0.0f);
			float fTemp517 = std::min<float>(1.0f, 3.3333333f * fRec179[0]);
			float fTemp518 = 1.0f - fTemp517;
			int iTemp519 = std::max<int>(1, std::min<int>(static_cast<int>(fSlow34 * fTemp78 * fTemp247 * (fTemp76 - fTemp77 * Freeze95DSP_faustpower2_f(fRec101[0] * (1.0f - 0.15f * fTemp518) + 0.045f * fTemp518))), iConst18));
			int iTemp520 = std::max<int>(1, iTemp519);
			float fTemp521 = static_cast<float>(iTemp519);
			float fTemp522 = fTemp264 / std::max<float>(fConst21, fConst21 * fTemp521);
			float fTemp523 = ((iTemp92) ? 0.0f : fRec180[1] + fConst21 * std::min<float>(5e+02f, fTemp522));
			float fTemp524 = fTemp523 - std::floor(fTemp523);
			fRec180[0] = ((*reinterpret_cast<int*>(&fTemp524) & 2139095040) ? fTemp524 : 0.0f);
			int iTemp525 = static_cast<int>(fRec180[0] * fTemp521) % iTemp520;
			int iTemp526 = iTemp259 + iTemp519;
			float fTemp527 = static_cast<float>(iTemp525 % iTemp273) / fTemp274;
			float fTemp528 = std::max<float>(0.05f, ((iTemp270) ? std::sin(3.1415927f * fTemp527) : ((iTemp271) ? 1.0f - std::fabs(2.0f * fTemp527 + -1.0f) : std::pow(fTemp527, 0.35f) * (1.0f - Freeze95DSP_faustpower2_f(fTemp527)))));
			float fTemp529 = fTemp528 * fVec22[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fTemp105 * static_cast<float>((iTemp526 + iTemp525) % iTemp520) + fTemp82 * static_cast<float>(iTemp519 + -1)) % iTemp520 - iTemp519)) % iConst18))) & 2097151];
			int iTemp530 = fTemp517 > 0.65f;
			iVec23[0] = iTemp530;
			float fTemp531 = fConst24 * fRec181[1] + std::max<float>(0.0f, static_cast<float>(iTemp530 - iVec23[1]));
			fRec181[0] = ((*reinterpret_cast<int*>(&fTemp531) & 2139095040) ? fTemp531 : 0.0f);
			float fTemp532 = fRec3[0] * std::min<float>(0.35f, fRec181[0] * fTemp115 * (0.4f * fTemp517 + 0.28f));
			float fTemp533 = std::min<float>(1.0f, std::max<float>(0.0f, fRec110[0] + 0.2f * (fTemp515 + -0.5f)));
			float fTemp534 = 0.001f * static_cast<float>(((iSlow8) ? 0 : ((fTemp533 < fTemp121) ? 0 : ((fTemp533 < fTemp127) ? 1 : ((fTemp533 < fTemp129) ? 2 : 3))))) + 0.999f * fRec182[1];
			fRec182[0] = ((*reinterpret_cast<int*>(&fTemp534) & 2139095040) ? fTemp534 : 0.0f);
			int iTemp535 = static_cast<int>(fRec182[0]);
			iVec24[0] = iTemp535;
			int iTemp536 = iTemp535 == 0;
			float fTemp537 = 0.8f * fTemp136 * fVec22[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + static_cast<int>(fRec56[0] * fTemp521) % iTemp520 - iTemp519)) % iConst18))) & 2097151];
			float fTemp538 = ((iTemp92) ? 0.0f : fRec183[1] + fConst21 * std::min<float>(5e+02f, 0.001f * fTemp522));
			float fTemp539 = fTemp538 - std::floor(fTemp538);
			fRec183[0] = ((*reinterpret_cast<int*>(&fTemp539) & 2139095040) ? fTemp539 : 0.0f);
			float fTemp540 = fVec22[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (static_cast<int>(fRec183[0] * fTemp521) % iTemp520 + iTemp526) % iTemp520 - iTemp519)) % iConst18))) & 2097151] * fTemp528;
			float fTemp541 = fTemp528 * fVec22[(IOTA0 - std::min<int>(iConst18, std::max<int>(0, (iConst18 + (iRec47[0] + (iTemp526 + static_cast<int>(fTemp521 * (1.0f - fRec180[0])) % iTemp520) % iTemp520 - iTemp519)) % iConst18))) & 2097151];
			float fTemp542 = fConst26 * static_cast<float>(std::abs(iTemp535 - iVec24[1]) > 0) + fConst25 * fRec184[1];
			fRec184[0] = ((*reinterpret_cast<int*>(&fTemp542) & 2139095040) ? fTemp542 : 0.0f);
			float fTemp543 = std::max<float>(-1.0f, std::min<float>(1.0f, 0.98f * (fTemp64 * (((iSlow31) ? fTemp529 : fTemp143 * ((1.0f - fRec184[0]) * ((iVec24[1] == 0) ? fTemp529 : ((iVec24[1] == 1) ? fTemp541 : ((iVec24[1] == 2) ? fTemp540 : fTemp537))) + fRec184[0] * ((iTemp536) ? fTemp529 : ((iTemp535 == 1) ? fTemp541 : ((iTemp535 == 2) ? fTemp540 : fTemp537)))) * ((iTemp536) ? 1.0f : fTemp133)) * (1.0f - fTemp532) + fTemp532 * (0.35f * fTemp512 + 0.65f * fTemp529)) + 1.3969839e-12f * fTemp65 * (1.0f - std::min<float>(0.8f, 0.8f * fTemp515)))));
			float fTemp544 = 6.0f * fTemp210 * std::fabs(fTemp543) + 1.0f;
			float fTemp545 = fTemp293 * fTemp543;
			float fTemp546 = fTemp545 / fTemp544;
			float fTemp547 = fTemp544 * (fTemp296 * std::fabs(fTemp546) + 1.0f);
			float fTemp548 = ((iTemp230) ? fTemp545 / fTemp547 : fRec171[1]);
			fRec171[0] = ((*reinterpret_cast<int*>(&fTemp548) & 2139095040) ? fTemp548 : 0.0f);
			float fTemp549 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec171[0] * fTemp229 + 0.5f) / fTemp229));
			float fTemp550 = ((iTemp230) ? fTemp546 : fRec185[1]);
			fRec185[0] = ((*reinterpret_cast<int*>(&fTemp550) & 2139095040) ? fTemp550 : 0.0f);
			float fTemp551 = fTemp293 * fTemp229 * fTemp543;
			float fTemp552 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp551 / fTemp547 + 0.5f) / fTemp229));
			float fTemp553 = fTemp228 * (fTemp303 * (0.65f * fTemp302 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp551 / fTemp544 + 0.5f) / fTemp229)) + fTemp300 * fTemp552 / std::max<float>(0.05f, 1.0f - fTemp296 * std::min<float>(0.999f, std::fabs(fTemp552)))) + fTemp212 * (0.65f * fTemp302 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec185[0] * fTemp229 + 0.5f) / fTemp229)) + fTemp300 * fTemp549 / std::max<float>(0.05f, 1.0f - fTemp296 * std::min<float>(0.999f, std::fabs(fTemp549)))));
			fVec25[0] = fTemp553;
			float fTemp554 = (fTemp307 * fRec170[1] - (fTemp553 + fVec25[1])) / fTemp227;
			float n9 = -fTemp554; 			fRec170[0] = ((*reinterpret_cast<int*>(&n9) & 2139095040) ? n9 : 0.0f);
			float fTemp555 = (fTemp309 * fRec169[1] - (fRec170[0] - fRec170[1]) / fTemp223) / fTemp225;
			float n10 = -fTemp555; 			fRec169[0] = ((*reinterpret_cast<int*>(&n10) & 2139095040) ? n10 : 0.0f);
			float fTemp556 = fRec170[0] + fRec169[0] * fTemp222;
			float fTemp557 = ((iTemp220) ? fTemp556 : fRec168[1]);
			fRec168[0] = ((*reinterpret_cast<int*>(&fTemp557) & 2139095040) ? fTemp557 : 0.0f);
			float fTemp558 = (fTemp313 * fRec167[1] - (fRec168[0] + fRec168[1])) / fTemp219;
			float n11 = -fTemp558; 			fRec167[0] = ((*reinterpret_cast<int*>(&n11) & 2139095040) ? n11 : 0.0f);
			float fTemp559 = 6.0f * fTemp145 * std::fabs(fTemp502) + 1.0f;
			float fTemp560 = fTemp337 * fTemp502;
			float fTemp561 = fTemp560 / fTemp559;
			float fTemp562 = fTemp559 * (fTemp340 * std::fabs(fTemp561) + 1.0f);
			float fTemp563 = ((iTemp335) ? fTemp560 / fTemp562 : fRec190[1]);
			fRec190[0] = ((*reinterpret_cast<int*>(&fTemp563) & 2139095040) ? fTemp563 : 0.0f);
			float fTemp564 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec190[0] * fTemp334 + 0.5f) / fTemp334));
			float fTemp565 = ((iTemp335) ? fTemp561 : fRec191[1]);
			fRec191[0] = ((*reinterpret_cast<int*>(&fTemp565) & 2139095040) ? fTemp565 : 0.0f);
			float fTemp566 = fTemp337 * fTemp334 * fTemp502;
			float fTemp567 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp566 / fTemp562 + 0.5f) / fTemp334));
			float fTemp568 = fTemp333 * (fTemp347 * (0.65f * fTemp346 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp566 / fTemp559 + 0.5f) / fTemp334)) + fTemp344 * fTemp567 / std::max<float>(0.05f, 1.0f - fTemp340 * std::min<float>(0.999f, std::fabs(fTemp567)))) + fTemp147 * (0.65f * fTemp346 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec191[0] * fTemp334 + 0.5f) / fTemp334)) + fTemp344 * fTemp564 / std::max<float>(0.05f, 1.0f - fTemp340 * std::min<float>(0.999f, std::fabs(fTemp564)))));
			fVec26[0] = fTemp568;
			float fTemp569 = (fTemp351 * fRec189[1] - (fTemp568 + fVec26[1])) / fTemp332;
			float n12 = -fTemp569; 			fRec189[0] = ((*reinterpret_cast<int*>(&n12) & 2139095040) ? n12 : 0.0f);
			float fTemp570 = (fTemp353 * fRec188[1] - (fRec189[0] - fRec189[1]) / fTemp328) / fTemp330;
			float n13 = -fTemp570; 			fRec188[0] = ((*reinterpret_cast<int*>(&n13) & 2139095040) ? n13 : 0.0f);
			float fTemp571 = fRec189[0] + fRec188[0] * fTemp327;
			float fTemp572 = ((iTemp325) ? fTemp571 : fRec187[1]);
			fRec187[0] = ((*reinterpret_cast<int*>(&fTemp572) & 2139095040) ? fTemp572 : 0.0f);
			float fTemp573 = (fTemp357 * fRec186[1] - (fRec187[0] + fRec187[1])) / fTemp324;
			float n14 = -fTemp573; 			fRec186[0] = ((*reinterpret_cast<int*>(&n14) & 2139095040) ? n14 : 0.0f);
			float fTemp574 = 6.0f * fTemp21 * std::fabs(fTemp464) + 1.0f;
			float fTemp575 = fTemp379 * fTemp464;
			float fTemp576 = fTemp575 / fTemp574;
			float fTemp577 = fTemp574 * (fTemp382 * std::fabs(fTemp576) + 1.0f);
			float fTemp578 = ((iTemp377) ? fTemp575 / fTemp577 : fRec196[1]);
			fRec196[0] = ((*reinterpret_cast<int*>(&fTemp578) & 2139095040) ? fTemp578 : 0.0f);
			float fTemp579 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec196[0] * fTemp376 + 0.5f) / fTemp376));
			float fTemp580 = ((iTemp377) ? fTemp576 : fRec197[1]);
			fRec197[0] = ((*reinterpret_cast<int*>(&fTemp580) & 2139095040) ? fTemp580 : 0.0f);
			float fTemp581 = fTemp379 * fTemp376 * fTemp464;
			float fTemp582 = std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp581 / fTemp577 + 0.5f) / fTemp376));
			float fTemp583 = fTemp375 * (fTemp389 * (0.65f * fTemp388 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fTemp581 / fTemp574 + 0.5f) / fTemp376)) + fTemp386 * fTemp582 / std::max<float>(0.05f, 1.0f - fTemp382 * std::min<float>(0.999f, std::fabs(fTemp582)))) + fTemp23 * (0.65f * fTemp388 * std::min<float>(1.0f, std::max<float>(-1.0f, std::floor(fRec197[0] * fTemp376 + 0.5f) / fTemp376)) + fTemp386 * fTemp579 / std::max<float>(0.05f, 1.0f - fTemp382 * std::min<float>(0.999f, std::fabs(fTemp579)))));
			fVec27[0] = fTemp583;
			float fTemp584 = (fTemp393 * fRec195[1] - (fTemp583 + fVec27[1])) / fTemp374;
			float n15 = -fTemp584; 			fRec195[0] = ((*reinterpret_cast<int*>(&n15) & 2139095040) ? n15 : 0.0f);
			float fTemp585 = (fTemp395 * fRec194[1] - (fRec195[0] - fRec195[1]) / fTemp370) / fTemp372;
			float n16 = -fTemp585; 			fRec194[0] = ((*reinterpret_cast<int*>(&n16) & 2139095040) ? n16 : 0.0f);
			float fTemp586 = fRec195[0] + fRec194[0] * fTemp369;
			float fTemp587 = ((iTemp367) ? fTemp586 : fRec193[1]);
			fRec193[0] = ((*reinterpret_cast<int*>(&fTemp587) & 2139095040) ? fTemp587 : 0.0f);
			float fTemp588 = (fTemp399 * fRec192[1] - (fRec193[0] + fRec193[1])) / fTemp366;
			float n17 = -fTemp588; 			fRec192[0] = ((*reinterpret_cast<int*>(&n17) & 2139095040) ? n17 : 0.0f);
			float fTemp589 = fTemp405 * fTemp402;
			fVec28[IOTA0 & 2047] = fTemp589 + fRec3[0] * (fTemp24 * (fTemp401 * (fTemp379 * fTemp388 * fTemp464 / fTemp574 + fTemp21 * fTemp586) + fRec192[0] * fTemp21 * fTemp360) + fTemp148 * (fTemp359 * (fTemp337 * fTemp346 * fTemp502 / fTemp559 + fTemp145 * fTemp571) + fRec186[0] * fTemp145 * fTemp318) + fTemp317 * fTemp543 + fTemp316 * (fTemp315 * (fTemp293 * fTemp302 * fTemp543 / fTemp544 + fTemp210 * fTemp556) + fRec167[0] * fTemp210 * fTemp213) + fTemp149 * fTemp502 + fTemp25 * fTemp464);
			float fTemp590 = fVec28[(IOTA0 - std::min<int>(1024, std::max<int>(0, iTemp14))) & 2047];
			float fTemp591 = 1.0f - fTemp19;
			fVec29[IOTA0 & 4095] = fTemp591 * fTemp590 + fTemp404 * fTemp18 * fTemp6;
			float fTemp592 = ((iTemp66) ? fTemp12 : fRec199[1]);
			fRec199[0] = ((*reinterpret_cast<int*>(&fTemp592) & 2139095040) ? fTemp592 : 0.0f);
			float fTemp593 = fConst30 * (fRec199[0] + -0.5f) + fConst29 * fRec198[1];
			fRec198[0] = ((*reinterpret_cast<int*>(&fTemp593) & 2139095040) ? fTemp593 : 0.0f);
			float fTemp594 = std::max<float>(0.0f, fRec2[0] + -0.32f);
			int iTemp595 = std::max<int>(0, static_cast<int>(fConst31 * fRec3[0] * std::fabs(fRec198[0]) * fTemp594 * (5.0f * fTemp1 + 0.4f)));
			float fTemp596 = fVec29[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec198[0] < 0.0f) ? iTemp595 : 0)))) & 4095];
			fVec30[IOTA0 & 4095] = fTemp404 * fTemp591 + fTemp19 * fTemp590;
			float fTemp597 = fVec30[(IOTA0 - std::min<int>(2048, std::max<int>(0, ((fRec198[0] >= 0.0f) ? iTemp595 : 0)))) & 4095];
			float fTemp598 = fTemp597 + fTemp596;
			float fTemp599 = (0.5f * (fTemp597 - fTemp596) + 0.7352941f * fRec3[0] * fRec198[0] * fTemp594 * fTemp598 * (0.16f * fTemp1 + 0.06f)) * (1.0f - 1.2f * fTemp19);
			float fTemp600 = 0.5f * fTemp598;
			fVec31[IOTA0 & 511] = (fTemp403 + fRec3[0] * (fTemp600 + fTemp599)) * fTemp5;
			float fTemp601 = fVec31[(IOTA0 - iConst32) & 511];
			float fTemp602 = std::max<float>(fRec1[1] * fTemp3, std::fabs(fTemp601));
			fRec1[0] = ((*reinterpret_cast<int*>(&fTemp602) & 2139095040) ? fTemp602 : 0.0f);
			float fTemp603 = 0.08f * fTemp2;
			float fTemp604 = std::max<float>(1e-06f, fTemp603);
			float fTemp605 = std::min<float>(1.0f, std::max<float>(0.0f, fRec1[0] + fTemp603 + -0.98f) / fTemp604) * (0.98f / std::max<float>(0.98f, fRec1[0]) + -1.0f) + 1.0f;
			float fTemp606 = Freeze95DSP_faustpower2_f(fTemp601) * Freeze95DSP_faustpower2_f(fTemp605);
			float fTemp607 = fTemp601 * fTemp605 * (fTemp606 + 27.0f) / (9.0f * fTemp606 + 27.0f);
			fVec32[0] = fTemp607;
			float fTemp608 = 0.995f * fRec0[1] + fTemp607 - fVec32[1];
			fRec0[0] = ((*reinterpret_cast<int*>(&fTemp608) & 2139095040) ? fTemp608 : 0.0f);
			output0[i0] = static_cast<FAUSTFLOAT>(fRec0[0]);
			fVec33[IOTA0 & 511] = fTemp5 * (fTemp589 + fRec3[0] * (fTemp600 - fTemp599));
			float fTemp609 = fVec33[(IOTA0 - iConst32) & 511];
			float fTemp610 = std::max<float>(fTemp3 * fRec201[1], std::fabs(fTemp609));
			fRec201[0] = ((*reinterpret_cast<int*>(&fTemp610) & 2139095040) ? fTemp610 : 0.0f);
			float fTemp611 = std::min<float>(1.0f, std::max<float>(0.0f, fTemp603 + fRec201[0] + -0.98f) / fTemp604) * (0.98f / std::max<float>(0.98f, fRec201[0]) + -1.0f) + 1.0f;
			float fTemp612 = Freeze95DSP_faustpower2_f(fTemp609) * Freeze95DSP_faustpower2_f(fTemp611);
			float fTemp613 = fTemp609 * fTemp611 * (fTemp612 + 27.0f) / (9.0f * fTemp612 + 27.0f);
			fVec34[0] = fTemp613;
			float fTemp614 = 0.995f * fRec200[1] + fTemp613 - fVec34[1];
			fRec200[0] = ((*reinterpret_cast<int*>(&fTemp614) & 2139095040) ? fTemp614 : 0.0f);
			output1[i0] = static_cast<FAUSTFLOAT>(fRec200[0]);
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
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			iRec30[1] = iRec30[0];
			fRec29[1] = fRec29[0];
			iRec31[1] = iRec31[0];
			iRec32[1] = iRec32[0];
			iRec35[1] = iRec35[0];
			iRec36[1] = iRec36[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec37[1] = fRec37[0];
			iRec39[1] = iRec39[0];
			fRec38[1] = fRec38[0];
			iRec42[1] = iRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			iRec44[1] = iRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			iRec47[1] = iRec47[0];
			iRec49[1] = iRec49[0];
			fRec48[1] = fRec48[0];
			fRec50[1] = fRec50[0];
			iVec6[1] = iVec6[0];
			fRec51[1] = fRec51[0];
			fRec53[1] = fRec53[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec52[1] = fRec52[0];
			iVec7[1] = iVec7[0];
			fRec56[1] = fRec56[0];
			fRec57[1] = fRec57[0];
			fRec58[1] = fRec58[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec59[1] = fRec59[0];
			fRec63[1] = fRec63[0];
			iRec65[1] = iRec65[0];
			fRec64[1] = fRec64[0];
			iRec68[1] = iRec68[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			iRec72[1] = iRec72[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec73[1] = fRec73[0];
			fRec69[1] = fRec69[0];
			iRec75[1] = iRec75[0];
			fRec74[1] = fRec74[0];
			iRec77[1] = iRec77[0];
			fRec76[1] = fRec76[0];
			fRec78[1] = fRec78[0];
			iVec8[1] = iVec8[0];
			fRec79[1] = fRec79[0];
			fRec80[1] = fRec80[0];
			iVec9[1] = iVec9[0];
			fRec81[1] = fRec81[0];
			fRec82[1] = fRec82[0];
			iRec85[1] = iRec85[0];
			iRec89[1] = iRec89[0];
			fRec94[1] = fRec94[0];
			fRec95[1] = fRec95[0];
			fRec91[1] = fRec91[0];
			fRec92[1] = fRec92[0];
			fRec90[1] = fRec90[0];
			iRec99[1] = iRec99[0];
			fRec98[1] = fRec98[0];
			fRec97[1] = fRec97[0];
			fRec100[1] = fRec100[0];
			iRec102[1] = iRec102[0];
			fRec101[1] = fRec101[0];
			iRec105[1] = iRec105[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			iRec107[1] = iRec107[0];
			fRec106[1] = fRec106[0];
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
			iRec111[1] = iRec111[0];
			fRec110[1] = fRec110[0];
			fRec112[1] = fRec112[0];
			iVec11[1] = iVec11[0];
			fRec113[1] = fRec113[0];
			fRec114[1] = fRec114[0];
			iVec12[1] = iVec12[0];
			fRec115[1] = fRec115[0];
			fRec116[1] = fRec116[0];
			fRec88[1] = fRec88[0];
			fRec117[1] = fRec117[0];
			fVec13[1] = fVec13[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			iRec120[1] = iRec120[0];
			iRec124[1] = iRec124[0];
			fRec123[1] = fRec123[0];
			fRec125[1] = fRec125[0];
			fVec14[1] = fVec14[0];
			fRec122[1] = fRec122[0];
			fRec121[1] = fRec121[0];
			fRec119[1] = fRec119[0];
			fRec118[1] = fRec118[0];
			iRec128[1] = iRec128[0];
			iRec132[1] = iRec132[0];
			fRec131[1] = fRec131[0];
			fRec133[1] = fRec133[0];
			fVec15[1] = fVec15[0];
			fRec130[1] = fRec130[0];
			fRec129[1] = fRec129[0];
			fRec127[1] = fRec127[0];
			fRec126[1] = fRec126[0];
			fRec139[1] = fRec139[0];
			fRec140[1] = fRec140[0];
			fRec135[1] = fRec135[0];
			fRec136[1] = fRec136[0];
			fRec143[1] = fRec143[0];
			fRec144[1] = fRec144[0];
			fRec147[1] = fRec147[0];
			fRec148[1] = fRec148[0];
			fRec134[1] = fRec134[0];
			fRec151[1] = fRec151[0];
			fRec152[1] = fRec152[0];
			iVec18[1] = iVec18[0];
			fRec153[1] = fRec153[0];
			fRec154[1] = fRec154[0];
			iVec19[1] = iVec19[0];
			fRec155[1] = fRec155[0];
			fRec156[1] = fRec156[0];
			fRec158[1] = fRec158[0];
			fRec159[1] = fRec159[0];
			fRec157[1] = fRec157[0];
			fRec161[1] = fRec161[0];
			fRec162[1] = fRec162[0];
			iVec20[1] = iVec20[0];
			fRec163[1] = fRec163[0];
			fRec164[1] = fRec164[0];
			iVec21[1] = iVec21[0];
			fRec165[1] = fRec165[0];
			fRec166[1] = fRec166[0];
			fRec176[1] = fRec176[0];
			fRec177[1] = fRec177[0];
			fRec173[1] = fRec173[0];
			fRec174[1] = fRec174[0];
			fRec172[1] = fRec172[0];
			fRec179[1] = fRec179[0];
			fRec180[1] = fRec180[0];
			iVec23[1] = iVec23[0];
			fRec181[1] = fRec181[0];
			fRec182[1] = fRec182[0];
			iVec24[1] = iVec24[0];
			fRec183[1] = fRec183[0];
			fRec184[1] = fRec184[0];
			fRec171[1] = fRec171[0];
			fRec185[1] = fRec185[0];
			fVec25[1] = fVec25[0];
			fRec170[1] = fRec170[0];
			fRec169[1] = fRec169[0];
			fRec168[1] = fRec168[0];
			fRec167[1] = fRec167[0];
			fRec190[1] = fRec190[0];
			fRec191[1] = fRec191[0];
			fVec26[1] = fVec26[0];
			fRec189[1] = fRec189[0];
			fRec188[1] = fRec188[0];
			fRec187[1] = fRec187[0];
			fRec186[1] = fRec186[0];
			fRec196[1] = fRec196[0];
			fRec197[1] = fRec197[0];
			fVec27[1] = fVec27[0];
			fRec195[1] = fRec195[0];
			fRec194[1] = fRec194[0];
			fRec193[1] = fRec193[0];
			fRec192[1] = fRec192[0];
			fRec199[1] = fRec199[0];
			fRec198[1] = fRec198[0];
			fRec1[1] = fRec1[0];
			fVec32[1] = fVec32[0];
			fRec0[1] = fRec0[0];
			fRec201[1] = fRec201[0];
			fVec34[1] = fVec34[0];
			fRec200[1] = fRec200[0];
		}
	}

};

#endif
