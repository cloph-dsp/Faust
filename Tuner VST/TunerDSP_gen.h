/* ------------------------------------------------------------
author: "CLOPH"
license: "MIT"
name: "TunerDSP"
version: "1.0"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn TunerDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2
------------------------------------------------------------ */

#ifndef  __TunerDSP_H__
#define  __TunerDSP_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>

#ifndef FAUSTCLASS 
#define FAUSTCLASS TunerDSP
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


class TunerDSP final : public dsp {
	
 private:
	
	int fSampleRate;
	
 public:
	TunerDSP() {
	}
	
	TunerDSP(const TunerDSP&) = default;
	
	virtual ~TunerDSP() = default;
	
	TunerDSP& operator=(const TunerDSP&) = default;
	
	void metadata(Meta* m) { 
		m->declare("author", "CLOPH");
		m->declare("compile_options", "-lang cpp -i -fpga-mem-th 4 -light -nvi -ec -ct 1 -cn TunerDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2");
		m->declare("description", "Transparent stereo pass-through for iPlug2 VST3 tuner host");
		m->declare("filename", "TunerDSP.dsp");
		m->declare("license", "MIT");
		m->declare("name", "TunerDSP");
		m->declare("version", "1.0");
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
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
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
	
	TunerDSP* clone() {
		return new TunerDSP(*this);
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("TunerDSP");
		ui_interface->closeBox();
	}
	
	void control() {
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			output0[i0] = static_cast<FAUSTFLOAT>(static_cast<float>(input0[i0]));
			output1[i0] = static_cast<FAUSTFLOAT>(static_cast<float>(input1[i0]));
		}
	}

};

#endif
