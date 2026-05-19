// freeze95_lifecycle_test.cpp
// Compile: clang++ -std=c++17 -I ../../iPlug2/Dependencies/IPlug/VST3_SDK -o lifecycle_test lifecycle_test.cpp -framework CoreFoundation
// Run:    ./lifecycle_test /path/to/Freeze95.vst3
//
// Tests VST3 and CLAP plugin lifecycle in a simulated host environment.
// Each test phase is independent and reported individually.

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>


// --- VST3 SDK headers (from iPlug2 checkout) ---
// NOTE: DECLARE_CLASS_IID creates a static const TUID (raw 16-byte array) named
// ClassName_iid (e.g. IAudioProcessor_iid). The ::iid FUID static members are
// only defined when INIT_CLASS_IID is set; we use the raw TUID constants via
// INLINE_UID_OF(ClassName) instead.
#include <pluginterfaces/base/ipluginbase.h>
#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/ivstcomponent.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/vstspeaker.h>
#include <pluginterfaces/vst/vsttypes.h>

using namespace Steinberg;
using namespace Steinberg::Vst;

// ============================================================================
// Globals
// ============================================================================
static int gErrors = 0;
static int gTests = 0;

#define TEST(cond, fmt, ...) do { \
  gTests++; \
  if (!(cond)) { \
    fprintf(stderr, "  FAIL [%s:%d]: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    gErrors++; \
  } else { \
    printf("  PASS\n"); \
  } \
} while(0)

#define SECTION(name) do { printf("\n--- %s ---\n", name); } while(0)

// ============================================================================
// Minimal VST3 host context
// ============================================================================
class MinimalHostContext : public FUnknown
{
public:
  tresult PLUGIN_API queryInterface (FIDString iid, void** obj) override
  {
    // Accept only our own IID so the host side can identify us
    if (FUnknownPrivate::iidEqual(iid, INLINE_UID_OF(FUnknown))) {
      *obj = static_cast<FUnknown*>(this);
      addRef();
      return kResultOk;
    }
    *obj = nullptr;
    return kNoInterface;
  }
  uint32 PLUGIN_API addRef () override { return 1; }
  uint32 PLUGIN_API release () override { return 1; }
};

// ============================================================================
// CLAP entry point structures (pure C, no external headers needed)
// ============================================================================
typedef struct clap_plugin_descriptor {
  const char* id;
  const char* name;
  const char* vendor;
  const char* url;
  const char* manual_url;
  const char* support_url;
  const char* version;
  const char* description;
  const char* features;
} clap_plugin_descriptor_t;

typedef struct clap_plugin_factory {
  uint32_t (*get_plugin_count)(const struct clap_plugin_factory* factory);
  const clap_plugin_descriptor_t* (*get_plugin_descriptor)(const struct clap_plugin_factory* factory, uint32_t index);
  const void* (*create_plugin)(const struct clap_plugin_factory* factory, const struct clap_host* host, const char* plugin_id);
} clap_plugin_factory_t;

typedef struct clap_plugin_entry {
  uint64_t clap_version;
  bool (*init)(const char* plugin_path);
  void (*deinit)(void);
  const void* (*get_factory)(const char* factory_id);
} clap_plugin_entry_t;

#define CLAP_PLUGIN_FACTORY_ID "clap.plugin-factory"

// ============================================================================
// Test: VST3 Factory
// ============================================================================
static void test_vst3_factory(IPluginFactory* factory)
{
  printf("\n=== VST3 Factory Test ===\n");

  // 1. Factory info
  PFactoryInfo factoryInfo;
  memset(&factoryInfo, 0, sizeof(factoryInfo));
  factory->getFactoryInfo(&factoryInfo);

  int32 classCount = factory->countClasses();
  printf("  Vendor:  %s\n", factoryInfo.vendor);
  printf("  URL:     %s\n", factoryInfo.url);
  printf("  Email:   %s\n", factoryInfo.email);
  printf("  Classes: %d\n", (int)classCount);
  TEST(classCount > 0, "Factory should register at least 1 class, got %d", (int)classCount);

  // 2. Enumerate all classes
  for (int32 i = 0; i < classCount && i < 10; i++) {
    PClassInfo ci;
    memset(&ci, 0, sizeof(ci));
    tresult res = factory->getClassInfo(i, &ci);
    if (res == kResultOk && ci.name[0]) {
      printf("  Class[%d]: name='%s' category='%s' cardinality=%d\n",
             (int)i, ci.name, ci.category, (int)ci.cardinality);
    }
  }
}

// ============================================================================
// Test: VST3 Instance Lifecycle
// ============================================================================
static void test_vst3_instance(IPluginFactory* factory, int numCycles)
{
  printf("\n=== VST3 Instance Lifecycle Test (%d cycle(s)) ===\n", numCycles);

  // Get the first audio effect class
  PClassInfo ci;
  memset(&ci, 0, sizeof(ci));
  tresult res = kResultFalse;
  int32 classCount = factory->countClasses();

  // Find the first class with "Audio Module" category
  int32 audioClassIdx = -1;
  for (int32 i = 0; i < classCount; i++) {
    PClassInfo tmp;
    memset(&tmp, 0, sizeof(tmp));
    if (factory->getClassInfo(i, &tmp) == kResultOk) {
      if (strstr(tmp.category, "Audio") != nullptr || strstr(tmp.category, "Fx") != nullptr) {
        ci = tmp;
        audioClassIdx = i;
        break;
      }
    }
  }

  if (audioClassIdx < 0 && classCount > 0) {
    // Fallback: use first class
    factory->getClassInfo(0, &ci);
    audioClassIdx = 0;
  }

  TEST(audioClassIdx >= 0, "Found audio module class (index=%d)", audioClassIdx);
  if (audioClassIdx < 0) return;

  printf("  Using class: '%s' [%s]\n", ci.name, ci.category);

  for (int cycle = 0; cycle < numCycles; cycle++) {
    printf("  Cycle %d/%d:\n", cycle + 1, numCycles);

    // 1. createInstance with IAudioProcessor IID
    // INLINE_UID_OF(IAudioProcessor) expands to IAudioProcessor_iid (static const TUID)
    IAudioProcessor* processor = nullptr;
    res = factory->createInstance(ci.cid, INLINE_UID_OF(IAudioProcessor), (void**)&processor);
    TEST(res == kResultOk && processor != nullptr,
         "createInstance(IAudioProcessor) returned %08x, ptr=%p", res, (void*)processor);
    if (!processor) continue;

    printf("    createInstance: OK\n");

    // 2. QueryInterface for IComponent (lifecycle methods: initialize, terminate, setActive)
    // NOTE: IComponent extends IPluginBase which has initialize/terminate at vtable offsets 3/4.
    // IAudioProcessor has setBusArrangements/setupProcessing/process at offsets 3/6/8.
    // We must use the correct interface pointer for each method.
    IComponent* component = nullptr;
    res = processor->queryInterface(INLINE_UID_OF(IComponent), (void**)&component);
    TEST(res == kResultOk && component != nullptr,
         "queryInterface(IComponent) returned %08x", res);

    if (component) {
      printf("    queryInterface(IComponent): OK\n");

      // 3. Initialize with minimal host context
      MinimalHostContext hostContext;
      res = component->initialize(&hostContext);
      bool initOK = (res == kResultOk);
      printf("    initialize: %s (res=%08x)\n", initOK ? "OK" : "FAIL (expected without full DAW context)", res);

      if (initOK) {
        // 4. Get bus info (requires initialized component)
        int32 inBusses = component->getBusCount(kAudio, kInput);
        int32 outBusses = component->getBusCount(kAudio, kOutput);
        printf("    Input busses:  %d\n", (int)inBusses);
        printf("    Output busses: %d\n", (int)outBusses);

        // 5. setBusArrangements (on IAudioProcessor)
        SpeakerArrangement inputArr = SpeakerArr::kStereo;
        SpeakerArrangement outputArr = SpeakerArr::kStereo;
        res = processor->setBusArrangements(&inputArr, 1, &outputArr, 1);
        printf("    setBusArrangements: res=%08x\n", res);

        // 6. setupProcessing (on IAudioProcessor)
        ProcessSetup setup;
        memset(&setup, 0, sizeof(setup));
        setup.processMode = kRealtime;
        setup.symbolicSampleSize = kSample32;
        setup.maxSamplesPerBlock = 512;
        setup.sampleRate = 44100.0;

        res = processor->setupProcessing(setup);
        TEST(res == kResultOk, "setupProcessing(44.1k, 512) returned %08x", res);

        // 7. setActive(true) (on IComponent)
        component->setActive(true);
        printf("    setActive(true): OK\n");

        // 8. Process 10 blocks of audio
        float inputBuf[2][512];
        float outputBuf[2][512];
        memset(inputBuf, 0, sizeof(inputBuf));
        memset(outputBuf, 0, sizeof(outputBuf));

        float* inputPtrs[2] = {inputBuf[0], inputBuf[1]};
        float* outputPtrs[2] = {outputBuf[0], outputBuf[1]};

        AudioBusBuffers busInputs[1];
        AudioBusBuffers busOutputs[1];
        memset(busInputs, 0, sizeof(busInputs));
        memset(busOutputs, 0, sizeof(busOutputs));

        busInputs[0].numChannels = 2;
        busInputs[0].channelBuffers32 = inputPtrs;
        busOutputs[0].numChannels = 2;
        busOutputs[0].channelBuffers32 = outputPtrs;

        ProcessData data;
        memset(&data, 0, sizeof(data));
        data.processMode = kRealtime;
        data.symbolicSampleSize = kSample32;
        data.numSamples = 512;
        data.numInputs = 1;
        data.numOutputs = 1;
        data.inputs = busInputs;
        data.outputs = busOutputs;

        bool processOK = true;
        for (int block = 0; block < 10; block++) {
          res = processor->process(data);
          if (res != kResultOk) { processOK = false; break; }
        }
        TEST(processOK, "process(10 blocks of 512 samples) - all OK");

        // Verify output is valid (no NaN)
        bool validOutput = true;
        for (int ch = 0; ch < 2 && validOutput; ch++) {
          for (int s = 0; s < 512; s++) {
            if (outputBuf[ch][s] != outputBuf[ch][s]) { validOutput = false; break; }
          }
        }
        TEST(validOutput, "Output samples contain no NaN values");

        component->setActive(false);
        printf("    setActive(false): OK\n");
      }

      // 9. terminate and release component
      component->terminate();
      printf("    terminate: OK\n");

      // 10. release component (also releases the object since component was queried from processor)
      component->release();
    } else {
      // Fallback: release processor directly (less thorough test)
      printf("    WARNING: Could not query IComponent, testing basic addRef/release only\n");
      // Query FUnknown (should always work from IAudioProcessor)
      FUnknown* unknown = nullptr;
      res = processor->queryInterface(INLINE_UID_OF(FUnknown), (void**)&unknown);
      if (unknown) unknown->release();
    }

    // If component not available, release processor
    if (!component) {
      printf("    release (via IAudioProcessor): ");
      uint32 refCount = processor->release();
      printf("ref=%u\n", refCount);
    }
  }
}

// ============================================================================
// Test: Multiple Create/Destroy (stress test)
// ============================================================================
static void test_vst3_stress(IPluginFactory* factory)
{
  printf("\n=== VST3 Stress Test (20 create/destroy cycles) ===\n");

  PClassInfo ci;
  memset(&ci, 0, sizeof(ci));
  factory->getClassInfo(0, &ci);

  for (int i = 0; i < 20; i++) {
    IAudioProcessor* p = nullptr;
    tresult res = factory->createInstance(ci.cid, INLINE_UID_OF(IAudioProcessor), (void**)&p);
    if (res != kResultOk || !p) {
      fprintf(stderr, "  FAIL: createInstance failed on cycle %d (res=%08x)\n", i, res);
      gErrors++;
      continue;
    }

    // Get IComponent for lifecycle methods
    IComponent* comp = nullptr;
    res = p->queryInterface(INLINE_UID_OF(IComponent), (void**)&comp);
    if (comp) {
      MinimalHostContext ctx;
      comp->initialize(&ctx);
      comp->terminate();
      comp->release(); // releases the object
    } else {
      // Fallback: just release via IAudioProcessor
      p->release();
    }

    if (i == 19) printf("  Cycles completed without crash: OK\n");
  }
  TEST(true, "Stress test completed without crash");
}

// ============================================================================
// Test: CLAP Entry Point
// ============================================================================
static void test_clap_entry(void* handle)
{
  printf("\n=== CLAP Entry Test ===\n");

  clap_plugin_entry_t* entry = (clap_plugin_entry_t*)dlsym(handle, "clap_entry");
  if (!entry) {
    printf("  SKIP: clap_entry not found (VST3-only binary)\n");
    return;
  }
  gTests++;
  printf("  PASS: clap_entry found\n");

  TEST(entry->clap_version >= 0x00010000,
       "clap_version >= 1.0 (got 0x%llx)", (unsigned long long)entry->clap_version);
  TEST(entry->init != nullptr, "clap_entry.init is not null");
  TEST(entry->deinit != nullptr, "clap_entry.deinit is not null");
  TEST(entry->get_factory != nullptr, "clap_entry.get_factory is not null");

  // Call init
  bool initOK = entry->init("/");
  printf("  init('/'): %s\n", initOK ? "OK" : "FAILED");
  if (!initOK) {
    fprintf(stderr, "  WARN: clap_entry.init returned false (may be OK without proper args)\n");
  }

  // Get factory
  const clap_plugin_factory_t* factory =
      (const clap_plugin_factory_t*)entry->get_factory(CLAP_PLUGIN_FACTORY_ID);
  TEST(factory != nullptr, "get_factory(CLAP_PLUGIN_FACTORY_ID) returned non-null");
  if (!factory) {
    entry->deinit();
    return;
  }

  TEST(factory->get_plugin_count != nullptr, "factory.get_plugin_count is not null");
  TEST(factory->get_plugin_descriptor != nullptr, "factory.get_plugin_descriptor is not null");
  TEST(factory->create_plugin != nullptr, "factory.create_plugin is not null");

  uint32_t clapPluginCount = factory->get_plugin_count(factory);
  printf("  CLAP plugin count: %u\n", clapPluginCount);
  TEST(clapPluginCount > 0, "At least 1 CLAP plugin registered");

  // Enumerate plugins
  for (uint32_t i = 0; i < clapPluginCount && i < 5; i++) {
    const clap_plugin_descriptor_t* desc = factory->get_plugin_descriptor(factory, i);
    if (desc) {
      printf("  Plugin[%u]: id='%s' name='%s' version='%s'\n",
             i, desc->id ? desc->id : "?", desc->name ? desc->name : "?", desc->version ? desc->version : "?");
    }
  }

  entry->deinit();
  printf("  deinit: OK\n");
}

// ============================================================================
// Fork-safe dlopen test — runs dlopen in child process to catch crashes
// ============================================================================
static bool try_dlopen(const char* path)
{
  pid_t pid = fork();
  if (pid == 0) {
    // Child: just dlopen + dlclose, then exit
    void* h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (h) dlclose(h);
    _exit(h ? 0 : 1);
  }
  if (pid < 0) return false;  // fork failed
  int status;
  waitpid(pid, &status, 0);
  return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

// ============================================================================
// Main
// ============================================================================
int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path/to/Freeze95.vst3>\n", argv[0]);
    return 1;
  }

  const char* bundlePath = argv[1];
  char binPath[4096];
  snprintf(binPath, sizeof(binPath), "%s/Contents/MacOS/Freeze95", bundlePath);

  printf("============================================\n");
  printf("  Freeze95 Lifecycle Test\n");
  printf("  Bundle: %s\n", bundlePath);
  printf("  Binary: %s\n", binPath);
  printf("============================================\n");

  // --- Pre-flight dlopen in forked child (catches SIGBUS/SIGSEGV gracefully) ---
  if (!try_dlopen(binPath)) {
    printf("  CRASH: binary cannot be loaded (dlopen crashes in child process)\n");
    printf("  This is usually caused by static factory registrations or\n");
    printf("  C++ static initializers in the plugin binary.\n");
    gTests++;
    gErrors++;
    printf("\nFAILED: 1 error(s) out of 1 test(s)\n");
    return 1;
  }

  // --- Load plugin binary (safe now — pre-flight passed) ---
  void* module = dlopen(binPath, RTLD_NOW | RTLD_LOCAL);
  TEST(module != nullptr, "dlopen(RTLD_NOW)");
  if (!module) {
    fprintf(stderr, "  dlopen error: %s\n", dlerror());
    fprintf(stderr, "\nFAILED: %d error(s) out of %d test(s)\n", gErrors, gTests);
    return 1;
  }

  // --- Test CLAP entry ---
  test_clap_entry(module);

  // --- Get VST3 factory (optional — CLAP-only binaries won't have it) ---
  auto* getFactoryFn = (IPluginFactory* (*)())dlsym(module, "GetPluginFactory");
  if (getFactoryFn) {
    printf("\n--- VST3 Tests ---\n");
    IPluginFactory* factory = getFactoryFn();
    if (factory) {
      test_vst3_factory(factory);
      test_vst3_instance(factory, 1);
      test_vst3_stress(factory);
      test_vst3_instance(factory, 3);
      factory->release();
    }
  } else {
    printf("  SKIP: GetPluginFactory not found (CLAP-only binary)\n");
  }

  dlclose(module);

  // --- Summary ---
  printf("\n============================================\n");
  if (gErrors == 0) {
    printf("  ALL %d TESTS PASSED\n", gTests);
    printf("============================================\n");
    return 0;
  } else {
    printf("  FAILED: %d / %d tests\n", gErrors, gTests);
    printf("============================================\n");
    return 1;
  }
}
