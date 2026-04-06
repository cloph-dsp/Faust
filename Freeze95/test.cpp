/* ------------------------------------------------------------
name: "Freeze95"
version: "1.0"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -a FaustCompat.h -lang cpp -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __Freeze95DSP_H__
#define  __Freeze95DSP_H__

#pragma once

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct Meta {
  virtual ~Meta() {}
  virtual void declare(const char* key, const char* value) = 0;
};

class UI {
public:
  virtual ~UI() {}

  virtual void openTabBox(const char* label) {}
  virtual void openHorizontalBox(const char* label) {}
  virtual void openVerticalBox(const char* label) {}
  virtual void closeBox() {}

  virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
  virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
  virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}

  virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}

  virtual void declare(FAUSTFLOAT* zone, const char* key, const char* value) {}
};

class dsp {
public:
  virtual ~dsp() {}

  virtual int getNumInputs() = 0;
  virtual int getNumOutputs() = 0;

  virtual void buildUserInterface(UI* ui_interface) = 0;
  virtual int getSampleRate() = 0;

  virtual void init(int sample_rate) = 0;
  virtual void instanceInit(int sample_rate) = 0;
  virtual void instanceConstants(int sample_rate) = 0;
  virtual void instanceResetUserInterface() = 0;
  virtual void instanceClear() = 0;

  virtual dsp* clone() = 0;
  virtual void metadata(Meta* m) = 0;

  virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
};
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

class Freeze95DSP : public dsp {
	
 private:
	
	int IOTA0;
	float fVec0[1048576];
	int iVec1[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1[2];
	float fConst3;
	int iRec3[2];
	FAUSTFLOAT fHslider1;
	float fVec2[2];
	float fRec4[2];
	float fConst4;
	int iRec6[2];
	int iVec3[2];
	int iRec7[2];
	float fRec5[2];
	float fConst5;
	int iRec8[2];
	int iRec10[2];
	float fRec9[2];
	float fConst6;
	int iRec12[2];
	float fRec11[2];
	int iConst7;
	float fConst8;
	FAUSTFLOAT fCheckbox0;
	int iRec15[2];
	float fRec14[2];
	FAUSTFLOAT fHslider2;
	float fRec13[2];
	int iRec16[2];
	float fConst9;
	float fConst10;
	float fRec2[2];
	float fVec4[2];
	float fRec0[2];
	FAUSTFLOAT fCheckbox1;
	float fRec17[2];
	float fVec5[1048576];
	float fRec19[2];
	float fVec6[2];
	float fRec18[2];
	
 public:
	Freeze95DSP() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-a FaustCompat.h -lang cpp -ct 1 -cn Freeze95DSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "Freeze95DSP.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
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
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "Freeze95");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "1.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::exp(-(1.0f / std::max<float>(1.0f, 0.008f * fConst0)));
		fConst2 = 1.0f - fConst1;
		fConst3 = 3.1415927f / fConst0;
		fConst4 = 0.001f * fConst0;
		fConst5 = 0.002f * fConst0;
		fConst6 = float(std::max<int>(16, int(0.0015f * fConst0)));
		iConst7 = int(4.0f * fConst0);
		fConst8 = 1.0f / fConst0;
		fConst9 = float(iConst7);
		fConst10 = 1.0f / fConst9;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.55f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.2e+02f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 1048576; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec3[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec6[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iVec3[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec7[l8] = 0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec5[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec8[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec10[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec12[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iRec15[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec13[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			iRec16[l18] = 0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec2[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fVec4[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec0[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec17[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 1048576; l23 = l23 + 1) {
			fVec5[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec19[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fVec6[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec18[l26] = 0.0f;
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
	
	virtual Freeze95DSP* clone() {
		return new Freeze95DSP();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Freeze95");
		ui_interface->declare(&fCheckbox1, "tooltip", "Power on/off");
		ui_interface->addCheckButton("Bypass", &fCheckbox1);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Chaos", &fHslider1, FAUSTFLOAT(0.55f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Lo-Fi", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fCheckbox0, "tooltip", "Quantize stutter to host BPM");
		ui_interface->addCheckButton("Sync BPM", &fCheckbox0);
		ui_interface->addHorizontalSlider("transport/bpm", &fHslider2, FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(3e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst2 * float(fHslider0);
		float fSlow1 = float(fHslider1);
		float fSlow2 = std::min<float>(1.0f, std::max<float>(0.0f, 1.0f - 8.333333f * fSlow1));
		int iSlow3 = fSlow2 < 0.05f;
		float fSlow4 = fConst2 * fSlow1;
		float fSlow5 = 1.0f - 0.9f * fSlow2;
		float fSlow6 = 0.38f * fSlow5;
		int iSlow7 = float(fCheckbox0) > 0.0f;
		float fSlow8 = std::max<float>(2e+01f, float(fHslider2));
		float fSlow9 = 0.00625f * fSlow8;
		float fSlow10 = 0.0125f * fSlow8;
		float fSlow11 = 0.025f * fSlow8;
		float fSlow12 = 0.041666668f * fSlow8;
		float fSlow13 = 0.058333334f * fSlow8;
		float fSlow14 = 0.1f * fSlow8;
		float fSlow15 = 0.13333334f * fSlow8;
		float fSlow16 = 0.06666667f * fSlow8;
		float fSlow17 = 0.05f * fSlow8;
		float fSlow18 = 0.033333335f * fSlow8;
		float fSlow19 = 0.016666668f * fSlow8;
		float fSlow20 = 0.008333334f * fSlow8;
		float fSlow21 = 0.004166667f * fSlow8;
		float fSlow22 = fConst2 * (1.0f - float(fCheckbox1));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			fVec0[IOTA0 & 1048575] = fTemp0;
			iVec1[0] = 1;
			fRec1[0] = fSlow0 + fConst1 * fRec1[1];
			float fTemp1 = 1.0f / std::tan(fConst3 * std::max<float>(9e+01f, 1.5e+04f - 1.32e+04f * fRec1[0]));
			float fTemp2 = fTemp1 + 1.0f;
			float fTemp3 = std::pow(2.0f, std::max<float>(5.0f, 16.0f - 1e+01f * fRec1[0]));
			iRec3[0] = (iVec1[1] + iRec3[1]) % std::max<int>(1, int(fConst0 / std::max<float>(4.5e+02f, 2.205e+04f - 2.16e+04f * fRec1[0])));
			int iTemp4 = iRec3[0] == 0;
			fVec2[0] = fSlow1;
			fRec4[0] = fSlow4 + fConst1 * fRec4[1];
			float fTemp5 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, 2.3809524f * (fRec4[0] + -0.58f))), 1.8f);
			float fTemp6 = std::pow(fRec4[0], 1.1f);
			iRec6[0] = (iRec6[1] + 1) % std::max<int>(1, int(fConst4 * (3.04e+03f * Freeze95DSP_faustpower2_f(1.0f - fTemp6) + 1.6e+02f)));
			int iTemp7 = std::fabs(fSlow1 - fVec2[1]) > 0.004f;
			iVec3[0] = iTemp7;
			int iTemp8 = int(std::max<float>(float(iRec6[0] == 0), std::max<float>(0.0f, float(iTemp7 - iVec3[1]))));
			iRec7[0] = 1103515245 * iRec7[1] + 555;
			fRec5[0] = ((iTemp8) ? 4.656613e-10f * float((iRec7[0] + 56789) % 2147483647) : fRec5[1]);
			int iTemp9 = std::max<int>(1, int(fConst0 * (0.19f * fTemp5 + 0.03f)));
			float fTemp10 = float(std::max<int>(1, iTemp9));
			float fTemp11 = std::min<float>(0.49f, fConst5 / fTemp10);
			iRec8[0] = (iVec1[1] + iRec8[1]) % iTemp9;
			float fTemp12 = float(iRec8[0]);
			float fTemp13 = fTemp12 / fTemp10;
			float fTemp14 = std::max<float>(1e-06f, fTemp11);
			float fTemp15 = (((fRec5[0] < (fSlow6 * fTemp5)) & iSlow3) ? ((fTemp13 < fTemp11) ? fTemp12 / (fTemp10 * fTemp14) : ((fTemp13 < (0.5f - fTemp11)) ? 0.0f : ((fTemp13 < 0.5f) ? (fTemp13 + fTemp11 + -0.5f) / fTemp14 : 1.0f))) : 1.0f);
			float fTemp16 = std::pow(std::min<float>(1.0f, std::max<float>(0.0f, 1.6129032f * (fRec4[0] + -0.18f))), 1.35f);
			iRec10[0] = 1103515245 * iRec10[1] + 444;
			fRec9[0] = ((iTemp8) ? 4.656613e-10f * float((iRec10[0] + 45678) % 2147483647) : fRec9[1]);
			int iTemp17 = ((fRec9[0] > (0.94f - (0.16f * fTemp16 + 0.2f * fTemp5))) & (fTemp16 > 0.05f)) & iSlow3;
			float fTemp18 = std::min<float>(1.0f, 0.72f * fTemp6 + 0.28f * fTemp16);
			float fTemp19 = 1.0f - fTemp18;
			iRec12[0] = 1103515245 * iRec12[1] + 111;
			fRec11[0] = ((iTemp8) ? 4.656613e-10f * float((iRec12[0] + 12345) % 2147483647) : fRec11[1]);
			int iTemp20 = std::max<int>(1, std::min<int>(int(fConst4 * (137.0f * fTemp19 + Freeze95DSP_faustpower2_f(fRec11[0]) * (138.0f * fTemp19 + 5e+01f) + 5e+01f)), iConst7));
			float fTemp21 = float(std::max<int>(1, iTemp20));
			float fTemp22 = 2.6f * fTemp18;
			iRec15[0] = 1103515245 * iRec15[1] + 333;
			fRec14[0] = ((iTemp8) ? 4.656613e-10f * float((iRec15[0] + 34567) % 2147483647) : fRec14[1]);
			float fTemp23 = (fTemp22 + fRec14[0] * (2e+01f * fTemp6 + 34.0f * fTemp16 + 54.0f * fTemp5 + (9.65f - fTemp22)) + 0.35f) * (1.0f - 0.45f * fTemp19) + 0.3825f * fTemp19;
			float fTemp24 = ((1 - iVec1[1]) ? 0.0f : fRec13[1] + fConst8 * std::min<float>(5e+02f, ((iSlow7) ? ((fTemp23 < fSlow9) ? fSlow21 : ((fTemp23 < fSlow10) ? fSlow20 : ((fTemp23 < fSlow11) ? fSlow19 : ((fTemp23 < fSlow12) ? fSlow18 : ((fTemp23 < fSlow13) ? fSlow17 : ((fTemp23 < fSlow14) ? fSlow16 : fSlow15)))))) : fTemp23) / std::max<float>(fConst8, fConst8 * float(iTemp20))));
			fRec13[0] = fTemp24 - std::floor(fTemp24);
			float fTemp25 = fRec13[0] + 1.0f;
			float fTemp26 = float(int(fSlow5 * (fRec9[0] + -0.5f) * (9.0f * fTemp16 + 8.0f * fTemp5 + 0.35f)));
			float fTemp27 = fTemp25 - std::floor((fTemp26 + fTemp21 * fTemp25) / fTemp21);
			float fTemp28 = fTemp21 * (1.0f - fTemp27);
			float fTemp29 = std::max<float>(0.05f, std::sin(1.5707964f * std::min<float>(1.0f, std::min<float>(fTemp26 + fTemp21 * fTemp27, fTemp28 - fTemp26) / std::max<float>(1.0f, std::min<float>(fConst6, 0.5f * fTemp21)))));
			iRec16[0] = (iVec1[1] + iRec16[1]) % iConst7;
			float fTemp30 = float(iRec16[0]);
			float fTemp31 = fTemp30 + fTemp26;
			float fTemp32 = fTemp31 + fConst9 * (1.0f - std::floor(fConst10 * (fConst9 + (fTemp31 - fTemp28))));
			int iTemp33 = int(std::floor(fTemp32 - fTemp28));
			float fTemp34 = fTemp32 - (fTemp28 + float(iTemp33));
			int iTemp35 = std::min<int>(iConst7, std::max<int>(0, iTemp33));
			float fTemp36 = fVec0[(IOTA0 - iTemp35) & 1048575];
			int iTemp37 = std::min<int>(iConst7, std::max<int>(0, (iTemp33 + 1) % iConst7));
			float fTemp38 = 1.0f - fRec13[0];
			float fTemp39 = fTemp21 * (1.0f - (fTemp38 - std::floor(fTemp38)));
			float fTemp40 = fTemp30 + fConst9 * (1.0f - std::floor(fConst10 * (fConst9 + (fTemp30 - fTemp39))));
			int iTemp41 = int(std::floor(fTemp40 - fTemp39));
			float fTemp42 = fTemp40 - (fTemp39 + float(iTemp41));
			int iTemp43 = std::min<int>(iConst7, std::max<int>(0, iTemp41));
			float fTemp44 = fVec0[(IOTA0 - iTemp43) & 1048575];
			int iTemp45 = std::min<int>(iConst7, std::max<int>(0, (iTemp41 + 1) % iConst7));
			float fTemp46 = ((iTemp17) ? fTemp29 * (fTemp44 + (fVec0[(IOTA0 - iTemp45) & 1048575] - fTemp44) * fTemp42) : (fTemp36 + (fVec0[(IOTA0 - iTemp37) & 1048575] - fTemp36) * fTemp34) * fTemp29) * fTemp15;
			float fTemp47 = 6.0f * fRec1[0] + 1.0f;
			fRec2[0] = ((iTemp4) ? fTemp46 * fTemp47 / (6.0f * fRec1[0] * std::fabs(fTemp46) + 1.0f) : fRec2[1]);
			float fTemp48 = std::floor(std::max<float>(-1.0f, std::min<float>(1.0f, fRec2[0])) * fTemp3 + 0.5f) / fTemp3;
			fVec4[0] = fTemp48;
			float fTemp49 = 1.0f - fTemp1;
			fRec0[0] = -((fRec0[1] * fTemp49 - (fTemp48 + fVec4[1])) / fTemp2);
			float fTemp50 = Freeze95DSP_faustpower2_f(fRec0[0]);
			fRec17[0] = fSlow22 + fConst1 * fRec17[1];
			output0[i0] = FAUSTFLOAT(fTemp0 + fRec17[0] * (fRec0[0] * (fTemp50 + 27.0f) / (9.0f * fTemp50 + 27.0f) - fTemp0));
			float fTemp51 = float(input1[i0]);
			fVec5[IOTA0 & 1048575] = fTemp51;
			float fTemp52 = fVec5[(IOTA0 - iTemp35) & 1048575];
			float fTemp53 = fVec5[(IOTA0 - iTemp43) & 1048575];
			float fTemp54 = ((iTemp17) ? fTemp29 * (fTemp53 + fTemp42 * (fVec5[(IOTA0 - iTemp45) & 1048575] - fTemp53)) : fTemp29 * (fTemp52 + fTemp34 * (fVec5[(IOTA0 - iTemp37) & 1048575] - fTemp52)));
			fRec19[0] = ((iTemp4) ? fTemp15 * fTemp47 * fTemp54 / (6.0f * fRec1[0] * std::fabs(fTemp15 * fTemp54) + 1.0f) : fRec19[1]);
			float fTemp55 = std::floor(std::max<float>(-1.0f, std::min<float>(1.0f, fRec19[0])) * fTemp3 + 0.5f) / fTemp3;
			fVec6[0] = fTemp55;
			fRec18[0] = -((fTemp49 * fRec18[1] - (fTemp55 + fVec6[1])) / fTemp2);
			float fTemp56 = Freeze95DSP_faustpower2_f(fRec18[0]);
			output1[i0] = FAUSTFLOAT(fTemp51 + fRec17[0] * (fRec18[0] * (fTemp56 + 27.0f) / (9.0f * fTemp56 + 27.0f) - fTemp51));
			IOTA0 = IOTA0 + 1;
			iVec1[1] = iVec1[0];
			fRec1[1] = fRec1[0];
			iRec3[1] = iRec3[0];
			fVec2[1] = fVec2[0];
			fRec4[1] = fRec4[0];
			iRec6[1] = iRec6[0];
			iVec3[1] = iVec3[0];
			iRec7[1] = iRec7[0];
			fRec5[1] = fRec5[0];
			iRec8[1] = iRec8[0];
			iRec10[1] = iRec10[0];
			fRec9[1] = fRec9[0];
			iRec12[1] = iRec12[0];
			fRec11[1] = fRec11[0];
			iRec15[1] = iRec15[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			iRec16[1] = iRec16[0];
			fRec2[1] = fRec2[0];
			fVec4[1] = fVec4[0];
			fRec0[1] = fRec0[0];
			fRec17[1] = fRec17[0];
			fRec19[1] = fRec19[0];
			fVec6[1] = fVec6[0];
			fRec18[1] = fRec18[0];
		}
	}

};

#endif
