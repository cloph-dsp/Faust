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
constexpr int kParamsPerCircle = 21; // 1 Steps + 1 Note + 1 Solo + 1 Mute + 1 FillGap + 16 step bools

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
    kParamSwing = kNumCircles * kParamsPerCircle,     // 63
    kParamMutation,                                      // 64
    kNumParams = 65
  };

  // Public methods for Fill/Rotate pattern actions
  void FillCircle(int c);
  void RotateCircle(int c, int direction); // direction: -1=left, +1=right

  // Param layout per circle: [Steps, Note, Solo, Mute, FillGap, Step0..Step15]
  static int StepsParam(int c)   { return kParamCircle0Steps + c * kParamsPerCircle; }
  static int NoteParam(int c)    { return kParamCircle0Steps + c * kParamsPerCircle + 1; }
  static int SoloParam(int c)    { return kParamCircle0Steps + c * kParamsPerCircle + 18; }
  static int MuteParam(int c)    { return kParamCircle0Steps + c * kParamsPerCircle + 19; }
  static int FillGapParam(int c) { return kParamCircle0Steps + c * kParamsPerCircle + 20; }
  static int StepParam(int c, int s) { return kParamCircle0Steps + c * kParamsPerCircle + 2 + s; }

  // Inline gap-enum bar (5 cells: gap=1,2,3,4,8) + Fill button per circle
  class FillControl final : public IControl {
  public:
    FillControl(IRECT bounds, int circleIdx, std::function<void()> onFill)
      : IControl(bounds), mCircleIdx(circleIdx), mOnFill(std::move(onFill)) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      const int gapVals[5] = {1, 2, 3, 4, 8};
      const float cellW = R.W() / 6.0f;
      for (int i = 0; i < 5; i++) {
        IRECT cellR(R.L + i * cellW, R.T, R.L + (i + 1) * cellW - 2, R.B);
        IColor bg = (i == mGapSel - 1) ? IColor(255, 41, 211, 178) : IColor(255, 30, 45, 51);
        g.FillRect(bg, cellR);
        g.DrawRect(IColor(255, 54, 81, 95), cellR);
        char label[4];
        snprintf(label, sizeof(label), "%d", gapVals[i]);
        IColor txtCol = (i == mGapSel - 1) ? IColor(255, 15, 20, 23) : IColor(255, 233, 242, 244);
        g.DrawText(IText(18.f, txtCol, nullptr, EAlign::Center, EVAlign::Middle), label, cellR);
      }
      IRECT btnR(R.R - cellW + 2, R.T, R.R, R.B);
      g.FillRect(IColor(255, 41, 211, 178), btnR);
      g.DrawRect(IColor(255, 54, 81, 95), btnR);
      g.DrawText(IText(18.f, IColor(255, 15, 20, 23), nullptr, EAlign::Center, EVAlign::Middle), "FILL", btnR);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      const IRECT& R = mRECT;
      const float cellW = R.W() / 6.0f;
      const int cell = int((x - R.L) / cellW);
      if (cell >= 0 && cell < 5) {
        mGapSel = cell + 1;
        SetDirty();
      } else if (cell == 5) {
        if (mOnFill) mOnFill();
      }
    }
    int GetGapSel() const { return mGapSel; }
  private:
    int mCircleIdx;
    int mGapSel = 1;
    std::function<void()> mOnFill;
  };

  // Inline rotate L/R buttons per circle
  class RotateControl final : public IControl {
  public:
    RotateControl(IRECT bounds, int circleIdx, int direction, std::function<void()> onRotate)
      : IControl(bounds), mCircleIdx(circleIdx), mDirection(direction), mOnRotate(std::move(onRotate)) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      g.FillRect(IColor(255, 30, 45, 51), R);
      g.DrawRect(IColor(255, 54, 81, 95), R);
      const char* arrow = mDirection < 0 ? "<" : ">";
      g.DrawText(IText(20.f, IColor(255, 41, 211, 178), nullptr, EAlign::Center, EVAlign::Middle), arrow, R);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (mOnRotate) mOnRotate();
    }
  private:
    int mCircleIdx;
    int mDirection; // -1=left, +1=right
    std::function<void()> mOnRotate;
  };

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

  // Per-circle cached fill gap
  int mCachedFillGap[kNumCircles] = {1, 1, 1};

  // Per-circle step tracking
  int mCurrentSteps[kNumCircles] = {-1, -1, -1};
  std::atomic<int> mCurrentStepUIs[kNumCircles];

  // UI control pointers
  CircleSteps* mCircles[kNumCircles] = {};
  FillControl* mFills[kNumCircles] = {};
  RotateControl* mRotatesL[kNumCircles] = {};
  RotateControl* mRotatesR[kNumCircles] = {};
};
