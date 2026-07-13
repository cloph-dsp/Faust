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

/* link with : "" */
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
	double fConst3;
	int iConst4;
	double fRec2[2];
	double fConst5;
	int iConst6;
	FAUSTFLOAT fHslider1;
	double fRec4[2];
	double fConst7;
	FAUSTFLOAT fHslider2;
	double fRec8[2];
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
	double fConst46;
	double fConst47;
	double fConst48;
	double fConst49;
	double fConst50;
	double fConst51;
	double fConst52;
	double fConst53;
	double fConst54;
	double fConst55;
	double fConst56;
	double fConst57;
	double fConst58;
	double fConst59;
	double fConst60;
	double fConst61;
	double fConst62;
	double fConst63;
	double fConst64;
	double fConst65;
	double fConst66;
	double fConst67;
	double fConst68;
	double fConst69;
	double fConst70;
	double fConst71;
	double fConst72;
	double fConst73;
	double fConst74;
	double fConst75;
	double fConst76;
	double fConst77;
	double fVec0[2];
	double fRec55[2];
	double fRec54[3];
	double fVec1[2];
	double fConst78;
	double fConst79;
	double fConst80;
	double fRec53[2];
	double fConst81;
	double fConst82;
	double fRec52[2];
	double fConst83;
	double fRec51[2];
	double fRec50[3];
	double fConst84;
	double fConst85;
	double fConst86;
	double fRec49[2];
	double fConst87;
	double fRec48[2];
	double fVec2[3];
	double fVec3[2];
	double fConst88;
	double fConst89;
	double fRec47[2];
	double fConst90;
	double fRec46[2];
	double fVec4[3];
	double fVec5[2];
	double fRec45[2];
	double fRec44[3];
	double fVec6[2];
	double fRec43[2];
	double fRec42[2];
	double fRec41[2];
	double fVec7[3];
	double fVec8[2];
	double fRec40[2];
	double fRec39[3];
	double fRec38[3];
	double fConst91;
	double fVec9[2];
	double fRec37[2];
	double fRec36[2];
	double fRec35[2];
	double fVec10[2];
	double fConst92;
	double fConst93;
	double fRec34[2];
	double fRec33[3];
	double fRec57[2];
	double fRec56[3];
	double fVec11[2];
	double fConst94;
	double fConst95;
	double fConst96;
	double fRec32[2];
	double fRec31[3];
	double fRec59[2];
	double fRec58[3];
	double fVec12[3];
	double fConst97;
	double fVec13[3];
	double fVec14[3];
	double fVec15[3];
	double fVec16[2];
	double fRec30[2];
	double fRec29[3];
	double fRec61[2];
	double fRec60[3];
	double fVec17[2];
	double fRec28[2];
	double fRec27[3];
	double fRec63[2];
	double fRec62[3];
	double fRec26[3];
	double fVec18[2];
	double fRec25[2];
	double fConst98;
	double fConst99;
	double fRec24[2];
	double fRec23[3];
	FAUSTFLOAT fHslider3;
	double fRec64[2];
	double fRec66[2];
	double fRec65[3];
	double fVec19[2];
	double fConst100;
	double fConst101;
	double fConst102;
	double fRec22[2];
	double fRec21[3];
	double fRec68[2];
	double fRec67[3];
	double fRec20[3];
	double fRec19[3];
	double fRec18[3];
	double fRec17[3];
	double fRec16[3];
	double fRec15[3];
	double fVec20[2];
	double fConst103;
	double fConst104;
	double fRec14[2];
	double fRec13[2];
	double fRec12[3];
	double fRec70[2];
	double fRec69[3];
	double fRec11[3];
	double fRec10[3];
	double fRec9[3];
	double fRec7[3];
	FAUSTFLOAT fHslider4;
	double fRec71[2];
	double fRec6[2];
	double fRec5[2];
	double fVec21[3];
	int IOTA0;
	double fVec22[1024];
	double fConst105;
	int iConst106;
	int iRec74[2];
	double fRec75[2];
	double fRec73[2];
	double fRec72[2];
	double fVec23[2];
	int iConst107;
	double fVec24[3];
	int iConst108;
	double fVec25[5];
	int iConst109;
	int iConst110;
	double fVec26[12];
	int iConst111;
	int iConst112;
	double fVec27[32];
	int iConst113;
	int iConst114;
	double fVec28[64];
	int iConst115;
	int iConst116;
	double fVec29[128];
	int iConst117;
	int iConst118;
	double fVec30[256];
	int iConst119;
	int iConst120;
	double fVec31[512];
	int iConst121;
	int iConst122;
	double fVec32[1024];
	int iConst123;
	int iConst124;
	double fVec33[2048];
	int iConst125;
	double fRec3[2];
	double fVec34[2048];
	int iConst126;
	int iRec78[2];
	double fRec79[2];
	double fRec77[2];
	double fRec76[2];
	double fVec35[2];
	double fVec36[3];
	double fVec37[5];
	int iConst127;
	double fVec38[12];
	int iConst128;
	double fVec39[32];
	int iConst129;
	double fVec40[64];
	int iConst130;
	double fVec41[128];
	int iConst131;
	double fVec42[256];
	int iConst132;
	double fVec43[512];
	int iConst133;
	double fVec44[1024];
	int iConst134;
	double fVec45[2048];
	int iConst135;
	double fConst136;
	double fRec1[2];
	double fVec46[2];
	double fRec131[2];
	double fRec130[3];
	double fVec47[2];
	double fRec129[2];
	double fRec128[2];
	double fRec127[2];
	double fRec126[3];
	double fRec125[2];
	double fRec124[2];
	double fVec48[3];
	double fVec49[2];
	double fRec123[2];
	double fRec122[2];
	double fVec50[3];
	double fVec51[2];
	double fRec121[2];
	double fRec120[3];
	double fVec52[2];
	double fRec119[2];
	double fRec118[2];
	double fRec117[2];
	double fVec53[3];
	double fVec54[2];
	double fRec116[2];
	double fRec115[3];
	double fRec114[3];
	double fVec55[2];
	double fRec113[2];
	double fRec112[2];
	double fRec111[2];
	double fVec56[2];
	double fRec110[2];
	double fRec109[3];
	double fRec133[2];
	double fRec132[3];
	double fVec57[2];
	double fRec108[2];
	double fRec107[3];
	double fRec135[2];
	double fRec134[3];
	double fVec58[3];
	double fVec59[3];
	double fVec60[3];
	double fVec61[3];
	double fVec62[2];
	double fRec106[2];
	double fRec105[3];
	double fRec137[2];
	double fRec136[3];
	double fVec63[2];
	double fRec104[2];
	double fRec103[3];
	double fRec139[2];
	double fRec138[3];
	double fRec102[3];
	double fVec64[2];
	double fRec101[2];
	double fRec100[2];
	double fRec99[3];
	double fRec141[2];
	double fRec140[3];
	double fVec65[2];
	double fRec98[2];
	double fRec97[3];
	double fRec143[2];
	double fRec142[3];
	double fRec96[3];
	double fRec95[3];
	double fRec94[3];
	double fRec93[3];
	double fRec92[3];
	double fRec91[3];
	double fVec66[2];
	double fRec90[2];
	double fRec89[2];
	double fRec88[3];
	double fRec145[2];
	double fRec144[3];
	double fRec87[3];
	double fRec86[3];
	double fRec85[3];
	double fRec84[3];
	double fRec83[2];
	double fRec82[2];
	double fVec67[3];
	double fVec68[1024];
	int iRec148[2];
	double fRec149[2];
	double fRec147[2];
	double fRec146[2];
	double fVec69[2];
	double fVec70[3];
	double fVec71[5];
	double fVec72[12];
	double fVec73[32];
	double fVec74[64];
	double fVec75[128];
	double fVec76[256];
	double fVec77[512];
	double fVec78[1024];
	double fVec79[2048];
	double fRec81[2];
	double fVec80[2048];
	int iRec152[2];
	double fRec153[2];
	double fRec151[2];
	double fRec150[2];
	double fVec81[2];
	double fVec82[3];
	double fVec83[5];
	double fVec84[12];
	double fVec85[32];
	double fVec86[64];
	double fVec87[128];
	double fVec88[256];
	double fVec89[512];
	double fVec90[1024];
	double fVec91[2048];
	double fRec80[2];
	
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
		m->declare("basics.lib/parallelOp:author", "Bart Brouns");
		m->declare("basics.lib/parallelOp:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelOp:licence", "GPL-3.0");
		m->declare("basics.lib/peakholder:author", "Dario Sanfilippo");
		m->declare("basics.lib/peakholder:copyright", "Copyright (C) 2022 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("basics.lib/peakholder:license", "MIT-style STK-4.3 license");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("compressors.lib/expanderSC_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expanderSC_N_chan:license", "GPLv3");
		m->declare("compressors.lib/expander_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expander_N_chan:license", "GPLv3");
		m->declare("compressors.lib/limiter_lad_N:author", "Dario Sanfilippo");
		m->declare("compressors.lib/limiter_lad_N:copyright", "Copyright (C) 2020 Dario Sanfilippo       <sanfilippo.dario@gmail.com>");
		m->declare("compressors.lib/limiter_lad_N:license", "GPLv3 license");
		m->declare("compressors.lib/limiter_lad_mono:author", "Dario Sanfilippo");
		m->declare("compressors.lib/limiter_lad_mono:copyright", "Copyright (C) 2020 Dario Sanfilippo       <sanfilippo.dario@gmail.com>");
		m->declare("compressors.lib/limiter_lad_mono:license", "GPLv3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
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
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
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
		fConst3 = 0.02 * fConst0;
		iConst4 = static_cast<int>(std::floor(fConst3)) % 2;
		fConst5 = 0.01 * fConst0;
		iConst6 = static_cast<int>(std::floor(fConst5)) % 2;
		fConst7 = 3.141592653589793 / fConst0;
		fConst8 = 9424.77796076938 / (fConst0 * std::sin(28274.33388230814 / fConst0));
		fConst9 = std::tan(14137.16694115407 / fConst0);
		fConst10 = 1.0 / fConst9;
		fConst11 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst9));
		fConst12 = std::tan(94.24777960769379 / fConst0);
		fConst13 = 1.0 / fConst12;
		fConst14 = 1.0 - fConst13;
		fConst15 = 785.3981633974482 / (fConst0 * std::sin(1570.7963267948965 / fConst0));
		fConst16 = std::tan(785.3981633974482 / fConst0);
		fConst17 = 1.0 / fConst16;
		fConst18 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst16));
		fConst19 = fConst0 * std::sin(345.57519189487726 / fConst0);
		fConst20 = 246.83942278205518 / fConst19;
		fConst21 = std::tan(172.78759594743863 / fConst0);
		fConst22 = 1.0 / fConst21;
		fConst23 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst21));
		fConst24 = 913.9178628624852 / (fConst0 * std::sin(2010.6192982974676 / fConst0));
		fConst25 = std::tan(1005.3096491487338 / fConst0);
		fConst26 = 1.0 / fConst25;
		fConst27 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst25));
		fConst28 = 69.11503837897544 / fConst19;
		fConst29 = 6.283185307179586 / fConst0;
		fConst30 = 4.363323129985823 / fConst0;
		fConst31 = 2050.3025739217596 / (fConst0 * std::sin(3895.5748904513434 / fConst0));
		fConst32 = std::tan(1947.7874452256717 / fConst0);
		fConst33 = 1.0 / fConst32;
		fConst34 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst32));
		fConst35 = std::tan(9424.77796076938 / fConst0);
		fConst36 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst35);
		fConst37 = 2.0 * (1.0 - fConst36);
		fConst38 = 1.0 / fConst35;
		fConst39 = (fConst38 + -1.0000000000000004) / fConst35 + 1.0;
		fConst40 = 1.0 / ((fConst38 + 1.0000000000000004) / fConst35 + 1.0);
		fConst41 = std::tan(376.99111843077515 / fConst0);
		fConst42 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst41);
		fConst43 = 2.0 * (1.0 - fConst42);
		fConst44 = 1.0 / fConst41;
		fConst45 = (fConst44 + -1.0000000000000004) / fConst41 + 1.0;
		fConst46 = (fConst44 + 1.0000000000000004) / fConst41 + 1.0;
		fConst47 = 1.0 / fConst46;
		fConst48 = std::tan(13194.68914507713 / fConst0);
		fConst49 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst48);
		fConst50 = 2.0 * (1.0 - fConst49);
		fConst51 = 1.0 / fConst48;
		fConst52 = (fConst51 + -1.0000000000000004) / fConst48 + 1.0;
		fConst53 = (fConst51 + 1.0000000000000004) / fConst48 + 1.0;
		fConst54 = 1.0 / fConst53;
		fConst55 = std::tan(10995.574287564275 / fConst0);
		fConst56 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst55);
		fConst57 = 2.0 * (1.0 - fConst56);
		fConst58 = 1.0 / fConst55;
		fConst59 = (fConst58 + -1.0000000000000004) / fConst55 + 1.0;
		fConst60 = (fConst58 + 1.0000000000000004) / fConst55 + 1.0;
		fConst61 = 1.0 / fConst60;
		fConst62 = std::tan(471.23889803846896 / fConst0);
		fConst63 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst62));
		fConst64 = 1.0 / fConst62;
		fConst65 = (fConst64 + -1.414213562373095) / fConst62 + 1.0;
		fConst66 = (fConst64 + 1.414213562373095) / fConst62 + 1.0;
		fConst67 = 1.0 / fConst66;
		fConst68 = std::tan(109.95574287564276 / fConst0);
		fConst69 = 1.0 / fConst68;
		fConst70 = 1.0 - fConst69;
		fConst71 = std::exp(-(2e+02 / fConst0));
		fConst72 = GrungrFaustDSP_faustpower2_f(fConst68);
		fConst73 = 2.0 * (1.0 - 1.0 / fConst72);
		fConst74 = (fConst69 + -1.414213562373095) / fConst68 + 1.0;
		fConst75 = (fConst69 + 1.414213562373095) / fConst68 + 1.0;
		fConst76 = 1.0 / fConst75;
		fConst77 = std::exp(-(1e+03 / fConst0));
		fConst78 = 1.0 / std::tan(47123.8898038469 / fConst0);
		fConst79 = 1.0 - fConst78;
		fConst80 = 1.0 / (fConst78 + 1.0);
		fConst81 = std::exp(-(2e+01 / fConst0));
		fConst82 = 1.0 - fConst81;
		fConst83 = 1.0 - fConst77;
		fConst84 = 1.0 / (fConst72 * fConst75);
		fConst85 = std::exp(-(12.5 / fConst0));
		fConst86 = 1.0 - fConst85;
		fConst87 = 1.0 - fConst71;
		fConst88 = 1.1459155902616465 / fConst68;
		fConst89 = 1.0 / (fConst69 + 1.0);
		fConst90 = 1e+06 / fConst0;
		fConst91 = 0.18 / fConst66;
		fConst92 = 1.0 - fConst58;
		fConst93 = 1.0 / (fConst58 + 1.0);
		fConst94 = 1.0 / (fConst48 * fConst60);
		fConst95 = 1.0 - fConst51;
		fConst96 = 1.0 / (fConst51 + 1.0);
		fConst97 = 0.3333333333333333 / fConst53;
		fConst98 = 1.0 - fConst44;
		fConst99 = 1.0 / (fConst44 + 1.0);
		fConst100 = 1.0 / (fConst35 * fConst46);
		fConst101 = 1.0 - fConst38;
		fConst102 = 1.0 / (fConst38 + 1.0);
		fConst103 = 0.86 / fConst12;
		fConst104 = 1.0 / (fConst13 + 1.0);
		fConst105 = 0.005 * fConst0;
		iConst106 = static_cast<int>(fConst105);
		iConst107 = static_cast<int>(std::floor(fConst105)) % 2;
		iConst108 = static_cast<int>(std::floor(0.0025 * fConst0)) % 2;
		iConst109 = iConst6 + 2 * iConst107;
		iConst110 = static_cast<int>(std::floor(0.00125 * fConst0)) % 2;
		iConst111 = iConst109 + 4 * iConst108;
		iConst112 = static_cast<int>(std::floor(0.000625 * fConst0)) % 2;
		iConst113 = iConst111 + 8 * iConst110;
		iConst114 = static_cast<int>(std::floor(0.0003125 * fConst0)) % 2;
		iConst115 = iConst113 + 16 * iConst112;
		iConst116 = static_cast<int>(std::floor(0.00015625 * fConst0)) % 2;
		iConst117 = iConst115 + 32 * iConst114;
		iConst118 = static_cast<int>(std::floor(7.8125e-05 * fConst0)) % 2;
		iConst119 = iConst117 + 64 * iConst116;
		iConst120 = static_cast<int>(std::floor(3.90625e-05 * fConst0)) % 2;
		iConst121 = iConst119 + 128 * iConst118;
		iConst122 = static_cast<int>(std::floor(1.953125e-05 * fConst0)) % 2;
		iConst123 = iConst121 + 256 * iConst120;
		iConst124 = static_cast<int>(std::floor(9.765625e-06 * fConst0)) % 2;
		iConst125 = iConst123 + 512 * iConst122;
		iConst126 = static_cast<int>(fConst5);
		iConst127 = iConst4 + 2 * iConst6;
		iConst128 = iConst127 + 4 * iConst107;
		iConst129 = iConst128 + 8 * iConst108;
		iConst130 = iConst129 + 16 * iConst110;
		iConst131 = iConst130 + 32 * iConst112;
		iConst132 = iConst131 + 64 * iConst114;
		iConst133 = iConst132 + 128 * iConst116;
		iConst134 = iConst133 + 256 * iConst118;
		iConst135 = iConst134 + 512 * iConst120;
		fConst136 = std::exp(-(8.333333333333334 / fConst0));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0);
		fHslider0 = static_cast<FAUSTFLOAT>(0.0);
		fHslider1 = static_cast<FAUSTFLOAT>(0.7);
		fHslider2 = static_cast<FAUSTFLOAT>(0.56);
		fHslider3 = static_cast<FAUSTFLOAT>(0.62);
		fHslider4 = static_cast<FAUSTFLOAT>(0.72);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec4[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec8[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec0[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec55[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec54[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec53[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec52[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec51[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec50[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec49[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec48[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fVec2[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fVec3[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec47[l16] = 0.0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec46[l17] = 0.0;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fVec4[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec5[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec45[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec44[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec6[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec43[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec42[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec41[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fVec7[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fVec8[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec40[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec39[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec38[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec9[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec37[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec36[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec35[l34] = 0.0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fVec10[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec34[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec33[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec57[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec56[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fVec11[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec32[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec31[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec59[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec58[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fVec12[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fVec13[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fVec14[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fVec15[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fVec16[l49] = 0.0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec30[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec29[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec61[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec60[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fVec17[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec28[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec27[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec63[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec62[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec26[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fVec18[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec25[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec24[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec23[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec64[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec66[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec65[l66] = 0.0;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fVec19[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec22[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec21[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec68[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec67[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec20[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec19[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec18[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec17[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec16[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec15[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fVec20[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec14[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec13[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 3; l81 = l81 + 1) {
			fRec12[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec70[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec69[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec11[l84] = 0.0;
		}
		for (int l85 = 0; l85 < 3; l85 = l85 + 1) {
			fRec10[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec9[l86] = 0.0;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec7[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec71[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec6[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec5[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fVec21[l91] = 0.0;
		}
		IOTA0 = 0;
		for (int l92 = 0; l92 < 1024; l92 = l92 + 1) {
			fVec22[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			iRec74[l93] = 0;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec75[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec73[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec72[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fVec23[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fVec24[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 5; l99 = l99 + 1) {
			fVec25[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 12; l100 = l100 + 1) {
			fVec26[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 32; l101 = l101 + 1) {
			fVec27[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 64; l102 = l102 + 1) {
			fVec28[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 128; l103 = l103 + 1) {
			fVec29[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 256; l104 = l104 + 1) {
			fVec30[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 512; l105 = l105 + 1) {
			fVec31[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 1024; l106 = l106 + 1) {
			fVec32[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 2048; l107 = l107 + 1) {
			fVec33[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec3[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 2048; l109 = l109 + 1) {
			fVec34[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			iRec78[l110] = 0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec79[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec77[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec76[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fVec35[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 3; l115 = l115 + 1) {
			fVec36[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 5; l116 = l116 + 1) {
			fVec37[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 12; l117 = l117 + 1) {
			fVec38[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 32; l118 = l118 + 1) {
			fVec39[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 64; l119 = l119 + 1) {
			fVec40[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 128; l120 = l120 + 1) {
			fVec41[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 256; l121 = l121 + 1) {
			fVec42[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 512; l122 = l122 + 1) {
			fVec43[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 1024; l123 = l123 + 1) {
			fVec44[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 2048; l124 = l124 + 1) {
			fVec45[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec1[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fVec46[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec131[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 3; l128 = l128 + 1) {
			fRec130[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fVec47[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec129[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec128[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec127[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 3; l133 = l133 + 1) {
			fRec126[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec125[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec124[l135] = 0.0;
		}
		for (int l136 = 0; l136 < 3; l136 = l136 + 1) {
			fVec48[l136] = 0.0;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fVec49[l137] = 0.0;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fRec123[l138] = 0.0;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec122[l139] = 0.0;
		}
		for (int l140 = 0; l140 < 3; l140 = l140 + 1) {
			fVec50[l140] = 0.0;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fVec51[l141] = 0.0;
		}
		for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
			fRec121[l142] = 0.0;
		}
		for (int l143 = 0; l143 < 3; l143 = l143 + 1) {
			fRec120[l143] = 0.0;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fVec52[l144] = 0.0;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec119[l145] = 0.0;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec118[l146] = 0.0;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fRec117[l147] = 0.0;
		}
		for (int l148 = 0; l148 < 3; l148 = l148 + 1) {
			fVec53[l148] = 0.0;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fVec54[l149] = 0.0;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fRec116[l150] = 0.0;
		}
		for (int l151 = 0; l151 < 3; l151 = l151 + 1) {
			fRec115[l151] = 0.0;
		}
		for (int l152 = 0; l152 < 3; l152 = l152 + 1) {
			fRec114[l152] = 0.0;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fVec55[l153] = 0.0;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fRec113[l154] = 0.0;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec112[l155] = 0.0;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec111[l156] = 0.0;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fVec56[l157] = 0.0;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec110[l158] = 0.0;
		}
		for (int l159 = 0; l159 < 3; l159 = l159 + 1) {
			fRec109[l159] = 0.0;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec133[l160] = 0.0;
		}
		for (int l161 = 0; l161 < 3; l161 = l161 + 1) {
			fRec132[l161] = 0.0;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fVec57[l162] = 0.0;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec108[l163] = 0.0;
		}
		for (int l164 = 0; l164 < 3; l164 = l164 + 1) {
			fRec107[l164] = 0.0;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec135[l165] = 0.0;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec134[l166] = 0.0;
		}
		for (int l167 = 0; l167 < 3; l167 = l167 + 1) {
			fVec58[l167] = 0.0;
		}
		for (int l168 = 0; l168 < 3; l168 = l168 + 1) {
			fVec59[l168] = 0.0;
		}
		for (int l169 = 0; l169 < 3; l169 = l169 + 1) {
			fVec60[l169] = 0.0;
		}
		for (int l170 = 0; l170 < 3; l170 = l170 + 1) {
			fVec61[l170] = 0.0;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fVec62[l171] = 0.0;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec106[l172] = 0.0;
		}
		for (int l173 = 0; l173 < 3; l173 = l173 + 1) {
			fRec105[l173] = 0.0;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec137[l174] = 0.0;
		}
		for (int l175 = 0; l175 < 3; l175 = l175 + 1) {
			fRec136[l175] = 0.0;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fVec63[l176] = 0.0;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec104[l177] = 0.0;
		}
		for (int l178 = 0; l178 < 3; l178 = l178 + 1) {
			fRec103[l178] = 0.0;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fRec139[l179] = 0.0;
		}
		for (int l180 = 0; l180 < 3; l180 = l180 + 1) {
			fRec138[l180] = 0.0;
		}
		for (int l181 = 0; l181 < 3; l181 = l181 + 1) {
			fRec102[l181] = 0.0;
		}
		for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
			fVec64[l182] = 0.0;
		}
		for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
			fRec101[l183] = 0.0;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec100[l184] = 0.0;
		}
		for (int l185 = 0; l185 < 3; l185 = l185 + 1) {
			fRec99[l185] = 0.0;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fRec141[l186] = 0.0;
		}
		for (int l187 = 0; l187 < 3; l187 = l187 + 1) {
			fRec140[l187] = 0.0;
		}
		for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
			fVec65[l188] = 0.0;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec98[l189] = 0.0;
		}
		for (int l190 = 0; l190 < 3; l190 = l190 + 1) {
			fRec97[l190] = 0.0;
		}
		for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
			fRec143[l191] = 0.0;
		}
		for (int l192 = 0; l192 < 3; l192 = l192 + 1) {
			fRec142[l192] = 0.0;
		}
		for (int l193 = 0; l193 < 3; l193 = l193 + 1) {
			fRec96[l193] = 0.0;
		}
		for (int l194 = 0; l194 < 3; l194 = l194 + 1) {
			fRec95[l194] = 0.0;
		}
		for (int l195 = 0; l195 < 3; l195 = l195 + 1) {
			fRec94[l195] = 0.0;
		}
		for (int l196 = 0; l196 < 3; l196 = l196 + 1) {
			fRec93[l196] = 0.0;
		}
		for (int l197 = 0; l197 < 3; l197 = l197 + 1) {
			fRec92[l197] = 0.0;
		}
		for (int l198 = 0; l198 < 3; l198 = l198 + 1) {
			fRec91[l198] = 0.0;
		}
		for (int l199 = 0; l199 < 2; l199 = l199 + 1) {
			fVec66[l199] = 0.0;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec90[l200] = 0.0;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec89[l201] = 0.0;
		}
		for (int l202 = 0; l202 < 3; l202 = l202 + 1) {
			fRec88[l202] = 0.0;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fRec145[l203] = 0.0;
		}
		for (int l204 = 0; l204 < 3; l204 = l204 + 1) {
			fRec144[l204] = 0.0;
		}
		for (int l205 = 0; l205 < 3; l205 = l205 + 1) {
			fRec87[l205] = 0.0;
		}
		for (int l206 = 0; l206 < 3; l206 = l206 + 1) {
			fRec86[l206] = 0.0;
		}
		for (int l207 = 0; l207 < 3; l207 = l207 + 1) {
			fRec85[l207] = 0.0;
		}
		for (int l208 = 0; l208 < 3; l208 = l208 + 1) {
			fRec84[l208] = 0.0;
		}
		for (int l209 = 0; l209 < 2; l209 = l209 + 1) {
			fRec83[l209] = 0.0;
		}
		for (int l210 = 0; l210 < 2; l210 = l210 + 1) {
			fRec82[l210] = 0.0;
		}
		for (int l211 = 0; l211 < 3; l211 = l211 + 1) {
			fVec67[l211] = 0.0;
		}
		for (int l212 = 0; l212 < 1024; l212 = l212 + 1) {
			fVec68[l212] = 0.0;
		}
		for (int l213 = 0; l213 < 2; l213 = l213 + 1) {
			iRec148[l213] = 0;
		}
		for (int l214 = 0; l214 < 2; l214 = l214 + 1) {
			fRec149[l214] = 0.0;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fRec147[l215] = 0.0;
		}
		for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
			fRec146[l216] = 0.0;
		}
		for (int l217 = 0; l217 < 2; l217 = l217 + 1) {
			fVec69[l217] = 0.0;
		}
		for (int l218 = 0; l218 < 3; l218 = l218 + 1) {
			fVec70[l218] = 0.0;
		}
		for (int l219 = 0; l219 < 5; l219 = l219 + 1) {
			fVec71[l219] = 0.0;
		}
		for (int l220 = 0; l220 < 12; l220 = l220 + 1) {
			fVec72[l220] = 0.0;
		}
		for (int l221 = 0; l221 < 32; l221 = l221 + 1) {
			fVec73[l221] = 0.0;
		}
		for (int l222 = 0; l222 < 64; l222 = l222 + 1) {
			fVec74[l222] = 0.0;
		}
		for (int l223 = 0; l223 < 128; l223 = l223 + 1) {
			fVec75[l223] = 0.0;
		}
		for (int l224 = 0; l224 < 256; l224 = l224 + 1) {
			fVec76[l224] = 0.0;
		}
		for (int l225 = 0; l225 < 512; l225 = l225 + 1) {
			fVec77[l225] = 0.0;
		}
		for (int l226 = 0; l226 < 1024; l226 = l226 + 1) {
			fVec78[l226] = 0.0;
		}
		for (int l227 = 0; l227 < 2048; l227 = l227 + 1) {
			fVec79[l227] = 0.0;
		}
		for (int l228 = 0; l228 < 2; l228 = l228 + 1) {
			fRec81[l228] = 0.0;
		}
		for (int l229 = 0; l229 < 2048; l229 = l229 + 1) {
			fVec80[l229] = 0.0;
		}
		for (int l230 = 0; l230 < 2; l230 = l230 + 1) {
			iRec152[l230] = 0;
		}
		for (int l231 = 0; l231 < 2; l231 = l231 + 1) {
			fRec153[l231] = 0.0;
		}
		for (int l232 = 0; l232 < 2; l232 = l232 + 1) {
			fRec151[l232] = 0.0;
		}
		for (int l233 = 0; l233 < 2; l233 = l233 + 1) {
			fRec150[l233] = 0.0;
		}
		for (int l234 = 0; l234 < 2; l234 = l234 + 1) {
			fVec81[l234] = 0.0;
		}
		for (int l235 = 0; l235 < 3; l235 = l235 + 1) {
			fVec82[l235] = 0.0;
		}
		for (int l236 = 0; l236 < 5; l236 = l236 + 1) {
			fVec83[l236] = 0.0;
		}
		for (int l237 = 0; l237 < 12; l237 = l237 + 1) {
			fVec84[l237] = 0.0;
		}
		for (int l238 = 0; l238 < 32; l238 = l238 + 1) {
			fVec85[l238] = 0.0;
		}
		for (int l239 = 0; l239 < 64; l239 = l239 + 1) {
			fVec86[l239] = 0.0;
		}
		for (int l240 = 0; l240 < 128; l240 = l240 + 1) {
			fVec87[l240] = 0.0;
		}
		for (int l241 = 0; l241 < 256; l241 = l241 + 1) {
			fVec88[l241] = 0.0;
		}
		for (int l242 = 0; l242 < 512; l242 = l242 + 1) {
			fVec89[l242] = 0.0;
		}
		for (int l243 = 0; l243 < 1024; l243 = l243 + 1) {
			fVec90[l243] = 0.0;
		}
		for (int l244 = 0; l244 < 2048; l244 = l244 + 1) {
			fVec91[l244] = 0.0;
		}
		for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
			fRec80[l245] = 0.0;
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
		ui_interface->declare(&fHslider1, "1", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Grunge", &fHslider1, FAUSTFLOAT(0.7), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider3, "2", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Butt", &fHslider3, FAUSTFLOAT(0.62), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider2, "3", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Face", &fHslider2, FAUSTFLOAT(0.56), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider4, "4", "");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("Loud", &fHslider4, FAUSTFLOAT(0.72), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->declare(&fHslider0, "5", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("RAW Voicing", &fHslider0, FAUSTFLOAT(0.0), FAUSTFLOAT(0.0), FAUSTFLOAT(2.0), FAUSTFLOAT(1.0));
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
		int iSlow1 = std::max<int>(0, std::min<int>(2, static_cast<int>(static_cast<double>(fHslider0) + 0.5)));
		double fSlow2 = fConst1 * static_cast<double>(iSlow1 == 2);
		double fSlow3 = fConst1 * static_cast<double>(iSlow1 == 1);
		double fSlow4 = fConst1 * static_cast<double>(fHslider1);
		double fSlow5 = fConst1 * static_cast<double>(fHslider2);
		double fSlow6 = fConst1 * static_cast<double>(fHslider3);
		double fSlow7 = fConst1 * static_cast<double>(fHslider4);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow2 + fConst2 * fRec0[1];
			int iTemp0 = static_cast<int>(1.0 - fRec0[0]);
			fRec2[0] = fSlow3 + fConst2 * fRec2[1];
			int iTemp1 = static_cast<int>(1.0 - fRec2[0]);
			fRec4[0] = fSlow4 + fConst2 * fRec4[1];
			double fTemp2 = std::min<double>(1.0, std::max<double>(0.0, 3.3333333333333335 * (fRec4[0] + -0.7)));
			double fTemp3 = std::min<double>(1.0, std::max<double>(0.0, 2.2222222222222223 * (fRec4[0] + -0.55)));
			double fTemp4 = fRec2[0] * fTemp3;
			double fTemp5 = std::min<double>(1.0, std::max<double>(0.0, fRec4[0] * (1.32 - 0.32 * fRec4[0])));
			double fTemp6 = 0.09 * fTemp5 + fTemp4 * (0.04 * fTemp2 + 0.12) + 0.64;
			double fTemp7 = fRec0[0] * std::min<double>(1.0, std::max<double>(0.0, 1.6666666666666667 * (fRec4[0] + -0.4)));
			double fTemp8 = 1.0 - (fTemp4 + fTemp7);
			double fTemp9 = std::max<double>(0.06 * fTemp7 + (0.16000000000000003 - 0.03 * fTemp8), 2.220446049250313e-16);
			double fTemp10 = GrungrFaustDSP_faustpower2_f(fTemp5);
			double fTemp11 = fTemp4 * (0.04 * fTemp10 + 0.016) + fTemp7 * (0.05 * fTemp10 + 0.024);
			double fTemp12 = std::min<double>(1.0, std::max<double>(0.0, 5.555555555555555 * (fRec4[0] + -0.82)));
			double fTemp13 = std::min<double>(1.0, std::max<double>(0.0, 2.857142857142857 * (fRec4[0] + -0.65)));
			double fTemp14 = 0.55 * fTemp5;
			double fTemp15 = std::min<double>(1.0, std::max<double>(0.0, 1.5384615384615383 * (fRec4[0] + -0.35)));
			double fTemp16 = 1.0 - fTemp5;
			double fTemp17 = std::min<double>(1.0, std::max<double>(0.0, fTemp5 + fTemp16 * (0.22 * fTemp8 + 0.07) + fTemp4 * (0.1 * fTemp16 + 0.05 * fTemp12) + fTemp7 * (0.08 * fTemp16 + 0.03 * fTemp13)));
			double fTemp18 = std::pow(1e+01, 0.05 * (2.1 * fTemp8 * (fTemp17 - fTemp5) + fRec2[0] * (fTemp15 * (0.45 * fTemp5 + (0.35 - 0.25 * fTemp12)) - 0.55 * fTemp3 * fTemp2) + fTemp7 * (fTemp14 + (0.5 - 0.2 * fTemp13)) + (0.95 - 1.1 * GrungrFaustDSP_faustpower2_f(fTemp12))));
			double fTemp19 = std::tan(fConst7 * (1.8e+04 - 1.2e+04 * fTemp7));
			double fTemp20 = 1.0 / fTemp19;
			double fTemp21 = (fTemp20 + 1.414213562373095) / fTemp19 + 1.0;
			double fTemp22 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp19);
			double fTemp23 = (fTemp20 + -1.414213562373095) / fTemp19 + 1.0;
			fRec8[0] = fSlow5 + fConst2 * fRec8[1];
			double fTemp24 = std::pow(std::max<double>(fRec8[0], 2.220446049250313e-16), 0.88);
			double fTemp25 = fTemp24 + 1.0;
			double fTemp26 = fTemp15 * fTemp25;
			double fTemp27 = 1.46e+04 - 0.5 * fTemp26 * (9e+02 * fTemp5 + 6.5e+02 * fTemp12 + fRec2[0] * (8.5e+02 * fTemp12 + 3.5e+02));
			double fTemp28 = std::tan(fConst7 * fTemp27 * (1.0 - 0.1 * fTemp4));
			double fTemp29 = 1.0 / fTemp28;
			double fTemp30 = (fTemp29 + 1.414213562373095) / fTemp28 + 1.0;
			double fTemp31 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp28);
			double fTemp32 = (fTemp29 + -1.414213562373095) / fTemp28 + 1.0;
			double fTemp33 = std::tan(fConst7 * fTemp27);
			double fTemp34 = 1.0 / fTemp33;
			double fTemp35 = (fTemp34 + 1.414213562373095) / fTemp33 + 1.0;
			double fTemp36 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp33);
			double fTemp37 = (fTemp34 + -1.414213562373095) / fTemp33 + 1.0;
			double fTemp38 = fTemp4 * (0.6 * fTemp12 + 1.8);
			int iTemp39 = -fTemp38 > 0.0;
			double fTemp40 = fConst8 * std::pow(1e+01, 0.05 * std::fabs(-fTemp38));
			double fTemp41 = ((iTemp39) ? fConst8 : fTemp40);
			double fTemp42 = fConst10 * (fConst10 + fTemp41) + 1.0;
			double fTemp43 = ((iTemp39) ? fTemp40 : fConst8);
			double fTemp44 = fConst10 * (fConst10 - fTemp43) + 1.0;
			double fTemp45 = fConst11 * fRec11[1];
			double fTemp46 = fConst10 * (fConst10 - fTemp41) + 1.0;
			double fTemp47 = std::tan(fConst7 * (4.2e+03 - 1.7e+03 * fRec0[0]));
			double fTemp48 = 1.0 / fTemp47;
			double fTemp49 = (fTemp48 + 1.0000000000000004) / fTemp47 + 1.0;
			double fTemp50 = GrungrFaustDSP_faustpower2_f(fTemp47);
			double fTemp51 = 1.8 * fTemp12;
			double fTemp52 = fRec2[0] * fTemp15;
			double fTemp53 = std::pow(1e+01, -(0.025 * fTemp25 * (fTemp52 * (fTemp51 + 0.9) + fTemp7 * (1.4 * fTemp13 + 0.7))));
			double fTemp54 = 1.0 - 1.0 / fTemp50;
			double fTemp55 = (fTemp48 + -1.0000000000000004) / fTemp47 + 1.0;
			double fTemp56 = fTemp48 + 1.0;
			double fTemp57 = 2.0 * fTemp7;
			int iTemp58 = -fTemp57 > 0.0;
			double fTemp59 = fConst15 * std::pow(1e+01, 0.05 * std::fabs(-fTemp57));
			double fTemp60 = ((iTemp58) ? fConst15 : fTemp59);
			double fTemp61 = fConst17 * (fConst17 + fTemp60) + 1.0;
			double fTemp62 = ((iTemp58) ? fTemp59 : fConst15);
			double fTemp63 = fConst17 * (fConst17 - fTemp62) + 1.0;
			double fTemp64 = fConst18 * fRec15[1];
			double fTemp65 = fConst17 * (fConst17 - fTemp60) + 1.0;
			double fTemp66 = 1.5 * fTemp7;
			int iTemp67 = fTemp66 > 0.0;
			double fTemp68 = fConst20 * std::pow(1e+01, 0.05 * std::fabs(fTemp66));
			double fTemp69 = ((iTemp67) ? fConst20 : fTemp68);
			double fTemp70 = fConst22 * (fConst22 + fTemp69) + 1.0;
			double fTemp71 = ((iTemp67) ? fTemp68 : fConst20);
			double fTemp72 = fConst22 * (fConst22 - fTemp71) + 1.0;
			double fTemp73 = fConst23 * fRec16[1];
			double fTemp74 = fConst22 * (fConst22 - fTemp69) + 1.0;
			double fTemp75 = 2.5 * fTemp4;
			int iTemp76 = -fTemp75 > 0.0;
			double fTemp77 = fConst24 * std::pow(1e+01, 0.05 * std::fabs(-fTemp75));
			double fTemp78 = ((iTemp76) ? fConst24 : fTemp77);
			double fTemp79 = fConst26 * (fConst26 + fTemp78) + 1.0;
			double fTemp80 = ((iTemp76) ? fTemp77 : fConst24);
			double fTemp81 = fConst26 * (fConst26 - fTemp80) + 1.0;
			double fTemp82 = fConst27 * fRec17[1];
			double fTemp83 = fConst26 * (fConst26 - fTemp78) + 1.0;
			double fTemp84 = fTemp4 * fTemp2;
			double fTemp85 = 3.0 * fTemp84;
			int iTemp86 = -fTemp85 > 0.0;
			double fTemp87 = fConst28 * std::pow(1e+01, 0.05 * std::fabs(-fTemp85));
			double fTemp88 = ((iTemp86) ? fConst28 : fTemp87);
			double fTemp89 = fConst22 * (fConst22 + fTemp88) + 1.0;
			double fTemp90 = ((iTemp86) ? fTemp87 : fConst28);
			double fTemp91 = fConst22 * (fConst22 - fTemp90) + 1.0;
			double fTemp92 = fConst23 * fRec18[1];
			double fTemp93 = fConst22 * (fConst22 - fTemp88) + 1.0;
			double fTemp94 = 9.5e+02 * fTemp4 + 1.45e+03;
			double fTemp95 = std::tan(fConst7 * fTemp94);
			double fTemp96 = 0.5 * fTemp26 * (0.7 - 0.3 * fTemp5 + fRec2[0] * (0.4 * fTemp16 + 0.2));
			int iTemp97 = fTemp96 > 0.0;
			double fTemp98 = std::sin(fConst29 * fTemp94);
			double fTemp99 = fConst30 * (fTemp94 * std::pow(1e+01, 0.05 * std::fabs(fTemp96)) / fTemp98);
			double fTemp100 = fConst30 * (fTemp94 / fTemp98);
			double fTemp101 = ((iTemp97) ? fTemp100 : fTemp99);
			double fTemp102 = 1.0 / fTemp95;
			double fTemp103 = (fTemp102 + fTemp101) / fTemp95 + 1.0;
			double fTemp104 = ((iTemp97) ? fTemp99 : fTemp100);
			double fTemp105 = (fTemp102 - fTemp104) / fTemp95 + 1.0;
			double fTemp106 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp95);
			double fTemp107 = 2.0 * fRec19[1] * fTemp106;
			double fTemp108 = (fTemp102 - fTemp101) / fTemp95 + 1.0;
			double fTemp109 = fTemp15 * (0.95 - fTemp14) + 3.0 * fTemp7 + -5.6;
			int iTemp110 = fTemp109 > 0.0;
			double fTemp111 = fConst31 * std::pow(1e+01, 0.05 * std::fabs(fTemp109));
			double fTemp112 = ((iTemp110) ? fConst31 : fTemp111);
			double fTemp113 = fConst33 * (fConst33 + fTemp112) + 1.0;
			double fTemp114 = ((iTemp110) ? fTemp111 : fConst31);
			double fTemp115 = fConst33 * (fConst33 - fTemp114) + 1.0;
			double fTemp116 = fConst34 * fRec20[1];
			double fTemp117 = fConst33 * (fConst33 - fTemp112) + 1.0;
			double fTemp118 = std::pow(1e+01, 0.05 * (19.0 * fTemp24 + -8.0));
			double fTemp119 = std::tan(fConst7 * (1.12e+04 - (1.8e+03 * fTemp17 + 9e+02 * fTemp12 + 0.5 * fTemp25 * (fTemp52 * (9.6e+02 * fTemp12 + 4.2e+02) + fTemp7 * (7.2e+02 * fTemp13 + 3.2e+02)))));
			double fTemp120 = 1.0 / fTemp119;
			double fTemp121 = (fTemp120 + 1.414213562373095) / fTemp119 + 1.0;
			double fTemp122 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp119);
			double fTemp123 = (fTemp120 + -1.414213562373095) / fTemp119 + 1.0;
			double fTemp124 = 0.05 * fTemp4 * (0.5 * fTemp16 + 1.3);
			double fTemp125 = std::pow(1e+01, -fTemp124);
			double fTemp126 = std::pow(1e+01, fTemp124);
			double fTemp127 = std::max<double>(0.08 * fTemp7 + (0.24 - 0.04 * fTemp8), 2.220446049250313e-16);
			double fTemp128 = std::pow(1e+01, 0.05 * (6.0 * (fTemp17 + 1.0) + fTemp4 * (fTemp51 + 0.7) + fTemp7 * (1.5 * fTemp13 + 0.5)));
			double fTemp129 = std::tan(fConst7 * (2e+01 * fTemp17 + fTemp4 * (18.0 * fTemp5 + 1e+01) + fTemp7 * (1e+01 * fTemp5 + 6.0) + 1.1e+02));
			double fTemp130 = 1.0 / fTemp129;
			double fTemp131 = fTemp130 + 1.0;
			double fTemp132 = 1.6e+03 * fTemp17 + fTemp4 * (6.2e+02 * fTemp12 + 2.2e+02) + fTemp7 * (4.6e+02 * fTemp13 + 1.6e+02);
			double fTemp133 = std::tan(fConst7 * (8.1e+03 - fTemp132));
			double fTemp134 = 1.0 / fTemp133;
			double fTemp135 = (fTemp134 + 1.414213562373095) / fTemp133 + 1.0;
			double fTemp136 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp133);
			double fTemp137 = (fTemp134 + -1.414213562373095) / fTemp133 + 1.0;
			double fTemp138 = 0.12 * fTemp17;
			double fTemp139 = fTemp4 * (0.015 * fTemp16 + 0.02) + fTemp7 * (0.02 * fTemp16 + 0.025);
			double fTemp140 = 0.85 * fTemp139 + (0.74 - fTemp138);
			double fTemp141 = 1.0 - 0.16 * fTemp4;
			double fTemp142 = 0.1 * fTemp5 + 1.1;
			double fTemp143 = -0.01 - (0.018 * fTemp17 + fTemp4 * (0.006 * fTemp2 + 0.004) + fTemp7 * (0.005 * fTemp13 + 0.003));
			double fTemp144 = std::atan(fTemp143 * fTemp142 * fTemp141 / fTemp140);
			double fTemp145 = fTemp4 * (1.7 * fTemp5 + 0.9) + fTemp7 * (2.0 * fTemp5 + 1.2);
			double fTemp146 = std::pow(1e+01, 0.05 * (14.0 * fTemp17 + (12.0 - 0.65 * fTemp145)));
			double fTemp147 = 1.0 / std::tan(fConst7 * (8.4e+03 - fTemp132));
			double fTemp148 = fTemp147 + 1.0;
			double fTemp149 = 12.0 * fTemp5;
			double fTemp150 = std::tan(fConst7 * (25.0 * fTemp17 + fRec2[0] * (fTemp15 * (24.0 * fTemp5 + 16.0) + 12.0 * fTemp3) + fTemp7 * (fTemp149 + 8.0) + 8e+01));
			double fTemp151 = 1.0 / fTemp150;
			double fTemp152 = fTemp151 + 1.0;
			double fTemp153 = std::tan(fConst7 * (1.08e+04 - (2e+03 * fTemp17 + fTemp4 * (8.2e+02 * fTemp12 + 2.8e+02) + fTemp7 * (6e+02 * fTemp13 + 2e+02))));
			double fTemp154 = 1.0 / fTemp153;
			double fTemp155 = (fTemp154 + 1.414213562373095) / fTemp153 + 1.0;
			double fTemp156 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp153);
			double fTemp157 = (fTemp154 + -1.414213562373095) / fTemp153 + 1.0;
			double fTemp158 = 0.1 * fTemp17;
			double fTemp159 = fTemp139 + (0.76 - fTemp158);
			double fTemp160 = 1.0 - 0.18 * fTemp4;
			double fTemp161 = 0.05 * fTemp5 + 1.0;
			double fTemp162 = std::atan(0.014 * (fTemp161 * fTemp160 / fTemp159));
			double fTemp163 = std::pow(1e+01, 0.05 * (19.0 * fTemp17 + (18.0 - fTemp145)));
			double fTemp164 = 0.07 * fTemp5 + (0.4 - 0.03 * fTemp52);
			double fTemp165 = std::atan(0.005555555555555556 * fTemp164);
			double fTemp166 = std::tan(fConst7 * std::max<double>(2e+01, fRec2[0] * (fTemp149 + 4e+01)));
			double fTemp167 = 1.0 / fTemp166;
			double fTemp168 = (fTemp167 + 1.414213562373095) / fTemp166 + 1.0;
			double fTemp169 = GrungrFaustDSP_faustpower2_f(fTemp166);
			double fTemp170 = fTemp169 * fTemp168;
			double fTemp171 = 1.0 - 1.0 / fTemp169;
			double fTemp172 = (fTemp167 + -1.414213562373095) / fTemp166 + 1.0;
			double fTemp173 = std::tan(fConst7 * (fTemp52 * (3e+01 * fTemp5 + 12.0) + 1.6));
			double fTemp174 = 1.0 / fTemp173;
			double fTemp175 = fTemp174 + 1.0;
			double fTemp176 = 1.0 - fTemp174;
			double fTemp177 = static_cast<double>(input0[i0]);
			double fTemp178 = ((iSlow0) ? 0.0 : fTemp177);
			fVec0[0] = fTemp178;
			fRec55[0] = (0.2 * ((fTemp178 - fVec0[1]) / fTemp173) - fRec55[1] * fTemp176) / fTemp175;
			fRec54[0] = fRec55[0] - (fRec54[2] * fTemp172 + 2.0 * fRec54[1] * fTemp171) / fTemp168;
			double fTemp179 = (fRec54[2] + (fRec54[0] - 2.0 * fRec54[1])) / fTemp170;
			fVec1[0] = fTemp179;
			fRec53[0] = -(fConst80 * (fConst79 * fRec53[1] - (fTemp179 + fVec1[1])));
			double fTemp180 = std::fabs(fRec53[0]);
			fRec52[0] = std::max<double>(fTemp180, fConst81 * fRec52[1] + fConst82 * fTemp180);
			fRec51[0] = fConst83 * fRec52[0] + fConst77 * fRec51[1];
			double fTemp181 = fRec53[0] * (1.0 - 0.7 * fTemp4 * std::min<double>(1.0, std::max<double>(0.0, fRec51[0] + -0.4)));
			fRec50[0] = fTemp181 - fConst76 * (fConst74 * fRec50[2] + fConst73 * fRec50[1]);
			double fTemp182 = std::fabs(fConst84 * (fRec50[2] + (fRec50[0] - 2.0 * fRec50[1])));
			fRec49[0] = std::max<double>(fTemp182, fConst85 * fRec49[1] + fConst86 * fTemp182);
			fRec48[0] = fConst87 * fRec49[0] + fConst71 * fRec48[1];
			double fTemp183 = (fTemp181 * (1.0 - 0.5 * fTemp7 * std::min<double>(1.0, std::max<double>(0.0, fRec48[0] + -0.5))) + 0.01) * fTemp164;
			fVec2[0] = fTemp183;
			double fTemp184 = fVec2[1] - fVec2[2];
			double fTemp185 = 0.5555555555555556 * fTemp184;
			double fTemp186 = 0.5555555555555556 * fVec2[2];
			double fTemp187 = GrungrFaustDSP_faustpower2_f(fTemp186) + 1.0;
			double fTemp188 = std::atan(fTemp186);
			double fTemp189 = 0.5555555555555556 * fVec2[1];
			double fTemp190 = GrungrFaustDSP_faustpower2_f(fTemp189) + 1.0;
			double fTemp191 = std::atan(fTemp189);
			double fTemp192 = fTemp191 * fTemp190;
			double fTemp193 = fVec2[1] * fTemp191;
			double fTemp194 = std::log(fTemp190);
			double fTemp195 = 1.1111111111111112 * fVec2[1];
			double fTemp196 = fTemp183 - fVec2[1];
			double fTemp197 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp196);
			double fTemp198 = 0.5555555555555556 * fTemp183;
			double fTemp199 = GrungrFaustDSP_faustpower2_f(fTemp198) + 1.0;
			double fTemp200 = std::atan(fTemp198);
			double fTemp201 = ((fTemp197 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp198 + fTemp195)) : (0.5555555555555556 * fTemp183 * (0.5555555555555556 * (fTemp183 * fTemp200 - fTemp193) - 0.5 * (std::log(fTemp199) - fTemp194)) + 0.2777777777777778 * fTemp196 - 0.5 * (fTemp200 * fTemp199 - fTemp192)) / fTemp197) + ((GrungrFaustDSP_faustpower2_f(fTemp185) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp195 + fTemp186)) : (0.5555555555555556 * fVec2[2] * (0.5 * (fTemp194 - std::log(fTemp187)) - 0.5555555555555556 * (fTemp193 - fVec2[2] * fTemp188)) + 0.5 * (fTemp192 - fTemp188 * fTemp187) - 0.2777777777777778 * fTemp184) / GrungrFaustDSP_faustpower2_f(-fTemp185)) - fTemp165;
			fVec3[0] = fTemp201;
			fRec47[0] = fConst89 * (fConst88 * (fTemp201 - fVec3[1]) - fConst70 * fRec47[1]);
			double fTemp202 = fRec47[0] * fTemp163;
			double fTemp203 = fConst90 + fRec46[1];
			double fTemp204 = fRec46[1] - fConst90;
			fRec46[0] = ((fTemp203 < fTemp202) ? fTemp203 : ((fTemp204 > fTemp202) ? fTemp204 : fTemp202));
			double fTemp205 = (fRec46[0] + 0.014) * fTemp161 * fTemp160;
			double fTemp206 = fTemp205 / fTemp159;
			fVec4[0] = fTemp206;
			double fTemp207 = GrungrFaustDSP_faustpower2_f(fVec4[2]);
			double fTemp208 = std::atan(fVec4[2]);
			double fTemp209 = GrungrFaustDSP_faustpower2_f(fVec4[1]);
			double fTemp210 = std::atan(fVec4[1]);
			double fTemp211 = fTemp210 - (fVec4[1] - fTemp210 * fTemp209);
			double fTemp212 = fVec4[1] * fTemp210;
			double fTemp213 = std::log(fTemp209 + 1.0);
			double fTemp214 = 2.0 * fVec4[1];
			double fTemp215 = GrungrFaustDSP_faustpower2_f(fTemp206 - fVec4[1]);
			double fTemp216 = GrungrFaustDSP_faustpower2_f(fTemp206);
			double fTemp217 = std::atan(fTemp206);
			double fTemp218 = fTemp159 * (((fTemp215 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp206 + fTemp214)) : (fTemp205 * (fTemp205 * fTemp217 / fTemp159 - (fTemp212 + 0.5 * (std::log(fTemp216 + 1.0) - fTemp213))) / fTemp159 - 0.5 * (fTemp217 - (fTemp206 - fTemp217 * fTemp216) - fTemp211)) / fTemp215) + ((GrungrFaustDSP_faustpower2_f(fVec4[1] - fVec4[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp214 + fVec4[2])) : (fVec4[2] * (fVec4[2] * fTemp208 + 0.5 * (fTemp213 - std::log(fTemp207 + 1.0)) - fTemp212) + 0.5 * (fTemp211 - (fTemp208 - (fVec4[2] - fTemp208 * fTemp207)))) / GrungrFaustDSP_faustpower2_f(fVec4[2] - fVec4[1])) - fTemp162);
			fVec5[0] = fTemp218;
			fRec45[0] = 0.995 * fRec45[1] + 0.6366197723675814 * (fTemp218 - fVec5[1]);
			fRec44[0] = fRec45[0] - (fRec44[2] * fTemp157 + 2.0 * fRec44[1] * fTemp156) / fTemp155;
			double fTemp219 = (fRec44[2] + fRec44[0] + 2.0 * fRec44[1]) / fTemp155;
			fVec6[0] = fTemp219;
			double fTemp220 = 1.0 - fTemp151;
			fRec43[0] = -((fRec43[1] * fTemp220 - (fTemp219 - fVec6[1]) / fTemp150) / fTemp152);
			double fTemp221 = 1.0 - fTemp147;
			fRec42[0] = -((fRec42[1] * fTemp221 - (fRec43[0] + fRec43[1])) / fTemp148);
			double fTemp222 = fRec42[0] * fTemp146;
			double fTemp223 = fConst90 + fRec41[1];
			double fTemp224 = fRec41[1] - fConst90;
			fRec41[0] = ((fTemp223 < fTemp222) ? fTemp223 : ((fTemp224 > fTemp222) ? fTemp224 : fTemp222));
			double fTemp225 = (fRec41[0] + fTemp143) * fTemp142 * fTemp141;
			double fTemp226 = fTemp225 / fTemp140;
			fVec7[0] = fTemp226;
			double fTemp227 = GrungrFaustDSP_faustpower2_f(fVec7[2]);
			double fTemp228 = std::atan(fVec7[2]);
			double fTemp229 = GrungrFaustDSP_faustpower2_f(fVec7[1]);
			double fTemp230 = std::atan(fVec7[1]);
			double fTemp231 = fTemp230 - (fVec7[1] - fTemp230 * fTemp229);
			double fTemp232 = fVec7[1] * fTemp230;
			double fTemp233 = std::log(fTemp229 + 1.0);
			double fTemp234 = 2.0 * fVec7[1];
			double fTemp235 = GrungrFaustDSP_faustpower2_f(fTemp226 - fVec7[1]);
			double fTemp236 = GrungrFaustDSP_faustpower2_f(fTemp226);
			double fTemp237 = std::atan(fTemp226);
			double fTemp238 = fTemp140 * (((fTemp235 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp226 + fTemp234)) : (fTemp225 * (fTemp225 * fTemp237 / fTemp140 - (fTemp232 + 0.5 * (std::log(fTemp236 + 1.0) - fTemp233))) / fTemp140 - 0.5 * (fTemp237 - (fTemp226 - fTemp237 * fTemp236) - fTemp231)) / fTemp235) + ((GrungrFaustDSP_faustpower2_f(fVec7[1] - fVec7[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp234 + fVec7[2])) : (fVec7[2] * (fVec7[2] * fTemp228 + 0.5 * (fTemp233 - std::log(fTemp227 + 1.0)) - fTemp232) + 0.5 * (fTemp231 - (fTemp228 - (fVec7[2] - fTemp228 * fTemp227)))) / GrungrFaustDSP_faustpower2_f(fVec7[2] - fVec7[1])) - fTemp144);
			fVec8[0] = fTemp238;
			fRec40[0] = 0.995 * fRec40[1] + 0.6366197723675814 * (fTemp238 - fVec8[1]);
			fRec39[0] = fRec40[0] - (fRec39[2] * fTemp137 + 2.0 * fRec39[1] * fTemp136) / fTemp135;
			double fTemp239 = (fRec39[2] + fRec39[0] + 2.0 * fRec39[1]) / fTemp135;
			double fTemp240 = fTemp239 + 0.05 * fTemp4 * tanh(0.4 * fTemp239);
			double fTemp241 = tanh(1.6 * fTemp240);
			fRec38[0] = tanh(1.4 * std::fabs(fTemp240 + 0.2 * fTemp7 * fTemp241)) - fConst67 * (fConst65 * fRec38[2] + fConst63 * fRec38[1]);
			double fTemp242 = fTemp240 + fTemp7 * (0.2 * fTemp241 + fConst91 * (fRec38[2] + fRec38[0] + 2.0 * fRec38[1]));
			fVec9[0] = fTemp242;
			double fTemp243 = 1.0 - fTemp130;
			fRec37[0] = -((fRec37[1] * fTemp243 - (fTemp242 - fVec9[1]) / fTemp129) / fTemp131);
			double fTemp244 = fRec37[0] * fTemp128;
			double fTemp245 = fConst90 + fRec36[1];
			double fTemp246 = fRec36[1] - fConst90;
			fRec36[0] = ((fTemp245 < fTemp244) ? fTemp245 : ((fTemp246 > fTemp244) ? fTemp246 : fTemp244));
			fRec35[0] = fConst1 * std::fabs(fRec36[0]) + fConst2 * fRec35[1];
			double fTemp247 = fRec36[0] * (1.0 - fTemp127 * std::min<double>(1.0, std::max<double>(0.0, fRec35[0] * fTemp11 / fTemp127)));
			fVec10[0] = fTemp247;
			fRec34[0] = -(fConst93 * (fConst92 * fRec34[1] - fConst58 * (fTemp247 - fVec10[1])));
			fRec33[0] = fRec34[0] - fConst61 * (fConst59 * fRec33[2] + fConst57 * fRec33[1]);
			fRec57[0] = -(fConst93 * (fConst92 * fRec57[1] - (fTemp247 + fVec10[1])));
			fRec56[0] = fRec57[0] - fConst61 * (fConst59 * fRec56[2] + fConst57 * fRec56[1]);
			double fTemp248 = fRec56[2] + fRec56[0] + 2.0 * fRec56[1] + fConst56 * (fRec33[2] + (fRec33[0] - 2.0 * fRec33[1])) * fTemp126;
			fVec11[0] = fTemp248;
			fRec32[0] = -(fConst96 * (fConst95 * fRec32[1] - fConst94 * (fTemp248 - fVec11[1])));
			fRec31[0] = fRec32[0] - fConst54 * (fConst52 * fRec31[2] + fConst50 * fRec31[1]);
			fRec59[0] = -(fConst96 * (fConst95 * fRec59[1] - fConst61 * (fTemp248 + fVec11[1])));
			fRec58[0] = fRec59[0] - fConst54 * (fConst52 * fRec58[2] + fConst50 * fRec58[1]);
			double fTemp249 = fRec58[2] + fRec58[0] + 2.0 * fRec58[1] + fConst49 * fTemp126 * (fRec31[2] + (fRec31[0] - 2.0 * fRec31[1]));
			int iTemp250 = (fConst54 * fTemp249) > 0.0;
			double fTemp251 = 1.08 - 0.08 * fTemp17;
			double fTemp252 = fTemp249 / fTemp251;
			fVec12[0] = fTemp252;
			double fTemp253 = fConst54 * (fVec12[1] - fVec12[2]);
			double fTemp254 = fConst54 * fVec12[2];
			int iTemp255 = (fTemp254 <= 1.0) & (fTemp254 >= -1.0);
			double fTemp256 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp254);
			double fTemp257 = static_cast<double>((fTemp254 > 0.0) - (fTemp254 < 0.0));
			double fTemp258 = fConst54 * fVec12[1];
			int iTemp259 = (fTemp258 <= 1.0) & (fTemp258 >= -1.0);
			double fTemp260 = static_cast<double>((fTemp258 > 0.0) - (fTemp258 < 0.0));
			double fTemp261 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp258);
			double fTemp262 = ((iTemp259) ? fTemp261 : fConst54 * fVec12[1] * fTemp260 + -0.5);
			double fTemp263 = ((iTemp259) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp258) : fTemp260 * (fTemp261 + -0.16666666666666666));
			double fTemp264 = 2.0 * fVec12[1];
			double fTemp265 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp252 - fVec12[1]));
			double fTemp266 = fConst54 * fTemp252;
			int iTemp267 = (fTemp266 <= 1.0) & (fTemp266 >= -1.0);
			double fTemp268 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp266);
			double fTemp269 = static_cast<double>((fTemp266 > 0.0) - (fTemp266 < 0.0));
			double fTemp270 = 1.62 - fTemp138;
			double fTemp271 = fTemp249 / fTemp270;
			fVec13[0] = fTemp271;
			double fTemp272 = fConst54 * (fVec13[1] - fVec13[2]);
			double fTemp273 = fConst54 * fVec13[2];
			int iTemp274 = (fTemp273 <= 1.0) & (fTemp273 >= -1.0);
			double fTemp275 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp273);
			double fTemp276 = static_cast<double>((fTemp273 > 0.0) - (fTemp273 < 0.0));
			double fTemp277 = fConst54 * fVec13[1];
			int iTemp278 = (fTemp277 <= 1.0) & (fTemp277 >= -1.0);
			double fTemp279 = static_cast<double>((fTemp277 > 0.0) - (fTemp277 < 0.0));
			double fTemp280 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp277);
			double fTemp281 = ((iTemp278) ? fTemp280 : fConst54 * fVec13[1] * fTemp279 + -0.5);
			double fTemp282 = ((iTemp278) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp277) : fTemp279 * (fTemp280 + -0.16666666666666666));
			double fTemp283 = 2.0 * fVec13[1];
			double fTemp284 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp271 - fVec13[1]));
			double fTemp285 = fConst54 * fTemp271;
			int iTemp286 = (fTemp285 <= 1.0) & (fTemp285 >= -1.0);
			double fTemp287 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp285);
			double fTemp288 = static_cast<double>((fTemp285 > 0.0) - (fTemp285 < 0.0));
			double fTemp289 = fTemp84 * (0.34 * fTemp12 + 0.08);
			double fTemp290 = 0.74 - fTemp158;
			double fTemp291 = fTemp249 / fTemp290;
			fVec14[0] = fTemp291;
			double fTemp292 = fConst54 * (fVec14[1] - fVec14[2]);
			double fTemp293 = fConst54 * fVec14[2];
			int iTemp294 = (fTemp293 <= 1.0) & (fTemp293 >= -1.0);
			double fTemp295 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp293);
			double fTemp296 = static_cast<double>((fTemp293 > 0.0) - (fTemp293 < 0.0));
			double fTemp297 = fConst54 * fVec14[1];
			int iTemp298 = (fTemp297 <= 1.0) & (fTemp297 >= -1.0);
			double fTemp299 = static_cast<double>((fTemp297 > 0.0) - (fTemp297 < 0.0));
			double fTemp300 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp297);
			double fTemp301 = ((iTemp298) ? fTemp300 : fConst54 * fVec14[1] * fTemp299 + -0.5);
			double fTemp302 = ((iTemp298) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp297) : fTemp299 * (fTemp300 + -0.16666666666666666));
			double fTemp303 = 2.0 * fVec14[1];
			double fTemp304 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp291 - fVec14[1]));
			double fTemp305 = fConst54 * fTemp291;
			int iTemp306 = (fTemp305 <= 1.0) & (fTemp305 >= -1.0);
			double fTemp307 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp305);
			double fTemp308 = static_cast<double>((fTemp305 > 0.0) - (fTemp305 < 0.0));
			double fTemp309 = 1.1 - 0.16 * fTemp17;
			double fTemp310 = fTemp249 / fTemp309;
			fVec15[0] = fTemp310;
			double fTemp311 = fConst54 * (fVec15[1] - fVec15[2]);
			double fTemp312 = fConst54 * fVec15[2];
			int iTemp313 = (fTemp312 <= 1.0) & (fTemp312 >= -1.0);
			double fTemp314 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp312);
			double fTemp315 = static_cast<double>((fTemp312 > 0.0) - (fTemp312 < 0.0));
			double fTemp316 = fConst54 * fVec15[1];
			int iTemp317 = (fTemp316 <= 1.0) & (fTemp316 >= -1.0);
			double fTemp318 = static_cast<double>((fTemp316 > 0.0) - (fTemp316 < 0.0));
			double fTemp319 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp316);
			double fTemp320 = ((iTemp317) ? fTemp319 : fConst54 * fVec15[1] * fTemp318 + -0.5);
			double fTemp321 = ((iTemp317) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp316) : fTemp318 * (fTemp319 + -0.16666666666666666));
			double fTemp322 = 2.0 * fVec15[1];
			double fTemp323 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp310 - fVec15[1]));
			double fTemp324 = fConst54 * fTemp310;
			int iTemp325 = (fTemp324 <= 1.0) & (fTemp324 >= -1.0);
			double fTemp326 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp324);
			double fTemp327 = static_cast<double>((fTemp324 > 0.0) - (fTemp324 < 0.0));
			double fTemp328 = 1.0 - fTemp289;
			double fTemp329 = fTemp328 * ((iTemp250) ? fTemp309 * (((fTemp323 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp310 + fTemp322))) : (fConst54 * (fTemp249 * (((iTemp325) ? fTemp326 : fConst54 * (fTemp249 * fTemp327 / fTemp309) + -0.5) - fTemp320) / fTemp309) + fTemp321 - ((iTemp325) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp324) : fTemp327 * (fTemp326 + -0.16666666666666666))) / fTemp323) + ((GrungrFaustDSP_faustpower2_f(fTemp311) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp322 + fVec15[2]))) : (fTemp321 + fConst54 * fVec15[2] * (((iTemp313) ? fTemp314 : fConst54 * fVec15[2] * fTemp315 + -0.5) - fTemp320) - ((iTemp313) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp312) : fTemp315 * (fTemp314 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp311))) : fTemp290 * (((fTemp304 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp291 + fTemp303))) : (fConst54 * (fTemp249 * (((iTemp306) ? fTemp307 : fConst54 * (fTemp249 * fTemp308 / fTemp290) + -0.5) - fTemp301) / fTemp290) + fTemp302 - ((iTemp306) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp305) : fTemp308 * (fTemp307 + -0.16666666666666666))) / fTemp304) + ((GrungrFaustDSP_faustpower2_f(fTemp292) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp303 + fVec14[2]))) : (fTemp302 + fConst54 * fVec14[2] * (((iTemp294) ? fTemp295 : fConst54 * fVec14[2] * fTemp296 + -0.5) - fTemp301) - ((iTemp294) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp293) : fTemp296 * (fTemp295 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp292)))) + fTemp289 * ((iTemp250) ? fTemp270 * (((fTemp284 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp271 + fTemp283))) : (fConst54 * (fTemp249 * (((iTemp286) ? fTemp287 : fConst54 * (fTemp249 * fTemp288 / fTemp270) + -0.5) - fTemp281) / fTemp270) + fTemp282 - ((iTemp286) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp285) : fTemp288 * (fTemp287 + -0.16666666666666666))) / fTemp284) + ((GrungrFaustDSP_faustpower2_f(fTemp272) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp283 + fVec13[2]))) : (fTemp282 + fConst54 * fVec13[2] * (((iTemp274) ? fTemp275 : fConst54 * fVec13[2] * fTemp276 + -0.5) - fTemp281) - ((iTemp274) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp273) : fTemp276 * (fTemp275 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp272))) : fTemp251 * (((fTemp265 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp252 + fTemp264))) : (fConst54 * (fTemp249 * (((iTemp267) ? fTemp268 : fConst54 * (fTemp249 * fTemp269 / fTemp251) + -0.5) - fTemp262) / fTemp251) + fTemp263 - ((iTemp267) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp266) : fTemp269 * (fTemp268 + -0.16666666666666666))) / fTemp265) + ((GrungrFaustDSP_faustpower2_f(fTemp253) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp264 + fVec12[2]))) : (fTemp263 + fConst54 * fVec12[2] * (((iTemp255) ? fTemp256 : fConst54 * fVec12[2] * fTemp257 + -0.5) - fTemp262) - ((iTemp255) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp254) : fTemp257 * (fTemp256 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp253))));
			fVec16[0] = fTemp329;
			fRec30[0] = -(fConst93 * (fConst92 * fRec30[1] - fConst58 * (fTemp329 - fVec16[1])));
			fRec29[0] = fRec30[0] - fConst61 * (fConst59 * fRec29[2] + fConst57 * fRec29[1]);
			fRec61[0] = -(fConst93 * (fConst92 * fRec61[1] - (fTemp329 + fVec16[1])));
			fRec60[0] = fRec61[0] - fConst61 * (fConst59 * fRec60[2] + fConst57 * fRec60[1]);
			double fTemp330 = fRec60[2] + fRec60[0] + 2.0 * fRec60[1] + fConst56 * (fRec29[2] + (fRec29[0] - 2.0 * fRec29[1])) * fTemp125;
			fVec17[0] = fTemp330;
			fRec28[0] = -(fConst96 * (fConst95 * fRec28[1] - fConst94 * (fTemp330 - fVec17[1])));
			fRec27[0] = fRec28[0] - fConst54 * (fConst52 * fRec27[2] + fConst50 * fRec27[1]);
			fRec63[0] = -(fConst96 * (fConst95 * fRec63[1] - fConst61 * (fTemp330 + fVec17[1])));
			fRec62[0] = fRec63[0] - fConst54 * (fConst52 * fRec62[2] + fConst50 * fRec62[1]);
			fRec26[0] = fConst54 * (fRec62[2] + fRec62[0] + 2.0 * fRec62[1] + fConst49 * fTemp125 * (fRec27[2] + (fRec27[0] - 2.0 * fRec27[1]))) - (fRec26[2] * fTemp123 + 2.0 * fRec26[1] * fTemp122) / fTemp121;
			double fTemp331 = (fRec26[2] + fRec26[0] + 2.0 * fRec26[1]) / fTemp121;
			fVec18[0] = fTemp331;
			fRec25[0] = 0.995 * fRec25[1] + fTemp331 - fVec18[1];
			fRec24[0] = -(fConst99 * (fConst98 * fRec24[1] - fConst44 * (fRec25[0] - fRec25[1])));
			fRec23[0] = fRec24[0] - fConst47 * (fConst45 * fRec23[2] + fConst43 * fRec23[1]);
			fRec64[0] = fSlow6 + fConst2 * fRec64[1];
			double fTemp332 = std::pow(1e+01, 0.05 * (24.0 * std::pow(std::max<double>(fRec64[0], 2.220446049250313e-16), 0.82) + -9.0));
			fRec66[0] = -(fConst99 * (fConst98 * fRec66[1] - (fRec25[0] + fRec25[1])));
			fRec65[0] = fRec66[0] - fConst47 * (fConst45 * fRec65[2] + fConst43 * fRec65[1]);
			double fTemp333 = (fRec65[2] + fRec65[0] + 2.0 * fRec65[1]) * fTemp332 + fConst42 * (fRec23[2] + (fRec23[0] - 2.0 * fRec23[1]));
			fVec19[0] = fTemp333;
			fRec22[0] = -(fConst102 * (fConst101 * fRec22[1] - fConst100 * (fTemp333 - fVec19[1])));
			fRec21[0] = fRec22[0] - fConst40 * (fConst39 * fRec21[2] + fConst37 * fRec21[1]);
			fRec68[0] = -(fConst102 * (fConst101 * fRec68[1] - fConst47 * (fTemp333 + fVec19[1])));
			fRec67[0] = fRec68[0] - fConst40 * (fConst39 * fRec67[2] + fConst37 * fRec67[1]);
			fRec20[0] = fConst40 * (fRec67[2] + fRec67[0] + 2.0 * fRec67[1] + fConst36 * (fRec21[2] + (fRec21[0] - 2.0 * fRec21[1])) * fTemp118) - (fRec20[2] * fTemp117 + fTemp116) / fTemp113;
			double fTemp334 = fConst33 * (fConst33 + fTemp114) + 1.0;
			fRec19[0] = (fTemp116 + fRec20[0] * fTemp334 + fRec20[2] * fTemp115) / fTemp113 - (fRec19[2] * fTemp108 + fTemp107) / fTemp103;
			double fTemp335 = (fTemp102 + fTemp104) / fTemp95 + 1.0;
			fRec18[0] = (fTemp107 + fRec19[0] * fTemp335 + fRec19[2] * fTemp105) / fTemp103 - (fRec18[2] * fTemp93 + fTemp92) / fTemp89;
			double fTemp336 = fConst22 * (fConst22 + fTemp90) + 1.0;
			fRec17[0] = (fTemp92 + fRec18[0] * fTemp336 + fRec18[2] * fTemp91) / fTemp89 - (fRec17[2] * fTemp83 + fTemp82) / fTemp79;
			double fTemp337 = fConst26 * (fConst26 + fTemp80) + 1.0;
			fRec16[0] = (fTemp82 + fRec17[0] * fTemp337 + fRec17[2] * fTemp81) / fTemp79 - (fRec16[2] * fTemp74 + fTemp73) / fTemp70;
			double fTemp338 = fConst22 * (fConst22 + fTemp71) + 1.0;
			fRec15[0] = (fTemp73 + fRec16[0] * fTemp338 + fRec16[2] * fTemp72) / fTemp70 - (fRec15[2] * fTemp65 + fTemp64) / fTemp61;
			double fTemp339 = fConst17 * (fConst17 + fTemp62) + 1.0;
			double fTemp340 = (fTemp64 + fRec15[0] * fTemp339 + fRec15[2] * fTemp63) / fTemp61;
			fVec20[0] = fTemp340;
			fRec14[0] = fConst104 * (fConst103 * (fTemp340 - fVec20[1]) - fConst14 * fRec14[1]);
			double fTemp341 = 1.0 - fTemp48;
			fRec13[0] = -((fRec13[1] * fTemp341 - (fRec14[0] - fRec14[1]) / fTemp47) / fTemp56);
			fRec12[0] = fRec13[0] - (fRec12[2] * fTemp55 + 2.0 * fRec12[1] * fTemp54) / fTemp49;
			fRec70[0] = -((fTemp341 * fRec70[1] - (fRec14[0] + fRec14[1])) / fTemp56);
			fRec69[0] = fRec70[0] - (fTemp55 * fRec69[2] + 2.0 * fTemp54 * fRec69[1]) / fTemp49;
			fRec11[0] = (fRec69[2] + fRec69[0] + 2.0 * fRec69[1] + (fRec12[2] + (fRec12[0] - 2.0 * fRec12[1])) * fTemp53 / fTemp50) / fTemp49 - (fRec11[2] * fTemp46 + fTemp45) / fTemp42;
			double fTemp342 = fConst10 * (fConst10 + fTemp43) + 1.0;
			fRec10[0] = (fTemp45 + fRec11[0] * fTemp342 + fRec11[2] * fTemp44) / fTemp42 - (fRec10[2] * fTemp37 + 2.0 * fRec10[1] * fTemp36) / fTemp35;
			fRec9[0] = (fRec10[2] + fRec10[0] + 2.0 * fRec10[1]) / fTemp35 - (fRec9[2] * fTemp32 + 2.0 * fRec9[1] * fTemp31) / fTemp30;
			double fTemp343 = (fRec9[2] + fRec9[0] + 2.0 * fRec9[1]) / fTemp30;
			fRec7[0] = ((iTemp0) ? 0.0 : fTemp343) - (fRec7[2] * fTemp23 + 2.0 * fRec7[1] * fTemp22) / fTemp21;
			fRec71[0] = fSlow7 + fConst2 * fRec71[1];
			double fTemp344 = std::pow(std::max<double>(fRec71[0], 2.220446049250313e-16), 1.65);
			double fTemp345 = fTemp344 * ((iTemp0) ? fTemp343 : (fRec7[2] + fRec7[0] + 2.0 * fRec7[1]) / fTemp21) * fTemp18;
			double fTemp346 = fConst90 + fRec6[1];
			double fTemp347 = fRec6[1] - fConst90;
			fRec6[0] = ((fTemp346 < fTemp345) ? fTemp346 : ((fTemp347 > fTemp345) ? fTemp347 : fTemp345));
			fRec5[0] = fConst1 * std::fabs(fRec6[0]) + fConst2 * fRec5[1];
			double fTemp348 = fRec6[0] * (1.0 - fTemp9 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec5[0] * fTemp11 / fTemp9)))) * fTemp6;
			fVec21[0] = fTemp348;
			double fTemp349 = fVec21[1] - fVec21[2];
			double fTemp350 = 0.8928571428571428 * fTemp349;
			double fTemp351 = 0.8928571428571428 * fVec21[2];
			double fTemp352 = GrungrFaustDSP_faustpower2_f(fTemp351) + 1.0;
			double fTemp353 = std::atan(fTemp351);
			double fTemp354 = 0.8928571428571428 * fVec21[1];
			double fTemp355 = GrungrFaustDSP_faustpower2_f(fTemp354) + 1.0;
			double fTemp356 = std::atan(fTemp354);
			double fTemp357 = fTemp356 * fTemp355;
			double fTemp358 = fVec21[1] * fTemp356;
			double fTemp359 = std::log(fTemp355);
			double fTemp360 = 1.7857142857142856 * fVec21[1];
			double fTemp361 = fTemp348 - fVec21[1];
			double fTemp362 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp361);
			double fTemp363 = 0.8928571428571428 * fTemp348;
			double fTemp364 = GrungrFaustDSP_faustpower2_f(fTemp363) + 1.0;
			double fTemp365 = std::atan(fTemp363);
			double fTemp366 = 0.7130141450516913 * (((fTemp362 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp363 + fTemp360)) : (0.8928571428571428 * fTemp348 * (0.8928571428571428 * (fTemp348 * fTemp365 - fTemp358) - 0.5 * (std::log(fTemp364) - fTemp359)) + 0.4464285714285714 * fTemp361 - 0.5 * (fTemp365 * fTemp364 - fTemp357)) / fTemp362) + ((GrungrFaustDSP_faustpower2_f(fTemp350) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp360 + fTemp351)) : (0.8928571428571428 * fVec21[2] * (0.5 * (fTemp359 - std::log(fTemp352)) - 0.8928571428571428 * (fTemp358 - fVec21[2] * fTemp353)) + 0.5 * (fTemp357 - fTemp353 * fTemp352) - 0.4464285714285714 * fTemp349) / GrungrFaustDSP_faustpower2_f(-fTemp350)));
			double fTemp367 = ((iTemp1) ? 0.0 : fTemp366);
			fVec22[IOTA0 & 1023] = fTemp367;
			double fTemp368 = std::fabs(fTemp367);
			int iTemp369 = (fTemp368 >= fRec75[1]) | (static_cast<double>(iRec74[1]) >= fConst5);
			iRec74[0] = ((iTemp369) ? 0 : iRec74[1] + 1);
			fRec75[0] = ((iTemp369) ? fTemp368 : fRec75[1]);
			fRec73[0] = fConst83 * fRec75[0] + fConst77 * fRec73[1];
			double fTemp370 = std::fabs(fRec73[0]);
			fRec72[0] = std::max<double>(fTemp370, fConst81 * fRec72[1] + fConst82 * fTemp370);
			double fTemp371 = ((iTemp1) ? fTemp366 : std::min<double>(1.0, 0.944 / std::max<double>(fRec72[0], 2.220446049250313e-16)) * fVec22[(IOTA0 - iConst106) & 1023]);
			double fTemp372 = ((iTemp1) ? 0.0 : fTemp371);
			double fTemp373 = std::fabs(fTemp372);
			fVec23[0] = fTemp373;
			double fTemp374 = std::max<double>(fTemp373, fVec23[1]);
			fVec24[0] = fTemp374;
			double fTemp375 = std::max<double>(fTemp374, fVec24[2]);
			fVec25[0] = fTemp375;
			double fTemp376 = std::max<double>(fTemp375, fVec25[4]);
			fVec26[0] = fTemp376;
			double fTemp377 = std::max<double>(fTemp376, fVec26[8]);
			fVec27[IOTA0 & 31] = fTemp377;
			double fTemp378 = std::max<double>(fTemp377, fVec27[(IOTA0 - 16) & 31]);
			fVec28[IOTA0 & 63] = fTemp378;
			double fTemp379 = std::max<double>(fTemp378, fVec28[(IOTA0 - 32) & 63]);
			fVec29[IOTA0 & 127] = fTemp379;
			double fTemp380 = std::max<double>(fTemp379, fVec29[(IOTA0 - 64) & 127]);
			fVec30[IOTA0 & 255] = fTemp380;
			double fTemp381 = std::max<double>(fTemp380, fVec30[(IOTA0 - 128) & 255]);
			fVec31[IOTA0 & 511] = fTemp381;
			double fTemp382 = std::max<double>(fTemp381, fVec31[(IOTA0 - 256) & 511]);
			fVec32[IOTA0 & 1023] = fTemp382;
			fVec33[IOTA0 & 2047] = std::max<double>(fTemp382, fVec32[(IOTA0 - 512) & 1023]);
			double fTemp383 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst6) ? fTemp373 : -1.7976931348623157e+308), ((iConst107) ? fVec24[iConst6] : -1.7976931348623157e+308)), ((iConst108) ? fVec25[iConst109] : -1.7976931348623157e+308)), ((iConst110) ? fVec26[iConst111] : -1.7976931348623157e+308)), ((iConst112) ? fVec27[(IOTA0 - iConst113) & 31] : -1.7976931348623157e+308)), ((iConst114) ? fVec28[(IOTA0 - iConst115) & 63] : -1.7976931348623157e+308)), ((iConst116) ? fVec29[(IOTA0 - iConst117) & 127] : -1.7976931348623157e+308)), ((iConst118) ? fVec30[(IOTA0 - iConst119) & 255] : -1.7976931348623157e+308)), ((iConst120) ? fVec31[(IOTA0 - iConst121) & 511] : -1.7976931348623157e+308)), ((iConst122) ? fVec32[(IOTA0 - iConst123) & 1023] : -1.7976931348623157e+308)), ((iConst124) ? fVec33[(IOTA0 - iConst125) & 2047] : -1.7976931348623157e+308));
			double fTemp384 = ((fTemp383 > fRec3[1]) ? fConst71 : fConst85);
			fRec3[0] = fTemp383 * (1.0 - fTemp384) + fRec3[1] * fTemp384;
			double fTemp385 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec3[0]));
			int iTemp386 = (fTemp385 > -53.0) + (fTemp385 > -47.0);
			double fTemp387 = ((iTemp1) ? fTemp371 : fTemp372 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp386 == 0) ? fTemp385 + 5e+01 : ((iTemp386 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp385 + 47.0)) : 0.0)))));
			double fTemp388 = ((iTemp1) ? 0.0 : fTemp387);
			double fTemp389 = ((iTemp1) ? fTemp387 : static_cast<double>((fTemp388 > 0.0) - (fTemp388 < 0.0)) * std::pow(std::fabs(fTemp388), 0.97));
			double fTemp390 = ((iTemp0) ? 0.0 : fTemp389);
			fVec34[IOTA0 & 2047] = fTemp390;
			double fTemp391 = std::fabs(fTemp390);
			int iTemp392 = (fTemp391 >= fRec79[1]) | (static_cast<double>(iRec78[1]) >= fConst3);
			iRec78[0] = ((iTemp392) ? 0 : iRec78[1] + 1);
			fRec79[0] = ((iTemp392) ? fTemp391 : fRec79[1]);
			fRec77[0] = fConst87 * fRec79[0] + fConst71 * fRec77[1];
			double fTemp393 = std::fabs(fRec77[0]);
			fRec76[0] = std::max<double>(fTemp393, fConst85 * fRec76[1] + fConst86 * fTemp393);
			double fTemp394 = ((iTemp0) ? fTemp389 : std::min<double>(1.0, 0.944 / std::max<double>(fRec76[0], 2.220446049250313e-16)) * fVec34[(IOTA0 - iConst126) & 2047]);
			double fTemp395 = ((iTemp0) ? 0.0 : fTemp394);
			double fTemp396 = std::fabs(fTemp395);
			fVec35[0] = fTemp396;
			double fTemp397 = std::max<double>(fTemp396, fVec35[1]);
			fVec36[0] = fTemp397;
			double fTemp398 = std::max<double>(fTemp397, fVec36[2]);
			fVec37[0] = fTemp398;
			double fTemp399 = std::max<double>(fTemp398, fVec37[4]);
			fVec38[0] = fTemp399;
			double fTemp400 = std::max<double>(fTemp399, fVec38[8]);
			fVec39[IOTA0 & 31] = fTemp400;
			double fTemp401 = std::max<double>(fTemp400, fVec39[(IOTA0 - 16) & 31]);
			fVec40[IOTA0 & 63] = fTemp401;
			double fTemp402 = std::max<double>(fTemp401, fVec40[(IOTA0 - 32) & 63]);
			fVec41[IOTA0 & 127] = fTemp402;
			double fTemp403 = std::max<double>(fTemp402, fVec41[(IOTA0 - 64) & 127]);
			fVec42[IOTA0 & 255] = fTemp403;
			double fTemp404 = std::max<double>(fTemp403, fVec42[(IOTA0 - 128) & 255]);
			fVec43[IOTA0 & 511] = fTemp404;
			double fTemp405 = std::max<double>(fTemp404, fVec43[(IOTA0 - 256) & 511]);
			fVec44[IOTA0 & 1023] = fTemp405;
			fVec45[IOTA0 & 2047] = std::max<double>(fTemp405, fVec44[(IOTA0 - 512) & 1023]);
			double fTemp406 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp396 : -1.7976931348623157e+308), ((iConst6) ? fVec36[iConst4] : -1.7976931348623157e+308)), ((iConst107) ? fVec37[iConst127] : -1.7976931348623157e+308)), ((iConst108) ? fVec38[iConst128] : -1.7976931348623157e+308)), ((iConst110) ? fVec39[(IOTA0 - iConst129) & 31] : -1.7976931348623157e+308)), ((iConst112) ? fVec40[(IOTA0 - iConst130) & 63] : -1.7976931348623157e+308)), ((iConst114) ? fVec41[(IOTA0 - iConst131) & 127] : -1.7976931348623157e+308)), ((iConst116) ? fVec42[(IOTA0 - iConst132) & 255] : -1.7976931348623157e+308)), ((iConst118) ? fVec43[(IOTA0 - iConst133) & 511] : -1.7976931348623157e+308)), ((iConst120) ? fVec44[(IOTA0 - iConst134) & 1023] : -1.7976931348623157e+308)), ((iConst122) ? fVec45[(IOTA0 - iConst135) & 2047] : -1.7976931348623157e+308));
			double fTemp407 = ((fTemp406 > fRec1[1]) ? fConst71 : fConst136);
			fRec1[0] = fTemp406 * (1.0 - fTemp407) + fRec1[1] * fTemp407;
			double fTemp408 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec1[0]));
			int iTemp409 = (fTemp408 > -58.0) + (fTemp408 > -52.0);
			double fTemp410 = ((iTemp0) ? fTemp394 : fTemp395 * std::pow(1e+01, 0.05 * std::max<double>(-35.0, 4.0 * ((iTemp409 == 0) ? fTemp408 + 55.0 : ((iTemp409 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp408 + 52.0)) : 0.0)))));
			double fTemp411 = ((iTemp0) ? 0.0 : fTemp410);
			output0[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp177 : ((iTemp0) ? fTemp410 : static_cast<double>((fTemp411 > 0.0) - (fTemp411 < 0.0)) * std::pow(std::fabs(fTemp411), 0.97))));
			double fTemp412 = fConst11 * fRec87[1];
			double fTemp413 = fConst18 * fRec91[1];
			double fTemp414 = fConst23 * fRec92[1];
			double fTemp415 = fConst27 * fRec93[1];
			double fTemp416 = fConst23 * fRec94[1];
			double fTemp417 = 2.0 * fTemp106 * fRec95[1];
			double fTemp418 = fConst34 * fRec96[1];
			double fTemp419 = static_cast<double>(input1[i0]);
			double fTemp420 = ((iSlow0) ? 0.0 : fTemp419);
			fVec46[0] = fTemp420;
			fRec131[0] = (0.2 * ((fTemp420 - fVec46[1]) / fTemp173) - fTemp176 * fRec131[1]) / fTemp175;
			fRec130[0] = fRec131[0] - (fTemp172 * fRec130[2] + 2.0 * fTemp171 * fRec130[1]) / fTemp168;
			double fTemp421 = (fRec130[2] + (fRec130[0] - 2.0 * fRec130[1])) / fTemp170;
			fVec47[0] = fTemp421;
			fRec129[0] = -(fConst80 * (fConst79 * fRec129[1] - (fTemp421 + fVec47[1])));
			double fTemp422 = std::fabs(fRec129[0]);
			fRec128[0] = std::max<double>(fTemp422, fConst81 * fRec128[1] + fConst82 * fTemp422);
			fRec127[0] = fConst83 * fRec128[0] + fConst77 * fRec127[1];
			double fTemp423 = fRec129[0] * (1.0 - 0.7 * fTemp4 * std::min<double>(1.0, std::max<double>(0.0, fRec127[0] + -0.4)));
			fRec126[0] = fTemp423 - fConst76 * (fConst74 * fRec126[2] + fConst73 * fRec126[1]);
			double fTemp424 = std::fabs(fConst84 * (fRec126[2] + (fRec126[0] - 2.0 * fRec126[1])));
			fRec125[0] = std::max<double>(fTemp424, fConst85 * fRec125[1] + fConst86 * fTemp424);
			fRec124[0] = fConst87 * fRec125[0] + fConst71 * fRec124[1];
			double fTemp425 = fTemp164 * (fTemp423 * (1.0 - 0.5 * fTemp7 * std::min<double>(1.0, std::max<double>(0.0, fRec124[0] + -0.5))) + 0.01);
			fVec48[0] = fTemp425;
			double fTemp426 = fVec48[1] - fVec48[2];
			double fTemp427 = 0.5555555555555556 * fTemp426;
			double fTemp428 = 0.5555555555555556 * fVec48[2];
			double fTemp429 = GrungrFaustDSP_faustpower2_f(fTemp428) + 1.0;
			double fTemp430 = std::atan(fTemp428);
			double fTemp431 = 0.5555555555555556 * fVec48[1];
			double fTemp432 = GrungrFaustDSP_faustpower2_f(fTemp431) + 1.0;
			double fTemp433 = std::atan(fTemp431);
			double fTemp434 = fTemp433 * fTemp432;
			double fTemp435 = fVec48[1] * fTemp433;
			double fTemp436 = std::log(fTemp432);
			double fTemp437 = 1.1111111111111112 * fVec48[1];
			double fTemp438 = fTemp425 - fVec48[1];
			double fTemp439 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp438);
			double fTemp440 = 0.5555555555555556 * fTemp425;
			double fTemp441 = GrungrFaustDSP_faustpower2_f(fTemp440) + 1.0;
			double fTemp442 = std::atan(fTemp440);
			double fTemp443 = fTemp165 - (((fTemp439 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp440 + fTemp437)) : (0.5555555555555556 * fTemp425 * (0.5555555555555556 * (fTemp425 * fTemp442 - fTemp435) - 0.5 * (std::log(fTemp441) - fTemp436)) + 0.2777777777777778 * fTemp438 - 0.5 * (fTemp442 * fTemp441 - fTemp434)) / fTemp439) + ((GrungrFaustDSP_faustpower2_f(fTemp427) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp437 + fTemp428)) : (0.5555555555555556 * fVec48[2] * (0.5 * (fTemp436 - std::log(fTemp429)) - 0.5555555555555556 * (fTemp435 - fVec48[2] * fTemp430)) + 0.5 * (fTemp434 - fTemp430 * fTemp429) - 0.2777777777777778 * fTemp426) / GrungrFaustDSP_faustpower2_f(-fTemp427)));
			fVec49[0] = fTemp443;
			fRec123[0] = -(fConst89 * (fConst70 * fRec123[1] + fConst88 * (fTemp443 - fVec49[1])));
			double fTemp444 = fRec123[0] * fTemp163;
			double fTemp445 = fConst90 + fRec122[1];
			double fTemp446 = fRec122[1] - fConst90;
			fRec122[0] = ((fTemp445 < fTemp444) ? fTemp445 : ((fTemp446 > fTemp444) ? fTemp446 : fTemp444));
			double fTemp447 = (fRec122[0] + 0.014) * fTemp161 * fTemp160;
			double fTemp448 = fTemp447 / fTemp159;
			fVec50[0] = fTemp448;
			double fTemp449 = GrungrFaustDSP_faustpower2_f(fVec50[2]);
			double fTemp450 = std::atan(fVec50[2]);
			double fTemp451 = GrungrFaustDSP_faustpower2_f(fVec50[1]);
			double fTemp452 = std::atan(fVec50[1]);
			double fTemp453 = fTemp452 - (fVec50[1] - fTemp452 * fTemp451);
			double fTemp454 = fVec50[1] * fTemp452;
			double fTemp455 = std::log(fTemp451 + 1.0);
			double fTemp456 = 2.0 * fVec50[1];
			double fTemp457 = GrungrFaustDSP_faustpower2_f(fTemp448 - fVec50[1]);
			double fTemp458 = GrungrFaustDSP_faustpower2_f(fTemp448);
			double fTemp459 = std::atan(fTemp448);
			double fTemp460 = fTemp159 * (fTemp162 - (((fTemp457 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp448 + fTemp456)) : (fTemp447 * (fTemp447 * fTemp459 / fTemp159 - (fTemp454 + 0.5 * (std::log(fTemp458 + 1.0) - fTemp455))) / fTemp159 - 0.5 * (fTemp459 - (fTemp448 - fTemp459 * fTemp458) - fTemp453)) / fTemp457) + ((GrungrFaustDSP_faustpower2_f(fVec50[1] - fVec50[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp456 + fVec50[2])) : (fVec50[2] * (fVec50[2] * fTemp450 + 0.5 * (fTemp455 - std::log(fTemp449 + 1.0)) - fTemp454) + 0.5 * (fTemp453 - (fTemp450 - (fVec50[2] - fTemp450 * fTemp449)))) / GrungrFaustDSP_faustpower2_f(fVec50[2] - fVec50[1]))));
			fVec51[0] = fTemp460;
			fRec121[0] = 0.995 * fRec121[1] - 0.6366197723675814 * (fTemp460 - fVec51[1]);
			fRec120[0] = fRec121[0] - (fTemp157 * fRec120[2] + 2.0 * fTemp156 * fRec120[1]) / fTemp155;
			double fTemp461 = (fRec120[2] + fRec120[0] + 2.0 * fRec120[1]) / fTemp155;
			fVec52[0] = fTemp461;
			fRec119[0] = -((fTemp220 * fRec119[1] - (fTemp461 - fVec52[1]) / fTemp150) / fTemp152);
			fRec118[0] = -((fTemp221 * fRec118[1] - (fRec119[0] + fRec119[1])) / fTemp148);
			double fTemp462 = fRec118[0] * fTemp146;
			double fTemp463 = fConst90 + fRec117[1];
			double fTemp464 = fRec117[1] - fConst90;
			fRec117[0] = ((fTemp463 < fTemp462) ? fTemp463 : ((fTemp464 > fTemp462) ? fTemp464 : fTemp462));
			double fTemp465 = fTemp142 * fTemp141 * (fRec117[0] + fTemp143);
			double fTemp466 = fTemp465 / fTemp140;
			fVec53[0] = fTemp466;
			double fTemp467 = GrungrFaustDSP_faustpower2_f(fVec53[2]);
			double fTemp468 = std::atan(fVec53[2]);
			double fTemp469 = GrungrFaustDSP_faustpower2_f(fVec53[1]);
			double fTemp470 = std::atan(fVec53[1]);
			double fTemp471 = fTemp470 - (fVec53[1] - fTemp470 * fTemp469);
			double fTemp472 = fVec53[1] * fTemp470;
			double fTemp473 = std::log(fTemp469 + 1.0);
			double fTemp474 = 2.0 * fVec53[1];
			double fTemp475 = GrungrFaustDSP_faustpower2_f(fTemp466 - fVec53[1]);
			double fTemp476 = GrungrFaustDSP_faustpower2_f(fTemp466);
			double fTemp477 = std::atan(fTemp466);
			double fTemp478 = fTemp140 * (fTemp144 - (((fTemp475 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp466 + fTemp474)) : (fTemp465 * (fTemp465 * fTemp477 / fTemp140 - (fTemp472 + 0.5 * (std::log(fTemp476 + 1.0) - fTemp473))) / fTemp140 - 0.5 * (fTemp477 - (fTemp466 - fTemp477 * fTemp476) - fTemp471)) / fTemp475) + ((GrungrFaustDSP_faustpower2_f(fVec53[1] - fVec53[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp474 + fVec53[2])) : (fVec53[2] * (fVec53[2] * fTemp468 + 0.5 * (fTemp473 - std::log(fTemp467 + 1.0)) - fTemp472) + 0.5 * (fTemp471 - (fTemp468 - (fVec53[2] - fTemp468 * fTemp467)))) / GrungrFaustDSP_faustpower2_f(fVec53[2] - fVec53[1]))));
			fVec54[0] = fTemp478;
			fRec116[0] = 0.995 * fRec116[1] - 0.6366197723675814 * (fTemp478 - fVec54[1]);
			fRec115[0] = fRec116[0] - (fTemp137 * fRec115[2] + 2.0 * fTemp136 * fRec115[1]) / fTemp135;
			double fTemp479 = (fRec115[2] + fRec115[0] + 2.0 * fRec115[1]) / fTemp135;
			double fTemp480 = fTemp479 + 0.05 * fTemp4 * tanh(0.4 * fTemp479);
			double fTemp481 = tanh(1.6 * fTemp480);
			fRec114[0] = tanh(1.4 * std::fabs(fTemp480 + 0.2 * fTemp7 * fTemp481)) - fConst67 * (fConst65 * fRec114[2] + fConst63 * fRec114[1]);
			double fTemp482 = fTemp480 + fTemp7 * (0.2 * fTemp481 + fConst91 * (fRec114[2] + fRec114[0] + 2.0 * fRec114[1]));
			fVec55[0] = fTemp482;
			fRec113[0] = -((fTemp243 * fRec113[1] - (fTemp482 - fVec55[1]) / fTemp129) / fTemp131);
			double fTemp483 = fRec113[0] * fTemp128;
			double fTemp484 = fConst90 + fRec112[1];
			double fTemp485 = fRec112[1] - fConst90;
			fRec112[0] = ((fTemp484 < fTemp483) ? fTemp484 : ((fTemp485 > fTemp483) ? fTemp485 : fTemp483));
			fRec111[0] = fConst1 * std::fabs(fRec112[0]) + fConst2 * fRec111[1];
			double fTemp486 = fRec112[0] * (1.0 - fTemp127 * std::min<double>(1.0, std::max<double>(0.0, fRec111[0] * fTemp11 / fTemp127)));
			fVec56[0] = fTemp486;
			fRec110[0] = -(fConst93 * (fConst92 * fRec110[1] - fConst58 * (fTemp486 - fVec56[1])));
			fRec109[0] = fRec110[0] - fConst61 * (fConst59 * fRec109[2] + fConst57 * fRec109[1]);
			fRec133[0] = -(fConst93 * (fConst92 * fRec133[1] - (fTemp486 + fVec56[1])));
			fRec132[0] = fRec133[0] - fConst61 * (fConst59 * fRec132[2] + fConst57 * fRec132[1]);
			double fTemp487 = fRec132[2] + fRec132[0] + 2.0 * fRec132[1] + fConst56 * fTemp126 * (fRec109[2] + (fRec109[0] - 2.0 * fRec109[1]));
			fVec57[0] = fTemp487;
			fRec108[0] = -(fConst96 * (fConst95 * fRec108[1] - fConst94 * (fTemp487 - fVec57[1])));
			fRec107[0] = fRec108[0] - fConst54 * (fConst52 * fRec107[2] + fConst50 * fRec107[1]);
			fRec135[0] = -(fConst96 * (fConst95 * fRec135[1] - fConst61 * (fTemp487 + fVec57[1])));
			fRec134[0] = fRec135[0] - fConst54 * (fConst52 * fRec134[2] + fConst50 * fRec134[1]);
			double fTemp488 = fRec134[2] + fRec134[0] + 2.0 * fRec134[1] + fConst49 * fTemp126 * (fRec107[2] + (fRec107[0] - 2.0 * fRec107[1]));
			int iTemp489 = (fConst54 * fTemp488) > 0.0;
			double fTemp490 = fTemp488 / fTemp251;
			fVec58[0] = fTemp490;
			double fTemp491 = fConst54 * (fVec58[1] - fVec58[2]);
			double fTemp492 = fConst54 * fVec58[2];
			int iTemp493 = (fTemp492 <= 1.0) & (fTemp492 >= -1.0);
			double fTemp494 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp492);
			double fTemp495 = static_cast<double>((fTemp492 > 0.0) - (fTemp492 < 0.0));
			double fTemp496 = fConst54 * fVec58[1];
			int iTemp497 = (fTemp496 <= 1.0) & (fTemp496 >= -1.0);
			double fTemp498 = static_cast<double>((fTemp496 > 0.0) - (fTemp496 < 0.0));
			double fTemp499 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp496);
			double fTemp500 = ((iTemp497) ? fTemp499 : fConst54 * fVec58[1] * fTemp498 + -0.5);
			double fTemp501 = ((iTemp497) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp496) : fTemp498 * (fTemp499 + -0.16666666666666666));
			double fTemp502 = 2.0 * fVec58[1];
			double fTemp503 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp490 - fVec58[1]));
			double fTemp504 = fConst54 * fTemp490;
			int iTemp505 = (fTemp504 <= 1.0) & (fTemp504 >= -1.0);
			double fTemp506 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp504);
			double fTemp507 = static_cast<double>((fTemp504 > 0.0) - (fTemp504 < 0.0));
			double fTemp508 = fTemp488 / fTemp270;
			fVec59[0] = fTemp508;
			double fTemp509 = fConst54 * (fVec59[1] - fVec59[2]);
			double fTemp510 = fConst54 * fVec59[2];
			int iTemp511 = (fTemp510 <= 1.0) & (fTemp510 >= -1.0);
			double fTemp512 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp510);
			double fTemp513 = static_cast<double>((fTemp510 > 0.0) - (fTemp510 < 0.0));
			double fTemp514 = fConst54 * fVec59[1];
			int iTemp515 = (fTemp514 <= 1.0) & (fTemp514 >= -1.0);
			double fTemp516 = static_cast<double>((fTemp514 > 0.0) - (fTemp514 < 0.0));
			double fTemp517 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp514);
			double fTemp518 = ((iTemp515) ? fTemp517 : fConst54 * fVec59[1] * fTemp516 + -0.5);
			double fTemp519 = ((iTemp515) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp514) : fTemp516 * (fTemp517 + -0.16666666666666666));
			double fTemp520 = 2.0 * fVec59[1];
			double fTemp521 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp508 - fVec59[1]));
			double fTemp522 = fConst54 * fTemp508;
			int iTemp523 = (fTemp522 <= 1.0) & (fTemp522 >= -1.0);
			double fTemp524 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp522);
			double fTemp525 = static_cast<double>((fTemp522 > 0.0) - (fTemp522 < 0.0));
			double fTemp526 = fTemp488 / fTemp290;
			fVec60[0] = fTemp526;
			double fTemp527 = fConst54 * (fVec60[1] - fVec60[2]);
			double fTemp528 = fConst54 * fVec60[2];
			int iTemp529 = (fTemp528 <= 1.0) & (fTemp528 >= -1.0);
			double fTemp530 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp528);
			double fTemp531 = static_cast<double>((fTemp528 > 0.0) - (fTemp528 < 0.0));
			double fTemp532 = fConst54 * fVec60[1];
			int iTemp533 = (fTemp532 <= 1.0) & (fTemp532 >= -1.0);
			double fTemp534 = static_cast<double>((fTemp532 > 0.0) - (fTemp532 < 0.0));
			double fTemp535 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp532);
			double fTemp536 = ((iTemp533) ? fTemp535 : fConst54 * fVec60[1] * fTemp534 + -0.5);
			double fTemp537 = ((iTemp533) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp532) : fTemp534 * (fTemp535 + -0.16666666666666666));
			double fTemp538 = 2.0 * fVec60[1];
			double fTemp539 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp526 - fVec60[1]));
			double fTemp540 = fConst54 * fTemp526;
			int iTemp541 = (fTemp540 <= 1.0) & (fTemp540 >= -1.0);
			double fTemp542 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp540);
			double fTemp543 = static_cast<double>((fTemp540 > 0.0) - (fTemp540 < 0.0));
			double fTemp544 = fTemp488 / fTemp309;
			fVec61[0] = fTemp544;
			double fTemp545 = fConst54 * (fVec61[1] - fVec61[2]);
			double fTemp546 = fConst54 * fVec61[2];
			int iTemp547 = (fTemp546 <= 1.0) & (fTemp546 >= -1.0);
			double fTemp548 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp546);
			double fTemp549 = static_cast<double>((fTemp546 > 0.0) - (fTemp546 < 0.0));
			double fTemp550 = fConst54 * fVec61[1];
			int iTemp551 = (fTemp550 <= 1.0) & (fTemp550 >= -1.0);
			double fTemp552 = static_cast<double>((fTemp550 > 0.0) - (fTemp550 < 0.0));
			double fTemp553 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp550);
			double fTemp554 = ((iTemp551) ? fTemp553 : fConst54 * fVec61[1] * fTemp552 + -0.5);
			double fTemp555 = ((iTemp551) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp550) : fTemp552 * (fTemp553 + -0.16666666666666666));
			double fTemp556 = 2.0 * fVec61[1];
			double fTemp557 = GrungrFaustDSP_faustpower2_f(fConst54 * (fTemp544 - fVec61[1]));
			double fTemp558 = fConst54 * fTemp544;
			int iTemp559 = (fTemp558 <= 1.0) & (fTemp558 >= -1.0);
			double fTemp560 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp558);
			double fTemp561 = static_cast<double>((fTemp558 > 0.0) - (fTemp558 < 0.0));
			double fTemp562 = fTemp328 * ((iTemp489) ? fTemp309 * (((fTemp557 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp544 + fTemp556))) : (fConst54 * (fTemp488 * (((iTemp559) ? fTemp560 : fConst54 * (fTemp488 * fTemp561 / fTemp309) + -0.5) - fTemp554) / fTemp309) + fTemp555 - ((iTemp559) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp558) : fTemp561 * (fTemp560 + -0.16666666666666666))) / fTemp557) + ((GrungrFaustDSP_faustpower2_f(fTemp545) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp556 + fVec61[2]))) : (fTemp555 + fConst54 * fVec61[2] * (((iTemp547) ? fTemp548 : fConst54 * fVec61[2] * fTemp549 + -0.5) - fTemp554) - ((iTemp547) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp546) : fTemp549 * (fTemp548 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp545))) : fTemp290 * (((fTemp539 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp526 + fTemp538))) : (fConst54 * (fTemp488 * (((iTemp541) ? fTemp542 : fConst54 * (fTemp488 * fTemp543 / fTemp290) + -0.5) - fTemp536) / fTemp290) + fTemp537 - ((iTemp541) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp540) : fTemp543 * (fTemp542 + -0.16666666666666666))) / fTemp539) + ((GrungrFaustDSP_faustpower2_f(fTemp527) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp538 + fVec60[2]))) : (fTemp537 + fConst54 * fVec60[2] * (((iTemp529) ? fTemp530 : fConst54 * fVec60[2] * fTemp531 + -0.5) - fTemp536) - ((iTemp529) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp528) : fTemp531 * (fTemp530 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp527)))) + fTemp289 * ((iTemp489) ? fTemp270 * (((fTemp521 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp508 + fTemp520))) : (fConst54 * (fTemp488 * (((iTemp523) ? fTemp524 : fConst54 * (fTemp488 * fTemp525 / fTemp270) + -0.5) - fTemp518) / fTemp270) + fTemp519 - ((iTemp523) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp522) : fTemp525 * (fTemp524 + -0.16666666666666666))) / fTemp521) + ((GrungrFaustDSP_faustpower2_f(fTemp509) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp520 + fVec59[2]))) : (fTemp519 + fConst54 * fVec59[2] * (((iTemp511) ? fTemp512 : fConst54 * fVec59[2] * fTemp513 + -0.5) - fTemp518) - ((iTemp511) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp510) : fTemp513 * (fTemp512 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp509))) : fTemp251 * (((fTemp503 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp490 + fTemp502))) : (fConst54 * (fTemp488 * (((iTemp505) ? fTemp506 : fConst54 * (fTemp488 * fTemp507 / fTemp251) + -0.5) - fTemp500) / fTemp251) + fTemp501 - ((iTemp505) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp504) : fTemp507 * (fTemp506 + -0.16666666666666666))) / fTemp503) + ((GrungrFaustDSP_faustpower2_f(fTemp491) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst97 * (fTemp502 + fVec58[2]))) : (fTemp501 + fConst54 * fVec58[2] * (((iTemp493) ? fTemp494 : fConst54 * fVec58[2] * fTemp495 + -0.5) - fTemp500) - ((iTemp493) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp492) : fTemp495 * (fTemp494 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp491))));
			fVec62[0] = fTemp562;
			fRec106[0] = -(fConst93 * (fConst92 * fRec106[1] - fConst58 * (fTemp562 - fVec62[1])));
			fRec105[0] = fRec106[0] - fConst61 * (fConst59 * fRec105[2] + fConst57 * fRec105[1]);
			fRec137[0] = -(fConst93 * (fConst92 * fRec137[1] - (fTemp562 + fVec62[1])));
			fRec136[0] = fRec137[0] - fConst61 * (fConst59 * fRec136[2] + fConst57 * fRec136[1]);
			double fTemp563 = fRec136[2] + fRec136[0] + 2.0 * fRec136[1] + fConst56 * fTemp125 * (fRec105[2] + (fRec105[0] - 2.0 * fRec105[1]));
			fVec63[0] = fTemp563;
			fRec104[0] = -(fConst96 * (fConst95 * fRec104[1] - fConst94 * (fTemp563 - fVec63[1])));
			fRec103[0] = fRec104[0] - fConst54 * (fConst52 * fRec103[2] + fConst50 * fRec103[1]);
			fRec139[0] = -(fConst96 * (fConst95 * fRec139[1] - fConst61 * (fTemp563 + fVec63[1])));
			fRec138[0] = fRec139[0] - fConst54 * (fConst52 * fRec138[2] + fConst50 * fRec138[1]);
			fRec102[0] = fConst54 * (fRec138[2] + fRec138[0] + 2.0 * fRec138[1] + fConst49 * fTemp125 * (fRec103[2] + (fRec103[0] - 2.0 * fRec103[1]))) - (fTemp123 * fRec102[2] + 2.0 * fTemp122 * fRec102[1]) / fTemp121;
			double fTemp564 = (fRec102[2] + fRec102[0] + 2.0 * fRec102[1]) / fTemp121;
			fVec64[0] = fTemp564;
			fRec101[0] = 0.995 * fRec101[1] + fTemp564 - fVec64[1];
			fRec100[0] = -(fConst99 * (fConst98 * fRec100[1] - fConst44 * (fRec101[0] - fRec101[1])));
			fRec99[0] = fRec100[0] - fConst47 * (fConst45 * fRec99[2] + fConst43 * fRec99[1]);
			fRec141[0] = -(fConst99 * (fConst98 * fRec141[1] - (fRec101[0] + fRec101[1])));
			fRec140[0] = fRec141[0] - fConst47 * (fConst45 * fRec140[2] + fConst43 * fRec140[1]);
			double fTemp565 = fTemp332 * (fRec140[2] + fRec140[0] + 2.0 * fRec140[1]) + fConst42 * (fRec99[2] + (fRec99[0] - 2.0 * fRec99[1]));
			fVec65[0] = fTemp565;
			fRec98[0] = -(fConst102 * (fConst101 * fRec98[1] - fConst100 * (fTemp565 - fVec65[1])));
			fRec97[0] = fRec98[0] - fConst40 * (fConst39 * fRec97[2] + fConst37 * fRec97[1]);
			fRec143[0] = -(fConst102 * (fConst101 * fRec143[1] - fConst47 * (fTemp565 + fVec65[1])));
			fRec142[0] = fRec143[0] - fConst40 * (fConst39 * fRec142[2] + fConst37 * fRec142[1]);
			fRec96[0] = fConst40 * (fRec142[2] + fRec142[0] + 2.0 * fRec142[1] + fConst36 * fTemp118 * (fRec97[2] + (fRec97[0] - 2.0 * fRec97[1]))) - (fTemp117 * fRec96[2] + fTemp418) / fTemp113;
			fRec95[0] = (fTemp418 + fRec96[0] * fTemp334 + fTemp115 * fRec96[2]) / fTemp113 - (fTemp108 * fRec95[2] + fTemp417) / fTemp103;
			fRec94[0] = (fTemp417 + fRec95[0] * fTemp335 + fTemp105 * fRec95[2]) / fTemp103 - (fTemp93 * fRec94[2] + fTemp416) / fTemp89;
			fRec93[0] = (fTemp416 + fRec94[0] * fTemp336 + fTemp91 * fRec94[2]) / fTemp89 - (fTemp83 * fRec93[2] + fTemp415) / fTemp79;
			fRec92[0] = (fTemp415 + fRec93[0] * fTemp337 + fTemp81 * fRec93[2]) / fTemp79 - (fTemp74 * fRec92[2] + fTemp414) / fTemp70;
			fRec91[0] = (fTemp414 + fRec92[0] * fTemp338 + fTemp72 * fRec92[2]) / fTemp70 - (fTemp65 * fRec91[2] + fTemp413) / fTemp61;
			double fTemp566 = (fTemp413 + fRec91[0] * fTemp339 + fTemp63 * fRec91[2]) / fTemp61;
			fVec66[0] = fTemp566;
			fRec90[0] = fConst104 * (fConst103 * (fTemp566 - fVec66[1]) - fConst14 * fRec90[1]);
			fRec89[0] = -((fTemp341 * fRec89[1] - (fRec90[0] - fRec90[1]) / fTemp47) / fTemp56);
			fRec88[0] = fRec89[0] - (fTemp55 * fRec88[2] + 2.0 * fTemp54 * fRec88[1]) / fTemp49;
			fRec145[0] = -((fTemp341 * fRec145[1] - (fRec90[0] + fRec90[1])) / fTemp56);
			fRec144[0] = fRec145[0] - (fTemp55 * fRec144[2] + 2.0 * fTemp54 * fRec144[1]) / fTemp49;
			fRec87[0] = (fRec144[2] + fRec144[0] + 2.0 * fRec144[1] + fTemp53 * (fRec88[2] + (fRec88[0] - 2.0 * fRec88[1])) / fTemp50) / fTemp49 - (fTemp46 * fRec87[2] + fTemp412) / fTemp42;
			fRec86[0] = (fTemp412 + fRec87[0] * fTemp342 + fTemp44 * fRec87[2]) / fTemp42 - (fTemp37 * fRec86[2] + 2.0 * fTemp36 * fRec86[1]) / fTemp35;
			fRec85[0] = (fRec86[2] + fRec86[0] + 2.0 * fRec86[1]) / fTemp35 - (fTemp32 * fRec85[2] + 2.0 * fTemp31 * fRec85[1]) / fTemp30;
			double fTemp567 = (fRec85[2] + fRec85[0] + 2.0 * fRec85[1]) / fTemp30;
			fRec84[0] = ((iTemp0) ? 0.0 : fTemp567) - (fTemp23 * fRec84[2] + 2.0 * fTemp22 * fRec84[1]) / fTemp21;
			double fTemp568 = fTemp344 * fTemp18 * ((iTemp0) ? fTemp567 : (fRec84[2] + fRec84[0] + 2.0 * fRec84[1]) / fTemp21);
			double fTemp569 = fConst90 + fRec83[1];
			double fTemp570 = fRec83[1] - fConst90;
			fRec83[0] = ((fTemp569 < fTemp568) ? fTemp569 : ((fTemp570 > fTemp568) ? fTemp570 : fTemp568));
			fRec82[0] = fConst1 * std::fabs(fRec83[0]) + fConst2 * fRec82[1];
			double fTemp571 = fRec83[0] * fTemp6 * (1.0 - fTemp9 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec82[0] * fTemp11 / fTemp9))));
			fVec67[0] = fTemp571;
			double fTemp572 = fVec67[1] - fVec67[2];
			double fTemp573 = 0.8928571428571428 * fTemp572;
			double fTemp574 = 0.8928571428571428 * fVec67[2];
			double fTemp575 = GrungrFaustDSP_faustpower2_f(fTemp574) + 1.0;
			double fTemp576 = std::atan(fTemp574);
			double fTemp577 = 0.8928571428571428 * fVec67[1];
			double fTemp578 = GrungrFaustDSP_faustpower2_f(fTemp577) + 1.0;
			double fTemp579 = std::atan(fTemp577);
			double fTemp580 = fTemp579 * fTemp578;
			double fTemp581 = fVec67[1] * fTemp579;
			double fTemp582 = std::log(fTemp578);
			double fTemp583 = 1.7857142857142856 * fVec67[1];
			double fTemp584 = fTemp571 - fVec67[1];
			double fTemp585 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp584);
			double fTemp586 = 0.8928571428571428 * fTemp571;
			double fTemp587 = GrungrFaustDSP_faustpower2_f(fTemp586) + 1.0;
			double fTemp588 = std::atan(fTemp586);
			double fTemp589 = 0.7130141450516913 * (((fTemp585 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp586 + fTemp583)) : (0.8928571428571428 * fTemp571 * (0.8928571428571428 * (fTemp571 * fTemp588 - fTemp581) - 0.5 * (std::log(fTemp587) - fTemp582)) + 0.4464285714285714 * fTemp584 - 0.5 * (fTemp588 * fTemp587 - fTemp580)) / fTemp585) + ((GrungrFaustDSP_faustpower2_f(fTemp573) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp583 + fTemp574)) : (0.8928571428571428 * fVec67[2] * (0.5 * (fTemp582 - std::log(fTemp575)) - 0.8928571428571428 * (fTemp581 - fVec67[2] * fTemp576)) + 0.5 * (fTemp580 - fTemp576 * fTemp575) - 0.4464285714285714 * fTemp572) / GrungrFaustDSP_faustpower2_f(-fTemp573)));
			double fTemp590 = ((iTemp1) ? 0.0 : fTemp589);
			fVec68[IOTA0 & 1023] = fTemp590;
			double fTemp591 = std::fabs(fTemp590);
			int iTemp592 = (fTemp591 >= fRec149[1]) | (static_cast<double>(iRec148[1]) >= fConst5);
			iRec148[0] = ((iTemp592) ? 0 : iRec148[1] + 1);
			fRec149[0] = ((iTemp592) ? fTemp591 : fRec149[1]);
			fRec147[0] = fConst83 * fRec149[0] + fConst77 * fRec147[1];
			double fTemp593 = std::fabs(fRec147[0]);
			fRec146[0] = std::max<double>(fTemp593, fConst81 * fRec146[1] + fConst82 * fTemp593);
			double fTemp594 = ((iTemp1) ? fTemp589 : std::min<double>(1.0, 0.944 / std::max<double>(fRec146[0], 2.220446049250313e-16)) * fVec68[(IOTA0 - iConst106) & 1023]);
			double fTemp595 = ((iTemp1) ? 0.0 : fTemp594);
			double fTemp596 = std::fabs(fTemp595);
			fVec69[0] = fTemp596;
			double fTemp597 = std::max<double>(fTemp596, fVec69[1]);
			fVec70[0] = fTemp597;
			double fTemp598 = std::max<double>(fTemp597, fVec70[2]);
			fVec71[0] = fTemp598;
			double fTemp599 = std::max<double>(fTemp598, fVec71[4]);
			fVec72[0] = fTemp599;
			double fTemp600 = std::max<double>(fTemp599, fVec72[8]);
			fVec73[IOTA0 & 31] = fTemp600;
			double fTemp601 = std::max<double>(fTemp600, fVec73[(IOTA0 - 16) & 31]);
			fVec74[IOTA0 & 63] = fTemp601;
			double fTemp602 = std::max<double>(fTemp601, fVec74[(IOTA0 - 32) & 63]);
			fVec75[IOTA0 & 127] = fTemp602;
			double fTemp603 = std::max<double>(fTemp602, fVec75[(IOTA0 - 64) & 127]);
			fVec76[IOTA0 & 255] = fTemp603;
			double fTemp604 = std::max<double>(fTemp603, fVec76[(IOTA0 - 128) & 255]);
			fVec77[IOTA0 & 511] = fTemp604;
			double fTemp605 = std::max<double>(fTemp604, fVec77[(IOTA0 - 256) & 511]);
			fVec78[IOTA0 & 1023] = fTemp605;
			fVec79[IOTA0 & 2047] = std::max<double>(fTemp605, fVec78[(IOTA0 - 512) & 1023]);
			double fTemp606 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst6) ? fTemp596 : -1.7976931348623157e+308), ((iConst107) ? fVec70[iConst6] : -1.7976931348623157e+308)), ((iConst108) ? fVec71[iConst109] : -1.7976931348623157e+308)), ((iConst110) ? fVec72[iConst111] : -1.7976931348623157e+308)), ((iConst112) ? fVec73[(IOTA0 - iConst113) & 31] : -1.7976931348623157e+308)), ((iConst114) ? fVec74[(IOTA0 - iConst115) & 63] : -1.7976931348623157e+308)), ((iConst116) ? fVec75[(IOTA0 - iConst117) & 127] : -1.7976931348623157e+308)), ((iConst118) ? fVec76[(IOTA0 - iConst119) & 255] : -1.7976931348623157e+308)), ((iConst120) ? fVec77[(IOTA0 - iConst121) & 511] : -1.7976931348623157e+308)), ((iConst122) ? fVec78[(IOTA0 - iConst123) & 1023] : -1.7976931348623157e+308)), ((iConst124) ? fVec79[(IOTA0 - iConst125) & 2047] : -1.7976931348623157e+308));
			double fTemp607 = ((fTemp606 > fRec81[1]) ? fConst71 : fConst85);
			fRec81[0] = fTemp606 * (1.0 - fTemp607) + fRec81[1] * fTemp607;
			double fTemp608 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec81[0]));
			int iTemp609 = (fTemp608 > -53.0) + (fTemp608 > -47.0);
			double fTemp610 = ((iTemp1) ? fTemp594 : fTemp595 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp609 == 0) ? fTemp608 + 5e+01 : ((iTemp609 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp608 + 47.0)) : 0.0)))));
			double fTemp611 = ((iTemp1) ? 0.0 : fTemp610);
			double fTemp612 = ((iTemp1) ? fTemp610 : static_cast<double>((fTemp611 > 0.0) - (fTemp611 < 0.0)) * std::pow(std::fabs(fTemp611), 0.97));
			double fTemp613 = ((iTemp0) ? 0.0 : fTemp612);
			fVec80[IOTA0 & 2047] = fTemp613;
			double fTemp614 = std::fabs(fTemp613);
			int iTemp615 = (fTemp614 >= fRec153[1]) | (static_cast<double>(iRec152[1]) >= fConst3);
			iRec152[0] = ((iTemp615) ? 0 : iRec152[1] + 1);
			fRec153[0] = ((iTemp615) ? fTemp614 : fRec153[1]);
			fRec151[0] = fConst87 * fRec153[0] + fConst71 * fRec151[1];
			double fTemp616 = std::fabs(fRec151[0]);
			fRec150[0] = std::max<double>(fTemp616, fConst85 * fRec150[1] + fConst86 * fTemp616);
			double fTemp617 = ((iTemp0) ? fTemp612 : std::min<double>(1.0, 0.944 / std::max<double>(fRec150[0], 2.220446049250313e-16)) * fVec80[(IOTA0 - iConst126) & 2047]);
			double fTemp618 = ((iTemp0) ? 0.0 : fTemp617);
			double fTemp619 = std::fabs(fTemp618);
			fVec81[0] = fTemp619;
			double fTemp620 = std::max<double>(fTemp619, fVec81[1]);
			fVec82[0] = fTemp620;
			double fTemp621 = std::max<double>(fTemp620, fVec82[2]);
			fVec83[0] = fTemp621;
			double fTemp622 = std::max<double>(fTemp621, fVec83[4]);
			fVec84[0] = fTemp622;
			double fTemp623 = std::max<double>(fTemp622, fVec84[8]);
			fVec85[IOTA0 & 31] = fTemp623;
			double fTemp624 = std::max<double>(fTemp623, fVec85[(IOTA0 - 16) & 31]);
			fVec86[IOTA0 & 63] = fTemp624;
			double fTemp625 = std::max<double>(fTemp624, fVec86[(IOTA0 - 32) & 63]);
			fVec87[IOTA0 & 127] = fTemp625;
			double fTemp626 = std::max<double>(fTemp625, fVec87[(IOTA0 - 64) & 127]);
			fVec88[IOTA0 & 255] = fTemp626;
			double fTemp627 = std::max<double>(fTemp626, fVec88[(IOTA0 - 128) & 255]);
			fVec89[IOTA0 & 511] = fTemp627;
			double fTemp628 = std::max<double>(fTemp627, fVec89[(IOTA0 - 256) & 511]);
			fVec90[IOTA0 & 1023] = fTemp628;
			fVec91[IOTA0 & 2047] = std::max<double>(fTemp628, fVec90[(IOTA0 - 512) & 1023]);
			double fTemp629 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp619 : -1.7976931348623157e+308), ((iConst6) ? fVec82[iConst4] : -1.7976931348623157e+308)), ((iConst107) ? fVec83[iConst127] : -1.7976931348623157e+308)), ((iConst108) ? fVec84[iConst128] : -1.7976931348623157e+308)), ((iConst110) ? fVec85[(IOTA0 - iConst129) & 31] : -1.7976931348623157e+308)), ((iConst112) ? fVec86[(IOTA0 - iConst130) & 63] : -1.7976931348623157e+308)), ((iConst114) ? fVec87[(IOTA0 - iConst131) & 127] : -1.7976931348623157e+308)), ((iConst116) ? fVec88[(IOTA0 - iConst132) & 255] : -1.7976931348623157e+308)), ((iConst118) ? fVec89[(IOTA0 - iConst133) & 511] : -1.7976931348623157e+308)), ((iConst120) ? fVec90[(IOTA0 - iConst134) & 1023] : -1.7976931348623157e+308)), ((iConst122) ? fVec91[(IOTA0 - iConst135) & 2047] : -1.7976931348623157e+308));
			double fTemp630 = ((fTemp629 > fRec80[1]) ? fConst71 : fConst136);
			fRec80[0] = fTemp629 * (1.0 - fTemp630) + fRec80[1] * fTemp630;
			double fTemp631 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec80[0]));
			int iTemp632 = (fTemp631 > -58.0) + (fTemp631 > -52.0);
			double fTemp633 = ((iTemp0) ? fTemp617 : fTemp618 * std::pow(1e+01, 0.05 * std::max<double>(-35.0, 4.0 * ((iTemp632 == 0) ? fTemp631 + 55.0 : ((iTemp632 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp631 + 52.0)) : 0.0)))));
			double fTemp634 = ((iTemp0) ? 0.0 : fTemp633);
			output1[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp419 : ((iTemp0) ? fTemp633 : static_cast<double>((fTemp634 > 0.0) - (fTemp634 < 0.0)) * std::pow(std::fabs(fTemp634), 0.97))));
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			fRec8[1] = fRec8[0];
			fVec0[1] = fVec0[0];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fVec1[1] = fVec1[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fVec6[1] = fVec6[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fVec9[1] = fVec9[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fVec10[1] = fVec10[0];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fVec11[1] = fVec11[0];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fVec13[2] = fVec13[1];
			fVec13[1] = fVec13[0];
			fVec14[2] = fVec14[1];
			fVec14[1] = fVec14[0];
			fVec15[2] = fVec15[1];
			fVec15[1] = fVec15[0];
			fVec16[1] = fVec16[0];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec61[1] = fRec61[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fVec17[1] = fVec17[0];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fVec18[1] = fVec18[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec64[1] = fRec64[0];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fVec19[1] = fVec19[0];
			fRec22[1] = fRec22[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec68[1] = fRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec20[1] = fVec20[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec71[1] = fRec71[0];
			fRec6[1] = fRec6[0];
			fRec5[1] = fRec5[0];
			fVec21[2] = fVec21[1];
			fVec21[1] = fVec21[0];
			IOTA0 = IOTA0 + 1;
			iRec74[1] = iRec74[0];
			fRec75[1] = fRec75[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fVec23[1] = fVec23[0];
			fVec24[2] = fVec24[1];
			fVec24[1] = fVec24[0];
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fVec25[j0] = fVec25[j0 - 1];
			}
			for (int j1 = 11; j1 > 0; j1 = j1 - 1) {
				fVec26[j1] = fVec26[j1 - 1];
			}
			fRec3[1] = fRec3[0];
			iRec78[1] = iRec78[0];
			fRec79[1] = fRec79[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fVec35[1] = fVec35[0];
			fVec36[2] = fVec36[1];
			fVec36[1] = fVec36[0];
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fVec37[j2] = fVec37[j2 - 1];
			}
			for (int j3 = 11; j3 > 0; j3 = j3 - 1) {
				fVec38[j3] = fVec38[j3 - 1];
			}
			fRec1[1] = fRec1[0];
			fVec46[1] = fVec46[0];
			fRec131[1] = fRec131[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
			fVec47[1] = fVec47[0];
			fRec129[1] = fRec129[0];
			fRec128[1] = fRec128[0];
			fRec127[1] = fRec127[0];
			fRec126[2] = fRec126[1];
			fRec126[1] = fRec126[0];
			fRec125[1] = fRec125[0];
			fRec124[1] = fRec124[0];
			fVec48[2] = fVec48[1];
			fVec48[1] = fVec48[0];
			fVec49[1] = fVec49[0];
			fRec123[1] = fRec123[0];
			fRec122[1] = fRec122[0];
			fVec50[2] = fVec50[1];
			fVec50[1] = fVec50[0];
			fVec51[1] = fVec51[0];
			fRec121[1] = fRec121[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fVec52[1] = fVec52[0];
			fRec119[1] = fRec119[0];
			fRec118[1] = fRec118[0];
			fRec117[1] = fRec117[0];
			fVec53[2] = fVec53[1];
			fVec53[1] = fVec53[0];
			fVec54[1] = fVec54[0];
			fRec116[1] = fRec116[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fVec55[1] = fVec55[0];
			fRec113[1] = fRec113[0];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fVec56[1] = fVec56[0];
			fRec110[1] = fRec110[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec133[1] = fRec133[0];
			fRec132[2] = fRec132[1];
			fRec132[1] = fRec132[0];
			fVec57[1] = fVec57[0];
			fRec108[1] = fRec108[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec135[1] = fRec135[0];
			fRec134[2] = fRec134[1];
			fRec134[1] = fRec134[0];
			fVec58[2] = fVec58[1];
			fVec58[1] = fVec58[0];
			fVec59[2] = fVec59[1];
			fVec59[1] = fVec59[0];
			fVec60[2] = fVec60[1];
			fVec60[1] = fVec60[0];
			fVec61[2] = fVec61[1];
			fVec61[1] = fVec61[0];
			fVec62[1] = fVec62[0];
			fRec106[1] = fRec106[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fRec137[1] = fRec137[0];
			fRec136[2] = fRec136[1];
			fRec136[1] = fRec136[0];
			fVec63[1] = fVec63[0];
			fRec104[1] = fRec104[0];
			fRec103[2] = fRec103[1];
			fRec103[1] = fRec103[0];
			fRec139[1] = fRec139[0];
			fRec138[2] = fRec138[1];
			fRec138[1] = fRec138[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fVec64[1] = fVec64[0];
			fRec101[1] = fRec101[0];
			fRec100[1] = fRec100[0];
			fRec99[2] = fRec99[1];
			fRec99[1] = fRec99[0];
			fRec141[1] = fRec141[0];
			fRec140[2] = fRec140[1];
			fRec140[1] = fRec140[0];
			fVec65[1] = fVec65[0];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec143[1] = fRec143[0];
			fRec142[2] = fRec142[1];
			fRec142[1] = fRec142[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fVec66[1] = fVec66[0];
			fRec90[1] = fRec90[0];
			fRec89[1] = fRec89[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec145[1] = fRec145[0];
			fRec144[2] = fRec144[1];
			fRec144[1] = fRec144[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fVec67[2] = fVec67[1];
			fVec67[1] = fVec67[0];
			iRec148[1] = iRec148[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec146[1] = fRec146[0];
			fVec69[1] = fVec69[0];
			fVec70[2] = fVec70[1];
			fVec70[1] = fVec70[0];
			for (int j4 = 4; j4 > 0; j4 = j4 - 1) {
				fVec71[j4] = fVec71[j4 - 1];
			}
			for (int j5 = 11; j5 > 0; j5 = j5 - 1) {
				fVec72[j5] = fVec72[j5 - 1];
			}
			fRec81[1] = fRec81[0];
			iRec152[1] = iRec152[0];
			fRec153[1] = fRec153[0];
			fRec151[1] = fRec151[0];
			fRec150[1] = fRec150[0];
			fVec81[1] = fVec81[0];
			fVec82[2] = fVec82[1];
			fVec82[1] = fVec82[0];
			for (int j6 = 4; j6 > 0; j6 = j6 - 1) {
				fVec83[j6] = fVec83[j6 - 1];
			}
			for (int j7 = 11; j7 > 0; j7 = j7 - 1) {
				fVec84[j7] = fVec84[j7 - 1];
			}
			fRec80[1] = fRec80[0];
		}
	}

};

#endif
