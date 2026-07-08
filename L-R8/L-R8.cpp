#include "L-R8.h"
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include <string>
#ifdef OS_WIN
#include <windows.h>
#include <intrin.h>
#elif defined(__APPLE__) && defined(__x86_64__)
#include <xmmintrin.h>
#endif
#include "IPlug_include_in_plug_src.h"
#include "IPlugPaths.h"
#include "IControls.h"

using namespace iplug;
using namespace iplug::igraphics;

namespace {

constexpr float kPi = 3.14159265358979323846f;

// Font IDs — long names to avoid conflict with iplug::EParamSource::kUI
constexpr const char* kFontID_UI = "UIFont";
constexpr const char* kFontID_Val = "ValFont";

// Colors — dark industrial palette
const IColor colBG   {255,14,16,22};
const IColor colPN   {255,48,50,62};
const IColor colTX   {255,225,228,240};
const IColor colTD   {255,140,145,160};
const IColor colTM   {255,85,90,105};
const IColor colL    {255,30,210,215};   // teal — side L
const IColor colR    {255,245,155,55};   // amber — side R
const IColor colM    {255,175,182,195};  // cool gray — mid
const IColor colGlow {180,80,220,240};  // teal glow
const IColor colGR   {255,42,44,56};
const IColor colSB   {255,20,22,32};
const IColor colKB   {255,36,38,50};
const IColor colKA   {255,50,52,64};
const IColor colFB   {255,24,26,36};

float C01(float v) { return std::max(0.f,std::min(1.f,v)); }
template<typename T> T CV(T v,T mn,T mx) { return std::max(mn,std::min(mx,v)); }
float LRP(float a,float b,float t) { return a+(b-a)*t; }
float EC(float t) { t=C01(t); return t*t*(3.f-2.f*t); }
float EQ(float t) { t=C01(t); float i=1.f-t; return 1.f-i*i*i*i*i; }
IColor WA(const IColor& c,int a) { return IColor(std::max(0,std::min(255,a)),c.R,c.G,c.B); }
IColor BC(const IColor& a,const IColor& b,float t) {
  t=C01(t); auto m=[t](int x,int y){return int(std::lround(x+(y-x)*t));};
  return IColor(m(a.A,b.A),m(a.R,b.R),m(a.G,b.G),m(a.B,b.B));
}
float DR(float d) { return d*kPi/180.f; }

bool LoadFontUI(IGraphics* g) {
  return g&&(g->LoadFont(kFontID_UI,"Segoe UI",ETextStyle::Normal)||g->LoadFont(kFontID_UI,"Tahoma",ETextStyle::Normal)||g->LoadFont(kFontID_UI,"Arial",ETextStyle::Normal));
}
bool LoadFontVal(IGraphics* g) {
  return g&&(g->LoadFont(kFontID_Val,"Consolas",ETextStyle::Normal)||g->LoadFont(kFontID_Val,"Courier New",ETextStyle::Normal)||g->LoadFont(kFontID_Val,"Lucida Console",ETextStyle::Normal)||g->LoadFont(kFontID_Val,"Courier",ETextStyle::Normal));
}
IText MakeTxt(float sz,const IColor& c,const char* f=kFontID_UI,EAlign a=EAlign::Center,EVAlign va=EVAlign::Middle) {
  return IText{sz,c,f,a,va};
}

class ZoneCaptureUI final : public UI {
public:
  explicit ZoneCaptureUI(std::unordered_map<std::string,FAUSTFLOAT*>& z) : mZ(z) {}
  void addCheckButton(const char* l,FAUSTFLOAT* z) override { mZ[l]=z; }
  void addHorizontalSlider(const char* l,FAUSTFLOAT* z,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT) override { mZ[l]=z; }
  void addVerticalSlider(const char* l,FAUSTFLOAT* z,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT) override { mZ[l]=z; }
  void addNumEntry(const char* l,FAUSTFLOAT* z,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT,FAUSTFLOAT) override { mZ[l]=z; }
private:
  std::unordered_map<std::string,FAUSTFLOAT*>& mZ;
};

// ════════════════════════════════════════════════════════════════════
// STEREO WATERFALL — scroll-zoomable, gray mid, teal L, amber R
// ════════════════════════════════════════════════════════════════════
class ScopeControl final : public IControl {
public:
  static constexpr int kHistorySize = 1024;
  ScopeControl(const IRECT& b) : IControl(b,kNoParameter), mBufL(kHistorySize,0.f), mBufR(kHistorySize,0.f), mBufMid(kHistorySize,0.f), mCorrelation(1.f), mCorrelationRaw(1.f) { Clear(); }
  void Push(float l,float r,float mid) {
    mBufL[mPos]=l; mBufR[mPos]=r; mBufMid[mPos]=mid;
    mPos=(mPos+1)%kHistorySize;
    SetDirty(false);
  }
  void SetCorrelation(float c) {
    // Smooth: EMA with 0.95/0.05 blend
    mCorrelationRaw = c;
    mCorrelation = mCorrelation * 0.95f + c * 0.05f;
  }
  void Clear() { std::fill(mBufL.begin(),mBufL.end(),0.f); std::fill(mBufR.begin(),mBufR.end(),0.f); std::fill(mBufMid.begin(),mBufMid.end(),0.f); mPos=0; mCorrelation=1.f; mCorrelationRaw=1.f; }
  void HapticPulse() {
    mPulse=1.f;
    SetAnimation([](IControl* c){auto* s=c->As<ScopeControl>();s->mPulse=1.f-EC(float(s->GetAnimationProgress()));s->SetDirty(false);},200);
  }
  void OnEndAnimation() override { mPulse=0.f; IControl::OnEndAnimation(); }

  void OnMouseWheel(float x,float y,const IMouseMod& mod,float d) override {
    mZoom*= (d>0.f) ? 1.15f : (1.f/1.15f);
    mZoom=CV(mZoom,0.3f,5.f);
    SetDirty(false);
  }

  void Draw(IGraphics& g) override {
    const IRECT r=mRECT.GetPadded(-2.f);
    float w=r.W(),h=r.H();
    float baseScale=w*0.42f*mZoom;
    float lineH=h/float(kHistorySize);

    // Background
    g.FillRoundRect(colSB,r,6.f);
    g.DrawRoundRect(colPN,r,6.f,nullptr,1.f);
    if(mPulse>0.01f) g.FillRoundRect(WA(colGlow,int(25.f*mPulse)),r,6.f);

    // ── Waterfall: Bottom→Top (newest at bottom) ──
    for(int i=0;i<kHistorySize;++i) {
      int idx=(mPos-1-i+kHistorySize)%kHistorySize;
      float L=mBufL[idx],R=mBufR[idx];
      float mid=(L+R)*0.5f;
      float side=(L-R)*0.5f;
      float peakVal=std::max(std::abs(L),std::abs(R));
      float y=r.B-(float(i+1)/float(kHistorySize))*h;
      float lineW=std::max(1.f,lineH);

      float midW=std::abs(mid)*baseScale;
      float sideW=std::abs(side)*baseScale;

      // Higher alpha for better visibility (restored to near-original levels)
      float midAlpha=CV(std::abs(mid)*300.f,60.f,200.f);
      float sideAlpha=CV(std::abs(side)*360.f,80.f,240.f);

      // Mid: cool gray
      if(midW>0.5f) {
        g.FillRect(WA(colM,int(midAlpha*0.7f)),IRECT(r.MW()-midW,y-lineW,r.MW(),y));
        g.FillRect(WA(colM,int(midAlpha*0.5f)),IRECT(r.MW(),y-lineW,r.MW()+midW,y));
      }

      // Side: teal L (negative), amber R (positive)
      if(side>0.f) {
        if(sideW>0.5f) g.FillRect(WA(colL,int(sideAlpha)),IRECT(r.MW()-sideW,y-lineW,r.MW(),y));
      } else {
        if(sideW>0.5f) g.FillRect(WA(colR,int(sideAlpha)),IRECT(r.MW(),y-lineW,r.MW()+sideW,y));
      }

      // ── Peak glow: signals above -6dBFS get a highlight pass ──
      const float glowThreshold=0.5f;
      if(peakVal>glowThreshold) {
        float glowIntensity=CV((peakVal-glowThreshold)*4.f,0.f,1.f);
        int glowAlpha=int(glowIntensity*80.f);
        if(glowAlpha>5) {
          // Slightly wider +1px on each side for the glow spread
          if(midW>0.5f)
            g.FillRect(WA(colM,glowAlpha),IRECT(r.MW()-midW-1.f,y-lineW-1.f,r.MW()+midW+1.f,y+1.f));
          if(sideW>0.5f) {
            if(side>0.f)
              g.FillRect(WA(colL,glowAlpha),IRECT(r.MW()-sideW-1.f,y-lineW-1.f,r.MW(),y+1.f));
            else
              g.FillRect(WA(colR,glowAlpha),IRECT(r.MW(),y-lineW-1.f,r.MW()+sideW,y+1.f));
          }
        }
      }
    }

    // ── Vertical vignette: fade top and bottom edges ──
    auto vg=IPattern::CreateLinearGradient(0,r.T,0,r.B,{{colSB,0.f},{IColor(0,colSB.R,colSB.G,colSB.B),0.12f},{IColor(0,colSB.R,colSB.G,colSB.B),0.88f},{colSB,1.f}});
    g.PathRect(r); g.PathFill(vg);

    // ── dB grid: vertical lines at -6, -12, -18, -24 dB from center ──
    // Amplitude ratio for each dB level
    const float dbLevels[]={-6.f,-12.f,-18.f,-24.f};
    const float halfW=w*0.5f;
    for(int di=0;di<4;++di) {
      float ratio=std::pow(10.f,dbLevels[di]/20.f);
      float offset=ratio*baseScale;
      if(offset<halfW) {
        float xL=r.MW()-offset;
        float xR=r.MW()+offset;
        // Faint vertical line
        g.DrawLine(WA(colGR,25),xL,r.T+4.f,xL,r.B-4.f,nullptr,0.5f);
        g.DrawLine(WA(colGR,25),xR,r.T+4.f,xR,r.B-4.f,nullptr,0.5f);
        // Small dB label at top
        char dbBuf[8];
        std::snprintf(dbBuf,sizeof(dbBuf),"%.0f",dbLevels[di]);
        g.DrawText(MakeTxt(6.f,colTM,kFontID_UI,EAlign::Center,EVAlign::Top),dbBuf,IRECT(xL-10.f,r.T+1.f,xL+10.f,r.T+10.f));
        g.DrawText(MakeTxt(6.f,colTM,kFontID_UI,EAlign::Center,EVAlign::Top),dbBuf,IRECT(xR-10.f,r.T+1.f,xR+10.f,r.T+10.f));
      }
    }

    // Center axis (0 dB / mono reference)
    g.DrawLine(WA(colGR,120),r.MW(),r.T+4.f,r.MW(),r.B-4.f,nullptr,1.f);

    // ── Labels ──
    // MID / SIDE labels at top corners (keep as orientation hints)
    g.DrawText(MakeTxt(8.f,colM,kFontID_UI,EAlign::Near,EVAlign::Top),"MID",IRECT(r.L+4.f,r.T+2.f,r.L+32.f,r.T+16.f));
    g.DrawText(MakeTxt(8.f,BC(colL,colR,0.5f),kFontID_UI,EAlign::Far,EVAlign::Top),"SIDE",IRECT(r.R-32.f,r.T+2.f,r.R-4.f,r.T+16.f));

    // Zoom indicator (bottom-left corner)
    char zBuf[16]; std::snprintf(zBuf,sizeof(zBuf),"x%.1f",mZoom);
    g.DrawText(MakeTxt(8.f,colTM,kFontID_UI,EAlign::Far,EVAlign::Bottom),zBuf,IRECT(r.R-6.f,r.B-12.f,r.R,r.B));

    // ── Correlation meter: horizontal bar at bottom ──
    const float cmH=14.f;
    const float cmY=r.B-cmH-4.f;
    const float cmX=r.L+60.f;
    const float cmW=r.W()-120.f;

    // Background track
    g.FillRect(WA(colGR,60),IRECT(cmX,cmY,cmX+cmW,cmY+cmH));
    g.DrawRect(WA(colPN,40),IRECT(cmX,cmY,cmX+cmW,cmY+cmH),nullptr,0.5f);

    // Fill from center
    float cW=cmW*0.5f;
    float cFill=std::abs(mCorrelation)*cW;
    float cMid=cmX+cW;

    if(mCorrelation>=0.f) {
      if(cFill>0.5f) {
        auto cg=IPattern::CreateLinearGradient(0,cmY,0,cmY+cmH,{{IColor(140,100,240,100),0.f},{IColor(200,100,240,60),1.f}});
        g.PathRect(IRECT(cMid,cmY,cMid+cFill,cmY+cmH));
        g.PathFill(cg);
      }
    } else {
      if(cFill>0.5f) {
        auto cg=IPattern::CreateLinearGradient(0,cmY,0,cmY+cmH,{{IColor(200,240,140,60),0.f},{IColor(140,240,200,100),1.f}});
        g.PathRect(IRECT(cMid-cFill,cmY,cMid,cmY+cmH));
        g.PathFill(cg);
      }
    }

    g.DrawLine(WA(colTM,100),cMid,cmY,cMid,cmY+cmH,nullptr,1.f);

    g.DrawText(MakeTxt(7.f,colTM,kFontID_UI,EAlign::Near,EVAlign::Middle),"-1",IRECT(cmX-18.f,cmY,cmX,cmY+cmH));
    g.DrawText(MakeTxt(7.f,colTM,kFontID_UI,EAlign::Far,EVAlign::Middle),"+1",IRECT(cmX+cmW,cmY,cmX+cmW+18.f,cmY+cmH));
    g.DrawText(MakeTxt(7.f,colTX,kFontID_UI,EAlign::Center,EVAlign::Bottom),"CORR",IRECT(cmX,cmY-2.f,cmX+cmW,cmY));

    // ═══════ MINIMAL DEBUG (corner, won't block waterfall) ═══════
    char ov[64]; std::snprintf(ov,sizeof(ov),"mPos=%d",mPos);
    g.DrawText(MakeTxt(9.f,colTM,kFontID_UI,EAlign::Near,EVAlign::Bottom),ov,IRECT(r.L+4.f,r.B-36.f,r.L+80.f,r.B-22.f));
  }
  std::vector<float> mBufL,mBufR,mBufMid;
  int mPos=0; float mPulse=0.f;
  float mZoom=1.f;
  float mCorrelation; // smoothed correlation display value
  float mCorrelationRaw; // raw per-block correlation for smoothing
};

// ════════════════════════════════════════════════════════════════════
// TOOLTIP OVERLAY — draws label+value pill near hovered controls
// ════════════════════════════════════════════════════════════════════
class TooltipOverlayControl final : public IControl {
public:
  static TooltipOverlayControl* sInstance;
  static void ShowTooltip(const IRECT& src,const char* lbl,const char* val) { if(sInstance) sInstance->SetTooltip(src,lbl,val); }
  static void HideTooltip() { if(sInstance) { sInstance->mVisible=false; sInstance->SetDirty(false); } }
  TooltipOverlayControl(const IRECT& b) : IControl(b,kNoParameter) { mIgnoreMouse=true; sInstance=this; }
  void Draw(IGraphics& g) override {
    if(!mVisible) return;
    float tw=160.f,th=22.f;
    float tx=std::max(4.f,std::min(mSrcRect.MW()-tw*0.5f,mRECT.R-tw-4.f));
    float ty=mSrcRect.T-th-6.f;
    if(ty<mRECT.T+4.f) ty=mSrcRect.B+6.f;
    g.FillRoundRect(colGR,IRECT(tx,ty,tx+tw,ty+th),th*0.5f);
    g.DrawRoundRect(WA(colPN,120),IRECT(tx,ty,tx+tw,ty+th),th*0.5f,nullptr,0.5f);
    WDL_String full; full.SetFormatted(64,"%s: %s",mLbl.Get(),mVal.Get());
    g.DrawText(MakeTxt(11.f,colTX),full.Get(),IRECT(tx+4.f,ty,tx+tw-4.f,ty+th));
  }
private:
  void SetTooltip(const IRECT& src,const char* lbl,const char* val) { mSrcRect=src; mLbl.Set(lbl); mVal.Set(val); mVisible=true; SetDirty(false); }
  IRECT mSrcRect; WDL_String mLbl,mVal; bool mVisible=false;
};
TooltipOverlayControl* TooltipOverlayControl::sInstance=nullptr;

// ════════════════════════════════════════════════════════════════════
// KNOB — personalized: arc track, traveling dot, clean center
// ════════════════════════════════════════════════════════════════════
enum class KnobMode { Generic, RateKnob };

class KnobControl final : public IKnobControlBase {
public:
  KnobControl(const IRECT& b,int p,const char* lbl,KnobMode mode=KnobMode::Generic) : IKnobControlBase(b,p), mLabel(lbl?lbl:""), mKnobMode(mode) { mDblAsSingleClick=true; }
  void SetSvgCap(const ISVG& svg) { mSvgCap = svg; }
  void SetValueFromDelegate(double v,int i=0) override {
    double prv=GetValue(); IControl::SetValueFromDelegate(v,i);
    if(float d=float(std::abs(GetValue()-prv));d>0.002f) TriggerPulse(d);
  }
  void OnMouseOver(float x,float y,const IMouseMod& m) override {
    IKnobControlBase::OnMouseOver(x,y,m);
    if(GetUI())GetUI()->SetMouseCursor(ECursor::HAND);
    mHT=1.f; mHF=mHA;
    SetAnimation([](IControl* c){auto*s=c->As<KnobControl>();s->mHA=s->mHF+(1.f-s->mHF)*EQ(float(s->GetAnimationProgress()));s->SetDirty(false);},120);
    if(TooltipOverlayControl::sInstance&&GetParam()) {
      WDL_String valStr; GetParam()->GetDisplay(valStr);
      TooltipOverlayControl::ShowTooltip(mRECT,mLabel.Get(),valStr.Get());
    }
  }
  void OnMouseOut() override {
    IKnobControlBase::OnMouseOut(); if(GetUI())GetUI()->SetMouseCursor();
    mHF=mHA; mHT=0.f;
    SetAnimation([](IControl* c){auto*s=c->As<KnobControl>();s->mHA=s->mHF*(1.f-EQ(float(s->GetAnimationProgress())));s->SetDirty(false);},80);
    TooltipOverlayControl::HideTooltip();
  }
  void OnEndAnimation() override { mHA=mHT; mPP=0.f; mPK=0.f; IKnobControlBase::OnEndAnimation(); }

  void Draw(IGraphics& g) override {
    float val=float(GetValue()),cx=mRECT.MW(),cy=mRECT.MH();
    float R=std::min(mRECT.W(),mRECT.H())*0.5f-5.f;
    bool hv=GetMouseIsOver(),cp=GetUI()&&GetUI()->ControlIsCaptured(this);
    bool ac=(hv||cp||mMouseDown)&&!IsDisabled();
    float po=mMouseDown?1.5f:0.f;

    // 135°→405° range (7:30 to 4:30)
    float ang=135.f+val*270.f;
    float aRad=DR(ang);

    // Glows
    float ga=std::max(mHA*0.4f,(cp||mMouseDown)?0.7f:0.f);
    if(ga>0.01f) g.FillCircle(WA(colGlow,int(50.f*ga)),cx,cy+po,R+3.f);
    if(mPP>0.01f) g.FillCircle(BC(colL,colR,0.5f).WithOpacity(int(35.f*mPP)),cx,cy+po,R+5.f);

    // Outer track ring (thin, dim)
    float tR=R-2.f;
    g.DrawArc(WA(colKA,80),cx,cy+po,tR,225.f,495.f,nullptr,2.f);

    // Fill arc
    if(val>0.002f) {
      IColor fc=ac?colL:BC(colL,colTD,0.3f);
      g.DrawArc(WA(fc,ac?200:120),cx,cy+po,tR,225.f,225.f+val*270.f,nullptr,2.5f);
    }

    // Face — SVG cap if loaded, else vector-drawn
    float fR=tR-4.f;
    if(mSvgCap.IsValid()) {
      IRECT capRect(cx-fR,cy+po-fR,cx+fR,cy+po+fR);
      g.DrawRotatedSVG(mSvgCap,capRect.MW(),capRect.MH(),capRect.W(),capRect.H(),ang);
      g.DrawCircle(WA(colPN,60),cx,cy+po,fR,nullptr,0.8f);
    } else {
      g.FillCircle(BC(colBG,colPN,0.2f),cx,cy+po,fR);
      g.DrawCircle(WA(colPN,60),cx,cy+po,fR,nullptr,0.8f);
    }

    // Traveling dot — travels along the arc endpoint
    float dotR=fR*0.16f;
    float dx=cx+std::cos(aRad)*(tR-1.f);
    float dy=cy+po+std::sin(aRad)*(tR-1.f);
    g.FillCircle(WA(colL,ac?230:150),dx,dy,dotR);
    if(ac) g.FillCircle(WA(colL,100),dx,dy,dotR*2.f); // glow halo

    // Center indicator — subtle ring
    g.DrawCircle(WA(colL,int(60.f*(ac?1.f:0.4f))),cx,cy+po,fR*0.3f,nullptr,1.2f);

    // Label
    g.DrawText(MakeTxt(10.f,ac?colTX:colTD),mLabel.Get(),IRECT(mRECT.L,mRECT.B-32.f,mRECT.R,mRECT.B-16.f));
    char buf[32];
    if(mKnobMode==KnobMode::RateKnob) {
      auto* d=GetDelegate(); bool synced=false;
      if(d){const IParam* p=d->GetParam(kParamSync);if(p)synced=p->Value()>0.5;}
      if(synced) {
        int ri=int(val*14.f+0.5f);
        int idxs[]={-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8};
        int shift=idxs[CV(ri,0,14)];
        if(shift<0) std::snprintf(buf,sizeof(buf),"1/%d",1<<(-shift));
        else if(shift==0) std::snprintf(buf,sizeof(buf),"1x");
        else std::snprintf(buf,sizeof(buf),"x%d",1<<shift);
      } else {
        float hz=0.1f+val*9.9f;
        std::snprintf(buf,sizeof(buf),"%.1fHz",CV(hz,0.1f,10.f));
      }
      g.DrawText(MakeTxt(12.f,ac?colTX:colTD,kFontID_Val),buf,IRECT(mRECT.L,mRECT.B-16.f,mRECT.R,mRECT.B));
    } else {
      WDL_String valStr;
      if(GetParam()) GetParam()->GetDisplay(valStr); else valStr.Set("---");
      g.DrawText(MakeTxt(11.f,ac?colTX:colTD,kFontID_Val),valStr.Get(),IRECT(mRECT.L,mRECT.B-16.f,mRECT.R,mRECT.B));
    }
  }
private:
  void TriggerPulse(float d){float in=std::min(1.f,0.3f+d*2.f);if(in>mPK){mPK=in;mPP=in;SetAnimation([](IControl* c){auto*s=c->As<KnobControl>();s->mPP=s->mPK*(1.f-EC(float(c->GetAnimationProgress())));s->SetDirty(false);},180);}}
  WDL_String mLabel; KnobMode mKnobMode; ISVG mSvgCap = ISVG(nullptr); float mHA=0.f,mHT=0.f,mHF=0.f,mPP=0.f,mPK=0.f;
};

// ════════════════════════════════════════════════════════════════════
// POWER BUTTON — simple circle with icon
// ════════════════════════════════════════════════════════════════════
class PowerControl final : public ISwitchControlBase {
public:
  PowerControl(const IRECT& b,int p) : ISwitchControlBase(b,p) {}
  void OnInit() override { ISwitchControlBase::OnInit(); mV=GetValue()>0.5?1.f:0.f; mT=mV; }
  void SetValueFromDelegate(double v,int i=0) override { IControl::SetValueFromDelegate(v,i); mV=GetValue()>0.5?1.f:0.f; mA=mV; mT=mV; SetDirty(false); }
  void OnMouseDown(float x,float y,const IMouseMod& m) override { ISwitchControlBase::OnMouseDown(x,y,m); Anim(GetValue()>0.5?1.f:0.f); }
  void OnEndAnimation() override { mV=mT; ISwitchControlBase::OnEndAnimation(); }

  void Draw(IGraphics& g) override {
    float cx=mRECT.MW(),cy=mRECT.MH(),R=std::min(mRECT.W(),mRECT.H())*0.5f-1.f;
    bool on=mV>0.5f;
    // Outer ring
    g.DrawCircle(on?WA(colL,200):WA(colTD,80),cx,cy,R,nullptr,1.8f);
    // Center dot when on
    if(on) g.FillCircle(WA(colL,180),cx,cy,R*0.3f);
    else g.FillCircle(WA(colTD,50),cx,cy,R*0.2f);
    // Power icon: vertical line from center up
    float ln=R*0.5f;
    g.DrawLine(on?WA(colL,220):WA(colTD,100),cx,cy-ln*0.3f,cx,cy+ln*0.8f,nullptr,1.5f);
    // Arc at top (power symbol gap)
    g.DrawArc(on?WA(colL,200):WA(colTD,80),cx,cy-ln*0.1f,R*0.7f,220.f,320.f,nullptr,1.8f);
  }
private:
  void Anim(float t){if(std::abs(mV-t)<0.001f){mV=t;mT=t;SetDirty(false);return;}mA=mV;mT=t;SetAnimation([](IControl* c){auto*s=c->As<PowerControl>();s->mV=LRP(s->mA,s->mT,EC(float(s->GetAnimationProgress())));s->SetDirty(false);},200);}
  float mV=0.f,mA=0.f,mT=0.f;
};

// ════════════════════════════════════════════════════════════════════
// SYNC TOGGLE
// ════════════════════════════════════════════════════════════════════
class SyncControl final : public ISwitchControlBase {
public:
  SyncControl(const IRECT& b,int p) : ISwitchControlBase(b,p) {}
  void Draw(IGraphics& g) override {
    float cx=mRECT.MW(),cy=mRECT.MH(),R=std::min(mRECT.W(),mRECT.H())*0.5f-1.f;
    bool on=GetValue()>0.5f,hover=GetMouseIsOver();
    g.FillCircle(WA(colBG,80),cx+1.f,cy+1.5f,R+1.f);
    g.FillCircle(on?BC(colL,colPN,0.5f):colPN,cx,cy,R);
    if(on) {
      g.FillCircle(WA(colL,30),cx,cy,R+4.f);
      g.DrawText(MakeTxt(10.f,colM,kFontID_UI,EAlign::Center,EVAlign::Middle),"S",mRECT);
    } else {
      g.DrawText(MakeTxt(10.f,colTD,kFontID_UI,EAlign::Center,EVAlign::Middle),"S",mRECT);
    }
    g.DrawCircle(on?colL:WA(colTD,80),cx,cy,R-2.f,nullptr,1.5f);
    if(hover) g.DrawCircle(WA(colTX,40),cx,cy,R+1.f,nullptr,1.f);
  }
};

// ════════════════════════════════════════════════════════════════════
// BPM DISPLAY — click to edit via CreateTextEntry (Freeze95 pattern)
// ════════════════════════════════════════════════════════════════════
class BpmDisplayControl final : public IControl {
public:
  BpmDisplayControl(const IRECT& b,int bpmIdx,int rateIdx,int syncIdx,LR8& p)
    : IControl(b,bpmIdx), mRateIdx(rateIdx), mSyncIdx(syncIdx), mPlugin(p) { mDblAsSingleClick=true; }

  void OnInit() override { SetTextEntryLength(16); }

  void OnResize() override {
    mField=mRECT.GetPadded(-4.f);
    SetTargetRECT(mField);
  }

  void SetValueFromDelegate(double v,int i=0) override {
    IControl::SetValueFromDelegate(v,i); SetDirty(false);
  }

  bool IsManualEnabled() {
    auto* d=GetDelegate(); if(!d) return true;
    const IParam* sp=d->GetParam(mSyncIdx);
    return !sp||sp->Value()<0.5||!mPlugin.GetTransportIsRunning();
  }

  void OnMouseOver(float x,float y,const IMouseMod&) override {
    if(IsManualEnabled()&&GetUI()) GetUI()->SetMouseCursor(ECursor::HAND);
    SetDirty(false);
  }

  void OnMouseOut() override {
    if(GetUI()) GetUI()->SetMouseCursor();
    SetDirty(false);
  }

  void OnMouseDown(float x,float y,const IMouseMod&) override {
    if(!IsManualEnabled()) { SetDirty(false); return; }
    OpenTextEntry();
    SetDirty(false);
  }

  void OnMouseDblClick(float x,float y,const IMouseMod&) override { OnMouseDown(x,y,IMouseMod()); }

  bool OnKeyDown(float x,float y,const IKeyPress& key) override {
    if(key.C||key.A||!IsManualEnabled()) return false;
    if(key.VK==kVK_RETURN||key.VK==kVK_SPACE){OpenTextEntry();return true;}
    if(key.VK==kVK_UP){Nudge(1.0);return true;}
    if(key.VK==kVK_DOWN){Nudge(-1.0);return true;}
    return false;
  }

  void OnTextEntryCompletion(const char* str,int) override {
    if(!GetParam()||!IsManualEnabled()) return;
    char* end=nullptr; double v=std::strtod(str,&end);
    if(end==str||(end&&*end&&!std::isspace(static_cast<unsigned char>(*end)))) {
      mFlashV=1.f; mFlashI=1.f;
      SetAnimation([](IControl* c){auto*s=c->As<BpmDisplayControl>();s->mFlashI=1.f-EC(float(c->GetAnimationProgress()));s->SetDirty(false);},220);
      return;
    }
    mFlashV=1.f;
    SetAnimation([](IControl* c){auto*s=c->As<BpmDisplayControl>();s->mFlashV=1.f-EQ(float(c->GetAnimationProgress()));s->SetDirty(false);},180);
    SetValueFromUserInput(GetParam()->ToNormalized(CV(v,GetParam()->GetMin(),GetParam()->GetMax())));
  }

  void Draw(IGraphics& g) override {
    auto* d=GetDelegate(); if(!d) return;
    IRECT r=mRECT;
    g.FillRoundRect(WA(colFB,200),r,6.f); g.DrawRoundRect(WA(colPN,120),r,6.f,nullptr,1.f);
    if(mFlashI>0.01f) g.DrawRoundRect(WA(colR,180),r.GetPadded(-1.f),6.f,nullptr,2.f);
    else if(mFlashV>0.01f) g.DrawRoundRect(WA(colL,150),r.GetPadded(-1.f),6.f,nullptr,1.5f);
    g.DrawText(MakeTxt(9.f,colTM,kFontID_UI,EAlign::Near,EVAlign::Top),"BPM",IRECT(r.L+8.f,r.T+5.f,r.R,r.T+18.f));
    const IParam* sp=d->GetParam(mSyncIdx);
    bool sync=sp&&sp->Value()>0.5;
    bool hostRunning=mPlugin.GetTransportIsRunning();
    bool manual=!sync||!hostRunning;
    double bpm;
    if(!manual) {
      bpm=CV(mPlugin.GetTempo(),1.0,300.0);
    } else {
      const IParam* bp=d->GetParam(GetParamIdx());
      bpm=bp?CV(bp->Value(),1.0,300.0):120.0;
    }
    IColor bpmCol=manual?colL:colM;
    char buf[32]; std::snprintf(buf,sizeof(buf),"%.1f",bpm);
    g.DrawText(MakeTxt(28.f,bpmCol,kFontID_Val),buf,IRECT(r.L,r.MH()-18.f,r.R,r.MH()+18.f));
    const IParam* rp=d->GetParam(mRateIdx);
    double rv=rp?rp->Value():0.5;
    int ri=int(rv*14.0+0.5);
    int shift=ri-7;
    if(shift>0) std::snprintf(buf,sizeof(buf),"x%d",1<<shift);
    else if(shift==0) std::snprintf(buf,sizeof(buf),"1x");
    else std::snprintf(buf,sizeof(buf),"1/%d",1<<(-shift));
    g.DrawText(MakeTxt(10.f,colTM),buf,IRECT(r.L,r.B-20.f,r.R,r.B-4.f));
  }

private:
  void Nudge(double d){
    if(!GetParam())return;
    double c=GetParam()->FromNormalized(GetValue());
    SetValueFromUserInput(GetParam()->ToNormalized(CV(c+d,GetParam()->GetMin(),GetParam()->GetMax())));
  }

  void OpenTextEntry() {
    auto* ui=GetUI(); if(!ui||!GetParam()) return;
    char buf[32]; std::snprintf(buf,sizeof(buf),"%.1f",CV(GetParam()->FromNormalized(GetValue()),GetParam()->GetMin(),GetParam()->GetMax()));
    IRECT eb=mField.GetPadded(-2.f);
    float fs=std::min(24.f,std::max(10.f,eb.H()*0.6f));
    IText txt{fs,colTX,kFontID_Val,EAlign::Center,EVAlign::Middle};
    ui->CreateTextEntry(*this,txt,eb,buf);
  }

  int mRateIdx,mSyncIdx; IRECT mField; LR8& mPlugin;
  float mFlashV=0.f,mFlashI=0.f;
};

} // anon namespace

// ════════════════════════════════════════════════════════════════════
// LR8 Plugin
// ════════════════════════════════════════════════════════════════════
LR8::LR8(const InstanceInfo& info)
  : Plugin(info,MakeConfig(kNumParams,kNumPresets))
  , mVisBufferL(kVisBufferSize,0.f), mVisBufferR(kVisBufferSize,0.f) {

  GetParam(kParamBypass)->InitBool("Bypass",false);
  GetParam(kParamSync)->InitBool("Sync",true);
  GetParam(kParamRate)->InitInt("Rate",7,0,14,"step");
  GetParam(kParamBpm)->InitDouble("BPM",120.0,1.0,300.0,1.0,"bpm");
  GetParam(kParamFreeRate)->InitDouble("Free Rate",2.0,0.1,10.0,0.1,"Hz");
  GetParam(kParamCrossfade)->InitDouble("Crossfade",0.0,0.0,1.0,0.01,"");
  GetParam(kParamHighPass)->InitDouble("High Pass",1000.0,20.0,1000.0,1.0,"Hz");
  GetParam(kParamLowPass)->InitDouble("Low Pass",8000.0,1000.0,20000.0,1.0,"Hz");

  // Factory presets
  MakePreset("Default", 
    GetParam(kParamBypass)->ToNormalized(false),
    GetParam(kParamSync)->ToNormalized(true),
    GetParam(kParamRate)->ToNormalized(7.0),
    GetParam(kParamBpm)->ToNormalized(120.0),
    GetParam(kParamFreeRate)->ToNormalized(2.0),
    GetParam(kParamCrossfade)->ToNormalized(0.0),
    GetParam(kParamHighPass)->ToNormalized(1000.0),
    GetParam(kParamLowPass)->ToNormalized(8000.0));
  MakePreset("Wide Swirl",
    GetParam(kParamBypass)->ToNormalized(false),
    GetParam(kParamSync)->ToNormalized(true),
    GetParam(kParamRate)->ToNormalized(5.0),
    GetParam(kParamBpm)->ToNormalized(120.0),
    GetParam(kParamFreeRate)->ToNormalized(2.0),
    GetParam(kParamCrossfade)->ToNormalized(0.7),
    GetParam(kParamHighPass)->ToNormalized(500.0),
    GetParam(kParamLowPass)->ToNormalized(12000.0));
  MakePreset("Slow Motion",
    GetParam(kParamBypass)->ToNormalized(false),
    GetParam(kParamSync)->ToNormalized(false),
    GetParam(kParamRate)->ToNormalized(7.0),
    GetParam(kParamBpm)->ToNormalized(120.0),
    GetParam(kParamFreeRate)->ToNormalized(0.5),
    GetParam(kParamCrossfade)->ToNormalized(0.3),
    GetParam(kParamHighPass)->ToNormalized(200.0),
    GetParam(kParamLowPass)->ToNormalized(6000.0));
  MakePreset("Rhythmic Pulse",
    GetParam(kParamBypass)->ToNormalized(false),
    GetParam(kParamSync)->ToNormalized(true),
    GetParam(kParamRate)->ToNormalized(10.0),
    GetParam(kParamBpm)->ToNormalized(140.0),
    GetParam(kParamFreeRate)->ToNormalized(2.0),
    GetParam(kParamCrossfade)->ToNormalized(0.5),
    GetParam(kParamHighPass)->ToNormalized(800.0),
    GetParam(kParamLowPass)->ToNormalized(15000.0));
  MakePreset("Airy Shift",
    GetParam(kParamBypass)->ToNormalized(false),
    GetParam(kParamSync)->ToNormalized(false),
    GetParam(kParamRate)->ToNormalized(7.0),
    GetParam(kParamBpm)->ToNormalized(120.0),
    GetParam(kParamFreeRate)->ToNormalized(3.5),
    GetParam(kParamCrossfade)->ToNormalized(0.2),
    GetParam(kParamHighPass)->ToNormalized(1000.0),
    GetParam(kParamLowPass)->ToNormalized(20000.0));

  mDSP=std::make_unique<L8FaustDSP>();
  ZoneCaptureUI cap(mFaustZones); mDSP->buildUserInterface(&cap);
  SyncParamsToDSP();

  mInL.resize(kMaxBlockFrames); mInR.resize(kMaxBlockFrames);
  mOutL.resize(kMaxBlockFrames); mOutR.resize(kMaxBlockFrames);

#if IPLUG_EDITOR
  mMakeGraphicsFunc=[&]() {
#ifdef OS_WIN
    return MakeGraphics(*this,PLUG_WIDTH,PLUG_HEIGHT,PLUG_FPS,1.f);
#else
    return MakeGraphics(*this,PLUG_WIDTH,PLUG_HEIGHT,PLUG_FPS,GetScaleForScreen(PLUG_WIDTH,PLUG_HEIGHT));
#endif
  };
  mLayoutFunc=[&](IGraphics* g){LayoutUI(g);};
#endif
}

#if IPLUG_EDITOR
void LR8::LayoutUI(IGraphics* g) {
  float W=float(g->Width()),H=float(g->Height());
  g->SetScaleConstraints(0.5f,2.0f);
  if(g->NControls()) return;
  g->EnableMouseOver(true);
  g->AttachTextEntryControl();
  g->AttachPanelBackground(colBG);
  LoadFontUI(g); LoadFontVal(g);
  
  // SVG knob asset — loaded once, shared by all knobs via SetSvgCap
  ISVG knobSvg = g->LoadSVG(KNOB_CAP_FN);

  const float M=8.f,G=6.f;

  // TOP BAR
  float barH=28.f;
  g->AttachControl(new PowerControl(IRECT(M+4.f,M+2.f,M+36.f,M+barH-2.f),kParamBypass));
  g->AttachControl(new ITextControl(IRECT(W-90.f,M,W-4.f,M+barH),"L-R8",MakeTxt(12.f,colTM,kFontID_UI,EAlign::Far,EVAlign::Middle)));

  // SIDE PANELS + SCOPE
  float sideW=std::clamp(W*0.18f,120.f,150.f);
  float scopeL=M+sideW+G;
  float scopeR=W-M-sideW-G;

  // L/R labels above scope
  float lblY=barH+4.f;
  float lblH=20.f;
  g->AttachControl(new ITextControl(IRECT(scopeL,lblY,scopeL+(scopeR-scopeL)*0.5f,lblY+lblH),"L",MakeTxt(13.f,colTM,kFontID_UI,EAlign::Center,EVAlign::Bottom)));
  g->AttachControl(new ITextControl(IRECT(scopeL+(scopeR-scopeL)*0.5f,lblY,scopeR,lblY+lblH),"R",MakeTxt(13.f,colTM,kFontID_UI,EAlign::Center,EVAlign::Bottom)));

  // Scope
  float scopeT=lblY+lblH;
  float scopeB=H-70.f;
  const IRECT sRect(scopeL,scopeT,scopeR,scopeB);
  auto* scope=new ScopeControl(sRect);
  mVisControlPtr.store(scope, std::memory_order_release);
  g->AttachControl(scope);

  // LEFT KNOBS
  float lL=M,lR=M+sideW;
  float kY=barH+8.f;
  float kH=(scopeB-kY)*0.42f;

  auto mkKnob = [&](const IRECT& b,int p,const char* lbl,KnobMode mode=KnobMode::Generic) {
    auto* k = new KnobControl(b,p,lbl,mode);
    if(knobSvg.IsValid()) k->SetSvgCap(knobSvg);
    return k;
  };

  g->AttachControl(mkKnob(IRECT(lL+2.f,kY,lR-2.f,kY+kH),kParamFreeRate,"Rate",KnobMode::RateKnob));
  float k2Y=kY+kH+G;
  g->AttachControl(mkKnob(IRECT(lL+2.f,k2Y,lR-2.f,k2Y+kH),kParamCrossfade,"Cross"));

  // RIGHT KNOBS
  float rL=scopeR+G,rR=W-M;
  g->AttachControl(mkKnob(IRECT(rL+2.f,kY,rR-2.f,kY+kH),kParamHighPass,"Split"));
  g->AttachControl(mkKnob(IRECT(rL+2.f,k2Y,rR-2.f,k2Y+kH),kParamLowPass,"Air"));

  // BOTTOM BAR
  float bY=H-62.f,bH=54.f;
  g->AttachControl(new SyncControl(IRECT(M+6.f,bY+4.f,M+50.f,bY+48.f),kParamSync));
  g->AttachControl(new BpmDisplayControl(IRECT(M+58.f,bY+2.f,W-M-4.f,bY+bH),kParamBpm,kParamRate,kParamSync,*this));

  g->AttachControl(new TooltipOverlayControl(IRECT(0,0,W,H)));

  g->AttachCornerResizer(EUIResizerMode::Scale,false,
    IColor(60,255,255,255),WA(colTD,120),WA(colTX,210),24.f);
}

void LR8::OnParentWindowResize(int w,int h) {
  if(!GetUI()) return;
  float s=CV(std::min(float(w)/float(PLUG_WIDTH),float(h)/float(PLUG_HEIGHT)),0.5f,2.0f);
  GetUI()->Resize(PLUG_WIDTH,PLUG_HEIGHT,s,false);
}
#endif

void LR8::SetFaustParam(const char* l,float v) {
  auto it=mFaustZones.find(l);
  if(it!=mFaustZones.end()&&it->second) *it->second=v;
}

void LR8::OnUIClose() {
  // Reset vis control pointer so audio thread doesn't use deleted control
  mVisControlPtr.store(nullptr, std::memory_order_release);
  // Spin until any in-flight ProcessBlock scope access finishes
  // (should be nearly instant — just a few float writes + SetDirty)
  while(mVisBusy.load(std::memory_order_acquire)) {
    _mm_pause();
  }
}

void LR8::SyncParamsToDSP() {
  if(!mDSP) return;
  float bp=float(GetParam(kParamBypass)->Value()>0.5);
  // NOTE: bypass inversion intentional. Faust's "Bypass" = 0 means DSP active,
  // = 1 means DSP bypassed. The iPlug bypass param (1=bypass, 0=process) is
  // inverted here so they align: plugin bypassed → Faust processing off.
  SetFaustParam("Bypass",bp?0.f:1.f);
  bool sync=GetParam(kParamSync)->Value()>0.5;
  SetFaustParam("Sync",sync?1.f:0.f);

  // Host transport sync: read tempo from DAW when synced and running
  double hostTempo = GetTempo();
  bool transportRunning = GetTransportIsRunning();
  if(sync && transportRunning && hostTempo > 0.0) {
    SetFaustParam("BPM", float(hostTempo));
  } else {
    SetFaustParam("BPM", float(GetParam(kParamBpm)->Value()));
  }

  // Dual-function Rate knob
  float fv=float(GetParam(kParamFreeRate)->Value());
  if(sync) {
    float norm=(fv-0.1f)/9.9f;
    int ridx=CV(int(norm*14.f+0.5f),0,14);
    SetFaustParam("Rate",float(ridx));
    SetFaustParam("Free Rate",0.1f);
  } else {
    SetFaustParam("Free Rate",fv);
    SetFaustParam("Rate",7.f);
  }

  SetFaustParam("Cross",float(GetParam(kParamCrossfade)->Value()));
  SetFaustParam("High Pass",float(GetParam(kParamHighPass)->Value()));
  SetFaustParam("Low Pass",float(GetParam(kParamLowPass)->Value()));
}

void LR8::OnParamChange(int idx) {
  switch(idx) {
    case kParamBypass: case kParamRate: case kParamBpm:
    case kParamFreeRate: case kParamCrossfade: case kParamHighPass: case kParamLowPass:
      SyncParamsToDSP();
      if(GetUI()) {
        GetUI()->SetAllControlsDirty();
        if(auto* sc = static_cast<ScopeControl*>(mVisControlPtr.load(std::memory_order_acquire))) sc->HapticPulse();
      }
      break;
    case kParamSync: {
      // Smooth transition: remap knob so LFO speed doesn't jump
      bool nowSync = GetParam(kParamSync)->Value() > 0.5;
      double fv = GetParam(kParamFreeRate)->Value();
      double rv = GetParam(kParamRate)->Value();
      if(nowSync) {
        // Sync ON: map free_rate position to nearest rate division
        double norm = (fv - 0.1) / 9.9;
        int newRate = CV(int(norm * 14.0 + 0.5), 0, 14);
        GetParam(kParamRate)->Set(newRate / 14.0);
      } else {
        // Sync OFF: map rate position to equivalent free_rate
        double newFree = 0.1 + (rv / 14.0) * 9.9;
        GetParam(kParamFreeRate)->Set((newFree - 0.1) / 9.9);
      }
      SyncParamsToDSP();
      if(GetUI()) {
        GetUI()->SetAllControlsDirty();
        if(auto* sc = static_cast<ScopeControl*>(mVisControlPtr.load(std::memory_order_acquire))) sc->HapticPulse();
      }
      break;
    }
    default: break;
  }
}

void LR8::OnReset() {
#ifdef _WIN32
  _mm_setcsr(_mm_getcsr()|0x8040);
#endif
  if(!mDSP) return;
  mDSP->init(int(GetSampleRate()));
  mBufferSize=std::max(16384,GetBlockSize());
  std::fill(mVisBufferL.begin(),mVisBufferL.end(),0.f);
  std::fill(mVisBufferR.begin(),mVisBufferR.end(),0.f);
  mVisWritePos=0;
  SyncParamsToDSP();
}

void LR8::ProcessBlock(sample** in,sample** out,int nF) {
#ifdef _WIN32
  _mm_setcsr(_mm_getcsr()|0x8040);
#elif defined(__APPLE__)&&defined(__x86_64__)
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

  int nI=NInChansConnected(),nO=NOutChansConnected(),origN=nF;
  auto dry=[&](int st,int ct) {
    int ch=std::min(nI,nO);
    for(int s=0;s<ct;++s){for(int c=0;c<ch;++c)out[c][st+s]=in[c][st+s];for(int c=ch;c<nO;++c)out[c][st+s]=0.0;}
  };
  if(!mDSP||nF<=0){dry(0,origN);return;}
  if(GetBypassed()){dry(0,origN);return;}
  if(mBufferSize<nF)nF=mBufferSize;

  if(nI>=2){for(int s=0;s<nF;++s){mInL[s]=float(in[0][s]);mInR[s]=float(in[1][s]);}}
  else if(nI==1){for(int s=0;s<nF;++s){float t=float(in[0][s]);mInL[s]=t;mInR[s]=t;}}
  else{std::fill(mInL.begin(),mInL.begin()+nF,0.f);std::fill(mInR.begin(),mInR.begin()+nF,0.f);}

  mInPtrs[0]=mInL.data();mInPtrs[1]=mInR.data();
  mOutPtrs[0]=mOutL.data();mOutPtrs[1]=mOutR.data();
  mDSP->control();mDSP->compute(nF,mInPtrs,mOutPtrs);

  mTargetBypassGain=GetParam(kParamBypass)->Value()>0.5f?0.f:1.f;
  const float ramp=0.04f;
  if(nO>=2){for(int s=0;s<nF;++s){mBypassGain+=(mTargetBypassGain-mBypassGain)*ramp;out[0][s]=sample(mOutL[s]*mBypassGain+in[0][s]*(1.f-mBypassGain));out[1][s]=sample(mOutR[s]*mBypassGain+in[1][s]*(1.f-mBypassGain));}}
  else if(nO==1){for(int s=0;s<nF;++s){mBypassGain+=(mTargetBypassGain-mBypassGain)*ramp;out[0][s]=sample(mOutL[s]*mBypassGain+in[0][s]*(1.f-mBypassGain));}}
  int ex=(nO>=2)?2:1;
  for(int c=ex;c<nO;++c)std::fill(out[c],out[c]+nF,sample(0.0));
  if(nF<origN)dry(nF,origN-nF);

  constexpr int kDec=48;
  for(int s=0;s<nF;s+=kDec){mVisBufferL[mVisWritePos]=mOutL[s];mVisBufferR[mVisWritePos]=mOutR[s];mVisWritePos=(mVisWritePos+1)%kVisBufferSize;}

  // Compute correlation over the first min(nF,256) samples of this block
  float sumL2=0.f,sumR2=0.f,sumLR=0.f;
  int ct=std::min(nF,256);
  for(int s=0;s<ct;++s){
    sumL2+=mOutL[s]*mOutL[s];
    sumR2+=mOutR[s]*mOutR[s];
    sumLR+=mOutL[s]*mOutR[s];
  }

  mVisBusy.store(1, std::memory_order_release);
  if(auto* sc = static_cast<ScopeControl*>(mVisControlPtr.load(std::memory_order_acquire))){
    int last=(mVisWritePos-1+kVisBufferSize)%kVisBufferSize;
    float l=mVisBufferL[last],r=mVisBufferR[last];
    sc->Push(l,r,(l+r)*0.5f);
    // Set correlation: compute from block statistics
    float corr=0.f;
    if(sumL2*sumR2>1e-12f)
      corr=sumLR/std::sqrt(sumL2*sumR2);
    sc->SetCorrelation(corr);
  }
  mVisBusy.store(0, std::memory_order_release);

  // Transport stop → clear scope
  bool running = GetTransportIsRunning();
  if(mTransportWasRunning&&!running){
    mVisBusy.store(1, std::memory_order_release);
    if(auto* sc = static_cast<ScopeControl*>(mVisControlPtr.load(std::memory_order_acquire))) sc->Clear();
    mVisBusy.store(0, std::memory_order_release);
  }
  mTransportWasRunning = running;
}

bool LR8::SerializeState(IByteChunk& c) const {
  // Write version tag for forward compatibility
  c.Put(&kStateVersion);
  return SerializeParams(c);
}
int LR8::UnserializeState(const IByteChunk& c,int s) {
  int version=0;
  c.Get(&version,s);
  s+=sizeof(int);
  if(version!=kStateVersion) {
    return -1; // Unknown version — can't safely deserialize
  }
  if(GetUI()) GetUI()->SetAllControlsDirty();
  int r=UnserializeParams(c,s); SyncParamsToDSP(); return r;
}
