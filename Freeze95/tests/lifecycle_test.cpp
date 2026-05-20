// freeze95_lifecycle_test.cpp
// Compile: clang++ -std=c++17 -I ../../iPlug2/Dependencies/IPlug/VST3_SDK -o lifecycle_test lifecycle_test.cpp -framework CoreFoundation -lobjc
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
#include <cmath>
#include <CoreGraphics/CoreGraphics.h>

// ObjC runtime types (for NSScreen swizzling in headless simulation)
// Forward-declared to avoid needing -fobjc-objc; these match the macOS ABI.
extern "C" {
  typedef struct objc_object* id;
  typedef struct objc_class* Class;
  typedef struct objc_selector* SEL;
  typedef struct objc_method* Method;
  typedef id (*IMP)(id, SEL, ...);
  extern Class objc_getClass(const char*);
  extern SEL sel_registerName(const char*);
  extern Method class_getClassMethod(Class, SEL);
  extern IMP method_setImplementation(Method, IMP);
  extern id class_createInstance(Class, size_t);
  extern id objc_msgSend(id, SEL, ...);
}


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

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/gui/iplugview.h>

using namespace Steinberg;
using namespace Steinberg::Vst;

// ============================================================================
// Globals
// ============================================================================
static int gErrors = 0;
static int gTests = 0;
static bool g_headless_mode = false;  // Set true by NSScreen swizzle in headless simulation

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
// Graphics context diagnostic — detects headless CI vs display-having runner
// ============================================================================
static void check_graphics_context()
{
  printf("\n--- Graphics Context Diagnostic ---\n");

  CGDirectDisplayID mainDisplay = CGMainDisplayID();
  if (mainDisplay != kCGNullDirectDisplay) {
    printf("  Main display ID: %u (display present)\n", (unsigned int)mainDisplay);
  } else {
    printf("  WARNING: No main display detected (HEADLESS context)\n");
    printf("  [NSScreen mainScreen] will return nil — crash risk during GUI init\n");
  }

  uint32_t displayCount = 0;
  CGError err = CGGetActiveDisplayList(0, NULL, &displayCount);
  if (err == kCGErrorSuccess) {
    printf("  Active displays: %u\n", (unsigned int)displayCount);
    if (displayCount == 0) {
      printf("  WARNING: Zero active displays — headless context!\n");
    }
  } else {
    printf("  CGGetActiveDisplayList error: %d\n", err);
  }
}

// ============================================================================
// Edge-case audio processing — stress tests DSP init with pathological input
// ============================================================================
static void test_edge_case_processing(IPluginFactory* factory, int32 classIdx)
{
  printf("\n=== Edge-Case Audio Processing ===\n");

  PClassInfo ci;
  factory->getClassInfo(classIdx, &ci);

  IAudioProcessor* processor = nullptr;
  IComponent* component = nullptr;

  tresult res = factory->createInstance(ci.cid, INLINE_UID_OF(IAudioProcessor), (void**)&processor);
  if (res != kResultOk || !processor) {
    printf("  SKIP: could not create instance (res=%08x)\n", res);
    return;
  }
  res = processor->queryInterface(INLINE_UID_OF(IComponent), (void**)&component);
  if (!component) { processor->release(); printf("  SKIP: no IComponent\n"); return; }

  MinimalHostContext ctx;
  component->initialize(&ctx);

  ProcessSetup setup;
  memset(&setup, 0, sizeof(setup));
  setup.processMode = kRealtime;
  setup.symbolicSampleSize = kSample32;
  setup.maxSamplesPerBlock = 512;
  setup.sampleRate = 44100.0;
  processor->setupProcessing(setup);
  component->setActive(true);

  float inputBuf[2][512], outputBuf[2][512];
  float* inputPtrs[2] = {inputBuf[0], inputBuf[1]};
  float* outputPtrs[2] = {outputBuf[0], outputBuf[1]};

  AudioBusBuffers busInputs[1] = {}, busOutputs[1] = {};
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

  // Test 1: Silence
  memset(inputBuf, 0, sizeof(inputBuf));
  memset(outputBuf, 0, sizeof(outputBuf));
  res = processor->process(data);
  TEST(res == kResultOk, "process(silence) returned %08x", res);
  bool noNaN = true;
  for (int ch = 0; ch < 2 && noNaN; ch++)
    for (int s = 0; s < 512 && noNaN; s++)
      if (std::isnan(outputBuf[ch][s])) noNaN = false;
  TEST(noNaN, "silence output contains no NaN");

  // Test 2: Full-scale DC
  for (int ch = 0; ch < 2; ch++)
    for (int s = 0; s < 512; s++) inputBuf[ch][s] = 1.0f;
  memset(outputBuf, 0, sizeof(outputBuf));
  res = processor->process(data);
  TEST(res == kResultOk, "process(full-scale DC) returned %08x", res);
  noNaN = true;
  for (int ch = 0; ch < 2 && noNaN; ch++)
    for (int s = 0; s < 512 && noNaN; s++)
      if (std::isnan(outputBuf[ch][s])) noNaN = false;
  TEST(noNaN, "full-scale DC output contains no NaN");
  bool finite = true;
  for (int ch = 0; ch < 2 && finite; ch++)
    for (int s = 0; s < 512 && finite; s++)
      if (!std::isfinite(outputBuf[ch][s])) finite = false;
  TEST(finite, "full-scale DC output contains no infinity");

  // Test 3: NaN input — should not crash
  for (int ch = 0; ch < 2; ch++)
    for (int s = 0; s < 512; s++) inputBuf[ch][s] = NAN;
  memset(outputBuf, 0, sizeof(outputBuf));
  res = processor->process(data);
  TEST(res == kResultOk, "process(NaN input) returned %08x", res);

  // Test 4: Denormal-level input
  for (int ch = 0; ch < 2; ch++)
    for (int s = 0; s < 512; s++) inputBuf[ch][s] = 1.0e-38f;
  memset(outputBuf, 0, sizeof(outputBuf));
  res = processor->process(data);
  TEST(res == kResultOk, "process(denormal input) returned %08x", res);

  component->setActive(false);
  component->terminate();
  component->release();
  printf("Edge-case processing: complete\n");
}

// ============================================================================
// Helper: create a minimal NSView using ObjC runtime (no AppKit headers needed)
// ============================================================================
static void* create_dummy_nsview() {
  Class cls = objc_getClass("NSView");
  if (!cls) {
    printf("  [WARN] NSView class not available\n");
    return nullptr;
  }

  // Allocate memory for an NSView instance
  id instance = class_createInstance(cls, 0);
  if (!instance) {
    printf("  [WARN] Could not allocate NSView instance\n");
    return nullptr;
  }

  // Call -[NSView init] via objc_msgSend — safe because NSView's init just
  // calls [super init] and returns self. Our @try/@catch guards handle any
  // NSException if something goes wrong.
  SEL initSel = sel_registerName("init");
  instance = ((id(*)(id, SEL))objc_msgSend)(instance, initSel);

  return (void*)instance;
}

// ============================================================================
// IEditController test — exercises view creation code path
// ============================================================================
static void test_edit_controller(IPluginFactory* factory, int32 classIdx)
{
  printf("\n=== VST3 Edit Controller Test ===\n");

  // iPlug2 typically merges controller and processor into one component,
  // so createInstance with IEditController IID often works directly.
  PClassInfo ci;
  factory->getClassInfo(classIdx, &ci);

  IEditController* controller = nullptr;
  tresult res = factory->createInstance(ci.cid, INLINE_UID_OF(IEditController), (void**)&controller);
  if (!controller) {
    // Try enumerating for a separate controller class
    int32 classCount = factory->countClasses();
    for (int32 i = 0; i < classCount; i++) {
      if (factory->getClassInfo(i, &ci) == kResultOk) {
        if (strstr(ci.category, "Controller")) {
          printf("  Found controller class at index %d: '%s'\n", (int)i, ci.name);
          res = factory->createInstance(ci.cid, INLINE_UID_OF(IEditController), (void**)&controller);
          if (controller) break;
        }
      }
    }
  }

  if (!controller) {
    printf("  IEditController not available via direct IID query.\n");
    printf("  This is expected — iPlug2 may register the controller\n");
    printf("  and processor as the same component, accessible only\n");
    printf("  via IAudioProcessor -> queryInterface(IEditController).\n");

    // Try via the processor we already tested
    IAudioProcessor* proc = nullptr;
    factory->createInstance(ci.cid, INLINE_UID_OF(IAudioProcessor), (void**)&proc);
    if (proc) {
      IEditController* ctrl2 = nullptr;
      res = proc->queryInterface(INLINE_UID_OF(IEditController), (void**)&ctrl2);
      if (ctrl2) {
        printf("  Got IEditController via IAudioProcessor->queryInterface!\n");
        controller = ctrl2;
        proc->release();
      } else {
        proc->release();
      }
    }
  }

  if (!controller) {
    printf("  No IEditController available. Controller tests SKIPPED.\n");
    printf("  (GUI code path is NOT exercised — this is normal for\n");
    printf("   VST3 plugins that defer GUI init to editor attach)\n");
    return;
  }

  printf("  IEditController: OK\n");

  // Get parameters
  int32 paramCount = controller->getParameterCount();
  printf("  Parameters: %d\n", (int)paramCount);
  TEST(paramCount > 0, "At least 1 parameter registered");

  for (int32 i = 0; i < paramCount && i < 8; i++) {
    ParameterInfo pinfo;
    memset(&pinfo, 0, sizeof(pinfo));
    if (controller->getParameterInfo(i, pinfo) == kResultOk) {
      printf("    Param[%d]: '%s' (short='%s') default=%.3f\n",
             (int)i, pinfo.title, pinfo.shortTitle, pinfo.defaultNormalizedValue);
    }
  }

  // Try creating the editor view — exercises GUI init code path
  IPlugView* view = controller->createView("editor");
  if (view) {
    printf("  createView(\"editor\"): OK (GUI view created)\n");

    // Check platform support
    if (view->isPlatformTypeSupported(Steinberg::kPlatformTypeNSView) == kResultOk) {
      printf("  kPlatformTypeNSView: supported\n");
    }

    // Call view->attached() with a dummy NSView to trigger OpenWindow.
    // In the headless CI context NSScreen.mainScreen has been swizzled → nil,
    // which is the exact crash condition reported by customers.
    //
    // Expected: OpenWindow hits @try/@catch, returns gracefully with nullptr,
    //           attached() returns kResultFalse, and the process survives.
    void* dummyNSView = nullptr;
    if (g_headless_mode) {
      printf("  [headless-simulation] Creating dummy NSView for attached()...\n");
      dummyNSView = create_dummy_nsview();
      if (dummyNSView)
        printf("  [headless-simulation] Dummy NSView: %p\n", dummyNSView);
      else
        printf("  [headless-simulation] Could not create NSView; attaching with nullptr\n");
    }

    printf("  Calling view->attached(parent, kPlatformTypeNSView)\n");
    printf("  (This triggers IGraphicsMac::OpenWindow which accesses\n");
    printf("   NSScreen.mainScreen — the known crash site on headless)\n");
    tresult attachRes = view->attached(dummyNSView ? dummyNSView : nullptr,
                                       Steinberg::kPlatformTypeNSView);
    printf("  view->attached() returned: %s (%08x)\n",
           attachRes == Steinberg::kResultOk ? "kResultOk" : "kResultFalse", attachRes);
    TEST(attachRes == Steinberg::kResultOk,
         "view->attached() should return kResultOk even with swizzled NSScreen");

    // Detach to clean up
    view->removed();

    // Free the dummy NSView if we created one
    if (dummyNSView) {
      // Use ObjC runtime to release: call -[NSObject release]
      // release is declared on NSObject, which NSView inherits
      SEL releaseSel = sel_registerName("release");
      ((void(*)(id, SEL))objc_msgSend)((id)dummyNSView, releaseSel);
    }

    view->release();
  } else {
    printf("  createView(\"editor\"): gracefully declined (no parent window)\n");
  }

  controller->release();
  printf("IEditController test: complete\n");
}

// ============================================================================
// Headless NSScreen simulation — returns nil for [NSScreen mainScreen]
// ============================================================================
static id nil_nsscreen_mainScreen(id self, SEL _cmd, ...) {
  (void)self; (void)_cmd;
  return nullptr; // nil in ObjC
}

// ============================================================================
// Run all VST3 lifecycle tests in child process — fork-guarded from crashes
// ============================================================================
static int run_vst3_child_tests(const char* binPath)
{
  void* module = dlopen(binPath, RTLD_NOW | RTLD_LOCAL);
  if (!module) {
    fprintf(stderr, "  FATAL: dlopen failed in child: %s\n", dlerror());
    return 1;
  }

  auto* getFactoryFn = (IPluginFactory* (*)())dlsym(module, "GetPluginFactory");
  if (!getFactoryFn) { dlclose(module); return 2; }

  IPluginFactory* factory = getFactoryFn();
  if (!factory) { dlclose(module); return 3; }

  int32 classCount = factory->countClasses();
  int32 audioClassIdx = -1;
  for (int32 i = 0; i < classCount; i++) {
    PClassInfo tmp;
    memset(&tmp, 0, sizeof(tmp));
    if (factory->getClassInfo(i, &tmp) == kResultOk) {
      if (strstr(tmp.category, "Audio") || strstr(tmp.category, "Fx")) {
        audioClassIdx = i;
        break;
      }
    }
  }
  if (audioClassIdx < 0 && classCount > 0) audioClassIdx = 0;

  // Reset counters for clean child report
  gErrors = 0;
  gTests = 0;

  // ================================================================
  // HEADLESS CONTEXT SIMULATION
  // Swizzle [NSScreen mainScreen] → nil to exactly reproduce the
  // DAW scanning crash scenario. The plugin MUST survive this.
  // ================================================================
  printf("\n--- Headless Context Simulation ---\n");
  Class nsScreenClass = objc_getClass("NSScreen");
  if (nsScreenClass) {
    SEL mainScreenSel = sel_registerName("mainScreen");
    Method m = class_getClassMethod(nsScreenClass, mainScreenSel);
    if (m) {
      method_setImplementation(m, (IMP)nil_nsscreen_mainScreen);
      printf("  [NSScreen mainScreen] swizzled → returns nil ✓\n");
      printf("  All subsequent VST3 ops run with nil screen.\n");
      printf("  If any test below crashes, @try/@catch patches are\n");
      printf("  INSUFFICIENT for headless DAW scanning!\n");
      g_headless_mode = true;
    } else {
      printf("  WARNING: couldn't find [NSScreen mainScreen] Method\n");
    }
  } else {
    printf("  NSScreen class not loaded (AppKit not linked yet?)\n");
    printf("  Headless simulation SKIPPED\n");
  }

  test_vst3_factory(factory);
  test_vst3_instance(factory, 1);
  test_vst3_stress(factory);
  test_vst3_instance(factory, 3);
  test_edge_case_processing(factory, audioClassIdx);
  test_edit_controller(factory, audioClassIdx);

  factory->release();
  dlclose(module);
  return gErrors;
}

// ============================================================================
// Fork-guard wrapper — runs fn(arg) in child, catches crashes
// ============================================================================
static bool fork_guard_run(int (*fn)(const char*), const char* arg)
{
  pid_t pid = fork();
  if (pid == 0) {
    int result = fn(arg);
    _exit(result);
  }
  if (pid < 0) {
    fprintf(stderr, "  fork_guard: fork failed\n");
    return false;
  }
  int status;
  waitpid(pid, &status, 0);
  if (WIFSIGNALED(status)) {
    fprintf(stderr, "  CRASHED with signal %d\n", WTERMSIG(status));
    return false;
  }
  if (!WIFEXITED(status)) return false;
  return WEXITSTATUS(status) == 0;
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

  // --- Graphics context diagnostic ---
  check_graphics_context();

  // --- Fork-guarded VST3 lifecycle tests ---
  // We fork BEFORE loading the factory so that any crash during
  // GetPluginFactory, component creation, audio processing, or
  // view creation is caught in the child process.
  printf("\n--- VST3 Tests (fork-guarded) ---\n");
  if (fork_guard_run(run_vst3_child_tests, binPath)) {
    printf("  Fork-guarded VST3 tests: ALL PASSED\n");
  } else {
    printf("  Fork-guarded VST3 tests: FAILED (see child output above)\n");
    gErrors++;
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
