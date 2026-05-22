/* ------------------------------------------------------------
name: "Grungr"
version: "2.6"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
------------------------------------------------------------ */

#ifndef  __GrungrFaustDSP_H__
#define  __GrungrFaustDSP_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS GrungrFaustDSP
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

static double GrungrFaustDSP_faustpower2_f(double value) {
	return value * value;
}
static double GrungrFaustDSP_faustpower3_f(double value) {
	return value * value * value;
}

class GrungrFaustDSP : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	FAUSTFLOAT fHslider0;
	double fRec0[2];
	FAUSTFLOAT fCheckbox1;
	double fRec1[2];
	FAUSTFLOAT fHslider1;
	double fRec4[2];
	double fConst3;
	double fConst4;
	double fConst5;
	double fConst6;
	double fConst7;
	double fConst8;
	double fConst9;
	double fConst10;
	double fConst11;
	double fConst12;
	double fConst13;
	double fConst14;
	double fConst15;
	double fConst16;
	double fConst17;
	double fConst18;
	double fConst19;
	double fConst20;
	double fConst21;
	double fConst22;
	double fConst23;
	double fConst24;
	double fConst25;
	double fConst26;
	double fConst27;
	double fConst28;
	double fConst29;
	double fConst30;
	double fConst31;
	double fConst32;
	double fConst33;
	double fConst34;
	double fConst35;
	double fConst36;
	double fConst37;
	double fVec0[2];
	double fConst38;
	double fConst39;
	double fRec29[2];
	double fConst40;
	double fConst41;
	double fConst42;
	double fRec28[2];
	double fVec1[3];
	double fVec2[2];
	double fConst43;
	double fConst44;
	double fRec27[2];
	double fConst45;
	double fRec26[2];
	double fVec3[3];
	double fVec4[2];
	double fRec25[2];
	double fRec24[3];
	double fVec5[2];
	double fRec23[2];
	double fRec22[2];
	double fRec21[2];
	double fVec6[3];
	double fVec7[2];
	double fRec20[2];
	double fRec19[3];
	double fVec8[2];
	double fRec18[2];
	double fRec17[2];
	double fRec16[2];
	double fVec9[3];
	double fVec10[3];
	double fVec11[3];
	double fVec12[3];
	double fRec15[3];
	double fVec13[2];
	double fRec14[2];
	double fConst46;
	double fConst47;
	double fRec13[2];
	double fRec12[3];
	FAUSTFLOAT fHslider2;
	double fRec30[2];
	double fRec32[2];
	double fRec31[3];
	double fVec14[2];
	double fConst48;
	double fConst49;
	double fConst50;
	double fRec11[2];
	double fRec10[3];
	double fRec34[2];
	double fRec33[3];
	double fRec9[3];
	double fRec8[3];
	double fRec7[3];
	double fVec15[2];
	double fConst51;
	double fConst52;
	double fRec6[2];
	double fRec5[3];
	FAUSTFLOAT fHslider3;
	double fRec35[2];
	double fRec3[2];
	double fRec2[2];
	double fVec16[3];
	double fVec17[2];
	double fRec62[2];
	double fRec61[2];
	double fVec18[3];
	double fVec19[2];
	double fRec60[2];
	double fRec59[2];
	double fVec20[3];
	double fVec21[2];
	double fRec58[2];
	double fRec57[3];
	double fVec22[2];
	double fRec56[2];
	double fRec55[2];
	double fRec54[2];
	double fVec23[3];
	double fVec24[2];
	double fRec53[2];
	double fRec52[3];
	double fVec25[2];
	double fRec51[2];
	double fRec50[2];
	double fRec49[2];
	double fVec26[3];
	double fVec27[3];
	double fVec28[3];
	double fVec29[3];
	double fRec48[3];
	double fVec30[2];
	double fRec47[2];
	double fRec46[2];
	double fRec45[3];
	double fRec64[2];
	double fRec63[3];
	double fVec31[2];
	double fRec44[2];
	double fRec43[3];
	double fRec66[2];
	double fRec65[3];
	double fRec42[3];
	double fRec41[3];
	double fRec40[3];
	double fVec32[2];
	double fRec39[2];
	double fRec38[3];
	double fRec37[2];
	double fRec36[2];
	double fVec33[3];
	
 public:
	GrungrFaustDSP() {
	}
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA2:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA2:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA2:license", "MIT License");
		m->declare("aanl.lib/arctan2:author", "Dario Sanfilippo");
		m->declare("aanl.lib/arctan2:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/arctan2:license", "MIT License");
		m->declare("aanl.lib/hardclip2:author", "Dario Sanfilippo");
		m->declare("aanl.lib/hardclip2:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/hardclip2:license", "MIT License");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.4.2");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.3.0");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("filename", "Grungr.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/highshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/low_shelf:author", "Julius O. Smith III");
		m->declare("filters.lib/low_shelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/low_shelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/lowshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/peak_eq_cq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_cq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_cq:license", "MIT-style STK-4.3 license");
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
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "Grungr");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "2.6");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
		fConst1 = 44.1 / fConst0;
		fConst2 = 1.0 - fConst1;
		fConst3 = 3.141592653589793 / fConst0;
		fConst4 = std::tan(94.24777960769379 / fConst0);
		fConst5 = 1.0 / fConst4;
		fConst6 = 1.0 - fConst5;
		fConst7 = 69.11503837897544 / (fConst0 * std::sin(345.57519189487726 / fConst0));
		fConst8 = std::tan(172.78759594743863 / fConst0);
		fConst9 = 1.0 / fConst8;
		fConst10 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst8));
		fConst11 = 6326.818538479444 / (fConst0 * std::sin(9110.6186954104 / fConst0));
		fConst12 = std::tan(4555.3093477052 / fConst0);
		fConst13 = 1.0 / fConst12;
		fConst14 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst12));
		fConst15 = 2050.3025739217596 / (fConst0 * std::sin(3895.5748904513434 / fConst0));
		fConst16 = std::tan(1947.7874452256717 / fConst0);
		fConst17 = 1.0 / fConst16;
		fConst18 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst16));
		fConst19 = std::tan(9424.77796076938 / fConst0);
		fConst20 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst19);
		fConst21 = 2.0 * (1.0 - fConst20);
		fConst22 = 1.0 / fConst19;
		fConst23 = (fConst22 + -1.0000000000000004) / fConst19 + 1.0;
		fConst24 = 1.0 / ((fConst22 + 1.0000000000000004) / fConst19 + 1.0);
		fConst25 = std::tan(376.99111843077515 / fConst0);
		fConst26 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst25);
		fConst27 = 2.0 * (1.0 - fConst26);
		fConst28 = 1.0 / fConst25;
		fConst29 = (fConst28 + -1.0000000000000004) / fConst25 + 1.0;
		fConst30 = (fConst28 + 1.0000000000000004) / fConst25 + 1.0;
		fConst31 = 1.0 / fConst30;
		fConst32 = std::tan(109.95574287564276 / fConst0);
		fConst33 = 1.0 / fConst32;
		fConst34 = 1.0 - fConst33;
		fConst35 = std::tan(5.026548245743669 / fConst0);
		fConst36 = 1.0 / fConst35;
		fConst37 = 1.0 - fConst36;
		fConst38 = 0.2 / fConst35;
		fConst39 = 1.0 / (fConst36 + 1.0);
		fConst40 = 1.0 / std::tan(47123.8898038469 / fConst0);
		fConst41 = 1.0 - fConst40;
		fConst42 = 1.0 / (fConst40 + 1.0);
		fConst43 = 1.1459155902616465 / fConst32;
		fConst44 = 1.0 / (fConst33 + 1.0);
		fConst45 = 1e+06 / fConst0;
		fConst46 = 1.0 - fConst28;
		fConst47 = 1.0 / (fConst28 + 1.0);
		fConst48 = 1.0 / (fConst19 * fConst30);
		fConst49 = 1.0 - fConst22;
		fConst50 = 1.0 / (fConst22 + 1.0);
		fConst51 = 0.86 / fConst4;
		fConst52 = 1.0 / (fConst5 + 1.0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0);
		fHslider0 = FAUSTFLOAT(0.7);
		fCheckbox1 = FAUSTFLOAT(0.0);
		fHslider1 = FAUSTFLOAT(0.56);
		fHslider2 = FAUSTFLOAT(0.62);
		fHslider3 = FAUSTFLOAT(0.72);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec4[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec0[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec29[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec28[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fVec1[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec2[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec27[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec26[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fVec3[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec4[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec25[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec24[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec5[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec23[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec22[l16] = 0.0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec21[l17] = 0.0;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fVec6[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec7[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec20[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec19[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec8[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec18[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec17[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec16[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fVec9[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fVec10[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fVec11[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fVec12[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec15[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec13[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec14[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec13[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec12[l34] = 0.0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec30[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec32[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec31[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fVec14[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec11[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec10[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec34[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec33[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec9[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec8[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec7[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fVec15[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec6[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec5[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec35[l49] = 0.0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec3[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec2[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fVec16[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fVec17[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec62[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec61[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fVec18[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec19[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec60[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec59[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fVec20[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fVec21[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec58[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec57[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fVec22[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec56[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec55[l66] = 0.0;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec54[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fVec23[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fVec24[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec53[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec52[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fVec25[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec51[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec50[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec49[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fVec26[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fVec27[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fVec28[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fVec29[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec48[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fVec30[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec47[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec46[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec45[l84] = 0.0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec64[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec63[l86] = 0.0;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fVec31[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec44[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 3; l89 = l89 + 1) {
			fRec43[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec66[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec65[l91] = 0.0;
		}
		for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
			fRec42[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fRec41[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec40[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fVec32[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec39[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 3; l97 = l97 + 1) {
			fRec38[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec37[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec36[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 3; l100 = l100 + 1) {
			fVec33[l100] = 0.0;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual GrungrFaustDSP* clone() {
		return new GrungrFaustDSP();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Grungr");
		ui_interface->declare(&fHslider0, "1", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Grunge", &fHslider0, FAUSTFLOAT(0.7), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider2, "2", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Butt", &fHslider2, FAUSTFLOAT(0.62), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider1, "3", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Face", &fHslider1, FAUSTFLOAT(0.56), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider3, "4", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Loud", &fHslider3, FAUSTFLOAT(0.72), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fCheckbox1, "5", "");
		ui_interface->declare(&fCheckbox1, "style", "switch");
		ui_interface->addCheckButton("Enhanced", &fCheckbox1);
		ui_interface->declare(&fCheckbox0, "6", "");
		ui_interface->declare(&fCheckbox0, "style", "switch");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(double(fCheckbox0));
		double fSlow1 = fConst1 * double(fHslider0);
		double fSlow2 = fConst1 * double(fCheckbox1);
		double fSlow3 = fConst1 * double(fHslider1);
		double fSlow4 = fConst1 * double(fHslider2);
		double fSlow5 = fConst1 * double(fHslider3);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			double fTemp0 = std::min<double>(1.0, std::max<double>(0.0, fRec0[0] * (1.32 - 0.32 * fRec0[0])));
			double fTemp1 = std::min<double>(1.0, std::max<double>(0.0, 2.2222222222222223 * (fRec0[0] + -0.55)));
			fRec1[0] = fSlow2 + fConst2 * fRec1[1];
			double fTemp2 = fRec1[0] * fTemp1;
			double fTemp3 = 0.12 * fTemp2;
			double fTemp4 = fTemp3 + 0.09 * fTemp0 + 0.64;
			double fTemp5 = 1.0 - fTemp2;
			double fTemp6 = std::max<double>(0.16000000000000003 - 0.03 * fTemp5, 2.220446049250313e-16);
			double fTemp7 = 0.04 * GrungrFaustDSP_faustpower2_f(fTemp0) + 0.016;
			double fTemp8 = std::min<double>(1.0, std::max<double>(0.0, 5.555555555555555 * (fRec0[0] + -0.82)));
			fRec4[0] = fSlow3 + fConst2 * fRec4[1];
			double fTemp9 = std::pow(std::max<double>(fRec4[0], 2.220446049250313e-16), 0.88);
			double fTemp10 = std::min<double>(1.0, std::max<double>(0.0, 1.5384615384615383 * (fRec0[0] + -0.35)));
			double fTemp11 = fTemp10 * (fTemp9 + 1.0);
			double fTemp12 = std::tan(fConst3 * (1.46e+04 - 0.5 * fTemp11 * (9e+02 * fTemp0 + 6.5e+02 * fTemp8)));
			double fTemp13 = 1.0 / fTemp12;
			double fTemp14 = (fTemp13 + 1.414213562373095) / fTemp12 + 1.0;
			double fTemp15 = fTemp2 * std::min<double>(1.0, std::max<double>(0.0, 3.3333333333333335 * (fRec0[0] + -0.7)));
			double fTemp16 = 1.0 - fTemp0;
			double fTemp17 = std::min<double>(1.0, std::max<double>(0.0, fTemp0 + fTemp16 * (0.22 * fTemp5 + 0.07) + fTemp2 * (0.1 * fTemp16 + 0.05 * fTemp8)));
			double fTemp18 = std::pow(1e+01, 0.05 * (2.1 * fTemp5 * (fTemp17 - fTemp0) + (0.95 - (1.1 * GrungrFaustDSP_faustpower2_f(fTemp8) + 0.55 * fTemp15))));
			double fTemp19 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp12);
			double fTemp20 = (fTemp13 + -1.414213562373095) / fTemp12 + 1.0;
			double fTemp21 = 3.0 * fTemp15;
			int iTemp22 = -fTemp21 > 0.0;
			double fTemp23 = fConst7 * std::pow(1e+01, 0.05 * std::fabs(-fTemp21));
			double fTemp24 = ((iTemp22) ? fConst7 : fTemp23);
			double fTemp25 = fConst9 * (fConst9 + fTemp24) + 1.0;
			double fTemp26 = ((iTemp22) ? fTemp23 : fConst7);
			double fTemp27 = fConst9 * (fConst9 - fTemp26) + 1.0;
			double fTemp28 = fConst10 * fRec7[1];
			double fTemp29 = fConst9 * (fConst9 - fTemp24) + 1.0;
			double fTemp30 = 0.5 * fTemp11 * (0.7 - 0.3 * fTemp0);
			int iTemp31 = fTemp30 > 0.0;
			double fTemp32 = fConst11 * std::pow(1e+01, 0.05 * std::fabs(fTemp30));
			double fTemp33 = ((iTemp31) ? fConst11 : fTemp32);
			double fTemp34 = fConst13 * (fConst13 + fTemp33) + 1.0;
			double fTemp35 = ((iTemp31) ? fTemp32 : fConst11);
			double fTemp36 = fConst13 * (fConst13 - fTemp35) + 1.0;
			double fTemp37 = fConst14 * fRec8[1];
			double fTemp38 = fConst13 * (fConst13 - fTemp33) + 1.0;
			double fTemp39 = fTemp10 * (0.95 - 0.55 * fTemp0) + -5.6;
			int iTemp40 = fTemp39 > 0.0;
			double fTemp41 = fConst15 * std::pow(1e+01, 0.05 * std::fabs(fTemp39));
			double fTemp42 = ((iTemp40) ? fConst15 : fTemp41);
			double fTemp43 = fConst17 * (fConst17 + fTemp42) + 1.0;
			double fTemp44 = ((iTemp40) ? fTemp41 : fConst15);
			double fTemp45 = fConst17 * (fConst17 - fTemp44) + 1.0;
			double fTemp46 = fConst18 * fRec9[1];
			double fTemp47 = fConst17 * (fConst17 - fTemp42) + 1.0;
			double fTemp48 = std::pow(1e+01, 0.05 * (19.0 * fTemp9 + -8.0));
			double fTemp49 = std::tan(fConst3 * (1.12e+04 - (1.8e+03 * fTemp17 + 9e+02 * fTemp8)));
			double fTemp50 = 1.0 / fTemp49;
			double fTemp51 = (fTemp50 + 1.414213562373095) / fTemp49 + 1.0;
			double fTemp52 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp49);
			double fTemp53 = (fTemp50 + -1.414213562373095) / fTemp49 + 1.0;
			double fTemp54 = std::max<double>(0.24 - 0.04 * fTemp5, 2.220446049250313e-16);
			double fTemp55 = std::pow(1e+01, 0.3 * (fTemp17 + 1.0));
			double fTemp56 = std::tan(fConst3 * (2e+01 * fTemp17 + 1.1e+02));
			double fTemp57 = 1.0 / fTemp56;
			double fTemp58 = fTemp57 + 1.0;
			double fTemp59 = 1.6e+03 * fTemp17;
			double fTemp60 = std::tan(fConst3 * (8.1e+03 - fTemp59));
			double fTemp61 = 1.0 / fTemp60;
			double fTemp62 = (fTemp61 + 1.414213562373095) / fTemp60 + 1.0;
			double fTemp63 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp60);
			double fTemp64 = (fTemp61 + -1.414213562373095) / fTemp60 + 1.0;
			double fTemp65 = 0.12 * fTemp17;
			double fTemp66 = 0.74 - fTemp65;
			double fTemp67 = 1.0 - 0.1 * fTemp2;
			double fTemp68 = 0.1 * fTemp0 + 1.1;
			double fTemp69 = -0.01 - 0.018 * fTemp17;
			double fTemp70 = std::atan(fTemp69 * fTemp68 * fTemp67 / fTemp66);
			double fTemp71 = std::pow(1e+01, 0.05 * (14.0 * fTemp17 + 12.0));
			double fTemp72 = 1.0 / std::tan(fConst3 * (8.4e+03 - fTemp59));
			double fTemp73 = fTemp72 + 1.0;
			double fTemp74 = std::tan(fConst3 * (25.0 * fTemp17 + 12.0 * fTemp2 + 8e+01));
			double fTemp75 = 1.0 / fTemp74;
			double fTemp76 = fTemp75 + 1.0;
			double fTemp77 = std::tan(fConst3 * (1.08e+04 - 2e+03 * fTemp17));
			double fTemp78 = 1.0 / fTemp77;
			double fTemp79 = (fTemp78 + 1.414213562373095) / fTemp77 + 1.0;
			double fTemp80 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp77);
			double fTemp81 = (fTemp78 + -1.414213562373095) / fTemp77 + 1.0;
			double fTemp82 = 0.1 * fTemp17;
			double fTemp83 = 0.76 - fTemp82;
			double fTemp84 = 1.0 - fTemp3;
			double fTemp85 = 0.05 * fTemp0 + 1.0;
			double fTemp86 = std::atan(0.014 * (fTemp85 * fTemp84 / fTemp83));
			double fTemp87 = std::pow(1e+01, 0.05 * (19.0 * fTemp17 + 18.0));
			double fTemp88 = 0.07 * fTemp0 + 0.4;
			double fTemp89 = std::atan(0.005555555555555556 * fTemp88);
			double fTemp90 = double(input0[i0]);
			double fTemp91 = ((iSlow0) ? 0.0 : fTemp90);
			fVec0[0] = fTemp91;
			fRec29[0] = fConst39 * (fConst38 * (fTemp91 - fVec0[1]) - fConst37 * fRec29[1]);
			fRec28[0] = -(fConst42 * (fConst41 * fRec28[1] - (fRec29[0] + fRec29[1])));
			double fTemp92 = (fRec28[0] + 0.01) * fTemp88;
			fVec1[0] = fTemp92;
			double fTemp93 = fVec1[1] - fVec1[2];
			double fTemp94 = 0.5555555555555556 * fTemp93;
			double fTemp95 = 0.5555555555555556 * fVec1[2];
			double fTemp96 = GrungrFaustDSP_faustpower2_f(fTemp95) + 1.0;
			double fTemp97 = std::atan(fTemp95);
			double fTemp98 = 0.5555555555555556 * fVec1[1];
			double fTemp99 = GrungrFaustDSP_faustpower2_f(fTemp98) + 1.0;
			double fTemp100 = std::atan(fTemp98);
			double fTemp101 = fTemp100 * fTemp99;
			double fTemp102 = fVec1[1] * fTemp100;
			double fTemp103 = std::log(fTemp99);
			double fTemp104 = 1.1111111111111112 * fVec1[1];
			double fTemp105 = fTemp92 - fVec1[1];
			double fTemp106 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp105);
			double fTemp107 = 0.5555555555555556 * fTemp92;
			double fTemp108 = GrungrFaustDSP_faustpower2_f(fTemp107) + 1.0;
			double fTemp109 = std::atan(fTemp107);
			double fTemp110 = ((fTemp106 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp107 + fTemp104)) : (0.5555555555555556 * fTemp92 * (0.5555555555555556 * (fTemp92 * fTemp109 - fTemp102) - 0.5 * (std::log(fTemp108) - fTemp103)) + 0.2777777777777778 * fTemp105 - 0.5 * (fTemp109 * fTemp108 - fTemp101)) / fTemp106) + ((GrungrFaustDSP_faustpower2_f(fTemp94) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp104 + fTemp95)) : (0.5555555555555556 * fVec1[2] * (0.5 * (fTemp103 - std::log(fTemp96)) - 0.5555555555555556 * (fTemp102 - fVec1[2] * fTemp97)) + 0.5 * (fTemp101 - fTemp97 * fTemp96) - 0.2777777777777778 * fTemp93) / GrungrFaustDSP_faustpower2_f(-fTemp94)) - fTemp89;
			fVec2[0] = fTemp110;
			fRec27[0] = fConst44 * (fConst43 * (fTemp110 - fVec2[1]) - fConst34 * fRec27[1]);
			double fTemp111 = fRec27[0] * fTemp87;
			double fTemp112 = fConst45 + fRec26[1];
			double fTemp113 = fRec26[1] - fConst45;
			fRec26[0] = ((fTemp112 < fTemp111) ? fTemp112 : ((fTemp113 > fTemp111) ? fTemp113 : fTemp111));
			double fTemp114 = (fRec26[0] + 0.014) * fTemp85 * fTemp84;
			double fTemp115 = fTemp114 / fTemp83;
			fVec3[0] = fTemp115;
			double fTemp116 = GrungrFaustDSP_faustpower2_f(fVec3[2]);
			double fTemp117 = std::atan(fVec3[2]);
			double fTemp118 = GrungrFaustDSP_faustpower2_f(fVec3[1]);
			double fTemp119 = std::atan(fVec3[1]);
			double fTemp120 = fTemp119 - (fVec3[1] - fTemp119 * fTemp118);
			double fTemp121 = fVec3[1] * fTemp119;
			double fTemp122 = std::log(fTemp118 + 1.0);
			double fTemp123 = 2.0 * fVec3[1];
			double fTemp124 = GrungrFaustDSP_faustpower2_f(fTemp115 - fVec3[1]);
			double fTemp125 = GrungrFaustDSP_faustpower2_f(fTemp115);
			double fTemp126 = std::atan(fTemp115);
			double fTemp127 = fTemp83 * (((fTemp124 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp115 + fTemp123)) : (fTemp114 * (fTemp114 * fTemp126 / fTemp83 - (fTemp121 + 0.5 * (std::log(fTemp125 + 1.0) - fTemp122))) / fTemp83 - 0.5 * (fTemp126 - (fTemp115 - fTemp126 * fTemp125) - fTemp120)) / fTemp124) + ((GrungrFaustDSP_faustpower2_f(fVec3[1] - fVec3[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp123 + fVec3[2])) : (fVec3[2] * (fVec3[2] * fTemp117 + 0.5 * (fTemp122 - std::log(fTemp116 + 1.0)) - fTemp121) + 0.5 * (fTemp120 - (fTemp117 - (fVec3[2] - fTemp117 * fTemp116)))) / GrungrFaustDSP_faustpower2_f(fVec3[2] - fVec3[1])) - fTemp86);
			fVec4[0] = fTemp127;
			fRec25[0] = 0.995 * fRec25[1] + 0.6366197723675814 * (fTemp127 - fVec4[1]);
			fRec24[0] = fRec25[0] - (fRec24[2] * fTemp81 + 2.0 * fRec24[1] * fTemp80) / fTemp79;
			double fTemp128 = (fRec24[2] + fRec24[0] + 2.0 * fRec24[1]) / fTemp79;
			fVec5[0] = fTemp128;
			double fTemp129 = 1.0 - fTemp75;
			fRec23[0] = -((fRec23[1] * fTemp129 - (fTemp128 - fVec5[1]) / fTemp74) / fTemp76);
			double fTemp130 = 1.0 - fTemp72;
			fRec22[0] = -((fRec22[1] * fTemp130 - (fRec23[0] + fRec23[1])) / fTemp73);
			double fTemp131 = fRec22[0] * fTemp71;
			double fTemp132 = fConst45 + fRec21[1];
			double fTemp133 = fRec21[1] - fConst45;
			fRec21[0] = ((fTemp132 < fTemp131) ? fTemp132 : ((fTemp133 > fTemp131) ? fTemp133 : fTemp131));
			double fTemp134 = (fRec21[0] + fTemp69) * fTemp68 * fTemp67;
			double fTemp135 = fTemp134 / fTemp66;
			fVec6[0] = fTemp135;
			double fTemp136 = GrungrFaustDSP_faustpower2_f(fVec6[2]);
			double fTemp137 = std::atan(fVec6[2]);
			double fTemp138 = GrungrFaustDSP_faustpower2_f(fVec6[1]);
			double fTemp139 = std::atan(fVec6[1]);
			double fTemp140 = fTemp139 - (fVec6[1] - fTemp139 * fTemp138);
			double fTemp141 = fVec6[1] * fTemp139;
			double fTemp142 = std::log(fTemp138 + 1.0);
			double fTemp143 = 2.0 * fVec6[1];
			double fTemp144 = GrungrFaustDSP_faustpower2_f(fTemp135 - fVec6[1]);
			double fTemp145 = GrungrFaustDSP_faustpower2_f(fTemp135);
			double fTemp146 = std::atan(fTemp135);
			double fTemp147 = fTemp66 * (((fTemp144 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp135 + fTemp143)) : (fTemp134 * (fTemp134 * fTemp146 / fTemp66 - (fTemp141 + 0.5 * (std::log(fTemp145 + 1.0) - fTemp142))) / fTemp66 - 0.5 * (fTemp146 - (fTemp135 - fTemp146 * fTemp145) - fTemp140)) / fTemp144) + ((GrungrFaustDSP_faustpower2_f(fVec6[1] - fVec6[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp143 + fVec6[2])) : (fVec6[2] * (fVec6[2] * fTemp137 + 0.5 * (fTemp142 - std::log(fTemp136 + 1.0)) - fTemp141) + 0.5 * (fTemp140 - (fTemp137 - (fVec6[2] - fTemp137 * fTemp136)))) / GrungrFaustDSP_faustpower2_f(fVec6[2] - fVec6[1])) - fTemp70);
			fVec7[0] = fTemp147;
			fRec20[0] = 0.995 * fRec20[1] + 0.6366197723675814 * (fTemp147 - fVec7[1]);
			fRec19[0] = fRec20[0] - (fRec19[2] * fTemp64 + 2.0 * fRec19[1] * fTemp63) / fTemp62;
			double fTemp148 = (fRec19[2] + fRec19[0] + 2.0 * fRec19[1]) / fTemp62;
			fVec8[0] = fTemp148;
			double fTemp149 = 1.0 - fTemp57;
			fRec18[0] = -((fRec18[1] * fTemp149 - (fTemp148 - fVec8[1]) / fTemp56) / fTemp58);
			double fTemp150 = fRec18[0] * fTemp55;
			double fTemp151 = fConst45 + fRec17[1];
			double fTemp152 = fRec17[1] - fConst45;
			fRec17[0] = ((fTemp151 < fTemp150) ? fTemp151 : ((fTemp152 > fTemp150) ? fTemp152 : fTemp150));
			fRec16[0] = fConst1 * std::fabs(fRec17[0]) + fConst2 * fRec16[1];
			double fTemp153 = fRec17[0] * (1.0 - fTemp54 * std::min<double>(1.0, std::max<double>(0.0, fRec1[0] * fRec16[0] * fTemp1 * fTemp7 / fTemp54)));
			int iTemp154 = fTemp153 > 0.0;
			double fTemp155 = 1.08 - 0.08 * fTemp17;
			double fTemp156 = fTemp153 / fTemp155;
			fVec9[0] = fTemp156;
			int iTemp157 = (fVec9[2] <= 1.0) & (fVec9[2] >= -1.0);
			double fTemp158 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec9[2]);
			double fTemp159 = double((fVec9[2] > 0.0) - (fVec9[2] < 0.0));
			int iTemp160 = (fVec9[1] <= 1.0) & (fVec9[1] >= -1.0);
			double fTemp161 = double((fVec9[1] > 0.0) - (fVec9[1] < 0.0));
			double fTemp162 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec9[1]);
			double fTemp163 = ((iTemp160) ? fTemp162 : fVec9[1] * fTemp161 + -0.5);
			double fTemp164 = ((iTemp160) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec9[1]) : fTemp161 * (fTemp162 + -0.16666666666666666));
			double fTemp165 = 2.0 * fVec9[1];
			double fTemp166 = GrungrFaustDSP_faustpower2_f(fTemp156 - fVec9[1]);
			int iTemp167 = (fTemp156 <= 1.0) & (fTemp156 >= -1.0);
			double fTemp168 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp156);
			double fTemp169 = double((fTemp156 > 0.0) - (fTemp156 < 0.0));
			double fTemp170 = 1.62 - fTemp65;
			double fTemp171 = fTemp153 / fTemp170;
			fVec10[0] = fTemp171;
			int iTemp172 = (fVec10[2] <= 1.0) & (fVec10[2] >= -1.0);
			double fTemp173 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec10[2]);
			double fTemp174 = double((fVec10[2] > 0.0) - (fVec10[2] < 0.0));
			int iTemp175 = (fVec10[1] <= 1.0) & (fVec10[1] >= -1.0);
			double fTemp176 = double((fVec10[1] > 0.0) - (fVec10[1] < 0.0));
			double fTemp177 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec10[1]);
			double fTemp178 = ((iTemp175) ? fTemp177 : fVec10[1] * fTemp176 + -0.5);
			double fTemp179 = ((iTemp175) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec10[1]) : fTemp176 * (fTemp177 + -0.16666666666666666));
			double fTemp180 = 2.0 * fVec10[1];
			double fTemp181 = GrungrFaustDSP_faustpower2_f(fTemp171 - fVec10[1]);
			int iTemp182 = (fTemp171 <= 1.0) & (fTemp171 >= -1.0);
			double fTemp183 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp171);
			double fTemp184 = double((fTemp171 > 0.0) - (fTemp171 < 0.0));
			double fTemp185 = fTemp15 * (0.3 * fTemp8 + 0.06);
			double fTemp186 = 0.74 - fTemp82;
			double fTemp187 = fTemp153 / fTemp186;
			fVec11[0] = fTemp187;
			int iTemp188 = (fVec11[2] <= 1.0) & (fVec11[2] >= -1.0);
			double fTemp189 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec11[2]);
			double fTemp190 = double((fVec11[2] > 0.0) - (fVec11[2] < 0.0));
			int iTemp191 = (fVec11[1] <= 1.0) & (fVec11[1] >= -1.0);
			double fTemp192 = double((fVec11[1] > 0.0) - (fVec11[1] < 0.0));
			double fTemp193 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec11[1]);
			double fTemp194 = ((iTemp191) ? fTemp193 : fVec11[1] * fTemp192 + -0.5);
			double fTemp195 = ((iTemp191) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec11[1]) : fTemp192 * (fTemp193 + -0.16666666666666666));
			double fTemp196 = 2.0 * fVec11[1];
			double fTemp197 = GrungrFaustDSP_faustpower2_f(fTemp187 - fVec11[1]);
			int iTemp198 = (fTemp187 <= 1.0) & (fTemp187 >= -1.0);
			double fTemp199 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp187);
			double fTemp200 = double((fTemp187 > 0.0) - (fTemp187 < 0.0));
			double fTemp201 = 1.1 - 0.16 * fTemp17;
			double fTemp202 = fTemp153 / fTemp201;
			fVec12[0] = fTemp202;
			int iTemp203 = (fVec12[2] <= 1.0) & (fVec12[2] >= -1.0);
			double fTemp204 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec12[2]);
			double fTemp205 = double((fVec12[2] > 0.0) - (fVec12[2] < 0.0));
			int iTemp206 = (fVec12[1] <= 1.0) & (fVec12[1] >= -1.0);
			double fTemp207 = double((fVec12[1] > 0.0) - (fVec12[1] < 0.0));
			double fTemp208 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec12[1]);
			double fTemp209 = ((iTemp206) ? fTemp208 : fVec12[1] * fTemp207 + -0.5);
			double fTemp210 = ((iTemp206) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec12[1]) : fTemp207 * (fTemp208 + -0.16666666666666666));
			double fTemp211 = 2.0 * fVec12[1];
			double fTemp212 = GrungrFaustDSP_faustpower2_f(fTemp202 - fVec12[1]);
			int iTemp213 = (fTemp202 <= 1.0) & (fTemp202 >= -1.0);
			double fTemp214 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp202);
			double fTemp215 = double((fTemp202 > 0.0) - (fTemp202 < 0.0));
			double fTemp216 = 1.0 - fTemp185;
			fRec15[0] = fTemp216 * ((iTemp154) ? fTemp201 * (((fTemp212 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp202 + fTemp211))) : (fTemp153 * (((iTemp213) ? fTemp214 : fTemp153 * fTemp215 / fTemp201 + -0.5) - fTemp209) / fTemp201 + fTemp210 - ((iTemp213) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp202) : fTemp215 * (fTemp214 + -0.16666666666666666))) / fTemp212) + ((GrungrFaustDSP_faustpower2_f(fVec12[1] - fVec12[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp211 + fVec12[2]))) : (fTemp210 + fVec12[2] * (((iTemp203) ? fTemp204 : fVec12[2] * fTemp205 + -0.5) - fTemp209) - ((iTemp203) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec12[2]) : fTemp205 * (fTemp204 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec12[2] - fVec12[1]))) : fTemp186 * (((fTemp197 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp187 + fTemp196))) : (fTemp153 * (((iTemp198) ? fTemp199 : fTemp153 * fTemp200 / fTemp186 + -0.5) - fTemp194) / fTemp186 + fTemp195 - ((iTemp198) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp187) : fTemp200 * (fTemp199 + -0.16666666666666666))) / fTemp197) + ((GrungrFaustDSP_faustpower2_f(fVec11[1] - fVec11[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp196 + fVec11[2]))) : (fTemp195 + fVec11[2] * (((iTemp188) ? fTemp189 : fVec11[2] * fTemp190 + -0.5) - fTemp194) - ((iTemp188) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec11[2]) : fTemp190 * (fTemp189 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec11[2] - fVec11[1])))) + fTemp185 * ((iTemp154) ? fTemp170 * (((fTemp181 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp171 + fTemp180))) : (fTemp153 * (((iTemp182) ? fTemp183 : fTemp153 * fTemp184 / fTemp170 + -0.5) - fTemp178) / fTemp170 + fTemp179 - ((iTemp182) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp171) : fTemp184 * (fTemp183 + -0.16666666666666666))) / fTemp181) + ((GrungrFaustDSP_faustpower2_f(fVec10[1] - fVec10[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp180 + fVec10[2]))) : (fTemp179 + fVec10[2] * (((iTemp172) ? fTemp173 : fVec10[2] * fTemp174 + -0.5) - fTemp178) - ((iTemp172) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec10[2]) : fTemp174 * (fTemp173 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec10[2] - fVec10[1]))) : fTemp155 * (((fTemp166 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp156 + fTemp165))) : (fTemp153 * (((iTemp167) ? fTemp168 : fTemp153 * fTemp169 / fTemp155 + -0.5) - fTemp163) / fTemp155 + fTemp164 - ((iTemp167) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp156) : fTemp169 * (fTemp168 + -0.16666666666666666))) / fTemp166) + ((GrungrFaustDSP_faustpower2_f(fVec9[1] - fVec9[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp165 + fVec9[2]))) : (fTemp164 + fVec9[2] * (((iTemp157) ? fTemp158 : fVec9[2] * fTemp159 + -0.5) - fTemp163) - ((iTemp157) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec9[2]) : fTemp159 * (fTemp158 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec9[2] - fVec9[1])))) - (fRec15[2] * fTemp53 + 2.0 * fRec15[1] * fTemp52) / fTemp51;
			double fTemp217 = (fRec15[2] + fRec15[0] + 2.0 * fRec15[1]) / fTemp51;
			fVec13[0] = fTemp217;
			fRec14[0] = 0.995 * fRec14[1] + fTemp217 - fVec13[1];
			fRec13[0] = -(fConst47 * (fConst46 * fRec13[1] - fConst28 * (fRec14[0] - fRec14[1])));
			fRec12[0] = fRec13[0] - fConst31 * (fConst29 * fRec12[2] + fConst27 * fRec12[1]);
			fRec30[0] = fSlow4 + fConst2 * fRec30[1];
			double fTemp218 = std::pow(1e+01, 0.05 * (24.0 * std::pow(std::max<double>(fRec30[0], 2.220446049250313e-16), 0.82) + -9.0));
			fRec32[0] = -(fConst47 * (fConst46 * fRec32[1] - (fRec14[0] + fRec14[1])));
			fRec31[0] = fRec32[0] - fConst31 * (fConst29 * fRec31[2] + fConst27 * fRec31[1]);
			double fTemp219 = (fRec31[2] + fRec31[0] + 2.0 * fRec31[1]) * fTemp218 + fConst26 * (fRec12[2] + (fRec12[0] - 2.0 * fRec12[1]));
			fVec14[0] = fTemp219;
			fRec11[0] = -(fConst50 * (fConst49 * fRec11[1] - fConst48 * (fTemp219 - fVec14[1])));
			fRec10[0] = fRec11[0] - fConst24 * (fConst23 * fRec10[2] + fConst21 * fRec10[1]);
			fRec34[0] = -(fConst50 * (fConst49 * fRec34[1] - fConst31 * (fTemp219 + fVec14[1])));
			fRec33[0] = fRec34[0] - fConst24 * (fConst23 * fRec33[2] + fConst21 * fRec33[1]);
			fRec9[0] = fConst24 * (fRec33[2] + fRec33[0] + 2.0 * fRec33[1] + fConst20 * (fRec10[2] + (fRec10[0] - 2.0 * fRec10[1])) * fTemp48) - (fRec9[2] * fTemp47 + fTemp46) / fTemp43;
			double fTemp220 = fConst17 * (fConst17 + fTemp44) + 1.0;
			fRec8[0] = (fTemp46 + fRec9[0] * fTemp220 + fRec9[2] * fTemp45) / fTemp43 - (fRec8[2] * fTemp38 + fTemp37) / fTemp34;
			double fTemp221 = fConst13 * (fConst13 + fTemp35) + 1.0;
			fRec7[0] = (fTemp37 + fRec8[0] * fTemp221 + fRec8[2] * fTemp36) / fTemp34 - (fRec7[2] * fTemp29 + fTemp28) / fTemp25;
			double fTemp222 = fConst9 * (fConst9 + fTemp26) + 1.0;
			double fTemp223 = (fTemp28 + fRec7[0] * fTemp222 + fRec7[2] * fTemp27) / fTemp25;
			fVec15[0] = fTemp223;
			fRec6[0] = fConst52 * (fConst51 * (fTemp223 - fVec15[1]) - fConst6 * fRec6[1]);
			fRec5[0] = fRec6[0] - (fRec5[2] * fTemp20 + 2.0 * fRec5[1] * fTemp19) / fTemp14;
			fRec35[0] = fSlow5 + fConst2 * fRec35[1];
			double fTemp224 = std::pow(std::max<double>(fRec35[0], 2.220446049250313e-16), 1.65);
			double fTemp225 = fTemp224 * (fRec5[2] + fRec5[0] + 2.0 * fRec5[1]) * fTemp18 / fTemp14;
			double fTemp226 = fConst45 + fRec3[1];
			double fTemp227 = fRec3[1] - fConst45;
			fRec3[0] = ((fTemp226 < fTemp225) ? fTemp226 : ((fTemp227 > fTemp225) ? fTemp227 : fTemp225));
			fRec2[0] = fConst1 * std::fabs(fRec3[0]) + fConst2 * fRec2[1];
			double fTemp228 = fRec3[0] * (1.0 - fTemp6 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec1[0] * fRec2[0] * fTemp1 * fTemp7 / fTemp6)))) * fTemp4;
			fVec16[0] = fTemp228;
			double fTemp229 = fVec16[1] - fVec16[2];
			double fTemp230 = 0.8928571428571428 * fTemp229;
			double fTemp231 = 0.8928571428571428 * fVec16[2];
			double fTemp232 = GrungrFaustDSP_faustpower2_f(fTemp231) + 1.0;
			double fTemp233 = std::atan(fTemp231);
			double fTemp234 = 0.8928571428571428 * fVec16[1];
			double fTemp235 = GrungrFaustDSP_faustpower2_f(fTemp234) + 1.0;
			double fTemp236 = std::atan(fTemp234);
			double fTemp237 = fTemp236 * fTemp235;
			double fTemp238 = fVec16[1] * fTemp236;
			double fTemp239 = std::log(fTemp235);
			double fTemp240 = 1.7857142857142856 * fVec16[1];
			double fTemp241 = fTemp228 - fVec16[1];
			double fTemp242 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp241);
			double fTemp243 = 0.8928571428571428 * fTemp228;
			double fTemp244 = GrungrFaustDSP_faustpower2_f(fTemp243) + 1.0;
			double fTemp245 = std::atan(fTemp243);
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp90 : 0.7130141450516913 * (((fTemp242 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp243 + fTemp240)) : (0.8928571428571428 * fTemp228 * (0.8928571428571428 * (fTemp228 * fTemp245 - fTemp238) - 0.5 * (std::log(fTemp244) - fTemp239)) + 0.4464285714285714 * fTemp241 - 0.5 * (fTemp245 * fTemp244 - fTemp237)) / fTemp242) + ((GrungrFaustDSP_faustpower2_f(fTemp230) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp240 + fTemp231)) : (0.8928571428571428 * fVec16[2] * (0.5 * (fTemp239 - std::log(fTemp232)) - 0.8928571428571428 * (fTemp238 - fVec16[2] * fTemp233)) + 0.5 * (fTemp237 - fTemp233 * fTemp232) - 0.4464285714285714 * fTemp229) / GrungrFaustDSP_faustpower2_f(-fTemp230)))));
			double fTemp246 = fConst10 * fRec40[1];
			double fTemp247 = fConst14 * fRec41[1];
			double fTemp248 = fConst18 * fRec42[1];
			double fTemp249 = double(input1[i0]);
			double fTemp250 = ((iSlow0) ? 0.0 : fTemp249);
			fVec17[0] = fTemp250;
			fRec62[0] = fConst39 * (fConst38 * (fTemp250 - fVec17[1]) - fConst37 * fRec62[1]);
			fRec61[0] = -(fConst42 * (fConst41 * fRec61[1] - (fRec62[0] + fRec62[1])));
			double fTemp251 = (fRec61[0] + 0.01) * fTemp88;
			fVec18[0] = fTemp251;
			double fTemp252 = fVec18[1] - fVec18[2];
			double fTemp253 = 0.5555555555555556 * fTemp252;
			double fTemp254 = 0.5555555555555556 * fVec18[2];
			double fTemp255 = GrungrFaustDSP_faustpower2_f(fTemp254) + 1.0;
			double fTemp256 = std::atan(fTemp254);
			double fTemp257 = 0.5555555555555556 * fVec18[1];
			double fTemp258 = GrungrFaustDSP_faustpower2_f(fTemp257) + 1.0;
			double fTemp259 = std::atan(fTemp257);
			double fTemp260 = fTemp259 * fTemp258;
			double fTemp261 = fVec18[1] * fTemp259;
			double fTemp262 = std::log(fTemp258);
			double fTemp263 = 1.1111111111111112 * fVec18[1];
			double fTemp264 = fTemp251 - fVec18[1];
			double fTemp265 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp264);
			double fTemp266 = 0.5555555555555556 * fTemp251;
			double fTemp267 = GrungrFaustDSP_faustpower2_f(fTemp266) + 1.0;
			double fTemp268 = std::atan(fTemp266);
			double fTemp269 = fTemp89 - (((fTemp265 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp266 + fTemp263)) : (0.5555555555555556 * fTemp251 * (0.5555555555555556 * (fTemp251 * fTemp268 - fTemp261) - 0.5 * (std::log(fTemp267) - fTemp262)) + 0.2777777777777778 * fTemp264 - 0.5 * (fTemp268 * fTemp267 - fTemp260)) / fTemp265) + ((GrungrFaustDSP_faustpower2_f(fTemp253) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp263 + fTemp254)) : (0.5555555555555556 * fVec18[2] * (0.5 * (fTemp262 - std::log(fTemp255)) - 0.5555555555555556 * (fTemp261 - fVec18[2] * fTemp256)) + 0.5 * (fTemp260 - fTemp256 * fTemp255) - 0.2777777777777778 * fTemp252) / GrungrFaustDSP_faustpower2_f(-fTemp253)));
			fVec19[0] = fTemp269;
			fRec60[0] = -(fConst44 * (fConst34 * fRec60[1] + fConst43 * (fTemp269 - fVec19[1])));
			double fTemp270 = fRec60[0] * fTemp87;
			double fTemp271 = fConst45 + fRec59[1];
			double fTemp272 = fRec59[1] - fConst45;
			fRec59[0] = ((fTemp271 < fTemp270) ? fTemp271 : ((fTemp272 > fTemp270) ? fTemp272 : fTemp270));
			double fTemp273 = (fRec59[0] + 0.014) * fTemp85 * fTemp84;
			double fTemp274 = fTemp273 / fTemp83;
			fVec20[0] = fTemp274;
			double fTemp275 = GrungrFaustDSP_faustpower2_f(fVec20[2]);
			double fTemp276 = std::atan(fVec20[2]);
			double fTemp277 = GrungrFaustDSP_faustpower2_f(fVec20[1]);
			double fTemp278 = std::atan(fVec20[1]);
			double fTemp279 = fTemp278 - (fVec20[1] - fTemp278 * fTemp277);
			double fTemp280 = fVec20[1] * fTemp278;
			double fTemp281 = std::log(fTemp277 + 1.0);
			double fTemp282 = 2.0 * fVec20[1];
			double fTemp283 = GrungrFaustDSP_faustpower2_f(fTemp274 - fVec20[1]);
			double fTemp284 = GrungrFaustDSP_faustpower2_f(fTemp274);
			double fTemp285 = std::atan(fTemp274);
			double fTemp286 = fTemp83 * (fTemp86 - (((fTemp283 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp274 + fTemp282)) : (fTemp273 * (fTemp273 * fTemp285 / fTemp83 - (fTemp280 + 0.5 * (std::log(fTemp284 + 1.0) - fTemp281))) / fTemp83 - 0.5 * (fTemp285 - (fTemp274 - fTemp285 * fTemp284) - fTemp279)) / fTemp283) + ((GrungrFaustDSP_faustpower2_f(fVec20[1] - fVec20[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp282 + fVec20[2])) : (fVec20[2] * (fVec20[2] * fTemp276 + 0.5 * (fTemp281 - std::log(fTemp275 + 1.0)) - fTemp280) + 0.5 * (fTemp279 - (fTemp276 - (fVec20[2] - fTemp276 * fTemp275)))) / GrungrFaustDSP_faustpower2_f(fVec20[2] - fVec20[1]))));
			fVec21[0] = fTemp286;
			fRec58[0] = 0.995 * fRec58[1] - 0.6366197723675814 * (fTemp286 - fVec21[1]);
			fRec57[0] = fRec58[0] - (fTemp81 * fRec57[2] + 2.0 * fTemp80 * fRec57[1]) / fTemp79;
			double fTemp287 = (fRec57[2] + fRec57[0] + 2.0 * fRec57[1]) / fTemp79;
			fVec22[0] = fTemp287;
			fRec56[0] = -((fTemp129 * fRec56[1] - (fTemp287 - fVec22[1]) / fTemp74) / fTemp76);
			fRec55[0] = -((fTemp130 * fRec55[1] - (fRec56[0] + fRec56[1])) / fTemp73);
			double fTemp288 = fRec55[0] * fTemp71;
			double fTemp289 = fConst45 + fRec54[1];
			double fTemp290 = fRec54[1] - fConst45;
			fRec54[0] = ((fTemp289 < fTemp288) ? fTemp289 : ((fTemp290 > fTemp288) ? fTemp290 : fTemp288));
			double fTemp291 = fTemp68 * fTemp67 * (fRec54[0] + fTemp69);
			double fTemp292 = fTemp291 / fTemp66;
			fVec23[0] = fTemp292;
			double fTemp293 = GrungrFaustDSP_faustpower2_f(fVec23[2]);
			double fTemp294 = std::atan(fVec23[2]);
			double fTemp295 = GrungrFaustDSP_faustpower2_f(fVec23[1]);
			double fTemp296 = std::atan(fVec23[1]);
			double fTemp297 = fTemp296 - (fVec23[1] - fTemp296 * fTemp295);
			double fTemp298 = fVec23[1] * fTemp296;
			double fTemp299 = std::log(fTemp295 + 1.0);
			double fTemp300 = 2.0 * fVec23[1];
			double fTemp301 = GrungrFaustDSP_faustpower2_f(fTemp292 - fVec23[1]);
			double fTemp302 = GrungrFaustDSP_faustpower2_f(fTemp292);
			double fTemp303 = std::atan(fTemp292);
			double fTemp304 = fTemp66 * (fTemp70 - (((fTemp301 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp292 + fTemp300)) : (fTemp291 * (fTemp291 * fTemp303 / fTemp66 - (fTemp298 + 0.5 * (std::log(fTemp302 + 1.0) - fTemp299))) / fTemp66 - 0.5 * (fTemp303 - (fTemp292 - fTemp303 * fTemp302) - fTemp297)) / fTemp301) + ((GrungrFaustDSP_faustpower2_f(fVec23[1] - fVec23[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp300 + fVec23[2])) : (fVec23[2] * (fVec23[2] * fTemp294 + 0.5 * (fTemp299 - std::log(fTemp293 + 1.0)) - fTemp298) + 0.5 * (fTemp297 - (fTemp294 - (fVec23[2] - fTemp294 * fTemp293)))) / GrungrFaustDSP_faustpower2_f(fVec23[2] - fVec23[1]))));
			fVec24[0] = fTemp304;
			fRec53[0] = 0.995 * fRec53[1] - 0.6366197723675814 * (fTemp304 - fVec24[1]);
			fRec52[0] = fRec53[0] - (fTemp64 * fRec52[2] + 2.0 * fTemp63 * fRec52[1]) / fTemp62;
			double fTemp305 = (fRec52[2] + fRec52[0] + 2.0 * fRec52[1]) / fTemp62;
			fVec25[0] = fTemp305;
			fRec51[0] = -((fTemp149 * fRec51[1] - (fTemp305 - fVec25[1]) / fTemp56) / fTemp58);
			double fTemp306 = fRec51[0] * fTemp55;
			double fTemp307 = fConst45 + fRec50[1];
			double fTemp308 = fRec50[1] - fConst45;
			fRec50[0] = ((fTemp307 < fTemp306) ? fTemp307 : ((fTemp308 > fTemp306) ? fTemp308 : fTemp306));
			fRec49[0] = fConst1 * std::fabs(fRec50[0]) + fConst2 * fRec49[1];
			double fTemp309 = fRec50[0] * (1.0 - fTemp54 * std::min<double>(1.0, std::max<double>(0.0, fRec1[0] * fRec49[0] * fTemp1 * fTemp7 / fTemp54)));
			int iTemp310 = fTemp309 > 0.0;
			double fTemp311 = fTemp309 / fTemp155;
			fVec26[0] = fTemp311;
			int iTemp312 = (fVec26[2] <= 1.0) & (fVec26[2] >= -1.0);
			double fTemp313 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec26[2]);
			double fTemp314 = double((fVec26[2] > 0.0) - (fVec26[2] < 0.0));
			int iTemp315 = (fVec26[1] <= 1.0) & (fVec26[1] >= -1.0);
			double fTemp316 = double((fVec26[1] > 0.0) - (fVec26[1] < 0.0));
			double fTemp317 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec26[1]);
			double fTemp318 = ((iTemp315) ? fTemp317 : fVec26[1] * fTemp316 + -0.5);
			double fTemp319 = ((iTemp315) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec26[1]) : fTemp316 * (fTemp317 + -0.16666666666666666));
			double fTemp320 = 2.0 * fVec26[1];
			double fTemp321 = GrungrFaustDSP_faustpower2_f(fTemp311 - fVec26[1]);
			int iTemp322 = (fTemp311 <= 1.0) & (fTemp311 >= -1.0);
			double fTemp323 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp311);
			double fTemp324 = double((fTemp311 > 0.0) - (fTemp311 < 0.0));
			double fTemp325 = fTemp309 / fTemp170;
			fVec27[0] = fTemp325;
			int iTemp326 = (fVec27[2] <= 1.0) & (fVec27[2] >= -1.0);
			double fTemp327 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec27[2]);
			double fTemp328 = double((fVec27[2] > 0.0) - (fVec27[2] < 0.0));
			int iTemp329 = (fVec27[1] <= 1.0) & (fVec27[1] >= -1.0);
			double fTemp330 = double((fVec27[1] > 0.0) - (fVec27[1] < 0.0));
			double fTemp331 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec27[1]);
			double fTemp332 = ((iTemp329) ? fTemp331 : fVec27[1] * fTemp330 + -0.5);
			double fTemp333 = ((iTemp329) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec27[1]) : fTemp330 * (fTemp331 + -0.16666666666666666));
			double fTemp334 = 2.0 * fVec27[1];
			double fTemp335 = GrungrFaustDSP_faustpower2_f(fTemp325 - fVec27[1]);
			int iTemp336 = (fTemp325 <= 1.0) & (fTemp325 >= -1.0);
			double fTemp337 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp325);
			double fTemp338 = double((fTemp325 > 0.0) - (fTemp325 < 0.0));
			double fTemp339 = fTemp309 / fTemp186;
			fVec28[0] = fTemp339;
			int iTemp340 = (fVec28[2] <= 1.0) & (fVec28[2] >= -1.0);
			double fTemp341 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec28[2]);
			double fTemp342 = double((fVec28[2] > 0.0) - (fVec28[2] < 0.0));
			int iTemp343 = (fVec28[1] <= 1.0) & (fVec28[1] >= -1.0);
			double fTemp344 = double((fVec28[1] > 0.0) - (fVec28[1] < 0.0));
			double fTemp345 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec28[1]);
			double fTemp346 = ((iTemp343) ? fTemp345 : fVec28[1] * fTemp344 + -0.5);
			double fTemp347 = ((iTemp343) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec28[1]) : fTemp344 * (fTemp345 + -0.16666666666666666));
			double fTemp348 = 2.0 * fVec28[1];
			double fTemp349 = GrungrFaustDSP_faustpower2_f(fTemp339 - fVec28[1]);
			int iTemp350 = (fTemp339 <= 1.0) & (fTemp339 >= -1.0);
			double fTemp351 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp339);
			double fTemp352 = double((fTemp339 > 0.0) - (fTemp339 < 0.0));
			double fTemp353 = fTemp309 / fTemp201;
			fVec29[0] = fTemp353;
			int iTemp354 = (fVec29[2] <= 1.0) & (fVec29[2] >= -1.0);
			double fTemp355 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec29[2]);
			double fTemp356 = double((fVec29[2] > 0.0) - (fVec29[2] < 0.0));
			int iTemp357 = (fVec29[1] <= 1.0) & (fVec29[1] >= -1.0);
			double fTemp358 = double((fVec29[1] > 0.0) - (fVec29[1] < 0.0));
			double fTemp359 = 0.5 * GrungrFaustDSP_faustpower2_f(fVec29[1]);
			double fTemp360 = ((iTemp357) ? fTemp359 : fVec29[1] * fTemp358 + -0.5);
			double fTemp361 = ((iTemp357) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec29[1]) : fTemp358 * (fTemp359 + -0.16666666666666666));
			double fTemp362 = 2.0 * fVec29[1];
			double fTemp363 = GrungrFaustDSP_faustpower2_f(fTemp353 - fVec29[1]);
			int iTemp364 = (fTemp353 <= 1.0) & (fTemp353 >= -1.0);
			double fTemp365 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp353);
			double fTemp366 = double((fTemp353 > 0.0) - (fTemp353 < 0.0));
			fRec48[0] = fTemp216 * ((iTemp310) ? fTemp201 * (((fTemp363 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp353 + fTemp362))) : (fTemp309 * (((iTemp364) ? fTemp365 : fTemp309 * fTemp366 / fTemp201 + -0.5) - fTemp360) / fTemp201 + fTemp361 - ((iTemp364) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp353) : fTemp366 * (fTemp365 + -0.16666666666666666))) / fTemp363) + ((GrungrFaustDSP_faustpower2_f(fVec29[1] - fVec29[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp362 + fVec29[2]))) : (fTemp361 + fVec29[2] * (((iTemp354) ? fTemp355 : fVec29[2] * fTemp356 + -0.5) - fTemp360) - ((iTemp354) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec29[2]) : fTemp356 * (fTemp355 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec29[2] - fVec29[1]))) : fTemp186 * (((fTemp349 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp339 + fTemp348))) : (fTemp309 * (((iTemp350) ? fTemp351 : fTemp309 * fTemp352 / fTemp186 + -0.5) - fTemp346) / fTemp186 + fTemp347 - ((iTemp350) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp339) : fTemp352 * (fTemp351 + -0.16666666666666666))) / fTemp349) + ((GrungrFaustDSP_faustpower2_f(fVec28[1] - fVec28[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp348 + fVec28[2]))) : (fTemp347 + fVec28[2] * (((iTemp340) ? fTemp341 : fVec28[2] * fTemp342 + -0.5) - fTemp346) - ((iTemp340) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec28[2]) : fTemp342 * (fTemp341 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec28[2] - fVec28[1])))) + fTemp185 * ((iTemp310) ? fTemp170 * (((fTemp335 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp325 + fTemp334))) : (fTemp309 * (((iTemp336) ? fTemp337 : fTemp309 * fTemp338 / fTemp170 + -0.5) - fTemp332) / fTemp170 + fTemp333 - ((iTemp336) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp325) : fTemp338 * (fTemp337 + -0.16666666666666666))) / fTemp335) + ((GrungrFaustDSP_faustpower2_f(fVec27[1] - fVec27[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp334 + fVec27[2]))) : (fTemp333 + fVec27[2] * (((iTemp326) ? fTemp327 : fVec27[2] * fTemp328 + -0.5) - fTemp332) - ((iTemp326) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec27[2]) : fTemp328 * (fTemp327 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec27[2] - fVec27[1]))) : fTemp155 * (((fTemp321 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp311 + fTemp320))) : (fTemp309 * (((iTemp322) ? fTemp323 : fTemp309 * fTemp324 / fTemp155 + -0.5) - fTemp318) / fTemp155 + fTemp319 - ((iTemp322) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp311) : fTemp324 * (fTemp323 + -0.16666666666666666))) / fTemp321) + ((GrungrFaustDSP_faustpower2_f(fVec26[1] - fVec26[2]) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, 0.3333333333333333 * (fTemp320 + fVec26[2]))) : (fTemp319 + fVec26[2] * (((iTemp312) ? fTemp313 : fVec26[2] * fTemp314 + -0.5) - fTemp318) - ((iTemp312) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fVec26[2]) : fTemp314 * (fTemp313 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(fVec26[2] - fVec26[1])))) - (fTemp53 * fRec48[2] + 2.0 * fTemp52 * fRec48[1]) / fTemp51;
			double fTemp367 = (fRec48[2] + fRec48[0] + 2.0 * fRec48[1]) / fTemp51;
			fVec30[0] = fTemp367;
			fRec47[0] = 0.995 * fRec47[1] + fTemp367 - fVec30[1];
			fRec46[0] = -(fConst47 * (fConst46 * fRec46[1] - fConst28 * (fRec47[0] - fRec47[1])));
			fRec45[0] = fRec46[0] - fConst31 * (fConst29 * fRec45[2] + fConst27 * fRec45[1]);
			fRec64[0] = -(fConst47 * (fConst46 * fRec64[1] - (fRec47[0] + fRec47[1])));
			fRec63[0] = fRec64[0] - fConst31 * (fConst29 * fRec63[2] + fConst27 * fRec63[1]);
			double fTemp368 = fTemp218 * (fRec63[2] + fRec63[0] + 2.0 * fRec63[1]) + fConst26 * (fRec45[2] + (fRec45[0] - 2.0 * fRec45[1]));
			fVec31[0] = fTemp368;
			fRec44[0] = -(fConst50 * (fConst49 * fRec44[1] - fConst48 * (fTemp368 - fVec31[1])));
			fRec43[0] = fRec44[0] - fConst24 * (fConst23 * fRec43[2] + fConst21 * fRec43[1]);
			fRec66[0] = -(fConst50 * (fConst49 * fRec66[1] - fConst31 * (fTemp368 + fVec31[1])));
			fRec65[0] = fRec66[0] - fConst24 * (fConst23 * fRec65[2] + fConst21 * fRec65[1]);
			fRec42[0] = fConst24 * (fRec65[2] + fRec65[0] + 2.0 * fRec65[1] + fConst20 * fTemp48 * (fRec43[2] + (fRec43[0] - 2.0 * fRec43[1]))) - (fTemp47 * fRec42[2] + fTemp248) / fTemp43;
			fRec41[0] = (fTemp248 + fRec42[0] * fTemp220 + fTemp45 * fRec42[2]) / fTemp43 - (fTemp38 * fRec41[2] + fTemp247) / fTemp34;
			fRec40[0] = (fTemp247 + fRec41[0] * fTemp221 + fTemp36 * fRec41[2]) / fTemp34 - (fTemp29 * fRec40[2] + fTemp246) / fTemp25;
			double fTemp369 = (fTemp246 + fRec40[0] * fTemp222 + fTemp27 * fRec40[2]) / fTemp25;
			fVec32[0] = fTemp369;
			fRec39[0] = fConst52 * (fConst51 * (fTemp369 - fVec32[1]) - fConst6 * fRec39[1]);
			fRec38[0] = fRec39[0] - (fTemp20 * fRec38[2] + 2.0 * fTemp19 * fRec38[1]) / fTemp14;
			double fTemp370 = fTemp224 * fTemp18 * (fRec38[2] + fRec38[0] + 2.0 * fRec38[1]) / fTemp14;
			double fTemp371 = fConst45 + fRec37[1];
			double fTemp372 = fRec37[1] - fConst45;
			fRec37[0] = ((fTemp371 < fTemp370) ? fTemp371 : ((fTemp372 > fTemp370) ? fTemp372 : fTemp370));
			fRec36[0] = fConst1 * std::fabs(fRec37[0]) + fConst2 * fRec36[1];
			double fTemp373 = fRec37[0] * fTemp4 * (1.0 - fTemp6 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec1[0] * fRec36[0] * fTemp1 * fTemp7 / fTemp6))));
			fVec33[0] = fTemp373;
			double fTemp374 = fVec33[1] - fVec33[2];
			double fTemp375 = 0.8928571428571428 * fTemp374;
			double fTemp376 = 0.8928571428571428 * fVec33[2];
			double fTemp377 = GrungrFaustDSP_faustpower2_f(fTemp376) + 1.0;
			double fTemp378 = std::atan(fTemp376);
			double fTemp379 = 0.8928571428571428 * fVec33[1];
			double fTemp380 = GrungrFaustDSP_faustpower2_f(fTemp379) + 1.0;
			double fTemp381 = std::atan(fTemp379);
			double fTemp382 = fTemp381 * fTemp380;
			double fTemp383 = fVec33[1] * fTemp381;
			double fTemp384 = std::log(fTemp380);
			double fTemp385 = 1.7857142857142856 * fVec33[1];
			double fTemp386 = fTemp373 - fVec33[1];
			double fTemp387 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp386);
			double fTemp388 = 0.8928571428571428 * fTemp373;
			double fTemp389 = GrungrFaustDSP_faustpower2_f(fTemp388) + 1.0;
			double fTemp390 = std::atan(fTemp388);
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp249 : 0.7130141450516913 * (((fTemp387 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp388 + fTemp385)) : (0.8928571428571428 * fTemp373 * (0.8928571428571428 * (fTemp373 * fTemp390 - fTemp383) - 0.5 * (std::log(fTemp389) - fTemp384)) + 0.4464285714285714 * fTemp386 - 0.5 * (fTemp390 * fTemp389 - fTemp382)) / fTemp387) + ((GrungrFaustDSP_faustpower2_f(fTemp375) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp385 + fTemp376)) : (0.8928571428571428 * fVec33[2] * (0.5 * (fTemp384 - std::log(fTemp377)) - 0.8928571428571428 * (fTemp383 - fVec33[2] * fTemp378)) + 0.5 * (fTemp382 - fTemp378 * fTemp377) - 0.4464285714285714 * fTemp374) / GrungrFaustDSP_faustpower2_f(-fTemp375)))));
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec4[1] = fRec4[0];
			fVec0[1] = fVec0[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fVec1[2] = fVec1[1];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fVec3[2] = fVec3[1];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fVec5[1] = fVec5[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fVec6[2] = fVec6[1];
			fVec6[1] = fVec6[0];
			fVec7[1] = fVec7[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fVec8[1] = fVec8[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			fVec10[2] = fVec10[1];
			fVec10[1] = fVec10[0];
			fVec11[2] = fVec11[1];
			fVec11[1] = fVec11[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec13[1] = fVec13[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec30[1] = fRec30[0];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fVec14[1] = fVec14[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fVec15[1] = fVec15[0];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec35[1] = fRec35[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fVec16[2] = fVec16[1];
			fVec16[1] = fVec16[0];
			fVec17[1] = fVec17[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fVec18[2] = fVec18[1];
			fVec18[1] = fVec18[0];
			fVec19[1] = fVec19[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fVec20[2] = fVec20[1];
			fVec20[1] = fVec20[0];
			fVec21[1] = fVec21[0];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fVec22[1] = fVec22[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fVec23[2] = fVec23[1];
			fVec23[1] = fVec23[0];
			fVec24[1] = fVec24[0];
			fRec53[1] = fRec53[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fVec25[1] = fVec25[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec26[2] = fVec26[1];
			fVec26[1] = fVec26[0];
			fVec27[2] = fVec27[1];
			fVec27[1] = fVec27[0];
			fVec28[2] = fVec28[1];
			fVec28[1] = fVec28[0];
			fVec29[2] = fVec29[1];
			fVec29[1] = fVec29[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fVec30[1] = fVec30[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fVec31[1] = fVec31[0];
			fRec44[1] = fRec44[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fVec32[1] = fVec32[0];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fVec33[2] = fVec33[1];
			fVec33[1] = fVec33[0];
		}
	}

};

#endif
