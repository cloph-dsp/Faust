#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControl.h"

using namespace iplug;
using namespace igraphics;

#include <cmath>
#include <cstring>
#include <atomic>
#include <functional>

constexpr int kMaxSteps = 16;
constexpr int kNumCircles = 3;
constexpr int kParamsPerCircle = 20; // 1 Steps + 1 Note + 1 Solo + 1 Mute + 16 step bools

// Custom IControl: single circle with dynamic step dots (1..kMaxSteps).
// Clicks toggle a step, right-click clears the pattern.
class CircleSteps final : public IControl {
public:
  CircleSteps(IRECT bounds, const IColor& onCol, const IColor& offCol,
              const IColor& playCol, const IColor& bgCol)
    : IControl(bounds)
    , mOnCol(onCol), mOffCol(offCol), mPlayCol(playCol), mBgCol(bgCol)
  {
    SetTooltip("Click a dot to toggle, right-click to clear");
  }

  void Draw(IGraphics& g) override {
    const IRECT& R = mRECT;
    const float cx = R.MW();
    const float cy = R.MH();
    const float radius = std::min(R.W(), R.H()) * 0.45f;

    g.FillCircle(mBgCol.WithOpacity(0.20f), cx, cy, radius);

    const int steps = std::max(1, std::min(kMaxSteps, mSteps));
    const int stepIdx = mPlayhead;
    const float dotR = std::max(8.0f, 24.0f - steps * 0.70f);
    for (int s = 0; s < steps; s++) {
      const float a = (float(s) / float(steps)) * 6.2831853f - 1.5707963f;
      const float dx = cx + std::cos(a) * radius;
      const float dy = cy + std::sin(a) * radius;
      const bool on = (s < kMaxSteps) ? mPattern[s] : false;
      const bool isPlay = (s == stepIdx);
      IColor c = on ? mOnCol : mOffCol;
      if (isPlay) c = mPlayCol;
      g.FillCircle(c, dx, dy, dotR);
      if (on && isPlay) {
        g.DrawCircle(mPlayCol.WithOpacity(0.6f), dx, dy, dotR * 1.8f, nullptr, 4.0f);
      }
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    const IRECT& R = mRECT;
    const float cx = R.MW();
    const float cy = R.MH();
    const float radius = std::min(R.W(), R.H()) * 0.45f;
    const int steps = std::max(1, std::min(kMaxSteps, mSteps));

    if (mod.R) {
      for (int s = 0; s < kMaxSteps; s++) mPattern[s] = false;
      SetDirty();
      if (mParamChangeFn) mParamChangeFn();
      return;
    }

    const float dotR = std::max(8.0f, 24.0f - steps * 0.70f) + 8.0f;
    for (int s = 0; s < steps; s++) {
      const float a = (float(s) / float(steps)) * 6.2831853f - 1.5707963f;
      const float dx = cx + std::cos(a) * radius;
      const float dy = cy + std::sin(a) * radius;
      const float dxm = x - dx;
      const float dym = y - dy;
      if (dxm * dxm + dym * dym < dotR * dotR) {
        mPattern[s] = !mPattern[s];
        SetDirty();
        if (mParamChangeFn) mParamChangeFn();
        return;
      }
    }
  }

  void SetSteps(int steps) { mSteps = steps; SetDirty(); }
  void SetPlayhead(int p) { if (p != mPlayhead) { mPlayhead = p; SetDirty(); } }
  void SetPattern(const bool* p) { memcpy(mPattern, p, sizeof(mPattern)); SetDirty(); }
  void GetPattern(bool* p) const { memcpy(p, mPattern, sizeof(mPattern)); }
  void SetParamChangeFn(std::function<void()> fn) { mParamChangeFn = std::move(fn); }

private:
  IColor mOnCol, mOffCol, mPlayCol, mBgCol;
  int mSteps = 8;
  int mPlayhead = -1;
  bool mPattern[kMaxSteps] = {0};
  std::function<void()> mParamChangeFn;
};

class ThreeBeatProblem final : public Plugin {
public:
  explicit ThreeBeatProblem(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
#endif

  void OnParamChange(int paramIdx) override;
  void OnIdle() override;

  enum EParams {
    kParamCircle0Steps = 0,
    kParamSwing = kNumCircles * kParamsPerCircle,     // 60
    kParamMutation,                                      // 61
    kNumParams = 62
  };

  // Param layout per circle: [Steps, Note, Solo, Mute, Step0..Step15]
  static int StepsParam(int c) { return kParamCircle0Steps + c * kParamsPerCircle; }
  static int NoteParam(int c)  { return kParamCircle0Steps + c * kParamsPerCircle + 1; }
  static int SoloParam(int c)  { return kParamCircle0Steps + c * kParamsPerCircle + 18; }
  static int MuteParam(int c)  { return kParamCircle0Steps + c * kParamsPerCircle + 19; }
  static int StepParam(int c, int s) { return kParamCircle0Steps + c * kParamsPerCircle + 2 + s; }

private:
  // Internal sine-voice synth (shared across all circles)
  struct Voice {
    bool mActive = false;
    int  mNote = 60;
    double mPhase = 0.0;
    int  mSamplesLeft = 0;
    int  mTotalSamples = 1;
  };
  static constexpr int kMaxVoices = 8;
  Voice mVoices[kMaxVoices];

  // Per-circle cached state (audio-thread reads)
  int mCachedSteps[kNumCircles] = {8, 8, 8};
  int mCachedNote[kNumCircles] = {60, 64, 67}; // C4, E4, G4 defaults
  bool mCachedPatterns[kNumCircles][kMaxSteps] = {};

  // Global cached state
  int mCachedSwing = 0;
  int mCachedMutation = 0;

  // Per-circle step tracking
  int mCurrentSteps[kNumCircles] = {-1, -1, -1};
  std::atomic<int> mCurrentStepUIs[kNumCircles];

  // UI control pointers
  CircleSteps* mCircles[kNumCircles] = {};
};
