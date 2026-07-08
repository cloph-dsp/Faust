// vst3_probe.cpp -- loads a VST3 module, lists classes, and tries to
// instantiate the first IComponent + IAudioProcessor.  Wraps every
// interesting call in __try/__except so a crash prints a label
// instead of nuking the host.
//
// Build (from a VS x64 dev prompt):
//   cl /nologo /EHsc /std:c++17 vst3_probe.cpp /link /LIBPATH:...\x64
//      kernel32.lib ole32.lib
//   /I D:\Github\VST3 Developments\iPlug2\Dependencies\IPlug\VST3_SDK
//   /I D:\Github\VST3 Developments\iPlug2\Dependencies\IPlug\VST3_SDK\public.sdk\source\vst

#include <windows.h>
#include <cstdio>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

typedef void* (*GetPluginFactory_t)();

static const char* SEHStr(unsigned code) {
  switch (code) {
    case EXCEPTION_ACCESS_VIOLATION: return "ACCESS_VIOLATION";
    case EXCEPTION_ILLEGAL_INSTRUCTION: return "ILLEGAL_INSTRUCTION";
    case EXCEPTION_STACK_OVERFLOW: return "STACK_OVERFLOW";
    case EXCEPTION_INT_DIVIDE_BY_ZERO: return "INT_DIVIDE_BY_ZERO";
    default: return "OTHER";
  }
}

#define TRY_STEP(label, expr) \
  do { \
    __try { \
      printf("[try] %s ... ", label); fflush(stdout); \
      expr; \
      printf("OK\n"); fflush(stdout); \
    } __except(EXCEPTION_EXECUTE_HANDLER) { \
      printf("CRASH (%s, code 0x%08lx)\n", SEHStr(GetExceptionCode()), GetExceptionCode()); \
      fflush(stdout); \
      return 1; \
    } \
  } while (0)

int wmain(int argc, wchar_t** argv) {
  if (argc < 2) { printf("Usage: vst3_probe <path-to-vst3>\n"); return 1; }
  printf("Loading: %S\n\n", argv[1]);

  HMODULE mod = nullptr;
  TRY_STEP("LoadLibraryW", mod = LoadLibraryW(argv[1]));

  if (!mod) {
    printf("LoadLibraryW failed: 0x%08lx\n", GetLastError());
    return 2;
  }

  GetPluginFactory_t getFactory = nullptr;
  TRY_STEP("GetProcAddress(GetPluginFactory)",
           getFactory = (GetPluginFactory_t)GetProcAddress(mod, "GetPluginFactory"));
  if (!getFactory) {
    printf("GetProcAddress failed\n"); return 3;
  }

  void* factoryObj = nullptr;
  TRY_STEP("factory()", factoryObj = getFactory());
  if (!factoryObj) { printf("factory() returned NULL\n"); return 4; }

  IPluginFactory* factory = (IPluginFactory*)factoryObj;
  int32 numClasses = 0;
  TRY_STEP("factory->getClassInfo(-1) count",
           { PFactoryInfo info; if (factory->getFactoryInfo(&info) == kResultOk) numClasses = info.classCount; });

  TRY_STEP("factory->getClassInfo(0)",
           {
             PClassInfo ci;
             if (factory->getClassInfo(0, &ci) == kResultOk) {
               printf("\n  Class[0]: name='%s' category='%s' cid=%08x\n",
                      ci.name, ci.category, ci.cid);
             }
           });

  // Try to create IComponent
  IComponent* comp = nullptr;
  TRY_STEP("factory->createInstance(IComponent)",
           factory->createInstance(PClassInfo::kCID_Invalid, IComponent::iid, (void**)&comp));
  // Actually use the class id we just got -- but we need to refetch.
  PClassInfo ci;
  if (factory->getClassInfo(0, &ci) == kResultOk) {
    TRY_STEP("factory->createInstance(cid, IComponent)",
             factory->createInstance(ci.cid, IComponent::iid, (void**)&comp));
  }

  if (comp) {
    printf("\n  IComponent at %p\n", comp);
    TRY_STEP("comp->initialize(nullptr)",
             comp->initialize(nullptr));
    TRY_STEP("comp->terminate()", comp->terminate());
    comp->release();
  }

  printf("\nALL STEPS PASSED\n");
  return 0;
}