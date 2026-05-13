/* ------------------------------------------------------------
name: "L-R8"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn L8FaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2
------------------------------------------------------------ */

#ifndef  __L8FaustDSP_H__
#define  __L8FaustDSP_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS L8FaustDSP
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

static float L8FaustDSP_faustpower2_f(float value) {
	return value * value;
}

class L8FaustDSP final : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int iSlow0;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow1;
	float fRec0[2];
	float fConst3;
	FAUSTFLOAT fCheckbox1;
	int iSlow2;
	FAUSTFLOAT fHslider1;
	float fSlow3;
	float fRec3[2];
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	int iSlow4;
	float fSlow5;
	float fSlow6;
	float fConst4;
	float fRec2[2];
	FAUSTFLOAT fHslider4;
	float fSlow7;
	float fRec4[2];
	FAUSTFLOAT fHslider5;
	float fSlow8;
	float fRec5[2];
	float fRec6[3];
	int IOTA0;
	float fVec1[2048];
	float fSlow9;
	float fConst5;
	float fRec7[3];
	float fVec2[2048];
	float fRec1[3];
	float fRec8[3];
	
 public:
	L8FaustDSP() {
	}
	
	L8FaustDSP(const L8FaustDSP&) = default;
	
	virtual ~L8FaustDSP() = default;
	
	L8FaustDSP& operator=(const L8FaustDSP&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn L8FaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "L-R8.dsp");
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
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "L-R8");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/lf_triangle:author", "Bart Brouns");
		m->declare("oscillators.lib/lf_triangle:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw1:author", "Bart Brouns");
		m->declare("oscillators.lib/saw1:licence", "STK-4.3");
		m->declare("oscillators.lib/version", "1.7.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
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
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 3.1415927f / fConst0;
		fConst4 = 1.0f / fConst0;
		fConst5 = 0.001f * fConst0;
	}
	
	void instanceResetUserInterface() {
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider0 = static_cast<FAUSTFLOAT>(8e+03f);
		fCheckbox1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(1.2e+02f);
		fHslider3 = static_cast<FAUSTFLOAT>(7.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(1e+03f);
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec4[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec6[l6] = 0.0f;
		}
		IOTA0 = 0;
		for (int l7 = 0; l7 < 2048; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2048; l9 = l9 + 1) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec1[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec8[l11] = 0.0f;
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
	
	L8FaustDSP* clone() {
		return new L8FaustDSP(*this);
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("L-R8");
		ui_interface->declare(0, "0", "");
		ui_interface->openHorizontalBox("Main");
		ui_interface->declare(&fCheckbox0, "0", "");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->declare(0, "1", "");
		ui_interface->openHorizontalBox("Timing");
		ui_interface->declare(&fCheckbox1, "0", "");
		ui_interface->addCheckButton("Sync", &fCheckbox1);
		ui_interface->declare(&fHslider3, "1", "");
		ui_interface->addHorizontalSlider("Rate", &fHslider3, FAUSTFLOAT(7.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(14.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider2, "2", "");
		ui_interface->addHorizontalSlider("BPM", &fHslider2, FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(1.0f), FAUSTFLOAT(3e+02f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider1, "3", "");
		ui_interface->addHorizontalSlider("Free Rate", &fHslider1, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider4, "4", "");
		ui_interface->addHorizontalSlider("Cross", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->declare(0, "2", "");
		ui_interface->openHorizontalBox("Frequency");
		ui_interface->declare(&fHslider5, "0", "");
		ui_interface->addHorizontalSlider("High Pass", &fHslider5, FAUSTFLOAT(1e+03f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider0, "1", "");
		ui_interface->addHorizontalSlider("Low Pass", &fHslider0, FAUSTFLOAT(8e+03f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	void control() {
		iSlow0 = static_cast<int>(static_cast<float>(fCheckbox0));
		fSlow1 = fConst1 * static_cast<float>(fHslider0);
		iSlow2 = static_cast<int>(static_cast<float>(fCheckbox1));
		fSlow3 = fConst1 * static_cast<float>(fHslider1);
		iSlow4 = static_cast<int>(static_cast<float>(fHslider3));
		fSlow5 = ((iSlow4 >= 8) ? static_cast<float>(((iSlow4 >= 12) ? ((iSlow4 >= 14) ? 256 : ((iSlow4 >= 13) ? 128 : 64)) : ((iSlow4 >= 10) ? ((iSlow4 >= 11) ? 32 : 16) : ((iSlow4 >= 9) ? 8 : 4)))) : ((iSlow4 >= 4) ? ((iSlow4 >= 6) ? static_cast<float>(((iSlow4 >= 7) ? 2 : 1)) : ((iSlow4 >= 5) ? 0.5f : 0.25f)) : ((iSlow4 >= 2) ? ((iSlow4 >= 3) ? 0.125f : 0.0625f) : ((iSlow4 >= 1) ? 0.03125f : 0.015625f)))) * ((iSlow2) ? static_cast<float>(fHslider2) : 1.2e+02f);
		fSlow6 = 0.016666668f * fSlow5;
		fSlow7 = fConst1 * static_cast<float>(fHslider4);
		fSlow8 = fConst1 * static_cast<float>(fHslider5);
		fSlow9 = 6e+04f / fSlow5;
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			float fTemp0 = fSlow1 + fConst2 * fRec0[1];
			fRec0[0] = ((*reinterpret_cast<int*>(&fTemp0) & 2139095040) ? fTemp0 : 0.0f);
			float fTemp1 = std::tan(fConst3 * fRec0[0]);
			float fTemp2 = 1.0f / fTemp1;
			float fTemp3 = (fTemp2 + 1.4142135f) / fTemp1 + 1.0f;
			float fTemp4 = L8FaustDSP_faustpower2_f(fTemp1);
			float fTemp5 = 1.0f - 1.0f / fTemp4;
			float fTemp6 = (fTemp2 + -1.4142135f) / fTemp1 + 1.0f;
			float fTemp7 = fSlow3 + fConst2 * fRec3[1];
			fRec3[0] = ((*reinterpret_cast<int*>(&fTemp7) & 2139095040) ? fTemp7 : 0.0f);
			float fTemp8 = ((1 - iVec0[1]) ? 0.0f : fRec2[1] + fConst4 * ((iSlow2) ? fSlow6 : fRec3[0]));
			float fTemp9 = fTemp8 - std::floor(fTemp8);
			fRec2[0] = ((*reinterpret_cast<int*>(&fTemp9) & 2139095040) ? fTemp9 : 0.0f);
			float fTemp10 = 2.0f * (1.0f - std::fabs(2.0f * fRec2[0] + -1.0f)) + -1.0f;
			float fTemp11 = fSlow7 + fConst2 * fRec4[1];
			fRec4[0] = ((*reinterpret_cast<int*>(&fTemp11) & 2139095040) ? fTemp11 : 0.0f);
			int iTemp12 = ((fTemp10 >= 0.0f) ? -1 : 1);
			float fTemp13 = 1.0f - fRec4[0];
			float fTemp14 = fTemp13 * static_cast<float>(iTemp12) + fRec4[0] * fTemp10;
			float fTemp15 = fTemp14 + 1.0f;
			float fTemp16 = fSlow8 + fConst2 * fRec5[1];
			fRec5[0] = ((*reinterpret_cast<int*>(&fTemp16) & 2139095040) ? fTemp16 : 0.0f);
			float fTemp17 = std::tan(fConst3 * fRec5[0]);
			float fTemp18 = 1.0f / fTemp17;
			float fTemp19 = (fTemp18 + 1.4142135f) / fTemp17 + 1.0f;
			float fTemp20 = L8FaustDSP_faustpower2_f(fTemp17);
			float fTemp21 = fTemp20 * fTemp19;
			float fTemp22 = 1.0f - 1.0f / fTemp20;
			float fTemp23 = (fTemp18 + -1.4142135f) / fTemp17 + 1.0f;
			float fTemp24 = static_cast<float>(input0[i0]);
			float fTemp25 = fTemp24 - (fRec6[2] * fTemp23 + 2.0f * fRec6[1] * fTemp22) / fTemp19;
			fRec6[0] = ((*reinterpret_cast<int*>(&fTemp25) & 2139095040) ? fTemp25 : 0.0f);
			float fTemp26 = 2.0f * fRec6[1];
			fVec1[IOTA0 & 2047] = (fRec6[2] + (fRec6[0] - fTemp26)) / fTemp21;
			float fTemp27 = ((iSlow2) ? fSlow9 : 1e+03f / fRec3[0]);
			float fTemp28 = fVec1[(IOTA0 - std::min<int>(2048, std::max<int>(0, std::min<int>(2000, std::max<int>(1, static_cast<int>(fConst5 * ((iSlow2) ? 0.05f * fTemp27 : 2e+01f))))))) & 2047];
			float fTemp29 = 1.0f - fTemp14;
			float fTemp30 = static_cast<float>(input1[i0]);
			float fTemp31 = fTemp30 - (fTemp23 * fRec7[2] + 2.0f * fTemp22 * fRec7[1]) / fTemp19;
			fRec7[0] = ((*reinterpret_cast<int*>(&fTemp31) & 2139095040) ? fTemp31 : 0.0f);
			float fTemp32 = 2.0f * fRec7[1];
			fVec2[IOTA0 & 2047] = (fRec7[2] + (fRec7[0] - fTemp32)) / fTemp21;
			float fTemp33 = fVec2[(IOTA0 - std::min<int>(2048, std::max<int>(0, std::min<int>(2000, std::max<int>(1, static_cast<int>(fConst5 * ((iSlow2) ? 0.1f * fTemp27 : 4e+01f))))))) & 2047];
			int iTemp34 = iTemp12 == 1;
			float fTemp35 = fTemp13 * ((iTemp34) ? fTemp33 : fTemp28) + 0.5f * fRec4[0] * (fTemp33 * fTemp29 + fTemp28 * fTemp15) - (fTemp6 * fRec1[2] + 2.0f * fTemp5 * fRec1[1]) / fTemp3;
			fRec1[0] = ((*reinterpret_cast<int*>(&fTemp35) & 2139095040) ? fTemp35 : 0.0f);
			float fTemp36 = 2.0f * fRec1[1];
			float fTemp37 = fRec1[2] + (fRec1[0] - fTemp36);
			float fTemp38 = std::min<float>(1.0f, 1.3f * std::max<float>(0.0f, -fTemp14));
			float fTemp39 = fTemp13 * ((iTemp34) ? fTemp28 : fTemp33) + 0.5f * fRec4[0] * (fTemp28 * fTemp29 + fTemp33 * fTemp15) - (fRec8[2] * fTemp6 + 2.0f * fRec8[1] * fTemp5) / fTemp3;
			fRec8[0] = ((*reinterpret_cast<int*>(&fTemp39) & 2139095040) ? fTemp39 : 0.0f);
			float fTemp40 = 2.0f * fRec8[1];
			float fTemp41 = fRec8[2] + (fRec8[0] - fTemp40);
			output0[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp24 : (fRec6[2] + fRec6[0] + fTemp26) / fTemp19 + (fRec8[2] + fRec8[0] + fTemp40 + (fTemp41 * (1.0f - fTemp38) + fTemp38 * fTemp37) / fTemp4) / fTemp3));
			float fTemp42 = std::min<float>(1.0f, 1.3f * std::max<float>(0.0f, fTemp14));
			output1[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp30 : (fRec7[2] + fRec7[0] + fTemp32) / fTemp19 + (fRec1[2] + fRec1[0] + fTemp36 + (fTemp37 * (1.0f - fTemp42) + fTemp41 * fTemp42) / fTemp4) / fTemp3));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			IOTA0 = IOTA0 + 1;
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
		}
	}

};

#endif
