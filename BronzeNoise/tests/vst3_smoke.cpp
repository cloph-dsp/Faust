#define NOMINMAX
#include <windows.h>

#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/vstspeaker.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace {

using InitDllProc = bool (*)();
using ExitDllProc = bool (*)();
using GetFactoryProc = IPluginFactory* (*)();

struct PluginInstance {
  IComponent* component = nullptr;
  IAudioProcessor* processor = nullptr;
  IEditController* controller = nullptr;

  ~PluginInstance() {
    if (processor) processor->setProcessing(false);
    if (component) component->setActive(false);
    if (controller) controller->release();
    if (processor) processor->release();
    if (component) { component->terminate(); component->release(); }
  }
};

bool CreateInstance(IPluginFactory* factory, PluginInstance& instance, double sampleRate, int blockSize) {
  PClassInfo info {};
  bool found = false;
  for (int32 i = 0; i < factory->countClasses(); ++i) {
    if (factory->getClassInfo(i, &info) == kResultOk &&
        std::strcmp(info.category, kVstAudioEffectClass) == 0) {
      found = true;
      break;
    }
  }
  if (!found) return false;

  if (factory->createInstance(info.cid, IComponent::iid,
                              reinterpret_cast<void**>(&instance.component)) != kResultOk)
    return false;
  if (instance.component->initialize(nullptr) != kResultOk) return false;
  if (instance.component->queryInterface(IAudioProcessor::iid,
      reinterpret_cast<void**>(&instance.processor)) != kResultOk) return false;
  instance.component->queryInterface(IEditController::iid,
      reinterpret_cast<void**>(&instance.controller));

  SpeakerArrangement inputArrangement = SpeakerArr::kStereo;
  SpeakerArrangement outputArrangement = SpeakerArr::kStereo;
  if (instance.processor->setBusArrangements(&inputArrangement, 1, &outputArrangement, 1) != kResultOk)
    return false;

  ProcessSetup setup {};
  setup.processMode = kRealtime;
  setup.symbolicSampleSize = kSample64;
  setup.maxSamplesPerBlock = blockSize;
  setup.sampleRate = sampleRate;
  if (instance.processor->canProcessSampleSize(kSample64) != kResultTrue) return false;
  if (instance.processor->setupProcessing(setup) != kResultOk) return false;
  if (instance.component->activateBus(kAudio, kInput, 0, true) != kResultOk) return false;
  if (instance.component->activateBus(kAudio, kOutput, 0, true) != kResultOk) return false;
  if (instance.component->setActive(true) != kResultOk) return false;
  if (instance.processor->setProcessing(true) != kResultOk) return false;
  return true;
}

bool SetNormalized(PluginInstance& instance, ParamID id, ParamValue value) {
  return instance.controller && instance.controller->setParamNormalized(id, value) == kResultOk;
}

bool RestartProcessing(PluginInstance& instance, double sampleRate, int blockSize) {
  instance.processor->setProcessing(false);
  instance.component->setActive(false);
  ProcessSetup setup {};
  setup.processMode = kRealtime;
  setup.symbolicSampleSize = kSample64;
  setup.maxSamplesPerBlock = blockSize;
  setup.sampleRate = sampleRate;
  if (instance.processor->setupProcessing(setup) != kResultOk) return false;
  if (instance.component->setActive(true) != kResultOk) return false;
  return instance.processor->setProcessing(true) == kResultOk;
}

bool ProcessSignal(PluginInstance& instance,
                   const std::vector<double>& inputL,
                   const std::vector<double>& inputR,
                   std::vector<double>& outputL,
                   std::vector<double>& outputR,
                   int blockSize) {
  const int total = static_cast<int>(inputL.size());
  outputL.assign(total, 0.0);
  outputR.assign(total, 0.0);
  std::vector<double> blockInL(blockSize), blockInR(blockSize);
  std::vector<double> blockOutL(blockSize), blockOutR(blockSize);

  for (int offset = 0; offset < total; offset += blockSize) {
    const int frames = std::min(blockSize, total - offset);
    std::copy_n(inputL.data() + offset, frames, blockInL.data());
    std::copy_n(inputR.data() + offset, frames, blockInR.data());
    std::fill_n(blockOutL.data(), frames, 0.0);
    std::fill_n(blockOutR.data(), frames, 0.0);

    double* inputChannels[2] = {blockInL.data(), blockInR.data()};
    double* outputChannels[2] = {blockOutL.data(), blockOutR.data()};
    AudioBusBuffers inputBus {};
    inputBus.numChannels = 2;
    inputBus.channelBuffers64 = inputChannels;
    AudioBusBuffers outputBus {};
    outputBus.numChannels = 2;
    outputBus.channelBuffers64 = outputChannels;
    ProcessData data {};
    data.processMode = kRealtime;
    data.symbolicSampleSize = kSample64;
    data.numSamples = frames;
    data.numInputs = 1;
    data.numOutputs = 1;
    data.inputs = &inputBus;
    data.outputs = &outputBus;
    if (instance.processor->process(data) != kResultOk) return false;

    std::copy_n(blockOutL.data(), frames, outputL.data() + offset);
    std::copy_n(blockOutR.data(), frames, outputR.data() + offset);
  }
  return true;
}

} // namespace

int wmain(int argc, wchar_t** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::setvbuf(stderr, nullptr, _IONBF, 0);
  if (argc != 2) {
    std::fprintf(stderr, "usage: vst3_smoke.exe <BronzeNoise.vst3>\n");
    return 2;
  }

  HMODULE module = LoadLibraryW(argv[1]);
  if (!module) {
    std::fprintf(stderr, "FAIL: LoadLibraryW (%lu)\n", GetLastError());
    return 1;
  }
  auto initDll = reinterpret_cast<InitDllProc>(GetProcAddress(module, "InitDll"));
  auto exitDll = reinterpret_cast<ExitDllProc>(GetProcAddress(module, "ExitDll"));
  auto getFactory = reinterpret_cast<GetFactoryProc>(GetProcAddress(module, "GetPluginFactory"));
  if (!getFactory || (initDll && !initDll())) {
    std::fprintf(stderr, "FAIL: VST3 entry points\n");
    FreeLibrary(module);
    return 1;
  }
  std::printf("module initialized\n");

  IPluginFactory* factory = getFactory();
  std::printf("factory=%p classes=%d\n", static_cast<void*>(factory), factory ? factory->countClasses() : -1);
  constexpr int blockSize = 128;
  constexpr int totalSamples = 30000;
  constexpr int expectedLatency = 4095;
  constexpr int signalStart = 10000;
  bool ok = factory != nullptr;

  {
    PluginInstance instance;
    std::printf("creating impulse instance\n");
    const bool instanceReady = CreateInstance(factory, instance, 48000.0, blockSize);
    ok = ok && instanceReady;
    std::printf("impulse instance ready=%d\n", instanceReady ? 1 : 0);
    const uint32 reportedLatency = instanceReady ? instance.processor->getLatencySamples() : 0;
    std::vector<double> inputL(totalSamples, 0.0), inputR(totalSamples, 0.0);
    std::vector<double> outputL, outputR;
    inputL[0] = 0.25;
    const bool processed = instanceReady && ProcessSignal(instance, inputL, inputR, outputL, outputR, blockSize);
    ok = ok && processed;
    int firstNonZero = -1;
    int peakIndex = -1;
    double peakValue = 0.0;
    double rightPeak = 0.0;
    for (int i = 0; i < totalSamples; ++i) {
      ok = ok && std::isfinite(outputL[i]) && std::isfinite(outputR[i]);
      if (firstNonZero < 0 && std::abs(outputL[i]) > 1.0e-10) firstNonZero = i;
      if (std::abs(outputL[i]) > peakValue) { peakValue = std::abs(outputL[i]); peakIndex = i; }
      rightPeak = std::max(rightPeak, std::abs(outputR[i]));
    }
    std::printf("default impulse: latency=%u first=%d peak=%d rightPeak=%.3e\n",
                reportedLatency, firstNonZero, peakIndex, rightPeak);
    ok = ok && firstNonZero >= 2047 && firstNonZero <= expectedLatency &&
         std::abs(peakIndex - expectedLatency) <= 1 && rightPeak < 1.0e-10 &&
         reportedLatency == expectedLatency;
  }

  {
    PluginInstance instance;
    std::printf("creating identity instance\n");
    const bool instanceReady = CreateInstance(factory, instance, 48000.0, blockSize);
    ok = ok && instanceReady;
    // Param IDs follow EParams. Amount=0 makes the STFT path an identity.
    const bool amountSet = instanceReady && SetNormalized(instance, 0, 0.0);
    const bool mixSet = instanceReady && SetNormalized(instance, 7, 1.0); // wet STFT identity check
    const bool restarted = instanceReady && RestartProcessing(instance, 48000.0, blockSize);
    ok = ok && amountSet && mixSet && restarted;
    std::vector<double> inputL(totalSamples), inputR(totalSamples);
    std::vector<double> outputL, outputR;
    uint32_t state = 0x12345678u;
    for (int i = signalStart; i < totalSamples; ++i) {
      state = state * 1664525u + 1013904223u;
      inputL[i] = (static_cast<double>(state) / 4294967296.0 - 0.5) * 0.12;
      state = state * 1664525u + 1013904223u;
      inputR[i] = (static_cast<double>(state) / 4294967296.0 - 0.5) * 0.12;
    }
    const bool processed = instanceReady && ProcessSignal(instance, inputL, inputR, outputL, outputR, blockSize);
    ok = ok && processed;
    double maxError = 0.0;
    for (int i = signalStart + expectedLatency + 4096; i < totalSamples; ++i) {
      maxError = std::max(maxError, std::abs(outputL[i] - inputL[i - expectedLatency]));
      maxError = std::max(maxError, std::abs(outputR[i] - inputR[i - expectedLatency]));
    }
    int firstNonZero = -1;
    for (int i = 0; i < totalSamples; ++i) {
      if (std::abs(outputL[i]) > 1.0e-7) { firstNonZero = i; break; }
    }
    int bestLag = -1;
    double bestRms = 1.0e9;
    for (int lag = 1900; lag <= 4300; ++lag) {
      double errorPower = 0.0;
      int count = 0;
      for (int i = signalStart + lag + 4096; i < totalSamples; ++i) {
        const double eL = outputL[i] - inputL[i - lag];
        const double eR = outputR[i] - inputR[i - lag];
        errorPower += eL * eL + eR * eR;
        count += 2;
      }
      const double rms = std::sqrt(errorPower / std::max(1, count));
      if (rms < bestRms) { bestRms = rms; bestLag = lag; }
    }
    std::printf("identity/null: first=%d maxError@%d=%.3e bestLag=%d rms=%.3e\n",
                firstNonZero, expectedLatency, maxError, bestLag, bestRms);
    ok = ok && maxError < 2.0e-6;
  }

  {
    constexpr int fftSizes[] = {256, 512, 1024, 2048, 4096, 8192, 16384};
    for (int selector = 0; selector < 7; ++selector) {
      PluginInstance instance;
      const bool instanceReady = CreateInstance(factory, instance, 48000.0, blockSize);
      const bool fftSet = instanceReady && SetNormalized(instance, 4, selector / 6.0);
      const bool mixSet = instanceReady && SetNormalized(instance, 7, 0.0);
      const bool restarted = instanceReady && RestartProcessing(instance, 48000.0, blockSize);
      const int latency = fftSizes[selector] - 1;
      const int length = latency + 512;
      std::vector<double> inputL(length, 0.0), inputR(length, 0.0), outputL, outputR;
      inputL[0] = 0.125;
      const bool processed = restarted && ProcessSignal(instance, inputL, inputR, outputL, outputR, blockSize);
      int firstNonZero = -1;
      for (int i = 0; i < length; ++i) {
        if (!std::isfinite(outputL[i]) || !std::isfinite(outputR[i])) ok = false;
        if (firstNonZero < 0 && std::abs(outputL[i]) > 1.0e-12) firstNonZero = i;
      }
      const uint32 reported = instanceReady ? instance.processor->getLatencySamples() : 0;
      std::printf("FFT %5d: latency=%u dryImpulse=%d\n", fftSizes[selector], reported, firstNonZero);
      ok = ok && instanceReady && fftSet && mixSet && restarted && processed &&
           reported == static_cast<uint32>(latency) && firstNonZero == latency;
    }
  }

  {
    PluginInstance instance;
    const bool instanceReady = CreateInstance(factory, instance, 96000.0, blockSize);
    ok = ok && instanceReady;
    uint32_t noiseState = 0xA5A5F00Du;
    int casesPassed = 0;
    for (int target = 0; target < 10; ++target) {
      for (int stereo = 0; stereo < 4; ++stereo) {
        const bool paramsSet =
          SetNormalized(instance, 0, 1.0) &&
          SetNormalized(instance, 1, target / 9.0) &&
          SetNormalized(instance, 2, ((target + stereo) & 1) ? 1.0 : 0.0) &&
          SetNormalized(instance, 3, (stereo & 1) ? 1.0 : 0.0) &&
          SetNormalized(instance, 4, 0.0) && // FFT 256 for rapid stress coverage
          SetNormalized(instance, 5, (target & 1) ? 1.0 : 0.0) &&
          SetNormalized(instance, 6, (stereo & 1) ? 1.0 : 0.0) &&
          SetNormalized(instance, 7, 1.0) &&
          SetNormalized(instance, 8, stereo / 3.0) &&
          SetNormalized(instance, 9, ((target + stereo) & 1) ? 1.0 : 0.0);
        const bool restarted = paramsSet && RestartProcessing(instance, 96000.0, blockSize);
        std::vector<double> inputL(4096), inputR(4096), outputL, outputR;
        for (int i = 0; i < 4096; ++i) {
          noiseState = noiseState * 1664525u + 1013904223u;
          inputL[i] = (static_cast<double>(noiseState) / 4294967296.0 - 0.5) * 0.5;
          noiseState = noiseState * 1664525u + 1013904223u;
          inputR[i] = (static_cast<double>(noiseState) / 4294967296.0 - 0.5) * 0.5;
        }
        const bool processed = restarted && ProcessSignal(instance, inputL, inputR, outputL, outputR, blockSize);
        bool finiteAndBounded = processed;
        double outputPeak = 0.0;
        for (int i = 0; i < 4096 && finiteAndBounded; ++i) {
          finiteAndBounded = std::isfinite(outputL[i]) && std::isfinite(outputR[i]);
          outputPeak = std::max(outputPeak, std::abs(outputL[i]));
          outputPeak = std::max(outputPeak, std::abs(outputR[i]));
        }
        finiteAndBounded = finiteAndBounded && outputPeak <= 1.001;
        ok = ok && finiteAndBounded;
        if (finiteAndBounded) ++casesPassed;
      }
    }
    std::printf("parameter stress @96k: %d/40 cases finite and bounded\n", casesPassed);
  }

  if (factory) factory->release();
  if (exitDll) exitDll();
  FreeLibrary(module);
  std::printf("%s\n", ok ? "PASS" : "FAIL");
  return ok ? 0 : 1;
}
