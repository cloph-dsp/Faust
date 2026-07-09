#pragma once

// =============================================================================
// Tuner.h - iPlug2 plugin class declaration
// =============================================================================

#include <memory>
#include <optional>
#include <atomic>
#include <array>
#include <cmath>
#include <chrono>

// FTZ/DAZ intrinsics for real-time denormal protection.
//   - On MSVC x86/x64: <intrin.h> provides _mm_setcsr / _mm_getcsr.
//   - On GCC/Clang x86/x64: <xmmintrin.h> does the same.
//   - On ARM64 (macOS Apple Silicon, Linux aarch64): NEON uses FPCR/FPSCR
//     instead, and the math library handles denormals at the OS level --
//     no runtime flush is needed.  We skip the include + call entirely.
// TUNER_HAS_SSE_DENORMALS gates the call site at the constructor so the
// binary compiles cleanly on every host arch, not just x86.
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
  #include <intrin.h>
  #define TUNER_HAS_SSE_DENORMALS 1
#elif defined(__SSE__)
  // GCC/Clang need the explicit SSE intrinsic header; MSVC pulls them
  // in via <intrin.h> above.  Without this, _mm_getcsr/_mm_setcsr are
  // undeclared on macOS Clang (and any non-MSVC x64 toolchain).
  #include <xmmintrin.h>
  #define TUNER_HAS_SSE_DENORMALS 1
#else
  #define TUNER_HAS_SSE_DENORMALS 0
#endif

#include "IPlug_include_in_plug_hdr.h"
#include "FaustCompat.h"
#include "IControls.h"

// TunerAnalysis::Detector pulled out of Tuner.cpp into a header-only
// module so the detector can be linked without iPlug2 (host-free smoke
// test, see Tuner VST/tests/detector_smoke.cpp).  Header-only means no
// separate .cpp / vcxproj / xcodeproj edit to add a new TU.
#include "TunerAnalysis.h"

const int kNumPresets = 4;

using namespace iplug;
using namespace igraphics;


// =============================================================================
// TunerReadoutControl -- draws the live tuner face
// =============================================================================
//
// Layout (540×300):
//   - top:      note name (64px large) + octave + cents (right-aligned beside note)
//   - below:    frequency in Hz (14px)
//   - middle:   horizontal 5-tick tuning meter with animated needle + teal in-tune glow
//   - left:     horizontal smoothing slider (150×24px, drag left/right, double-click resets)
//   - center:   A4 reference segmented bar (6 cells)
//   - bottom-right: CLOPH logo (72px, teal tint)
//
// Animation:
//   - distance-based needle smoothing (4 tiers: 0.30 / 0.12 / 0.05 / 0.015)
//   - smoothMul = (smoothAmount < 0.01) ? 100 : 0.02 + (1 - smoothAmount) * 1.8
//   - in-tune glow lerp at 0.12 toward (hasSignal && |cents|<=2 ? 1 : 0)
//   - color tier: <=1 -> teal, <=3 -> lime, <=7 -> yellow, <=15 -> amber, <=30 -> orange, else -> red

class TunerReadoutControl : public iplug::igraphics::IControl {
public:
  TunerReadoutControl(const IRECT& bounds, class Tuner* pTuner)
    : iplug::igraphics::IControl(bounds, kNoParameter)
    , mTuner(pTuner)
  {
  }

  void Draw(IGraphics& g) override;
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
  void OnMouseUp(float x, float y, const IMouseMod& mod) override;
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override;
  void OnMouseOver(float x, float y, const IMouseMod& mod) override;
  void OnMouseOut() override;

  // Read by the Tuner plugin to publish the current smoothing knob value.
  // NOTE: no Get/SetSmoothingAmount here — the smoothing knob reads the
  // plugin param directly in Draw (single source of truth), so GUI drags
  // and host automation always agree.

  // For OnIdle to ask the control to redraw without taking action.
  void MarkForRedraw() { SetDirty(false); }

  // The CLOPH logo SVG (loaded once at LayoutUI time, drawn every frame).
  void SetLogo(iplug::igraphics::ISVG svg) { mLogo = svg; }

private:
  class Tuner*                 mTuner       = nullptr;
  std::optional<iplug::igraphics::ISVG> mLogo;

  // Animated needle position (0..100%) -- mirrors Amorph's needle state.
  float mNeedlePos = 50.0f;
  float mGlow      = 0.0f;

  // Smoothing slider hit rect (set during Draw, used by mouse handlers).
  IRECT mSmoothRect;

  // Mouse-drag state for the horizontal smoothing slider.
  float mDragStartX      = 0.0f;
  float mDragStartSmooth = 0.0f;

  // Slider interaction states -- brighten when hovered/dragged.
  bool mIsHovering = false;
  bool mIsDragging = false;

  // A4 cell hover tracking (0-5, -1 = none).
  int mHoveredA4Cell = -1;
};


class TunerDSPWrapper;

enum EParams {
  kParamSmoothing = 0,
  kParamA4Ref,
  kNumParams
};


class Tuner final : public iplug::Plugin {
public:
  explicit Tuner(const InstanceInfo& info);

  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
  void OnRestoreState() override;
  void OnParamChange(int paramIdx) override;
  void OnReset() override;
  void OnActivate(bool active) override;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;

#if IPLUG_EDITOR
  void OnIdle() override;
  void LayoutUI(IGraphics* pGraphics) override;
#endif

  const TunerAnalysis::Result& GetAnalysisResult() const {
    return mDetector.GetResult();
  }

private:
  std::unique_ptr<TunerDSPWrapper> mDSP;
  TunerAnalysis::Detector          mDetector;
  bool mSendUpdate = false;
};