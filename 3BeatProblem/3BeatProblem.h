#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControl.h"

using namespace iplug;
using namespace igraphics;

#include <cmath>
#include <cstring>
#include <cstdint>
#include <atomic>
#include <functional>
#include <array>

// Struct definitions must be in the header so controls can access members
struct ChordType {
  const char* name;
  int n;
  int intervals[8];  // n values: root(0) + n-1 scale-degree offsets; max is 7th chord = 8
};

struct ScaleType {
  const char* name;
  int n;
  int intervals[12];
};

// kScales is defined in 3BeatProblem.cpp (extern so control classes can reference it)
extern const ScaleType kScales[80];

// kScales defined in 3BeatProblem.cpp

constexpr int kMaxSteps = 16;
constexpr int kNumCircles = 3;
constexpr int kParamsPerCircle = 25; // 1 Steps + 1 Chord + 1 Scale + 1 OctMin + 1 OctMax + 1 Solo + 1 Mute + 1 FillGap + 1 PlayProb + 16 step bools = 25
static constexpr int kHistorySize = 32;

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

    // Capture pre-toggle snapshot for undo (before any mutation)
    for (int i = 0; i < kMaxSteps; i++) mPreToggleSnapshot[i] = mPattern[i];

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
  void GetPreToggleSnapshot(bool arr[kMaxSteps]) const { memcpy(arr, mPreToggleSnapshot, sizeof(mPreToggleSnapshot)); }

private:
  IColor mOnCol, mOffCol, mPlayCol, mBgCol;
  int mSteps = 8;
  int mPlayhead = -1;
  bool mPattern[kMaxSteps] = {0};
  bool mPreToggleSnapshot[kMaxSteps] = {0};
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
    kParamSwing = kNumCircles * kParamsPerCircle,     // = 75
    kParamMutation,                                      // = 76
    kNumParams = 77
  };

  // Public methods for Fill/Rotate/Clear pattern actions
  void FillCircle(int c);
  void RotateCircle(int c, int direction); // direction: -1=left, +1=right
  void ClearCircle(int c);
  void InvertCircle(int c);
  void UndoCircle(int c);
  void RedoCircle(int c);
  // public for CircleSteps callback access
  void PushHistory(int c);
  void PushHistoryExplicit(int c, const bool pre[kMaxSteps]);
  int PickRandomNote(int c); // samples (chord ∩ scale ∩ octaves) pool; returns 0..127

  // Param layout per circle: [Steps, Chord, Scale, OctMin, OctMax, Solo, Mute, FillGap, PlayProb, Step0..Step15]
  static int StepsParam(int c)     { return kParamCircle0Steps + c * kParamsPerCircle; }
  static int ChordParam(int c)     { return kParamCircle0Steps + c * kParamsPerCircle + 1; }
  static int ScaleParam(int c)     { return kParamCircle0Steps + c * kParamsPerCircle + 2; }
  static int OctMinParam(int c)    { return kParamCircle0Steps + c * kParamsPerCircle + 3; }
  static int OctMaxParam(int c)    { return kParamCircle0Steps + c * kParamsPerCircle + 4; }
  static int SoloParam(int c)      { return kParamCircle0Steps + c * kParamsPerCircle + 21; }
  static int MuteParam(int c)      { return kParamCircle0Steps + c * kParamsPerCircle + 22; }
  static int FillGapParam(int c)   { return kParamCircle0Steps + c * kParamsPerCircle + 23; }
  static int PlayProbParam(int c)   { return kParamCircle0Steps + c * kParamsPerCircle + 5; }
  static int StepParam(int c, int s) { return kParamCircle0Steps + c * kParamsPerCircle + 6 + s; }
  // Inline gap-enum bar (5 cells: gap=1,2,3,4,8) + Fill button per circle
  class FillControl final : public IControl {
  public:
    FillControl(IRECT bounds, int circleIdx, std::function<void()> onFill, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mOnFill(std::move(onFill)), mFontName(fontName) {}
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
        g.DrawText(IText(18.f, txtCol, mFontName, EAlign::Center, EVAlign::Middle), label, cellR);
      }
      IRECT btnR(R.R - cellW + 2, R.T, R.R, R.B);
      g.FillRect(IColor(255, 41, 211, 178), btnR);
      g.DrawRect(IColor(255, 54, 81, 95), btnR);
      g.DrawText(IText(18.f, IColor(255, 15, 20, 23), mFontName, EAlign::Center, EVAlign::Middle), "FILL", btnR);
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
    void SetGapSel(int g) {
      if (g >= 1 && g <= 5 && g != mGapSel) {
        mGapSel = g;
        SetDirty();
      }
    }
  private:
    int mCircleIdx;
    int mGapSel = 1;
    std::function<void()> mOnFill;
    const char* mFontName;
  };

  // Inline rotate L/R buttons per circle
  class RotateControl final : public IControl {
  public:
    RotateControl(IRECT bounds, int circleIdx, int direction, std::function<void()> onRotate, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mDirection(direction), mOnRotate(std::move(onRotate)), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      g.FillRect(IColor(255, 30, 45, 51), R);
      g.DrawRect(IColor(255, 54, 81, 95), R);
      // Draw arrow as vector lines (volunmo.ttf lacks < > glyphs)
      const float ax = R.MW();
      const float ay = R.MH();
      const float sz = std::min(R.W(), R.H()) * 0.22f;
      const IColor arrowCol(255, 41, 211, 178);
      if (mDirection < 0) {
        g.DrawLine(arrowCol, ax + sz, ay - sz, ax - sz, ay, nullptr, 3.f);
        g.DrawLine(arrowCol, ax - sz, ay, ax + sz, ay + sz, nullptr, 3.f);
      } else {
        g.DrawLine(arrowCol, ax - sz, ay - sz, ax + sz, ay, nullptr, 3.f);
        g.DrawLine(arrowCol, ax + sz, ay, ax - sz, ay + sz, nullptr, 3.f);
      }
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (mOnRotate) mOnRotate();
    }
  private:
    int mCircleIdx;
    int mDirection; // -1=left, +1=right
    std::function<void()> mOnRotate;
    const char* mFontName;
  };

  // Clear button per circle (resets all 16 step toggles)
  class ClearControl final : public IControl {
  public:
    ClearControl(IRECT bounds, int circleIdx, std::function<void()> onClear, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mOnClear(std::move(onClear)), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      g.FillRect(IColor(255, 30, 45, 51), R);
      g.DrawRect(IColor(255, 54, 81, 95), R);
      g.DrawText(IText(18.f, IColor(255, 233, 242, 244), mFontName, EAlign::Center, EVAlign::Middle), "CLR", R);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (mOnClear) mOnClear();
    }
  private:
    int mCircleIdx;
    std::function<void()> mOnClear;
    const char* mFontName;
  };

  // Invert button per circle (inverts all step toggles)
  class InvertControl final : public IControl {
  public:
    InvertControl(IRECT bounds, int circleIdx, std::function<void()> onInvert, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mOnInvert(std::move(onInvert)), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      g.FillRect(IColor(255, 41, 211, 178), R);
      g.DrawRect(IColor(255, 54, 81, 95), R);
      g.DrawText(IText(18.f, IColor(255, 233, 242, 244), mFontName, EAlign::Center, EVAlign::Middle), "INV", R);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (mOnInvert) mOnInvert();
    }
  private:
    int mCircleIdx;
    std::function<void()> mOnInvert;
    const char* mFontName;
  };

  // Undo button per circle
  class UndoControl final : public IControl {
  public:
    UndoControl(IRECT bounds, int circleIdx, std::function<void()> onUndo, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mOnUndo(std::move(onUndo)), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      const float a = mEnabled ? 1.0f : 0.3f;
      g.FillRect(IColor(255, 30, 45, 51).WithOpacity(a), R);
      g.DrawRect(IColor(255, 54, 81, 95).WithOpacity(a), R);
      IColor iconCol = IColor(255, 233, 242, 244).WithOpacity(a);
      const float cx = R.MW(), cy = R.MH();
      g.DrawLine(iconCol, cx-7, cy-4, cx-7, cy+5, nullptr, 1.5f);
      g.DrawLine(iconCol, cx-7, cy-4, cx+5, cy-4, nullptr, 1.5f);
      g.DrawLine(iconCol, cx+5, cy-4, cx+7, cy+5, nullptr, 1.5f);
      g.DrawLine(iconCol, cx+7, cy+5, cx+2, cy+2, nullptr, 1.5f);
      g.DrawLine(iconCol, cx+7, cy+5, cx+4, cy+8, nullptr, 1.5f);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (!mEnabled || !mOnUndo) return;
      mOnUndo();
    }
    void SetEnabled(bool e) { mEnabled = e; SetDirty(); }
  private:
    int mCircleIdx;
    std::function<void()> mOnUndo;
    const char* mFontName;
    bool mEnabled = true;
  };

  // Redo button per circle
  class RedoControl final : public IControl {
  public:
    RedoControl(IRECT bounds, int circleIdx, std::function<void()> onRedo, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mOnRedo(std::move(onRedo)), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      const IRECT& R = mRECT;
      const float a = mEnabled ? 1.0f : 0.3f;
      g.FillRect(IColor(255, 30, 45, 51).WithOpacity(a), R);
      g.DrawRect(IColor(255, 54, 81, 95).WithOpacity(a), R);
      IColor iconCol = IColor(255, 233, 242, 244).WithOpacity(a);
      const float cx = R.MW(), cy = R.MH();
      g.DrawLine(iconCol, cx+7, cy-4, cx+7, cy+5, nullptr, 1.5f);
      g.DrawLine(iconCol, cx+7, cy-4, cx-5, cy-4, nullptr, 1.5f);
      g.DrawLine(iconCol, cx-5, cy-4, cx-7, cy+5, nullptr, 1.5f);
      g.DrawLine(iconCol, cx-7, cy+5, cx-2, cy+2, nullptr, 1.5f);
      g.DrawLine(iconCol, cx-7, cy+5, cx-4, cy+8, nullptr, 1.5f);
    }
    void OnMouseDown(float x, float y, const IMouseMod& mod) override {
      if (!mEnabled || !mOnRedo) return;
      mOnRedo();
    }
    void SetEnabled(bool e) { mEnabled = e; SetDirty(); }
  private:
    int mCircleIdx;
    std::function<void()> mOnRedo;
    const char* mFontName;
    bool mEnabled = true;
  };

  // Wave 3: Static helpers for chord/scale parsing (defined in .cpp, used by nested controls)
  static int ParseChordIdStatic(const char* s);
  static int ParseScaleIdxStatic(const char* s);
  static void FormatChordIdStatic(int chordId, char* buf, int bufSize);

  // Custom display control: shows chord name as text, opens ITextEntryControl on dbl-click.
  class ChordDisplayControl final : public IControl {
  public:
    ChordDisplayControl(IRECT bounds, int circleIdx, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      char buf[32];
      FormatChordIdStatic(GetParam(ChordParam(mCircleIdx))->Int(), buf, sizeof(buf));
      g.FillRect(IColor(255, 30, 38, 44), mRECT);
      g.DrawRect(IColor(255, 54, 81, 95), mRECT);
      g.DrawText(IText(15.f, IColor(255, 233, 242, 244), mFontName, EAlign::Near, EVAlign::Middle), buf, mRECT);
    }
    void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
      char buf[32];
      FormatChordIdStatic(GetParam(ChordParam(mCircleIdx))->Int(), buf, sizeof(buf));
      GetUI()->CreateTextEntry(*this, IText(15.f, IColor(255, 233, 242, 244), mFontName, EAlign::Near, EVAlign::Middle),
                               mRECT, buf);
    }
    void OnTextEntryCompletion(const char* str, int valIdx) override {
      int id = ParseChordIdStatic(str);
      if (id >= 0) {
        const_cast<IParam*>(GetParam(ChordParam(mCircleIdx)))->Set((double)id);
        SetDirty(false);
      }
    }
  private:
    int mCircleIdx;
    const char* mFontName;
  };

  // Custom display control: shows scale name as text, opens ITextEntryControl on dbl-click.
  class ScaleDisplayControl final : public IControl {
  public:
    ScaleDisplayControl(IRECT bounds, int circleIdx, const char* fontName)
      : IControl(bounds), mCircleIdx(circleIdx), mFontName(fontName) {}
    void Draw(IGraphics& g) override {
      int idx = GetParam(ScaleParam(mCircleIdx))->Int();
      const char* name = (idx >= 0 && idx < 80) ? kScales[idx].name : "";
      g.FillRect(IColor(255, 30, 38, 44), mRECT);
      g.DrawRect(IColor(255, 54, 81, 95), mRECT);
      g.DrawText(IText(15.f, IColor(255, 233, 242, 244), mFontName, EAlign::Near, EVAlign::Middle), name, mRECT);
    }
    void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
      int idx = GetParam(ScaleParam(mCircleIdx))->Int();
      const char* name = (idx >= 0 && idx < 80) ? kScales[idx].name : "";
      GetUI()->CreateTextEntry(*this, IText(15.f, IColor(255, 233, 242, 244), mFontName, EAlign::Near, EVAlign::Middle),
                               mRECT, const_cast<char*>(name));
    }
    void OnTextEntryCompletion(const char* str, int valIdx) override {
      int idx = ParseScaleIdxStatic(str);
      if (idx >= 0) {
        const_cast<IParam*>(GetParam(ScaleParam(mCircleIdx)))->Set((double)idx);
        SetDirty(false);
      }
    }
  private:
    int mCircleIdx;
    const char* mFontName;
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
  int mCachedChordId[kNumCircles] = {7, 7, 7};   // packed root*100+type; 7 = C maj7
  int mCachedScaleId[kNumCircles] = {2, 2, 2};    // Ionian
  int mCachedOctMin[kNumCircles] = {4, 4, 4};
  int mCachedOctMax[kNumCircles] = {5, 5, 5};
  int mCachedPlayProb[kNumCircles] = {80, 80, 80}; // 80% chance to fire per step
  bool mCachedPatterns[kNumCircles][kMaxSteps] = {};

  // Global cached state
  int mCachedSwing = 0;
  int mCachedMutation = 0;

  // Audio-thread PRNG (xorshift, no mutex)
  uint32_t mRngState = 0xCAFEF00Du;
  uint32_t RandXor() { mRngState ^= mRngState << 13; mRngState ^= mRngState >> 17; mRngState ^= mRngState << 5; return mRngState; }

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
  ClearControl* mClears[kNumCircles] = {};
  InvertControl* mInverts[kNumCircles] = {};
  UndoControl* mUndos[kNumCircles] = {};
  RedoControl* mRedos[kNumCircles] = {};
  // Wave 3: LEFT side panel display controls (ChordDisplayControl / ScaleDisplayControl)
  IControl* mChordSelectors[kNumCircles] = {};
  IControl* mScaleSelectors[kNumCircles] = {};

  // Per-circle history ring (32 pre-state snapshots per circle)
  std::array<bool, kMaxSteps> mHistory[kNumCircles][kHistorySize];
  int mHistoryHead[kNumCircles] = {0, 0, 0};
  int mHistoryCount[kNumCircles] = {0, 0, 0};
  // Per-circle redo stack (cleared on any new edit)
  std::array<bool, kMaxSteps> mRedoStack[kNumCircles][kHistorySize];
  int mRedoStackHead[kNumCircles] = {0, 0, 0};
  int mRedoCount[kNumCircles] = {0, 0, 0};
};
