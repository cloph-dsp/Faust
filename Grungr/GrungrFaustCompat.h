// GrungrFaustCompat.h — minimal shim providing the Faust C++ runtime API surface
// used by Grungr.h. Replaces `faust/dsp/dsp.h`, `faust/gui/MapUI.h`, and
// `faust/gui/meta.h` so we can build the IPlugEffect-staged Grungr sources
// without depending on the full Faust runtime headers (which would otherwise
// need a separate download + include-path step).
//
// The shim mirrors the real Faust API surface for the symbols Grungr actually
// uses:
//   - FAUSTFLOAT       typedef
//   - Meta             base class for metadata callbacks
//   - UI               base class for parameter UI callbacks
//   - dsp              abstract base for the Faust-generated DSP class
//   - MapUI            concrete UI that maps path strings to FAUSTFLOAT zones,
//                      used by `mFaustUI.setParamValue("Grunge", val)` etc.
//
// Pattern proven by Freeze95/FaustCompat.h. Extended here with MapUI because
// Grungr.h instantiates a real `MapUI` and calls its setParamValue method.

#pragma once

#ifndef FAUSTFLOAT
#define FAUSTFLOAT double
#endif

#include <map>
#include <string>

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

// Concrete MapUI — the only UI implementation Grungr uses. Stores FAUSTFLOAT
// zone pointers keyed by parameter path, and exposes setParamValue /
// getParamValue as the runtime entry point used by SyncParamToFaust().
class MapUI : public UI {
public:
  std::map<std::string, FAUSTFLOAT*> fZoneMap;

  ~MapUI() override {}

  void addButton(const char* label, FAUSTFLOAT* zone) override { fZoneMap[label] = zone; }
  void addCheckButton(const char* label, FAUSTFLOAT* zone) override { fZoneMap[label] = zone; }
  void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override { fZoneMap[label] = zone; }
  void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override { fZoneMap[label] = zone; }
  void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override { fZoneMap[label] = zone; }
  void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override { fZoneMap[label] = zone; }
  void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override { fZoneMap[label] = zone; }

  void setParamValue(const char* path, FAUSTFLOAT v) {
    auto it = fZoneMap.find(path);
    if (it != fZoneMap.end()) {
      *it->second = v;
    }
  }

  FAUSTFLOAT getParamValue(const char* path) {
    auto it = fZoneMap.find(path);
    return (it != fZoneMap.end()) ? *it->second : FAUSTFLOAT(0);
  }
};
