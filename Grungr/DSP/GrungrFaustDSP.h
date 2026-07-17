/* ------------------------------------------------------------
author: "cloph"
name: "Grungr"
version: "2.7.0"
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
	
	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	FAUSTFLOAT fHslider0;
	double fRec0[2];
	int iConst3;
	double fRec2[2];
	int iConst4;
	FAUSTFLOAT fHslider1;
	double fRec4[2];
	double fConst5;
	FAUSTFLOAT fHslider2;
	double fRec8[2];
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
	double fVec0[2];
	double fRec55[2];
	double fRec54[3];
	double fVec1[2];
	double fConst76;
	double fConst77;
	double fConst78;
	double fRec53[2];
	double fConst79;
	double fConst80;
	double fRec52[2];
	double fConst81;
	double fRec51[2];
	double fRec50[3];
	double fConst82;
	double fConst83;
	double fConst84;
	double fRec49[2];
	double fConst85;
	double fRec48[2];
	double fVec2[3];
	double fVec3[2];
	double fConst86;
	double fConst87;
	double fRec47[2];
	double fConst88;
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
	double fConst89;
	double fVec9[2];
	double fRec37[2];
	double fRec36[2];
	double fRec35[2];
	double fVec10[2];
	double fConst90;
	double fConst91;
	double fRec34[2];
	double fRec33[3];
	double fRec57[2];
	double fRec56[3];
	double fVec11[2];
	double fConst92;
	double fConst93;
	double fConst94;
	double fRec32[2];
	double fRec31[3];
	double fRec59[2];
	double fRec58[3];
	double fVec12[3];
	double fConst95;
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
	double fConst96;
	double fConst97;
	double fRec24[2];
	double fRec23[3];
	FAUSTFLOAT fHslider3;
	double fRec64[2];
	double fRec66[2];
	double fRec65[3];
	double fVec19[2];
	double fConst98;
	double fConst99;
	double fConst100;
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
	double fConst101;
	double fConst102;
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
	double fVec22[2];
	int iConst103;
	double fVec23[3];
	int iConst104;
	double fVec24[5];
	int iConst105;
	int iConst106;
	double fVec25[12];
	int iConst107;
	int iConst108;
	int IOTA0;
	double fVec26[32];
	int iConst109;
	int iConst110;
	double fVec27[64];
	int iConst111;
	int iConst112;
	double fVec28[128];
	int iConst113;
	int iConst114;
	double fVec29[256];
	int iConst115;
	int iConst116;
	double fVec30[512];
	int iConst117;
	int iConst118;
	double fVec31[1024];
	int iConst119;
	int iConst120;
	double fVec32[2048];
	int iConst121;
	double fRec3[2];
	double fVec33[2];
	double fVec34[3];
	double fVec35[5];
	int iConst122;
	double fVec36[12];
	int iConst123;
	double fVec37[32];
	int iConst124;
	double fVec38[64];
	int iConst125;
	double fVec39[128];
	int iConst126;
	double fVec40[256];
	int iConst127;
	double fVec41[512];
	int iConst128;
	double fVec42[1024];
	int iConst129;
	double fVec43[2048];
	int iConst130;
	double fConst131;
	double fRec1[2];
	FAUSTFLOAT fCheckbox0;
	double fRec72[2];
	double fVec44[2];
	double fRec124[2];
	double fRec123[3];
	double fVec45[2];
	double fRec122[2];
	double fRec121[2];
	double fRec120[2];
	double fRec119[3];
	double fRec118[2];
	double fRec117[2];
	double fVec46[3];
	double fVec47[2];
	double fRec116[2];
	double fRec115[2];
	double fVec48[3];
	double fVec49[2];
	double fRec114[2];
	double fRec113[3];
	double fVec50[2];
	double fRec112[2];
	double fRec111[2];
	double fRec110[2];
	double fVec51[3];
	double fVec52[2];
	double fRec109[2];
	double fRec108[3];
	double fRec107[3];
	double fVec53[2];
	double fRec106[2];
	double fRec105[2];
	double fRec104[2];
	double fVec54[2];
	double fRec103[2];
	double fRec102[3];
	double fRec126[2];
	double fRec125[3];
	double fVec55[2];
	double fRec101[2];
	double fRec100[3];
	double fRec128[2];
	double fRec127[3];
	double fVec56[3];
	double fVec57[3];
	double fVec58[3];
	double fVec59[3];
	double fVec60[2];
	double fRec99[2];
	double fRec98[3];
	double fRec130[2];
	double fRec129[3];
	double fVec61[2];
	double fRec97[2];
	double fRec96[3];
	double fRec132[2];
	double fRec131[3];
	double fRec95[3];
	double fVec62[2];
	double fRec94[2];
	double fRec93[2];
	double fRec92[3];
	double fRec134[2];
	double fRec133[3];
	double fVec63[2];
	double fRec91[2];
	double fRec90[3];
	double fRec136[2];
	double fRec135[3];
	double fRec89[3];
	double fRec88[3];
	double fRec87[3];
	double fRec86[3];
	double fRec85[3];
	double fRec84[3];
	double fVec64[2];
	double fRec83[2];
	double fRec82[2];
	double fRec81[3];
	double fRec138[2];
	double fRec137[3];
	double fRec80[3];
	double fRec79[3];
	double fRec78[3];
	double fRec77[3];
	double fRec76[2];
	double fRec75[2];
	double fVec65[3];
	double fVec66[2];
	double fVec67[3];
	double fVec68[5];
	double fVec69[12];
	double fVec70[32];
	double fVec71[64];
	double fVec72[128];
	double fVec73[256];
	double fVec74[512];
	double fVec75[1024];
	double fVec76[2048];
	double fRec74[2];
	double fVec77[2];
	double fVec78[3];
	double fVec79[5];
	double fVec80[12];
	double fVec81[32];
	double fVec82[64];
	double fVec83[128];
	double fVec84[256];
	double fVec85[512];
	double fVec86[1024];
	double fVec87[2048];
	double fRec73[2];
	
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
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("compressors.lib/expanderSC_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expanderSC_N_chan:license", "GPLv3");
		m->declare("compressors.lib/expander_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expander_N_chan:license", "GPLv3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("description", "Three-knob drive effect with RAW, MOD, and BASS voicings plus a click-free stomp bypass.");
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
		m->declare("version", "2.7.0");
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
		iConst3 = static_cast<int>(std::floor(0.02 * fConst0)) % 2;
		iConst4 = static_cast<int>(std::floor(0.01 * fConst0)) % 2;
		fConst5 = 3.141592653589793 / fConst0;
		fConst6 = 9424.77796076938 / (fConst0 * std::sin(28274.33388230814 / fConst0));
		fConst7 = std::tan(14137.16694115407 / fConst0);
		fConst8 = 1.0 / fConst7;
		fConst9 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst7));
		fConst10 = std::tan(94.24777960769379 / fConst0);
		fConst11 = 1.0 / fConst10;
		fConst12 = 1.0 - fConst11;
		fConst13 = 785.3981633974482 / (fConst0 * std::sin(1570.7963267948965 / fConst0));
		fConst14 = std::tan(785.3981633974482 / fConst0);
		fConst15 = 1.0 / fConst14;
		fConst16 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst14));
		fConst17 = fConst0 * std::sin(345.57519189487726 / fConst0);
		fConst18 = 246.83942278205518 / fConst17;
		fConst19 = std::tan(172.78759594743863 / fConst0);
		fConst20 = 1.0 / fConst19;
		fConst21 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst19));
		fConst22 = 913.9178628624852 / (fConst0 * std::sin(2010.6192982974676 / fConst0));
		fConst23 = std::tan(1005.3096491487338 / fConst0);
		fConst24 = 1.0 / fConst23;
		fConst25 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst23));
		fConst26 = 69.11503837897544 / fConst17;
		fConst27 = 6.283185307179586 / fConst0;
		fConst28 = 4.363323129985823 / fConst0;
		fConst29 = 2050.3025739217596 / (fConst0 * std::sin(3895.5748904513434 / fConst0));
		fConst30 = std::tan(1947.7874452256717 / fConst0);
		fConst31 = 1.0 / fConst30;
		fConst32 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst30));
		fConst33 = std::tan(9424.77796076938 / fConst0);
		fConst34 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst33);
		fConst35 = 2.0 * (1.0 - fConst34);
		fConst36 = 1.0 / fConst33;
		fConst37 = (fConst36 + -1.0000000000000004) / fConst33 + 1.0;
		fConst38 = 1.0 / ((fConst36 + 1.0000000000000004) / fConst33 + 1.0);
		fConst39 = std::tan(376.99111843077515 / fConst0);
		fConst40 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst39);
		fConst41 = 2.0 * (1.0 - fConst40);
		fConst42 = 1.0 / fConst39;
		fConst43 = (fConst42 + -1.0000000000000004) / fConst39 + 1.0;
		fConst44 = (fConst42 + 1.0000000000000004) / fConst39 + 1.0;
		fConst45 = 1.0 / fConst44;
		fConst46 = std::tan(13194.68914507713 / fConst0);
		fConst47 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst46);
		fConst48 = 2.0 * (1.0 - fConst47);
		fConst49 = 1.0 / fConst46;
		fConst50 = (fConst49 + -1.0000000000000004) / fConst46 + 1.0;
		fConst51 = (fConst49 + 1.0000000000000004) / fConst46 + 1.0;
		fConst52 = 1.0 / fConst51;
		fConst53 = std::tan(10995.574287564275 / fConst0);
		fConst54 = 1.0 / GrungrFaustDSP_faustpower2_f(fConst53);
		fConst55 = 2.0 * (1.0 - fConst54);
		fConst56 = 1.0 / fConst53;
		fConst57 = (fConst56 + -1.0000000000000004) / fConst53 + 1.0;
		fConst58 = (fConst56 + 1.0000000000000004) / fConst53 + 1.0;
		fConst59 = 1.0 / fConst58;
		fConst60 = std::tan(471.23889803846896 / fConst0);
		fConst61 = 2.0 * (1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fConst60));
		fConst62 = 1.0 / fConst60;
		fConst63 = (fConst62 + -1.414213562373095) / fConst60 + 1.0;
		fConst64 = (fConst62 + 1.414213562373095) / fConst60 + 1.0;
		fConst65 = 1.0 / fConst64;
		fConst66 = std::tan(109.95574287564276 / fConst0);
		fConst67 = 1.0 / fConst66;
		fConst68 = 1.0 - fConst67;
		fConst69 = std::exp(-(2e+02 / fConst0));
		fConst70 = GrungrFaustDSP_faustpower2_f(fConst66);
		fConst71 = 2.0 * (1.0 - 1.0 / fConst70);
		fConst72 = (fConst67 + -1.414213562373095) / fConst66 + 1.0;
		fConst73 = (fConst67 + 1.414213562373095) / fConst66 + 1.0;
		fConst74 = 1.0 / fConst73;
		fConst75 = std::exp(-(1e+03 / fConst0));
		fConst76 = 1.0 / std::tan(47123.8898038469 / fConst0);
		fConst77 = 1.0 - fConst76;
		fConst78 = 1.0 / (fConst76 + 1.0);
		fConst79 = std::exp(-(2e+01 / fConst0));
		fConst80 = 1.0 - fConst79;
		fConst81 = 1.0 - fConst75;
		fConst82 = 1.0 / (fConst70 * fConst73);
		fConst83 = std::exp(-(12.5 / fConst0));
		fConst84 = 1.0 - fConst83;
		fConst85 = 1.0 - fConst69;
		fConst86 = 1.1459155902616465 / fConst66;
		fConst87 = 1.0 / (fConst67 + 1.0);
		fConst88 = 1e+06 / fConst0;
		fConst89 = 0.18 / fConst64;
		fConst90 = 1.0 - fConst56;
		fConst91 = 1.0 / (fConst56 + 1.0);
		fConst92 = 1.0 / (fConst46 * fConst58);
		fConst93 = 1.0 - fConst49;
		fConst94 = 1.0 / (fConst49 + 1.0);
		fConst95 = 0.3333333333333333 / fConst51;
		fConst96 = 1.0 - fConst42;
		fConst97 = 1.0 / (fConst42 + 1.0);
		fConst98 = 1.0 / (fConst33 * fConst44);
		fConst99 = 1.0 - fConst36;
		fConst100 = 1.0 / (fConst36 + 1.0);
		fConst101 = 0.86 / fConst10;
		fConst102 = 1.0 / (fConst11 + 1.0);
		iConst103 = static_cast<int>(std::floor(0.005 * fConst0)) % 2;
		iConst104 = static_cast<int>(std::floor(0.0025 * fConst0)) % 2;
		iConst105 = iConst4 + 2 * iConst103;
		iConst106 = static_cast<int>(std::floor(0.00125 * fConst0)) % 2;
		iConst107 = iConst105 + 4 * iConst104;
		iConst108 = static_cast<int>(std::floor(0.000625 * fConst0)) % 2;
		iConst109 = iConst107 + 8 * iConst106;
		iConst110 = static_cast<int>(std::floor(0.0003125 * fConst0)) % 2;
		iConst111 = iConst109 + 16 * iConst108;
		iConst112 = static_cast<int>(std::floor(0.00015625 * fConst0)) % 2;
		iConst113 = iConst111 + 32 * iConst110;
		iConst114 = static_cast<int>(std::floor(7.8125e-05 * fConst0)) % 2;
		iConst115 = iConst113 + 64 * iConst112;
		iConst116 = static_cast<int>(std::floor(3.90625e-05 * fConst0)) % 2;
		iConst117 = iConst115 + 128 * iConst114;
		iConst118 = static_cast<int>(std::floor(1.953125e-05 * fConst0)) % 2;
		iConst119 = iConst117 + 256 * iConst116;
		iConst120 = static_cast<int>(std::floor(9.765625e-06 * fConst0)) % 2;
		iConst121 = iConst119 + 512 * iConst118;
		iConst122 = iConst3 + 2 * iConst4;
		iConst123 = iConst122 + 4 * iConst103;
		iConst124 = iConst123 + 8 * iConst104;
		iConst125 = iConst124 + 16 * iConst106;
		iConst126 = iConst125 + 32 * iConst108;
		iConst127 = iConst126 + 64 * iConst110;
		iConst128 = iConst127 + 128 * iConst112;
		iConst129 = iConst128 + 256 * iConst114;
		iConst130 = iConst129 + 512 * iConst116;
		fConst131 = std::exp(-(8.333333333333334 / fConst0));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.0);
		fHslider1 = static_cast<FAUSTFLOAT>(0.7);
		fHslider2 = static_cast<FAUSTFLOAT>(0.56);
		fHslider3 = static_cast<FAUSTFLOAT>(0.62);
		fHslider4 = static_cast<FAUSTFLOAT>(0.72);
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0);
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
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fVec22[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fVec23[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 5; l94 = l94 + 1) {
			fVec24[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 12; l95 = l95 + 1) {
			fVec25[l95] = 0.0;
		}
		IOTA0 = 0;
		for (int l96 = 0; l96 < 32; l96 = l96 + 1) {
			fVec26[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 64; l97 = l97 + 1) {
			fVec27[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 128; l98 = l98 + 1) {
			fVec28[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 256; l99 = l99 + 1) {
			fVec29[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 512; l100 = l100 + 1) {
			fVec30[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 1024; l101 = l101 + 1) {
			fVec31[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 2048; l102 = l102 + 1) {
			fVec32[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec3[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fVec33[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 3; l105 = l105 + 1) {
			fVec34[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 5; l106 = l106 + 1) {
			fVec35[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 12; l107 = l107 + 1) {
			fVec36[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 32; l108 = l108 + 1) {
			fVec37[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 64; l109 = l109 + 1) {
			fVec38[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 128; l110 = l110 + 1) {
			fVec39[l110] = 0.0;
		}
		for (int l111 = 0; l111 < 256; l111 = l111 + 1) {
			fVec40[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 512; l112 = l112 + 1) {
			fVec41[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 1024; l113 = l113 + 1) {
			fVec42[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 2048; l114 = l114 + 1) {
			fVec43[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec1[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec72[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fVec44[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec124[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 3; l119 = l119 + 1) {
			fRec123[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fVec45[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec122[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec121[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec120[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 3; l124 = l124 + 1) {
			fRec119[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec118[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec117[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 3; l127 = l127 + 1) {
			fVec46[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fVec47[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec116[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec115[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 3; l131 = l131 + 1) {
			fVec48[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fVec49[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec114[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 3; l134 = l134 + 1) {
			fRec113[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fVec50[l135] = 0.0;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec112[l136] = 0.0;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec111[l137] = 0.0;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fRec110[l138] = 0.0;
		}
		for (int l139 = 0; l139 < 3; l139 = l139 + 1) {
			fVec51[l139] = 0.0;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fVec52[l140] = 0.0;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec109[l141] = 0.0;
		}
		for (int l142 = 0; l142 < 3; l142 = l142 + 1) {
			fRec108[l142] = 0.0;
		}
		for (int l143 = 0; l143 < 3; l143 = l143 + 1) {
			fRec107[l143] = 0.0;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fVec53[l144] = 0.0;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec106[l145] = 0.0;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec105[l146] = 0.0;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fRec104[l147] = 0.0;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fVec54[l148] = 0.0;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec103[l149] = 0.0;
		}
		for (int l150 = 0; l150 < 3; l150 = l150 + 1) {
			fRec102[l150] = 0.0;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec126[l151] = 0.0;
		}
		for (int l152 = 0; l152 < 3; l152 = l152 + 1) {
			fRec125[l152] = 0.0;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fVec55[l153] = 0.0;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fRec101[l154] = 0.0;
		}
		for (int l155 = 0; l155 < 3; l155 = l155 + 1) {
			fRec100[l155] = 0.0;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec128[l156] = 0.0;
		}
		for (int l157 = 0; l157 < 3; l157 = l157 + 1) {
			fRec127[l157] = 0.0;
		}
		for (int l158 = 0; l158 < 3; l158 = l158 + 1) {
			fVec56[l158] = 0.0;
		}
		for (int l159 = 0; l159 < 3; l159 = l159 + 1) {
			fVec57[l159] = 0.0;
		}
		for (int l160 = 0; l160 < 3; l160 = l160 + 1) {
			fVec58[l160] = 0.0;
		}
		for (int l161 = 0; l161 < 3; l161 = l161 + 1) {
			fVec59[l161] = 0.0;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fVec60[l162] = 0.0;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec99[l163] = 0.0;
		}
		for (int l164 = 0; l164 < 3; l164 = l164 + 1) {
			fRec98[l164] = 0.0;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec130[l165] = 0.0;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec129[l166] = 0.0;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fVec61[l167] = 0.0;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec97[l168] = 0.0;
		}
		for (int l169 = 0; l169 < 3; l169 = l169 + 1) {
			fRec96[l169] = 0.0;
		}
		for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
			fRec132[l170] = 0.0;
		}
		for (int l171 = 0; l171 < 3; l171 = l171 + 1) {
			fRec131[l171] = 0.0;
		}
		for (int l172 = 0; l172 < 3; l172 = l172 + 1) {
			fRec95[l172] = 0.0;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fVec62[l173] = 0.0;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec94[l174] = 0.0;
		}
		for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
			fRec93[l175] = 0.0;
		}
		for (int l176 = 0; l176 < 3; l176 = l176 + 1) {
			fRec92[l176] = 0.0;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec134[l177] = 0.0;
		}
		for (int l178 = 0; l178 < 3; l178 = l178 + 1) {
			fRec133[l178] = 0.0;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fVec63[l179] = 0.0;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec91[l180] = 0.0;
		}
		for (int l181 = 0; l181 < 3; l181 = l181 + 1) {
			fRec90[l181] = 0.0;
		}
		for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
			fRec136[l182] = 0.0;
		}
		for (int l183 = 0; l183 < 3; l183 = l183 + 1) {
			fRec135[l183] = 0.0;
		}
		for (int l184 = 0; l184 < 3; l184 = l184 + 1) {
			fRec89[l184] = 0.0;
		}
		for (int l185 = 0; l185 < 3; l185 = l185 + 1) {
			fRec88[l185] = 0.0;
		}
		for (int l186 = 0; l186 < 3; l186 = l186 + 1) {
			fRec87[l186] = 0.0;
		}
		for (int l187 = 0; l187 < 3; l187 = l187 + 1) {
			fRec86[l187] = 0.0;
		}
		for (int l188 = 0; l188 < 3; l188 = l188 + 1) {
			fRec85[l188] = 0.0;
		}
		for (int l189 = 0; l189 < 3; l189 = l189 + 1) {
			fRec84[l189] = 0.0;
		}
		for (int l190 = 0; l190 < 2; l190 = l190 + 1) {
			fVec64[l190] = 0.0;
		}
		for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
			fRec83[l191] = 0.0;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fRec82[l192] = 0.0;
		}
		for (int l193 = 0; l193 < 3; l193 = l193 + 1) {
			fRec81[l193] = 0.0;
		}
		for (int l194 = 0; l194 < 2; l194 = l194 + 1) {
			fRec138[l194] = 0.0;
		}
		for (int l195 = 0; l195 < 3; l195 = l195 + 1) {
			fRec137[l195] = 0.0;
		}
		for (int l196 = 0; l196 < 3; l196 = l196 + 1) {
			fRec80[l196] = 0.0;
		}
		for (int l197 = 0; l197 < 3; l197 = l197 + 1) {
			fRec79[l197] = 0.0;
		}
		for (int l198 = 0; l198 < 3; l198 = l198 + 1) {
			fRec78[l198] = 0.0;
		}
		for (int l199 = 0; l199 < 3; l199 = l199 + 1) {
			fRec77[l199] = 0.0;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec76[l200] = 0.0;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec75[l201] = 0.0;
		}
		for (int l202 = 0; l202 < 3; l202 = l202 + 1) {
			fVec65[l202] = 0.0;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fVec66[l203] = 0.0;
		}
		for (int l204 = 0; l204 < 3; l204 = l204 + 1) {
			fVec67[l204] = 0.0;
		}
		for (int l205 = 0; l205 < 5; l205 = l205 + 1) {
			fVec68[l205] = 0.0;
		}
		for (int l206 = 0; l206 < 12; l206 = l206 + 1) {
			fVec69[l206] = 0.0;
		}
		for (int l207 = 0; l207 < 32; l207 = l207 + 1) {
			fVec70[l207] = 0.0;
		}
		for (int l208 = 0; l208 < 64; l208 = l208 + 1) {
			fVec71[l208] = 0.0;
		}
		for (int l209 = 0; l209 < 128; l209 = l209 + 1) {
			fVec72[l209] = 0.0;
		}
		for (int l210 = 0; l210 < 256; l210 = l210 + 1) {
			fVec73[l210] = 0.0;
		}
		for (int l211 = 0; l211 < 512; l211 = l211 + 1) {
			fVec74[l211] = 0.0;
		}
		for (int l212 = 0; l212 < 1024; l212 = l212 + 1) {
			fVec75[l212] = 0.0;
		}
		for (int l213 = 0; l213 < 2048; l213 = l213 + 1) {
			fVec76[l213] = 0.0;
		}
		for (int l214 = 0; l214 < 2; l214 = l214 + 1) {
			fRec74[l214] = 0.0;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fVec77[l215] = 0.0;
		}
		for (int l216 = 0; l216 < 3; l216 = l216 + 1) {
			fVec78[l216] = 0.0;
		}
		for (int l217 = 0; l217 < 5; l217 = l217 + 1) {
			fVec79[l217] = 0.0;
		}
		for (int l218 = 0; l218 < 12; l218 = l218 + 1) {
			fVec80[l218] = 0.0;
		}
		for (int l219 = 0; l219 < 32; l219 = l219 + 1) {
			fVec81[l219] = 0.0;
		}
		for (int l220 = 0; l220 < 64; l220 = l220 + 1) {
			fVec82[l220] = 0.0;
		}
		for (int l221 = 0; l221 < 128; l221 = l221 + 1) {
			fVec83[l221] = 0.0;
		}
		for (int l222 = 0; l222 < 256; l222 = l222 + 1) {
			fVec84[l222] = 0.0;
		}
		for (int l223 = 0; l223 < 512; l223 = l223 + 1) {
			fVec85[l223] = 0.0;
		}
		for (int l224 = 0; l224 < 1024; l224 = l224 + 1) {
			fVec86[l224] = 0.0;
		}
		for (int l225 = 0; l225 < 2048; l225 = l225 + 1) {
			fVec87[l225] = 0.0;
		}
		for (int l226 = 0; l226 < 2; l226 = l226 + 1) {
			fRec73[l226] = 0.0;
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
		int iSlow0 = std::max<int>(0, std::min<int>(2, static_cast<int>(static_cast<double>(fHslider0) + 0.5)));
		double fSlow1 = fConst1 * static_cast<double>(iSlow0 == 2);
		double fSlow2 = fConst1 * static_cast<double>(iSlow0 == 1);
		double fSlow3 = fConst1 * static_cast<double>(fHslider1);
		double fSlow4 = fConst1 * static_cast<double>(fHslider2);
		double fSlow5 = fConst1 * static_cast<double>(fHslider3);
		double fSlow6 = fConst1 * static_cast<double>(fHslider4);
		double fSlow7 = fConst1 * static_cast<double>(fCheckbox0);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			int iTemp0 = static_cast<int>(1.0 - fRec0[0]);
			fRec2[0] = fSlow2 + fConst2 * fRec2[1];
			int iTemp1 = static_cast<int>(1.0 - fRec2[0]);
			fRec4[0] = fSlow3 + fConst2 * fRec4[1];
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
			double fTemp19 = std::tan(fConst5 * (1.8e+04 - 1.2e+04 * fTemp7));
			double fTemp20 = 1.0 / fTemp19;
			double fTemp21 = (fTemp20 + 1.414213562373095) / fTemp19 + 1.0;
			double fTemp22 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp19);
			double fTemp23 = (fTemp20 + -1.414213562373095) / fTemp19 + 1.0;
			fRec8[0] = fSlow4 + fConst2 * fRec8[1];
			double fTemp24 = std::pow(std::max<double>(fRec8[0], 2.220446049250313e-16), 0.88);
			double fTemp25 = fTemp24 + 1.0;
			double fTemp26 = fTemp15 * fTemp25;
			double fTemp27 = 1.46e+04 - 0.5 * fTemp26 * (9e+02 * fTemp5 + 6.5e+02 * fTemp12 + fRec2[0] * (8.5e+02 * fTemp12 + 3.5e+02));
			double fTemp28 = std::tan(fConst5 * fTemp27 * (1.0 - 0.1 * fTemp4));
			double fTemp29 = 1.0 / fTemp28;
			double fTemp30 = (fTemp29 + 1.414213562373095) / fTemp28 + 1.0;
			double fTemp31 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp28);
			double fTemp32 = (fTemp29 + -1.414213562373095) / fTemp28 + 1.0;
			double fTemp33 = std::tan(fConst5 * fTemp27);
			double fTemp34 = 1.0 / fTemp33;
			double fTemp35 = (fTemp34 + 1.414213562373095) / fTemp33 + 1.0;
			double fTemp36 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp33);
			double fTemp37 = (fTemp34 + -1.414213562373095) / fTemp33 + 1.0;
			double fTemp38 = fTemp4 * (0.6 * fTemp12 + 1.8);
			int iTemp39 = -fTemp38 > 0.0;
			double fTemp40 = fConst6 * std::pow(1e+01, 0.05 * std::fabs(-fTemp38));
			double fTemp41 = ((iTemp39) ? fConst6 : fTemp40);
			double fTemp42 = fConst8 * (fConst8 + fTemp41) + 1.0;
			double fTemp43 = ((iTemp39) ? fTemp40 : fConst6);
			double fTemp44 = fConst8 * (fConst8 - fTemp43) + 1.0;
			double fTemp45 = fConst9 * fRec11[1];
			double fTemp46 = fConst8 * (fConst8 - fTemp41) + 1.0;
			double fTemp47 = std::tan(fConst5 * (4.2e+03 - 1.7e+03 * fRec0[0]));
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
			double fTemp59 = fConst13 * std::pow(1e+01, 0.05 * std::fabs(-fTemp57));
			double fTemp60 = ((iTemp58) ? fConst13 : fTemp59);
			double fTemp61 = fConst15 * (fConst15 + fTemp60) + 1.0;
			double fTemp62 = ((iTemp58) ? fTemp59 : fConst13);
			double fTemp63 = fConst15 * (fConst15 - fTemp62) + 1.0;
			double fTemp64 = fConst16 * fRec15[1];
			double fTemp65 = fConst15 * (fConst15 - fTemp60) + 1.0;
			double fTemp66 = 1.5 * fTemp7;
			int iTemp67 = fTemp66 > 0.0;
			double fTemp68 = fConst18 * std::pow(1e+01, 0.05 * std::fabs(fTemp66));
			double fTemp69 = ((iTemp67) ? fConst18 : fTemp68);
			double fTemp70 = fConst20 * (fConst20 + fTemp69) + 1.0;
			double fTemp71 = ((iTemp67) ? fTemp68 : fConst18);
			double fTemp72 = fConst20 * (fConst20 - fTemp71) + 1.0;
			double fTemp73 = fConst21 * fRec16[1];
			double fTemp74 = fConst20 * (fConst20 - fTemp69) + 1.0;
			double fTemp75 = 2.5 * fTemp4;
			int iTemp76 = -fTemp75 > 0.0;
			double fTemp77 = fConst22 * std::pow(1e+01, 0.05 * std::fabs(-fTemp75));
			double fTemp78 = ((iTemp76) ? fConst22 : fTemp77);
			double fTemp79 = fConst24 * (fConst24 + fTemp78) + 1.0;
			double fTemp80 = ((iTemp76) ? fTemp77 : fConst22);
			double fTemp81 = fConst24 * (fConst24 - fTemp80) + 1.0;
			double fTemp82 = fConst25 * fRec17[1];
			double fTemp83 = fConst24 * (fConst24 - fTemp78) + 1.0;
			double fTemp84 = fTemp4 * fTemp2;
			double fTemp85 = 3.0 * fTemp84;
			int iTemp86 = -fTemp85 > 0.0;
			double fTemp87 = fConst26 * std::pow(1e+01, 0.05 * std::fabs(-fTemp85));
			double fTemp88 = ((iTemp86) ? fConst26 : fTemp87);
			double fTemp89 = fConst20 * (fConst20 + fTemp88) + 1.0;
			double fTemp90 = ((iTemp86) ? fTemp87 : fConst26);
			double fTemp91 = fConst20 * (fConst20 - fTemp90) + 1.0;
			double fTemp92 = fConst21 * fRec18[1];
			double fTemp93 = fConst20 * (fConst20 - fTemp88) + 1.0;
			double fTemp94 = 9.5e+02 * fTemp4 + 1.45e+03;
			double fTemp95 = std::tan(fConst5 * fTemp94);
			double fTemp96 = 0.5 * fTemp26 * (0.7 - 0.3 * fTemp5 + fRec2[0] * (0.4 * fTemp16 + 0.2));
			int iTemp97 = fTemp96 > 0.0;
			double fTemp98 = std::sin(fConst27 * fTemp94);
			double fTemp99 = fConst28 * (fTemp94 * std::pow(1e+01, 0.05 * std::fabs(fTemp96)) / fTemp98);
			double fTemp100 = fConst28 * (fTemp94 / fTemp98);
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
			double fTemp111 = fConst29 * std::pow(1e+01, 0.05 * std::fabs(fTemp109));
			double fTemp112 = ((iTemp110) ? fConst29 : fTemp111);
			double fTemp113 = fConst31 * (fConst31 + fTemp112) + 1.0;
			double fTemp114 = ((iTemp110) ? fTemp111 : fConst29);
			double fTemp115 = fConst31 * (fConst31 - fTemp114) + 1.0;
			double fTemp116 = fConst32 * fRec20[1];
			double fTemp117 = fConst31 * (fConst31 - fTemp112) + 1.0;
			double fTemp118 = std::pow(1e+01, 0.05 * (19.0 * fTemp24 + -8.0));
			double fTemp119 = std::tan(fConst5 * (1.12e+04 - (1.8e+03 * fTemp17 + 9e+02 * fTemp12 + 0.5 * fTemp25 * (fTemp52 * (9.6e+02 * fTemp12 + 4.2e+02) + fTemp7 * (7.2e+02 * fTemp13 + 3.2e+02)))));
			double fTemp120 = 1.0 / fTemp119;
			double fTemp121 = (fTemp120 + 1.414213562373095) / fTemp119 + 1.0;
			double fTemp122 = 1.0 - 1.0 / GrungrFaustDSP_faustpower2_f(fTemp119);
			double fTemp123 = (fTemp120 + -1.414213562373095) / fTemp119 + 1.0;
			double fTemp124 = 0.05 * fTemp4 * (0.5 * fTemp16 + 1.3);
			double fTemp125 = std::pow(1e+01, -fTemp124);
			double fTemp126 = std::pow(1e+01, fTemp124);
			double fTemp127 = std::max<double>(0.08 * fTemp7 + (0.24 - 0.04 * fTemp8), 2.220446049250313e-16);
			double fTemp128 = std::pow(1e+01, 0.05 * (6.0 * (fTemp17 + 1.0) + fTemp4 * (fTemp51 + 0.7) + fTemp7 * (1.5 * fTemp13 + 0.5)));
			double fTemp129 = std::tan(fConst5 * (2e+01 * fTemp17 + fTemp4 * (18.0 * fTemp5 + 1e+01) + fTemp7 * (1e+01 * fTemp5 + 6.0) + 1.1e+02));
			double fTemp130 = 1.0 / fTemp129;
			double fTemp131 = fTemp130 + 1.0;
			double fTemp132 = 1.6e+03 * fTemp17 + fTemp4 * (6.2e+02 * fTemp12 + 2.2e+02) + fTemp7 * (4.6e+02 * fTemp13 + 1.6e+02);
			double fTemp133 = std::tan(fConst5 * (8.1e+03 - fTemp132));
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
			double fTemp147 = 1.0 / std::tan(fConst5 * (8.4e+03 - fTemp132));
			double fTemp148 = fTemp147 + 1.0;
			double fTemp149 = 12.0 * fTemp5;
			double fTemp150 = std::tan(fConst5 * (25.0 * fTemp17 + fRec2[0] * (fTemp15 * (24.0 * fTemp5 + 16.0) + 12.0 * fTemp3) + fTemp7 * (fTemp149 + 8.0) + 8e+01));
			double fTemp151 = 1.0 / fTemp150;
			double fTemp152 = fTemp151 + 1.0;
			double fTemp153 = std::tan(fConst5 * (1.08e+04 - (2e+03 * fTemp17 + fTemp4 * (8.2e+02 * fTemp12 + 2.8e+02) + fTemp7 * (6e+02 * fTemp13 + 2e+02))));
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
			double fTemp166 = std::tan(fConst5 * std::max<double>(2e+01, fRec2[0] * (fTemp149 + 4e+01)));
			double fTemp167 = 1.0 / fTemp166;
			double fTemp168 = (fTemp167 + 1.414213562373095) / fTemp166 + 1.0;
			double fTemp169 = GrungrFaustDSP_faustpower2_f(fTemp166);
			double fTemp170 = fTemp169 * fTemp168;
			double fTemp171 = 1.0 - 1.0 / fTemp169;
			double fTemp172 = (fTemp167 + -1.414213562373095) / fTemp166 + 1.0;
			double fTemp173 = std::tan(fConst5 * (fTemp52 * (3e+01 * fTemp5 + 12.0) + 1.6));
			double fTemp174 = 1.0 / fTemp173;
			double fTemp175 = fTemp174 + 1.0;
			double fTemp176 = 1.0 - fTemp174;
			double fTemp177 = static_cast<double>(input0[i0]);
			fVec0[0] = fTemp177;
			fRec55[0] = (0.2 * ((fTemp177 - fVec0[1]) / fTemp173) - fRec55[1] * fTemp176) / fTemp175;
			fRec54[0] = fRec55[0] - (fRec54[2] * fTemp172 + 2.0 * fRec54[1] * fTemp171) / fTemp168;
			double fTemp178 = (fRec54[2] + (fRec54[0] - 2.0 * fRec54[1])) / fTemp170;
			fVec1[0] = fTemp178;
			fRec53[0] = -(fConst78 * (fConst77 * fRec53[1] - (fTemp178 + fVec1[1])));
			double fTemp179 = std::fabs(fRec53[0]);
			fRec52[0] = std::max<double>(fTemp179, fConst79 * fRec52[1] + fConst80 * fTemp179);
			fRec51[0] = fConst81 * fRec52[0] + fConst75 * fRec51[1];
			double fTemp180 = fRec53[0] * (1.0 - 0.7 * fTemp4 * std::min<double>(1.0, std::max<double>(0.0, fRec51[0] + -0.4)));
			fRec50[0] = fTemp180 - fConst74 * (fConst72 * fRec50[2] + fConst71 * fRec50[1]);
			double fTemp181 = std::fabs(fConst82 * (fRec50[2] + (fRec50[0] - 2.0 * fRec50[1])));
			fRec49[0] = std::max<double>(fTemp181, fConst83 * fRec49[1] + fConst84 * fTemp181);
			fRec48[0] = fConst85 * fRec49[0] + fConst69 * fRec48[1];
			double fTemp182 = (fTemp180 * (1.0 - 0.5 * fTemp7 * std::min<double>(1.0, std::max<double>(0.0, fRec48[0] + -0.5))) + 0.01) * fTemp164;
			fVec2[0] = fTemp182;
			double fTemp183 = fVec2[1] - fVec2[2];
			double fTemp184 = 0.5555555555555556 * fTemp183;
			double fTemp185 = 0.5555555555555556 * fVec2[2];
			double fTemp186 = GrungrFaustDSP_faustpower2_f(fTemp185) + 1.0;
			double fTemp187 = std::atan(fTemp185);
			double fTemp188 = 0.5555555555555556 * fVec2[1];
			double fTemp189 = GrungrFaustDSP_faustpower2_f(fTemp188) + 1.0;
			double fTemp190 = std::atan(fTemp188);
			double fTemp191 = fTemp190 * fTemp189;
			double fTemp192 = fVec2[1] * fTemp190;
			double fTemp193 = std::log(fTemp189);
			double fTemp194 = 1.1111111111111112 * fVec2[1];
			double fTemp195 = fTemp182 - fVec2[1];
			double fTemp196 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp195);
			double fTemp197 = 0.5555555555555556 * fTemp182;
			double fTemp198 = GrungrFaustDSP_faustpower2_f(fTemp197) + 1.0;
			double fTemp199 = std::atan(fTemp197);
			double fTemp200 = ((fTemp196 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp197 + fTemp194)) : (0.5555555555555556 * fTemp182 * (0.5555555555555556 * (fTemp182 * fTemp199 - fTemp192) - 0.5 * (std::log(fTemp198) - fTemp193)) + 0.2777777777777778 * fTemp195 - 0.5 * (fTemp199 * fTemp198 - fTemp191)) / fTemp196) + ((GrungrFaustDSP_faustpower2_f(fTemp184) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp194 + fTemp185)) : (0.5555555555555556 * fVec2[2] * (0.5 * (fTemp193 - std::log(fTemp186)) - 0.5555555555555556 * (fTemp192 - fVec2[2] * fTemp187)) + 0.5 * (fTemp191 - fTemp187 * fTemp186) - 0.2777777777777778 * fTemp183) / GrungrFaustDSP_faustpower2_f(-fTemp184)) - fTemp165;
			fVec3[0] = fTemp200;
			fRec47[0] = fConst87 * (fConst86 * (fTemp200 - fVec3[1]) - fConst68 * fRec47[1]);
			double fTemp201 = fRec47[0] * fTemp163;
			double fTemp202 = fConst88 + fRec46[1];
			double fTemp203 = fRec46[1] - fConst88;
			fRec46[0] = ((fTemp202 < fTemp201) ? fTemp202 : ((fTemp203 > fTemp201) ? fTemp203 : fTemp201));
			double fTemp204 = (fRec46[0] + 0.014) * fTemp161 * fTemp160;
			double fTemp205 = fTemp204 / fTemp159;
			fVec4[0] = fTemp205;
			double fTemp206 = GrungrFaustDSP_faustpower2_f(fVec4[2]);
			double fTemp207 = std::atan(fVec4[2]);
			double fTemp208 = GrungrFaustDSP_faustpower2_f(fVec4[1]);
			double fTemp209 = std::atan(fVec4[1]);
			double fTemp210 = fTemp209 - (fVec4[1] - fTemp209 * fTemp208);
			double fTemp211 = fVec4[1] * fTemp209;
			double fTemp212 = std::log(fTemp208 + 1.0);
			double fTemp213 = 2.0 * fVec4[1];
			double fTemp214 = GrungrFaustDSP_faustpower2_f(fTemp205 - fVec4[1]);
			double fTemp215 = GrungrFaustDSP_faustpower2_f(fTemp205);
			double fTemp216 = std::atan(fTemp205);
			double fTemp217 = fTemp159 * (((fTemp214 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp205 + fTemp213)) : (fTemp204 * (fTemp204 * fTemp216 / fTemp159 - (fTemp211 + 0.5 * (std::log(fTemp215 + 1.0) - fTemp212))) / fTemp159 - 0.5 * (fTemp216 - (fTemp205 - fTemp216 * fTemp215) - fTemp210)) / fTemp214) + ((GrungrFaustDSP_faustpower2_f(fVec4[1] - fVec4[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp213 + fVec4[2])) : (fVec4[2] * (fVec4[2] * fTemp207 + 0.5 * (fTemp212 - std::log(fTemp206 + 1.0)) - fTemp211) + 0.5 * (fTemp210 - (fTemp207 - (fVec4[2] - fTemp207 * fTemp206)))) / GrungrFaustDSP_faustpower2_f(fVec4[2] - fVec4[1])) - fTemp162);
			fVec5[0] = fTemp217;
			fRec45[0] = 0.995 * fRec45[1] + 0.6366197723675814 * (fTemp217 - fVec5[1]);
			fRec44[0] = fRec45[0] - (fRec44[2] * fTemp157 + 2.0 * fRec44[1] * fTemp156) / fTemp155;
			double fTemp218 = (fRec44[2] + fRec44[0] + 2.0 * fRec44[1]) / fTemp155;
			fVec6[0] = fTemp218;
			double fTemp219 = 1.0 - fTemp151;
			fRec43[0] = -((fRec43[1] * fTemp219 - (fTemp218 - fVec6[1]) / fTemp150) / fTemp152);
			double fTemp220 = 1.0 - fTemp147;
			fRec42[0] = -((fRec42[1] * fTemp220 - (fRec43[0] + fRec43[1])) / fTemp148);
			double fTemp221 = fRec42[0] * fTemp146;
			double fTemp222 = fConst88 + fRec41[1];
			double fTemp223 = fRec41[1] - fConst88;
			fRec41[0] = ((fTemp222 < fTemp221) ? fTemp222 : ((fTemp223 > fTemp221) ? fTemp223 : fTemp221));
			double fTemp224 = (fRec41[0] + fTemp143) * fTemp142 * fTemp141;
			double fTemp225 = fTemp224 / fTemp140;
			fVec7[0] = fTemp225;
			double fTemp226 = GrungrFaustDSP_faustpower2_f(fVec7[2]);
			double fTemp227 = std::atan(fVec7[2]);
			double fTemp228 = GrungrFaustDSP_faustpower2_f(fVec7[1]);
			double fTemp229 = std::atan(fVec7[1]);
			double fTemp230 = fTemp229 - (fVec7[1] - fTemp229 * fTemp228);
			double fTemp231 = fVec7[1] * fTemp229;
			double fTemp232 = std::log(fTemp228 + 1.0);
			double fTemp233 = 2.0 * fVec7[1];
			double fTemp234 = GrungrFaustDSP_faustpower2_f(fTemp225 - fVec7[1]);
			double fTemp235 = GrungrFaustDSP_faustpower2_f(fTemp225);
			double fTemp236 = std::atan(fTemp225);
			double fTemp237 = fTemp140 * (((fTemp234 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp225 + fTemp233)) : (fTemp224 * (fTemp224 * fTemp236 / fTemp140 - (fTemp231 + 0.5 * (std::log(fTemp235 + 1.0) - fTemp232))) / fTemp140 - 0.5 * (fTemp236 - (fTemp225 - fTemp236 * fTemp235) - fTemp230)) / fTemp234) + ((GrungrFaustDSP_faustpower2_f(fVec7[1] - fVec7[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp233 + fVec7[2])) : (fVec7[2] * (fVec7[2] * fTemp227 + 0.5 * (fTemp232 - std::log(fTemp226 + 1.0)) - fTemp231) + 0.5 * (fTemp230 - (fTemp227 - (fVec7[2] - fTemp227 * fTemp226)))) / GrungrFaustDSP_faustpower2_f(fVec7[2] - fVec7[1])) - fTemp144);
			fVec8[0] = fTemp237;
			fRec40[0] = 0.995 * fRec40[1] + 0.6366197723675814 * (fTemp237 - fVec8[1]);
			fRec39[0] = fRec40[0] - (fRec39[2] * fTemp137 + 2.0 * fRec39[1] * fTemp136) / fTemp135;
			double fTemp238 = (fRec39[2] + fRec39[0] + 2.0 * fRec39[1]) / fTemp135;
			double fTemp239 = fTemp238 + 0.05 * fTemp4 * tanh(0.4 * fTemp238);
			double fTemp240 = tanh(1.6 * fTemp239);
			fRec38[0] = tanh(1.4 * std::fabs(fTemp239 + 0.2 * fTemp7 * fTemp240)) - fConst65 * (fConst63 * fRec38[2] + fConst61 * fRec38[1]);
			double fTemp241 = fTemp239 + fTemp7 * (0.2 * fTemp240 + fConst89 * (fRec38[2] + fRec38[0] + 2.0 * fRec38[1]));
			fVec9[0] = fTemp241;
			double fTemp242 = 1.0 - fTemp130;
			fRec37[0] = -((fRec37[1] * fTemp242 - (fTemp241 - fVec9[1]) / fTemp129) / fTemp131);
			double fTemp243 = fRec37[0] * fTemp128;
			double fTemp244 = fConst88 + fRec36[1];
			double fTemp245 = fRec36[1] - fConst88;
			fRec36[0] = ((fTemp244 < fTemp243) ? fTemp244 : ((fTemp245 > fTemp243) ? fTemp245 : fTemp243));
			fRec35[0] = fConst1 * std::fabs(fRec36[0]) + fConst2 * fRec35[1];
			double fTemp246 = fRec36[0] * (1.0 - fTemp127 * std::min<double>(1.0, std::max<double>(0.0, fRec35[0] * fTemp11 / fTemp127)));
			fVec10[0] = fTemp246;
			fRec34[0] = -(fConst91 * (fConst90 * fRec34[1] - fConst56 * (fTemp246 - fVec10[1])));
			fRec33[0] = fRec34[0] - fConst59 * (fConst57 * fRec33[2] + fConst55 * fRec33[1]);
			fRec57[0] = -(fConst91 * (fConst90 * fRec57[1] - (fTemp246 + fVec10[1])));
			fRec56[0] = fRec57[0] - fConst59 * (fConst57 * fRec56[2] + fConst55 * fRec56[1]);
			double fTemp247 = fRec56[2] + fRec56[0] + 2.0 * fRec56[1] + fConst54 * (fRec33[2] + (fRec33[0] - 2.0 * fRec33[1])) * fTemp126;
			fVec11[0] = fTemp247;
			fRec32[0] = -(fConst94 * (fConst93 * fRec32[1] - fConst92 * (fTemp247 - fVec11[1])));
			fRec31[0] = fRec32[0] - fConst52 * (fConst50 * fRec31[2] + fConst48 * fRec31[1]);
			fRec59[0] = -(fConst94 * (fConst93 * fRec59[1] - fConst59 * (fTemp247 + fVec11[1])));
			fRec58[0] = fRec59[0] - fConst52 * (fConst50 * fRec58[2] + fConst48 * fRec58[1]);
			double fTemp248 = fRec58[2] + fRec58[0] + 2.0 * fRec58[1] + fConst47 * fTemp126 * (fRec31[2] + (fRec31[0] - 2.0 * fRec31[1]));
			int iTemp249 = (fConst52 * fTemp248) > 0.0;
			double fTemp250 = 1.08 - 0.08 * fTemp17;
			double fTemp251 = fTemp248 / fTemp250;
			fVec12[0] = fTemp251;
			double fTemp252 = fConst52 * (fVec12[1] - fVec12[2]);
			double fTemp253 = fConst52 * fVec12[2];
			int iTemp254 = (fTemp253 <= 1.0) & (fTemp253 >= -1.0);
			double fTemp255 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp253);
			double fTemp256 = static_cast<double>((fTemp253 > 0.0) - (fTemp253 < 0.0));
			double fTemp257 = fConst52 * fVec12[1];
			int iTemp258 = (fTemp257 <= 1.0) & (fTemp257 >= -1.0);
			double fTemp259 = static_cast<double>((fTemp257 > 0.0) - (fTemp257 < 0.0));
			double fTemp260 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp257);
			double fTemp261 = ((iTemp258) ? fTemp260 : fConst52 * fVec12[1] * fTemp259 + -0.5);
			double fTemp262 = ((iTemp258) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp257) : fTemp259 * (fTemp260 + -0.16666666666666666));
			double fTemp263 = 2.0 * fVec12[1];
			double fTemp264 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp251 - fVec12[1]));
			double fTemp265 = fConst52 * fTemp251;
			int iTemp266 = (fTemp265 <= 1.0) & (fTemp265 >= -1.0);
			double fTemp267 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp265);
			double fTemp268 = static_cast<double>((fTemp265 > 0.0) - (fTemp265 < 0.0));
			double fTemp269 = 1.62 - fTemp138;
			double fTemp270 = fTemp248 / fTemp269;
			fVec13[0] = fTemp270;
			double fTemp271 = fConst52 * (fVec13[1] - fVec13[2]);
			double fTemp272 = fConst52 * fVec13[2];
			int iTemp273 = (fTemp272 <= 1.0) & (fTemp272 >= -1.0);
			double fTemp274 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp272);
			double fTemp275 = static_cast<double>((fTemp272 > 0.0) - (fTemp272 < 0.0));
			double fTemp276 = fConst52 * fVec13[1];
			int iTemp277 = (fTemp276 <= 1.0) & (fTemp276 >= -1.0);
			double fTemp278 = static_cast<double>((fTemp276 > 0.0) - (fTemp276 < 0.0));
			double fTemp279 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp276);
			double fTemp280 = ((iTemp277) ? fTemp279 : fConst52 * fVec13[1] * fTemp278 + -0.5);
			double fTemp281 = ((iTemp277) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp276) : fTemp278 * (fTemp279 + -0.16666666666666666));
			double fTemp282 = 2.0 * fVec13[1];
			double fTemp283 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp270 - fVec13[1]));
			double fTemp284 = fConst52 * fTemp270;
			int iTemp285 = (fTemp284 <= 1.0) & (fTemp284 >= -1.0);
			double fTemp286 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp284);
			double fTemp287 = static_cast<double>((fTemp284 > 0.0) - (fTemp284 < 0.0));
			double fTemp288 = fTemp84 * (0.34 * fTemp12 + 0.08);
			double fTemp289 = 0.74 - fTemp158;
			double fTemp290 = fTemp248 / fTemp289;
			fVec14[0] = fTemp290;
			double fTemp291 = fConst52 * (fVec14[1] - fVec14[2]);
			double fTemp292 = fConst52 * fVec14[2];
			int iTemp293 = (fTemp292 <= 1.0) & (fTemp292 >= -1.0);
			double fTemp294 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp292);
			double fTemp295 = static_cast<double>((fTemp292 > 0.0) - (fTemp292 < 0.0));
			double fTemp296 = fConst52 * fVec14[1];
			int iTemp297 = (fTemp296 <= 1.0) & (fTemp296 >= -1.0);
			double fTemp298 = static_cast<double>((fTemp296 > 0.0) - (fTemp296 < 0.0));
			double fTemp299 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp296);
			double fTemp300 = ((iTemp297) ? fTemp299 : fConst52 * fVec14[1] * fTemp298 + -0.5);
			double fTemp301 = ((iTemp297) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp296) : fTemp298 * (fTemp299 + -0.16666666666666666));
			double fTemp302 = 2.0 * fVec14[1];
			double fTemp303 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp290 - fVec14[1]));
			double fTemp304 = fConst52 * fTemp290;
			int iTemp305 = (fTemp304 <= 1.0) & (fTemp304 >= -1.0);
			double fTemp306 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp304);
			double fTemp307 = static_cast<double>((fTemp304 > 0.0) - (fTemp304 < 0.0));
			double fTemp308 = 1.1 - 0.16 * fTemp17;
			double fTemp309 = fTemp248 / fTemp308;
			fVec15[0] = fTemp309;
			double fTemp310 = fConst52 * (fVec15[1] - fVec15[2]);
			double fTemp311 = fConst52 * fVec15[2];
			int iTemp312 = (fTemp311 <= 1.0) & (fTemp311 >= -1.0);
			double fTemp313 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp311);
			double fTemp314 = static_cast<double>((fTemp311 > 0.0) - (fTemp311 < 0.0));
			double fTemp315 = fConst52 * fVec15[1];
			int iTemp316 = (fTemp315 <= 1.0) & (fTemp315 >= -1.0);
			double fTemp317 = static_cast<double>((fTemp315 > 0.0) - (fTemp315 < 0.0));
			double fTemp318 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp315);
			double fTemp319 = ((iTemp316) ? fTemp318 : fConst52 * fVec15[1] * fTemp317 + -0.5);
			double fTemp320 = ((iTemp316) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp315) : fTemp317 * (fTemp318 + -0.16666666666666666));
			double fTemp321 = 2.0 * fVec15[1];
			double fTemp322 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp309 - fVec15[1]));
			double fTemp323 = fConst52 * fTemp309;
			int iTemp324 = (fTemp323 <= 1.0) & (fTemp323 >= -1.0);
			double fTemp325 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp323);
			double fTemp326 = static_cast<double>((fTemp323 > 0.0) - (fTemp323 < 0.0));
			double fTemp327 = 1.0 - fTemp288;
			double fTemp328 = fTemp327 * ((iTemp249) ? fTemp308 * (((fTemp322 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp309 + fTemp321))) : (fConst52 * (fTemp248 * (((iTemp324) ? fTemp325 : fConst52 * (fTemp248 * fTemp326 / fTemp308) + -0.5) - fTemp319) / fTemp308) + fTemp320 - ((iTemp324) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp323) : fTemp326 * (fTemp325 + -0.16666666666666666))) / fTemp322) + ((GrungrFaustDSP_faustpower2_f(fTemp310) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp321 + fVec15[2]))) : (fTemp320 + fConst52 * fVec15[2] * (((iTemp312) ? fTemp313 : fConst52 * fVec15[2] * fTemp314 + -0.5) - fTemp319) - ((iTemp312) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp311) : fTemp314 * (fTemp313 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp310))) : fTemp289 * (((fTemp303 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp290 + fTemp302))) : (fConst52 * (fTemp248 * (((iTemp305) ? fTemp306 : fConst52 * (fTemp248 * fTemp307 / fTemp289) + -0.5) - fTemp300) / fTemp289) + fTemp301 - ((iTemp305) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp304) : fTemp307 * (fTemp306 + -0.16666666666666666))) / fTemp303) + ((GrungrFaustDSP_faustpower2_f(fTemp291) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp302 + fVec14[2]))) : (fTemp301 + fConst52 * fVec14[2] * (((iTemp293) ? fTemp294 : fConst52 * fVec14[2] * fTemp295 + -0.5) - fTemp300) - ((iTemp293) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp292) : fTemp295 * (fTemp294 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp291)))) + fTemp288 * ((iTemp249) ? fTemp269 * (((fTemp283 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp270 + fTemp282))) : (fConst52 * (fTemp248 * (((iTemp285) ? fTemp286 : fConst52 * (fTemp248 * fTemp287 / fTemp269) + -0.5) - fTemp280) / fTemp269) + fTemp281 - ((iTemp285) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp284) : fTemp287 * (fTemp286 + -0.16666666666666666))) / fTemp283) + ((GrungrFaustDSP_faustpower2_f(fTemp271) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp282 + fVec13[2]))) : (fTemp281 + fConst52 * fVec13[2] * (((iTemp273) ? fTemp274 : fConst52 * fVec13[2] * fTemp275 + -0.5) - fTemp280) - ((iTemp273) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp272) : fTemp275 * (fTemp274 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp271))) : fTemp250 * (((fTemp264 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp251 + fTemp263))) : (fConst52 * (fTemp248 * (((iTemp266) ? fTemp267 : fConst52 * (fTemp248 * fTemp268 / fTemp250) + -0.5) - fTemp261) / fTemp250) + fTemp262 - ((iTemp266) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp265) : fTemp268 * (fTemp267 + -0.16666666666666666))) / fTemp264) + ((GrungrFaustDSP_faustpower2_f(fTemp252) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp263 + fVec12[2]))) : (fTemp262 + fConst52 * fVec12[2] * (((iTemp254) ? fTemp255 : fConst52 * fVec12[2] * fTemp256 + -0.5) - fTemp261) - ((iTemp254) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp253) : fTemp256 * (fTemp255 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp252))));
			fVec16[0] = fTemp328;
			fRec30[0] = -(fConst91 * (fConst90 * fRec30[1] - fConst56 * (fTemp328 - fVec16[1])));
			fRec29[0] = fRec30[0] - fConst59 * (fConst57 * fRec29[2] + fConst55 * fRec29[1]);
			fRec61[0] = -(fConst91 * (fConst90 * fRec61[1] - (fTemp328 + fVec16[1])));
			fRec60[0] = fRec61[0] - fConst59 * (fConst57 * fRec60[2] + fConst55 * fRec60[1]);
			double fTemp329 = fRec60[2] + fRec60[0] + 2.0 * fRec60[1] + fConst54 * (fRec29[2] + (fRec29[0] - 2.0 * fRec29[1])) * fTemp125;
			fVec17[0] = fTemp329;
			fRec28[0] = -(fConst94 * (fConst93 * fRec28[1] - fConst92 * (fTemp329 - fVec17[1])));
			fRec27[0] = fRec28[0] - fConst52 * (fConst50 * fRec27[2] + fConst48 * fRec27[1]);
			fRec63[0] = -(fConst94 * (fConst93 * fRec63[1] - fConst59 * (fTemp329 + fVec17[1])));
			fRec62[0] = fRec63[0] - fConst52 * (fConst50 * fRec62[2] + fConst48 * fRec62[1]);
			fRec26[0] = fConst52 * (fRec62[2] + fRec62[0] + 2.0 * fRec62[1] + fConst47 * fTemp125 * (fRec27[2] + (fRec27[0] - 2.0 * fRec27[1]))) - (fRec26[2] * fTemp123 + 2.0 * fRec26[1] * fTemp122) / fTemp121;
			double fTemp330 = (fRec26[2] + fRec26[0] + 2.0 * fRec26[1]) / fTemp121;
			fVec18[0] = fTemp330;
			fRec25[0] = 0.995 * fRec25[1] + fTemp330 - fVec18[1];
			fRec24[0] = -(fConst97 * (fConst96 * fRec24[1] - fConst42 * (fRec25[0] - fRec25[1])));
			fRec23[0] = fRec24[0] - fConst45 * (fConst43 * fRec23[2] + fConst41 * fRec23[1]);
			fRec64[0] = fSlow5 + fConst2 * fRec64[1];
			double fTemp331 = std::pow(1e+01, 0.05 * (24.0 * std::pow(std::max<double>(fRec64[0], 2.220446049250313e-16), 0.82) + -9.0));
			fRec66[0] = -(fConst97 * (fConst96 * fRec66[1] - (fRec25[0] + fRec25[1])));
			fRec65[0] = fRec66[0] - fConst45 * (fConst43 * fRec65[2] + fConst41 * fRec65[1]);
			double fTemp332 = (fRec65[2] + fRec65[0] + 2.0 * fRec65[1]) * fTemp331 + fConst40 * (fRec23[2] + (fRec23[0] - 2.0 * fRec23[1]));
			fVec19[0] = fTemp332;
			fRec22[0] = -(fConst100 * (fConst99 * fRec22[1] - fConst98 * (fTemp332 - fVec19[1])));
			fRec21[0] = fRec22[0] - fConst38 * (fConst37 * fRec21[2] + fConst35 * fRec21[1]);
			fRec68[0] = -(fConst100 * (fConst99 * fRec68[1] - fConst45 * (fTemp332 + fVec19[1])));
			fRec67[0] = fRec68[0] - fConst38 * (fConst37 * fRec67[2] + fConst35 * fRec67[1]);
			fRec20[0] = fConst38 * (fRec67[2] + fRec67[0] + 2.0 * fRec67[1] + fConst34 * (fRec21[2] + (fRec21[0] - 2.0 * fRec21[1])) * fTemp118) - (fRec20[2] * fTemp117 + fTemp116) / fTemp113;
			double fTemp333 = fConst31 * (fConst31 + fTemp114) + 1.0;
			fRec19[0] = (fTemp116 + fRec20[0] * fTemp333 + fRec20[2] * fTemp115) / fTemp113 - (fRec19[2] * fTemp108 + fTemp107) / fTemp103;
			double fTemp334 = (fTemp102 + fTemp104) / fTemp95 + 1.0;
			fRec18[0] = (fTemp107 + fRec19[0] * fTemp334 + fRec19[2] * fTemp105) / fTemp103 - (fRec18[2] * fTemp93 + fTemp92) / fTemp89;
			double fTemp335 = fConst20 * (fConst20 + fTemp90) + 1.0;
			fRec17[0] = (fTemp92 + fRec18[0] * fTemp335 + fRec18[2] * fTemp91) / fTemp89 - (fRec17[2] * fTemp83 + fTemp82) / fTemp79;
			double fTemp336 = fConst24 * (fConst24 + fTemp80) + 1.0;
			fRec16[0] = (fTemp82 + fRec17[0] * fTemp336 + fRec17[2] * fTemp81) / fTemp79 - (fRec16[2] * fTemp74 + fTemp73) / fTemp70;
			double fTemp337 = fConst20 * (fConst20 + fTemp71) + 1.0;
			fRec15[0] = (fTemp73 + fRec16[0] * fTemp337 + fRec16[2] * fTemp72) / fTemp70 - (fRec15[2] * fTemp65 + fTemp64) / fTemp61;
			double fTemp338 = fConst15 * (fConst15 + fTemp62) + 1.0;
			double fTemp339 = (fTemp64 + fRec15[0] * fTemp338 + fRec15[2] * fTemp63) / fTemp61;
			fVec20[0] = fTemp339;
			fRec14[0] = fConst102 * (fConst101 * (fTemp339 - fVec20[1]) - fConst12 * fRec14[1]);
			double fTemp340 = 1.0 - fTemp48;
			fRec13[0] = -((fRec13[1] * fTemp340 - (fRec14[0] - fRec14[1]) / fTemp47) / fTemp56);
			fRec12[0] = fRec13[0] - (fRec12[2] * fTemp55 + 2.0 * fRec12[1] * fTemp54) / fTemp49;
			fRec70[0] = -((fTemp340 * fRec70[1] - (fRec14[0] + fRec14[1])) / fTemp56);
			fRec69[0] = fRec70[0] - (fTemp55 * fRec69[2] + 2.0 * fTemp54 * fRec69[1]) / fTemp49;
			fRec11[0] = (fRec69[2] + fRec69[0] + 2.0 * fRec69[1] + (fRec12[2] + (fRec12[0] - 2.0 * fRec12[1])) * fTemp53 / fTemp50) / fTemp49 - (fRec11[2] * fTemp46 + fTemp45) / fTemp42;
			double fTemp341 = fConst8 * (fConst8 + fTemp43) + 1.0;
			fRec10[0] = (fTemp45 + fRec11[0] * fTemp341 + fRec11[2] * fTemp44) / fTemp42 - (fRec10[2] * fTemp37 + 2.0 * fRec10[1] * fTemp36) / fTemp35;
			fRec9[0] = (fRec10[2] + fRec10[0] + 2.0 * fRec10[1]) / fTemp35 - (fRec9[2] * fTemp32 + 2.0 * fRec9[1] * fTemp31) / fTemp30;
			double fTemp342 = (fRec9[2] + fRec9[0] + 2.0 * fRec9[1]) / fTemp30;
			fRec7[0] = ((iTemp0) ? 0.0 : fTemp342) - (fRec7[2] * fTemp23 + 2.0 * fRec7[1] * fTemp22) / fTemp21;
			fRec71[0] = fSlow6 + fConst2 * fRec71[1];
			double fTemp343 = std::pow(std::max<double>(fRec71[0], 2.220446049250313e-16), 1.65);
			double fTemp344 = fTemp343 * ((iTemp0) ? fTemp342 : (fRec7[2] + fRec7[0] + 2.0 * fRec7[1]) / fTemp21) * fTemp18;
			double fTemp345 = fConst88 + fRec6[1];
			double fTemp346 = fRec6[1] - fConst88;
			fRec6[0] = ((fTemp345 < fTemp344) ? fTemp345 : ((fTemp346 > fTemp344) ? fTemp346 : fTemp344));
			fRec5[0] = fConst1 * std::fabs(fRec6[0]) + fConst2 * fRec5[1];
			double fTemp347 = fRec6[0] * (1.0 - fTemp9 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec5[0] * fTemp11 / fTemp9)))) * fTemp6;
			fVec21[0] = fTemp347;
			double fTemp348 = fVec21[1] - fVec21[2];
			double fTemp349 = 0.8928571428571428 * fTemp348;
			double fTemp350 = 0.8928571428571428 * fVec21[2];
			double fTemp351 = GrungrFaustDSP_faustpower2_f(fTemp350) + 1.0;
			double fTemp352 = std::atan(fTemp350);
			double fTemp353 = 0.8928571428571428 * fVec21[1];
			double fTemp354 = GrungrFaustDSP_faustpower2_f(fTemp353) + 1.0;
			double fTemp355 = std::atan(fTemp353);
			double fTemp356 = fTemp355 * fTemp354;
			double fTemp357 = fVec21[1] * fTemp355;
			double fTemp358 = std::log(fTemp354);
			double fTemp359 = 1.7857142857142856 * fVec21[1];
			double fTemp360 = fTemp347 - fVec21[1];
			double fTemp361 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp360);
			double fTemp362 = 0.8928571428571428 * fTemp347;
			double fTemp363 = GrungrFaustDSP_faustpower2_f(fTemp362) + 1.0;
			double fTemp364 = std::atan(fTemp362);
			double fTemp365 = 0.7130141450516913 * (((fTemp361 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp362 + fTemp359)) : (0.8928571428571428 * fTemp347 * (0.8928571428571428 * (fTemp347 * fTemp364 - fTemp357) - 0.5 * (std::log(fTemp363) - fTemp358)) + 0.4464285714285714 * fTemp360 - 0.5 * (fTemp364 * fTemp363 - fTemp356)) / fTemp361) + ((GrungrFaustDSP_faustpower2_f(fTemp349) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp359 + fTemp350)) : (0.8928571428571428 * fVec21[2] * (0.5 * (fTemp358 - std::log(fTemp351)) - 0.8928571428571428 * (fTemp357 - fVec21[2] * fTemp352)) + 0.5 * (fTemp356 - fTemp352 * fTemp351) - 0.4464285714285714 * fTemp348) / GrungrFaustDSP_faustpower2_f(-fTemp349)));
			double fTemp366 = ((iTemp1) ? fTemp365 : 0.944 * tanh(1.0593220338983051 * ((iTemp1) ? 0.0 : fTemp365)));
			double fTemp367 = ((iTemp1) ? 0.0 : fTemp366);
			double fTemp368 = std::fabs(fTemp367);
			fVec22[0] = fTemp368;
			double fTemp369 = std::max<double>(fTemp368, fVec22[1]);
			fVec23[0] = fTemp369;
			double fTemp370 = std::max<double>(fTemp369, fVec23[2]);
			fVec24[0] = fTemp370;
			double fTemp371 = std::max<double>(fTemp370, fVec24[4]);
			fVec25[0] = fTemp371;
			double fTemp372 = std::max<double>(fTemp371, fVec25[8]);
			fVec26[IOTA0 & 31] = fTemp372;
			double fTemp373 = std::max<double>(fTemp372, fVec26[(IOTA0 - 16) & 31]);
			fVec27[IOTA0 & 63] = fTemp373;
			double fTemp374 = std::max<double>(fTemp373, fVec27[(IOTA0 - 32) & 63]);
			fVec28[IOTA0 & 127] = fTemp374;
			double fTemp375 = std::max<double>(fTemp374, fVec28[(IOTA0 - 64) & 127]);
			fVec29[IOTA0 & 255] = fTemp375;
			double fTemp376 = std::max<double>(fTemp375, fVec29[(IOTA0 - 128) & 255]);
			fVec30[IOTA0 & 511] = fTemp376;
			double fTemp377 = std::max<double>(fTemp376, fVec30[(IOTA0 - 256) & 511]);
			fVec31[IOTA0 & 1023] = fTemp377;
			fVec32[IOTA0 & 2047] = std::max<double>(fTemp377, fVec31[(IOTA0 - 512) & 1023]);
			double fTemp378 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp368 : -1.7976931348623157e+308), ((iConst103) ? fVec23[iConst4] : -1.7976931348623157e+308)), ((iConst104) ? fVec24[iConst105] : -1.7976931348623157e+308)), ((iConst106) ? fVec25[iConst107] : -1.7976931348623157e+308)), ((iConst108) ? fVec26[(IOTA0 - iConst109) & 31] : -1.7976931348623157e+308)), ((iConst110) ? fVec27[(IOTA0 - iConst111) & 63] : -1.7976931348623157e+308)), ((iConst112) ? fVec28[(IOTA0 - iConst113) & 127] : -1.7976931348623157e+308)), ((iConst114) ? fVec29[(IOTA0 - iConst115) & 255] : -1.7976931348623157e+308)), ((iConst116) ? fVec30[(IOTA0 - iConst117) & 511] : -1.7976931348623157e+308)), ((iConst118) ? fVec31[(IOTA0 - iConst119) & 1023] : -1.7976931348623157e+308)), ((iConst120) ? fVec32[(IOTA0 - iConst121) & 2047] : -1.7976931348623157e+308));
			double fTemp379 = ((fTemp378 > fRec3[1]) ? fConst69 : fConst83);
			fRec3[0] = fTemp378 * (1.0 - fTemp379) + fRec3[1] * fTemp379;
			double fTemp380 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec3[0]));
			int iTemp381 = (fTemp380 > -53.0) + (fTemp380 > -47.0);
			double fTemp382 = ((iTemp1) ? fTemp366 : fTemp367 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp381 == 0) ? fTemp380 + 5e+01 : ((iTemp381 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp380 + 47.0)) : 0.0)))));
			double fTemp383 = ((iTemp1) ? 0.0 : fTemp382);
			double fTemp384 = ((iTemp1) ? fTemp382 : static_cast<double>((fTemp383 > 0.0) - (fTemp383 < 0.0)) * std::pow(std::fabs(fTemp383), 0.97));
			double fTemp385 = ((iTemp0) ? fTemp384 : 0.944 * tanh(1.0593220338983051 * ((iTemp0) ? 0.0 : fTemp384)));
			double fTemp386 = ((iTemp0) ? 0.0 : fTemp385);
			double fTemp387 = std::fabs(fTemp386);
			fVec33[0] = fTemp387;
			double fTemp388 = std::max<double>(fTemp387, fVec33[1]);
			fVec34[0] = fTemp388;
			double fTemp389 = std::max<double>(fTemp388, fVec34[2]);
			fVec35[0] = fTemp389;
			double fTemp390 = std::max<double>(fTemp389, fVec35[4]);
			fVec36[0] = fTemp390;
			double fTemp391 = std::max<double>(fTemp390, fVec36[8]);
			fVec37[IOTA0 & 31] = fTemp391;
			double fTemp392 = std::max<double>(fTemp391, fVec37[(IOTA0 - 16) & 31]);
			fVec38[IOTA0 & 63] = fTemp392;
			double fTemp393 = std::max<double>(fTemp392, fVec38[(IOTA0 - 32) & 63]);
			fVec39[IOTA0 & 127] = fTemp393;
			double fTemp394 = std::max<double>(fTemp393, fVec39[(IOTA0 - 64) & 127]);
			fVec40[IOTA0 & 255] = fTemp394;
			double fTemp395 = std::max<double>(fTemp394, fVec40[(IOTA0 - 128) & 255]);
			fVec41[IOTA0 & 511] = fTemp395;
			double fTemp396 = std::max<double>(fTemp395, fVec41[(IOTA0 - 256) & 511]);
			fVec42[IOTA0 & 1023] = fTemp396;
			fVec43[IOTA0 & 2047] = std::max<double>(fTemp396, fVec42[(IOTA0 - 512) & 1023]);
			double fTemp397 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst3) ? fTemp387 : -1.7976931348623157e+308), ((iConst4) ? fVec34[iConst3] : -1.7976931348623157e+308)), ((iConst103) ? fVec35[iConst122] : -1.7976931348623157e+308)), ((iConst104) ? fVec36[iConst123] : -1.7976931348623157e+308)), ((iConst106) ? fVec37[(IOTA0 - iConst124) & 31] : -1.7976931348623157e+308)), ((iConst108) ? fVec38[(IOTA0 - iConst125) & 63] : -1.7976931348623157e+308)), ((iConst110) ? fVec39[(IOTA0 - iConst126) & 127] : -1.7976931348623157e+308)), ((iConst112) ? fVec40[(IOTA0 - iConst127) & 255] : -1.7976931348623157e+308)), ((iConst114) ? fVec41[(IOTA0 - iConst128) & 511] : -1.7976931348623157e+308)), ((iConst116) ? fVec42[(IOTA0 - iConst129) & 1023] : -1.7976931348623157e+308)), ((iConst118) ? fVec43[(IOTA0 - iConst130) & 2047] : -1.7976931348623157e+308));
			double fTemp398 = ((fTemp397 > fRec1[1]) ? fConst69 : fConst131);
			fRec1[0] = fTemp397 * (1.0 - fTemp398) + fRec1[1] * fTemp398;
			double fTemp399 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec1[0]));
			int iTemp400 = (fTemp399 > -58.0) + (fTemp399 > -52.0);
			double fTemp401 = ((iTemp0) ? fTemp385 : fTemp386 * std::pow(1e+01, 0.05 * std::max<double>(-35.0, 4.0 * ((iTemp400 == 0) ? fTemp399 + 55.0 : ((iTemp400 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp399 + 52.0)) : 0.0)))));
			double fTemp402 = ((iTemp0) ? 0.0 : fTemp401);
			fRec72[0] = fSlow7 + fConst2 * fRec72[1];
			double fTemp403 = 1.0 - fRec72[0];
			output0[i0] = static_cast<FAUSTFLOAT>(fTemp177 * fRec72[0] + fTemp403 * ((iTemp0) ? fTemp401 : static_cast<double>((fTemp402 > 0.0) - (fTemp402 < 0.0)) * std::pow(std::fabs(fTemp402), 0.97)));
			double fTemp404 = fConst9 * fRec80[1];
			double fTemp405 = fConst16 * fRec84[1];
			double fTemp406 = fConst21 * fRec85[1];
			double fTemp407 = fConst25 * fRec86[1];
			double fTemp408 = fConst21 * fRec87[1];
			double fTemp409 = 2.0 * fTemp106 * fRec88[1];
			double fTemp410 = fConst32 * fRec89[1];
			double fTemp411 = static_cast<double>(input1[i0]);
			fVec44[0] = fTemp411;
			fRec124[0] = (0.2 * ((fTemp411 - fVec44[1]) / fTemp173) - fTemp176 * fRec124[1]) / fTemp175;
			fRec123[0] = fRec124[0] - (fTemp172 * fRec123[2] + 2.0 * fTemp171 * fRec123[1]) / fTemp168;
			double fTemp412 = (fRec123[2] + (fRec123[0] - 2.0 * fRec123[1])) / fTemp170;
			fVec45[0] = fTemp412;
			fRec122[0] = -(fConst78 * (fConst77 * fRec122[1] - (fTemp412 + fVec45[1])));
			double fTemp413 = std::fabs(fRec122[0]);
			fRec121[0] = std::max<double>(fTemp413, fConst79 * fRec121[1] + fConst80 * fTemp413);
			fRec120[0] = fConst81 * fRec121[0] + fConst75 * fRec120[1];
			double fTemp414 = fRec122[0] * (1.0 - 0.7 * fTemp4 * std::min<double>(1.0, std::max<double>(0.0, fRec120[0] + -0.4)));
			fRec119[0] = fTemp414 - fConst74 * (fConst72 * fRec119[2] + fConst71 * fRec119[1]);
			double fTemp415 = std::fabs(fConst82 * (fRec119[2] + (fRec119[0] - 2.0 * fRec119[1])));
			fRec118[0] = std::max<double>(fTemp415, fConst83 * fRec118[1] + fConst84 * fTemp415);
			fRec117[0] = fConst85 * fRec118[0] + fConst69 * fRec117[1];
			double fTemp416 = fTemp164 * (fTemp414 * (1.0 - 0.5 * fTemp7 * std::min<double>(1.0, std::max<double>(0.0, fRec117[0] + -0.5))) + 0.01);
			fVec46[0] = fTemp416;
			double fTemp417 = fVec46[1] - fVec46[2];
			double fTemp418 = 0.5555555555555556 * fTemp417;
			double fTemp419 = 0.5555555555555556 * fVec46[2];
			double fTemp420 = GrungrFaustDSP_faustpower2_f(fTemp419) + 1.0;
			double fTemp421 = std::atan(fTemp419);
			double fTemp422 = 0.5555555555555556 * fVec46[1];
			double fTemp423 = GrungrFaustDSP_faustpower2_f(fTemp422) + 1.0;
			double fTemp424 = std::atan(fTemp422);
			double fTemp425 = fTemp424 * fTemp423;
			double fTemp426 = fVec46[1] * fTemp424;
			double fTemp427 = std::log(fTemp423);
			double fTemp428 = 1.1111111111111112 * fVec46[1];
			double fTemp429 = fTemp416 - fVec46[1];
			double fTemp430 = GrungrFaustDSP_faustpower2_f(0.5555555555555556 * fTemp429);
			double fTemp431 = 0.5555555555555556 * fTemp416;
			double fTemp432 = GrungrFaustDSP_faustpower2_f(fTemp431) + 1.0;
			double fTemp433 = std::atan(fTemp431);
			double fTemp434 = fTemp165 - (((fTemp430 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp431 + fTemp428)) : (0.5555555555555556 * fTemp416 * (0.5555555555555556 * (fTemp416 * fTemp433 - fTemp426) - 0.5 * (std::log(fTemp432) - fTemp427)) + 0.2777777777777778 * fTemp429 - 0.5 * (fTemp433 * fTemp432 - fTemp425)) / fTemp430) + ((GrungrFaustDSP_faustpower2_f(fTemp418) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp428 + fTemp419)) : (0.5555555555555556 * fVec46[2] * (0.5 * (fTemp427 - std::log(fTemp420)) - 0.5555555555555556 * (fTemp426 - fVec46[2] * fTemp421)) + 0.5 * (fTemp425 - fTemp421 * fTemp420) - 0.2777777777777778 * fTemp417) / GrungrFaustDSP_faustpower2_f(-fTemp418)));
			fVec47[0] = fTemp434;
			fRec116[0] = -(fConst87 * (fConst68 * fRec116[1] + fConst86 * (fTemp434 - fVec47[1])));
			double fTemp435 = fRec116[0] * fTemp163;
			double fTemp436 = fConst88 + fRec115[1];
			double fTemp437 = fRec115[1] - fConst88;
			fRec115[0] = ((fTemp436 < fTemp435) ? fTemp436 : ((fTemp437 > fTemp435) ? fTemp437 : fTemp435));
			double fTemp438 = (fRec115[0] + 0.014) * fTemp161 * fTemp160;
			double fTemp439 = fTemp438 / fTemp159;
			fVec48[0] = fTemp439;
			double fTemp440 = GrungrFaustDSP_faustpower2_f(fVec48[2]);
			double fTemp441 = std::atan(fVec48[2]);
			double fTemp442 = GrungrFaustDSP_faustpower2_f(fVec48[1]);
			double fTemp443 = std::atan(fVec48[1]);
			double fTemp444 = fTemp443 - (fVec48[1] - fTemp443 * fTemp442);
			double fTemp445 = fVec48[1] * fTemp443;
			double fTemp446 = std::log(fTemp442 + 1.0);
			double fTemp447 = 2.0 * fVec48[1];
			double fTemp448 = GrungrFaustDSP_faustpower2_f(fTemp439 - fVec48[1]);
			double fTemp449 = GrungrFaustDSP_faustpower2_f(fTemp439);
			double fTemp450 = std::atan(fTemp439);
			double fTemp451 = fTemp159 * (fTemp162 - (((fTemp448 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp439 + fTemp447)) : (fTemp438 * (fTemp438 * fTemp450 / fTemp159 - (fTemp445 + 0.5 * (std::log(fTemp449 + 1.0) - fTemp446))) / fTemp159 - 0.5 * (fTemp450 - (fTemp439 - fTemp450 * fTemp449) - fTemp444)) / fTemp448) + ((GrungrFaustDSP_faustpower2_f(fVec48[1] - fVec48[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp447 + fVec48[2])) : (fVec48[2] * (fVec48[2] * fTemp441 + 0.5 * (fTemp446 - std::log(fTemp440 + 1.0)) - fTemp445) + 0.5 * (fTemp444 - (fTemp441 - (fVec48[2] - fTemp441 * fTemp440)))) / GrungrFaustDSP_faustpower2_f(fVec48[2] - fVec48[1]))));
			fVec49[0] = fTemp451;
			fRec114[0] = 0.995 * fRec114[1] - 0.6366197723675814 * (fTemp451 - fVec49[1]);
			fRec113[0] = fRec114[0] - (fTemp157 * fRec113[2] + 2.0 * fTemp156 * fRec113[1]) / fTemp155;
			double fTemp452 = (fRec113[2] + fRec113[0] + 2.0 * fRec113[1]) / fTemp155;
			fVec50[0] = fTemp452;
			fRec112[0] = -((fTemp219 * fRec112[1] - (fTemp452 - fVec50[1]) / fTemp150) / fTemp152);
			fRec111[0] = -((fTemp220 * fRec111[1] - (fRec112[0] + fRec112[1])) / fTemp148);
			double fTemp453 = fRec111[0] * fTemp146;
			double fTemp454 = fConst88 + fRec110[1];
			double fTemp455 = fRec110[1] - fConst88;
			fRec110[0] = ((fTemp454 < fTemp453) ? fTemp454 : ((fTemp455 > fTemp453) ? fTemp455 : fTemp453));
			double fTemp456 = fTemp142 * fTemp141 * (fRec110[0] + fTemp143);
			double fTemp457 = fTemp456 / fTemp140;
			fVec51[0] = fTemp457;
			double fTemp458 = GrungrFaustDSP_faustpower2_f(fVec51[2]);
			double fTemp459 = std::atan(fVec51[2]);
			double fTemp460 = GrungrFaustDSP_faustpower2_f(fVec51[1]);
			double fTemp461 = std::atan(fVec51[1]);
			double fTemp462 = fTemp461 - (fVec51[1] - fTemp461 * fTemp460);
			double fTemp463 = fVec51[1] * fTemp461;
			double fTemp464 = std::log(fTemp460 + 1.0);
			double fTemp465 = 2.0 * fVec51[1];
			double fTemp466 = GrungrFaustDSP_faustpower2_f(fTemp457 - fVec51[1]);
			double fTemp467 = GrungrFaustDSP_faustpower2_f(fTemp457);
			double fTemp468 = std::atan(fTemp457);
			double fTemp469 = fTemp140 * (fTemp144 - (((fTemp466 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp457 + fTemp465)) : (fTemp456 * (fTemp456 * fTemp468 / fTemp140 - (fTemp463 + 0.5 * (std::log(fTemp467 + 1.0) - fTemp464))) / fTemp140 - 0.5 * (fTemp468 - (fTemp457 - fTemp468 * fTemp467) - fTemp462)) / fTemp466) + ((GrungrFaustDSP_faustpower2_f(fVec51[1] - fVec51[2]) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp465 + fVec51[2])) : (fVec51[2] * (fVec51[2] * fTemp459 + 0.5 * (fTemp464 - std::log(fTemp458 + 1.0)) - fTemp463) + 0.5 * (fTemp462 - (fTemp459 - (fVec51[2] - fTemp459 * fTemp458)))) / GrungrFaustDSP_faustpower2_f(fVec51[2] - fVec51[1]))));
			fVec52[0] = fTemp469;
			fRec109[0] = 0.995 * fRec109[1] - 0.6366197723675814 * (fTemp469 - fVec52[1]);
			fRec108[0] = fRec109[0] - (fTemp137 * fRec108[2] + 2.0 * fTemp136 * fRec108[1]) / fTemp135;
			double fTemp470 = (fRec108[2] + fRec108[0] + 2.0 * fRec108[1]) / fTemp135;
			double fTemp471 = fTemp470 + 0.05 * fTemp4 * tanh(0.4 * fTemp470);
			double fTemp472 = tanh(1.6 * fTemp471);
			fRec107[0] = tanh(1.4 * std::fabs(fTemp471 + 0.2 * fTemp7 * fTemp472)) - fConst65 * (fConst63 * fRec107[2] + fConst61 * fRec107[1]);
			double fTemp473 = fTemp471 + fTemp7 * (0.2 * fTemp472 + fConst89 * (fRec107[2] + fRec107[0] + 2.0 * fRec107[1]));
			fVec53[0] = fTemp473;
			fRec106[0] = -((fTemp242 * fRec106[1] - (fTemp473 - fVec53[1]) / fTemp129) / fTemp131);
			double fTemp474 = fRec106[0] * fTemp128;
			double fTemp475 = fConst88 + fRec105[1];
			double fTemp476 = fRec105[1] - fConst88;
			fRec105[0] = ((fTemp475 < fTemp474) ? fTemp475 : ((fTemp476 > fTemp474) ? fTemp476 : fTemp474));
			fRec104[0] = fConst1 * std::fabs(fRec105[0]) + fConst2 * fRec104[1];
			double fTemp477 = fRec105[0] * (1.0 - fTemp127 * std::min<double>(1.0, std::max<double>(0.0, fRec104[0] * fTemp11 / fTemp127)));
			fVec54[0] = fTemp477;
			fRec103[0] = -(fConst91 * (fConst90 * fRec103[1] - fConst56 * (fTemp477 - fVec54[1])));
			fRec102[0] = fRec103[0] - fConst59 * (fConst57 * fRec102[2] + fConst55 * fRec102[1]);
			fRec126[0] = -(fConst91 * (fConst90 * fRec126[1] - (fTemp477 + fVec54[1])));
			fRec125[0] = fRec126[0] - fConst59 * (fConst57 * fRec125[2] + fConst55 * fRec125[1]);
			double fTemp478 = fRec125[2] + fRec125[0] + 2.0 * fRec125[1] + fConst54 * fTemp126 * (fRec102[2] + (fRec102[0] - 2.0 * fRec102[1]));
			fVec55[0] = fTemp478;
			fRec101[0] = -(fConst94 * (fConst93 * fRec101[1] - fConst92 * (fTemp478 - fVec55[1])));
			fRec100[0] = fRec101[0] - fConst52 * (fConst50 * fRec100[2] + fConst48 * fRec100[1]);
			fRec128[0] = -(fConst94 * (fConst93 * fRec128[1] - fConst59 * (fTemp478 + fVec55[1])));
			fRec127[0] = fRec128[0] - fConst52 * (fConst50 * fRec127[2] + fConst48 * fRec127[1]);
			double fTemp479 = fRec127[2] + fRec127[0] + 2.0 * fRec127[1] + fConst47 * fTemp126 * (fRec100[2] + (fRec100[0] - 2.0 * fRec100[1]));
			int iTemp480 = (fConst52 * fTemp479) > 0.0;
			double fTemp481 = fTemp479 / fTemp250;
			fVec56[0] = fTemp481;
			double fTemp482 = fConst52 * (fVec56[1] - fVec56[2]);
			double fTemp483 = fConst52 * fVec56[2];
			int iTemp484 = (fTemp483 <= 1.0) & (fTemp483 >= -1.0);
			double fTemp485 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp483);
			double fTemp486 = static_cast<double>((fTemp483 > 0.0) - (fTemp483 < 0.0));
			double fTemp487 = fConst52 * fVec56[1];
			int iTemp488 = (fTemp487 <= 1.0) & (fTemp487 >= -1.0);
			double fTemp489 = static_cast<double>((fTemp487 > 0.0) - (fTemp487 < 0.0));
			double fTemp490 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp487);
			double fTemp491 = ((iTemp488) ? fTemp490 : fConst52 * fVec56[1] * fTemp489 + -0.5);
			double fTemp492 = ((iTemp488) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp487) : fTemp489 * (fTemp490 + -0.16666666666666666));
			double fTemp493 = 2.0 * fVec56[1];
			double fTemp494 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp481 - fVec56[1]));
			double fTemp495 = fConst52 * fTemp481;
			int iTemp496 = (fTemp495 <= 1.0) & (fTemp495 >= -1.0);
			double fTemp497 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp495);
			double fTemp498 = static_cast<double>((fTemp495 > 0.0) - (fTemp495 < 0.0));
			double fTemp499 = fTemp479 / fTemp269;
			fVec57[0] = fTemp499;
			double fTemp500 = fConst52 * (fVec57[1] - fVec57[2]);
			double fTemp501 = fConst52 * fVec57[2];
			int iTemp502 = (fTemp501 <= 1.0) & (fTemp501 >= -1.0);
			double fTemp503 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp501);
			double fTemp504 = static_cast<double>((fTemp501 > 0.0) - (fTemp501 < 0.0));
			double fTemp505 = fConst52 * fVec57[1];
			int iTemp506 = (fTemp505 <= 1.0) & (fTemp505 >= -1.0);
			double fTemp507 = static_cast<double>((fTemp505 > 0.0) - (fTemp505 < 0.0));
			double fTemp508 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp505);
			double fTemp509 = ((iTemp506) ? fTemp508 : fConst52 * fVec57[1] * fTemp507 + -0.5);
			double fTemp510 = ((iTemp506) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp505) : fTemp507 * (fTemp508 + -0.16666666666666666));
			double fTemp511 = 2.0 * fVec57[1];
			double fTemp512 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp499 - fVec57[1]));
			double fTemp513 = fConst52 * fTemp499;
			int iTemp514 = (fTemp513 <= 1.0) & (fTemp513 >= -1.0);
			double fTemp515 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp513);
			double fTemp516 = static_cast<double>((fTemp513 > 0.0) - (fTemp513 < 0.0));
			double fTemp517 = fTemp479 / fTemp289;
			fVec58[0] = fTemp517;
			double fTemp518 = fConst52 * (fVec58[1] - fVec58[2]);
			double fTemp519 = fConst52 * fVec58[2];
			int iTemp520 = (fTemp519 <= 1.0) & (fTemp519 >= -1.0);
			double fTemp521 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp519);
			double fTemp522 = static_cast<double>((fTemp519 > 0.0) - (fTemp519 < 0.0));
			double fTemp523 = fConst52 * fVec58[1];
			int iTemp524 = (fTemp523 <= 1.0) & (fTemp523 >= -1.0);
			double fTemp525 = static_cast<double>((fTemp523 > 0.0) - (fTemp523 < 0.0));
			double fTemp526 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp523);
			double fTemp527 = ((iTemp524) ? fTemp526 : fConst52 * fVec58[1] * fTemp525 + -0.5);
			double fTemp528 = ((iTemp524) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp523) : fTemp525 * (fTemp526 + -0.16666666666666666));
			double fTemp529 = 2.0 * fVec58[1];
			double fTemp530 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp517 - fVec58[1]));
			double fTemp531 = fConst52 * fTemp517;
			int iTemp532 = (fTemp531 <= 1.0) & (fTemp531 >= -1.0);
			double fTemp533 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp531);
			double fTemp534 = static_cast<double>((fTemp531 > 0.0) - (fTemp531 < 0.0));
			double fTemp535 = fTemp479 / fTemp308;
			fVec59[0] = fTemp535;
			double fTemp536 = fConst52 * (fVec59[1] - fVec59[2]);
			double fTemp537 = fConst52 * fVec59[2];
			int iTemp538 = (fTemp537 <= 1.0) & (fTemp537 >= -1.0);
			double fTemp539 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp537);
			double fTemp540 = static_cast<double>((fTemp537 > 0.0) - (fTemp537 < 0.0));
			double fTemp541 = fConst52 * fVec59[1];
			int iTemp542 = (fTemp541 <= 1.0) & (fTemp541 >= -1.0);
			double fTemp543 = static_cast<double>((fTemp541 > 0.0) - (fTemp541 < 0.0));
			double fTemp544 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp541);
			double fTemp545 = ((iTemp542) ? fTemp544 : fConst52 * fVec59[1] * fTemp543 + -0.5);
			double fTemp546 = ((iTemp542) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp541) : fTemp543 * (fTemp544 + -0.16666666666666666));
			double fTemp547 = 2.0 * fVec59[1];
			double fTemp548 = GrungrFaustDSP_faustpower2_f(fConst52 * (fTemp535 - fVec59[1]));
			double fTemp549 = fConst52 * fTemp535;
			int iTemp550 = (fTemp549 <= 1.0) & (fTemp549 >= -1.0);
			double fTemp551 = 0.5 * GrungrFaustDSP_faustpower2_f(fTemp549);
			double fTemp552 = static_cast<double>((fTemp549 > 0.0) - (fTemp549 < 0.0));
			double fTemp553 = fTemp327 * ((iTemp480) ? fTemp308 * (((fTemp548 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp535 + fTemp547))) : (fConst52 * (fTemp479 * (((iTemp550) ? fTemp551 : fConst52 * (fTemp479 * fTemp552 / fTemp308) + -0.5) - fTemp545) / fTemp308) + fTemp546 - ((iTemp550) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp549) : fTemp552 * (fTemp551 + -0.16666666666666666))) / fTemp548) + ((GrungrFaustDSP_faustpower2_f(fTemp536) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp547 + fVec59[2]))) : (fTemp546 + fConst52 * fVec59[2] * (((iTemp538) ? fTemp539 : fConst52 * fVec59[2] * fTemp540 + -0.5) - fTemp545) - ((iTemp538) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp537) : fTemp540 * (fTemp539 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp536))) : fTemp289 * (((fTemp530 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp517 + fTemp529))) : (fConst52 * (fTemp479 * (((iTemp532) ? fTemp533 : fConst52 * (fTemp479 * fTemp534 / fTemp289) + -0.5) - fTemp527) / fTemp289) + fTemp528 - ((iTemp532) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp531) : fTemp534 * (fTemp533 + -0.16666666666666666))) / fTemp530) + ((GrungrFaustDSP_faustpower2_f(fTemp518) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp529 + fVec58[2]))) : (fTemp528 + fConst52 * fVec58[2] * (((iTemp520) ? fTemp521 : fConst52 * fVec58[2] * fTemp522 + -0.5) - fTemp527) - ((iTemp520) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp519) : fTemp522 * (fTemp521 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp518)))) + fTemp288 * ((iTemp480) ? fTemp269 * (((fTemp512 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp499 + fTemp511))) : (fConst52 * (fTemp479 * (((iTemp514) ? fTemp515 : fConst52 * (fTemp479 * fTemp516 / fTemp269) + -0.5) - fTemp509) / fTemp269) + fTemp510 - ((iTemp514) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp513) : fTemp516 * (fTemp515 + -0.16666666666666666))) / fTemp512) + ((GrungrFaustDSP_faustpower2_f(fTemp500) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp511 + fVec57[2]))) : (fTemp510 + fConst52 * fVec57[2] * (((iTemp502) ? fTemp503 : fConst52 * fVec57[2] * fTemp504 + -0.5) - fTemp509) - ((iTemp502) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp501) : fTemp504 * (fTemp503 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp500))) : fTemp250 * (((fTemp494 <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp481 + fTemp493))) : (fConst52 * (fTemp479 * (((iTemp496) ? fTemp497 : fConst52 * (fTemp479 * fTemp498 / fTemp250) + -0.5) - fTemp491) / fTemp250) + fTemp492 - ((iTemp496) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp495) : fTemp498 * (fTemp497 + -0.16666666666666666))) / fTemp494) + ((GrungrFaustDSP_faustpower2_f(fTemp482) <= 0.001) ? 0.5 * std::max<double>(-1.0, std::min<double>(1.0, fConst95 * (fTemp493 + fVec56[2]))) : (fTemp492 + fConst52 * fVec56[2] * (((iTemp484) ? fTemp485 : fConst52 * fVec56[2] * fTemp486 + -0.5) - fTemp491) - ((iTemp484) ? 0.3333333333333333 * GrungrFaustDSP_faustpower3_f(fTemp483) : fTemp486 * (fTemp485 + -0.16666666666666666))) / GrungrFaustDSP_faustpower2_f(-fTemp482))));
			fVec60[0] = fTemp553;
			fRec99[0] = -(fConst91 * (fConst90 * fRec99[1] - fConst56 * (fTemp553 - fVec60[1])));
			fRec98[0] = fRec99[0] - fConst59 * (fConst57 * fRec98[2] + fConst55 * fRec98[1]);
			fRec130[0] = -(fConst91 * (fConst90 * fRec130[1] - (fTemp553 + fVec60[1])));
			fRec129[0] = fRec130[0] - fConst59 * (fConst57 * fRec129[2] + fConst55 * fRec129[1]);
			double fTemp554 = fRec129[2] + fRec129[0] + 2.0 * fRec129[1] + fConst54 * fTemp125 * (fRec98[2] + (fRec98[0] - 2.0 * fRec98[1]));
			fVec61[0] = fTemp554;
			fRec97[0] = -(fConst94 * (fConst93 * fRec97[1] - fConst92 * (fTemp554 - fVec61[1])));
			fRec96[0] = fRec97[0] - fConst52 * (fConst50 * fRec96[2] + fConst48 * fRec96[1]);
			fRec132[0] = -(fConst94 * (fConst93 * fRec132[1] - fConst59 * (fTemp554 + fVec61[1])));
			fRec131[0] = fRec132[0] - fConst52 * (fConst50 * fRec131[2] + fConst48 * fRec131[1]);
			fRec95[0] = fConst52 * (fRec131[2] + fRec131[0] + 2.0 * fRec131[1] + fConst47 * fTemp125 * (fRec96[2] + (fRec96[0] - 2.0 * fRec96[1]))) - (fTemp123 * fRec95[2] + 2.0 * fTemp122 * fRec95[1]) / fTemp121;
			double fTemp555 = (fRec95[2] + fRec95[0] + 2.0 * fRec95[1]) / fTemp121;
			fVec62[0] = fTemp555;
			fRec94[0] = 0.995 * fRec94[1] + fTemp555 - fVec62[1];
			fRec93[0] = -(fConst97 * (fConst96 * fRec93[1] - fConst42 * (fRec94[0] - fRec94[1])));
			fRec92[0] = fRec93[0] - fConst45 * (fConst43 * fRec92[2] + fConst41 * fRec92[1]);
			fRec134[0] = -(fConst97 * (fConst96 * fRec134[1] - (fRec94[0] + fRec94[1])));
			fRec133[0] = fRec134[0] - fConst45 * (fConst43 * fRec133[2] + fConst41 * fRec133[1]);
			double fTemp556 = fTemp331 * (fRec133[2] + fRec133[0] + 2.0 * fRec133[1]) + fConst40 * (fRec92[2] + (fRec92[0] - 2.0 * fRec92[1]));
			fVec63[0] = fTemp556;
			fRec91[0] = -(fConst100 * (fConst99 * fRec91[1] - fConst98 * (fTemp556 - fVec63[1])));
			fRec90[0] = fRec91[0] - fConst38 * (fConst37 * fRec90[2] + fConst35 * fRec90[1]);
			fRec136[0] = -(fConst100 * (fConst99 * fRec136[1] - fConst45 * (fTemp556 + fVec63[1])));
			fRec135[0] = fRec136[0] - fConst38 * (fConst37 * fRec135[2] + fConst35 * fRec135[1]);
			fRec89[0] = fConst38 * (fRec135[2] + fRec135[0] + 2.0 * fRec135[1] + fConst34 * fTemp118 * (fRec90[2] + (fRec90[0] - 2.0 * fRec90[1]))) - (fTemp117 * fRec89[2] + fTemp410) / fTemp113;
			fRec88[0] = (fTemp410 + fRec89[0] * fTemp333 + fTemp115 * fRec89[2]) / fTemp113 - (fTemp108 * fRec88[2] + fTemp409) / fTemp103;
			fRec87[0] = (fTemp409 + fRec88[0] * fTemp334 + fTemp105 * fRec88[2]) / fTemp103 - (fTemp93 * fRec87[2] + fTemp408) / fTemp89;
			fRec86[0] = (fTemp408 + fRec87[0] * fTemp335 + fTemp91 * fRec87[2]) / fTemp89 - (fTemp83 * fRec86[2] + fTemp407) / fTemp79;
			fRec85[0] = (fTemp407 + fRec86[0] * fTemp336 + fTemp81 * fRec86[2]) / fTemp79 - (fTemp74 * fRec85[2] + fTemp406) / fTemp70;
			fRec84[0] = (fTemp406 + fRec85[0] * fTemp337 + fTemp72 * fRec85[2]) / fTemp70 - (fTemp65 * fRec84[2] + fTemp405) / fTemp61;
			double fTemp557 = (fTemp405 + fRec84[0] * fTemp338 + fTemp63 * fRec84[2]) / fTemp61;
			fVec64[0] = fTemp557;
			fRec83[0] = fConst102 * (fConst101 * (fTemp557 - fVec64[1]) - fConst12 * fRec83[1]);
			fRec82[0] = -((fTemp340 * fRec82[1] - (fRec83[0] - fRec83[1]) / fTemp47) / fTemp56);
			fRec81[0] = fRec82[0] - (fTemp55 * fRec81[2] + 2.0 * fTemp54 * fRec81[1]) / fTemp49;
			fRec138[0] = -((fTemp340 * fRec138[1] - (fRec83[0] + fRec83[1])) / fTemp56);
			fRec137[0] = fRec138[0] - (fTemp55 * fRec137[2] + 2.0 * fTemp54 * fRec137[1]) / fTemp49;
			fRec80[0] = (fRec137[2] + fRec137[0] + 2.0 * fRec137[1] + fTemp53 * (fRec81[2] + (fRec81[0] - 2.0 * fRec81[1])) / fTemp50) / fTemp49 - (fTemp46 * fRec80[2] + fTemp404) / fTemp42;
			fRec79[0] = (fTemp404 + fRec80[0] * fTemp341 + fTemp44 * fRec80[2]) / fTemp42 - (fTemp37 * fRec79[2] + 2.0 * fTemp36 * fRec79[1]) / fTemp35;
			fRec78[0] = (fRec79[2] + fRec79[0] + 2.0 * fRec79[1]) / fTemp35 - (fTemp32 * fRec78[2] + 2.0 * fTemp31 * fRec78[1]) / fTemp30;
			double fTemp558 = (fRec78[2] + fRec78[0] + 2.0 * fRec78[1]) / fTemp30;
			fRec77[0] = ((iTemp0) ? 0.0 : fTemp558) - (fTemp23 * fRec77[2] + 2.0 * fTemp22 * fRec77[1]) / fTemp21;
			double fTemp559 = fTemp343 * fTemp18 * ((iTemp0) ? fTemp558 : (fRec77[2] + fRec77[0] + 2.0 * fRec77[1]) / fTemp21);
			double fTemp560 = fConst88 + fRec76[1];
			double fTemp561 = fRec76[1] - fConst88;
			fRec76[0] = ((fTemp560 < fTemp559) ? fTemp560 : ((fTemp561 > fTemp559) ? fTemp561 : fTemp559));
			fRec75[0] = fConst1 * std::fabs(fRec76[0]) + fConst2 * fRec75[1];
			double fTemp562 = fRec76[0] * fTemp6 * (1.0 - fTemp9 * std::min<double>(1.0, std::max<double>(0.0, 0.5 * (fRec75[0] * fTemp11 / fTemp9))));
			fVec65[0] = fTemp562;
			double fTemp563 = fVec65[1] - fVec65[2];
			double fTemp564 = 0.8928571428571428 * fTemp563;
			double fTemp565 = 0.8928571428571428 * fVec65[2];
			double fTemp566 = GrungrFaustDSP_faustpower2_f(fTemp565) + 1.0;
			double fTemp567 = std::atan(fTemp565);
			double fTemp568 = 0.8928571428571428 * fVec65[1];
			double fTemp569 = GrungrFaustDSP_faustpower2_f(fTemp568) + 1.0;
			double fTemp570 = std::atan(fTemp568);
			double fTemp571 = fTemp570 * fTemp569;
			double fTemp572 = fVec65[1] * fTemp570;
			double fTemp573 = std::log(fTemp569);
			double fTemp574 = 1.7857142857142856 * fVec65[1];
			double fTemp575 = fTemp562 - fVec65[1];
			double fTemp576 = GrungrFaustDSP_faustpower2_f(0.8928571428571428 * fTemp575);
			double fTemp577 = 0.8928571428571428 * fTemp562;
			double fTemp578 = GrungrFaustDSP_faustpower2_f(fTemp577) + 1.0;
			double fTemp579 = std::atan(fTemp577);
			double fTemp580 = 0.7130141450516913 * (((fTemp576 <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp577 + fTemp574)) : (0.8928571428571428 * fTemp562 * (0.8928571428571428 * (fTemp562 * fTemp579 - fTemp572) - 0.5 * (std::log(fTemp578) - fTemp573)) + 0.4464285714285714 * fTemp575 - 0.5 * (fTemp579 * fTemp578 - fTemp571)) / fTemp576) + ((GrungrFaustDSP_faustpower2_f(fTemp564) <= 0.001) ? 0.5 * std::atan(0.3333333333333333 * (fTemp574 + fTemp565)) : (0.8928571428571428 * fVec65[2] * (0.5 * (fTemp573 - std::log(fTemp566)) - 0.8928571428571428 * (fTemp572 - fVec65[2] * fTemp567)) + 0.5 * (fTemp571 - fTemp567 * fTemp566) - 0.4464285714285714 * fTemp563) / GrungrFaustDSP_faustpower2_f(-fTemp564)));
			double fTemp581 = ((iTemp1) ? fTemp580 : 0.944 * tanh(1.0593220338983051 * ((iTemp1) ? 0.0 : fTemp580)));
			double fTemp582 = ((iTemp1) ? 0.0 : fTemp581);
			double fTemp583 = std::fabs(fTemp582);
			fVec66[0] = fTemp583;
			double fTemp584 = std::max<double>(fTemp583, fVec66[1]);
			fVec67[0] = fTemp584;
			double fTemp585 = std::max<double>(fTemp584, fVec67[2]);
			fVec68[0] = fTemp585;
			double fTemp586 = std::max<double>(fTemp585, fVec68[4]);
			fVec69[0] = fTemp586;
			double fTemp587 = std::max<double>(fTemp586, fVec69[8]);
			fVec70[IOTA0 & 31] = fTemp587;
			double fTemp588 = std::max<double>(fTemp587, fVec70[(IOTA0 - 16) & 31]);
			fVec71[IOTA0 & 63] = fTemp588;
			double fTemp589 = std::max<double>(fTemp588, fVec71[(IOTA0 - 32) & 63]);
			fVec72[IOTA0 & 127] = fTemp589;
			double fTemp590 = std::max<double>(fTemp589, fVec72[(IOTA0 - 64) & 127]);
			fVec73[IOTA0 & 255] = fTemp590;
			double fTemp591 = std::max<double>(fTemp590, fVec73[(IOTA0 - 128) & 255]);
			fVec74[IOTA0 & 511] = fTemp591;
			double fTemp592 = std::max<double>(fTemp591, fVec74[(IOTA0 - 256) & 511]);
			fVec75[IOTA0 & 1023] = fTemp592;
			fVec76[IOTA0 & 2047] = std::max<double>(fTemp592, fVec75[(IOTA0 - 512) & 1023]);
			double fTemp593 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst4) ? fTemp583 : -1.7976931348623157e+308), ((iConst103) ? fVec67[iConst4] : -1.7976931348623157e+308)), ((iConst104) ? fVec68[iConst105] : -1.7976931348623157e+308)), ((iConst106) ? fVec69[iConst107] : -1.7976931348623157e+308)), ((iConst108) ? fVec70[(IOTA0 - iConst109) & 31] : -1.7976931348623157e+308)), ((iConst110) ? fVec71[(IOTA0 - iConst111) & 63] : -1.7976931348623157e+308)), ((iConst112) ? fVec72[(IOTA0 - iConst113) & 127] : -1.7976931348623157e+308)), ((iConst114) ? fVec73[(IOTA0 - iConst115) & 255] : -1.7976931348623157e+308)), ((iConst116) ? fVec74[(IOTA0 - iConst117) & 511] : -1.7976931348623157e+308)), ((iConst118) ? fVec75[(IOTA0 - iConst119) & 1023] : -1.7976931348623157e+308)), ((iConst120) ? fVec76[(IOTA0 - iConst121) & 2047] : -1.7976931348623157e+308));
			double fTemp594 = ((fTemp593 > fRec74[1]) ? fConst69 : fConst83);
			fRec74[0] = fTemp593 * (1.0 - fTemp594) + fRec74[1] * fTemp594;
			double fTemp595 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec74[0]));
			int iTemp596 = (fTemp595 > -53.0) + (fTemp595 > -47.0);
			double fTemp597 = ((iTemp1) ? fTemp581 : fTemp582 * std::pow(1e+01, 0.05 * std::max<double>(-3e+01, 2.0 * ((iTemp596 == 0) ? fTemp595 + 5e+01 : ((iTemp596 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp595 + 47.0)) : 0.0)))));
			double fTemp598 = ((iTemp1) ? 0.0 : fTemp597);
			double fTemp599 = ((iTemp1) ? fTemp597 : static_cast<double>((fTemp598 > 0.0) - (fTemp598 < 0.0)) * std::pow(std::fabs(fTemp598), 0.97));
			double fTemp600 = ((iTemp0) ? fTemp599 : 0.944 * tanh(1.0593220338983051 * ((iTemp0) ? 0.0 : fTemp599)));
			double fTemp601 = ((iTemp0) ? 0.0 : fTemp600);
			double fTemp602 = std::fabs(fTemp601);
			fVec77[0] = fTemp602;
			double fTemp603 = std::max<double>(fTemp602, fVec77[1]);
			fVec78[0] = fTemp603;
			double fTemp604 = std::max<double>(fTemp603, fVec78[2]);
			fVec79[0] = fTemp604;
			double fTemp605 = std::max<double>(fTemp604, fVec79[4]);
			fVec80[0] = fTemp605;
			double fTemp606 = std::max<double>(fTemp605, fVec80[8]);
			fVec81[IOTA0 & 31] = fTemp606;
			double fTemp607 = std::max<double>(fTemp606, fVec81[(IOTA0 - 16) & 31]);
			fVec82[IOTA0 & 63] = fTemp607;
			double fTemp608 = std::max<double>(fTemp607, fVec82[(IOTA0 - 32) & 63]);
			fVec83[IOTA0 & 127] = fTemp608;
			double fTemp609 = std::max<double>(fTemp608, fVec83[(IOTA0 - 64) & 127]);
			fVec84[IOTA0 & 255] = fTemp609;
			double fTemp610 = std::max<double>(fTemp609, fVec84[(IOTA0 - 128) & 255]);
			fVec85[IOTA0 & 511] = fTemp610;
			double fTemp611 = std::max<double>(fTemp610, fVec85[(IOTA0 - 256) & 511]);
			fVec86[IOTA0 & 1023] = fTemp611;
			fVec87[IOTA0 & 2047] = std::max<double>(fTemp611, fVec86[(IOTA0 - 512) & 1023]);
			double fTemp612 = std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(std::max<double>(((iConst3) ? fTemp602 : -1.7976931348623157e+308), ((iConst4) ? fVec78[iConst3] : -1.7976931348623157e+308)), ((iConst103) ? fVec79[iConst122] : -1.7976931348623157e+308)), ((iConst104) ? fVec80[iConst123] : -1.7976931348623157e+308)), ((iConst106) ? fVec81[(IOTA0 - iConst124) & 31] : -1.7976931348623157e+308)), ((iConst108) ? fVec82[(IOTA0 - iConst125) & 63] : -1.7976931348623157e+308)), ((iConst110) ? fVec83[(IOTA0 - iConst126) & 127] : -1.7976931348623157e+308)), ((iConst112) ? fVec84[(IOTA0 - iConst127) & 255] : -1.7976931348623157e+308)), ((iConst114) ? fVec85[(IOTA0 - iConst128) & 511] : -1.7976931348623157e+308)), ((iConst116) ? fVec86[(IOTA0 - iConst129) & 1023] : -1.7976931348623157e+308)), ((iConst118) ? fVec87[(IOTA0 - iConst130) & 2047] : -1.7976931348623157e+308));
			double fTemp613 = ((fTemp612 > fRec73[1]) ? fConst69 : fConst131);
			fRec73[0] = fTemp612 * (1.0 - fTemp613) + fRec73[1] * fTemp613;
			double fTemp614 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, fRec73[0]));
			int iTemp615 = (fTemp614 > -58.0) + (fTemp614 > -52.0);
			double fTemp616 = ((iTemp0) ? fTemp600 : fTemp601 * std::pow(1e+01, 0.05 * std::max<double>(-35.0, 4.0 * ((iTemp615 == 0) ? fTemp614 + 55.0 : ((iTemp615 == 1) ? -(0.08333333333333333 * GrungrFaustDSP_faustpower2_f(fTemp614 + 52.0)) : 0.0)))));
			double fTemp617 = ((iTemp0) ? 0.0 : fTemp616);
			output1[i0] = static_cast<FAUSTFLOAT>(fTemp411 * fRec72[0] + fTemp403 * ((iTemp0) ? fTemp616 : static_cast<double>((fTemp617 > 0.0) - (fTemp617 < 0.0)) * std::pow(std::fabs(fTemp617), 0.97)));
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
			fVec22[1] = fVec22[0];
			fVec23[2] = fVec23[1];
			fVec23[1] = fVec23[0];
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fVec24[j0] = fVec24[j0 - 1];
			}
			for (int j1 = 11; j1 > 0; j1 = j1 - 1) {
				fVec25[j1] = fVec25[j1 - 1];
			}
			IOTA0 = IOTA0 + 1;
			fRec3[1] = fRec3[0];
			fVec33[1] = fVec33[0];
			fVec34[2] = fVec34[1];
			fVec34[1] = fVec34[0];
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fVec35[j2] = fVec35[j2 - 1];
			}
			for (int j3 = 11; j3 > 0; j3 = j3 - 1) {
				fVec36[j3] = fVec36[j3 - 1];
			}
			fRec1[1] = fRec1[0];
			fRec72[1] = fRec72[0];
			fVec44[1] = fVec44[0];
			fRec124[1] = fRec124[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fVec45[1] = fVec45[0];
			fRec122[1] = fRec122[0];
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec118[1] = fRec118[0];
			fRec117[1] = fRec117[0];
			fVec46[2] = fVec46[1];
			fVec46[1] = fVec46[0];
			fVec47[1] = fVec47[0];
			fRec116[1] = fRec116[0];
			fRec115[1] = fRec115[0];
			fVec48[2] = fVec48[1];
			fVec48[1] = fVec48[0];
			fVec49[1] = fVec49[0];
			fRec114[1] = fRec114[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fVec50[1] = fVec50[0];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fRec110[1] = fRec110[0];
			fVec51[2] = fVec51[1];
			fVec51[1] = fVec51[0];
			fVec52[1] = fVec52[0];
			fRec109[1] = fRec109[0];
			fRec108[2] = fRec108[1];
			fRec108[1] = fRec108[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fVec53[1] = fVec53[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fRec104[1] = fRec104[0];
			fVec54[1] = fVec54[0];
			fRec103[1] = fRec103[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fRec126[1] = fRec126[0];
			fRec125[2] = fRec125[1];
			fRec125[1] = fRec125[0];
			fVec55[1] = fVec55[0];
			fRec101[1] = fRec101[0];
			fRec100[2] = fRec100[1];
			fRec100[1] = fRec100[0];
			fRec128[1] = fRec128[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fVec56[2] = fVec56[1];
			fVec56[1] = fVec56[0];
			fVec57[2] = fVec57[1];
			fVec57[1] = fVec57[0];
			fVec58[2] = fVec58[1];
			fVec58[1] = fVec58[0];
			fVec59[2] = fVec59[1];
			fVec59[1] = fVec59[0];
			fVec60[1] = fVec60[0];
			fRec99[1] = fRec99[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec130[1] = fRec130[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fVec61[1] = fVec61[0];
			fRec97[1] = fRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec132[1] = fRec132[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fVec62[1] = fVec62[0];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec134[1] = fRec134[0];
			fRec133[2] = fRec133[1];
			fRec133[1] = fRec133[0];
			fVec63[1] = fVec63[0];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec136[1] = fRec136[0];
			fRec135[2] = fRec135[1];
			fRec135[1] = fRec135[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fVec64[1] = fVec64[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec138[1] = fRec138[0];
			fRec137[2] = fRec137[1];
			fRec137[1] = fRec137[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fVec65[2] = fVec65[1];
			fVec65[1] = fVec65[0];
			fVec66[1] = fVec66[0];
			fVec67[2] = fVec67[1];
			fVec67[1] = fVec67[0];
			for (int j4 = 4; j4 > 0; j4 = j4 - 1) {
				fVec68[j4] = fVec68[j4 - 1];
			}
			for (int j5 = 11; j5 > 0; j5 = j5 - 1) {
				fVec69[j5] = fVec69[j5 - 1];
			}
			fRec74[1] = fRec74[0];
			fVec77[1] = fVec77[0];
			fVec78[2] = fVec78[1];
			fVec78[1] = fVec78[0];
			for (int j6 = 4; j6 > 0; j6 = j6 - 1) {
				fVec79[j6] = fVec79[j6 - 1];
			}
			for (int j7 = 11; j7 > 0; j7 = j7 - 1) {
				fVec80[j7] = fVec80[j7 - 1];
			}
			fRec73[1] = fRec73[0];
		}
	}

};

#endif
