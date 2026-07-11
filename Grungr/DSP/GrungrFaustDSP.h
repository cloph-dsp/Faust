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
	FAUSTFLOAT fHslider1;
	double fRec2[2];
	double fRec3[2];
	FAUSTFLOAT fHslider2;
	double fRec6[2];
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
	double fVec0[2];
	double fRec47[2];
	double fRec46[3];
	double fVec1[2];
	double fConst58;
	double fConst59;
	double fConst60;
	double fRec45[2];
	double fConst61;
	double fConst62;
	double fRec44[2];
	double fConst63;
	double fRec43[2];
	double fVec2[3];
	double fVec3[2];
	double fConst64;
	double fConst65;
	double fRec42[2];
	double fConst66;
	double fRec41[2];
	double fVec4[3];
	double fVec5[2];
	double fRec40[2];
	double fRec39[3];
	double fVec6[2];
	double fRec38[2];
	double fRec37[2];
	double fRec36[2];
	double fVec7[3];
	double fVec8[2];
	double fRec35[2];
	double fRec34[3];
	double fVec9[2];
	double fRec33[2];
	double fRec32[2];
	double fRec31[2];
	double fVec10[2];
	double fConst67;
	double fConst68;
	double fRec30[2];
	double fRec29[3];
	double fRec49[2];
	double fRec48[3];
	double fVec11[2];
	double fConst69;
	double fConst70;
	double fConst71;
	double fRec28[2];
	double fRec27[3];
	double fRec51[2];
	double fRec50[3];
	double fVec12[3];
	double fConst72;
	double fVec13[3];
	double fVec14[3];
	double fVec15[3];
	double fVec16[2];
	double fRec26[2];
	double fRec25[3];
	double fRec53[2];
	double fRec52[3];
	double fVec17[2];
	double fRec24[2];
	double fRec23[3];
	double fRec55[2];
	double fRec54[3];
	double fRec22[3];
	double fVec18[2];
	double fRec21[2];
	double fConst73;
	double fConst74;
	double fRec20[2];
	double fRec19[3];
	FAUSTFLOAT fHslider3;
	double fRec56[2];
	double fRec58[2];
	double fRec57[3];
	double fVec19[2];
	double fConst75;
	double fConst76;
	double fConst77;
	double fRec18[2];
	double fRec17[3];
	double fRec60[2];
	double fRec59[3];
	double fRec16[3];
	double fRec15[3];
	double fRec14[3];
	double fRec13[3];
	double fVec20[2];
	double fConst78;
	double fConst79;
	double fRec12[2];
	double fRec11[2];
	double fRec10[3];
	double fRec62[2];
	double fRec61[3];
	double fRec9[3];
	double fRec8[3];
	double fRec7[3];
	FAUSTFLOAT fHslider4;
	double fRec63[2];
	double fRec5[2];
	double fRec4[2];
	double fVec21[3];
	int IOTA0;
	double fVec22[1024];
	double fConst80;
	int iConst81;
	int iRec66[2];
	double fRec67[2];
	double fRec65[2];
	double fRec64[2];
	double fVec23[2];
	int iConst82;
	double fVec24[3];
	int iConst83;
	double fVec25[5];
	int iConst84;
	int iConst85;
	double fVec26[12];
	int iConst86;
	int iConst87;
	double fVec27[32];
	int iConst88;
	int iConst89;
	double fVec28[64];
	int iConst90;
	int iConst91;
	double fVec29[128];
	int iConst92;
	int iConst93;
	double fVec30[256];
	int iConst94;
	int iConst95;
	double fVec31[512];
	int iConst96;
	int iConst97;
	double fVec32[1024];
	int iConst98;
	int iConst99;
	double fVec33[2048];
	int iConst100;
	double fConst101;
	double fConst102;
	double fRec1[2];
	double fVec34[2];
	double fRec111[2];
	double fRec110[3];
	double fVec35[2];
	double fRec109[2];
	double fRec108[2];
	double fRec107[2];
	double fVec36[3];
	double fVec37[2];
	double fRec106[2];
	double fRec105[2];
	double fVec38[3];
	double fVec39[2];
	double fRec104[2];
	double fRec103[3];
	double fVec40[2];
	double fRec102[2];
	double fRec101[2];
	double fRec100[2];
	double fVec41[3];
	double fVec42[2];
	double fRec99[2];
	double fRec98[3];
	double fVec43[2];
	double fRec97[2];
	double fRec96[2];
	double fRec95[2];
	double fVec44[2];
	double fRec94[2];
	double fRec93[3];
	double fRec113[2];
	double fRec112[3];
	double fVec45[2];
	double fRec92[2];
	double fRec91[3];
	double fRec115[2];
	double fRec114[3];
	double fVec46[3];
	double fVec47[3];
	double fVec48[3];
	double fVec49[3];
	double fVec50[2];
	double fRec90[2];
	double fRec89[3];
	double fRec117[2];
	double fRec116[3];
	double fVec51[2];
	double fRec88[2];
	double fRec87[3];
	double fRec119[2];
	double fRec118[3];
	double fRec86[3];
	double fVec52[2];
	double fRec85[2];
	double fRec84[2];
	double fRec83[3];
	double fRec121[2];
	double fRec120[3];
	double fVec53[2];
	double fRec82[2];
	double fRec81[3];
	double fRec123[2];
	double fRec122[3];
	double fRec80[3];
	double fRec79[3];
	double fRec78[3];
	double fRec77[3];
	double fVec54[2];
	double fRec76[2];
	double fRec75[2];
	double fRec74[3];
	double fRec125[2];
	double fRec124[3];
	double fRec73[3];
	double fRec72[3];
	double fRec71[3];
	double fRec70[2];
	double fRec69[2];
	double fVec55[3];
	double fVec56[1024];
	int iRec128[2];
	double fRec129[2];
	double fRec127[2];
	double fRec126[2];
	double fVec57[2];
	double fVec58[3];
	double fVec59[5];
	double fVec60[12];
	double fVec61[32];
	double fVec62[64];
	double fVec63[128];
	double fVec64[256];
	double fVec65[512];
	double fVec66[1024];
	double fVec67[2048];
	double fRec68[2];
	
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
		fConst3 = 0.01 * fConst0;
		iConst4 = static_cast<int>(std::floor(fConst3)) % 2;
		fConst5 = 3.141592653589793 / fConst0;
		fConst6 = 9424.77796076938 / (fConst0 * std::sin(28274.33388230814 / fConst0));
		fConst7 = std::tan(14137.16694115407 / fConst0);
		fConst8 = 1.0 / fConst7;
		fConst9 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst7));
		fConst10 = std::tan(94.24777960769379 / fConst0);
		fConst11 = 1.0 / fConst10;
		fConst12 = 1.0 - fConst11;
		fConst13 = 913.9178628624852 / (fConst0 * std::sin(2010.6192982974676 / fConst0));
		fConst14 = std::tan(1005.3096491487338 / fConst0);
		fConst15 = 1.0 / fConst14;
		fConst16 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst14));
		fConst17 = 69.11503837897544 / (fConst0 * std::sin(345.57519189487726 / fConst0));
		fConst18 = std::tan(172.78759594743863 / fConst0);
		fConst19 = 1.0 / fConst18;
		fConst20 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst18));
		fConst21 = 6.283185307179586 / fConst0;
		fConst22 = 4.363323129985823 / fConst0;
		fConst23 = 2050.3025739217596 / (fConst0 * std::sin(3895.5748904513434 / fConst0));
		fConst24 = std::tan(1947.7874452256717 / fConst0);
		fConst25 = 1.0 / fConst24;
		fConst26 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst24));
		fConst27 = std::tan(9424.77796076938 / fConst0);
		fConst28 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst27);
		fConst29 = 2.0 * (1.0 - fConst28);
		fConst30 = 1.0 / fConst27;
		fConst31 = (fConst30 + -1.0000000000000004) / fConst27 + 1.0;
		fConst32 = 1.0 / ((fConst30 + 1.0000000000000004) / fConst27 + 1.0);
		fConst33 = std::tan(376.99111843077515 / fConst0);
		fConst34 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst33);
		fConst35 = 2.0 * (1.0 - fConst34);
		fConst36 = 1.0 / fConst33;
		fConst37 = (fConst36 + -1.0000000000000004) / fConst33 + 1.0;
		fConst38 = (fConst36 + 1.0000000000000004) / fConst33 + 1.0;
		fConst39 = 1.0 / fConst38;
		fConst40 = std::tan(13194.68914507713 / fConst0);
		fConst41 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst40);
		fConst42 = 2.0 * (1.0 - fConst41);
		fConst43 = 1.0 / fConst40;
		fConst44 = (fConst43 + -1.0000000000000004) / fConst40 + 1.0;
		fConst45 = (fConst43 + 1.0000000000000004) / fConst40 + 1.0;
		fConst46 = 1.0 / fConst45;
		fConst47 = std::tan(10995.574287564275 / fConst0);
		fConst48 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst47);
		fConst49 = 2.0 * (1.0 - fConst48);
		fConst50 = 1.0 / fConst47;
		fConst51 = (fConst50 + -1.0000000000000004) / fConst47 + 1.0;
		fConst52 = (fConst50 + 1.0000000000000004) / fConst47 + 1.0;
		fConst53 = 1.0 / fConst52;
		fConst54 = std::tan(109.95574287564276 / fConst0);
		fConst55 = 1.0 / fConst54;
		fConst56 = 1.0 - fConst55;
		fConst57 = std::exp(-(1e+03 / fConst0));
		fConst58 = 1.0 / std::tan(47123.8898038469 / fConst0);
		fConst59 = 1.0 - fConst58;
		fConst60 = 1.0 / (fConst58 + 1.0);
		fConst61 = std::exp(-(2e+01 / fConst0));
		fConst62 = 1.0 - fConst61;
		fConst63 = 1.0 - fConst57;
		fConst64 = 1.1459155902616465 / fConst54;
		fConst65 = 1.0 / (fConst55 + 1.0);
		fConst66 = 1e+06 / fConst0;
		fConst67 = 1.0 - fConst50;
		fConst68 = 1.0 / (fConst50 + 1.0);
		fConst69 = 1.0 / (fConst40 * fConst52);
		fConst70 = 1.0 - fConst43;
		fConst71 = 1.0 / (fConst43 + 1.0);
		fConst72 = 0.3333333333333333 / fConst45;
		fConst73 = 1.0 - fConst36;
		fConst74 = 1.0 / (fConst36 + 1.0);
		fConst75 = 1.0 / (fConst27 * fConst38);
		fConst76 = 1.0 - fConst30;
		fConst77 = 1.0 / (fConst30 + 1.0);
		fConst78 = 0.86 / fConst10;
		fConst79 = 1.0 / (fConst11 + 1.0);
		fConst80 = 0.005 * fConst0;
		iConst81 = static_cast<int>(fConst80);
		iConst82 = static_cast<int>(std::floor(fConst80)) % 2;
		iConst83 = static_cast<int>(std::floor(0.0025 * fConst0)) % 2;
		iConst84 = iConst4 + 2 * iConst82;
		iConst85 = static_cast<int>(std::floor(0.00125 * fConst0)) % 2;
		iConst86 = iConst84 + 4 * iConst83;
		iConst87 = static_cast<int>(std::floor(0.000625 * fConst0)) % 2;
		iConst88 = iConst86 + 8 * iConst85;
		iConst89 = static_cast<int>(std::floor(0.0003125 * fConst0)) % 2;
		iConst90 = iConst88 + 16 * iConst87;
		iConst91 = static_cast<int>(std::floor(0.00015625 * fConst0)) % 2;
		iConst92 = iConst90 + 32 * iConst89;
		iConst93 = static_cast<int>(std::floor(7.8125e-05 * fConst0)) % 2;
		iConst94 = iConst92 + 64 * iConst91;
		iConst95 = static_cast<int>(std::floor(3.90625e-05 * fConst0)) % 2;
		iConst96 = iConst94 + 128 * iConst93;
		iConst97 = static_cast<int>(std::floor(1.953125e-05 * fConst0)) % 2;
		iConst98 = iConst96 + 256 * iConst95;
		iConst99 = static_cast<int>(std::floor(9.765625e-06 * fConst0)) % 2;
		iConst100 = iConst98 + 512 * iConst97;
		fConst101 = std::exp(-(12.5 / fConst0));
		fConst102 = std::exp(-(2e+02 / fConst0));
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
			fRec3[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec6[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec0[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec47[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec46[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec45[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec44[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec43[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fVec2[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fVec3[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec42[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec41[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fVec4[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fVec5[l16] = 0.0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec40[l17] = 0.0;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec39[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec6[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec38[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec37[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec36[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fVec7[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec8[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec35[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec34[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fVec9[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec33[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec32[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec31[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec10[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec30[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec29[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec49[l34] = 0.0;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec48[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec11[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec28[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec27[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec51[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec50[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fVec12[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fVec13[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fVec14[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fVec15[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fVec16[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec26[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec25[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec53[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec52[l49] = 0.0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fVec17[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec24[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fRec23[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec55[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec54[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec22[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fVec18[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec21[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec20[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec19[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec56[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec58[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec57[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fVec19[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec18[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec17[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec60[l66] = 0.0;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec59[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec16[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec15[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec14[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec13[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fVec20[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec12[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec11[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec10[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec62[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec61[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec9[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec8[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec7[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec63[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec5[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec4[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fVec21[l84] = 0.0;
		}
		IOTA0 = 0;
		for (int l85 = 0; l85 < 1024; l85 = l85 + 1) {
			fVec22[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			iRec66[l86] = 0;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec67[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec65[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec64[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fVec23[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fVec24[l91] = 0.0;
		}
		for (int l92 = 0; l92 < 5; l92 = l92 + 1) {
			fVec25[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 12; l93 = l93 + 1) {
			fVec26[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 32; l94 = l94 + 1) {
			fVec27[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 64; l95 = l95 + 1) {
			fVec28[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 128; l96 = l96 + 1) {
			fVec29[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 256; l97 = l97 + 1) {
			fVec30[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 512; l98 = l98 + 1) {
			fVec31[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 1024; l99 = l99 + 1) {
			fVec32[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 2048; l100 = l100 + 1) {
			fVec33[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec1[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fVec34[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec111[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 3; l104 = l104 + 1) {
			fRec110[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fVec35[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec109[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec108[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec107[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 3; l109 = l109 + 1) {
			fVec36[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			fVec37[l110] = 0.0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec106[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec105[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 3; l113 = l113 + 1) {
			fVec38[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fVec39[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec104[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 3; l116 = l116 + 1) {
			fRec103[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fVec40[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec102[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec101[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec100[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 3; l121 = l121 + 1) {
			fVec41[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fVec42[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec99[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 3; l124 = l124 + 1) {
			fRec98[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fVec43[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec97[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec96[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec95[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fVec44[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec94[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 3; l131 = l131 + 1) {
			fRec93[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec113[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 3; l133 = l133 + 1) {
			fRec112[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fVec45[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec92[l135] = 0.0;
		}
		for (int l136 = 0; l136 < 3; l136 = l136 + 1) {
			fRec91[l136] = 0.0;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec115[l137] = 0.0;
		}
		for (int l138 = 0; l138 < 3; l138 = l138 + 1) {
			fRec114[l138] = 0.0;
		}
		for (int l139 = 0; l139 < 3; l139 = l139 + 1) {
			fVec46[l139] = 0.0;
		}
		for (int l140 = 0; l140 < 3; l140 = l140 + 1) {
			fVec47[l140] = 0.0;
		}
		for (int l141 = 0; l141 < 3; l141 = l141 + 1) {
			fVec48[l141] = 0.0;
		}
		for (int l142 = 0; l142 < 3; l142 = l142 + 1) {
			fVec49[l142] = 0.0;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fVec50[l143] = 0.0;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec90[l144] = 0.0;
		}
		for (int l145 = 0; l145 < 3; l145 = l145 + 1) {
			fRec89[l145] = 0.0;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec117[l146] = 0.0;
		}
		for (int l147 = 0; l147 < 3; l147 = l147 + 1) {
			fRec116[l147] = 0.0;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fVec51[l148] = 0.0;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec88[l149] = 0.0;
		}
		for (int l150 = 0; l150 < 3; l150 = l150 + 1) {
			fRec87[l150] = 0.0;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec119[l151] = 0.0;
		}
		for (int l152 = 0; l152 < 3; l152 = l152 + 1) {
			fRec118[l152] = 0.0;
		}
		for (int l153 = 0; l153 < 3; l153 = l153 + 1) {
			fRec86[l153] = 0.0;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fVec52[l154] = 0.0;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec85[l155] = 0.0;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec84[l156] = 0.0;
		}
		for (int l157 = 0; l157 < 3; l157 = l157 + 1) {
			fRec83[l157] = 0.0;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec121[l158] = 0.0;
		}
		for (int l159 = 0; l159 < 3; l159 = l159 + 1) {
			fRec120[l159] = 0.0;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fVec53[l160] = 0.0;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec82[l161] = 0.0;
		}
		for (int l162 = 0; l162 < 3; l162 = l162 + 1) {
			fRec81[l162] = 0.0;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec123[l163] = 0.0;
		}
		for (int l164 = 0; l164 < 3; l164 = l164 + 1) {
			fRec122[l164] = 0.0;
		}
		for (int l165 = 0; l165 < 3; l165 = l165 + 1) {
			fRec80[l165] = 0.0;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec79[l166] = 0.0;
		}
		for (int l167 = 0; l167 < 3; l167 = l167 + 1) {
			fRec78[l167] = 0.0;
		}
		for (int l168 = 0; l168 < 3; l168 = l168 + 1) {
			fRec77[l168] = 0.0;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fVec54[l169] = 0.0;
		}
		for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
			fRec76[l170] = 0.0;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fRec75[l171] = 0.0;
		}
		for (int l172 = 0; l172 < 3; l172 = l172 + 1) {
			fRec74[l172] = 0.0;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec125[l173] = 0.0;
		}
		for (int l174 = 0; l174 < 3; l174 = l174 + 1) {
			fRec124[l174] = 0.0;
		}
		for (int l175 = 0; l175 < 3; l175 = l175 + 1) {
			fRec73[l175] = 0.0;
		}
		for (int l176 = 0; l176 < 3; l176 = l176 + 1) {
			fRec72[l176] = 0.0;
		}
		for (int l177 = 0; l177 < 3; l177 = l177 + 1) {
			fRec71[l177] = 0.0;
		}
		for (int l178 = 0; l178 < 2; l178 = l178 + 1) {
			fRec70[l178] = 0.0;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fRec69[l179] = 0.0;
		}
		for (int l180 = 0; l180 < 3; l180 = l180 + 1) {
			fVec55[l180] = 0.0;
		}
		for (int l181 = 0; l181 < 1024; l181 = l181 + 1) {
			fVec56[l181] = 0.0;
		}
		for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
			iRec128[l182] = 0;
		}
		for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
			fRec129[l183] = 0.0;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec127[l184] = 0.0;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fRec126[l185] = 0.0;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fVec57[l186] = 0.0;
		}
		for (int l187 = 0; l187 < 3; l187 = l187 + 1) {
			fVec58[l187] = 0.0;
		}
		for (int l188 = 0; l188 < 5; l188 = l188 + 1) {
			fVec59[l188] = 0.0;
		}
		for (int l189 = 0; l189 < 12; l189 = l189 + 1) {
			fVec60[l189] = 0.0;
		}
		for (int l190 = 0; l190 < 32; l190 = l190 + 1) {
			fVec61[l190] = 0.0;
		}
		for (int l191 = 0; l191 < 64; l191 = l191 + 1) {
			fVec62[l191] = 0.0;
		}
		for (int l192 = 0; l192 < 128; l192 = l192 + 1) {
			fVec63[l192] = 0.0;
		}
		for (int l193 = 0; l193 < 256; l193 = l193 + 1) {
			fVec64[l193] = 0.0;
		}
		for (int l194 = 0; l194 < 512; l194 = l194 + 1) {
			fVec65[l194] = 0.0;
		}
		for (int l195 = 0; l195 < 1024; l195 = l195 + 1) {
			fVec66[l195] = 0.0;
		}
		for (int l196 = 0; l196 < 2048; l196 = l196 + 1) {
			fVec67[l196] = 0.0;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec68[l197] = 0.0;
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
		double fSlow2 = fConst1 * static_cast<double>(iSlow1 == 1);
		double fSlow3 = fConst1 * static_cast<double>(fHslider1);
		double fSlow4 = fConst1 * static_cast<double>(iSlow1 == 2);
		double fSlow5 = fConst1 * static_cast<double>(fHslider2);
		double fSlow6 = fConst1 * static_cast<double>(fHslider3);
		double fSlow7 = fConst1 * static_cast<double>(fHslider4);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow2 + fConst2 * fRec0[1];
			int iTemp0 = static_cast<int>(1.0 - fRec0[0]);
			fRec2[0] = fSlow3 + fConst2 * fRec2[1];
			double fTemp1 = std::min<double>(1.0, std::max<double>(0.0, 3.3333333333333335 * (fRec2[0] + -0.7)));
			double fTemp2 = std::min<double>(1.0, std::max<double>(0.0, 2.2222222222222223 * (fRec2[0] + -0.55)));
			double fTemp3 = fRec0[0] * fTemp2;
			double fTemp4 = std::min<double>(1.0, std::max<double>(0.0, fRec2[0] * (1.32 - 0.32 * fRec2[0])));
			double fTemp5 = 0.09 * fTemp4 + fTemp3 * (0.04 * fTemp1 + 0.12) + 0.64;
			fRec3[0] = fSlow4 + fConst2 * fRec3[1];
			double fTemp6 = fRec3[0] * std::min<double>(1.0, std::max<double>(0.0, 1.6666666666666667 * (fRec2[0] + -0.4)));
			double fTemp7 = 1.0 - (fTemp3 + fTemp6);
			double fTemp8 = std::max<double>(0.06 * fTemp6 + (0.16000000000000003 - 0.03 * fTemp7), 2.220446049250313e-16);
			double fTemp9 = GrungrFaustDSP_faustpower2_f(fTemp4);
			double fTemp10 = fTemp3 * (0.04 * fTemp9 + 0.016) + fTemp6 * (0.05 * fTemp9 + 0.024);
			double fTemp11 = std::min<double>(1.0, std::max<double>(0.0, 5.555555555555555 * (fRec2[0] + -0.82)));
			fRec6[0] = fSlow5 + fConst2 * fRec6[1];
			double fTemp12 = std::pow(std::max<double>(fRec6[0], 2.220446049250313e-16), 0.88);
			double fTemp13 = fTemp12 + 1.0;
			double fTemp14 = std::min<double>(1.0, std::max<double>(0.0, 1.5384615384615383 * (fRec2[0] + -0.35)));
			double fTemp15 = fTemp14 * fTemp13;
			double fTemp16 = 1.46e+04 - 0.5 * fTemp15 * (9e+02 * fTemp4 + 6.5e+02 * fTemp11 + fRec0[0] * (8.5e+02 * fTemp11 + 3.5e+02));
			double fTemp17 = std::tan(fConst5 * fTemp16 * (1.0 - 0.1 * fTemp3));
			double fTemp18 = 1.0 / fTemp17;
			double fTemp19 = (fTemp18 + 1.414213562373095) / fTemp17 + 1.0;
			double fTemp20 = std::min<double>(1.0, std::max<double>(0.0, 2.857142857142857 * (fRec2[0] + -0.65)));
			double fTemp21 = 0.55 * fTemp4;
			double fTemp22 = 1.0 - fTemp4;
			double fTemp23 = std::min<double>(1.0, std::max<double>(0.0, fTemp4 + fTemp22 * (0.22 * fTemp7 + 0.07) + fTemp3 * (0.1 * fTemp22 + 0.05 * fTemp11) + fTemp6 * (0.08 * fTemp22 + 0.03 * fTemp20)));
			double fTemp24 = std::pow(1e+01, 0.05 * (2.1 * fTemp7 * (fTemp23 - fTemp4) + fRec0[0] * (fTemp14 * (0.45 * fTemp4 + (0.35 - 0.25 * fTemp11)) - 0.55 * fTemp2 * fTemp1) + fTemp6 * (fTemp21 + (0.5 - 0.2 * fTemp20)) + (0.95 - 1.1 * GrungrFaustDSP_faustpower2_f(fTemp11))));
			double fTemp25 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp17);
			double fTemp26 = (fTemp18 + -1.414213562373095) / fTemp17 + 1.0;
			double fTemp27 = std::tan(fConst5 * fTemp16);
			double fTemp28 = 1.0 / fTemp27;
			double fTemp29 = (fTemp28 + 1.414213562373095) / fTemp27 + 1.0;
			double fTemp30 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp27);
			double fTemp31 = (fTemp28 + -1.414213562373095) / fTemp27 + 1.0;
			double fTemp32 = fTemp3 * (0.6 * fTemp11 + 1.8);
			int iTemp33 = -fTemp32 > 0.0;
			double fTemp34 = fConst6 * std::pow(1e+01, 0.05 * std::fabs(-fTemp32));
			double fTemp35 = ((iTemp33) ? fConst6 : fTemp34);
			double fTemp36 = fConst8 * (fConst8 + fTemp35) + 1.0;
			double fTemp37 = ((iTemp33) ? fTemp34 : fConst6);
			double fTemp38 = fConst8 * (fConst8 - fTemp37) + 1.0;
			double fTemp39 = fConst9 * fRec9[1];
			double fTemp40 = fConst8 * (fConst8 - fTemp35) + 1.0;
			double fTemp41 = std::tan(fConst5 * (4.2e+03 - 1.7e+03 * fRec3[0]));
			double fTemp42 = 1.0 / fTemp41;
			double fTemp43 = (fTemp42 + 1.0000000000000004) / fTemp41 + 1.0;
			double fTemp44 = GrungrFaustDSP_faustpower2_f(fTemp41);
			double fTemp45 = 1.8 * fTemp11;
			double fTemp46 = fRec0[0] * fTemp14;
			double fTemp47 = std::pow(1e+01, -(0.025 * fTemp13 * (fTemp46 * (fTemp45 + 0.9) + fTemp6 * (1.4 * fTemp20 + 0.7))));
			double fTemp48 = 1.0 - 1.0 / fTemp44;
			double fTemp49 = (fTemp42 + -1.0000000000000004) / fTemp41 + 1.0;
			double fTemp50 = fTemp42 + 1.0;
			double fTemp51 = 2.5 * fTemp3;
			int iTemp52 = -fTemp51 > 0.0;
			double fTemp53 = fConst13 * std::pow(1e+01, 0.05 * std::fabs(-fTemp51));
			double fTemp54 = ((iTemp52) ? fConst13 : fTemp53);
			double fTemp55 = fConst15 * (fConst15 + fTemp54) + 1.0;
			double fTemp56 = ((iTemp52) ? fTemp53 : fConst13);
			double fTemp57 = fConst15 * (fConst15 - fTemp56) + 1.0;
			double fTemp58 = fConst16 * fRec13[1];
			double fTemp59 = fConst15 * (fConst15 - fTemp54) + 1.0;
			double fTemp60 = fTemp3 * fTemp1;
			double fTemp61 = 3.0 * fTemp60;
			int iTemp62 = -fTemp61 > 0.0;
			double fTemp63 = fConst17 * std::pow(1e+01, 0.05 * std::fabs(-fTemp61));
			double fTemp64 = ((iTemp62) ? fConst17 : fTemp63);
			double fTemp65 = fConst19 * (fConst19 + fTemp64) + 1.0;
			double fTemp66 = ((iTemp62) ? fTemp63 : fConst17);
			double fTemp67 = fConst19 * (fConst19 - fTemp66) + 1.0;
			double fTemp68 = fConst20 * fRec14[1];
			double fTemp69 = fConst19 * (fConst19 - fTemp64) + 1.0;
			double fTemp70 = 9.5e+02 * fTemp3 + 1.45e+03;
			double fTemp71 = std::tan(fConst5 * fTemp70);
			double fTemp72 = 0.5 * fTemp15 * (0.7 - 0.3 * fTemp4 + fRec0[0] * (0.4 * fTemp22 + 0.2));
			int iTemp73 = fTemp72 > 0.0;
			double fTemp74 = std::sin(fConst21 * fTemp70);
			double fTemp75 = fConst22 * (fTemp70 * std::pow(1e+01, 0.05 * std::fabs(fTemp72)) / fTemp74);
			double fTemp76 = fConst22 * (fTemp70 / fTemp74);
			double fTemp77 = ((iTemp73) ? fTemp76 : fTemp75);
			double fTemp78 = 1.0 / fTemp71;
			double fTemp79 = (fTemp78 + fTemp77) / fTemp71 + 1.0;
			double fTemp80 = ((iTemp73) ? fTemp75 : fTemp76);
			double fTemp81 = (fTemp78 - fTemp80) / fTemp71 + 1.0;
			double fTemp82 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp71);
			double fTemp83 = 2.0 * fRec15[1] * fTemp82;
			double fTemp84 = (fTemp78 - fTemp77) / fTemp71 + 1.0;
			double fTemp85 = fTemp14 * (0.95 - fTemp21) + 3.0 * fTemp6 + -5.6;
			int iTemp86 = fTemp85 > 0.0;
			double fTemp87 = fConst23 * std::pow(1e+01, 0.05 * std::fabs(fTemp85));
			double fTemp88 = ((iTemp86) ? fConst23 : fTemp87);
			double fTemp89 = fConst25 * (fConst25 + fTemp88) + 1.0;
			double fTemp90 = ((iTemp86) ? fTemp87 : fConst23);
			double fTemp91 = fConst25 * (fConst25 - fTemp90) + 1.0;
			double fTemp92 = fConst26 * fRec16[1];
			double fTemp93 = fConst25 * (fConst25 - fTemp88) + 1.0;
			double fTemp94 = std::pow(1e+01, 0.05 * (19.0 * fTemp12 + -8.0));
			double fTemp95 = std::tan(fConst5 * (1.12e+04 - (1.8e+03 * fTemp23 + 9e+02 * fTemp11 + 0.5 * fTemp13 * (fTemp46 * (9.6e+02 * fTemp11 + 4.2e+02) + fTemp6 * (7.2e+02 * fTemp20 + 3.2e+02)))));
			double fTemp96 = 1.0 / fTemp95;
			double fTemp97 = (fTemp96 + 1.414213562373095) / fTemp95 + 1.0;
			double fTemp98 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp95);
			double fTemp99 = (fTemp96 + -1.414213562373095) / fTemp95 + 1.0;
			double fTemp100 = 0.05 * fTemp3 * (0.5 * fTemp22 + 1.3);
			double fTemp101 = std::pow(1e+01, -fTemp100);
			double fTemp102 = std::pow(1e+01, fTemp100);
			double fTemp103 = std::max<double>(0.08 * fTemp6 + (0.24 - 0.04 * fTemp7), 2.220446049250313e-16);
			double fTemp104 = std::pow(1e+01, 0.05 * (6.0 * (fTemp23 + 1.0) + fTemp3 * (fTemp45 + 0.7) + fTemp6 * (1.5 * fTemp20 + 0.5)));
			double fTemp105 = std::tan(fConst5 * (2e+01 * fTemp23 + fTemp3 * (18.0 * fTemp4 + 1e+01) + fTemp6 * (1e+01 * fTemp4 + 6.0) + 1.1e+02));
			double fTemp106 = 1.0 / fTemp105;
			double fTemp107 = fTemp106 + 1.0;
			double fTemp108 = 1.6e+03 * fTemp23 + fTemp3 * (6.2e+02 * fTemp11 + 2.2e+02) + fTemp6 * (4.6e+02 * fTemp20 + 1.6e+02);
			double fTemp109 = std::tan(fConst5 * (8.1e+03 - fTemp108));
			double fTemp110 = 1.0 / fTemp109;
			double fTemp111 = (fTemp110 + 1.414213562373095) / fTemp109 + 1.0;
			double fTemp112 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp109);
			double fTemp113 = (fTemp110 + -1.414213562373095) / fTemp109 + 1.0;
			double fTemp114 = 0.12 * fTemp23;
			double fTemp115 = fTemp3 * (0.015 * fTemp22 + 0.02) + fTemp6 * (0.02 * fTemp22 + 0.025);
			double fTemp116 = 0.85 * fTemp115 + (0.74 - fTemp114);
			double fTemp117 = 1.0 - 0.16 * fTemp3;
			double fTemp118 = 0.1 * fTemp4 + 1.1;
			double fTemp119 = -0.01 - (0.018 * fTemp23 + fTemp3 * (0.006 * fTemp1 + 0.004) + fTemp6 * (0.005 * fTemp20 + 0.003));
			double fTemp120 = std::atan(fTemp119 * fTemp118 * fTemp117 / fTemp116);
			double fTemp121 = fTemp3 * (1.7 * fTemp4 + 0.9) + fTemp6 * (2.0 * fTemp4 + 1.2);
			double fTemp122 = std::pow(1e+01, 0.05 * (14.0 * fTemp23 + (12.0 - 0.65 * fTemp121)));
			double fTemp123 = 1.0 / std::tan(fConst5 * (8.4e+03 - fTemp108));
			double fTemp124 = fTemp123 + 1.0;
			double fTemp125 = 12.0 * fTemp4;
			double fTemp126 = std::tan(fConst5 * (25.0 * fTemp23 + fRec0[0] * (fTemp14 * (24.0 * fTemp4 + 16.0) + 12.0 * fTemp2) + fTemp6 * (fTemp125 + 8.0) + 8e+01));
			double fTemp127 = 1.0 / fTemp126;
			double fTemp128 = fTemp127 + 1.0;
			double fTemp129 = std::tan(fConst5 * (1.08e+04 - (2e+03 * fTemp23 + fTemp3 * (8.2e+02 * fTemp11 + 2.8e+02) + fTemp6 * (6e+02 * fTemp20 + 2e+02))));
			double fTemp130 = 1.0 / fTemp129;
			double fTemp131 = (fTemp130 + 1.414213562373095) / fTemp129 + 1.0;
			double fTemp132 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp129);
			double fTemp133 = (fTemp130 + -1.414213562373095) / fTemp129 + 1.0;
			double fTemp134 = 0.1 * fTemp23;
			double fTemp135 = fTemp115 + (0.76 - fTemp134);
			double fTemp136 = 1.0 - 0.18 * fTemp3;
			double fTemp137 = 0.05 * fTemp4 + 1.0;
			double fTemp138 = std::atan(0.014 * (fTemp137 * fTemp136 / fTemp135));
			double fTemp139 = std::pow(1e+01, 0.05 * (19.0 * fTemp23 + (18.0 - fTemp121)));
			double fTemp140 = 0.07 * fTemp4 + (0.4 - 0.03 * fTemp46);
			double fTemp141 = std::atan(0.005555555555555556 * fTemp140);
			double fTemp142 = std::tan(fConst5 * std::max<double>(2e+01, fRec0[0] * (fTemp125 + 4e+01)));
			double fTemp143 = 1.0 / fTemp142;
			double fTemp144 = (fTemp143 + 1.414213562373095) / fTemp142 + 1.0;
			double fTemp145 = GrungrFaustDSP_faustpower2_f(fTemp142);
			double fTemp146 = fTemp145 * fTemp144;
			double fTemp147 = 1.0 - 1.0 / fTemp145;
			double fTemp148 = (fTemp143 + -1.414213562373095) / fTemp142 + 1.0;
			double fTemp149 = std::tan(fConst5 * (fTemp46 * (3e+01 * fTemp4 + 12.0) + 1.6));
			double fTemp150 = 1.0 / fTemp149;
			double fTemp151 = fTemp150 + 1.0;
			double fTemp152 = 1.0 - fTemp150;
			double fTemp153 = static_cast<double>(input0[i0]);
			double fTemp154 = ((iSlow0) ? 0.0 : fTemp153);
			fVec0[0] = fTemp154;
			fRec47[0] = (0.2 * ((fTemp154 - fVec0[1]) / fTemp149) - fRec47[1] * fTemp152) / fTemp151;
			fRec46[0] = fRec47[0] - (fRec46[2] * fTemp148 + 2.0 * fRec46[1] * fTemp147) / fTemp144;
			double fTemp155 = (fRec46[2] + (fRec46[0] - 2.0 * fRec46[1])) / fTemp146;
			fVec1[0] = fTemp155;
			fRec45[0] = -(fConst60 * (fConst59 * fRec45[1] - (fTemp155 + fVec1[1])));
			double fTemp156 = std::fabs(fRec45[0]);
			fRec44[0] = std::max<double>(fTemp156, fConst61 * fRec44[1] + fConst62 * fTemp156);
			fRec43[0] = fConst63 * fRec44[0] + fConst57 * fRec43[1];
			double fTemp157 = (fRec45[0] * (1.0 - 0.7 * fTemp3 * std::min<double>(1.0, std::max<double>(0.0, fRec43[0] + -0.4))) + 0.01) * fTemp140;
			fVec2[0] = fTemp157;
			double fTemp158 = fVec2[1] - fVec2[2];
			double fTemp159 = 0.5555555555555556 * fTemp158;
			double fTemp160 = 0.5555555555555556 * fVec2[2];
			double fTemp161 = GrungrFaustDSP_faustpower2_f(fTemp160) + 1.0;
			double fTemp162 = std::atan(fTemp160);
			double fTemp163 = 0.5555555555555556 * fVec2[1];
			double fTemp164 = GrungrFaustDSP_faustpower2_f(fTemp163) + 1.0;
			double fTemp165 = std::atan(fTemp163);
			double fTemp166 = fTemp165 * fTemp164;
			double fTemp167 = fVec2[1] * fTemp165;
			double fTemp168 = std::log(fTemp164);
			double fTemp169 = 1.1111111111111112 * fVec2[1];
			double fTemp170 = fTemp157 - fVec2[1];
			double fTemp171 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp170);
			double fTemp172 = 0.5555555555555556 * fTemp157;
			double fTemp173 = GrungrFaustDSP_faustpower2_f(fTemp172) + 1.0;
			double fTemp174 = std::atan(fTemp172);
			double fTemp175 = ((fTemp171 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp172 + fTemp169)) : (0.5555555555555556 * fTemp157 * (0.5555555555555556 * (fTemp157 * fTemp174 - fTemp167) - 0.5 * (std::log(fTemp173) - fTemp168)) + 0.2777777777777778 * fTemp170 - 0.5 * (fTemp174 * fTemp173 - fTemp166)) / fTemp171) + ((GrungrFaustDSP_faustpower2_f(fTemp159) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp169 + fTemp160)) : (0.5555555555555556 * fVec2[2] * (0.5 * (fTemp168 - std::log(fTemp161)) - 0.5555555555555556 * (fTemp167 - fVec2[2] * fTemp162)) + 0.5 * (fTemp166 - fTemp162 * fTemp161) - 0.2777777777777778 * fTemp158) / GrungrFaustDSP_faustpower2_f(-fTemp159)) - fTemp141;
			fVec3[0] = fTemp175;
			fRec42[0] = fConst65 * (fConst64 * (fTemp175 - fVec3[1]) - fConst56 * fRec42[1]);
			double fTemp176 = fRec42[0] * fTemp139;
			double fTemp177 = fConst66 + fRec41[1];
			double fTemp178 = fRec41[1] - fConst66;
			fRec41[0] = ((fTemp177 < fTemp176) ? fTemp177 : ((fTemp178 > fTemp176) ? fTemp178 : fTemp176));
			double fTemp179 = (fRec41[0] + 0.014) * fTemp137 * fTemp136;
			double fTemp180 = fTemp179 / fTemp135;
			fVec4[0] = fTemp180;
			double fTemp181 = GrungrFaustDSP_faustpower2_f(fVec4[2]);
			double fTemp182 = std::atan(fVec4[2]);
			double fTemp183 = GrungrFaustDSP_faustpower2_f(fVec4[1]);
			double fTemp184 = std::atan(fVec4[1]);
			double fTemp185 = fTemp184 - (fVec4[1] - fTemp184 * fTemp183);
			double fTemp186 = fVec4[1] * fTemp184;
			double fTemp187 = std::log(fTemp183 + 1.0);
			double fTemp188 = 2.0 * fVec4[1];
			double fTemp189 = GrungrFaustDSP_faustpower2_f(fTemp180 - fVec4[1]);
			double fTemp190 = GrungrFaustDSP_faustpower2_f(fTemp180);
			double fTemp191 = std::atan(fTemp180);
			double fTemp192 = fTemp135 * (((fTemp189 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp180 + fTemp188)) : (fTemp179 * (fTemp179 * fTemp191 / fTemp135 - (fTemp186 + 0.5 * (std::log(fTemp190 + 1.0) - fTemp187))) / fTemp135 - 0.5 * (fTemp191 - (fTemp180 - fTemp191 * fTemp190) - fTemp185)) / fTemp189) + ((GrungrFaustDSP_faustpower2_f(fVec4[1] - fVec4[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp188 + fVec4[2])) : (fVec4[2] * (fVec4[2] * fTemp182 + 0.5 * (fTemp187 - std::log(fTemp181 + 1.0)) - fTemp186) + 0.5 * (fTemp185 - (fTemp182 - (fVec4[2] - fTemp182 * fTemp181)))) / GrungrFaustDSP_faustpower2_f(fVec4[2] - fVec4[1])) - fTemp138);
			fVec5[0] = fTemp192;
			fRec40[0] = 0.995 * fRec40[1] + 0.6366197723675814 * (fTemp192 - fVec5[1]);
			fRec39[0] = fRec40[0] - (fRec39[2] * fTemp133 + 2.0 * fRec39[1] * fTemp132) / fTemp131;
			double fTemp193 = (fRec39[2] + fRec39[0] + 2.0 * fRec39[1]) / fTemp131;
			fVec6[0] = fTemp193;
			double fTemp194 = 1.0 - fTemp127;
			fRec38[0] = -((fRec38[1] * fTemp194 - (fTemp193 - fVec6[1]) / fTemp126) / fTemp128);
			double fTemp195 = 1.0 - fTemp123;
			fRec37[0] = -((fRec37[1] * fTemp195 - (fRec38[0] + fRec38[1])) / fTemp124);
			double fTemp196 = fRec37[0] * fTemp122;
			double fTemp197 = fConst66 + fRec36[1];
			double fTemp198 = fRec36[1] - fConst66;
			fRec36[0] = ((fTemp197 < fTemp196) ? fTemp197 : ((fTemp198 > fTemp196) ? fTemp198 : fTemp196));
			double fTemp199 = (fRec36[0] + fTemp119) * fTemp118 * fTemp117;
			double fTemp200 = fTemp199 / fTemp116;
			fVec7[0] = fTemp200;
			double fTemp201 = GrungrFaustDSP_faustpower2_f(fVec7[2]);
			double fTemp202 = std::atan(fVec7[2]);
			double fTemp203 = GrungrFaustDSP_faustpower2_f(fVec7[1]);
			double fTemp204 = std::atan(fVec7[1]);
			double fTemp205 = fTemp204 - (fVec7[1] - fTemp204 * fTemp203);
			double fTemp206 = fVec7[1] * fTemp204;
			double fTemp207 = std::log(fTemp203 + 1.0);
			double fTemp208 = 2.0 * fVec7[1];
			double fTemp209 = GrungrFaustDSP_faustpower2_f(fTemp200 - fVec7[1]);
			double fTemp210 = GrungrFaustDSP_faustpower2_f(fTemp200);
			double fTemp211 = std::atan(fTemp200);
			double fTemp212 = fTemp116 * (((fTemp209 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp200 + fTemp208)) : (fTemp199 * (fTemp199 * fTemp211 / fTemp116 - (fTemp206 + 0.5 * (std::log(fTemp210 + 1.0) - fTemp207))) / fTemp116 - 0.5 * (fTemp211 - (fTemp200 - fTemp211 * fTemp210) - fTemp205)) / fTemp209) + ((GrungrFaustDSP_faustpower2_f(fVec7[1] - fVec7[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp208 + fVec7[2])) : (fVec7[2] * (fVec7[2] * fTemp202 + 0.5 * (fTemp207 - std::log(fTemp201 + 1.0)) - fTemp206) + 0.5 * (fTemp205 - (fTemp202 - (fVec7[2] - fTemp202 * fTemp201)))) / GrungrFaustDSP_faustpower2_f(fVec7[2] - fVec7[1])) - fTemp120);
			fVec8[0] = fTemp212;
			fRec35[0] = 0.995 * fRec35[1] + 0.6366197723675814 * (fTemp212 - fVec8[1]);
			fRec34[0] = fRec35[0] - (fRec34[2] * fTemp113 + 2.0 * fRec34[1] * fTemp112) / fTemp111;
			double fTemp213 = (fRec34[2] + fRec34[0] + 2.0 * fRec34[1]) / fTemp111;
			double fTemp214 = fTemp213 + 0.05 * fTemp3 * tanh(0.4 * fTemp213);
			fVec9[0] = fTemp214;
			double fTemp215 = 1.0 - fTemp106;
			fRec33[0] = -((fRec33[1] * fTemp215 - (fTemp214 - fVec9[1]) / fTemp105) / fTemp107);
			double fTemp216 = fRec33[0] * fTemp104;
			double fTemp217 = fConst66 + fRec32[1];
			double fTemp218 = fRec32[1] - fConst66;
			fRec32[0] = ((fTemp217 < fTemp216) ? fTemp217 : ((fTemp218 > fTemp216) ? fTemp218 : fTemp216));
			fRec31[0] = fConst1 * std::fabs(fRec32[0]) + fConst2 * fRec31[1];
			double fTemp219 = fRec32[0] * (1.0 - fTemp103 * std::min<double>(1.0, std::max<double>(0.0, fRec31[0] * fTemp10 / fTemp103)));
			fVec10[0] = fTemp219;
			fRec30[0] = -(fConst68 * (fConst67 * fRec30[1] - fConst50 * (fTemp219 - fVec10[1])));
			fRec29[0] = fRec30[0] - fConst53 * (fConst51 * fRec29[2] + fConst49 * fRec29[1]);
			fRec49[0] = -(fConst68 * (fConst67 * fRec49[1] - (fTemp219 + fVec10[1])));
			fRec48[0] = fRec49[0] - fConst53 * (fConst51 * fRec48[2] + fConst49 * fRec48[1]);
			double fTemp220 = fRec48[2] + fRec48[0] + 2.0 * fRec48[1] + fConst48 * (fRec29[2] + (fRec29[0] - 2.0 * fRec29[1])) * fTemp102;
			fVec11[0] = fTemp220;
			fRec28[0] = -(fConst71 * (fConst70 * fRec28[1] - fConst69 * (fTemp220 - fVec11[1])));
			fRec27[0] = fRec28[0] - fConst46 * (fConst44 * fRec27[2] + fConst42 * fRec27[1]);
			fRec51[0] = -(fConst71 * (fConst70 * fRec51[1] - fConst53 * (fTemp220 + fVec11[1])));
			fRec50[0] = fRec51[0] - fConst46 * (fConst44 * fRec50[2] + fConst42 * fRec50[1]);
			double fTemp221 = fRec50[2] + fRec50[0] + 2.0 * fRec50[1] + fConst41 * fTemp102 * (fRec27[2] + (fRec27[0] - 2.0 * fRec27[1]));
			int iTemp222 = (fConst46 * fTemp221) > 0.0;
			double fTemp223 = 1.08 - 0.08 * fTemp23;
			double fTemp224 = fTemp221 / fTemp223;
			fVec12[0] = fTemp224;
			double fTemp225 = fConst46 * (fVec12[1] - fVec12[2]);
			double fTemp226 = fConst46 * fVec12[2];
			int iTemp227 = (fTemp226 <= 1.0) & (fTemp226 >= -1.0);
			double fTemp228 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp226);
			double fTemp229 = static_cast<double>((fTemp226 > 0.0) - (fTemp226 < 0.0));
			double fTemp230 = fConst46 * fVec12[1];
			int iTemp231 = (fTemp230 <= 1.0) & (fTemp230 >= -1.0);
			double fTemp232 = static_cast<double>((fTemp230 > 0.0) - (fTemp230 < 0.0));
			double fTemp233 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp230);
			double fTemp234 = ((iTemp231) ? fTemp233 : fConst46 * fVec12[1] * fTemp232 + -0.5);
			double fTemp235 = ((iTemp231) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp230) : fTemp232 * (fTemp233 + -0.16666666666666666));
			double fTemp236 = 2.0 * fVec12[1];
			double fTemp237 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp224 - fVec12[1]));
			double fTemp238 = fConst46 * fTemp224;
			int iTemp239 = (fTemp238 <= 1.0) & (fTemp238 >= -1.0);
			double fTemp240 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp238);
			double fTemp241 = static_cast<double>((fTemp238 > 0.0) - (fTemp238 < 0.0));
			double fTemp242 = 1.62 - fTemp114;
			double fTemp243 = fTemp221 / fTemp242;
			fVec13[0] = fTemp243;
			double fTemp244 = fConst46 * (fVec13[1] - fVec13[2]);
			double fTemp245 = fConst46 * fVec13[2];
			int iTemp246 = (fTemp245 <= 1.0) & (fTemp245 >= -1.0);
			double fTemp247 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp245);
			double fTemp248 = static_cast<double>((fTemp245 > 0.0) - (fTemp245 < 0.0));
			double fTemp249 = fConst46 * fVec13[1];
			int iTemp250 = (fTemp249 <= 1.0) & (fTemp249 >= -1.0);
			double fTemp251 = static_cast<double>((fTemp249 > 0.0) - (fTemp249 < 0.0));
			double fTemp252 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp249);
			double fTemp253 = ((iTemp250) ? fTemp252 : fConst46 * fVec13[1] * fTemp251 + -0.5);
			double fTemp254 = ((iTemp250) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp249) : fTemp251 * (fTemp252 + -0.16666666666666666));
			double fTemp255 = 2.0 * fVec13[1];
			double fTemp256 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp243 - fVec13[1]));
			double fTemp257 = fConst46 * fTemp243;
			int iTemp258 = (fTemp257 <= 1.0) & (fTemp257 >= -1.0);
			double fTemp259 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp257);
			double fTemp260 = static_cast<double>((fTemp257 > 0.0) - (fTemp257 < 0.0));
			double fTemp261 = fTemp60 * (0.34 * fTemp11 + 0.08);
			double fTemp262 = 0.74 - fTemp134;
			double fTemp263 = fTemp221 / fTemp262;
			fVec14[0] = fTemp263;
			double fTemp264 = fConst46 * (fVec14[1] - fVec14[2]);
			double fTemp265 = fConst46 * fVec14[2];
			int iTemp266 = (fTemp265 <= 1.0) & (fTemp265 >= -1.0);
			double fTemp267 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp265);
			double fTemp268 = static_cast<double>((fTemp265 > 0.0) - (fTemp265 < 0.0));
			double fTemp269 = fConst46 * fVec14[1];
			int iTemp270 = (fTemp269 <= 1.0) & (fTemp269 >= -1.0);
			double fTemp271 = static_cast<double>((fTemp269 > 0.0) - (fTemp269 < 0.0));
			double fTemp272 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp269);
			double fTemp273 = ((iTemp270) ? fTemp272 : fConst46 * fVec14[1] * fTemp271 + -0.5);
			double fTemp274 = ((iTemp270) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp269) : fTemp271 * (fTemp272 + -0.16666666666666666));
			double fTemp275 = 2.0 * fVec14[1];
			double fTemp276 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp263 - fVec14[1]));
			double fTemp277 = fConst46 * fTemp263;
			int iTemp278 = (fTemp277 <= 1.0) & (fTemp277 >= -1.0);
			double fTemp279 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp277);
			double fTemp280 = static_cast<double>((fTemp277 > 0.0) - (fTemp277 < 0.0));
			double fTemp281 = 1.1 - 0.16 * fTemp23;
			double fTemp282 = fTemp221 / fTemp281;
			fVec15[0] = fTemp282;
			double fTemp283 = fConst46 * (fVec15[1] - fVec15[2]);
			double fTemp284 = fConst46 * fVec15[2];
			int iTemp285 = (fTemp284 <= 1.0) & (fTemp284 >= -1.0);
			double fTemp286 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp284);
			double fTemp287 = static_cast<double>((fTemp284 > 0.0) - (fTemp284 < 0.0));
			double fTemp288 = fConst46 * fVec15[1];
			int iTemp289 = (fTemp288 <= 1.0) & (fTemp288 >= -1.0);
			double fTemp290 = static_cast<double>((fTemp288 > 0.0) - (fTemp288 < 0.0));
			double fTemp291 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp288);
			double fTemp292 = ((iTemp289) ? fTemp291 : fConst46 * fVec15[1] * fTemp290 + -0.5);
			double fTemp293 = ((iTemp289) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp288) : fTemp290 * (fTemp291 + -0.16666666666666666));
			double fTemp294 = 2.0 * fVec15[1];
			double fTemp295 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp282 - fVec15[1]));
			double fTemp296 = fConst46 * fTemp282;
			int iTemp297 = (fTemp296 <= 1.0) & (fTemp296 >= -1.0);
			double fTemp298 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp296);
			double fTemp299 = static_cast<double>((fTemp296 > 0.0) - (fTemp296 < 0.0));
			double fTemp300 = 1.0 - fTemp261;
			double fTemp301 = fTemp300 * ((iTemp222) ? fTemp281 * (((fTemp295 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp282 + fTemp294))) : (fConst46 * (fTemp221 * (((iTemp297) ? fTemp298 : fConst46 * (fTemp221 * fTemp299 / fTemp281) + -0.5) - fTemp292) / fTemp281) + fTemp293 - ((iTemp297) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp296) : fTemp299 * (fTemp298 + -0.16666666666666666))) / fTemp295) + ((GrungrFaustDSP_faustpower2_f(fTemp283) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp294 + fVec15[2]))) : (fTemp293 + fConst46 * fVec15[2] * (((iTemp285) ? fTemp286 : fConst46 * fVec15[2] * fTemp287 + -0.5) - fTemp292) - ((iTemp285) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp284) : fTemp287 * (fTemp286 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp283))) : fTemp262 * (((fTemp276 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp263 + fTemp275))) : (fConst46 * (fTemp221 * (((iTemp278) ? fTemp279 : fConst46 * (fTemp221 * fTemp280 / fTemp262) + -0.5) - fTemp273) / fTemp262) + fTemp274 - ((iTemp278) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp277) : fTemp280 * (fTemp279 + -0.16666666666666666))) / fTemp276) + ((GrungrFaustDSP_faustpower2_f(fTemp264) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp275 + fVec14[2]))) : (fTemp274 + fConst46 * fVec14[2] * (((iTemp266) ? fTemp267 : fConst46 * fVec14[2] * fTemp268 + -0.5) - fTemp273) - ((iTemp266) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp265) : fTemp268 * (fTemp267 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp264)))) + fTemp261 * ((iTemp222) ? fTemp242 * (((fTemp256 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp243 + fTemp255))) : (fConst46 * (fTemp221 * (((iTemp258) ? fTemp259 : fConst46 * (fTemp221 * fTemp260 / fTemp242) + -0.5) - fTemp253) / fTemp242) + fTemp254 - ((iTemp258) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp257) : fTemp260 * (fTemp259 + -0.16666666666666666))) / fTemp256) + ((GrungrFaustDSP_faustpower2_f(fTemp244) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp255 + fVec13[2]))) : (fTemp254 + fConst46 * fVec13[2] * (((iTemp246) ? fTemp247 : fConst46 * fVec13[2] * fTemp248 + -0.5) - fTemp253) - ((iTemp246) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp245) : fTemp248 * (fTemp247 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp244))) : fTemp223 * (((fTemp237 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp224 + fTemp236))) : (fConst46 * (fTemp221 * (((iTemp239) ? fTemp240 : fConst46 * (fTemp221 * fTemp241 / fTemp223) + -0.5) - fTemp234) / fTemp223) + fTemp235 - ((iTemp239) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp238) : fTemp241 * (fTemp240 + -0.16666666666666666))) / fTemp237) + ((GrungrFaustDSP_faustpower2_f(fTemp225) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp236 + fVec12[2]))) : (fTemp235 + fConst46 * fVec12[2] * (((iTemp227) ? fTemp228 : fConst46 * fVec12[2] * fTemp229 + -0.5) - fTemp234) - ((iTemp227) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp226) : fTemp229 * (fTemp228 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp225))));
			fVec16[0] = fTemp301;
			fRec26[0] = -(fConst68 * (fConst67 * fRec26[1] - fConst50 * (fTemp301 - fVec16[1])));
			fRec25[0] = fRec26[0] - fConst53 * (fConst51 * fRec25[2] + fConst49 * fRec25[1]);
			fRec53[0] = -(fConst68 * (fConst67 * fRec53[1] - (fTemp301 + fVec16[1])));
			fRec52[0] = fRec53[0] - fConst53 * (fConst51 * fRec52[2] + fConst49 * fRec52[1]);
			double fTemp302 = fRec52[2] + fRec52[0] + 2.0 * fRec52[1] + fConst48 * (fRec25[2] + (fRec25[0] - 2.0 * fRec25[1])) * fTemp101;
			fVec17[0] = fTemp302;
			fRec24[0] = -(fConst71 * (fConst70 * fRec24[1] - fConst69 * (fTemp302 - fVec17[1])));
			fRec23[0] = fRec24[0] - fConst46 * (fConst44 * fRec23[2] + fConst42 * fRec23[1]);
			fRec55[0] = -(fConst71 * (fConst70 * fRec55[1] - fConst53 * (fTemp302 + fVec17[1])));
			fRec54[0] = fRec55[0] - fConst46 * (fConst44 * fRec54[2] + fConst42 * fRec54[1]);
			fRec22[0] = fConst46 * (fRec54[2] + fRec54[0] + 2.0 * fRec54[1] + fConst41 * fTemp101 * (fRec23[2] + (fRec23[0] - 2.0 * fRec23[1]))) - (fRec22[2] * fTemp99 + 2.0 * fRec22[1] * fTemp98) / fTemp97;
			double fTemp303 = (fRec22[2] + fRec22[0] + 2.0 * fRec22[1]) / fTemp97;
			fVec18[0] = fTemp303;
			fRec21[0] = 0.995 * fRec21[1] + fTemp303 - fVec18[1];
			fRec20[0] = -(fConst74 * (fConst73 * fRec20[1] - fConst36 * (fRec21[0] - fRec21[1])));
			fRec19[0] = fRec20[0] - fConst39 * (fConst37 * fRec19[2] + fConst35 * fRec19[1]);
			fRec56[0] = fSlow6 + fConst2 * fRec56[1];
			double fTemp304 = std::pow(1e+01, 0.05 * (24.0 * std::pow(std::max<double>(fRec56[0], 2.220446049250313e-16), 0.82) + -9.0));
			fRec58[0] = -(fConst74 * (fConst73 * fRec58[1] - (fRec21[0] + fRec21[1])));
			fRec57[0] = fRec58[0] - fConst39 * (fConst37 * fRec57[2] + fConst35 * fRec57[1]);
			double fTemp305 = (fRec57[2] + fRec57[0] + 2.0 * fRec57[1]) * fTemp304 + fConst34 * (fRec19[2] + (fRec19[0] - 2.0 * fRec19[1]));
			fVec19[0] = fTemp305;
			fRec18[0] = -(fConst77 * (fConst76 * fRec18[1] - fConst75 * (fTemp305 - fVec19[1])));
			fRec17[0] = fRec18[0] - fConst32 * (fConst31 * fRec17[2] + fConst29 * fRec17[1]);
			fRec60[0] = -(fConst77 * (fConst76 * fRec60[1] - fConst39 * (fTemp305 + fVec19[1])));
			fRec59[0] = fRec60[0] - fConst32 * (fConst31 * fRec59[2] + fConst29 * fRec59[1]);
			fRec16[0] = fConst32 * (fRec59[2] + fRec59[0] + 2.0 * fRec59[1] + fConst28 * (fRec17[2] + (fRec17[0] - 2.0 * fRec17[1])) * fTemp94) - (fRec16[2] * fTemp93 + fTemp92) / fTemp89;
			double fTemp306 = fConst25 * (fConst25 + fTemp90) + 1.0;
			fRec15[0] = (fTemp92 + fRec16[0] * fTemp306 + fRec16[2] * fTemp91) / fTemp89 - (fRec15[2] * fTemp84 + fTemp83) / fTemp79;
			double fTemp307 = (fTemp78 + fTemp80) / fTemp71 + 1.0;
			fRec14[0] = (fTemp83 + fRec15[0] * fTemp307 + fRec15[2] * fTemp81) / fTemp79 - (fRec14[2] * fTemp69 + fTemp68) / fTemp65;
			double fTemp308 = fConst19 * (fConst19 + fTemp66) + 1.0;
			fRec13[0] = (fTemp68 + fRec14[0] * fTemp308 + fRec14[2] * fTemp67) / fTemp65 - (fRec13[2] * fTemp59 + fTemp58) / fTemp55;
			double fTemp309 = fConst15 * (fConst15 + fTemp56) + 1.0;
			double fTemp310 = (fTemp58 + fRec13[0] * fTemp309 + fRec13[2] * fTemp57) / fTemp55;
			fVec20[0] = fTemp310;
			fRec12[0] = fConst79 * (fConst78 * (fTemp310 - fVec20[1]) - fConst12 * fRec12[1]);
			double fTemp311 = 1.0 - fTemp42;
			fRec11[0] = -((fRec11[1] * fTemp311 - (fRec12[0] - fRec12[1]) / fTemp41) / fTemp50);
			fRec10[0] = fRec11[0] - (fRec10[2] * fTemp49 + 2.0 * fRec10[1] * fTemp48) / fTemp43;
			fRec62[0] = -((fTemp311 * fRec62[1] - (fRec12[0] + fRec12[1])) / fTemp50);
			fRec61[0] = fRec62[0] - (fTemp49 * fRec61[2] + 2.0 * fTemp48 * fRec61[1]) / fTemp43;
			fRec9[0] = (fRec61[2] + fRec61[0] + 2.0 * fRec61[1] + (fRec10[2] + (fRec10[0] - 2.0 * fRec10[1])) * fTemp47 / fTemp44) / fTemp43 - (fRec9[2] * fTemp40 + fTemp39) / fTemp36;
			double fTemp312 = fConst8 * (fConst8 + fTemp37) + 1.0;
			fRec8[0] = (fTemp39 + fRec9[0] * fTemp312 + fRec9[2] * fTemp38) / fTemp36 - (fRec8[2] * fTemp31 + 2.0 * fRec8[1] * fTemp30) / fTemp29;
			fRec7[0] = (fRec8[2] + fRec8[0] + 2.0 * fRec8[1]) / fTemp29 - (fRec7[2] * fTemp26 + 2.0 * fRec7[1] * fTemp25) / fTemp19;
			fRec63[0] = fSlow7 + fConst2 * fRec63[1];
			double fTemp313 = std::pow(std::max<double>(fRec63[0], 2.220446049250313e-16), 1.65);
			double fTemp314 = fTemp313 * (fRec7[2] + fRec7[0] + 2.0 * fRec7[1]) * fTemp24 / fTemp19;
			double fTemp315 = fConst66 + fRec5[1];
			double fTemp316 = fRec5[1] - fConst66;
			fRec5[0] = ((fTemp315 < fTemp314) ? fTemp315 : ((fTemp316 > fTemp314) ? fTemp316 : fTemp314));
			fRec4[0] = fConst1 * std::fabs(fRec5[0]) + fConst2 * fRec4[1];
			double fTemp317 = fRec5[0] * (1.0 - fTemp8 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec4[0] * fTemp10 / fTemp8)))) * fTemp5;
			fVec21[0] = fTemp317;
			double fTemp318 = fVec21[1] - fVec21[2];
			double fTemp319 = 0.8928571428571428 * fTemp318;
			double fTemp320 = 0.8928571428571428 * fVec21[2];
			double fTemp321 = GrungrFaustDSP_faustpower2_f(fTemp320) + 1.0;
			double fTemp322 = std::atan(fTemp320);
			double fTemp323 = 0.8928571428571428 * fVec21[1];
			double fTemp324 = GrungrFaustDSP_faustpower2_f(fTemp323) + 1.0;
			double fTemp325 = std::atan(fTemp323);
			double fTemp326 = fTemp325 * fTemp324;
			double fTemp327 = fVec21[1] * fTemp325;
			double fTemp328 = std::log(fTemp324);
			double fTemp329 = 1.7857142857142856 * fVec21[1];
			double fTemp330 = fTemp317 - fVec21[1];
			double fTemp331 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp330);
			double fTemp332 = 0.8928571428571428 * fTemp317;
			double fTemp333 = GrungrFaustDSP_faustpower2_f(fTemp332) + 1.0;
			double fTemp334 = std::atan(fTemp332);
			double fTemp335 = 0.7130141450516913 * (((fTemp331 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp332 + fTemp329)) : (0.8928571428571428 * fTemp317 * (0.8928571428571428 * (fTemp317 * fTemp334 - fTemp327) - 0.5 * (std::log(fTemp333) - fTemp328)) + 0.4464285714285714 * fTemp330 - 0.5 * (fTemp334 * fTemp333 - fTemp326)) / fTemp331) + ((GrungrFaustDSP_faustpower2_f(fTemp319) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp329 + fTemp320)) : (0.8928571428571428 * fVec21[2] * (0.5 * (fTemp328 - std::log(fTemp321)) - 0.8928571428571428 * (fTemp327 - fVec21[2] * fTemp322)) + 0.5 * (fTemp326 - fTemp322 * fTemp321) - 0.4464285714285714 * fTemp318) / GrungrFaustDSP_faustpower2_f(-fTemp319)));
			double fTemp336 = ((iTemp0) ? 0.0 : fTemp335);
			fVec22[IOTA0 & 1023] = fTemp336;
			double fTemp337 = std::fabs(fTemp336);
			int iTemp338 = (fTemp337 >= fRec67[1]) | (static_cast<double>(iRec66[1]) >= fConst3);
			iRec66[0] = ((iTemp338) ? 0 : iRec66[1] + 1);
			fRec67[0] = ((iTemp338) ? fTemp337 : fRec67[1]);
			fRec65[0] = fConst63 * fRec67[0] + fConst57 * fRec65[1];
			double fTemp339 = std::fabs(fRec65[0]);
			fRec64[0] = std::max<double>(fTemp339, fConst61 * fRec64[1] + fConst62 * fTemp339);
			double fTemp340 = ((iTemp0) ? fTemp335 : std::min<double>(1.0, 0.944 / std::max<double>(fRec64[0], 2.220446049250313e-16)) * fVec22[(IOTA0 - iConst81) & 1023]);
			double fTemp341 = ((iTemp0) ? 0.0 : fTemp340);
			double fTemp342 = std::fabs(fTemp341);
			fVec23[0] = fTemp342;
			double fTemp343 = std::max<double>(fTemp342, fVec23[1]);
			fVec24[0] = fTemp343;
			double fTemp344 = std::max<double>(fTemp343, fVec24[2]);
			fVec25[0] = fTemp344;
			double fTemp345 = std::max<double>(fTemp344, fVec25[4]);
			fVec26[0] = fTemp345;
			double fTemp346 = std::max<double>(fTemp345, fVec26[8]);
			fVec27[IOTA0 & 31] = fTemp346;
			double fTemp347 = std::max<double>(fTemp346, fVec27[(IOTA0 - 16) & 31]);
			fVec28[IOTA0 & 63] = fTemp347;
			double fTemp348 = std::max<double>(fTemp347, fVec28[(IOTA0 - 32) & 63]);
			fVec29[IOTA0 & 127] = fTemp348;
			double fTemp349 = std::max<double>(fTemp348, fVec29[(IOTA0 - 64) & 127]);
			fVec30[IOTA0 & 255] = fTemp349;
			double fTemp350 = std::max<double>(fTemp349, fVec30[(IOTA0 - 128) & 255]);
			fVec31[IOTA0 & 511] = fTemp350;
			double fTemp351 = std::max<double>(fTemp350, fVec31[(IOTA0 - 256) & 511]);
			fVec32[IOTA0 & 1023] = fTemp351;
			fVec33[IOTA0 & 2047] = std::max<double>(fTemp351, fVec32[(IOTA0 - 512) & 1023]);
			double fTemp352 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp342 : -1.7976931348623157e+308), ((iConst82) ? fVec24[iConst4] : -1.7976931348623157e+308)), ((iConst83) ? fVec25[iConst84] : -1.7976931348623157e+308)), ((iConst85) ? fVec26[iConst86] : -1.7976931348623157e+308)), ((iConst87) ? fVec27[(IOTA0 - iConst88) & 31] : -1.7976931348623157e+308)), ((iConst89) ? fVec28[(IOTA0 - iConst90) & 63] : -1.7976931348623157e+308)), ((iConst91) ? fVec29[(IOTA0 - iConst92) & 127] : -1.7976931348623157e+308)), ((iConst93) ? fVec30[(IOTA0 - iConst94) & 255] : -1.7976931348623157e+308)), ((iConst95) ? fVec31[(IOTA0 - iConst96) & 511] : -1.7976931348623157e+308)), ((iConst97) ? fVec32[(IOTA0 - iConst98) & 1023] : -1.7976931348623157e+308)), ((iConst99) ? fVec33[(IOTA0 - iConst100) & 2047] : -1.7976931348623157e+308));
			double fTemp353 = ((fTemp352 > fRec1[1]) ? fConst102 : fConst101);
			fRec1[0] = fTemp352 * (1.0 - fTemp353) + fRec1[1] * fTemp353;
			double fTemp354 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec1[0]));
			int iTemp355 = (fTemp354 > -53.0) + (fTemp354 > -47.0);
			double fTemp356 = ((iTemp0) ? fTemp340 : fTemp341 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp355 == 0) ? fTemp354 + 5e+01 : ((iTemp355 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp354 + 47.0)) : 0.0)))));
			double fTemp357 = ((iTemp0) ? 0.0 : fTemp356);
			output0[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp153 : ((iTemp0) ? fTemp356 : static_cast<double>((fTemp357 > 0.0) - (fTemp357 < 0.0)) * std::pow(std::fabs(fTemp357), 0.97))));
			double fTemp358 = fConst9 * fRec73[1];
			double fTemp359 = fConst16 * fRec77[1];
			double fTemp360 = fConst20 * fRec78[1];
			double fTemp361 = 2.0 * fTemp82 * fRec79[1];
			double fTemp362 = fConst26 * fRec80[1];
			double fTemp363 = static_cast<double>(input1[i0]);
			double fTemp364 = ((iSlow0) ? 0.0 : fTemp363);
			fVec34[0] = fTemp364;
			fRec111[0] = (0.2 * ((fTemp364 - fVec34[1]) / fTemp149) - fTemp152 * fRec111[1]) / fTemp151;
			fRec110[0] = fRec111[0] - (fTemp148 * fRec110[2] + 2.0 * fTemp147 * fRec110[1]) / fTemp144;
			double fTemp365 = (fRec110[2] + (fRec110[0] - 2.0 * fRec110[1])) / fTemp146;
			fVec35[0] = fTemp365;
			fRec109[0] = -(fConst60 * (fConst59 * fRec109[1] - (fTemp365 + fVec35[1])));
			double fTemp366 = std::fabs(fRec109[0]);
			fRec108[0] = std::max<double>(fTemp366, fConst61 * fRec108[1] + fConst62 * fTemp366);
			fRec107[0] = fConst63 * fRec108[0] + fConst57 * fRec107[1];
			double fTemp367 = fTemp140 * (fRec109[0] * (1.0 - 0.7 * fTemp3 * std::min<double>(1.0, std::max<double>(0.0, fRec107[0] + -0.4))) + 0.01);
			fVec36[0] = fTemp367;
			double fTemp368 = fVec36[1] - fVec36[2];
			double fTemp369 = 0.5555555555555556 * fTemp368;
			double fTemp370 = 0.5555555555555556 * fVec36[2];
			double fTemp371 = GrungrFaustDSP_faustpower2_f(fTemp370) + 1.0;
			double fTemp372 = std::atan(fTemp370);
			double fTemp373 = 0.5555555555555556 * fVec36[1];
			double fTemp374 = GrungrFaustDSP_faustpower2_f(fTemp373) + 1.0;
			double fTemp375 = std::atan(fTemp373);
			double fTemp376 = fTemp375 * fTemp374;
			double fTemp377 = fVec36[1] * fTemp375;
			double fTemp378 = std::log(fTemp374);
			double fTemp379 = 1.1111111111111112 * fVec36[1];
			double fTemp380 = fTemp367 - fVec36[1];
			double fTemp381 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp380);
			double fTemp382 = 0.5555555555555556 * fTemp367;
			double fTemp383 = GrungrFaustDSP_faustpower2_f(fTemp382) + 1.0;
			double fTemp384 = std::atan(fTemp382);
			double fTemp385 = fTemp141 - (((fTemp381 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp382 + fTemp379)) : (0.5555555555555556 * fTemp367 * (0.5555555555555556 * (fTemp367 * fTemp384 - fTemp377) - 0.5 * (std::log(fTemp383) - fTemp378)) + 0.2777777777777778 * fTemp380 - 0.5 * (fTemp384 * fTemp383 - fTemp376)) / fTemp381) + ((GrungrFaustDSP_faustpower2_f(fTemp369) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp379 + fTemp370)) : (0.5555555555555556 * fVec36[2] * (0.5 * (fTemp378 - std::log(fTemp371)) - 0.5555555555555556 * (fTemp377 - fVec36[2] * fTemp372)) + 0.5 * (fTemp376 - fTemp372 * fTemp371) - 0.2777777777777778 * fTemp368) / GrungrFaustDSP_faustpower2_f(-fTemp369)));
			fVec37[0] = fTemp385;
			fRec106[0] = -(fConst65 * (fConst56 * fRec106[1] + fConst64 * (fTemp385 - fVec37[1])));
			double fTemp386 = fRec106[0] * fTemp139;
			double fTemp387 = fConst66 + fRec105[1];
			double fTemp388 = fRec105[1] - fConst66;
			fRec105[0] = ((fTemp387 < fTemp386) ? fTemp387 : ((fTemp388 > fTemp386) ? fTemp388 : fTemp386));
			double fTemp389 = (fRec105[0] + 0.014) * fTemp137 * fTemp136;
			double fTemp390 = fTemp389 / fTemp135;
			fVec38[0] = fTemp390;
			double fTemp391 = GrungrFaustDSP_faustpower2_f(fVec38[2]);
			double fTemp392 = std::atan(fVec38[2]);
			double fTemp393 = GrungrFaustDSP_faustpower2_f(fVec38[1]);
			double fTemp394 = std::atan(fVec38[1]);
			double fTemp395 = fTemp394 - (fVec38[1] - fTemp394 * fTemp393);
			double fTemp396 = fVec38[1] * fTemp394;
			double fTemp397 = std::log(fTemp393 + 1.0);
			double fTemp398 = 2.0 * fVec38[1];
			double fTemp399 = GrungrFaustDSP_faustpower2_f(fTemp390 - fVec38[1]);
			double fTemp400 = GrungrFaustDSP_faustpower2_f(fTemp390);
			double fTemp401 = std::atan(fTemp390);
			double fTemp402 = fTemp135 * (fTemp138 - (((fTemp399 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp390 + fTemp398)) : (fTemp389 * (fTemp389 * fTemp401 / fTemp135 - (fTemp396 + 0.5 * (std::log(fTemp400 + 1.0) - fTemp397))) / fTemp135 - 0.5 * (fTemp401 - (fTemp390 - fTemp401 * fTemp400) - fTemp395)) / fTemp399) + ((GrungrFaustDSP_faustpower2_f(fVec38[1] - fVec38[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp398 + fVec38[2])) : (fVec38[2] * (fVec38[2] * fTemp392 + 0.5 * (fTemp397 - std::log(fTemp391 + 1.0)) - fTemp396) + 0.5 * (fTemp395 - (fTemp392 - (fVec38[2] - fTemp392 * fTemp391)))) / GrungrFaustDSP_faustpower2_f(fVec38[2] - fVec38[1]))));
			fVec39[0] = fTemp402;
			fRec104[0] = 0.995 * fRec104[1] - 0.6366197723675814 * (fTemp402 - fVec39[1]);
			fRec103[0] = fRec104[0] - (fTemp133 * fRec103[2] + 2.0 * fTemp132 * fRec103[1]) / fTemp131;
			double fTemp403 = (fRec103[2] + fRec103[0] + 2.0 * fRec103[1]) / fTemp131;
			fVec40[0] = fTemp403;
			fRec102[0] = -((fTemp194 * fRec102[1] - (fTemp403 - fVec40[1]) / fTemp126) / fTemp128);
			fRec101[0] = -((fTemp195 * fRec101[1] - (fRec102[0] + fRec102[1])) / fTemp124);
			double fTemp404 = fRec101[0] * fTemp122;
			double fTemp405 = fConst66 + fRec100[1];
			double fTemp406 = fRec100[1] - fConst66;
			fRec100[0] = ((fTemp405 < fTemp404) ? fTemp405 : ((fTemp406 > fTemp404) ? fTemp406 : fTemp404));
			double fTemp407 = fTemp118 * fTemp117 * (fRec100[0] + fTemp119);
			double fTemp408 = fTemp407 / fTemp116;
			fVec41[0] = fTemp408;
			double fTemp409 = GrungrFaustDSP_faustpower2_f(fVec41[2]);
			double fTemp410 = std::atan(fVec41[2]);
			double fTemp411 = GrungrFaustDSP_faustpower2_f(fVec41[1]);
			double fTemp412 = std::atan(fVec41[1]);
			double fTemp413 = fTemp412 - (fVec41[1] - fTemp412 * fTemp411);
			double fTemp414 = fVec41[1] * fTemp412;
			double fTemp415 = std::log(fTemp411 + 1.0);
			double fTemp416 = 2.0 * fVec41[1];
			double fTemp417 = GrungrFaustDSP_faustpower2_f(fTemp408 - fVec41[1]);
			double fTemp418 = GrungrFaustDSP_faustpower2_f(fTemp408);
			double fTemp419 = std::atan(fTemp408);
			double fTemp420 = fTemp116 * (fTemp120 - (((fTemp417 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp408 + fTemp416)) : (fTemp407 * (fTemp407 * fTemp419 / fTemp116 - (fTemp414 + 0.5 * (std::log(fTemp418 + 1.0) - fTemp415))) / fTemp116 - 0.5 * (fTemp419 - (fTemp408 - fTemp419 * fTemp418) - fTemp413)) / fTemp417) + ((GrungrFaustDSP_faustpower2_f(fVec41[1] - fVec41[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp416 + fVec41[2])) : (fVec41[2] * (fVec41[2] * fTemp410 + 0.5 * (fTemp415 - std::log(fTemp409 + 1.0)) - fTemp414) + 0.5 * (fTemp413 - (fTemp410 - (fVec41[2] - fTemp410 * fTemp409)))) / GrungrFaustDSP_faustpower2_f(fVec41[2] - fVec41[1]))));
			fVec42[0] = fTemp420;
			fRec99[0] = 0.995 * fRec99[1] - 0.6366197723675814 * (fTemp420 - fVec42[1]);
			fRec98[0] = fRec99[0] - (fTemp113 * fRec98[2] + 2.0 * fTemp112 * fRec98[1]) / fTemp111;
			double fTemp421 = (fRec98[2] + fRec98[0] + 2.0 * fRec98[1]) / fTemp111;
			double fTemp422 = fTemp421 + 0.05 * fTemp3 * tanh(0.4 * fTemp421);
			fVec43[0] = fTemp422;
			fRec97[0] = -((fTemp215 * fRec97[1] - (fTemp422 - fVec43[1]) / fTemp105) / fTemp107);
			double fTemp423 = fRec97[0] * fTemp104;
			double fTemp424 = fConst66 + fRec96[1];
			double fTemp425 = fRec96[1] - fConst66;
			fRec96[0] = ((fTemp424 < fTemp423) ? fTemp424 : ((fTemp425 > fTemp423) ? fTemp425 : fTemp423));
			fRec95[0] = fConst1 * std::fabs(fRec96[0]) + fConst2 * fRec95[1];
			double fTemp426 = fRec96[0] * (1.0 - fTemp103 * std::min<double>(1.0, std::max<double>(0.0, fRec95[0] * fTemp10 / fTemp103)));
			fVec44[0] = fTemp426;
			fRec94[0] = -(fConst68 * (fConst67 * fRec94[1] - fConst50 * (fTemp426 - fVec44[1])));
			fRec93[0] = fRec94[0] - fConst53 * (fConst51 * fRec93[2] + fConst49 * fRec93[1]);
			fRec113[0] = -(fConst68 * (fConst67 * fRec113[1] - (fTemp426 + fVec44[1])));
			fRec112[0] = fRec113[0] - fConst53 * (fConst51 * fRec112[2] + fConst49 * fRec112[1]);
			double fTemp427 = fRec112[2] + fRec112[0] + 2.0 * fRec112[1] + fConst48 * fTemp102 * (fRec93[2] + (fRec93[0] - 2.0 * fRec93[1]));
			fVec45[0] = fTemp427;
			fRec92[0] = -(fConst71 * (fConst70 * fRec92[1] - fConst69 * (fTemp427 - fVec45[1])));
			fRec91[0] = fRec92[0] - fConst46 * (fConst44 * fRec91[2] + fConst42 * fRec91[1]);
			fRec115[0] = -(fConst71 * (fConst70 * fRec115[1] - fConst53 * (fTemp427 + fVec45[1])));
			fRec114[0] = fRec115[0] - fConst46 * (fConst44 * fRec114[2] + fConst42 * fRec114[1]);
			double fTemp428 = fRec114[2] + fRec114[0] + 2.0 * fRec114[1] + fConst41 * fTemp102 * (fRec91[2] + (fRec91[0] - 2.0 * fRec91[1]));
			int iTemp429 = (fConst46 * fTemp428) > 0.0;
			double fTemp430 = fTemp428 / fTemp223;
			fVec46[0] = fTemp430;
			double fTemp431 = fConst46 * (fVec46[1] - fVec46[2]);
			double fTemp432 = fConst46 * fVec46[2];
			int iTemp433 = (fTemp432 <= 1.0) & (fTemp432 >= -1.0);
			double fTemp434 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp432);
			double fTemp435 = static_cast<double>((fTemp432 > 0.0) - (fTemp432 < 0.0));
			double fTemp436 = fConst46 * fVec46[1];
			int iTemp437 = (fTemp436 <= 1.0) & (fTemp436 >= -1.0);
			double fTemp438 = static_cast<double>((fTemp436 > 0.0) - (fTemp436 < 0.0));
			double fTemp439 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp436);
			double fTemp440 = ((iTemp437) ? fTemp439 : fConst46 * fVec46[1] * fTemp438 + -0.5);
			double fTemp441 = ((iTemp437) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp436) : fTemp438 * (fTemp439 + -0.16666666666666666));
			double fTemp442 = 2.0 * fVec46[1];
			double fTemp443 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp430 - fVec46[1]));
			double fTemp444 = fConst46 * fTemp430;
			int iTemp445 = (fTemp444 <= 1.0) & (fTemp444 >= -1.0);
			double fTemp446 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp444);
			double fTemp447 = static_cast<double>((fTemp444 > 0.0) - (fTemp444 < 0.0));
			double fTemp448 = fTemp428 / fTemp242;
			fVec47[0] = fTemp448;
			double fTemp449 = fConst46 * (fVec47[1] - fVec47[2]);
			double fTemp450 = fConst46 * fVec47[2];
			int iTemp451 = (fTemp450 <= 1.0) & (fTemp450 >= -1.0);
			double fTemp452 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp450);
			double fTemp453 = static_cast<double>((fTemp450 > 0.0) - (fTemp450 < 0.0));
			double fTemp454 = fConst46 * fVec47[1];
			int iTemp455 = (fTemp454 <= 1.0) & (fTemp454 >= -1.0);
			double fTemp456 = static_cast<double>((fTemp454 > 0.0) - (fTemp454 < 0.0));
			double fTemp457 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp454);
			double fTemp458 = ((iTemp455) ? fTemp457 : fConst46 * fVec47[1] * fTemp456 + -0.5);
			double fTemp459 = ((iTemp455) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp454) : fTemp456 * (fTemp457 + -0.16666666666666666));
			double fTemp460 = 2.0 * fVec47[1];
			double fTemp461 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp448 - fVec47[1]));
			double fTemp462 = fConst46 * fTemp448;
			int iTemp463 = (fTemp462 <= 1.0) & (fTemp462 >= -1.0);
			double fTemp464 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp462);
			double fTemp465 = static_cast<double>((fTemp462 > 0.0) - (fTemp462 < 0.0));
			double fTemp466 = fTemp428 / fTemp262;
			fVec48[0] = fTemp466;
			double fTemp467 = fConst46 * (fVec48[1] - fVec48[2]);
			double fTemp468 = fConst46 * fVec48[2];
			int iTemp469 = (fTemp468 <= 1.0) & (fTemp468 >= -1.0);
			double fTemp470 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp468);
			double fTemp471 = static_cast<double>((fTemp468 > 0.0) - (fTemp468 < 0.0));
			double fTemp472 = fConst46 * fVec48[1];
			int iTemp473 = (fTemp472 <= 1.0) & (fTemp472 >= -1.0);
			double fTemp474 = static_cast<double>((fTemp472 > 0.0) - (fTemp472 < 0.0));
			double fTemp475 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp472);
			double fTemp476 = ((iTemp473) ? fTemp475 : fConst46 * fVec48[1] * fTemp474 + -0.5);
			double fTemp477 = ((iTemp473) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp472) : fTemp474 * (fTemp475 + -0.16666666666666666));
			double fTemp478 = 2.0 * fVec48[1];
			double fTemp479 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp466 - fVec48[1]));
			double fTemp480 = fConst46 * fTemp466;
			int iTemp481 = (fTemp480 <= 1.0) & (fTemp480 >= -1.0);
			double fTemp482 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp480);
			double fTemp483 = static_cast<double>((fTemp480 > 0.0) - (fTemp480 < 0.0));
			double fTemp484 = fTemp428 / fTemp281;
			fVec49[0] = fTemp484;
			double fTemp485 = fConst46 * (fVec49[1] - fVec49[2]);
			double fTemp486 = fConst46 * fVec49[2];
			int iTemp487 = (fTemp486 <= 1.0) & (fTemp486 >= -1.0);
			double fTemp488 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp486);
			double fTemp489 = static_cast<double>((fTemp486 > 0.0) - (fTemp486 < 0.0));
			double fTemp490 = fConst46 * fVec49[1];
			int iTemp491 = (fTemp490 <= 1.0) & (fTemp490 >= -1.0);
			double fTemp492 = static_cast<double>((fTemp490 > 0.0) - (fTemp490 < 0.0));
			double fTemp493 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp490);
			double fTemp494 = ((iTemp491) ? fTemp493 : fConst46 * fVec49[1] * fTemp492 + -0.5);
			double fTemp495 = ((iTemp491) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp490) : fTemp492 * (fTemp493 + -0.16666666666666666));
			double fTemp496 = 2.0 * fVec49[1];
			double fTemp497 = GrungrFaustDSP_faustpower2_f(fConst46 * (fTemp484 - fVec49[1]));
			double fTemp498 = fConst46 * fTemp484;
			int iTemp499 = (fTemp498 <= 1.0) & (fTemp498 >= -1.0);
			double fTemp500 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp498);
			double fTemp501 = static_cast<double>((fTemp498 > 0.0) - (fTemp498 < 0.0));
			double fTemp502 = fTemp300 * ((iTemp429) ? fTemp281 * (((fTemp497 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp484 + fTemp496))) : (fConst46 * (fTemp428 * (((iTemp499) ? fTemp500 : fConst46 * (fTemp428 * fTemp501 / fTemp281) + -0.5) - fTemp494) / fTemp281) + fTemp495 - ((iTemp499) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp498) : fTemp501 * (fTemp500 + -0.16666666666666666))) / fTemp497) + ((GrungrFaustDSP_faustpower2_f(fTemp485) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp496 + fVec49[2]))) : (fTemp495 + fConst46 * fVec49[2] * (((iTemp487) ? fTemp488 : fConst46 * fVec49[2] * fTemp489 + -0.5) - fTemp494) - ((iTemp487) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp486) : fTemp489 * (fTemp488 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp485))) : fTemp262 * (((fTemp479 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp466 + fTemp478))) : (fConst46 * (fTemp428 * (((iTemp481) ? fTemp482 : fConst46 * (fTemp428 * fTemp483 / fTemp262) + -0.5) - fTemp476) / fTemp262) + fTemp477 - ((iTemp481) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp480) : fTemp483 * (fTemp482 + -0.16666666666666666))) / fTemp479) + ((GrungrFaustDSP_faustpower2_f(fTemp467) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp478 + fVec48[2]))) : (fTemp477 + fConst46 * fVec48[2] * (((iTemp469) ? fTemp470 : fConst46 * fVec48[2] * fTemp471 + -0.5) - fTemp476) - ((iTemp469) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp468) : fTemp471 * (fTemp470 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp467)))) + fTemp261 * ((iTemp429) ? fTemp242 * (((fTemp461 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp448 + fTemp460))) : (fConst46 * (fTemp428 * (((iTemp463) ? fTemp464 : fConst46 * (fTemp428 * fTemp465 / fTemp242) + -0.5) - fTemp458) / fTemp242) + fTemp459 - ((iTemp463) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp462) : fTemp465 * (fTemp464 + -0.16666666666666666))) / fTemp461) + ((GrungrFaustDSP_faustpower2_f(fTemp449) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp460 + fVec47[2]))) : (fTemp459 + fConst46 * fVec47[2] * (((iTemp451) ? fTemp452 : fConst46 * fVec47[2] * fTemp453 + -0.5) - fTemp458) - ((iTemp451) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp450) : fTemp453 * (fTemp452 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp449))) : fTemp223 * (((fTemp443 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp430 + fTemp442))) : (fConst46 * (fTemp428 * (((iTemp445) ? fTemp446 : fConst46 * (fTemp428 * fTemp447 / fTemp223) + -0.5) - fTemp440) / fTemp223) + fTemp441 - ((iTemp445) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp444) : fTemp447 * (fTemp446 + -0.16666666666666666))) / fTemp443) + ((GrungrFaustDSP_faustpower2_f(fTemp431) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst72 * (fTemp442 + fVec46[2]))) : (fTemp441 + fConst46 * fVec46[2] * (((iTemp433) ? fTemp434 : fConst46 * fVec46[2] * fTemp435 + -0.5) - fTemp440) - ((iTemp433) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp432) : fTemp435 * (fTemp434 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp431))));
			fVec50[0] = fTemp502;
			fRec90[0] = -(fConst68 * (fConst67 * fRec90[1] - fConst50 * (fTemp502 - fVec50[1])));
			fRec89[0] = fRec90[0] - fConst53 * (fConst51 * fRec89[2] + fConst49 * fRec89[1]);
			fRec117[0] = -(fConst68 * (fConst67 * fRec117[1] - (fTemp502 + fVec50[1])));
			fRec116[0] = fRec117[0] - fConst53 * (fConst51 * fRec116[2] + fConst49 * fRec116[1]);
			double fTemp503 = fRec116[2] + fRec116[0] + 2.0 * fRec116[1] + fConst48 * fTemp101 * (fRec89[2] + (fRec89[0] - 2.0 * fRec89[1]));
			fVec51[0] = fTemp503;
			fRec88[0] = -(fConst71 * (fConst70 * fRec88[1] - fConst69 * (fTemp503 - fVec51[1])));
			fRec87[0] = fRec88[0] - fConst46 * (fConst44 * fRec87[2] + fConst42 * fRec87[1]);
			fRec119[0] = -(fConst71 * (fConst70 * fRec119[1] - fConst53 * (fTemp503 + fVec51[1])));
			fRec118[0] = fRec119[0] - fConst46 * (fConst44 * fRec118[2] + fConst42 * fRec118[1]);
			fRec86[0] = fConst46 * (fRec118[2] + fRec118[0] + 2.0 * fRec118[1] + fConst41 * fTemp101 * (fRec87[2] + (fRec87[0] - 2.0 * fRec87[1]))) - (fTemp99 * fRec86[2] + 2.0 * fTemp98 * fRec86[1]) / fTemp97;
			double fTemp504 = (fRec86[2] + fRec86[0] + 2.0 * fRec86[1]) / fTemp97;
			fVec52[0] = fTemp504;
			fRec85[0] = 0.995 * fRec85[1] + fTemp504 - fVec52[1];
			fRec84[0] = -(fConst74 * (fConst73 * fRec84[1] - fConst36 * (fRec85[0] - fRec85[1])));
			fRec83[0] = fRec84[0] - fConst39 * (fConst37 * fRec83[2] + fConst35 * fRec83[1]);
			fRec121[0] = -(fConst74 * (fConst73 * fRec121[1] - (fRec85[0] + fRec85[1])));
			fRec120[0] = fRec121[0] - fConst39 * (fConst37 * fRec120[2] + fConst35 * fRec120[1]);
			double fTemp505 = fTemp304 * (fRec120[2] + fRec120[0] + 2.0 * fRec120[1]) + fConst34 * (fRec83[2] + (fRec83[0] - 2.0 * fRec83[1]));
			fVec53[0] = fTemp505;
			fRec82[0] = -(fConst77 * (fConst76 * fRec82[1] - fConst75 * (fTemp505 - fVec53[1])));
			fRec81[0] = fRec82[0] - fConst32 * (fConst31 * fRec81[2] + fConst29 * fRec81[1]);
			fRec123[0] = -(fConst77 * (fConst76 * fRec123[1] - fConst39 * (fTemp505 + fVec53[1])));
			fRec122[0] = fRec123[0] - fConst32 * (fConst31 * fRec122[2] + fConst29 * fRec122[1]);
			fRec80[0] = fConst32 * (fRec122[2] + fRec122[0] + 2.0 * fRec122[1] + fConst28 * fTemp94 * (fRec81[2] + (fRec81[0] - 2.0 * fRec81[1]))) - (fTemp93 * fRec80[2] + fTemp362) / fTemp89;
			fRec79[0] = (fTemp362 + fRec80[0] * fTemp306 + fTemp91 * fRec80[2]) / fTemp89 - (fTemp84 * fRec79[2] + fTemp361) / fTemp79;
			fRec78[0] = (fTemp361 + fRec79[0] * fTemp307 + fTemp81 * fRec79[2]) / fTemp79 - (fTemp69 * fRec78[2] + fTemp360) / fTemp65;
			fRec77[0] = (fTemp360 + fRec78[0] * fTemp308 + fTemp67 * fRec78[2]) / fTemp65 - (fTemp59 * fRec77[2] + fTemp359) / fTemp55;
			double fTemp506 = (fTemp359 + fRec77[0] * fTemp309 + fTemp57 * fRec77[2]) / fTemp55;
			fVec54[0] = fTemp506;
			fRec76[0] = fConst79 * (fConst78 * (fTemp506 - fVec54[1]) - fConst12 * fRec76[1]);
			fRec75[0] = -((fTemp311 * fRec75[1] - (fRec76[0] - fRec76[1]) / fTemp41) / fTemp50);
			fRec74[0] = fRec75[0] - (fTemp49 * fRec74[2] + 2.0 * fTemp48 * fRec74[1]) / fTemp43;
			fRec125[0] = -((fTemp311 * fRec125[1] - (fRec76[0] + fRec76[1])) / fTemp50);
			fRec124[0] = fRec125[0] - (fTemp49 * fRec124[2] + 2.0 * fTemp48 * fRec124[1]) / fTemp43;
			fRec73[0] = (fRec124[2] + fRec124[0] + 2.0 * fRec124[1] + fTemp47 * (fRec74[2] + (fRec74[0] - 2.0 * fRec74[1])) / fTemp44) / fTemp43 - (fTemp40 * fRec73[2] + fTemp358) / fTemp36;
			fRec72[0] = (fTemp358 + fRec73[0] * fTemp312 + fTemp38 * fRec73[2]) / fTemp36 - (fTemp31 * fRec72[2] + 2.0 * fTemp30 * fRec72[1]) / fTemp29;
			fRec71[0] = (fRec72[2] + fRec72[0] + 2.0 * fRec72[1]) / fTemp29 - (fTemp26 * fRec71[2] + 2.0 * fTemp25 * fRec71[1]) / fTemp19;
			double fTemp507 = fTemp313 * fTemp24 * (fRec71[2] + fRec71[0] + 2.0 * fRec71[1]) / fTemp19;
			double fTemp508 = fConst66 + fRec70[1];
			double fTemp509 = fRec70[1] - fConst66;
			fRec70[0] = ((fTemp508 < fTemp507) ? fTemp508 : ((fTemp509 > fTemp507) ? fTemp509 : fTemp507));
			fRec69[0] = fConst1 * std::fabs(fRec70[0]) + fConst2 * fRec69[1];
			double fTemp510 = fRec70[0] * fTemp5 * (1.0 - fTemp8 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec69[0] * fTemp10 / fTemp8))));
			fVec55[0] = fTemp510;
			double fTemp511 = fVec55[1] - fVec55[2];
			double fTemp512 = 0.8928571428571428 * fTemp511;
			double fTemp513 = 0.8928571428571428 * fVec55[2];
			double fTemp514 = GrungrFaustDSP_faustpower2_f(fTemp513) + 1.0;
			double fTemp515 = std::atan(fTemp513);
			double fTemp516 = 0.8928571428571428 * fVec55[1];
			double fTemp517 = GrungrFaustDSP_faustpower2_f(fTemp516) + 1.0;
			double fTemp518 = std::atan(fTemp516);
			double fTemp519 = fTemp518 * fTemp517;
			double fTemp520 = fVec55[1] * fTemp518;
			double fTemp521 = std::log(fTemp517);
			double fTemp522 = 1.7857142857142856 * fVec55[1];
			double fTemp523 = fTemp510 - fVec55[1];
			double fTemp524 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp523);
			double fTemp525 = 0.8928571428571428 * fTemp510;
			double fTemp526 = GrungrFaustDSP_faustpower2_f(fTemp525) + 1.0;
			double fTemp527 = std::atan(fTemp525);
			double fTemp528 = 0.7130141450516913 * (((fTemp524 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp525 + fTemp522)) : (0.8928571428571428 * fTemp510 * (0.8928571428571428 * (fTemp510 * fTemp527 - fTemp520) - 0.5 * (std::log(fTemp526) - fTemp521)) + 0.4464285714285714 * fTemp523 - 0.5 * (fTemp527 * fTemp526 - fTemp519)) / fTemp524) + ((GrungrFaustDSP_faustpower2_f(fTemp512) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp522 + fTemp513)) : (0.8928571428571428 * fVec55[2] * (0.5 * (fTemp521 - std::log(fTemp514)) - 0.8928571428571428 * (fTemp520 - fVec55[2] * fTemp515)) + 0.5 * (fTemp519 - fTemp515 * fTemp514) - 0.4464285714285714 * fTemp511) / GrungrFaustDSP_faustpower2_f(-fTemp512)));
			double fTemp529 = ((iTemp0) ? 0.0 : fTemp528);
			fVec56[IOTA0 & 1023] = fTemp529;
			double fTemp530 = std::fabs(fTemp529);
			int iTemp531 = (fTemp530 >= fRec129[1]) | (static_cast<double>(iRec128[1]) >= fConst3);
			iRec128[0] = ((iTemp531) ? 0 : iRec128[1] + 1);
			fRec129[0] = ((iTemp531) ? fTemp530 : fRec129[1]);
			fRec127[0] = fConst63 * fRec129[0] + fConst57 * fRec127[1];
			double fTemp532 = std::fabs(fRec127[0]);
			fRec126[0] = std::max<double>(fTemp532, fConst61 * fRec126[1] + fConst62 * fTemp532);
			double fTemp533 = ((iTemp0) ? fTemp528 : std::min<double>(1.0, 0.944 / std::max<double>(fRec126[0], 2.220446049250313e-16)) * fVec56[(IOTA0 - iConst81) & 1023]);
			double fTemp534 = ((iTemp0) ? 0.0 : fTemp533);
			double fTemp535 = std::fabs(fTemp534);
			fVec57[0] = fTemp535;
			double fTemp536 = std::max<double>(fTemp535, fVec57[1]);
			fVec58[0] = fTemp536;
			double fTemp537 = std::max<double>(fTemp536, fVec58[2]);
			fVec59[0] = fTemp537;
			double fTemp538 = std::max<double>(fTemp537, fVec59[4]);
			fVec60[0] = fTemp538;
			double fTemp539 = std::max<double>(fTemp538, fVec60[8]);
			fVec61[IOTA0 & 31] = fTemp539;
			double fTemp540 = std::max<double>(fTemp539, fVec61[(IOTA0 - 16) & 31]);
			fVec62[IOTA0 & 63] = fTemp540;
			double fTemp541 = std::max<double>(fTemp540, fVec62[(IOTA0 - 32) & 63]);
			fVec63[IOTA0 & 127] = fTemp541;
			double fTemp542 = std::max<double>(fTemp541, fVec63[(IOTA0 - 64) & 127]);
			fVec64[IOTA0 & 255] = fTemp542;
			double fTemp543 = std::max<double>(fTemp542, fVec64[(IOTA0 - 128) & 255]);
			fVec65[IOTA0 & 511] = fTemp543;
			double fTemp544 = std::max<double>(fTemp543, fVec65[(IOTA0 - 256) & 511]);
			fVec66[IOTA0 & 1023] = fTemp544;
			fVec67[IOTA0 & 2047] = std::max<double>(fTemp544, fVec66[(IOTA0 - 512) & 1023]);
			double fTemp545 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp535 : -1.7976931348623157e+308), ((iConst82) ? fVec58[iConst4] : -1.7976931348623157e+308)), ((iConst83) ? fVec59[iConst84] : -1.7976931348623157e+308)), ((iConst85) ? fVec60[iConst86] : -1.7976931348623157e+308)), ((iConst87) ? fVec61[(IOTA0 - iConst88) & 31] : -1.7976931348623157e+308)), ((iConst89) ? fVec62[(IOTA0 - iConst90) & 63] : -1.7976931348623157e+308)), ((iConst91) ? fVec63[(IOTA0 - iConst92) & 127] : -1.7976931348623157e+308)), ((iConst93) ? fVec64[(IOTA0 - iConst94) & 255] : -1.7976931348623157e+308)), ((iConst95) ? fVec65[(IOTA0 - iConst96) & 511] : -1.7976931348623157e+308)), ((iConst97) ? fVec66[(IOTA0 - iConst98) & 1023] : -1.7976931348623157e+308)), ((iConst99) ? fVec67[(IOTA0 - iConst100) & 2047] : -1.7976931348623157e+308));
			double fTemp546 = ((fTemp545 > fRec68[1]) ? fConst102 : fConst101);
			fRec68[0] = fTemp545 * (1.0 - fTemp546) + fRec68[1] * fTemp546;
			double fTemp547 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec68[0]));
			int iTemp548 = (fTemp547 > -53.0) + (fTemp547 > -47.0);
			double fTemp549 = ((iTemp0) ? fTemp533 : fTemp534 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp548 == 0) ? fTemp547 + 5e+01 : ((iTemp548 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp547 + 47.0)) : 0.0)))));
			double fTemp550 = ((iTemp0) ? 0.0 : fTemp549);
			output1[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp363 : ((iTemp0) ? fTemp549 : static_cast<double>((fTemp550 > 0.0) - (fTemp550 < 0.0)) * std::pow(std::fabs(fTemp550), 0.97))));
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec6[1] = fRec6[0];
			fVec0[1] = fVec0[0];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fVec1[1] = fVec1[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fVec6[1] = fVec6[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fRec35[1] = fRec35[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fVec9[1] = fVec9[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fVec10[1] = fVec10[0];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fVec11[1] = fVec11[0];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fVec13[2] = fVec13[1];
			fVec13[1] = fVec13[0];
			fVec14[2] = fVec14[1];
			fVec14[1] = fVec14[0];
			fVec15[2] = fVec15[1];
			fVec15[1] = fVec15[0];
			fVec16[1] = fVec16[0];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec53[1] = fRec53[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fVec17[1] = fVec17[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fVec18[1] = fVec18[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec56[1] = fRec56[0];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fVec19[1] = fVec19[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fVec20[1] = fVec20[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec62[1] = fRec62[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec63[1] = fRec63[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fVec21[2] = fVec21[1];
			fVec21[1] = fVec21[0];
			IOTA0 = IOTA0 + 1;
			iRec66[1] = iRec66[0];
			fRec67[1] = fRec67[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fVec23[1] = fVec23[0];
			fVec24[2] = fVec24[1];
			fVec24[1] = fVec24[0];
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fVec25[j0] = fVec25[j0 - 1];
			}
			for (int j1 = 11; j1 > 0; j1 = j1 - 1) {
				fVec26[j1] = fVec26[j1 - 1];
			}
			fRec1[1] = fRec1[0];
			fVec34[1] = fVec34[0];
			fRec111[1] = fRec111[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fVec35[1] = fVec35[0];
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
			fRec107[1] = fRec107[0];
			fVec36[2] = fVec36[1];
			fVec36[1] = fVec36[0];
			fVec37[1] = fVec37[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fVec38[2] = fVec38[1];
			fVec38[1] = fVec38[0];
			fVec39[1] = fVec39[0];
			fRec104[1] = fRec104[0];
			fRec103[2] = fRec103[1];
			fRec103[1] = fRec103[0];
			fVec40[1] = fVec40[0];
			fRec102[1] = fRec102[0];
			fRec101[1] = fRec101[0];
			fRec100[1] = fRec100[0];
			fVec41[2] = fVec41[1];
			fVec41[1] = fVec41[0];
			fVec42[1] = fVec42[0];
			fRec99[1] = fRec99[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fVec43[1] = fVec43[0];
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
			fRec95[1] = fRec95[0];
			fVec44[1] = fVec44[0];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec113[1] = fRec113[0];
			fRec112[2] = fRec112[1];
			fRec112[1] = fRec112[0];
			fVec45[1] = fVec45[0];
			fRec92[1] = fRec92[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec115[1] = fRec115[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fVec46[2] = fVec46[1];
			fVec46[1] = fVec46[0];
			fVec47[2] = fVec47[1];
			fVec47[1] = fVec47[0];
			fVec48[2] = fVec48[1];
			fVec48[1] = fVec48[0];
			fVec49[2] = fVec49[1];
			fVec49[1] = fVec49[0];
			fVec50[1] = fVec50[0];
			fRec90[1] = fRec90[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec117[1] = fRec117[0];
			fRec116[2] = fRec116[1];
			fRec116[1] = fRec116[0];
			fVec51[1] = fVec51[0];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec119[1] = fRec119[0];
			fRec118[2] = fRec118[1];
			fRec118[1] = fRec118[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fVec52[1] = fVec52[0];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec121[1] = fRec121[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fVec53[1] = fVec53[0];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec123[1] = fRec123[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fVec54[1] = fVec54[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec125[1] = fRec125[0];
			fRec124[2] = fRec124[1];
			fRec124[1] = fRec124[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec69[1] = fRec69[0];
			fVec55[2] = fVec55[1];
			fVec55[1] = fVec55[0];
			iRec128[1] = iRec128[0];
			fRec129[1] = fRec129[0];
			fRec127[1] = fRec127[0];
			fRec126[1] = fRec126[0];
			fVec57[1] = fVec57[0];
			fVec58[2] = fVec58[1];
			fVec58[1] = fVec58[0];
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fVec59[j2] = fVec59[j2 - 1];
			}
			for (int j3 = 11; j3 > 0; j3 = j3 - 1) {
				fVec60[j3] = fVec60[j3 - 1];
			}
			fRec68[1] = fRec68[0];
		}
	}

};

#endif
