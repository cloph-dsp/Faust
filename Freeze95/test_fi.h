/* ------------------------------------------------------------
name: "test_fi"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -ct 1 -cn Test -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __Test_H__
#define  __Test_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Test
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


class Test : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fRec6[2];
	float fConst4;
	float fRec7[2];
	float fConst5;
	float fRec3[2];
	float fRec4[2];
	float fConst6;
	float fConst7;
	float fConst8;
	float fRec0[2];
	float fConst9;
	float fRec1[2];
	float fConst10;
	float fRec17[2];
	float fRec18[2];
	float fRec13[2];
	float fRec14[2];
	float fRec10[2];
	float fRec11[2];
	float fRec21[2];
	float fRec22[2];
	
 public:
	Test() {
	}
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -light -ct 1 -cn Test -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "test_fi.dsp");
		m->declare("filters.lib/crossover2LR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/crossover2LR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/crossover2LR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/crossover3LR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/crossover3LR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/crossover3LR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpassLR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/highpassLR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/highpassLR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpassLR4:author", "Dario Sanfilippo");
		m->declare("filters.lib/lowpassLR4:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/lowpassLR4:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/svf:author", "Oleg Nesterov");
		m->declare("filters.lib/svf:copyright", "Copyright (C) 2020 Oleg Nesterov <oleg@redhat.com>");
		m->declare("filters.lib/svf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "test_fi");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 3;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::tan(628.31854f / fConst0);
		fConst2 = fConst1 * (fConst1 + 1.4142135f) + 1.0f;
		fConst3 = 2.0f / fConst2;
		fConst4 = fConst1 / fConst2;
		fConst5 = 1.0f / fConst2;
		fConst6 = std::tan(6283.1855f / fConst0);
		fConst7 = fConst6 * (fConst6 + 1.4142135f) + 1.0f;
		fConst8 = 2.0f / fConst7;
		fConst9 = fConst6 / fConst7;
		fConst10 = 1.0f / fConst7;
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec6[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec7[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec0[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec1[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec17[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec18[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec13[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec14[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec10[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec21[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec22[l13] = 0.0f;
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
	
	virtual Test* clone() {
		return new Test();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("test_fi");
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = fRec6[1] + fConst1 * (fTemp0 - fRec7[1]);
			fRec6[0] = fConst3 * fTemp1 - fRec6[1];
			float fTemp2 = fRec7[1] + fConst4 * fTemp1;
			fRec7[0] = 2.0f * fTemp2 - fRec7[1];
			float fRec8 = fConst5 * fTemp1;
			float fRec9 = fTemp2;
			float fTemp3 = fRec3[1] + fConst1 * (fRec9 - fRec4[1]);
			fRec3[0] = fConst3 * fTemp3 - fRec3[1];
			float fTemp4 = fRec4[1] + fConst4 * fTemp3;
			fRec4[0] = 2.0f * fTemp4 - fRec4[1];
			float fRec5 = fTemp4;
			float fTemp5 = fRec0[1] + fConst6 * (fRec5 - fRec1[1]);
			fRec0[0] = fConst8 * fTemp5 - fRec0[1];
			float fTemp6 = fRec1[1] + fConst9 * fTemp5;
			fRec1[0] = 2.0f * fTemp6 - fRec1[1];
			float fRec2 = fConst10 * fTemp5;
			output0[i0] = FAUSTFLOAT(fRec5 - 2.828427f * fRec2);
			float fTemp7 = fRec9 + 1.4142135f * fRec8;
			float fTemp8 = fRec17[1] + fConst1 * (fTemp0 - (fTemp7 + fRec18[1]));
			fRec17[0] = fConst3 * fTemp8 - fRec17[1];
			float fTemp9 = fRec18[1] + fConst4 * fTemp8;
			fRec18[0] = 2.0f * fTemp9 - fRec18[1];
			float fRec19 = fConst5 * fTemp8;
			float fRec20 = fTemp9;
			float fTemp10 = fRec9 + fRec20;
			float fTemp11 = fRec8 + fRec19;
			float fTemp12 = 1.4142135f * fTemp11 + fTemp10;
			float fTemp13 = fRec13[1] + fConst6 * (fTemp0 - (fTemp12 + fRec14[1]));
			fRec13[0] = fConst8 * fTemp13 - fRec13[1];
			float fTemp14 = fRec14[1] + fConst9 * fTemp13;
			fRec14[0] = 2.0f * fTemp14 - fRec14[1];
			float fRec15 = fConst10 * fTemp13;
			float fRec16 = fTemp14;
			float fTemp15 = fRec10[1] + fConst6 * (fRec16 - fRec11[1]);
			fRec10[0] = fConst8 * fTemp15 - fRec10[1];
			float fTemp16 = fRec11[1] + fConst9 * fTemp15;
			fRec11[0] = 2.0f * fTemp16 - fRec11[1];
			float fRec12 = fTemp16;
			output1[i0] = FAUSTFLOAT(fRec12);
			float fTemp17 = fRec16 + fTemp10;
			float fTemp18 = fRec15 + fTemp11;
			float fTemp19 = 1.4142135f * fTemp18 + fTemp17;
			float fTemp20 = fRec21[1] + fConst6 * (fTemp0 - (fTemp19 + fRec22[1]));
			fRec21[0] = fConst8 * fTemp20 - fRec21[1];
			float fTemp21 = fRec22[1] + fConst9 * fTemp20;
			fRec22[0] = 2.0f * fTemp21 - fRec22[1];
			float fRec23 = fConst10 * fTemp20;
			float fRec24 = fTemp21;
			output2[i0] = FAUSTFLOAT(fTemp0 - (1.4142135f * (fRec23 + fTemp18) + fRec24 + fTemp17));
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
		}
	}

};

#endif
