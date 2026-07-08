// vst3_test.cpp - minimal VST3 plugin loader to identify crash location.
// Tries: LoadLibrary -> GetProcAddress("GetPluginFactory") -> factory().
// Wraps each step in __try/__except so any SEH crash surfaces as a message
// instead of taking down the test process.

#include <windows.h>
#include <cstdio>

typedef void* (*GetPluginFactory_t)();

int wmain(int argc, wchar_t** argv) {
  if (argc < 2) {
    printf("Usage: vst3_test.exe <path-to-vst3>\n");
    return 1;
  }

  printf("Loading: %S\n", argv[1]);

  HMODULE mod = nullptr;
  __try {
    mod = LoadLibraryW(argv[1]);
  } __except(EXCEPTION_EXECUTE_HANDLER) {
    printf("CRASH in LoadLibraryW (SEH 0x%08lx)\n", GetExceptionCode());
    return 2;
  }

  if (!mod) {
    printf("LoadLibraryW failed: 0x%08lx\n", GetLastError());
    return 3;
  }
  printf("Loaded OK at %p\n", mod);

  GetPluginFactory_t factory = nullptr;
  __try {
    factory = reinterpret_cast<GetPluginFactory_t>(
        GetProcAddress(mod, "GetPluginFactory"));
  } __except(EXCEPTION_EXECUTE_HANDLER) {
    printf("CRASH in GetProcAddress (SEH 0x%08lx)\n", GetExceptionCode());
    return 4;
  }

  if (!factory) {
    printf("GetProcAddress failed: %d\n", GetLastError());
    return 5;
  }
  printf("GetPluginFactory at %p\n", factory);

  void* pluginFactory = nullptr;
  __try {
    pluginFactory = factory();
  } __except(EXCEPTION_EXECUTE_HANDLER) {
    printf("CRASH calling GetPluginFactory() (SEH 0x%08lx)\n",
           GetExceptionCode());
    return 6;
  }

  if (!pluginFactory) {
    printf("GetPluginFactory returned NULL\n");
    return 7;
  }
  printf("PluginFactory object at %p\n", pluginFactory);
  printf("All steps succeeded -- plugin loads cleanly.\n");
  return 0;
}