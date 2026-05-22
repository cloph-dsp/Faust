/* ------------------------------------------------------------
author: "CLOPH"
name: "DustStop"
version: "5.1"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
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

class mydspSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec2[2];
	int fSampleRate;
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec2[l3] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec2[0] = (iVec1[1] + iRec2[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * static_cast<float>(iRec2[0]));
			iVec1[1] = iVec1[0];
			iRec2[1] = iRec2[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

class mydspSIG1 {
	
  private:
	
	int fSampleRate;
	
  public:
	
	int getNumInputsmydspSIG1() {
		return 0;
	}
	int getNumOutputsmydspSIG1() {
		return 1;
	}
	
	void instanceInitmydspSIG1(int sample_rate) {
		fSampleRate = sample_rate;
	}
	
	void fillmydspSIG1(int count, float* table) {
		for (int i2 = 0; i2 < count; i2 = i2 + 1) {
			table[i2] = 0.0f;
		}
	}

};

static mydspSIG1* newmydspSIG1() { return (mydspSIG1*)new mydspSIG1(); }
static void deletemydspSIG1(mydspSIG1* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

class mydsp : public dsp {
	
 private:
	
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fRec1[2];
	float fConst3;
	float fRec3[2];
	FAUSTFLOAT fCheckbox0;
	float fVec2[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst4;
	int iConst5;
	int iRec5[2];
	float fRec4[2];
	float fConst6;
	int iRec7[2];
	float fRec6[2];
	FAUSTFLOAT fHslider3;
	float fRec8[2];
	float ftbl1[262144];
	int iRec10[2];
	int IOTA0;
	float fVec3[128];
	int iRec12[2];
	float fConst7;
	float fRec11[2];
	float fRec9[2];
	float fVec4[2];
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec0[2];
	float fVec5[128];
	float ftbl2[262144];
	float fRec14[2];
	float fVec6[2];
	float fRec13[2];
	
 public:
	mydsp() {
	}
	
	mydsp(const mydsp&) = default;
	
	virtual ~mydsp() = default;
	
	mydsp& operator=(const mydsp&) = default;
	
	void metadata(Meta* m) { 
		m->declare("author", "CLOPH");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("description", "Tape stop with granular chime disintegration - sound slows to a stop then shatters into bell-like chime particles.");
		m->declare("filename", "DustStop.dsp");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "DustStop");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.7.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "5.1");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 45.0f / fConst0;
		fConst4 = 6e+01f * fConst0;
		iConst5 = static_cast<int>(3.0f * fConst0);
		fConst6 = 1.0f / static_cast<float>(iConst5);
		mydspSIG1* sig1 = newmydspSIG1();
		sig1->instanceInitmydspSIG1(sample_rate);
		sig1->fillmydspSIG1(262144, ftbl1);
		fConst7 = 2.0535662e-08f / fConst0;
		fConst8 = 1.0f / std::tan(94.24778f / fConst0);
		fConst9 = 1.0f - fConst8;
		fConst10 = 1.0f / (fConst8 + 1.0f);
		sig1->instanceInitmydspSIG1(sample_rate);
		sig1->fillmydspSIG1(262144, ftbl2);
		deletemydspSIG1(sig1);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(1.2e+02f);
		fHslider2 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec5[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec7[l8] = 0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec6[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec10[l11] = 0;
		}
		IOTA0 = 0;
		for (int l12 = 0; l12 < 128; l12 = l12 + 1) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec12[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec9[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fVec4[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec0[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 128; l18 = l18 + 1) {
			fVec5[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec14[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fVec6[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec13[l21] = 0.0f;
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
	
	virtual mydsp* clone() {
		return new mydsp(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("DustStop");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Damping", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Dust", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "style", "menu{'1/4':0.25; '1/2':0.5; '1/1':1; '2/1':2; '4/1':4; '8/1':8}");
		ui_interface->addHorizontalSlider("Time", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.25f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fCheckbox0, "tooltip", "Hold=effect active, Release=immediate dry");
		ui_interface->addCheckButton("Trigger", &fCheckbox0);
		ui_interface->addHorizontalSlider("transport/bpm", &fHslider1, FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(3e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst1 * static_cast<float>(fHslider0);
		float fSlow1 = static_cast<float>(fCheckbox0);
		int iSlow2 = fSlow1 < 0.5f;
		int iSlow3 = std::max<int>(1, static_cast<int>(fConst4 * (static_cast<float>(fHslider2) / std::max<float>(2e+01f, static_cast<float>(fHslider1)))));
		int iSlow4 = iConst5 + iSlow3;
		float fSlow5 = static_cast<float>(iSlow3);
		float fSlow6 = 1.0f / fSlow5;
		int iSlow7 = fSlow1 > 0.5f;
		float fSlow8 = fConst1 * static_cast<float>(fHslider3);
		float fSlow9 = 0.6666667f * fSlow5;
		int iSlow10 = static_cast<int>(fSlow9);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec1[0] = fSlow0 + fConst2 * fRec1[1];
			float fTemp0 = ((1 - iVec0[1]) ? 0.0f : fConst3 + fRec3[1]);
			fRec3[0] = fTemp0 - std::floor(fTemp0);
			fVec2[0] = fSlow1;
			iRec5[0] = (iVec0[1] + iRec5[1]) % 4194304;
			float fTemp1 = static_cast<float>(iRec5[0]);
			fRec4[0] = ((fSlow1 > fVec2[1]) ? fTemp1 : fRec4[1]);
			float fTemp2 = fTemp1 - fRec4[0];
			int iTemp3 = static_cast<int>(((fTemp2 < 0.0f) ? fTemp1 + (4194304.0f - fRec4[0]) : fTemp2));
			int iTemp4 = ((iSlow2) ? 0 : ((iTemp3 > iSlow4) ? iSlow4 : iTemp3));
			float fTemp5 = std::min<float>(1.0f, std::max<float>(0.0f, fSlow6 * static_cast<float>(iTemp4)));
			float fTemp6 = 0.08f * fTemp5 * ftbl0mydspSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec3[0]), 65535))] * (1.0f - 0.7f * fRec1[0]);
			int iTemp7 = ((iSlow7) ? ((iTemp4 < iSlow4) ? 1.0f : 0.0f) : 0.0f) > 0.0f;
			float fTemp8 = std::min<float>(1.0f, std::max<float>(0.0f, fConst6 * static_cast<float>(iTemp4 - iSlow3)));
			int iTemp9 = fTemp8 > 0.0f;
			iRec7[0] = 1103515245 * iRec7[1] + 333;
			fRec6[0] = 4.6100468e-10f * static_cast<float>((iRec7[0] + 34567) % 2147483647) + 0.01f * fRec6[1];
			fRec8[0] = fSlow8 + fConst2 * fRec8[1];
			float fTemp10 = ((iSlow2) ? 1.0f : ((iTemp7) ? (1.0f - 0.3f * fTemp5) * (1.0f - 0.35f * fRec1[0] * fTemp5) * std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - std::pow(fTemp8, 1.5f))) * ((iTemp9) ? 0.4f * fRec8[0] + 0.6f * fRec6[0] : 1.0f) : 0.0f));
			iRec10[0] = (iVec0[1] + iRec10[1]) % 262144;
			int iTemp11 = std::max<int>(0, std::min<int>(iRec10[0], 262143));
			ftbl1[iTemp11] = static_cast<float>(input0[i0]);
			int iTemp12 = std::max<int>(0, std::min<int>(((iTemp7) ? (iRec10[0] + (262144 - ((iTemp9) ? iSlow10 : std::max<int>(1, static_cast<int>(fSlow9 * std::pow(fTemp5, 1.5f)))))) % 262144 : (iRec10[0] + 262143) % 262144), 262143));
			float fTemp13 = ((iTemp9) ? 0.98f * std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 0.3f * fTemp8)) : 0.0f);
			fVec3[IOTA0 & 127] = fRec9[1] * fTemp13;
			iRec12[0] = 1103515245 * iRec12[1] + 222;
			fRec11[0] = fConst7 * static_cast<float>((iRec12[0] + 23456) % 2147483647) + fConst2 * fRec11[1];
			float fTemp14 = 0.5f * fRec8[0] * fTemp8 * (2.0f * fRec11[0] + -1.0f);
			fRec9[0] = fTemp14 + fVec3[(IOTA0 - 73) & 127] + ftbl1[iTemp12];
			float fTemp15 = fRec9[0] * fTemp10 + fTemp6;
			fVec4[0] = fTemp15;
			fRec0[0] = -(fConst10 * (fConst9 * fRec0[1] - fConst8 * (fTemp15 - fVec4[1])));
			output0[i0] = static_cast<FAUSTFLOAT>(0.95f * fRec0[0]);
			fVec5[IOTA0 & 127] = fTemp13 * fRec14[1];
			ftbl2[iTemp11] = static_cast<float>(input1[i0]);
			fRec14[0] = ftbl2[iTemp12] + fTemp14 + fVec5[(IOTA0 - 73) & 127];
			float fTemp16 = fTemp6 + fRec14[0] * fTemp10;
			fVec6[0] = fTemp16;
			fRec13[0] = -(fConst10 * (fConst9 * fRec13[1] - fConst8 * (fTemp16 - fVec6[1])));
			output1[i0] = static_cast<FAUSTFLOAT>(0.95f * fRec13[0]);
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fVec2[1] = fVec2[0];
			iRec5[1] = iRec5[0];
			fRec4[1] = fRec4[0];
			iRec7[1] = iRec7[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			iRec10[1] = iRec10[0];
			IOTA0 = IOTA0 + 1;
			iRec12[1] = iRec12[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fVec4[1] = fVec4[0];
			fRec0[1] = fRec0[0];
			fRec14[1] = fRec14[0];
			fVec6[1] = fVec6[0];
			fRec13[1] = fRec13[0];
		}
	}

};

#endif
