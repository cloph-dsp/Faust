/* ------------------------------------------------------------
author: "cloph"
name: "Grungr"
version: "2.7"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
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
	FAUSTFLOAT fHslider1;
	double fRec1[2];
	double fRec2[2];
	FAUSTFLOAT fHslider2;
	double fRec5[2];
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
	double fConst38;
	double fConst39;
	double fConst40;
	double fConst41;
	double fConst42;
	double fConst43;
	double fConst44;
	double fConst45;
	double fVec0[2];
	double fRec38[2];
	double fRec37[3];
	double fVec1[2];
	double fConst46;
	double fConst47;
	double fConst48;
	double fRec36[2];
	double fVec2[3];
	double fVec3[2];
	double fConst49;
	double fConst50;
	double fRec35[2];
	double fConst51;
	double fRec34[2];
	double fVec4[3];
	double fVec5[2];
	double fRec33[2];
	double fRec32[3];
	double fVec6[2];
	double fRec31[2];
	double fRec30[2];
	double fRec29[2];
	double fVec7[3];
	double fVec8[2];
	double fRec28[2];
	double fRec27[3];
	double fVec9[2];
	double fRec26[2];
	double fRec25[2];
	double fRec24[2];
	double fVec10[2];
	double fConst52;
	double fConst53;
	double fRec23[2];
	double fRec22[3];
	double fRec40[2];
	double fRec39[3];
	double fVec11[3];
	double fConst54;
	double fVec12[3];
	double fVec13[3];
	double fVec14[3];
	double fVec15[2];
	double fRec21[2];
	double fRec20[3];
	double fRec42[2];
	double fRec41[3];
	double fRec19[3];
	double fVec16[2];
	double fRec18[2];
	double fConst55;
	double fConst56;
	double fRec17[2];
	double fRec16[3];
	FAUSTFLOAT fHslider3;
	double fRec43[2];
	double fRec45[2];
	double fRec44[3];
	double fVec17[2];
	double fConst57;
	double fConst58;
	double fConst59;
	double fRec15[2];
	double fRec14[3];
	double fRec47[2];
	double fRec46[3];
	double fRec13[3];
	double fRec12[3];
	double fRec11[3];
	double fVec18[2];
	double fConst60;
	double fConst61;
	double fRec10[2];
	double fRec9[2];
	double fRec8[3];
	double fRec49[2];
	double fRec48[3];
	double fRec7[3];
	double fRec6[3];
	FAUSTFLOAT fHslider4;
	double fRec50[2];
	double fRec4[2];
	double fRec3[2];
	double fVec19[3];
	double fVec20[2];
	double fRec85[2];
	double fRec84[3];
	double fVec21[2];
	double fRec83[2];
	double fVec22[3];
	double fVec23[2];
	double fRec82[2];
	double fRec81[2];
	double fVec24[3];
	double fVec25[2];
	double fRec80[2];
	double fRec79[3];
	double fVec26[2];
	double fRec78[2];
	double fRec77[2];
	double fRec76[2];
	double fVec27[3];
	double fVec28[2];
	double fRec75[2];
	double fRec74[3];
	double fVec29[2];
	double fRec73[2];
	double fRec72[2];
	double fRec71[2];
	double fVec30[2];
	double fRec70[2];
	double fRec69[3];
	double fRec87[2];
	double fRec86[3];
	double fVec31[3];
	double fVec32[3];
	double fVec33[3];
	double fVec34[3];
	double fVec35[2];
	double fRec68[2];
	double fRec67[3];
	double fRec89[2];
	double fRec88[3];
	double fRec66[3];
	double fVec36[2];
	double fRec65[2];
	double fRec64[2];
	double fRec63[3];
	double fRec91[2];
	double fRec90[3];
	double fVec37[2];
	double fRec62[2];
	double fRec61[3];
	double fRec93[2];
	double fRec92[3];
	double fRec60[3];
	double fRec59[3];
	double fRec58[3];
	double fVec38[2];
	double fRec57[2];
	double fRec56[2];
	double fRec55[3];
	double fRec95[2];
	double fRec94[3];
	double fRec54[3];
	double fRec53[3];
	double fRec52[2];
	double fRec51[2];
	double fVec39[3];
	
 public:
	GrungrFaustDSP() {
	}
	
	GrungrFaustDSP(const GrungrFaustDSP&) = default;
	
	virtual ~GrungrFaustDSP() = default;
	
	GrungrFaustDSP& operator=(const GrungrFaustDSP&) = default;
	
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
		m->declare("author", "cloph");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
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
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "2.7");
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
		fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, static_cast<double>(fSampleRate)));
		fConst1 = 44.1 / fConst0;
		fConst2 = 1.0 - fConst1;
		fConst3 = 3.141592653589793 / fConst0;
		fConst4 = 9424.77796076938 / (fConst0 * std::sin(28274.33388230814 / fConst0));
		fConst5 = std::tan(14137.16694115407 / fConst0);
		fConst6 = 1.0 / fConst5;
		fConst7 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst5));
		fConst8 = std::tan(94.24777960769379 / fConst0);
		fConst9 = 1.0 / fConst8;
		fConst10 = 1.0 - fConst9;
		fConst11 = 69.11503837897544 / (fConst0 * std::sin(345.57519189487726 / fConst0));
		fConst12 = std::tan(172.78759594743863 / fConst0);
		fConst13 = 1.0 / fConst12;
		fConst14 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst12));
		fConst15 = 6326.818538479444 / (fConst0 * std::sin(9110.6186954104 / fConst0));
		fConst16 = std::tan(4555.3093477052 / fConst0);
		fConst17 = 1.0 / fConst16;
		fConst18 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst16));
		fConst19 = 2050.3025739217596 / (fConst0 * std::sin(3895.5748904513434 / fConst0));
		fConst20 = std::tan(1947.7874452256717 / fConst0);
		fConst21 = 1.0 / fConst20;
		fConst22 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst20));
		fConst23 = std::tan(9424.77796076938 / fConst0);
		fConst24 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst23);
		fConst25 = 2.0 * (1.0 - fConst24);
		fConst26 = 1.0 / fConst23;
		fConst27 = (fConst26 + -1.0000000000000004) / fConst23 + 1.0;
		fConst28 = 1.0 / ((fConst26 + 1.0000000000000004) / fConst23 + 1.0);
		fConst29 = std::tan(376.99111843077515 / fConst0);
		fConst30 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst29);
		fConst31 = 2.0 * (1.0 - fConst30);
		fConst32 = 1.0 / fConst29;
		fConst33 = (fConst32 + -1.0000000000000004) / fConst29 + 1.0;
		fConst34 = (fConst32 + 1.0000000000000004) / fConst29 + 1.0;
		fConst35 = 1.0 / fConst34;
		fConst36 = std::tan(10995.574287564275 / fConst0);
		fConst37 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst36);
		fConst38 = 2.0 * (1.0 - fConst37);
		fConst39 = 1.0 / fConst36;
		fConst40 = (fConst39 + -1.0000000000000004) / fConst36 + 1.0;
		fConst41 = (fConst39 + 1.0000000000000004) / fConst36 + 1.0;
		fConst42 = 1.0 / fConst41;
		fConst43 = std::tan(109.95574287564276 / fConst0);
		fConst44 = 1.0 / fConst43;
		fConst45 = 1.0 - fConst44;
		fConst46 = 1.0 / std::tan(47123.8898038469 / fConst0);
		fConst47 = 1.0 - fConst46;
		fConst48 = 1.0 / (fConst46 + 1.0);
		fConst49 = 1.1459155902616465 / fConst43;
		fConst50 = 1.0 / (fConst44 + 1.0);
		fConst51 = 1e+06 / fConst0;
		fConst52 = 1.0 - fConst39;
		fConst53 = 1.0 / (fConst39 + 1.0);
		fConst54 = 0.3333333333333333 / fConst41;
		fConst55 = 1.0 - fConst32;
		fConst56 = 1.0 / (fConst32 + 1.0);
		fConst57 = 1.0 / (fConst23 * fConst34);
		fConst58 = 1.0 - fConst26;
		fConst59 = 1.0 / (fConst26 + 1.0);
		fConst60 = 0.86 / fConst8;
		fConst61 = 1.0 / (fConst9 + 1.0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0);
		fHslider0 = static_cast<FAUSTFLOAT>(0.7);
		fHslider1 = static_cast<FAUSTFLOAT>(0.0);
		fHslider2 = static_cast<FAUSTFLOAT>(0.56);
		fHslider3 = static_cast<FAUSTFLOAT>(0.62);
		fHslider4 = static_cast<FAUSTFLOAT>(0.72);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec5[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec0[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec38[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec37[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec36[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fVec2[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fVec3[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec35[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec34[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fVec4[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec5[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec33[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec32[l16] = 0.0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fVec6[l17] = 0.0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec31[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec30[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec29[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fVec7[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec8[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec28[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec27[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fVec9[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec26[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec25[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec24[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec10[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec23[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec22[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec40[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec39[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fVec11[l34] = 0.0;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fVec12[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fVec13[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fVec14[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fVec15[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec21[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec20[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec42[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec41[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec19[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fVec16[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec18[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec17[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec16[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec43[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec45[l49] = 0.0;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec44[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fVec17[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec15[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec14[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec47[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec46[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec13[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec12[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec11[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fVec18[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec10[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec9[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec8[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec49[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fRec48[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec7[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec6[l66] = 0.0;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec50[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec4[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec3[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fVec19[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fVec20[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec85[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec84[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fVec21[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec83[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fVec22[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fVec23[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec82[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec81[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fVec24[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fVec25[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec80[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec79[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fVec26[l84] = 0.0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec78[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec77[l86] = 0.0;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec76[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fVec27[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fVec28[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec75[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec74[l91] = 0.0;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fVec29[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec73[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec72[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec71[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fVec30[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec70[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fRec69[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec87[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 3; l100 = l100 + 1) {
			fRec86[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 3; l101 = l101 + 1) {
			fVec31[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fVec32[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fVec33[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 3; l104 = l104 + 1) {
			fVec34[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fVec35[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec68[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec67[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec89[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 3; l109 = l109 + 1) {
			fRec88[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 3; l110 = l110 + 1) {
			fRec66[l110] = 0.0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fVec36[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec65[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec64[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 3; l114 = l114 + 1) {
			fRec63[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec91[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 3; l116 = l116 + 1) {
			fRec90[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fVec37[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec62[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 3; l119 = l119 + 1) {
			fRec61[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec93[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 3; l121 = l121 + 1) {
			fRec92[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 3; l122 = l122 + 1) {
			fRec60[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 3; l123 = l123 + 1) {
			fRec59[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 3; l124 = l124 + 1) {
			fRec58[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fVec38[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec57[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec56[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 3; l128 = l128 + 1) {
			fRec55[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec95[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 3; l130 = l130 + 1) {
			fRec94[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 3; l131 = l131 + 1) {
			fRec54[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 3; l132 = l132 + 1) {
			fRec53[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec52[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec51[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 3; l135 = l135 + 1) {
			fVec39[l135] = 0.0;
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
		return new GrungrFaustDSP(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Grungr");
		ui_interface->declare(&fHslider0, "1", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Grunge", &fHslider0, FAUSTFLOAT(0.7), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider3, "2", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Butt", &fHslider3, FAUSTFLOAT(0.62), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider2, "3", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Face", &fHslider2, FAUSTFLOAT(0.56), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider4, "4", "");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Loud", &fHslider4, FAUSTFLOAT(0.72), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider1, "5", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("RAW Voicing", &fHslider1, FAUSTFLOAT(0.0), FAUSTFLOAT(0.0), FAUSTFLOAT(2.0), FAUSTFLOAT(1.0));
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
		int iSlow0 = static_cast<int>(static_cast<double>(fCheckbox0));
		double fSlow1 = fConst1 * static_cast<double>(fHslider0);
		int iSlow2 = std::max<int>(0, std::min<int>(2, static_cast<int>(static_cast<double>(fHslider1) + 0.5)));
		double fSlow3 = fConst1 * static_cast<double>(iSlow2 == 1);
		double fSlow4 = fConst1 * static_cast<double>(iSlow2 == 2);
		double fSlow5 = fConst1 * static_cast<double>(fHslider2);
		double fSlow6 = fConst1 * static_cast<double>(fHslider3);
		double fSlow7 = fConst1 * static_cast<double>(fHslider4);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			double fTemp0 = std::min<double>(1.0, std::max<double>(0.0, 3.3333333333333335 * (fRec0[0] + -0.7)));
			double fTemp1 = std::min<double>(1.0, std::max<double>(0.0, 2.2222222222222223 * (fRec0[0] + -0.55)));
			fRec1[0] = fSlow3 + fConst2 * fRec1[1];
			double fTemp2 = fRec1[0] * fTemp1;
			double fTemp3 = std::min<double>(1.0, std::max<double>(0.0, fRec0[0] * (1.32 - 0.32 * fRec0[0])));
			double fTemp4 = 0.09 * fTemp3 + fTemp2 * (0.04 * fTemp0 + 0.12) + 0.64;
			fRec2[0] = fSlow4 + fConst2 * fRec2[1];
			double fTemp5 = fRec2[0] * std::min<double>(1.0, std::max<double>(0.0, 1.6666666666666667 * (fRec0[0] + -0.4)));
			double fTemp6 = 1.0 - (fTemp2 + fTemp5);
			double fTemp7 = std::max<double>(0.06 * fTemp5 + (0.16000000000000003 - 0.03 * fTemp6), 2.220446049250313e-16);
			double fTemp8 = GrungrFaustDSP_faustpower2_f(fTemp3);
			double fTemp9 = fTemp2 * (0.04 * fTemp8 + 0.016) + fTemp5 * (0.05 * fTemp8 + 0.024);
			double fTemp10 = std::min<double>(1.0, std::max<double>(0.0, 5.555555555555555 * (fRec0[0] + -0.82)));
			fRec5[0] = fSlow5 + fConst2 * fRec5[1];
			double fTemp11 = std::pow(std::max<double>(fRec5[0], 2.220446049250313e-16), 0.88);
			double fTemp12 = fTemp11 + 1.0;
			double fTemp13 = std::min<double>(1.0, std::max<double>(0.0, 1.5384615384615383 * (fRec0[0] + -0.35)));
			double fTemp14 = fTemp13 * fTemp12;
			double fTemp15 = std::tan(fConst3 * (1.46e+04 - 0.5 * fTemp14 * (9e+02 * fTemp3 + 6.5e+02 * fTemp10 + fRec1[0] * (8.5e+02 * fTemp10 + 3.5e+02))));
			double fTemp16 = 1.0 / fTemp15;
			double fTemp17 = (fTemp16 + 1.414213562373095) / fTemp15 + 1.0;
			double fTemp18 = std::min<double>(1.0, std::max<double>(0.0, 2.857142857142857 * (fRec0[0] + -0.65)));
			double fTemp19 = 0.55 * fTemp3;
			double fTemp20 = 1.0 - fTemp3;
			double fTemp21 = std::min<double>(1.0, std::max<double>(0.0, fTemp3 + fTemp20 * (0.22 * fTemp6 + 0.07) + fTemp2 * (0.1 * fTemp20 + 0.05 * fTemp10) + fTemp5 * (0.08 * fTemp20 + 0.03 * fTemp18)));
			double fTemp22 = std::pow(1e+01, 0.05 * (2.1 * fTemp6 * (fTemp21 - fTemp3) + fRec1[0] * (fTemp13 * (0.45 * fTemp3 + (0.35 - 0.25 * fTemp10)) - 0.55 * fTemp1 * fTemp0) + fTemp5 * (fTemp19 + (0.5 - 0.2 * fTemp18)) + (0.95 - 1.1 * GrungrFaustDSP_faustpower2_f(fTemp10))));
			double fTemp23 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp15);
			double fTemp24 = (fTemp16 + -1.414213562373095) / fTemp15 + 1.0;
			double fTemp25 = fTemp2 * (0.6 * fTemp10 + 1.8);
			int iTemp26 = -fTemp25 > 0.0;
			double fTemp27 = fConst4 * std::pow(1e+01, 0.05 * std::fabs(-fTemp25));
			double fTemp28 = ((iTemp26) ? fConst4 : fTemp27);
			double fTemp29 = fConst6 * (fConst6 + fTemp28) + 1.0;
			double fTemp30 = ((iTemp26) ? fTemp27 : fConst4);
			double fTemp31 = fConst6 * (fConst6 - fTemp30) + 1.0;
			double fTemp32 = fConst7 * fRec7[1];
			double fTemp33 = fConst6 * (fConst6 - fTemp28) + 1.0;
			double fTemp34 = std::tan(fConst3 * (4.2e+03 - 1.7e+03 * fRec2[0]));
			double fTemp35 = 1.0 / fTemp34;
			double fTemp36 = (fTemp35 + 1.0000000000000004) / fTemp34 + 1.0;
			double fTemp37 = GrungrFaustDSP_faustpower2_f(fTemp34);
			double fTemp38 = 1.8 * fTemp10;
			double fTemp39 = fRec1[0] * fTemp13;
			double fTemp40 = std::pow(1e+01, -(0.025 * fTemp12 * (fTemp39 * (fTemp38 + 0.9) + fTemp5 * (1.4 * fTemp18 + 0.7))));
			double fTemp41 = 1.0 - 1.0 / fTemp37;
			double fTemp42 = (fTemp35 + -1.0000000000000004) / fTemp34 + 1.0;
			double fTemp43 = fTemp35 + 1.0;
			double fTemp44 = fTemp2 * fTemp0;
			double fTemp45 = 3.0 * fTemp44;
			int iTemp46 = -fTemp45 > 0.0;
			double fTemp47 = fConst11 * std::pow(1e+01, 0.05 * std::fabs(-fTemp45));
			double fTemp48 = ((iTemp46) ? fConst11 : fTemp47);
			double fTemp49 = fConst13 * (fConst13 + fTemp48) + 1.0;
			double fTemp50 = ((iTemp46) ? fTemp47 : fConst11);
			double fTemp51 = fConst13 * (fConst13 - fTemp50) + 1.0;
			double fTemp52 = fConst14 * fRec11[1];
			double fTemp53 = fConst13 * (fConst13 - fTemp48) + 1.0;
			double fTemp54 = 0.5 * fTemp14 * (0.7 - 0.3 * fTemp3 + fRec1[0] * (0.4 * fTemp20 + 0.2));
			int iTemp55 = fTemp54 > 0.0;
			double fTemp56 = fConst15 * std::pow(1e+01, 0.05 * std::fabs(fTemp54));
			double fTemp57 = ((iTemp55) ? fConst15 : fTemp56);
			double fTemp58 = fConst17 * (fConst17 + fTemp57) + 1.0;
			double fTemp59 = ((iTemp55) ? fTemp56 : fConst15);
			double fTemp60 = fConst17 * (fConst17 - fTemp59) + 1.0;
			double fTemp61 = fConst18 * fRec12[1];
			double fTemp62 = fConst17 * (fConst17 - fTemp57) + 1.0;
			double fTemp63 = fTemp13 * (0.95 - fTemp19) + 3.0 * fTemp5 + -5.6;
			int iTemp64 = fTemp63 > 0.0;
			double fTemp65 = fConst19 * std::pow(1e+01, 0.05 * std::fabs(fTemp63));
			double fTemp66 = ((iTemp64) ? fConst19 : fTemp65);
			double fTemp67 = fConst21 * (fConst21 + fTemp66) + 1.0;
			double fTemp68 = ((iTemp64) ? fTemp65 : fConst19);
			double fTemp69 = fConst21 * (fConst21 - fTemp68) + 1.0;
			double fTemp70 = fConst22 * fRec13[1];
			double fTemp71 = fConst21 * (fConst21 - fTemp66) + 1.0;
			double fTemp72 = std::pow(1e+01, 0.05 * (19.0 * fTemp11 + -8.0));
			double fTemp73 = std::tan(fConst3 * (1.12e+04 - (1.8e+03 * fTemp21 + 9e+02 * fTemp10 + 0.5 * fTemp12 * (fTemp39 * (9.6e+02 * fTemp10 + 4.2e+02) + fTemp5 * (7.2e+02 * fTemp18 + 3.2e+02)))));
			double fTemp74 = 1.0 / fTemp73;
			double fTemp75 = (fTemp74 + 1.414213562373095) / fTemp73 + 1.0;
			double fTemp76 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp73);
			double fTemp77 = (fTemp74 + -1.414213562373095) / fTemp73 + 1.0;
			double fTemp78 = 0.05 * fTemp2 * (0.5 * fTemp20 + 1.3);
			double fTemp79 = std::pow(1e+01, -fTemp78);
			double fTemp80 = std::pow(1e+01, fTemp78);
			double fTemp81 = std::max<double>(0.08 * fTemp5 + (0.24 - 0.04 * fTemp6), 2.220446049250313e-16);
			double fTemp82 = std::pow(1e+01, 0.05 * (6.0 * (fTemp21 + 1.0) + fTemp2 * (fTemp38 + 0.7) + fTemp5 * (1.5 * fTemp18 + 0.5)));
			double fTemp83 = std::tan(fConst3 * (2e+01 * fTemp21 + fTemp2 * (18.0 * fTemp3 + 1e+01) + fTemp5 * (1e+01 * fTemp3 + 6.0) + 1.1e+02));
			double fTemp84 = 1.0 / fTemp83;
			double fTemp85 = fTemp84 + 1.0;
			double fTemp86 = 1.6e+03 * fTemp21 + fTemp2 * (6.2e+02 * fTemp10 + 2.2e+02) + fTemp5 * (4.6e+02 * fTemp18 + 1.6e+02);
			double fTemp87 = std::tan(fConst3 * (8.1e+03 - fTemp86));
			double fTemp88 = 1.0 / fTemp87;
			double fTemp89 = (fTemp88 + 1.414213562373095) / fTemp87 + 1.0;
			double fTemp90 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp87);
			double fTemp91 = (fTemp88 + -1.414213562373095) / fTemp87 + 1.0;
			double fTemp92 = 0.12 * fTemp21;
			double fTemp93 = fTemp2 * (0.015 * fTemp20 + 0.02) + fTemp5 * (0.02 * fTemp20 + 0.025);
			double fTemp94 = 0.85 * fTemp93 + (0.74 - fTemp92);
			double fTemp95 = 1.0 - 0.16 * fTemp2;
			double fTemp96 = 0.1 * fTemp3 + 1.1;
			double fTemp97 = -0.01 - (0.018 * fTemp21 + fTemp2 * (0.006 * fTemp0 + 0.004) + fTemp5 * (0.005 * fTemp18 + 0.003));
			double fTemp98 = std::atan(fTemp97 * fTemp96 * fTemp95 / fTemp94);
			double fTemp99 = fTemp2 * (1.7 * fTemp3 + 0.9) + fTemp5 * (2.0 * fTemp3 + 1.2);
			double fTemp100 = std::pow(1e+01, 0.05 * (14.0 * fTemp21 + (12.0 - 0.65 * fTemp99)));
			double fTemp101 = 1.0 / std::tan(fConst3 * (8.4e+03 - fTemp86));
			double fTemp102 = fTemp101 + 1.0;
			double fTemp103 = 12.0 * fTemp3;
			double fTemp104 = std::tan(fConst3 * (25.0 * fTemp21 + fRec1[0] * (fTemp13 * (24.0 * fTemp3 + 16.0) + 12.0 * fTemp1) + fTemp5 * (fTemp103 + 8.0) + 8e+01));
			double fTemp105 = 1.0 / fTemp104;
			double fTemp106 = fTemp105 + 1.0;
			double fTemp107 = std::tan(fConst3 * (1.08e+04 - (2e+03 * fTemp21 + fTemp2 * (8.2e+02 * fTemp10 + 2.8e+02) + fTemp5 * (6e+02 * fTemp18 + 2e+02))));
			double fTemp108 = 1.0 / fTemp107;
			double fTemp109 = (fTemp108 + 1.414213562373095) / fTemp107 + 1.0;
			double fTemp110 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp107);
			double fTemp111 = (fTemp108 + -1.414213562373095) / fTemp107 + 1.0;
			double fTemp112 = 0.1 * fTemp21;
			double fTemp113 = fTemp93 + (0.76 - fTemp112);
			double fTemp114 = 1.0 - 0.18 * fTemp2;
			double fTemp115 = 0.05 * fTemp3 + 1.0;
			double fTemp116 = std::atan(0.014 * (fTemp115 * fTemp114 / fTemp113));
			double fTemp117 = std::pow(1e+01, 0.05 * (19.0 * fTemp21 + (18.0 - fTemp99)));
			double fTemp118 = 0.07 * fTemp3 + (0.4 - 0.03 * fTemp39);
			double fTemp119 = std::atan(0.005555555555555556 * fTemp118);
			double fTemp120 = std::tan(fConst3 * std::max<double>(2e+01, fRec1[0] * (fTemp103 + 4e+01)));
			double fTemp121 = 1.0 / fTemp120;
			double fTemp122 = (fTemp121 + 1.414213562373095) / fTemp120 + 1.0;
			double fTemp123 = GrungrFaustDSP_faustpower2_f(fTemp120);
			double fTemp124 = fTemp123 * fTemp122;
			double fTemp125 = 1.0 - 1.0 / fTemp123;
			double fTemp126 = (fTemp121 + -1.414213562373095) / fTemp120 + 1.0;
			double fTemp127 = std::tan(fConst3 * (fTemp39 * (3e+01 * fTemp3 + 12.0) + 1.6));
			double fTemp128 = 1.0 / fTemp127;
			double fTemp129 = fTemp128 + 1.0;
			double fTemp130 = 1.0 - fTemp128;
			double fTemp131 = static_cast<double>(input0[i0]);
			double fTemp132 = ((iSlow0) ? 0.0 : fTemp131);
			fVec0[0] = fTemp132;
			fRec38[0] = (0.2 * ((fTemp132 - fVec0[1]) / fTemp127) - fRec38[1] * fTemp130) / fTemp129;
			fRec37[0] = fRec38[0] - (fRec37[2] * fTemp126 + 2.0 * fRec37[1] * fTemp125) / fTemp122;
			double fTemp133 = (fRec37[2] + (fRec37[0] - 2.0 * fRec37[1])) / fTemp124;
			fVec1[0] = fTemp133;
			fRec36[0] = -(fConst48 * (fConst47 * fRec36[1] - (fTemp133 + fVec1[1])));
			double fTemp134 = (fRec36[0] + 0.01) * fTemp118;
			fVec2[0] = fTemp134;
			double fTemp135 = fVec2[1] - fVec2[2];
			double fTemp136 = 0.5555555555555556 * fTemp135;
			double fTemp137 = 0.5555555555555556 * fVec2[2];
			double fTemp138 = GrungrFaustDSP_faustpower2_f(fTemp137) + 1.0;
			double fTemp139 = std::atan(fTemp137);
			double fTemp140 = 0.5555555555555556 * fVec2[1];
			double fTemp141 = GrungrFaustDSP_faustpower2_f(fTemp140) + 1.0;
			double fTemp142 = std::atan(fTemp140);
			double fTemp143 = fTemp142 * fTemp141;
			double fTemp144 = fVec2[1] * fTemp142;
			double fTemp145 = std::log(fTemp141);
			double fTemp146 = 1.1111111111111112 * fVec2[1];
			double fTemp147 = fTemp134 - fVec2[1];
			double fTemp148 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp147);
			double fTemp149 = 0.5555555555555556 * fTemp134;
			double fTemp150 = GrungrFaustDSP_faustpower2_f(fTemp149) + 1.0;
			double fTemp151 = std::atan(fTemp149);
			double fTemp152 = ((fTemp148 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp149 + fTemp146)) : (0.5555555555555556 * fTemp134 * (0.5555555555555556 * (fTemp134 * fTemp151 - fTemp144) - 0.5 * (std::log(fTemp150) - fTemp145)) + 0.2777777777777778 * fTemp147 - 0.5 * (fTemp151 * fTemp150 - fTemp143)) / fTemp148) + ((GrungrFaustDSP_faustpower2_f(fTemp136) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp146 + fTemp137)) : (0.5555555555555556 * fVec2[2] * (0.5 * (fTemp145 - std::log(fTemp138)) - 0.5555555555555556 * (fTemp144 - fVec2[2] * fTemp139)) + 0.5 * (fTemp143 - fTemp139 * fTemp138) - 0.2777777777777778 * fTemp135) / GrungrFaustDSP_faustpower2_f(-fTemp136)) - fTemp119;
			fVec3[0] = fTemp152;
			fRec35[0] = fConst50 * (fConst49 * (fTemp152 - fVec3[1]) - fConst45 * fRec35[1]);
			double fTemp153 = fRec35[0] * fTemp117;
			double fTemp154 = fConst51 + fRec34[1];
			double fTemp155 = fRec34[1] - fConst51;
			fRec34[0] = ((fTemp154 < fTemp153) ? fTemp154 : ((fTemp155 > fTemp153) ? fTemp155 : fTemp153));
			double fTemp156 = (fRec34[0] + 0.014) * fTemp115 * fTemp114;
			double fTemp157 = fTemp156 / fTemp113;
			fVec4[0] = fTemp157;
			double fTemp158 = GrungrFaustDSP_faustpower2_f(fVec4[2]);
			double fTemp159 = std::atan(fVec4[2]);
			double fTemp160 = GrungrFaustDSP_faustpower2_f(fVec4[1]);
			double fTemp161 = std::atan(fVec4[1]);
			double fTemp162 = fTemp161 - (fVec4[1] - fTemp161 * fTemp160);
			double fTemp163 = fVec4[1] * fTemp161;
			double fTemp164 = std::log(fTemp160 + 1.0);
			double fTemp165 = 2.0 * fVec4[1];
			double fTemp166 = GrungrFaustDSP_faustpower2_f(fTemp157 - fVec4[1]);
			double fTemp167 = GrungrFaustDSP_faustpower2_f(fTemp157);
			double fTemp168 = std::atan(fTemp157);
			double fTemp169 = fTemp113 * (((fTemp166 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp157 + fTemp165)) : (fTemp156 * (fTemp156 * fTemp168 / fTemp113 - (fTemp163 + 0.5 * (std::log(fTemp167 + 1.0) - fTemp164))) / fTemp113 - 0.5 * (fTemp168 - (fTemp157 - fTemp168 * fTemp167) - fTemp162)) / fTemp166) + ((GrungrFaustDSP_faustpower2_f(fVec4[1] - fVec4[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp165 + fVec4[2])) : (fVec4[2] * (fVec4[2] * fTemp159 + 0.5 * (fTemp164 - std::log(fTemp158 + 1.0)) - fTemp163) + 0.5 * (fTemp162 - (fTemp159 - (fVec4[2] - fTemp159 * fTemp158)))) / GrungrFaustDSP_faustpower2_f(fVec4[2] - fVec4[1])) - fTemp116);
			fVec5[0] = fTemp169;
			fRec33[0] = 0.995 * fRec33[1] + 0.6366197723675814 * (fTemp169 - fVec5[1]);
			fRec32[0] = fRec33[0] - (fRec32[2] * fTemp111 + 2.0 * fRec32[1] * fTemp110) / fTemp109;
			double fTemp170 = (fRec32[2] + fRec32[0] + 2.0 * fRec32[1]) / fTemp109;
			fVec6[0] = fTemp170;
			double fTemp171 = 1.0 - fTemp105;
			fRec31[0] = -((fRec31[1] * fTemp171 - (fTemp170 - fVec6[1]) / fTemp104) / fTemp106);
			double fTemp172 = 1.0 - fTemp101;
			fRec30[0] = -((fRec30[1] * fTemp172 - (fRec31[0] + fRec31[1])) / fTemp102);
			double fTemp173 = fRec30[0] * fTemp100;
			double fTemp174 = fConst51 + fRec29[1];
			double fTemp175 = fRec29[1] - fConst51;
			fRec29[0] = ((fTemp174 < fTemp173) ? fTemp174 : ((fTemp175 > fTemp173) ? fTemp175 : fTemp173));
			double fTemp176 = (fRec29[0] + fTemp97) * fTemp96 * fTemp95;
			double fTemp177 = fTemp176 / fTemp94;
			fVec7[0] = fTemp177;
			double fTemp178 = GrungrFaustDSP_faustpower2_f(fVec7[2]);
			double fTemp179 = std::atan(fVec7[2]);
			double fTemp180 = GrungrFaustDSP_faustpower2_f(fVec7[1]);
			double fTemp181 = std::atan(fVec7[1]);
			double fTemp182 = fTemp181 - (fVec7[1] - fTemp181 * fTemp180);
			double fTemp183 = fVec7[1] * fTemp181;
			double fTemp184 = std::log(fTemp180 + 1.0);
			double fTemp185 = 2.0 * fVec7[1];
			double fTemp186 = GrungrFaustDSP_faustpower2_f(fTemp177 - fVec7[1]);
			double fTemp187 = GrungrFaustDSP_faustpower2_f(fTemp177);
			double fTemp188 = std::atan(fTemp177);
			double fTemp189 = fTemp94 * (((fTemp186 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp177 + fTemp185)) : (fTemp176 * (fTemp176 * fTemp188 / fTemp94 - (fTemp183 + 0.5 * (std::log(fTemp187 + 1.0) - fTemp184))) / fTemp94 - 0.5 * (fTemp188 - (fTemp177 - fTemp188 * fTemp187) - fTemp182)) / fTemp186) + ((GrungrFaustDSP_faustpower2_f(fVec7[1] - fVec7[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp185 + fVec7[2])) : (fVec7[2] * (fVec7[2] * fTemp179 + 0.5 * (fTemp184 - std::log(fTemp178 + 1.0)) - fTemp183) + 0.5 * (fTemp182 - (fTemp179 - (fVec7[2] - fTemp179 * fTemp178)))) / GrungrFaustDSP_faustpower2_f(fVec7[2] - fVec7[1])) - fTemp98);
			fVec8[0] = fTemp189;
			fRec28[0] = 0.995 * fRec28[1] + 0.6366197723675814 * (fTemp189 - fVec8[1]);
			fRec27[0] = fRec28[0] - (fRec27[2] * fTemp91 + 2.0 * fRec27[1] * fTemp90) / fTemp89;
			double fTemp190 = (fRec27[2] + fRec27[0] + 2.0 * fRec27[1]) / fTemp89;
			fVec9[0] = fTemp190;
			double fTemp191 = 1.0 - fTemp84;
			fRec26[0] = -((fRec26[1] * fTemp191 - (fTemp190 - fVec9[1]) / fTemp83) / fTemp85);
			double fTemp192 = fRec26[0] * fTemp82;
			double fTemp193 = fConst51 + fRec25[1];
			double fTemp194 = fRec25[1] - fConst51;
			fRec25[0] = ((fTemp193 < fTemp192) ? fTemp193 : ((fTemp194 > fTemp192) ? fTemp194 : fTemp192));
			fRec24[0] = fConst1 * std::fabs(fRec25[0]) + fConst2 * fRec24[1];
			double fTemp195 = fRec25[0] * (1.0 - fTemp81 * std::min<double>(1.0, std::max<double>(0.0, fRec24[0] * fTemp9 / fTemp81)));
			fVec10[0] = fTemp195;
			fRec23[0] = -(fConst53 * (fConst52 * fRec23[1] - fConst39 * (fTemp195 - fVec10[1])));
			fRec22[0] = fRec23[0] - fConst42 * (fConst40 * fRec22[2] + fConst38 * fRec22[1]);
			fRec40[0] = -(fConst53 * (fConst52 * fRec40[1] - (fTemp195 + fVec10[1])));
			fRec39[0] = fRec40[0] - fConst42 * (fConst40 * fRec39[2] + fConst38 * fRec39[1]);
			double fTemp196 = fRec39[2] + fRec39[0] + 2.0 * fRec39[1] + fConst37 * (fRec22[2] + (fRec22[0] - 2.0 * fRec22[1])) * fTemp80;
			int iTemp197 = (fConst42 * fTemp196) > 0.0;
			double fTemp198 = 1.08 - 0.08 * fTemp21;
			double fTemp199 = fTemp196 / fTemp198;
			fVec11[0] = fTemp199;
			double fTemp200 = fConst42 * (fVec11[1] - fVec11[2]);
			double fTemp201 = fConst42 * fVec11[2];
			int iTemp202 = (fTemp201 <= 1.0) & (fTemp201 >= -1.0);
			double fTemp203 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp201);
			double fTemp204 = static_cast<double>((fTemp201 > 0.0) - (fTemp201 < 0.0));
			double fTemp205 = fConst42 * fVec11[1];
			int iTemp206 = (fTemp205 <= 1.0) & (fTemp205 >= -1.0);
			double fTemp207 = static_cast<double>((fTemp205 > 0.0) - (fTemp205 < 0.0));
			double fTemp208 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp205);
			double fTemp209 = ((iTemp206) ? fTemp208 : fConst42 * fVec11[1] * fTemp207 + -0.5);
			double fTemp210 = ((iTemp206) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp205) : fTemp207 * (fTemp208 + -0.16666666666666666));
			double fTemp211 = 2.0 * fVec11[1];
			double fTemp212 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp199 - fVec11[1]));
			double fTemp213 = fConst42 * fTemp199;
			int iTemp214 = (fTemp213 <= 1.0) & (fTemp213 >= -1.0);
			double fTemp215 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp213);
			double fTemp216 = static_cast<double>((fTemp213 > 0.0) - (fTemp213 < 0.0));
			double fTemp217 = 1.62 - fTemp92;
			double fTemp218 = fTemp196 / fTemp217;
			fVec12[0] = fTemp218;
			double fTemp219 = fConst42 * (fVec12[1] - fVec12[2]);
			double fTemp220 = fConst42 * fVec12[2];
			int iTemp221 = (fTemp220 <= 1.0) & (fTemp220 >= -1.0);
			double fTemp222 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp220);
			double fTemp223 = static_cast<double>((fTemp220 > 0.0) - (fTemp220 < 0.0));
			double fTemp224 = fConst42 * fVec12[1];
			int iTemp225 = (fTemp224 <= 1.0) & (fTemp224 >= -1.0);
			double fTemp226 = static_cast<double>((fTemp224 > 0.0) - (fTemp224 < 0.0));
			double fTemp227 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp224);
			double fTemp228 = ((iTemp225) ? fTemp227 : fConst42 * fVec12[1] * fTemp226 + -0.5);
			double fTemp229 = ((iTemp225) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp224) : fTemp226 * (fTemp227 + -0.16666666666666666));
			double fTemp230 = 2.0 * fVec12[1];
			double fTemp231 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp218 - fVec12[1]));
			double fTemp232 = fConst42 * fTemp218;
			int iTemp233 = (fTemp232 <= 1.0) & (fTemp232 >= -1.0);
			double fTemp234 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp232);
			double fTemp235 = static_cast<double>((fTemp232 > 0.0) - (fTemp232 < 0.0));
			double fTemp236 = fTemp44 * (0.34 * fTemp10 + 0.08);
			double fTemp237 = 0.74 - fTemp112;
			double fTemp238 = fTemp196 / fTemp237;
			fVec13[0] = fTemp238;
			double fTemp239 = fConst42 * (fVec13[1] - fVec13[2]);
			double fTemp240 = fConst42 * fVec13[2];
			int iTemp241 = (fTemp240 <= 1.0) & (fTemp240 >= -1.0);
			double fTemp242 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp240);
			double fTemp243 = static_cast<double>((fTemp240 > 0.0) - (fTemp240 < 0.0));
			double fTemp244 = fConst42 * fVec13[1];
			int iTemp245 = (fTemp244 <= 1.0) & (fTemp244 >= -1.0);
			double fTemp246 = static_cast<double>((fTemp244 > 0.0) - (fTemp244 < 0.0));
			double fTemp247 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp244);
			double fTemp248 = ((iTemp245) ? fTemp247 : fConst42 * fVec13[1] * fTemp246 + -0.5);
			double fTemp249 = ((iTemp245) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp244) : fTemp246 * (fTemp247 + -0.16666666666666666));
			double fTemp250 = 2.0 * fVec13[1];
			double fTemp251 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp238 - fVec13[1]));
			double fTemp252 = fConst42 * fTemp238;
			int iTemp253 = (fTemp252 <= 1.0) & (fTemp252 >= -1.0);
			double fTemp254 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp252);
			double fTemp255 = static_cast<double>((fTemp252 > 0.0) - (fTemp252 < 0.0));
			double fTemp256 = 1.1 - 0.16 * fTemp21;
			double fTemp257 = fTemp196 / fTemp256;
			fVec14[0] = fTemp257;
			double fTemp258 = fConst42 * (fVec14[1] - fVec14[2]);
			double fTemp259 = fConst42 * fVec14[2];
			int iTemp260 = (fTemp259 <= 1.0) & (fTemp259 >= -1.0);
			double fTemp261 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp259);
			double fTemp262 = static_cast<double>((fTemp259 > 0.0) - (fTemp259 < 0.0));
			double fTemp263 = fConst42 * fVec14[1];
			int iTemp264 = (fTemp263 <= 1.0) & (fTemp263 >= -1.0);
			double fTemp265 = static_cast<double>((fTemp263 > 0.0) - (fTemp263 < 0.0));
			double fTemp266 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp263);
			double fTemp267 = ((iTemp264) ? fTemp266 : fConst42 * fVec14[1] * fTemp265 + -0.5);
			double fTemp268 = ((iTemp264) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp263) : fTemp265 * (fTemp266 + -0.16666666666666666));
			double fTemp269 = 2.0 * fVec14[1];
			double fTemp270 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp257 - fVec14[1]));
			double fTemp271 = fConst42 * fTemp257;
			int iTemp272 = (fTemp271 <= 1.0) & (fTemp271 >= -1.0);
			double fTemp273 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp271);
			double fTemp274 = static_cast<double>((fTemp271 > 0.0) - (fTemp271 < 0.0));
			double fTemp275 = 1.0 - fTemp236;
			double fTemp276 = fTemp275 * ((iTemp197) ? fTemp256 * (((fTemp270 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp257 + fTemp269))) : (fConst42 * (fTemp196 * (((iTemp272) ? fTemp273 : fConst42 * (fTemp196 * fTemp274 / fTemp256) + -0.5) - fTemp267) / fTemp256) + fTemp268 - ((iTemp272) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp271) : fTemp274 * (fTemp273 + -0.16666666666666666))) / fTemp270) + ((GrungrFaustDSP_faustpower2_f(fTemp258) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp269 + fVec14[2]))) : (fTemp268 + fConst42 * fVec14[2] * (((iTemp260) ? fTemp261 : fConst42 * fVec14[2] * fTemp262 + -0.5) - fTemp267) - ((iTemp260) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp259) : fTemp262 * (fTemp261 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp258))) : fTemp237 * (((fTemp251 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp238 + fTemp250))) : (fConst42 * (fTemp196 * (((iTemp253) ? fTemp254 : fConst42 * (fTemp196 * fTemp255 / fTemp237) + -0.5) - fTemp248) / fTemp237) + fTemp249 - ((iTemp253) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp252) : fTemp255 * (fTemp254 + -0.16666666666666666))) / fTemp251) + ((GrungrFaustDSP_faustpower2_f(fTemp239) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp250 + fVec13[2]))) : (fTemp249 + fConst42 * fVec13[2] * (((iTemp241) ? fTemp242 : fConst42 * fVec13[2] * fTemp243 + -0.5) - fTemp248) - ((iTemp241) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp240) : fTemp243 * (fTemp242 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp239)))) + fTemp236 * ((iTemp197) ? fTemp217 * (((fTemp231 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp218 + fTemp230))) : (fConst42 * (fTemp196 * (((iTemp233) ? fTemp234 : fConst42 * (fTemp196 * fTemp235 / fTemp217) + -0.5) - fTemp228) / fTemp217) + fTemp229 - ((iTemp233) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp232) : fTemp235 * (fTemp234 + -0.16666666666666666))) / fTemp231) + ((GrungrFaustDSP_faustpower2_f(fTemp219) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp230 + fVec12[2]))) : (fTemp229 + fConst42 * fVec12[2] * (((iTemp221) ? fTemp222 : fConst42 * fVec12[2] * fTemp223 + -0.5) - fTemp228) - ((iTemp221) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp220) : fTemp223 * (fTemp222 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp219))) : fTemp198 * (((fTemp212 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp199 + fTemp211))) : (fConst42 * (fTemp196 * (((iTemp214) ? fTemp215 : fConst42 * (fTemp196 * fTemp216 / fTemp198) + -0.5) - fTemp209) / fTemp198) + fTemp210 - ((iTemp214) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp213) : fTemp216 * (fTemp215 + -0.16666666666666666))) / fTemp212) + ((GrungrFaustDSP_faustpower2_f(fTemp200) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp211 + fVec11[2]))) : (fTemp210 + fConst42 * fVec11[2] * (((iTemp202) ? fTemp203 : fConst42 * fVec11[2] * fTemp204 + -0.5) - fTemp209) - ((iTemp202) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp201) : fTemp204 * (fTemp203 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp200))));
			fVec15[0] = fTemp276;
			fRec21[0] = -(fConst53 * (fConst52 * fRec21[1] - fConst39 * (fTemp276 - fVec15[1])));
			fRec20[0] = fRec21[0] - fConst42 * (fConst40 * fRec20[2] + fConst38 * fRec20[1]);
			fRec42[0] = -(fConst53 * (fConst52 * fRec42[1] - (fTemp276 + fVec15[1])));
			fRec41[0] = fRec42[0] - fConst42 * (fConst40 * fRec41[2] + fConst38 * fRec41[1]);
			fRec19[0] = fConst42 * (fRec41[2] + fRec41[0] + 2.0 * fRec41[1] + fConst37 * (fRec20[2] + (fRec20[0] - 2.0 * fRec20[1])) * fTemp79) - (fRec19[2] * fTemp77 + 2.0 * fRec19[1] * fTemp76) / fTemp75;
			double fTemp277 = (fRec19[2] + fRec19[0] + 2.0 * fRec19[1]) / fTemp75;
			fVec16[0] = fTemp277;
			fRec18[0] = 0.995 * fRec18[1] + fTemp277 - fVec16[1];
			fRec17[0] = -(fConst56 * (fConst55 * fRec17[1] - fConst32 * (fRec18[0] - fRec18[1])));
			fRec16[0] = fRec17[0] - fConst35 * (fConst33 * fRec16[2] + fConst31 * fRec16[1]);
			fRec43[0] = fSlow6 + fConst2 * fRec43[1];
			double fTemp278 = std::pow(1e+01, 0.05 * (24.0 * std::pow(std::max<double>(fRec43[0], 2.220446049250313e-16), 0.82) + -9.0));
			fRec45[0] = -(fConst56 * (fConst55 * fRec45[1] - (fRec18[0] + fRec18[1])));
			fRec44[0] = fRec45[0] - fConst35 * (fConst33 * fRec44[2] + fConst31 * fRec44[1]);
			double fTemp279 = (fRec44[2] + fRec44[0] + 2.0 * fRec44[1]) * fTemp278 + fConst30 * (fRec16[2] + (fRec16[0] - 2.0 * fRec16[1]));
			fVec17[0] = fTemp279;
			fRec15[0] = -(fConst59 * (fConst58 * fRec15[1] - fConst57 * (fTemp279 - fVec17[1])));
			fRec14[0] = fRec15[0] - fConst28 * (fConst27 * fRec14[2] + fConst25 * fRec14[1]);
			fRec47[0] = -(fConst59 * (fConst58 * fRec47[1] - fConst35 * (fTemp279 + fVec17[1])));
			fRec46[0] = fRec47[0] - fConst28 * (fConst27 * fRec46[2] + fConst25 * fRec46[1]);
			fRec13[0] = fConst28 * (fRec46[2] + fRec46[0] + 2.0 * fRec46[1] + fConst24 * (fRec14[2] + (fRec14[0] - 2.0 * fRec14[1])) * fTemp72) - (fRec13[2] * fTemp71 + fTemp70) / fTemp67;
			double fTemp280 = fConst21 * (fConst21 + fTemp68) + 1.0;
			fRec12[0] = (fTemp70 + fRec13[0] * fTemp280 + fRec13[2] * fTemp69) / fTemp67 - (fRec12[2] * fTemp62 + fTemp61) / fTemp58;
			double fTemp281 = fConst17 * (fConst17 + fTemp59) + 1.0;
			fRec11[0] = (fTemp61 + fRec12[0] * fTemp281 + fRec12[2] * fTemp60) / fTemp58 - (fRec11[2] * fTemp53 + fTemp52) / fTemp49;
			double fTemp282 = fConst13 * (fConst13 + fTemp50) + 1.0;
			double fTemp283 = (fTemp52 + fRec11[0] * fTemp282 + fRec11[2] * fTemp51) / fTemp49;
			fVec18[0] = fTemp283;
			fRec10[0] = fConst61 * (fConst60 * (fTemp283 - fVec18[1]) - fConst10 * fRec10[1]);
			double fTemp284 = 1.0 - fTemp35;
			fRec9[0] = -((fRec9[1] * fTemp284 - (fRec10[0] - fRec10[1]) / fTemp34) / fTemp43);
			fRec8[0] = fRec9[0] - (fRec8[2] * fTemp42 + 2.0 * fRec8[1] * fTemp41) / fTemp36;
			fRec49[0] = -((fTemp284 * fRec49[1] - (fRec10[0] + fRec10[1])) / fTemp43);
			fRec48[0] = fRec49[0] - (fTemp42 * fRec48[2] + 2.0 * fTemp41 * fRec48[1]) / fTemp36;
			fRec7[0] = (fRec48[2] + fRec48[0] + 2.0 * fRec48[1] + (fRec8[2] + (fRec8[0] - 2.0 * fRec8[1])) * fTemp40 / fTemp37) / fTemp36 - (fRec7[2] * fTemp33 + fTemp32) / fTemp29;
			double fTemp285 = fConst6 * (fConst6 + fTemp30) + 1.0;
			fRec6[0] = (fTemp32 + fRec7[0] * fTemp285 + fRec7[2] * fTemp31) / fTemp29 - (fRec6[2] * fTemp24 + 2.0 * fRec6[1] * fTemp23) / fTemp17;
			fRec50[0] = fSlow7 + fConst2 * fRec50[1];
			double fTemp286 = std::pow(std::max<double>(fRec50[0], 2.220446049250313e-16), 1.65);
			double fTemp287 = fTemp286 * (fRec6[2] + fRec6[0] + 2.0 * fRec6[1]) * fTemp22 / fTemp17;
			double fTemp288 = fConst51 + fRec4[1];
			double fTemp289 = fRec4[1] - fConst51;
			fRec4[0] = ((fTemp288 < fTemp287) ? fTemp288 : ((fTemp289 > fTemp287) ? fTemp289 : fTemp287));
			fRec3[0] = fConst1 * std::fabs(fRec4[0]) + fConst2 * fRec3[1];
			double fTemp290 = fRec4[0] * (1.0 - fTemp7 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec3[0] * fTemp9 / fTemp7)))) * fTemp4;
			fVec19[0] = fTemp290;
			double fTemp291 = fVec19[1] - fVec19[2];
			double fTemp292 = 0.8928571428571428 * fTemp291;
			double fTemp293 = 0.8928571428571428 * fVec19[2];
			double fTemp294 = GrungrFaustDSP_faustpower2_f(fTemp293) + 1.0;
			double fTemp295 = std::atan(fTemp293);
			double fTemp296 = 0.8928571428571428 * fVec19[1];
			double fTemp297 = GrungrFaustDSP_faustpower2_f(fTemp296) + 1.0;
			double fTemp298 = std::atan(fTemp296);
			double fTemp299 = fTemp298 * fTemp297;
			double fTemp300 = fVec19[1] * fTemp298;
			double fTemp301 = std::log(fTemp297);
			double fTemp302 = 1.7857142857142856 * fVec19[1];
			double fTemp303 = fTemp290 - fVec19[1];
			double fTemp304 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp303);
			double fTemp305 = 0.8928571428571428 * fTemp290;
			double fTemp306 = GrungrFaustDSP_faustpower2_f(fTemp305) + 1.0;
			double fTemp307 = std::atan(fTemp305);
			output0[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp131 : 0.7130141450516913 * (((fTemp304 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp305 + fTemp302)) : (0.8928571428571428 * fTemp290 * (0.8928571428571428 * (fTemp290 * fTemp307 - fTemp300) - 0.5 * (std::log(fTemp306) - fTemp301)) + 0.4464285714285714 * fTemp303 - 0.5 * (fTemp307 * fTemp306 - fTemp299)) / fTemp304) + ((GrungrFaustDSP_faustpower2_f(fTemp292) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp302 + fTemp293)) : (0.8928571428571428 * fVec19[2] * (0.5 * (fTemp301 - std::log(fTemp294)) - 0.8928571428571428 * (fTemp300 - fVec19[2] * fTemp295)) + 0.5 * (fTemp299 - fTemp295 * fTemp294) - 0.4464285714285714 * fTemp291) / GrungrFaustDSP_faustpower2_f(-fTemp292)))));
			double fTemp308 = fConst7 * fRec54[1];
			double fTemp309 = fConst14 * fRec58[1];
			double fTemp310 = fConst18 * fRec59[1];
			double fTemp311 = fConst22 * fRec60[1];
			double fTemp312 = static_cast<double>(input1[i0]);
			double fTemp313 = ((iSlow0) ? 0.0 : fTemp312);
			fVec20[0] = fTemp313;
			fRec85[0] = (0.2 * ((fTemp313 - fVec20[1]) / fTemp127) - fTemp130 * fRec85[1]) / fTemp129;
			fRec84[0] = fRec85[0] - (fTemp126 * fRec84[2] + 2.0 * fTemp125 * fRec84[1]) / fTemp122;
			double fTemp314 = (fRec84[2] + (fRec84[0] - 2.0 * fRec84[1])) / fTemp124;
			fVec21[0] = fTemp314;
			fRec83[0] = -(fConst48 * (fConst47 * fRec83[1] - (fTemp314 + fVec21[1])));
			double fTemp315 = (fRec83[0] + 0.01) * fTemp118;
			fVec22[0] = fTemp315;
			double fTemp316 = fVec22[1] - fVec22[2];
			double fTemp317 = 0.5555555555555556 * fTemp316;
			double fTemp318 = 0.5555555555555556 * fVec22[2];
			double fTemp319 = GrungrFaustDSP_faustpower2_f(fTemp318) + 1.0;
			double fTemp320 = std::atan(fTemp318);
			double fTemp321 = 0.5555555555555556 * fVec22[1];
			double fTemp322 = GrungrFaustDSP_faustpower2_f(fTemp321) + 1.0;
			double fTemp323 = std::atan(fTemp321);
			double fTemp324 = fTemp323 * fTemp322;
			double fTemp325 = fVec22[1] * fTemp323;
			double fTemp326 = std::log(fTemp322);
			double fTemp327 = 1.1111111111111112 * fVec22[1];
			double fTemp328 = fTemp315 - fVec22[1];
			double fTemp329 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp328);
			double fTemp330 = 0.5555555555555556 * fTemp315;
			double fTemp331 = GrungrFaustDSP_faustpower2_f(fTemp330) + 1.0;
			double fTemp332 = std::atan(fTemp330);
			double fTemp333 = fTemp119 - (((fTemp329 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp330 + fTemp327)) : (0.5555555555555556 * fTemp315 * (0.5555555555555556 * (fTemp315 * fTemp332 - fTemp325) - 0.5 * (std::log(fTemp331) - fTemp326)) + 0.2777777777777778 * fTemp328 - 0.5 * (fTemp332 * fTemp331 - fTemp324)) / fTemp329) + ((GrungrFaustDSP_faustpower2_f(fTemp317) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp327 + fTemp318)) : (0.5555555555555556 * fVec22[2] * (0.5 * (fTemp326 - std::log(fTemp319)) - 0.5555555555555556 * (fTemp325 - fVec22[2] * fTemp320)) + 0.5 * (fTemp324 - fTemp320 * fTemp319) - 0.2777777777777778 * fTemp316) / GrungrFaustDSP_faustpower2_f(-fTemp317)));
			fVec23[0] = fTemp333;
			fRec82[0] = -(fConst50 * (fConst45 * fRec82[1] + fConst49 * (fTemp333 - fVec23[1])));
			double fTemp334 = fRec82[0] * fTemp117;
			double fTemp335 = fConst51 + fRec81[1];
			double fTemp336 = fRec81[1] - fConst51;
			fRec81[0] = ((fTemp335 < fTemp334) ? fTemp335 : ((fTemp336 > fTemp334) ? fTemp336 : fTemp334));
			double fTemp337 = (fRec81[0] + 0.014) * fTemp115 * fTemp114;
			double fTemp338 = fTemp337 / fTemp113;
			fVec24[0] = fTemp338;
			double fTemp339 = GrungrFaustDSP_faustpower2_f(fVec24[2]);
			double fTemp340 = std::atan(fVec24[2]);
			double fTemp341 = GrungrFaustDSP_faustpower2_f(fVec24[1]);
			double fTemp342 = std::atan(fVec24[1]);
			double fTemp343 = fTemp342 - (fVec24[1] - fTemp342 * fTemp341);
			double fTemp344 = fVec24[1] * fTemp342;
			double fTemp345 = std::log(fTemp341 + 1.0);
			double fTemp346 = 2.0 * fVec24[1];
			double fTemp347 = GrungrFaustDSP_faustpower2_f(fTemp338 - fVec24[1]);
			double fTemp348 = GrungrFaustDSP_faustpower2_f(fTemp338);
			double fTemp349 = std::atan(fTemp338);
			double fTemp350 = fTemp113 * (fTemp116 - (((fTemp347 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp338 + fTemp346)) : (fTemp337 * (fTemp337 * fTemp349 / fTemp113 - (fTemp344 + 0.5 * (std::log(fTemp348 + 1.0) - fTemp345))) / fTemp113 - 0.5 * (fTemp349 - (fTemp338 - fTemp349 * fTemp348) - fTemp343)) / fTemp347) + ((GrungrFaustDSP_faustpower2_f(fVec24[1] - fVec24[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp346 + fVec24[2])) : (fVec24[2] * (fVec24[2] * fTemp340 + 0.5 * (fTemp345 - std::log(fTemp339 + 1.0)) - fTemp344) + 0.5 * (fTemp343 - (fTemp340 - (fVec24[2] - fTemp340 * fTemp339)))) / GrungrFaustDSP_faustpower2_f(fVec24[2] - fVec24[1]))));
			fVec25[0] = fTemp350;
			fRec80[0] = 0.995 * fRec80[1] - 0.6366197723675814 * (fTemp350 - fVec25[1]);
			fRec79[0] = fRec80[0] - (fTemp111 * fRec79[2] + 2.0 * fTemp110 * fRec79[1]) / fTemp109;
			double fTemp351 = (fRec79[2] + fRec79[0] + 2.0 * fRec79[1]) / fTemp109;
			fVec26[0] = fTemp351;
			fRec78[0] = -((fTemp171 * fRec78[1] - (fTemp351 - fVec26[1]) / fTemp104) / fTemp106);
			fRec77[0] = -((fTemp172 * fRec77[1] - (fRec78[0] + fRec78[1])) / fTemp102);
			double fTemp352 = fRec77[0] * fTemp100;
			double fTemp353 = fConst51 + fRec76[1];
			double fTemp354 = fRec76[1] - fConst51;
			fRec76[0] = ((fTemp353 < fTemp352) ? fTemp353 : ((fTemp354 > fTemp352) ? fTemp354 : fTemp352));
			double fTemp355 = fTemp96 * fTemp95 * (fRec76[0] + fTemp97);
			double fTemp356 = fTemp355 / fTemp94;
			fVec27[0] = fTemp356;
			double fTemp357 = GrungrFaustDSP_faustpower2_f(fVec27[2]);
			double fTemp358 = std::atan(fVec27[2]);
			double fTemp359 = GrungrFaustDSP_faustpower2_f(fVec27[1]);
			double fTemp360 = std::atan(fVec27[1]);
			double fTemp361 = fTemp360 - (fVec27[1] - fTemp360 * fTemp359);
			double fTemp362 = fVec27[1] * fTemp360;
			double fTemp363 = std::log(fTemp359 + 1.0);
			double fTemp364 = 2.0 * fVec27[1];
			double fTemp365 = GrungrFaustDSP_faustpower2_f(fTemp356 - fVec27[1]);
			double fTemp366 = GrungrFaustDSP_faustpower2_f(fTemp356);
			double fTemp367 = std::atan(fTemp356);
			double fTemp368 = fTemp94 * (fTemp98 - (((fTemp365 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp356 + fTemp364)) : (fTemp355 * (fTemp355 * fTemp367 / fTemp94 - (fTemp362 + 0.5 * (std::log(fTemp366 + 1.0) - fTemp363))) / fTemp94 - 0.5 * (fTemp367 - (fTemp356 - fTemp367 * fTemp366) - fTemp361)) / fTemp365) + ((GrungrFaustDSP_faustpower2_f(fVec27[1] - fVec27[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp364 + fVec27[2])) : (fVec27[2] * (fVec27[2] * fTemp358 + 0.5 * (fTemp363 - std::log(fTemp357 + 1.0)) - fTemp362) + 0.5 * (fTemp361 - (fTemp358 - (fVec27[2] - fTemp358 * fTemp357)))) / GrungrFaustDSP_faustpower2_f(fVec27[2] - fVec27[1]))));
			fVec28[0] = fTemp368;
			fRec75[0] = 0.995 * fRec75[1] - 0.6366197723675814 * (fTemp368 - fVec28[1]);
			fRec74[0] = fRec75[0] - (fTemp91 * fRec74[2] + 2.0 * fTemp90 * fRec74[1]) / fTemp89;
			double fTemp369 = (fRec74[2] + fRec74[0] + 2.0 * fRec74[1]) / fTemp89;
			fVec29[0] = fTemp369;
			fRec73[0] = -((fTemp191 * fRec73[1] - (fTemp369 - fVec29[1]) / fTemp83) / fTemp85);
			double fTemp370 = fRec73[0] * fTemp82;
			double fTemp371 = fConst51 + fRec72[1];
			double fTemp372 = fRec72[1] - fConst51;
			fRec72[0] = ((fTemp371 < fTemp370) ? fTemp371 : ((fTemp372 > fTemp370) ? fTemp372 : fTemp370));
			fRec71[0] = fConst1 * std::fabs(fRec72[0]) + fConst2 * fRec71[1];
			double fTemp373 = fRec72[0] * (1.0 - fTemp81 * std::min<double>(1.0, std::max<double>(0.0, fRec71[0] * fTemp9 / fTemp81)));
			fVec30[0] = fTemp373;
			fRec70[0] = -(fConst53 * (fConst52 * fRec70[1] - fConst39 * (fTemp373 - fVec30[1])));
			fRec69[0] = fRec70[0] - fConst42 * (fConst40 * fRec69[2] + fConst38 * fRec69[1]);
			fRec87[0] = -(fConst53 * (fConst52 * fRec87[1] - (fTemp373 + fVec30[1])));
			fRec86[0] = fRec87[0] - fConst42 * (fConst40 * fRec86[2] + fConst38 * fRec86[1]);
			double fTemp374 = fRec86[2] + fRec86[0] + 2.0 * fRec86[1] + fConst37 * fTemp80 * (fRec69[2] + (fRec69[0] - 2.0 * fRec69[1]));
			int iTemp375 = (fConst42 * fTemp374) > 0.0;
			double fTemp376 = fTemp374 / fTemp198;
			fVec31[0] = fTemp376;
			double fTemp377 = fConst42 * (fVec31[1] - fVec31[2]);
			double fTemp378 = fConst42 * fVec31[2];
			int iTemp379 = (fTemp378 <= 1.0) & (fTemp378 >= -1.0);
			double fTemp380 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp378);
			double fTemp381 = static_cast<double>((fTemp378 > 0.0) - (fTemp378 < 0.0));
			double fTemp382 = fConst42 * fVec31[1];
			int iTemp383 = (fTemp382 <= 1.0) & (fTemp382 >= -1.0);
			double fTemp384 = static_cast<double>((fTemp382 > 0.0) - (fTemp382 < 0.0));
			double fTemp385 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp382);
			double fTemp386 = ((iTemp383) ? fTemp385 : fConst42 * fVec31[1] * fTemp384 + -0.5);
			double fTemp387 = ((iTemp383) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp382) : fTemp384 * (fTemp385 + -0.16666666666666666));
			double fTemp388 = 2.0 * fVec31[1];
			double fTemp389 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp376 - fVec31[1]));
			double fTemp390 = fConst42 * fTemp376;
			int iTemp391 = (fTemp390 <= 1.0) & (fTemp390 >= -1.0);
			double fTemp392 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp390);
			double fTemp393 = static_cast<double>((fTemp390 > 0.0) - (fTemp390 < 0.0));
			double fTemp394 = fTemp374 / fTemp217;
			fVec32[0] = fTemp394;
			double fTemp395 = fConst42 * (fVec32[1] - fVec32[2]);
			double fTemp396 = fConst42 * fVec32[2];
			int iTemp397 = (fTemp396 <= 1.0) & (fTemp396 >= -1.0);
			double fTemp398 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp396);
			double fTemp399 = static_cast<double>((fTemp396 > 0.0) - (fTemp396 < 0.0));
			double fTemp400 = fConst42 * fVec32[1];
			int iTemp401 = (fTemp400 <= 1.0) & (fTemp400 >= -1.0);
			double fTemp402 = static_cast<double>((fTemp400 > 0.0) - (fTemp400 < 0.0));
			double fTemp403 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp400);
			double fTemp404 = ((iTemp401) ? fTemp403 : fConst42 * fVec32[1] * fTemp402 + -0.5);
			double fTemp405 = ((iTemp401) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp400) : fTemp402 * (fTemp403 + -0.16666666666666666));
			double fTemp406 = 2.0 * fVec32[1];
			double fTemp407 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp394 - fVec32[1]));
			double fTemp408 = fConst42 * fTemp394;
			int iTemp409 = (fTemp408 <= 1.0) & (fTemp408 >= -1.0);
			double fTemp410 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp408);
			double fTemp411 = static_cast<double>((fTemp408 > 0.0) - (fTemp408 < 0.0));
			double fTemp412 = fTemp374 / fTemp237;
			fVec33[0] = fTemp412;
			double fTemp413 = fConst42 * (fVec33[1] - fVec33[2]);
			double fTemp414 = fConst42 * fVec33[2];
			int iTemp415 = (fTemp414 <= 1.0) & (fTemp414 >= -1.0);
			double fTemp416 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp414);
			double fTemp417 = static_cast<double>((fTemp414 > 0.0) - (fTemp414 < 0.0));
			double fTemp418 = fConst42 * fVec33[1];
			int iTemp419 = (fTemp418 <= 1.0) & (fTemp418 >= -1.0);
			double fTemp420 = static_cast<double>((fTemp418 > 0.0) - (fTemp418 < 0.0));
			double fTemp421 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp418);
			double fTemp422 = ((iTemp419) ? fTemp421 : fConst42 * fVec33[1] * fTemp420 + -0.5);
			double fTemp423 = ((iTemp419) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp418) : fTemp420 * (fTemp421 + -0.16666666666666666));
			double fTemp424 = 2.0 * fVec33[1];
			double fTemp425 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp412 - fVec33[1]));
			double fTemp426 = fConst42 * fTemp412;
			int iTemp427 = (fTemp426 <= 1.0) & (fTemp426 >= -1.0);
			double fTemp428 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp426);
			double fTemp429 = static_cast<double>((fTemp426 > 0.0) - (fTemp426 < 0.0));
			double fTemp430 = fTemp374 / fTemp256;
			fVec34[0] = fTemp430;
			double fTemp431 = fConst42 * (fVec34[1] - fVec34[2]);
			double fTemp432 = fConst42 * fVec34[2];
			int iTemp433 = (fTemp432 <= 1.0) & (fTemp432 >= -1.0);
			double fTemp434 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp432);
			double fTemp435 = static_cast<double>((fTemp432 > 0.0) - (fTemp432 < 0.0));
			double fTemp436 = fConst42 * fVec34[1];
			int iTemp437 = (fTemp436 <= 1.0) & (fTemp436 >= -1.0);
			double fTemp438 = static_cast<double>((fTemp436 > 0.0) - (fTemp436 < 0.0));
			double fTemp439 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp436);
			double fTemp440 = ((iTemp437) ? fTemp439 : fConst42 * fVec34[1] * fTemp438 + -0.5);
			double fTemp441 = ((iTemp437) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp436) : fTemp438 * (fTemp439 + -0.16666666666666666));
			double fTemp442 = 2.0 * fVec34[1];
			double fTemp443 = GrungrFaustDSP_faustpower2_f(fConst42 * (fTemp430 - fVec34[1]));
			double fTemp444 = fConst42 * fTemp430;
			int iTemp445 = (fTemp444 <= 1.0) & (fTemp444 >= -1.0);
			double fTemp446 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp444);
			double fTemp447 = static_cast<double>((fTemp444 > 0.0) - (fTemp444 < 0.0));
			double fTemp448 = fTemp275 * ((iTemp375) ? fTemp256 * (((fTemp443 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp430 + fTemp442))) : (fConst42 * (fTemp374 * (((iTemp445) ? fTemp446 : fConst42 * (fTemp374 * fTemp447 / fTemp256) + -0.5) - fTemp440) / fTemp256) + fTemp441 - ((iTemp445) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp444) : fTemp447 * (fTemp446 + -0.16666666666666666))) / fTemp443) + ((GrungrFaustDSP_faustpower2_f(fTemp431) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp442 + fVec34[2]))) : (fTemp441 + fConst42 * fVec34[2] * (((iTemp433) ? fTemp434 : fConst42 * fVec34[2] * fTemp435 + -0.5) - fTemp440) - ((iTemp433) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp432) : fTemp435 * (fTemp434 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp431))) : fTemp237 * (((fTemp425 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp412 + fTemp424))) : (fConst42 * (fTemp374 * (((iTemp427) ? fTemp428 : fConst42 * (fTemp374 * fTemp429 / fTemp237) + -0.5) - fTemp422) / fTemp237) + fTemp423 - ((iTemp427) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp426) : fTemp429 * (fTemp428 + -0.16666666666666666))) / fTemp425) + ((GrungrFaustDSP_faustpower2_f(fTemp413) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp424 + fVec33[2]))) : (fTemp423 + fConst42 * fVec33[2] * (((iTemp415) ? fTemp416 : fConst42 * fVec33[2] * fTemp417 + -0.5) - fTemp422) - ((iTemp415) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp414) : fTemp417 * (fTemp416 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp413)))) + fTemp236 * ((iTemp375) ? fTemp217 * (((fTemp407 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp394 + fTemp406))) : (fConst42 * (fTemp374 * (((iTemp409) ? fTemp410 : fConst42 * (fTemp374 * fTemp411 / fTemp217) + -0.5) - fTemp404) / fTemp217) + fTemp405 - ((iTemp409) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp408) : fTemp411 * (fTemp410 + -0.16666666666666666))) / fTemp407) + ((GrungrFaustDSP_faustpower2_f(fTemp395) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp406 + fVec32[2]))) : (fTemp405 + fConst42 * fVec32[2] * (((iTemp397) ? fTemp398 : fConst42 * fVec32[2] * fTemp399 + -0.5) - fTemp404) - ((iTemp397) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp396) : fTemp399 * (fTemp398 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp395))) : fTemp198 * (((fTemp389 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp376 + fTemp388))) : (fConst42 * (fTemp374 * (((iTemp391) ? fTemp392 : fConst42 * (fTemp374 * fTemp393 / fTemp198) + -0.5) - fTemp386) / fTemp198) + fTemp387 - ((iTemp391) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp390) : fTemp393 * (fTemp392 + -0.16666666666666666))) / fTemp389) + ((GrungrFaustDSP_faustpower2_f(fTemp377) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst54 * (fTemp388 + fVec31[2]))) : (fTemp387 + fConst42 * fVec31[2] * (((iTemp379) ? fTemp380 : fConst42 * fVec31[2] * fTemp381 + -0.5) - fTemp386) - ((iTemp379) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp378) : fTemp381 * (fTemp380 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp377))));
			fVec35[0] = fTemp448;
			fRec68[0] = -(fConst53 * (fConst52 * fRec68[1] - fConst39 * (fTemp448 - fVec35[1])));
			fRec67[0] = fRec68[0] - fConst42 * (fConst40 * fRec67[2] + fConst38 * fRec67[1]);
			fRec89[0] = -(fConst53 * (fConst52 * fRec89[1] - (fTemp448 + fVec35[1])));
			fRec88[0] = fRec89[0] - fConst42 * (fConst40 * fRec88[2] + fConst38 * fRec88[1]);
			fRec66[0] = fConst42 * (fRec88[2] + fRec88[0] + 2.0 * fRec88[1] + fConst37 * fTemp79 * (fRec67[2] + (fRec67[0] - 2.0 * fRec67[1]))) - (fTemp77 * fRec66[2] + 2.0 * fTemp76 * fRec66[1]) / fTemp75;
			double fTemp449 = (fRec66[2] + fRec66[0] + 2.0 * fRec66[1]) / fTemp75;
			fVec36[0] = fTemp449;
			fRec65[0] = 0.995 * fRec65[1] + fTemp449 - fVec36[1];
			fRec64[0] = -(fConst56 * (fConst55 * fRec64[1] - fConst32 * (fRec65[0] - fRec65[1])));
			fRec63[0] = fRec64[0] - fConst35 * (fConst33 * fRec63[2] + fConst31 * fRec63[1]);
			fRec91[0] = -(fConst56 * (fConst55 * fRec91[1] - (fRec65[0] + fRec65[1])));
			fRec90[0] = fRec91[0] - fConst35 * (fConst33 * fRec90[2] + fConst31 * fRec90[1]);
			double fTemp450 = fTemp278 * (fRec90[2] + fRec90[0] + 2.0 * fRec90[1]) + fConst30 * (fRec63[2] + (fRec63[0] - 2.0 * fRec63[1]));
			fVec37[0] = fTemp450;
			fRec62[0] = -(fConst59 * (fConst58 * fRec62[1] - fConst57 * (fTemp450 - fVec37[1])));
			fRec61[0] = fRec62[0] - fConst28 * (fConst27 * fRec61[2] + fConst25 * fRec61[1]);
			fRec93[0] = -(fConst59 * (fConst58 * fRec93[1] - fConst35 * (fTemp450 + fVec37[1])));
			fRec92[0] = fRec93[0] - fConst28 * (fConst27 * fRec92[2] + fConst25 * fRec92[1]);
			fRec60[0] = fConst28 * (fRec92[2] + fRec92[0] + 2.0 * fRec92[1] + fConst24 * fTemp72 * (fRec61[2] + (fRec61[0] - 2.0 * fRec61[1]))) - (fTemp71 * fRec60[2] + fTemp311) / fTemp67;
			fRec59[0] = (fTemp311 + fRec60[0] * fTemp280 + fTemp69 * fRec60[2]) / fTemp67 - (fTemp62 * fRec59[2] + fTemp310) / fTemp58;
			fRec58[0] = (fTemp310 + fRec59[0] * fTemp281 + fTemp60 * fRec59[2]) / fTemp58 - (fTemp53 * fRec58[2] + fTemp309) / fTemp49;
			double fTemp451 = (fTemp309 + fRec58[0] * fTemp282 + fTemp51 * fRec58[2]) / fTemp49;
			fVec38[0] = fTemp451;
			fRec57[0] = fConst61 * (fConst60 * (fTemp451 - fVec38[1]) - fConst10 * fRec57[1]);
			fRec56[0] = -((fTemp284 * fRec56[1] - (fRec57[0] - fRec57[1]) / fTemp34) / fTemp43);
			fRec55[0] = fRec56[0] - (fTemp42 * fRec55[2] + 2.0 * fTemp41 * fRec55[1]) / fTemp36;
			fRec95[0] = -((fTemp284 * fRec95[1] - (fRec57[0] + fRec57[1])) / fTemp43);
			fRec94[0] = fRec95[0] - (fTemp42 * fRec94[2] + 2.0 * fTemp41 * fRec94[1]) / fTemp36;
			fRec54[0] = (fRec94[2] + fRec94[0] + 2.0 * fRec94[1] + fTemp40 * (fRec55[2] + (fRec55[0] - 2.0 * fRec55[1])) / fTemp37) / fTemp36 - (fTemp33 * fRec54[2] + fTemp308) / fTemp29;
			fRec53[0] = (fTemp308 + fRec54[0] * fTemp285 + fTemp31 * fRec54[2]) / fTemp29 - (fTemp24 * fRec53[2] + 2.0 * fTemp23 * fRec53[1]) / fTemp17;
			double fTemp452 = fTemp286 * fTemp22 * (fRec53[2] + fRec53[0] + 2.0 * fRec53[1]) / fTemp17;
			double fTemp453 = fConst51 + fRec52[1];
			double fTemp454 = fRec52[1] - fConst51;
			fRec52[0] = ((fTemp453 < fTemp452) ? fTemp453 : ((fTemp454 > fTemp452) ? fTemp454 : fTemp452));
			fRec51[0] = fConst1 * std::fabs(fRec52[0]) + fConst2 * fRec51[1];
			double fTemp455 = fRec52[0] * fTemp4 * (1.0 - fTemp7 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec51[0] * fTemp9 / fTemp7))));
			fVec39[0] = fTemp455;
			double fTemp456 = fVec39[1] - fVec39[2];
			double fTemp457 = 0.8928571428571428 * fTemp456;
			double fTemp458 = 0.8928571428571428 * fVec39[2];
			double fTemp459 = GrungrFaustDSP_faustpower2_f(fTemp458) + 1.0;
			double fTemp460 = std::atan(fTemp458);
			double fTemp461 = 0.8928571428571428 * fVec39[1];
			double fTemp462 = GrungrFaustDSP_faustpower2_f(fTemp461) + 1.0;
			double fTemp463 = std::atan(fTemp461);
			double fTemp464 = fTemp463 * fTemp462;
			double fTemp465 = fVec39[1] * fTemp463;
			double fTemp466 = std::log(fTemp462);
			double fTemp467 = 1.7857142857142856 * fVec39[1];
			double fTemp468 = fTemp455 - fVec39[1];
			double fTemp469 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp468);
			double fTemp470 = 0.8928571428571428 * fTemp455;
			double fTemp471 = GrungrFaustDSP_faustpower2_f(fTemp470) + 1.0;
			double fTemp472 = std::atan(fTemp470);
			output1[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp312 : 0.7130141450516913 * (((fTemp469 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp470 + fTemp467)) : (0.8928571428571428 * fTemp455 * (0.8928571428571428 * (fTemp455 * fTemp472 - fTemp465) - 0.5 * (std::log(fTemp471) - fTemp466)) + 0.4464285714285714 * fTemp468 - 0.5 * (fTemp472 * fTemp471 - fTemp464)) / fTemp469) + ((GrungrFaustDSP_faustpower2_f(fTemp457) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp467 + fTemp458)) : (0.8928571428571428 * fVec39[2] * (0.5 * (fTemp466 - std::log(fTemp459)) - 0.8928571428571428 * (fTemp465 - fVec39[2] * fTemp460)) + 0.5 * (fTemp464 - fTemp460 * fTemp459) - 0.4464285714285714 * fTemp456) / GrungrFaustDSP_faustpower2_f(-fTemp457)))));
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fVec0[1] = fVec0[0];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fVec1[1] = fVec1[0];
			fRec36[1] = fRec36[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fVec6[1] = fVec6[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fVec9[1] = fVec9[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fVec10[1] = fVec10[0];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fVec11[2] = fVec11[1];
			fVec11[1] = fVec11[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fVec13[2] = fVec13[1];
			fVec13[1] = fVec13[0];
			fVec14[2] = fVec14[1];
			fVec14[1] = fVec14[0];
			fVec15[1] = fVec15[0];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec42[1] = fRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fVec16[1] = fVec16[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec43[1] = fRec43[0];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fVec17[1] = fVec17[0];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fVec18[1] = fVec18[0];
			fRec10[1] = fRec10[0];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec50[1] = fRec50[0];
			fRec4[1] = fRec4[0];
			fRec3[1] = fRec3[0];
			fVec19[2] = fVec19[1];
			fVec19[1] = fVec19[0];
			fVec20[1] = fVec20[0];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fVec21[1] = fVec21[0];
			fRec83[1] = fRec83[0];
			fVec22[2] = fVec22[1];
			fVec22[1] = fVec22[0];
			fVec23[1] = fVec23[0];
			fRec82[1] = fRec82[0];
			fRec81[1] = fRec81[0];
			fVec24[2] = fVec24[1];
			fVec24[1] = fVec24[0];
			fVec25[1] = fVec25[0];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fVec26[1] = fVec26[0];
			fRec78[1] = fRec78[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fVec27[2] = fVec27[1];
			fVec27[1] = fVec27[0];
			fVec28[1] = fVec28[0];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fVec29[1] = fVec29[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fVec30[1] = fVec30[0];
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fVec31[2] = fVec31[1];
			fVec31[1] = fVec31[0];
			fVec32[2] = fVec32[1];
			fVec32[1] = fVec32[0];
			fVec33[2] = fVec33[1];
			fVec33[1] = fVec33[0];
			fVec34[2] = fVec34[1];
			fVec34[1] = fVec34[0];
			fVec35[1] = fVec35[0];
			fRec68[1] = fRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec89[1] = fRec89[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fVec36[1] = fVec36[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fVec37[1] = fVec37[0];
			fRec62[1] = fRec62[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fVec38[1] = fVec38[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec95[1] = fRec95[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fVec39[2] = fVec39[1];
			fVec39[1] = fVec39[0];
		}
	}

};

#endif
