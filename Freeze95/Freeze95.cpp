#include "Freeze95.h"

#include <algorithm>

#include <chrono>
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
constexpr const char* kDOSFontID = "DOSFont";
constexpr const char* kFallbackFontID = "UIFontFallback";
constexpr const char* kTertiaryFontID = "UITertiaryFont";
constexpr const char* kUtilityFontID = "UIUtilityFont";
constexpr float kRetroTTFSizeScale = 0.9f;
constexpr float kMinTouchTargetSize = 44.f;

const char* gActiveFontID = kDOSFontID;
bool gUsePixelTextFallback = true;
bool gUseUtilityFont = false;
bool gUseTertiaryFont = false;

const IColor kShellBg         {255, 168, 159, 145};
const IColor kShellFace       {255, 211, 197, 148};
const IColor kShellLight      {255, 241, 228, 179};
const IColor kShellMid        {255, 184, 166, 119};
const IColor kShellDark       {255, 126, 113,  76};
const IColor kShellDeep       {255,  82,  72,  47};
const IColor kShellText       {255,  72,  60,  32};
const IColor kShellTextDim    {255, 102,  88,  52};
const IColor kFieldFace       {255, 202, 187, 137};
const IColor kFieldInset      {255, 145, 129,  83};
const IColor kFieldText       {255,  65,  53,  27};
const IColor kKnobSocket      {255, 171, 155, 112};
const IColor kKnobRim         {255,  92,  82,  57};
const IColor kKnobFace        {255, 182, 167, 122};
const IColor kKnobTop         {255, 225, 211, 158};
const IColor kKnobPointer     {255, 248, 239, 194};
const IColor kKnobPointerDark {255,  70,  58,  33};
const IColor kCoolOn          {255, 244, 228, 178};
const IColor kCoolGlow        {100, 255, 230, 154};
const IColor kCoolOff         {255, 136, 118,  77};
const IColor kLedOff          {255, 110,  30,  30};
const IColor kLedOn           {255, 255,  60,  60}; // Brighter red
const IColor kLedGlowOff      { 40, 255,  40,  40};
const IColor kLedGlowOn       {180, 255,  50,  50};

// Forward declarations for helper functions used below
void FillPatternCircle(IGraphics& g, float cx, float cy, float radius, const IPattern& pattern,
                       const IBlend* blend = nullptr);

// Subtle cool tint overlay — shifts white balance without washing out
static void ApplyCoolFilter(IGraphics& g, const IRECT& bounds) {
  // Dark cool blue-gray: same brightness as background (~140), shifted cool
  // Using IBlend weight for cleaner control over opacity
  g.FillRect(IColor(255, 90, 115, 160), bounds, &BLEND_05);
}
float Clamp01(float value) {
  return std::max(0.f, std::min(1.f, value));
}

template <typename T>
T ClampValue(T value, T minValue, T maxValue) {
  return std::max(minValue, std::min(maxValue, value));
}

float Lerp(float a, float b, float t) {
  return a + (b - a) * t;
}

float Snap4(float value) {
  return 4.f * std::round(value / 4.f);
}

float Snap8(float value) {
  return 8.f * std::round(value / 8.f);
}

float EaseSmooth(float t) {
  t = Clamp01(t);
  return t * t * (3.f - 2.f * t);
}

float EaseOutCubic(float t) {
  t = Clamp01(t);
  const float inv = 1.f - t;
  return 1.f - inv * inv * inv;
}

float EaseOutQuint(float t) {
  t = Clamp01(t);
  const float inv = 1.f - t;
  return 1.f - inv * inv * inv * inv * inv;
}

IColor WithAlpha(const IColor& color, int alpha) {
  return IColor(std::max(0, std::min(255, alpha)), color.R, color.G, color.B);
}

IColor BlendColor(const IColor& a, const IColor& b, float t) {
  t = Clamp01(t);
  auto mix = [t](int x, int y) {
    return static_cast<int>(std::lround(x + (y - x) * t));
  };
  return IColor(mix(a.A, b.A), mix(a.R, b.R), mix(a.G, b.G), mix(a.B, b.B));
}

float DegToRad(float degrees) {
  return degrees * kPi / 180.f;
}

void DrawRadialLine(IGraphics& g, const IColor& color, float cx, float cy, float angleDeg,
                    float innerR, float outerR, float thickness = 1.f,
                    const IBlend* blend = nullptr) {
  const float angle = DegToRad(angleDeg);
  const float x1 = cx + std::cos(angle) * innerR;
  const float y1 = cy + std::sin(angle) * innerR;
  const float x2 = cx + std::cos(angle) * outerR;
  const float y2 = cy + std::sin(angle) * outerR;
  g.DrawLine(color, x1, y1, x2, y2, blend, thickness);
}

void DrawBevel(IGraphics& g, const IRECT& bounds, const IColor& tl, const IColor& br, float thickness = 1.f) {
  g.DrawLine(tl, bounds.L, bounds.B, bounds.L, bounds.T, nullptr, thickness);
  g.DrawLine(tl, bounds.L, bounds.T, bounds.R, bounds.T, nullptr, thickness);
  g.DrawLine(br, bounds.R, bounds.T, bounds.R, bounds.B, nullptr, thickness);
  g.DrawLine(br, bounds.L, bounds.B, bounds.R, bounds.B, nullptr, thickness);
}

void FillPatternCircle(IGraphics& g, float cx, float cy, float radius, const IPattern& pattern,
                       const IBlend* blend) {
  g.PathCircle(cx, cy, radius);
  g.PathFill(pattern, IFillOptions(), blend);
}

void FillPatternEllipse(IGraphics& g, float cx, float cy, float rx, float ry, const IPattern& pattern,
                         const IBlend* blend = nullptr) {
  g.PathEllipse(cx, cy, rx, ry);
  g.PathFill(pattern, IFillOptions(), blend);
}

void DrawScrewHead(IGraphics& g, float cx, float cy, float radius, float rotationDeg) {
  // Screw shadow
  g.FillCircle(WithAlpha(kShellDeep, 65), cx + 0.8f, cy + 1.2f, radius + 1.2f);

  // Screw head base
  g.FillCircle(BlendColor(kShellDark, kShellMid, 0.25f), cx, cy, radius);

  // Screw head highlight (metallic gradient effect)
  FillPatternCircle(g, cx, cy, radius - 1.f,
                   IPattern::CreateLinearGradient(cx - radius, cy - radius, cx + radius, cy + radius,
                                                   {{kShellLight, 0.f},
                                                    {BlendColor(kShellMid, kShellDark, 0.3f), 0.6f},
                                                    {kShellDark, 1.f}}));

  // Screw slot (cross indentation)
  const float slotLen = radius * 0.55f;
  const float slotThick = 1.4f;
  const float rad = DegToRad(rotationDeg);
  const float c = std::cos(rad);
  const float s = std::sin(rad);

  // Horizontal slot (rotated)
  float h1x = cx - slotLen * c;
  float h1y = cy - slotLen * s;
  float h2x = cx + slotLen * c;
  float h2y = cy + slotLen * s;
  g.DrawLine(WithAlpha(kShellDeep, 110), h1x, h1y, h2x, h2y, nullptr, slotThick);

  // Vertical slot (rotated 90° from horizontal)
  float v1x = cx - slotLen * (-s);
  float v1y = cy - slotLen * c;
  float v2x = cx + slotLen * (-s);
  float v2y = cy + slotLen * c;
  g.DrawLine(WithAlpha(kShellDeep, 110), v1x, v1y, v2x, v2y, nullptr, slotThick);

  // Screw head highlight ring
  g.DrawCircle(WithAlpha(kShellLight, 60), cx - 0.3f, cy - 0.3f, radius - 0.8f, nullptr, 0.6f);
}

const char* GetActiveFontID() {
  return gActiveFontID;
}

void SetActiveFontID(const char* fontID) {
  gActiveFontID = (fontID && fontID[0]) ? fontID : kFallbackFontID;
}

bool ShouldUsePixelTextFallback() {
  return gUsePixelTextFallback;
}

bool ShouldUseUtilityFont() {
  return gUseUtilityFont;
}

bool ShouldUseTertiaryFont() {
  return gUseTertiaryFont;
}

void SetUsePixelTextFallback(bool usePixelTextFallback) {
  gUsePixelTextFallback = usePixelTextFallback;
}

void SetUseUtilityFont(bool useUtilityFont) {
  gUseUtilityFont = useUtilityFont;
}

void SetUseTertiaryFont(bool useTertiaryFont) {
  gUseTertiaryFont = useTertiaryFont;
}

IText MakeUIText(float size, const IColor& color, EAlign align, EVAlign valign) {
  return IText{size, color, GetActiveFontID(), align, valign};
}

IText MakeUtilityText(float size, const IColor& color, EAlign align, EVAlign valign) {
  const char* fontID = ShouldUseUtilityFont() ? kUtilityFontID : kFallbackFontID;
  return IText{size, color, fontID, align, valign};
}

IText MakeTertiaryText(float size, const IColor& color, EAlign align, EVAlign valign) {
  return IText{size, color, kTertiaryFontID, align, valign};
}

struct PixelGlyph {
  char ch;
  uint8_t rows[7];
  int advance;
};

constexpr PixelGlyph kFallbackPixelGlyph = {
  '?',
  {
    0b01110,
    0b10001,
    0b00010,
    0b00100,
    0b00100,
    0b00000,
    0b00100
  },
  6
};

constexpr PixelGlyph kPixelGlyphs[] = {
  {' ', {0, 0, 0, 0, 0, 0, 0}, 4},
  {'+', {0b00000, 0b00100, 0b00100, 0b11111, 0b00100, 0b00100, 0b00000}, 6},
  {'-', {0b00000, 0b00000, 0b00000, 0b11111, 0b00000, 0b00000, 0b00000}, 6},
  {'.', {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01100, 0b01100}, 3},
  {'%', {0b11001, 0b11010, 0b00100, 0b00100, 0b01011, 0b10011, 0b00000}, 6},
  {'0', {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110}, 6},
  {'1', {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, 6},
  {'2', {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}, 6},
  {'3', {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110}, 6},
  {'4', {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}, 6},
  {'5', {0b11111, 0b10000, 0b10000, 0b11110, 0b00001, 0b00001, 0b11110}, 6},
  {'6', {0b01110, 0b10000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}, 6},
  {'7', {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000}, 6},
  {'8', {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110}, 6},
  {'9', {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001, 0b01110}, 6},
  {'A', {0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}, 6},
  {'B', {0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110}, 6},
  {'C', {0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110}, 6},
  {'D', {0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110}, 6},
  {'E', {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111}, 6},
  {'F', {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000}, 6},
  {'G', {0b01110, 0b10001, 0b10000, 0b10111, 0b10001, 0b10001, 0b01110}, 6},
  {'H', {0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}, 6},
  {'I', {0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, 6},
  {'J', {0b00111, 0b00010, 0b00010, 0b00010, 0b10010, 0b10010, 0b01100}, 6},
  {'K', {0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001}, 6},
  {'L', {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111}, 6},
  {'M', {0b10001, 0b11011, 0b10101, 0b10101, 0b10001, 0b10001, 0b10001}, 6},
  {'N', {0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b10001}, 6},
  {'O', {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110}, 6},
  {'P', {0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000}, 6},
  {'Q', {0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101}, 6},
  {'R', {0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001}, 6},
  {'S', {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110}, 6},
  {'T', {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100}, 6},
  {'U', {0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110}, 6},
  {'V', {0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b00100}, 6},
  {'W', {0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010}, 6},
  {'X', {0b10001, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b10001}, 6},
  {'Y', {0b10001, 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100}, 6},
  {'Z', {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111}, 6}
};

char NormalizePixelChar(char c) {
  return (c >= 'a' && c <= 'z') ? static_cast<char>(c - ('a' - 'A')) : c;
}

const PixelGlyph& GetPixelGlyph(char c) {
  // Build a 128-entry lookup table once (thread-safe via C++11 static initialization)
  struct LUT {
    const PixelGlyph* entries[128] = {};
    LUT() {
      for (const auto& g : kPixelGlyphs) {
        const auto idx = static_cast<unsigned char>(NormalizePixelChar(g.ch));
        if (idx < 128) entries[idx] = &g;
      }
    }
  };
  static const LUT lut;
  const auto idx = static_cast<unsigned char>(NormalizePixelChar(c));
  if (idx < 128 && lut.entries[idx]) return *lut.entries[idx];
  return kFallbackPixelGlyph;
}

std::string TrimAscii(const char* text) {
  if (!text) {
    return {};
  }

  const unsigned char* begin = reinterpret_cast<const unsigned char*>(text);
  while (*begin && std::isspace(*begin)) {
    ++begin;
  }

  const unsigned char* end = begin + std::strlen(reinterpret_cast<const char*>(begin));
  while (end > begin && std::isspace(*(end - 1))) {
    --end;
  }

  return std::string(reinterpret_cast<const char*>(begin), reinterpret_cast<const char*>(end));
}

std::string NormalizeLocalizedNumber(const std::string& text) {
  std::string compact;
  compact.reserve(text.size());

  for (char c : text) {
    const unsigned char uc = static_cast<unsigned char>(c);

    if (std::isspace(uc) || c == '\'' || c == '_') {
      continue;
    }

    compact.push_back(c);
  }

  const std::size_t lastDot = compact.find_last_of('.');
  const std::size_t lastComma = compact.find_last_of(',');

  char decimalSep = '\0';
  std::size_t decimalIndex = std::string::npos;

  if (lastDot != std::string::npos || lastComma != std::string::npos) {
    if (lastDot == std::string::npos || (lastComma != std::string::npos && lastComma > lastDot)) {
      decimalSep = ',';
      decimalIndex = lastComma;
    } else {
      decimalSep = '.';
      decimalIndex = lastDot;
    }
  }

  std::string normalized;
  normalized.reserve(compact.size());

  for (std::size_t i = 0; i < compact.size(); ++i) {
    const char c = compact[i];

    if (c == '.' || c == ',') {
      if (decimalSep != '\0' && c == decimalSep && i == decimalIndex) {
        normalized.push_back('.');
      }

      continue;
    }

    normalized.push_back(c);
  }

  return normalized;
}

bool TryParseLocalizedDouble(const char* text, double& outValue) {
  const std::string trimmed = TrimAscii(text);
  if (trimmed.empty()) {
    return false;
  }

  const std::string normalized = NormalizeLocalizedNumber(trimmed);
  if (normalized.empty()) {
    return false;
  }

  errno = 0;
  char* end = nullptr;
  const double parsed = std::strtod(normalized.c_str(), &end);

  if (end == normalized.c_str() || (end && *end != '\0')) {
    return false;
  }

  if (!std::isfinite(parsed)) {
    return false;
  }

  outValue = parsed;
  return true;
}

bool TryParseClampedLocalizedDouble(const char* text, double minValue, double maxValue, double& outValue) {
  double parsed = 0.0;

  if (!TryParseLocalizedDouble(text, parsed)) {
    return false;
  }

  outValue = std::max(minValue, std::min(maxValue, parsed));
  return true;
}

float FitTextSizeToBounds(IGraphics& g, const char* fontID, const char* text,
                          float requestedSize, const IRECT& bounds, float minSize) {
  if (!text || !text[0] || requestedSize <= minSize || bounds.W() <= 1.f || bounds.H() <= 1.f) {
    return requestedSize;
  }

  IRECT measured;
  const IText probeText{requestedSize, IColor{255, 0, 0, 0}, fontID, EAlign::Center, EVAlign::Middle};
  g.MeasureText(probeText, text, measured);

  const float textWidth = std::max(1.f, measured.W());
  const float textHeight = std::max(1.f, measured.H());
  const float maxWidth = std::max(1.f, bounds.W() - 2.f);
  const float maxHeight = std::max(1.f, bounds.H() - 2.f);
  const float scale = std::min(1.f, std::min(maxWidth / textWidth, maxHeight / textHeight));

  return std::max(minSize, requestedSize * scale);
}

float MeasurePixelTextUnits(const char* text) {
  if (!text || !text[0]) {
    return 0.f;
  }

  float totalUnits = 0.f;
  for (const char* p = text; *p; ++p) {
    totalUnits += static_cast<float>(GetPixelGlyph(*p).advance);
  }

  return std::max(0.f, totalUnits - 1.f);
}

float GetPixelSizeForBounds(const char* text, float requestedSize, const IRECT& bounds) {
  const float textUnits = MeasurePixelTextUnits(text);
  float pixelSize = std::max(1.f, std::round(requestedSize / 7.f));
  pixelSize = std::min(pixelSize, std::max(1.f, std::floor(bounds.H() / 7.f)));

  if (textUnits > 0.f) {
    pixelSize = std::min(pixelSize, std::max(1.f, std::floor(bounds.W() / textUnits)));
  }

  return std::max(1.f, pixelSize);
}

// Accumulates rect subpaths for one glyph; caller must PathClear() before first glyph
// and PathFill() after the last to issue a single GPU draw per text run.
void AccumPixelGlyph(IGraphics& g, const PixelGlyph& glyph, float x, float y, float pixelSize) {
  for (int row = 0; row < 7; ++row) {
    const uint8_t rowBits = glyph.rows[row];
    for (int col = 0; col < 5; ++col) {
      if (!((rowBits >> (4 - col)) & 1)) continue;
      const float left = x + static_cast<float>(col) * pixelSize;
      const float top  = y + static_cast<float>(row) * pixelSize;
      g.PathRect(IRECT(left, top, left + pixelSize, top + pixelSize));
    }
  }
}

void DrawPixelText(IGraphics& g, float requestedSize, const IColor& color,
                   EAlign align, EVAlign valign, const char* text, const IRECT& bounds) {
  if (!text || !text[0]) {
    return;
  }

  const float pixelSize = GetPixelSizeForBounds(text, requestedSize, bounds);
  const float textWidth = MeasurePixelTextUnits(text) * pixelSize;
  const float textHeight = 7.f * pixelSize;

  float x = bounds.L;
  switch (align) {
    case EAlign::Center:
      x = bounds.MW() - textWidth * 0.5f;
      break;
    case EAlign::Far:
      x = bounds.R - textWidth;
      break;
    case EAlign::Near:
    default:
      x = bounds.L;
      break;
  }

  float y = bounds.T;
  switch (valign) {
    case EVAlign::Middle:
      y = bounds.MH() - textHeight * 0.5f;
      break;
    case EVAlign::Bottom:
      y = bounds.B - textHeight;
      break;
    case EVAlign::Top:
    default:
      y = bounds.T;
      break;
  }

  x = std::round(x);
  y = std::round(y);

  // Batch the entire text run into a single PathFill — one GPU draw call
  // regardless of how many pixels are set across all glyphs.
  g.PathClear();
  for (const char* p = text; *p; ++p) {
    const PixelGlyph& glyph = GetPixelGlyph(*p);
    AccumPixelGlyph(g, glyph, x, y, pixelSize);
    x += static_cast<float>(glyph.advance) * pixelSize;
  }
  g.PathFill(IPattern(color));
}

void DrawRetroText(IGraphics& g, float requestedSize, const IColor& color,
                   EAlign align, EVAlign valign, const char* text, const IRECT& bounds) {
  if (!text || !text[0]) {
    return;
  }

  if (ShouldUsePixelTextFallback()) {
    DrawPixelText(g, requestedSize, color, align, valign, text, bounds);
    return;
  }

  const float requestedTTFSize = requestedSize * kRetroTTFSizeScale;
  const float fittedTTFSize = FitTextSizeToBounds(g, GetActiveFontID(), text, requestedTTFSize, bounds, 7.f);
  g.DrawText(MakeUIText(fittedTTFSize, color, align, valign), text, bounds);
}

void DrawUtilityText(IGraphics& g, float size, const IColor& color,
                     EAlign align, EVAlign valign,
                     const char* text, const IRECT& bounds) {
  if (!text || !text[0]) {
    return;
  }

  const char* fontID = ShouldUseUtilityFont() ? kUtilityFontID : kFallbackFontID;
  const float fitted = FitTextSizeToBounds(g, fontID, text, size, bounds, 8.f);
  g.DrawText(IText{fitted, color, fontID, align, valign}, text, bounds);
}

void DrawTertiaryText(IGraphics& g, float size, const IColor& color,
                      EAlign align, EVAlign valign,
                      const char* text, const IRECT& bounds) {
  if (!text || !text[0]) {
    return;
  }

  const char* fontID = ShouldUseTertiaryFont() ? kTertiaryFontID : kFallbackFontID;
  const float fitted = FitTextSizeToBounds(g, fontID, text, size, bounds, 8.f);
  g.DrawText(IText{fitted, color, fontID, align, valign}, text, bounds);
}

IColor GetValidationAlertColor() {
  return BlendColor(kCoolOff, kShellDeep, 0.24f);
}

void DrawEmbossedBadgeText(IGraphics& g, float requestedSize, const char* text, const IRECT& bounds) {
  if (!text || !text[0]) {
    return;
  }

  const IColor badgeFace = BlendColor(kShellFace, kShellMid, 0.48f);
  const IColor badgeHighlight = BlendColor(kShellLight, kShellFace, 0.52f);
  const IColor badgeShadow = BlendColor(kShellDark, kShellDeep, 0.44f);

  DrawRetroText(g,
                requestedSize,
                badgeHighlight,
                EAlign::Center,
                EVAlign::Middle,
                text,
                bounds.GetTranslated(-0.9f, -0.9f));
  DrawRetroText(g,
                requestedSize,
                badgeShadow,
                EAlign::Center,
                EVAlign::Middle,
                text,
                bounds.GetTranslated(1.0f, 1.0f));
  DrawRetroText(g, requestedSize, badgeFace, EAlign::Center, EVAlign::Middle, text, bounds);
}

void DrawBrandPlate(IGraphics& g, const IRECT& bounds) {
  const float radius = 5.f;
  const IColor plateFace = BlendColor(kShellFace, kFieldFace, 0.28f);
  const IColor plateMid = BlendColor(kShellLight, kShellFace, 0.26f);
  const IColor plateLight = BlendColor(kShellLight, plateFace, 0.14f);
  const IColor plateShadow = BlendColor(kShellDark, kShellDeep, 0.30f);

  g.FillRoundRect(WithAlpha(plateShadow, 28), bounds.GetTranslated(0.f, 1.3f), radius);
  g.FillRoundRect(WithAlpha(plateFace, 154), bounds, radius);
  g.FillRoundRect(WithAlpha(plateMid, 78), bounds.GetPadded(-1.f), radius - 1.f);
  g.DrawLine(WithAlpha(plateLight, 176), bounds.L + 5.f, bounds.T + 1.f,
             bounds.R - 5.f, bounds.T + 1.f, nullptr, 1.f);
  g.DrawLine(WithAlpha(BlendColor(plateLight, plateFace, 0.38f), 104), bounds.L + 5.f, bounds.T + 2.2f,
             bounds.R - 5.f, bounds.T + 2.2f, nullptr, 1.f);
  g.DrawLine(WithAlpha(plateShadow, 154), bounds.L + 5.f, bounds.B - 1.f,
             bounds.R - 5.f, bounds.B - 1.f, nullptr, 1.f);
  g.DrawRoundRect(WithAlpha(BlendColor(plateShadow, kShellDeep, 0.52f), 116), bounds, radius, nullptr, 0.9f);
}

void DrawMonitorGlassOverlay(IGraphics& g, const IRECT& bounds, bool powerOn) {
  const IRECT glass = bounds.GetPadded(-6.f);

  const IColor scanline = powerOn
    ? WithAlpha(BlendColor(kCoolGlow, kShellLight, 0.26f), 18)
    : WithAlpha(BlendColor(kShellDark, kShellDeep, 0.42f), 14);
  const float scanStep = powerOn ? 3.4f : 4.2f;

  for (float y = glass.T + 1.f; y < glass.B; y += scanStep) {
    g.DrawLine(scanline, glass.L + 1.f, y, glass.R - 1.f, y, nullptr, 1.f);
  }

  const IColor rim = powerOn
    ? WithAlpha(BlendColor(kShellDeep, kCoolGlow, 0.20f), 52)
    : WithAlpha(kShellDeep, 64);
  g.DrawRoundRect(rim, glass, 10.f, nullptr, 1.4f);

  const IRECT sheen(glass.L + glass.W() * 0.07f,
                    glass.T + glass.H() * 0.08f,
                    glass.R - glass.W() * 0.43f,
                    glass.T + glass.H() * 0.37f);
  const IColor sheenColor = powerOn
    ? WithAlpha(BlendColor(kShellLight, kCoolGlow, 0.35f), 30)
    : WithAlpha(kShellLight, 18);
  g.FillRoundRect(sheenColor, sheen, 7.f);
}

void DrawEmbossedSVG(IGraphics& g, const ISVG& face, const ISVG& highlight,
                     const ISVG& shadow, const IRECT& bounds) {
  const ISVG* primary = nullptr;

  if (face.IsValid()) {
    primary = &face;
  } else if (shadow.IsValid()) {
    primary = &shadow;
  } else if (highlight.IsValid()) {
    primary = &highlight;
  }

  if (!primary) {
    return;
  }

  const float maxWidth = std::max(0.f, bounds.W() - 10.f);
  const float maxHeight = std::max(0.f, bounds.H() - 4.f);
  const float svgAspect = (primary->W() > 0.f && primary->H() > 0.f) ? (primary->W() / primary->H()) : 1.f;

  float logoWidth = maxWidth;
  float logoHeight = logoWidth / svgAspect;

  if (logoHeight > maxHeight) {
    logoHeight = maxHeight;
    logoWidth = logoHeight * svgAspect;
  }

  const IRECT logoBounds(bounds.MW() - logoWidth * 0.5f,
                         bounds.MH() - logoHeight * 0.5f,
                         bounds.MW() + logoWidth * 0.5f,
                         bounds.MH() + logoHeight * 0.5f);

  if (highlight.IsValid()) {
    g.DrawSVG(highlight, logoBounds.GetTranslated(-0.95f, -0.95f));
  }

  if (shadow.IsValid()) {
    g.DrawSVG(shadow, logoBounds.GetTranslated(1.0f, 1.0f));
  }

  g.DrawSVG(*primary, logoBounds);
}

#ifdef OS_WIN
bool LoadEmbeddedDOSFont(IGraphics* g, const char* fontID) {
  if (!g || !g->GetWinModuleHandle()) {
    return false;
  }

  const char* resourceIDs[] = {
    GOOD_OLD_DOS_RES,
    "GoodOldDOS",
    "GOODOLDDOS"
  };

  for (const char* resourceID : resourceIDs) {
    WDL_String resolvedResourceID;
    const EResourceLocation resourceLocation = LocateResource(resourceID, "ttf", resolvedResourceID,
                                                              g->GetBundleID(), g->GetWinModuleHandle(), nullptr);

    if (resourceLocation != EResourceLocation::kWinBinary) {
      continue;
    }

    int dataSize = 0;
    void* fontData = const_cast<void*>(LoadWinResource(resolvedResourceID.Get(), "ttf", dataSize, g->GetWinModuleHandle()));

    if (fontData && dataSize > 0 && g->LoadFont(fontID, fontData, dataSize)) {
      return true;
    }
  }

  return false;
}

bool LoadBundledFont(IGraphics* g, const char* fontID, const char* fileName) {
  if (!g || !g->GetWinModuleHandle() || !fileName || !fileName[0]) {
    return false;
  }

  char modulePath[MAX_PATH] = {};
  const DWORD modulePathLength = GetModuleFileNameA(static_cast<HMODULE>(g->GetWinModuleHandle()), modulePath, MAX_PATH);
  if (!modulePathLength || modulePathLength >= MAX_PATH) {
    return false;
  }

  char* lastSlash = std::strrchr(modulePath, '\\');
  if (!lastSlash) {
    return false;
  }

  *lastSlash = '\0';

  char candidatePath[MAX_PATH] = {};
  std::snprintf(candidatePath, sizeof(candidatePath), "%s\\..\\Resources\\%s", modulePath, fileName);

  char fullPath[MAX_PATH] = {};
  const DWORD fullPathLength = GetFullPathNameA(candidatePath, MAX_PATH, fullPath, nullptr);
  if (!fullPathLength || fullPathLength >= MAX_PATH) {
    return false;
  }

  return g->LoadFont(fontID, fullPath);
}
#endif

bool LoadFallbackUIFont(IGraphics* g) {
  if (!g) {
    return false;
  }

  if (g->LoadFont(kFallbackFontID, "Consolas", ETextStyle::Normal)) {
    return true;
  }

  if (g->LoadFont(kFallbackFontID, "Lucida Console", ETextStyle::Normal)) {
    return true;
  }


  if (g->LoadFont(kFallbackFontID, "Courier New", ETextStyle::Normal)) {
    return true;
  }

  return g->LoadFont(kFallbackFontID, "Arial", ETextStyle::Normal);
}

bool LoadUtilityUIFont(IGraphics* g) {
  if (!g) {
    return false;
  }

#ifdef OS_WIN
  if (LoadBundledFont(g, kUtilityFontID, UTILITY_UI_FN)) {
    return true;
  }

  if (g->LoadFont(kUtilityFontID, UTILITY_UI_FN)) {
    return true;
  }

  if (g->LoadFont(kUtilityFontID, "Segoe UI", ETextStyle::Normal)) {
    return true;
  }
#endif

  return g->LoadFont(kUtilityFontID, "Tahoma", ETextStyle::Normal);
}

bool LoadTertiaryUIFont(IGraphics* g) {
  if (!g) {
    return false;
  }

#ifdef OS_WIN
  if (LoadBundledFont(g, kTertiaryFontID, TERTIARY_UI_FN)) {
    return true;
  }

  if (g->LoadFont(kTertiaryFontID, TERTIARY_UI_FN)) {
    return true;
  }

  if (g->LoadFont(kTertiaryFontID, "Trebuchet MS", ETextStyle::Normal)) {
    return true;
  }
#endif

  return g->LoadFont(kTertiaryFontID, "Verdana", ETextStyle::Normal);
}



bool TryLoadDOSFont(IGraphics* g, const char* fontID) {
  bool fontLoaded = false;

#ifdef OS_WIN
  fontLoaded = LoadEmbeddedDOSFont(g, fontID);

  if (!fontLoaded) {
    fontLoaded = LoadBundledFont(g, fontID, GOOD_OLD_DOS_FN);
  }
#endif

  if (!fontLoaded) {
    fontLoaded = g->LoadFont(fontID, GOOD_OLD_DOS_RES);
  }

  if (!fontLoaded) {
    fontLoaded = g->LoadFont(fontID, GOOD_OLD_DOS_FN);
  }

  return fontLoaded;
}

const char* LoadActiveDOSFont(IGraphics* g) {
  return TryLoadDOSFont(g, kDOSFontID) ? kDOSFontID : nullptr;
}

void FillClassicPanel(IGraphics& g, const IRECT& bounds, const IColor& fill,
                      bool sunken = false, float thickness = 1.f, float radius = 0.f) {
  if (radius > 0.0f) {
    g.FillRoundRect(fill, bounds, radius);
    g.DrawRoundRect(sunken ? kShellDark : kShellLight, bounds.GetTranslated(-0.5f, -0.5f), radius, nullptr, thickness);
    g.DrawRoundRect(sunken ? kShellLight : kShellDark, bounds.GetTranslated(0.5f, 0.5f), radius, nullptr, thickness);

    const IRECT inner = bounds.GetPadded(-1.f);
    if (inner.W() > 2.f && inner.H() > 2.f) {
      g.DrawRoundRect(sunken ? kFieldInset : kShellFace, inner.GetTranslated(-0.5f, -0.5f), radius, nullptr, thickness);
      g.DrawRoundRect(sunken ? kShellLight : kFieldInset, inner.GetTranslated(0.5f, 0.5f), radius, nullptr, thickness);
    }
  } else {
    g.FillRect(fill, bounds);
    DrawBevel(g, bounds,
              sunken ? kShellDark : kShellLight,
              sunken ? kShellLight : kShellDark,
              thickness);

    const IRECT inner = bounds.GetPadded(-1.f);
    if (inner.W() > 2.f && inner.H() > 2.f) {
      DrawBevel(g, inner,
                sunken ? kFieldInset : kShellFace,
                sunken ? kShellLight : kFieldInset,
                thickness);
    }
  }
}

void DrawDustTexture(IGraphics& g, const IRECT& bounds) {
  struct DustSpeck {
    float x;
    float y;
    float radius;
    int alpha;
  };

  constexpr DustSpeck specks[] = {
    {0.04f, 0.10f, 1.8f, 54}, {0.09f, 0.18f, 1.2f, 44}, {0.14f, 0.73f, 1.5f, 38},
    {0.21f, 0.84f, 1.1f, 32}, {0.28f, 0.24f, 1.6f, 28}, {0.37f, 0.66f, 1.3f, 24},
    {0.54f, 0.30f, 1.5f, 20}, {0.66f, 0.80f, 1.0f, 28}, {0.74f, 0.17f, 1.8f, 34},
    {0.86f, 0.73f, 1.4f, 42}, {0.92f, 0.24f, 1.0f, 34}, {0.96f, 0.13f, 2.0f, 44}
  };

  for (const auto& speck : specks) {
    g.FillCircle(IColor{speck.alpha, 232, 225, 214},
                 bounds.L + bounds.W() * speck.x,
                 bounds.T + bounds.H() * speck.y,
                 speck.radius);
  }

  const IRECT smudgeA(bounds.L + bounds.W() * 0.03f, bounds.T + bounds.H() * 0.07f,
                      bounds.L + bounds.W() * 0.18f, bounds.T + bounds.H() * 0.14f);
  const IRECT smudgeB(bounds.L + bounds.W() * 0.78f, bounds.T + bounds.H() * 0.10f,
                      bounds.L + bounds.W() * 0.96f, bounds.T + bounds.H() * 0.17f);
  const IRECT smudgeC(bounds.L + bounds.W() * 0.05f, bounds.T + bounds.H() * 0.77f,
                      bounds.L + bounds.W() * 0.24f, bounds.T + bounds.H() * 0.86f);
  const IRECT smudgeD(bounds.L + bounds.W() * 0.80f, bounds.T + bounds.H() * 0.73f,
                      bounds.L + bounds.W() * 0.97f, bounds.T + bounds.H() * 0.83f);
  g.FillRoundRect(WithAlpha(kShellDeep, 16), smudgeA, smudgeA.H() * 0.5f);
  g.FillRoundRect(WithAlpha(kShellDeep, 14), smudgeB, smudgeB.H() * 0.5f);
  g.FillRoundRect(WithAlpha(kShellDeep, 12), smudgeC, smudgeC.H() * 0.5f);
  g.FillRoundRect(WithAlpha(kShellDeep, 16), smudgeD, smudgeD.H() * 0.5f);
}

void DrawRaisedButton(IGraphics& g, const IRECT& bounds, const IColor& faceColor, float cornerRadius = 4.f) {
  // Shadow underneath
  const IRECT shadow = bounds.GetTranslated(2.f, 2.f);
  g.FillRoundRect(WithAlpha(kShellDeep, 35), shadow, cornerRadius);
  // Face
  FillClassicPanel(g, bounds, faceColor, true, 1.5f, cornerRadius);
  // Bevel highlight (top/left)
  g.DrawLine(WithAlpha(kShellLight, 90), bounds.L + 2.f, bounds.T + 1.f, bounds.R - 2.f, bounds.T + 1.f, nullptr, 1.f);
  g.DrawLine(WithAlpha(kShellLight, 70), bounds.L + 1.f, bounds.T + 2.f, bounds.L + 1.f, bounds.B - 2.f, nullptr, 1.f);
  // Bevel shadow (bottom/right)
  g.DrawLine(WithAlpha(kShellDeep, 60), bounds.L + 2.f, bounds.B - 1.f, bounds.R - 2.f, bounds.B - 1.f, nullptr, 1.f);
  g.DrawLine(WithAlpha(kShellDeep, 50), bounds.R - 1.f, bounds.T + 2.f, bounds.R - 1.f, bounds.B - 2.f, nullptr, 1.f);
  // Inner gradient depth
  const IRECT inner = bounds.GetPadded(-2.f);
  FillPatternEllipse(g, inner.MW(), inner.MH(), inner.W() * 0.5f, inner.H() * 0.5f,
    IPattern::CreateLinearGradient(inner.L, inner.T, inner.L, inner.B,
      {{WithAlpha(kShellLight, 20), 0.f}, {WithAlpha(kShellMid, 15), 1.f}}));
}

void DrawCornerPatina(IGraphics& g, const IRECT& bounds) {
  constexpr float radius = 34.f;
  g.FillCircle(WithAlpha(kShellLight, 26), bounds.L + 22.f, bounds.T + 18.f, radius);
  g.FillCircle(WithAlpha(kShellDeep, 18), bounds.R - 22.f, bounds.T + 18.f, radius);
  g.FillCircle(WithAlpha(kShellDeep, 15), bounds.L + 22.f, bounds.B - 20.f, radius + 4.f);
  g.FillCircle(WithAlpha(kShellLight, 18), bounds.R - 24.f, bounds.B - 18.f, radius + 2.f);
}

void FormatPercentDisplay(double normalizedValue, WDL_String& str) {
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%0.0f", normalizedValue * 100.0);
  str.Set(buffer);
}

void FormatBpmDisplay(const IParam* param, double normalizedValue, WDL_String& str) {
  const double bpm = param ? param->FromNormalized(normalizedValue) : normalizedValue;
  const double clampedBpm = std::max(20.0, std::min(300.0, bpm));
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%0.1f", clampedBpm);
  str.Set(buffer);
}

const char* GetKnobStateLabel(int paramIdx, float normalizedValue) {
  const float t = Clamp01(normalizedValue);

  switch (paramIdx) {
    case kParamChaos:
      if (t < 0.20f) {
        return "TAME";
      }
      if (t < 0.45f) {
        return "SHIFT";
      }
      if (t < 0.70f) {
        return "FRACTURE";
      }
      return "MELT";
    case kParamLoFi:
      if (t < 0.20f) {
        return "CLEAN";
      }
      if (t < 0.45f) {
        return "WARM";
      }
      if (t < 0.70f) {
        return "GRAIN";
      }
      return "CRUSH";
    default:
      return "LEVEL";
  }
}

void FormatKnobReadout(int paramIdx, double normalizedValue, WDL_String& str) {
  const float value = Clamp01(static_cast<float>(normalizedValue));
  const float percentValue = std::round(value * 100.f);
  char buffer[48];
  std::snprintf(buffer, sizeof(buffer), "%s %0.0f", GetKnobStateLabel(paramIdx, value), percentValue);
  str.Set(buffer);
}

// Drawn on top of the full UI when the effect is bypassed,
// simulating a monitor power-off dim so state is unambiguous.
class BypassOverlayControl final : public IControl {
public:
  BypassOverlayControl(const IRECT& bounds, int powerParamIdx)
    : IControl(bounds, powerParamIdx) {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override {
    if (GetValue() < 0.5) {
      g.FillRect(WithAlpha(kShellDeep, 112), mRECT);
      const IRECT centerBand(mRECT.L + mRECT.W() * 0.12f,
                             mRECT.MH() - 22.f,
                             mRECT.R - mRECT.W() * 0.12f,
                             mRECT.MH() + 22.f);
      g.FillRoundRect(WithAlpha(kShellDeep, 68), centerBand, 8.f);
      DrawUtilityText(g,
                      16.f,
                      WithAlpha(kCoolOn, 178),
                      EAlign::Center,
                      EVAlign::Middle,
                      "BYPASSED",
                      centerBand.GetPadded(-8.f));
      g.DrawRect(WithAlpha(kShellDark, 124), mRECT);
    }
  }
};

class MonitorBezelControl final : public IControl {
public:
  MonitorBezelControl(const IRECT& bounds, int powerParamIdx,
                      const ISVG& backgroundOn, const ISVG& backgroundOff)
    : IControl(bounds, powerParamIdx)
    , mBackgroundOn(backgroundOn)
    , mBackgroundOff(backgroundOff) {
    mIgnoreMouse = true;
  }

  void OnInit() override {
    IControl::OnInit();
    mLastPowerOn = GetValue() > 0.5;
    mHasPowerState = true;
    mPulsePowerOn = mLastPowerOn;
    mPowerPulse = 0.f;
    mPulseProgress = 1.f;
  }

  void SetValueFromDelegate(double value, int valIdx = 0) override {
    IControl::SetValueFromDelegate(value, valIdx);
    const bool powerOn = GetValue() > 0.5;

    if (!mHasPowerState) {
      mHasPowerState = true;
      mLastPowerOn = powerOn;
      return;
    }

    if (powerOn != mLastPowerOn) {
      StartPowerPulse(powerOn);
      mLastPowerOn = powerOn;
    }
  }

  void Draw(IGraphics& g) override {
    const bool powerOn = GetValue() > 0.5;
    const bool useOnLayer = powerOn || !mBackgroundOff.IsValid();
    const ISVG& activeBg = useOnLayer
      ? (mBackgroundOn.IsValid() ? mBackgroundOn : mBackgroundOff)
      : (mBackgroundOff.IsValid() ? mBackgroundOff : mBackgroundOn);

    if (activeBg.IsValid()) {
      ILayerPtr& targetLayer = useOnLayer ? mOnLayer : mOffLayer;

      if (!g.CheckLayer(targetLayer)) {
        g.StartLayer(this, mRECT);
        g.DrawSVG(activeBg, mRECT);

        DrawMonitorGlassOverlay(g, mRECT, powerOn);
        // Overlay dust/patina on top of SVG so it's visible in production
        DrawCornerPatina(g, mRECT);
        DrawDustTexture(g, mRECT);
        targetLayer = g.EndLayer();
      }

      g.DrawLayer(targetLayer);
      DrawPowerPulseOverlay(g);
      return;
    }

    g.FillRect(kShellBg, mRECT);

    DrawCornerPatina(g, mRECT);
    DrawDustTexture(g, mRECT);
    DrawMonitorGlassOverlay(g, mRECT, powerOn);
    DrawPowerPulseOverlay(g);
  }

  void OnEndAnimation() override {
    mPowerPulse = 0.f;
    mPulseProgress = 1.f;
    IControl::OnEndAnimation();
  }

private:
  void StartPowerPulse(bool poweringOn) {
    mPulsePowerOn = poweringOn;
    mPowerPulse = 1.f;
    mPulseProgress = 0.f;
    const int durationMs = poweringOn ? 460 : 240;

    SetAnimation([](IControl* control) {
      auto* self = control->As<MonitorBezelControl>();
      const float t = Clamp01(static_cast<float>(self->GetAnimationProgress()));
      self->mPulseProgress = t;
      if (self->mPulsePowerOn) {
        self->mPowerPulse = std::sin(t * kPi);
      } else {
        self->mPowerPulse = 1.f - EaseOutCubic(t);
      }
      self->SetDirty(false);
    }, durationMs);
  }

  void DrawPowerPulseOverlay(IGraphics& g) {
    if (mPowerPulse <= 0.001f) {
      return;
    }

    const IRECT glass = mRECT.GetPadded(-8.f);
    const int bloomAlpha = static_cast<int>(std::lround(54.f * Clamp01(mPowerPulse)));
    g.FillRoundRect(WithAlpha(kCoolGlow, bloomAlpha), glass.GetPadded(-2.f), 10.f);

    if (mPulsePowerOn) {
      const float sweepT = EaseOutCubic(mPulseProgress);
      const float sweepY = Lerp(glass.T + 2.f, glass.B - 2.f, sweepT);
      const IRECT sweepBand(glass.L + 4.f, sweepY - 1.2f, glass.R - 4.f, sweepY + 1.2f);
      const int sweepAlpha = static_cast<int>(std::lround(130.f * mPowerPulse));
      g.FillRoundRect(WithAlpha(kCoolOn, sweepAlpha), sweepBand, 1.2f);
    } else {
      const float collapse = 1.f - EaseOutCubic(mPulseProgress);
      const float halfHeight = std::max(0.75f, glass.H() * 0.26f * collapse);
      const IRECT collapseBand(glass.L + 5.f, glass.MH() - halfHeight,
                               glass.R - 5.f, glass.MH() + halfHeight);
      const int collapseAlpha = static_cast<int>(std::lround(122.f * mPowerPulse));
      g.FillRoundRect(WithAlpha(kCoolOn, collapseAlpha), collapseBand, 1.6f);
      const int darkAlpha = static_cast<int>(std::lround(42.f * Clamp01(mPulseProgress)));
      g.FillRoundRect(WithAlpha(kShellDeep, darkAlpha), glass, 10.f);
    }
  }

  ISVG mBackgroundOn;
  ISVG mBackgroundOff;
  ILayerPtr mOnLayer;
  ILayerPtr mOffLayer;
  float mPowerPulse = 0.f;
  float mPulseProgress = 1.f;
  bool mPulsePowerOn = true;
  bool mHasPowerState = false;
  bool mLastPowerOn = true;
};

// Normalized unit-circle coordinates for the three vertices of each knob tooth.
// Precomputed once so Draw() needs only 2 trig calls (cos/sin of rotationRad)
// instead of 24*3 = 72 trig calls per knob per frame.
struct TeethBaseCoords { float cx1, cy1, cx2, cy2, cx3, cy3; };

const TeethBaseCoords* GetTeethBase() {
  struct Init {
    TeethBaseCoords teeth[24];
    Init() {
      constexpr int   N    = 24;
      constexpr float kStep = (2.f * kPi) / static_cast<float>(N);
      constexpr float kHalf = kStep * 0.35f;
      for (int i = 0; i < N; ++i) {
        const float base = kStep * static_cast<float>(i) - (kPi * 0.5f);
        teeth[i] = {
          std::cos(base - kHalf), std::sin(base - kHalf),
          std::cos(base),         std::sin(base),
          std::cos(base + kHalf), std::sin(base + kHalf)
        };
      }
    }
  };
  static const Init kInit;
  return kInit.teeth;
}

class SpeakerKnobControl final : public IKnobControlBase {
public:
  SpeakerKnobControl(const IRECT& bounds, int paramIdx, const char* label)
    : IKnobControlBase(bounds, paramIdx)
    , mLabel((label && label[0]) ? label : "CTRL") {
    mDblAsSingleClick = true;
    mDirtSeed = static_cast<unsigned int>((paramIdx * 7919 + 12345) ^ 0x9E3779B9);
  }

  void SetValueFromDelegate(double value, int valIdx = 0) override {
    const float previous = static_cast<float>(GetValue());
    IControl::SetValueFromDelegate(value, valIdx);
    const float current = static_cast<float>(GetValue());
    const float delta = std::abs(current - previous);
    if (delta > 0.001f) {
      TriggerReadoutPulse(delta);
    }
  }

  void OnResize() override {
    const float footerH = 48.f;
    const IRECT knobArea(mRECT.L + 6.f, mRECT.T + 6.f, mRECT.R - 6.f, mRECT.B - footerH);
    const float socketSize = std::min(knobArea.W(), knobArea.H());
    mSocketBounds = knobArea.GetCentredInside(socketSize, socketSize);
    // Label sits in a taller band above value; value gets generous room at bottom
    mLabelBounds = IRECT(mRECT.L, mRECT.B - footerH + 4.f, mRECT.R, mRECT.B - footerH + 26.f);
    mValueBounds = IRECT(mRECT.L, mRECT.B - 28.f, mRECT.R, mRECT.B - 2.f);
    SetTargetRECT(mSocketBounds.GetPadded(6.f));
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override {
    IKnobControlBase::OnMouseOver(x, y, mod);
    if (GetUI()) {
      GetUI()->SetMouseCursor(ECursor::HAND);
    }
    mHoverTarget = 1.f;
    mHoverFrom = mHoverAmt;
    SetAnimation([](IControl* c) {
      auto* self = c->As<SpeakerKnobControl>();
      const float t = EaseOutQuint(static_cast<float>(self->GetAnimationProgress()));
      self->mHoverAmt = self->mHoverFrom + (1.f - self->mHoverFrom) * t;
      self->SetDirty(false);
    }, 140);
  }

  void OnMouseOut() override {
    IKnobControlBase::OnMouseOut();
    if (GetUI()) {
      GetUI()->SetMouseCursor();
    }
    mHoverTarget = 0.f;
    mHoverFrom = mHoverAmt;
    SetAnimation([](IControl* c) {
      auto* self = c->As<SpeakerKnobControl>();
      const float t = EaseOutQuint(static_cast<float>(self->GetAnimationProgress()));
      self->mHoverAmt = self->mHoverFrom * (1.f - t);
      self->SetDirty(false);
    }, 80);
  }

  void OnEndAnimation() override {
    mHoverAmt = mHoverTarget;
    IKnobControlBase::OnEndAnimation();
  }

  void Draw(IGraphics& g) override {
    UpdateReadoutPulse();

    const float value = static_cast<float>(GetValue());
    const float cx = mSocketBounds.MW();
    const float cy = mSocketBounds.MH();
    const float socketR = std::min(mSocketBounds.W(), mSocketBounds.H()) * 0.5f - 2.f;
    const bool hover = GetMouseIsOver();
    const bool captured = GetUI() && GetUI()->ControlIsCaptured(this);
    const bool active = (hover || captured || mMouseDown) && !IsDisabled();
    const float pressOffset = mMouseDown ? 0.9f : (active ? 0.26f : 0.06f);
    const float knobCy = cy + pressOffset;
    const float outerR = socketR * 0.84f;
    const float innerR = outerR * 0.88f;
    const float bodyR = outerR * 0.80f;
    constexpr int numTeeth = 24;
    const float lineAngle = Lerp(135.f, 405.f, value);
    const float rotationDeg = lineAngle - 270.f;
    const float rotationRad = DegToRad(rotationDeg);

    const IColor knobFill = BlendColor(kKnobTop, kShellLight, active ? 0.28f : 0.19f);
    static const IColor grooveDark = BlendColor(kKnobPointerDark, kShellDeep, 0.24f);

    const float glowAmt = (mMouseDown || captured) ? 1.f : (mHoverAmt * 0.62f);
    if (glowAmt > 0.01f) {
      g.FillCircle(WithAlpha(kCoolGlow, static_cast<int>(70.f * glowAmt)), cx, knobCy, outerR + 2.8f);
    }

    // Apply "dirty old hardware" weathering to the socket area — varied per-knob
    const float cosD = std::cos(rotationRad);
    const float sinD = std::sin(rotationRad);
    auto dirtRand = [&](unsigned int n)->float {
      unsigned int s = mDirtSeed + n * 1664525u + 1013904223u;
      s = 1664525u * s + 1013904223u;
      return (s & 0x00FFFFFFu) / 16777216.0f;
    };
    // Socket aging — number of spots varies per knob for unique character
    const int socketSpots = 1 + static_cast<int>(dirtRand(0) * 2.5f); // 1 or 2 spots
    for (int spot = 0; spot < socketSpots; ++spot) {
      float dx = -outerR * 0.4f + (dirtRand(1 + spot * 4) - 0.5f) * outerR * 0.35f;
      float dy = outerR * 0.5f + (dirtRand(2 + spot * 4) - 0.5f) * outerR * 0.35f;
      const float x = cx + (dx * cosD - dy * sinD);
      const float y = cy + (dx * sinD + dy * cosD);
      const float r = outerR * (0.3f + 0.25f * dirtRand(3 + spot * 4));
      const int a = static_cast<int>(3 + 5 * dirtRand(4 + spot * 4));
      g.FillCircle(WithAlpha(kShellDeep, a), x, y, r);
    }

    // Travel arc — shows consumed range at a glance without decoding numbers
    if (value > 0.002f) {
      const float arcStart = 225.f;
      const float arcEnd = lineAngle + 90.f;
      const float arcR = outerR + 4.5f;
      const int arcAlpha = active
        ? 148
        : static_cast<int>(std::lround(80.f + 52.f * Clamp01(mReadoutPulse)));
      const IColor arcColor = active
        ? WithAlpha(kCoolGlow, arcAlpha)
        : WithAlpha(kCoolOn, arcAlpha);
      g.DrawArc(arcColor, cx, knobCy, arcR, arcStart, arcEnd, nullptr, 1.8f);
    }
    // Faint full-range track so the arc has a reference groove
    g.DrawArc(WithAlpha(IColor(65, 250, 245, 230), 65), cx, knobCy, outerR + 4.5f, 225.f, 495.f, nullptr, 3.0f);

    if (mReadoutPulse > 0.01f && value > 0.002f) {
      const float arcEndRad = DegToRad(Lerp(135.f, 405.f, value));
      const float sparkR = outerR + 4.5f;
      const float sparkX = cx + std::cos(arcEndRad) * sparkR;
      const float sparkY = knobCy + std::sin(arcEndRad) * sparkR;
      const int sparkAlpha = static_cast<int>(std::lround(98.f * Clamp01(mReadoutPulse)));
      g.FillCircle(WithAlpha(kCoolOn, sparkAlpha), sparkX, sparkY, 1.3f + 1.0f * mReadoutPulse);
    }

    // Drop shadow under the knob for depth
    g.FillCircle(WithAlpha(kShellDeep, 14), cx + 2.0f, cy + 3.0f, socketR + 5.f);
    g.FillCircle(WithAlpha(kShellDeep, 8), cx + 1.0f, cy + 2.0f, socketR + 8.f);

    g.FillCircle(knobFill, cx, knobCy, innerR);

    // 2 trig calls to set up rotation transform; precomputed unit-circle coords handle the rest
    const float cosR = std::cos(rotationRad);
    const float sinR = std::sin(rotationRad);
    const TeethBaseCoords* const teeth = GetTeethBase();
    for (int tooth = 0; tooth < numTeeth; ++tooth) {
      const TeethBaseCoords& t = teeth[tooth];
      float toothX[3] = {
        cx + (t.cx1 * cosR - t.cy1 * sinR) * innerR,
        cx + (t.cx2 * cosR - t.cy2 * sinR) * outerR,
        cx + (t.cx3 * cosR - t.cy3 * sinR) * innerR
      };
      float toothY[3] = {
        knobCy + (t.cx1 * sinR + t.cy1 * cosR) * innerR,
        knobCy + (t.cx2 * sinR + t.cy2 * cosR) * outerR,
        knobCy + (t.cx3 * sinR + t.cy3 * cosR) * innerR
      };
      g.FillConvexPolygon(knobFill, toothX, toothY, 3);

      // Chamfer shadow on trailing edge (2 -> 1)
      const IColor toothShadow = BlendColor(kKnobPointerDark, kShellDeep, 0.5f);
      g.DrawLine(WithAlpha(toothShadow, 50),
                 toothX[1], toothY[1], toothX[0], toothY[0],
                 nullptr, 0.9f);
      // Chamfer highlight on leading edge (2 -> 3)
      const IColor toothHighlight = BlendColor(kShellLight, kKnobTop, 0.4f);
      g.DrawLine(WithAlpha(toothHighlight, 55),
                 toothX[1], toothY[1], toothX[2], toothY[2],
                 nullptr, 0.6f);
    }

    g.FillCircle(BlendColor(knobFill, kShellLight, active ? 0.12f : 0.06f), cx, knobCy, bodyR);

    // [P2] Delight: Subtile anisotropic "spun metal" highlight
    // Adds complexity to match the high-end power button
    {
      const float hlA = 0.44f;
      IPattern hl = IPattern::CreateSweepGradient(cx, knobCy, {
        {WithAlpha(kShellLight, 0), 0.0f},
        {WithAlpha(kShellLight, static_cast<int>(35 * hlA)), 0.12f},
        {WithAlpha(kShellLight, 0), 0.25f},
        {WithAlpha(kShellLight, static_cast<int>(45 * hlA)), 0.5f},
        {WithAlpha(kShellLight, 0), 0.75f},
        {WithAlpha(kShellLight, static_cast<int>(35 * hlA)), 0.88f},
        {WithAlpha(kShellLight, 0), 1.0f}
      }, lineAngle - 45.f);
      g.PathCircle(cx, knobCy, bodyR);
      g.PathFill(hl);
    }

    // Apply "dirty old hardware" weathering to the knob cap itself — rotate with knob and vary per instance
    {
      float dx = bodyR * 0.2f + (dirtRand(21)-0.5f) * bodyR * 0.22f;
      float dy = bodyR * 0.4f + (dirtRand(22)-0.5f) * bodyR * 0.22f;
      const float gx = cx + (dx * cosD - dy * sinD);
      const float gy = knobCy + (dx * sinD + dy * cosD);
      const float gr = bodyR * (0.35f * (0.75f + 0.5f * dirtRand(23)));
      const int ga = static_cast<int>(8 * (0.8f + 0.5f * dirtRand(24)));
      g.FillCircle(WithAlpha(kShellDeep, ga), gx, gy, gr);
    }
    // Scratches — rotated with knob
    {
      float sx0 = -bodyR * 0.35f + (dirtRand(25)-0.5f) * bodyR * 0.2f;
      float sy0 = -bodyR * 0.25f + (dirtRand(26)-0.5f) * bodyR * 0.2f;
      float sx1 = bodyR * 0.25f + (dirtRand(27)-0.5f) * bodyR * 0.2f;
      float sy1 = bodyR * 0.45f + (dirtRand(28)-0.5f) * bodyR * 0.2f;
      const float sxs = cx + (sx0 * cosD - sy0 * sinD);
      const float sys = knobCy + (sx0 * sinD + sy0 * cosD);
      const float exs = cx + (sx1 * cosD - sy1 * sinD);
      const float eys = knobCy + (sx1 * sinD + sy1 * cosD);
      g.DrawLine(WithAlpha(IColor{32, 72, 68, 60}, static_cast<int>(16 * (0.7f + 0.6f * dirtRand(29)))), sxs, sys, exs, eys, nullptr, 1.2f);
      // second small scratch
      float s2x0 = bodyR * 0.1f + (dirtRand(30)-0.5f) * bodyR * 0.12f;
      float s2y0 = -bodyR * 0.4f + (dirtRand(31)-0.5f) * bodyR * 0.12f;
      float s2x1 = bodyR * 0.35f + (dirtRand(32)-0.5f) * bodyR * 0.12f;
      float s2y1 = -bodyR * 0.1f + (dirtRand(33)-0.5f) * bodyR * 0.12f;
      const float s2xs = cx + (s2x0 * cosD - s2y0 * sinD);
      const float s2ys = knobCy + (s2x0 * sinD + s2y0 * cosD);
      const float s2ex = cx + (s2x1 * cosD - s2y1 * sinD);
      const float s2ey = knobCy + (s2x1 * sinD + s2y1 * cosD);
      g.DrawLine(WithAlpha(IColor{24, 60, 56, 48}, static_cast<int>(14 * (0.7f + 0.6f * dirtRand(34)))), s2xs, s2ys, s2ex, s2ey, nullptr, 0.9f);
    }

    if (mHoverAmt > 0.01f || active) {
      const int rimAlpha = static_cast<int>(146.f * (mMouseDown ? 1.f : mHoverAmt));
      g.DrawCircle(WithAlpha(kShellLight, rimAlpha), cx - 0.2f, knobCy - 0.2f, outerR + 0.9f, nullptr, 1.0f);
    }

    DrawRadialLine(g,
                   WithAlpha(grooveDark, 196),
                   cx + 0.15f, knobCy + 0.18f, lineAngle,
                   bodyR * 0.22f,
                   bodyR * 0.76f,
                   3.0f);
    DrawRadialLine(g,
                   WithAlpha(kShellLight, 82),
                   cx - 0.18f, knobCy - 0.18f, lineAngle,
                   bodyR * 0.24f,
                   bodyR * 0.72f,
                   0.9f);

    // Function label — heavier weight for fast scanning
    DrawRetroText(g, 24.f, kShellText, EAlign::Center, EVAlign::Middle, mLabel.Get(), mLabelBounds);

    WDL_String valueTextStr;
    FormatKnobReadout(GetParamIdx(), value, valueTextStr);
        const float readoutSize = 21.f + mReadoutPulse * 1.1f;
        const IColor readoutBase = active ? kFieldText : BlendColor(kShellText, kFieldText, 0.28f);
        const IColor readoutColor = BlendColor(readoutBase, kCoolOn, 0.24f * Clamp01(mReadoutPulse));
    DrawTertiaryText(g,
                     readoutSize,
                     readoutColor,
                     EAlign::Center,
                     EVAlign::Middle,
                     valueTextStr.Get(),
                     mValueBounds);
  }

protected:
  IRECT GetKnobDragBounds() override {
    return mSocketBounds.GetPadded(-8.f);
  }

private:
  void TriggerReadoutPulse(float delta) {
    const float intensity = std::min(1.f, 0.36f + delta * 3.4f);
    mReadoutPulsePeak = std::max(mReadoutPulsePeak, intensity);
    mReadoutPulse = mReadoutPulsePeak;
    mReadoutPulseStart = std::chrono::steady_clock::now();
    mReadoutPulseActive = true;
  }

  void UpdateReadoutPulse() {
    if (!mReadoutPulseActive) {
      return;
    }

    constexpr float kPulseMs = 180.f;
    const float elapsed = std::chrono::duration<float, std::milli>(
      std::chrono::steady_clock::now() - mReadoutPulseStart).count();

    if (elapsed >= kPulseMs) {
      mReadoutPulse = 0.f;
      mReadoutPulsePeak = 0.f;
      mReadoutPulseActive = false;
      return;
    }

    const float t = Clamp01(elapsed / kPulseMs);
    mReadoutPulse = mReadoutPulsePeak * (1.f - EaseOutCubic(t));
    SetDirty(false);
  }

  WDL_String mLabel;
  unsigned int mDirtSeed = 0;
  IRECT mSocketBounds;
  IRECT mLabelBounds;
  IRECT mValueBounds;
  float mHoverAmt = 0.f;
  float mHoverTarget = 0.f;
  float mHoverFrom = 0.f;
  float mReadoutPulse = 0.f;
  float mReadoutPulsePeak = 0.f;
  bool mReadoutPulseActive = false;
  std::chrono::steady_clock::time_point mReadoutPulseStart;
};

// Compact circular knob for the Dry/Wet mix control.  Same visual language
// as SpeakerKnobControl (kKnobTop fill, radial indicator, travel arc,
// full-range track) but scaled down to ~16-20 px diameter so it fits in
// the lower margin below the power button.  A small "MIX" label is drawn
// below the knob using DrawUtilityText.
class TinyKnobControl final : public IKnobControlBase {
public:
  TinyKnobControl(const IRECT& bounds, int paramIdx, const char* label)
    : IKnobControlBase(bounds, paramIdx)
    , mLabel((label && label[0]) ? label : "MIX") {
    mDblAsSingleClick = true;
  }

  void OnResize() override {
    // Reserve a small band at the bottom of mRECT for the label, and let
    // the IKnobControlBase hit-test cover the whole rect so clicks are easy.
    SetTargetRECT(mRECT);
  }

  void Draw(IGraphics& g) override {
    const float value = static_cast<float>(GetValue());

    // Knob area = square at the top of the control, with a label band at the bottom.
    // The label rect is aligned to the knob rect (not the full control) so the
    // "MIX" caption sits directly under the disc, matching the SpeakerKnob layout.
    const float knobH = std::max(10.f, mRECT.H() * (2.f / 3.f));
    const IRECT knobRect(mRECT.L, mRECT.T, mRECT.R, mRECT.T + knobH);
    const IRECT labelRect(mRECT.L, mRECT.T + knobH, mRECT.R, mRECT.B);

    const float cx = knobRect.MW();
    const float cy = knobRect.MH();
    const float r = 0.5f * std::min(knobRect.W(), knobRect.H()) - 1.5f;
    if (r < 3.f) return;

    const bool hover = GetMouseIsOver();
    const bool captured = GetUI() && GetUI()->ControlIsCaptured(this);
    const bool active = (hover || captured || mMouseDown) && !IsDisabled();

    // Faint full-range track (225° → 495° = 270° of travel).
    g.DrawArc(WithAlpha(IColor(65, 250, 245, 230), 75),
              cx, cy, r + 1.8f, 225.f, 495.f, nullptr, 1.1f);

    // Travel arc — shows consumed range, brightens on hover/active.
    if (value > 0.002f) {
      const float lineAngle = Lerp(135.f, 405.f, value);
      g.DrawArc(active ? WithAlpha(kCoolGlow, 180) : WithAlpha(kCoolOn, 140),
                cx, cy, r + 1.8f, 225.f, lineAngle + 90.f, nullptr, 1.1f);
    }

    // Drop shadow under the knob for depth.
    g.FillCircle(WithAlpha(kShellDeep, 22), cx + 0.7f, cy + 1.1f, r + 0.5f);

    // Knob body — same kKnobTop → kShellLight blend as the main knobs.
    const IColor knobFill = BlendColor(kKnobTop, kShellLight, active ? 0.32f : 0.22f);
    g.FillCircle(knobFill, cx, cy, r);

    // Subtle highlight rim on hover.
    if (active) {
      g.DrawCircle(WithAlpha(kShellLight, 120), cx, cy, r + 0.4f, nullptr, 0.8f);
    }

    // Position indicator — same formula as SpeakerKnobControl (135°→405°).
    const float lineAngle = Lerp(135.f, 405.f, value);
    DrawRadialLine(g,
                   WithAlpha(BlendColor(kKnobPointerDark, kShellDeep, 0.5f), 235),
                   cx, cy, lineAngle,
                   r * 0.30f, r * 0.86f,
                   std::max(1.1f, r * 0.18f));
    DrawRadialLine(g,
                   WithAlpha(kShellLight, 95),
                   cx - 0.18f, cy - 0.18f, lineAngle,
                   r * 0.32f, r * 0.78f,
                   0.7f);

    // Label — slightly larger so it's readable, aligned under the knob.
    DrawUtilityText(g, 10.f, active ? kFieldText : kShellText,
                    EAlign::Center, EVAlign::Middle, mLabel.Get(), labelRect);
  }

protected:
  IRECT GetKnobDragBounds() override {
    return mRECT.GetPadded(-1.f);
  }

private:
  WDL_String mLabel;
};

class TransportGroupPanelControl final : public IControl {
public:
  TransportGroupPanelControl(const IRECT& bounds, int syncParamIdx)
    : IControl(bounds, syncParamIdx) {
    mIgnoreMouse = true;
  }

  bool IsSyncEnabled() {
    auto* delegate = GetDelegate();
    if (delegate) {
      if (const IParam* syncParam = delegate->GetParam(kParamSync)) {
        return syncParam->Value() > 0.5;
      }
    }
    return false;
  }

  void Draw(IGraphics& g) override {
    if (!g.CheckLayer(mLayer)) {
      g.StartLayer(this, mRECT);
      const IRECT panel = mRECT.GetPadded(-1.f);
      g.FillRoundRect(WithAlpha(kShellDeep, 30), panel.GetTranslated(0.f, 2.f), 8.f);
      FillClassicPanel(g, panel, BlendColor(kShellFace, kFieldFace, 0.17f), true, 1.f, 8.f);
      g.DrawRoundRect(WithAlpha(kShellLight, 72), panel.GetPadded(-2.f), 8.f, nullptr, 0.9f);
      
      const float w = panel.W();
      const float h = panel.H();
      
      g.FillCircle(WithAlpha(kShellDeep, 7), Snap8(panel.L + w * 0.15f), Snap8(panel.T + h * 0.8f), h * 0.4f);
      g.FillCircle(WithAlpha(kShellDeep, 5), Snap8(panel.R - w * 0.1f), Snap8(panel.B - h * 0.3f), h * 0.5f);
      g.FillCircle(WithAlpha(kShellDeep, 8), Snap8(panel.MW()), Snap8(panel.MH() - h * 0.2f), h * 0.6f);
      
      g.FillCircle(WithAlpha(kShellDeep, 14), Snap8(panel.L + 12.f), Snap8(panel.B - 8.f), 1.2f);
      g.FillCircle(WithAlpha(kShellDeep, 11), Snap8(panel.R - 18.f), Snap8(panel.T + 12.f), 0.8f);
      g.FillCircle(WithAlpha(kShellDeep, 16), Snap8(panel.MW() + 24.f), Snap8(panel.B - 6.f), 1.5f);
      g.FillCircle(WithAlpha(kShellDeep, 10), Snap8(panel.MW() - 32.f), Snap8(panel.T + 8.f), 0.7f);
      g.FillCircle(WithAlpha(kShellDeep, 12), Snap8(panel.L + 28.f), Snap8(panel.B - 14.f), 1.1f);
      
      g.DrawLine(WithAlpha(kShellDeep, 16), Snap8(panel.L + 8.f), Snap8(panel.MH() + 4.f), Snap8(panel.L + 22.f), Snap8(panel.MH() - 2.f), nullptr, 0.7f);
      g.DrawLine(WithAlpha(kShellDeep, 18), Snap8(panel.R - 15.f), Snap8(panel.B - 12.f), Snap8(panel.R - 5.f), Snap8(panel.B - 4.f), nullptr, 0.9f);
      g.DrawLine(WithAlpha(kShellDeep, 20), Snap8(panel.MW() - 10.f), Snap8(panel.T + 6.f), Snap8(panel.MW() + 15.f), Snap8(panel.T + 12.f), nullptr, 0.6f);
      g.DrawLine(WithAlpha(kShellDeep, 14), Snap8(panel.MW() + 40.f), Snap8(panel.B - 18.f), Snap8(panel.MW() + 52.f), Snap8(panel.B - 8.f), nullptr, 0.8f);

      // Removed "TEMPO" text and explicit background line to fix alignment
      // and prevent text bleeding behind the toggle button.
      
      mLayer = g.EndLayer();
    }
    g.DrawLayer(mLayer);
  }

private:
  ILayerPtr mLayer;
};

class MiniManualToggleControl final : public ISwitchControlBase {
public:
  MiniManualToggleControl(const IRECT& bounds, int paramIdx)
    : ISwitchControlBase(bounds, paramIdx) {
  }

  void OnInit() override {
    ISwitchControlBase::OnInit();
    const float initialManual = IsManualEnabled() ? 1.f : 0.f;
    mVisualManual = initialManual;
    mAnimFromManual = initialManual;
    mTargetManual = initialManual;
  }

  void SetValueFromDelegate(double value, int valIdx = 0) override {
    IControl::SetValueFromDelegate(value, valIdx);
    const float currentManual = IsManualEnabled() ? 1.f : 0.f;
    mVisualManual = currentManual;
    mAnimFromManual = currentManual;
    mTargetManual = currentManual;
    mPulse = 0.f;
    SetDirty(false);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    ISwitchControlBase::OnMouseDown(x, y, mod);
    StartToggleAnimation(IsManualEnabled() ? 1.f : 0.f);
  }

  bool OnKeyDown(float x, float y, const IKeyPress& key) override {
    if (key.C || key.A) {
      return false;
    }

    // Arrow keys explicitly set clock source: left/down = manual, right/up = host.
    if (key.VK == kVK_LEFT || key.VK == kVK_DOWN) {
      if (GetValue() > 0.5) {
        SetValueFromUserInput(0.0);
        StartToggleAnimation(1.f);
      }
      return true;
    }

    if (key.VK == kVK_RIGHT || key.VK == kVK_UP) {
      if (GetValue() < 0.5) {
        SetValueFromUserInput(1.0);
        StartToggleAnimation(0.f);
      }
      return true;
    }

    if (key.VK == kVK_SPACE || key.VK == kVK_RETURN) {
      const double toggledValue = (GetValue() > 0.5) ? 0.0 : 1.0;
      SetValueFromUserInput(toggledValue);
      StartToggleAnimation(toggledValue < 0.5 ? 1.f : 0.f);
      return true;
    }

    return false;
  }

  void OnEndAnimation() override {
    mVisualManual = mTargetManual;
    mPulse = 0.f;
    IControl::OnEndAnimation();
  }

  void OnResize() override {
    const float targetSize = Snap8(std::min(mRECT.W(), mRECT.H()));
    const float size = std::max(kMinTouchTargetSize, targetSize - Snap8(8.f));
    mButtonBounds = mRECT.GetCentredInside(size, size);
    SetTargetRECT(mRECT);
  }

  void Draw(IGraphics& g) override {
    const float shiftAngle = (mVisualManual >= 0.5f) ? 4.5f : -4.5f;
    const float shiftX = std::cos(DegToRad(shiftAngle)) * (mMouseDown ? 0.3f : 0.f);
    const float shiftY = std::sin(DegToRad(shiftAngle)) * (mMouseDown ? 0.3f : 0.f);

    const float cx = mButtonBounds.MW() + shiftX;
    const float cy = mButtonBounds.MH() + (mMouseDown ? 1.8f : 0.f) + mPulse * 0.35f + shiftY;
    const float ringR = mButtonBounds.W() * 0.52f;

    // Add hover state modifier logic
    const bool isHovered = GetMouseIsOver();
    const float hoverGlow = isHovered ? 0.15f : 0.f;
    const float glow = Clamp01(mVisualManual * 0.92f + mPulse * 0.42f + hoverGlow);

    if (glow > 0.01f) {
      g.FillCircle(WithAlpha(kCoolGlow, static_cast<int>(118.f * glow)), cx, cy, ringR + 2.f);
    }

    // [P1] Enhanced: Raised circular button with beveled rim (like power button but smaller)
    // Shadow underneath
    const float shadowOffX = mMouseDown ? -0.8f : 0.f;
    const float shadowOffY = mMouseDown ? 0.9f : 1.8f;
    g.FillCircle(WithAlpha(kShellDeep, 56), cx + shadowOffX, cy + shadowOffY, ringR + 2.2f);

    // Outer bezel ring - metallic gradient
    FillPatternCircle(g, cx, cy, ringR + 1.8f,
                      IPattern::CreateLinearGradient(cx - ringR, cy - ringR, cx + ringR, cy + ringR,
                                                     {{kShellLight, 0.f},
                                                      {BlendColor(kShellMid, kShellDark, 0.35f), 0.5f},
                                                      {kShellDark, 1.f}}));

    // Inner bezel ring
    FillPatternCircle(g, cx, cy, ringR + 0.8f,
                      IPattern::CreateLinearGradient(cx, cy - ringR, cx, cy + ringR,
                                                     {{BlendColor(kShellDark, kShellMid, 0.25f), 0.f},
                                                      {kShellLight, 1.f}}));

    // Button face with diagonal 3D gradient (highlight top-left, shadow bottom-right)
    FillPatternCircle(g, cx, cy, ringR - 0.8f,
                      IPattern::CreateLinearGradient(cx - ringR, cy - ringR, cx + ringR, cy + ringR,
                                                     {{kShellLight, 0.f},
                                                      {BlendColor(kShellLight, kShellMid, 0.4f), 0.45f},
                                                      {kShellMid, 1.f}}));

    // Strong bevel ring around the edge (highlight top-left, shadow bottom-right)
    g.DrawCircle(WithAlpha(kShellLight, 100), cx - 0.3f, cy - 0.3f, ringR + 0.9f, nullptr, 0.8f);
    g.DrawCircle(WithAlpha(kShellDeep, 120), cx + 0.3f, cy + 0.3f, ringR + 0.7f, nullptr, 1.0f);

    // Ring outline when manual mode is active (visual emphasis)
    if (mVisualManual >= 0.5f) {
      g.DrawCircle(WithAlpha(kCoolOn, static_cast<int>(100.f * mVisualManual)), cx, cy, ringR + 2.8f, nullptr, 1.2f);
    }

    const char* modeGlyph = (mVisualManual >= 0.5f) ? "M" : "H";
    const IColor modeColor = (mVisualManual >= 0.5f)
      ? BlendColor(kFieldText, kCoolOn, 0.45f)
      : BlendColor(kFieldText, kShellMid, 0.45f);
    const IRECT textBounds = mButtonBounds.GetPadded(-2.f);
    DrawUtilityText(g, 16.5f, modeColor, EAlign::Center, EVAlign::Middle,
                    modeGlyph, textBounds);

    // LED above button
    const float ledY = cy - ringR - 10.f;
    const float ledR2 = 3.5f;
    const bool hostMode = mVisualManual < 0.5f;
    const IColor ledColor = hostMode ? BlendColor(kCoolOn, kShellLight, 0.25f) : BlendColor(kShellDeep, kKnobRim, 0.6f);
    const IColor ledGlow = hostMode ? WithAlpha(kCoolGlow, 120) : WithAlpha(kLedGlowOff, 40);
    // Housing
    g.FillCircle(WithAlpha(kShellDeep, 80), cx + 0.5f, ledY + 0.5f, ledR2 + 1.5f);
    // Glow
    FillPatternCircle(g, cx, ledY, ledR2 + 3.f,
                      IPattern::CreateRadialGradient(cx, ledY, ledR2 * 3.f,
                                                    {{WithAlpha(ledGlow, static_cast<int>(ledGlow.A * 0.5f)), 0.f},
                                                     {WithAlpha(ledGlow, 0), 1.f}}));
    // Bulb
    FillPatternCircle(g, cx, ledY, ledR2,
                      IPattern::CreateRadialGradient(cx - ledR2 * 0.3f, ledY - ledR2 * 0.3f, ledR2 * 2.f,
                                                    {{IColor{255, 255, 240, 200}, 0.f},
                                                     {ledColor, 0.5f},
                                                     {BlendColor(ledColor, kShellDeep, 0.4f), 1.f}}));
  }

private:
  bool IsManualEnabled() const {
    return GetValue() < 0.5;
  }

  void StartToggleAnimation(float target) {
    if (std::abs(mVisualManual - target) < 0.001f) {
      mVisualManual = target;
      mTargetManual = target;
      mPulse = 0.f;
      SetDirty(false);
      return;
    }

    mAnimFromManual = mVisualManual;
    mTargetManual = target;
    SetAnimation([](IControl* control) {
      auto* self = control->As<MiniManualToggleControl>();
      const float t = static_cast<float>(self->GetAnimationProgress());
      const float eased = EaseSmooth(t);
      self->mVisualManual = Lerp(self->mAnimFromManual, self->mTargetManual, eased);
      self->mPulse = std::sin(t * kPi) * 0.36f;
      self->SetDirty(false);
    }, 120);
  }

  float mVisualManual = 0.f;
  float mAnimFromManual = 0.f;
  float mTargetManual = 0.f;
  float mPulse = 0.f;
  IRECT mButtonBounds;
};

class ManualBpmTextControl final : public IControl {
public:
  ManualBpmTextControl(const IRECT& bounds, int paramIdx)
    : IControl(bounds, paramIdx) {
    mDblAsSingleClick = true;
  }

  void OnInit() override {
    SetTextEntryLength(20);
  }

  void OnResize() override {
    const float insetX = Snap8(8.f);
    const float insetY = Snap8(8.f);
    mFieldBounds = IRECT(mRECT.L + insetX, mRECT.T + insetY, mRECT.R - insetX, mRECT.B - insetY);
    SetTargetRECT(mFieldBounds);
  }

  void SetValueFromDelegate(double value, int valIdx = 0) override {
    IControl::SetValueFromDelegate(value, valIdx);
    SetDirty(false);
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override {
    if (IsManualEnabled() && GetUI()) {
      GetUI()->SetMouseCursor(ECursor::HAND);
    }
    SetDirty(false);
  }

  void OnMouseOut() override {
    if (GetUI()) {
      GetUI()->SetMouseCursor();
    }
    SetDirty(false);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    if (IsManualEnabled() && mFieldBounds.Contains(x, y)) {
      OpenTextEntry();
    }

    SetDirty(false);
  }

  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
    OnMouseDown(x, y, mod);
  }

  bool OnKeyDown(float x, float y, const IKeyPress& key) override {
    if (!IsManualEnabled() || key.C || key.A) {
      return false;
    }

    switch (key.VK) {
      case kVK_RETURN:
      case kVK_SPACE:
        OpenTextEntry();
        return true;
      case kVK_UP:
        NudgeBpm(key.S ? 5.0 : 1.0);
        return true;
      case kVK_DOWN:
        NudgeBpm(key.S ? -5.0 : -1.0);
        return true;
      default:
        return false;
    }
  }

void Draw(IGraphics& g) override {
    const bool manual = IsManualEnabled();
    const bool isHovered = GetMouseIsOver();

    if (!manual) {
      const IRECT syncedBounds = mFieldBounds;

      // [P1] Enhanced: Raised button with 3D beveled edges
      DrawRaisedButton(g, syncedBounds, kShellFace, 6.f);

      const IColor hostColor = BlendColor(kCoolOn, kShellLight, 0.18f);

      // Text emboss: shadow first, then actual text
      DrawRetroText(g, 18.f, WithAlpha(kShellDeep, 55),
                    EAlign::Center, EVAlign::Middle, "HOST", syncedBounds.GetTranslated(1.f, 1.f));
      // Centered "HOST" text taking up the full bounds evenly
      DrawRetroText(g, 18.f, BlendColor(kFieldText, kCoolOn, 0.40f),
                    EAlign::Center, EVAlign::Middle, "HOST", syncedBounds);
    } else {
      // [P1] Enhanced: Raised button with 3D beveled edges
      DrawRaisedButton(g, mFieldBounds, kFieldFace, 6.f);

      g.FillRoundRect(WithAlpha(kCoolOn, 12), mFieldBounds.GetPadded(-1.f), 4.f);

      if (isHovered) {
        g.DrawRoundRect(WithAlpha(kShellLight, 160), mFieldBounds.GetPadded(-1.f), 8.f, nullptr, 1.0f);
      }

      if (mValidFlash > 0.001f) {
        const int validAlpha = static_cast<int>(std::lround(132.f * Clamp01(mValidFlash)));
        const IColor validColor = BlendColor(kCoolOn, kShellLight, 0.25f);
        g.DrawRoundRect(WithAlpha(validColor, validAlpha), mFieldBounds.GetPadded(-1.f), 8.f, nullptr, 1.3f);
      }

      if (mInvalidFlash > 0.001f) {
        const int flashAlpha = static_cast<int>(std::lround(188.f * Clamp01(mInvalidFlash)));
        g.DrawRoundRect(WithAlpha(GetValidationAlertColor(), flashAlpha), mFieldBounds.GetPadded(-1.f), 8.f, nullptr, 1.6f);
      }

      g.DrawLine(WithAlpha(kShellLight, 120), mFieldBounds.L + 6.f, mFieldBounds.T + 4.f,
                 mFieldBounds.R - 6.f, mFieldBounds.T + 4.f, nullptr, 1.f);

      WDL_String display;
      FormatBpmDisplay(GetParam(), GetValue(), display);
      // Text emboss: shadow first, then actual text
      DrawRetroText(g, 18.f, WithAlpha(kShellDeep, 55),
                    EAlign::Center, EVAlign::Middle, display.Get(), mFieldBounds.GetTranslated(1.f, 1.f));
      DrawRetroText(g,
                    18.f,
                    kFieldText,
                    EAlign::Center,
                    EVAlign::Middle,
                    display.Get(),
                    mFieldBounds);
    }
  }

  void OnTextEntryCompletion(const char* str, int valIdx) override {
    if (!GetParam() || !IsManualEnabled()) {
      return;
    }

    double clipped = 0.0;
    if (!TryParseClampedLocalizedDouble(str, GetParam()->GetMin(), GetParam()->GetMax(), clipped)) {
      TriggerInvalidInputFeedback();
      return;
    }

    mInvalidFlash = 0.f;
    TriggerValidInputFeedback();
    SetValueFromUserInput(GetParam()->ToNormalized(clipped));
  }

private:
  bool IsManualEnabled() {
    if (auto* delegate = GetDelegate()) {
      if (const IParam* syncParam = delegate->GetParam(kParamSync)) {
        return syncParam->Value() < 0.5;
      }
    }

    return true;
  }

  void NudgeBpm(double delta) {
    const IParam* param = GetParam();
    if (!param || !IsManualEnabled()) {
      return;
    }

    const double current = param->FromNormalized(GetValue());
    const double nudged = std::max(param->GetMin(), std::min(param->GetMax(), current + delta));
    SetValueFromUserInput(param->ToNormalized(nudged));
  }

  void TriggerInvalidInputFeedback() {
    mValidFlash = 0.f;
    mInvalidFlash = 1.f;
    SetAnimation([](IControl* control) {
      auto* self = control->As<ManualBpmTextControl>();
      const float t = static_cast<float>(self->GetAnimationProgress());
      self->mInvalidFlash = 1.f - EaseOutCubic(t);
      self->SetDirty(false);
    }, 220);
  }

  void TriggerValidInputFeedback() {
    mValidFlash = 1.f;
    SetAnimation([](IControl* control) {
      auto* self = control->As<ManualBpmTextControl>();
      const float t = static_cast<float>(self->GetAnimationProgress());
      self->mValidFlash = 1.f - EaseOutQuint(t);
      self->SetDirty(false);
    }, 180);
  }

  void OpenTextEntry() {
    IGraphics* ui = GetUI();
    if (!ui) {
      return;
    }

    WDL_String display;
    FormatBpmDisplay(GetParam(), GetValue(), display);
    const char* entryFontID = ShouldUseTertiaryFont() ? kTertiaryFontID : kFallbackFontID;
    const IRECT entryBounds = mFieldBounds.GetPadded(-6.f);
    const float entrySize = FitTextSizeToBounds(*ui, entryFontID, display.Get(), 20.f, entryBounds, 10.f);
    const IText entryText {entrySize, kFieldText, entryFontID, EAlign::Center, EVAlign::Middle};
    ui->CreateTextEntry(*this, entryText, entryBounds, display.Get());
  }

  IRECT mFieldBounds;
  float mInvalidFlash = 0.f;
  float mValidFlash = 0.f;
};

class MonitorBadgePlateControl final : public IControl {
public:
  MonitorBadgePlateControl(const IRECT& bounds, const char* text)
    : IControl(bounds)
    , mText(text) {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override {
    if (!g.CheckLayer(mLayer)) {
      g.StartLayer(this, mRECT);
      const IRECT plate(mRECT.L + 2.f, mRECT.T + 2.f, mRECT.R - 2.f, mRECT.B - 1.f);
      DrawBrandPlate(g, plate);
      DrawEmbossedBadgeText(g, 24.f, mText.Get(), plate);
      mLayer = g.EndLayer();
    }
    g.DrawLayer(mLayer);
  }

private:
  WDL_String mText;
  ILayerPtr mLayer;
};

class MonitorLogoPlateControl final : public IControl {
public:
  MonitorLogoPlateControl(const IRECT& bounds, const ISVG& face,
                          const ISVG& highlight, const ISVG& shadow)
    : IControl(bounds)
    , mFace(face)
    , mHighlight(highlight)
    , mShadow(shadow) {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override {
    if (!g.CheckLayer(mLayer)) {
      g.StartLayer(this, mRECT);
      const IRECT plate(mRECT.L + 2.f, mRECT.T + 2.f, mRECT.R - 2.f, mRECT.B - 1.f);
      DrawBrandPlate(g, plate);
      bool hasLogo = mFace.IsValid() || mHighlight.IsValid() || mShadow.IsValid();

      if (!hasLogo && !mTriedLateLoad) {
        mFace = g.LoadSVG(BRAND_LOGO_FACE_FN);
        mHighlight = g.LoadSVG(BRAND_LOGO_HIGHLIGHT_FN);
        mShadow = g.LoadSVG(BRAND_LOGO_SHADOW_FN);
        mTriedLateLoad = true;
        hasLogo = mFace.IsValid() || mHighlight.IsValid() || mShadow.IsValid();
      }

      if (hasLogo) {
        DrawEmbossedSVG(g, mFace, mHighlight, mShadow, plate);
      }
      mLayer = g.EndLayer();
    }
    g.DrawLayer(mLayer);
  }

private:
  ISVG mFace;
  ISVG mHighlight;
  ISVG mShadow;
  bool mTriedLateLoad = false;
  ILayerPtr mLayer;
};

class MonitorPowerButtonControl final : public ISwitchControlBase {
public:
  MonitorPowerButtonControl(const IRECT& bounds, int paramIdx, const char* label)
    : ISwitchControlBase(bounds, paramIdx)
    , mLabel((label && label[0]) ? label : "POWER") {
  }

  void OnInit() override {
    ISwitchControlBase::OnInit();
    mVisualOn = GetValue() > 0.5 ? 1.f : 0.f;
    mAnimFromOn = mVisualOn;
    mTargetOn = mVisualOn;
    mPressAmount = 0.f;
    mHoverAmt = 0.f;
    mHoverTarget = 0.f;
  }

  void SetValueFromDelegate(double value, int valIdx = 0) override {
    IControl::SetValueFromDelegate(value, valIdx);
    mVisualOn = GetValue() > 0.5 ? 1.f : 0.f;
    mAnimFromOn = mVisualOn;
    mTargetOn = mVisualOn;
    mPressAmount = 0.f;
    SetDirty(false);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override {
    ISwitchControlBase::OnMouseDown(x, y, mod);
    mVisualOn = GetValue() > 0.5 ? 1.f : 0.f;
    mAnimFromOn = mVisualOn;
    mTargetOn = mVisualOn;
    mPressAmount = 1.f;
    SetDirty(false);
  }

  void OnMouseUp(float x, float y, const IMouseMod& mod) override {
    ISwitchControlBase::OnMouseUp(x, y, mod);
    mPressAmount = 0.f;
    SetDirty(false);
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override {
    ISwitchControlBase::OnMouseOver(x, y, mod);
    if (GetUI()) {
      GetUI()->SetMouseCursor(ECursor::HAND);
    }
    mHoverTarget = 1.f;
    mHoverFadeFrom = mHoverAmt;
    SetAnimation([](IControl* c) {
      auto* self = c->As<MonitorPowerButtonControl>();
      const float t = EaseOutQuint(static_cast<float>(self->GetAnimationProgress()));
      self->mHoverAmt = self->mHoverFadeFrom + (1.f - self->mHoverFadeFrom) * t;
      self->SetDirty(false);
    }, 140);
  }

  void OnMouseOut() override {
    ISwitchControlBase::OnMouseOut();
    if (GetUI()) {
      GetUI()->SetMouseCursor();
    }
    mHoverFadeFrom = mHoverAmt;
    mHoverTarget = 0.f;
    SetAnimation([](IControl* c) {
      auto* self = c->As<MonitorPowerButtonControl>();
      const float t = EaseOutQuint(static_cast<float>(self->GetAnimationProgress()));
      self->mHoverAmt = self->mHoverFadeFrom * (1.f - t);
      self->SetDirty(false);
    }, 80);
  }

  bool OnKeyDown(float x, float y, const IKeyPress& key) override {
    if (key.C || key.A) {
      return false;
    }

    if (key.VK == kVK_SPACE || key.VK == kVK_RETURN) {
      const double toggledValue = (GetValue() > 0.5) ? 0.0 : 1.0;
      SetValueFromUserInput(toggledValue);
      const float target = toggledValue > 0.5 ? 1.f : 0.f;
      mVisualOn = target;
      mAnimFromOn = target;
      mTargetOn = target;
      mPressAmount = 1.f;
      SetDirty(false);
      return true;
    }

    return false;
  }

  void OnEndAnimation() override {
    mVisualOn = mTargetOn;
    mPressAmount = 0.f;
    mHoverAmt = mHoverTarget;
    IControl::OnEndAnimation();
  }

  void OnResize() override {
    // Button uses the full bounds for alignment with other controls
    mButtonBounds = mRECT;
    // Hide the title by default - button only shows icon
    mTitleBounds = IRECT(0, 0, 0, 0);
    SetTargetRECT(mButtonBounds.GetPadded(6.f));
  }

  void Draw(IGraphics& g) override {
    const bool hover = GetMouseIsOver();
    const bool captured = GetUI() && GetUI()->ControlIsCaptured(this);
    const bool active = (hover || captured || mMouseDown) && !IsDisabled();

    const float cx = mButtonBounds.MW();
    const float cy = mButtonBounds.MH();
    const float outerR = std::min(mButtonBounds.W(), mButtonBounds.H()) * 0.5f - 2.f;
    const float rimR = outerR - 3.2f;
    const float wellR = outerR - 9.2f;
    const float faceBaseR = outerR - 10.2f;
    
    // Enhance 3D depression physics
    const float faceScale = Lerp(1.f, 0.95f, mPressAmount);
    const float faceFlatten = Lerp(1.f, 0.96f, mPressAmount);
    const float faceRx = faceBaseR * faceScale;
    const float faceRy = faceBaseR * faceScale * faceFlatten;
    const float faceCx = cx;
    const float faceCy = cy + mPressAmount * 2.2f; // Deeper physical travel
    
    const float symbolR = std::min(faceRx, faceRy) * 0.31f;
    const float symbolThickness = 3.6f * faceScale; // slightly bolder
    const float ledHousingR = 4.6f * faceScale;
    const float ledR = 2.9f * faceScale;
    const float ledCx = faceCx - faceRx * 0.65f;
    const float ledCy = faceCy;

    // Blended colors from the global shell palette for perfect UI coherence
    static const IColor outerShell  = BlendColor(kShellDeep, kFieldInset, 0.40f);
    static const IColor rimLight    = kShellLight;
    static const IColor rimDark     = BlendColor(kShellMid, kFieldInset, 0.30f);
    static const IColor wellLight   = kShellFace;
    static const IColor wellDark    = kShellDark;
    
    // Core plastic face marching perfectly with the classic layout casing
    static const IColor faceLight   = BlendColor(kShellLight, kKnobTop, 0.35f);
    static const IColor faceMid     = BlendColor(kShellFace, kKnobTop, 0.20f);
    static const IColor faceDark    = BlendColor(kShellMid, kShellFace, 0.40f);
    static const IColor faceBarrel  = BlendColor(kShellDark, kShellMid, 0.45f);
    
    static const IColor symbolColor = BlendColor(kShellText, kFieldInset, 0.15f);
    // LED colors — promoted from local statics to named constants for palette consistency
    static const IColor ledOff      = kLedOff;
    static const IColor ledOn       = kLedOn;
    static const IColor ledGlowOff  = kLedGlowOff;
    static const IColor ledGlowOn   = kLedGlowOn;

    // These two change with mVisualOn and must remain non-static:
    const IColor ledColor = BlendColor(ledOff, ledOn, mVisualOn);
    const IColor ledGlow  = BlendColor(ledGlowOff, ledGlowOn, mVisualOn);

    const float hoverGlow = mMouseDown ? 0.75f : mHoverAmt * 0.45f;
    if (hoverGlow > 0.01f) {
      g.FillCircle(WithAlpha(kCoolGlow, static_cast<int>(105.f * hoverGlow)), cx, cy, outerR + 1.6f);
    }

    // Subtle bezel ring — kept within bounds to avoid capping
    const float bezelR = outerR + 1.6f;
    g.FillCircle(WithAlpha(kShellDeep, 68), cx, cy + 3.2f, bezelR + 1.2f);
    FillPatternCircle(g, cx, cy, bezelR,
                      IPattern::CreateLinearGradient(cx - bezelR, cy - bezelR, cx + bezelR, cy + bezelR,
                                                     {{kShellLight, 0.f},
                                                      {BlendColor(kShellMid, kShellDark, 0.4f), 0.45f},
                                                      {BlendColor(kShellDark, kShellDeep, 0.35f), 1.f}}));

    // Inner edge of outer bezel
    FillPatternCircle(g, cx, cy, bezelR - 1.0f,
                      IPattern::CreateLinearGradient(cx, cy - bezelR, cx, cy + bezelR,
                                                     {{BlendColor(kShellDark, kShellMid, 0.3f), 0.f},
                                                      {kShellLight, 1.f}}));

    g.FillCircle(WithAlpha(kShellDeep, 74), cx, cy + 2.6f, outerR + 0.8f);
    g.FillCircle(outerShell, cx, cy, outerR);
    FillPatternCircle(g, cx, cy, rimR,
                      IPattern::CreateLinearGradient(cx - rimR, cy - rimR, cx + rimR, cy + rimR,
                                                     {{rimLight, 0.f},
                                                      {rimDark, 1.f}}));
    FillPatternCircle(g, cx, cy, wellR,
                      IPattern::CreateLinearGradient(cx, cy - wellR, cx, cy + wellR,
                                                     {{wellDark, 0.f},
                                                      {wellLight, 1.f}}));

    if (mHoverAmt > 0.01f || mMouseDown) {
      const int rimA = static_cast<int>(120.f * (mMouseDown ? 1.f : mHoverAmt));
      g.DrawCircle(WithAlpha(kShellLight, rimA), cx - 0.22f, cy - 0.24f, rimR + 0.45f, nullptr, 1.2f);
    }
    
    // Draw the 3D barrel (the side of the button cylinder) underneath
    const float barrelDepression = 3.6f * (1.f - mPressAmount); 
    if (barrelDepression > 0.5f) {
      // Use the barrel linear gradient matching SVG #b (vertical)
      FillPatternEllipse(g, faceCx, faceCy + barrelDepression * 0.5f, faceRx, faceRy + barrelDepression * 0.5f,
                         IPattern::CreateLinearGradient(faceCx, (faceCy - faceRy), faceCx, (faceCy + faceRy + barrelDepression),
                                                        {{faceBarrel, 0.f},
                                                         {wellLight, 1.f}}));
    }

    // Shadow cast by the button inside the well
    const float faceDropShadowY = 4.2f * (1.1f - mPressAmount); 
    g.FillEllipse(WithAlpha(kFieldInset, static_cast<int>(110 * (1.f - mPressAmount * 0.5f))), 
                  faceCx, faceCy + faceDropShadowY, faceRx + 1.2f, faceRy + 0.8f);

    // The actual button face top surface - linear gradient matching SVG #c
    FillPatternEllipse(g, faceCx, faceCy, faceRx, faceRy, 
                       IPattern::CreateLinearGradient(faceCx - faceRx, faceCy - faceRy, faceCx + faceRx, faceCy + faceRy,
                                                      {{faceLight, 0.f},
                                                       {faceDark, 1.f}}));
    g.DrawEllipse(WithAlpha(faceLight, 100), faceCx - 0.3f, faceCy - 0.35f, faceRx, faceRy, 0.f, nullptr, 0.8f);
    g.DrawEllipse(WithAlpha(faceBarrel, 80), faceCx + 0.2f, faceCy + 0.45f, faceRx, faceRy, 0.f, nullptr, 0.8f);

    // Power Symbol
    g.DrawArc(symbolColor, faceCx, faceCy + 0.8f * mPressAmount, symbolR, 42.f, 318.f, nullptr, symbolThickness);
    g.DrawLine(symbolColor,
               faceCx,
               faceCy - symbolR - 7.6f * faceScale,
               faceCx,
               faceCy - symbolR * 0.08f,
               nullptr,
               symbolThickness);

    // Embedded LED Housing
    g.FillCircle(BlendColor(symbolColor, kShellDeep, 0.20f), ledCx + 0.45f, ledCy + 0.65f, ledHousingR + 0.75f);
    g.FillCircle(symbolColor, ledCx, ledCy, ledHousingR);
    
    // LED Inner Glow and bulb — clearly visible on/off states
    const float ledOpacity = mVisualOn > 0.5f ? 1.f : 0.12f;
    const IColor ledBulbCenter = mVisualOn > 0.5f
      ? IColor{255, 255, 236, 182}
      : BlendColor(IColor{255, 255, 236, 182}, symbolColor, 0.7f);
    FillPatternCircle(g, ledCx, ledCy, ledR + 2.0f,
                      IPattern::CreateRadialGradient(ledCx - ledR * 0.35f, ledCy - ledR * 0.42f, ledR * 2.8f,
                                                     {{WithAlpha(ledGlow, static_cast<int>(ledGlow.A * ledOpacity)), 0.f},
                                                      {WithAlpha(ledGlow, static_cast<int>(ledGlow.A * std::max(0.08f, ledOpacity * 0.65f))), 0.46f},
                                                      {WithAlpha(ledGlow, 8), 1.f}}));
    FillPatternCircle(g, ledCx, ledCy, ledR,
                      IPattern::CreateRadialGradient(ledCx - ledR * 0.34f, ledCy - ledR * 0.40f, ledR * 1.6f,
                                                     {{ledBulbCenter, 0.f},
                                                      {ledColor, 0.32f},
                                                      {BlendColor(ledColor, symbolColor, 0.46f), 1.f}}));

    if (mTitleBounds.H() > 8.f) {
      const IColor titleColor = active
        ? BlendColor(kShellText, kFieldText, 0.20f)
        : BlendColor(kShellText, kShellTextDim, 0.22f);
      DrawUtilityText(g,
                      10.5f,
                      titleColor,
                      EAlign::Center,
                      EVAlign::Middle,
                      mLabel.Get(),
                      mTitleBounds);
    }

  }

private:
  void StartToggleAnimation(float target) {
    if (std::abs(mVisualOn - target) < 0.001f) {
      mVisualOn = target;
      mTargetOn = target;
      mPressAmount = 0.f;
      SetDirty(false);
      return;
    }

    mAnimFromOn = mVisualOn;
    mTargetOn = target;
    SetAnimation([](IControl* control) {
      auto* self = control->As<MonitorPowerButtonControl>();
      const float totalMs = 380.f;
      const float elapsedMs = static_cast<float>(self->GetAnimationProgress()) * totalMs;

      if (elapsedMs < 100.f) {
        self->mPressAmount = EaseSmooth(elapsedMs / 100.f);
      } else if (elapsedMs < 180.f) {
        self->mPressAmount = 1.f - EaseOutCubic((elapsedMs - 100.f) / 80.f);
      } else {
        self->mPressAmount = 0.f;
      }

      if (elapsedMs < 180.f) {
        self->mVisualOn = self->mAnimFromOn;
      } else {
        self->mVisualOn = Lerp(self->mAnimFromOn, self->mTargetOn, EaseSmooth((elapsedMs - 180.f) / 200.f));
      }

      self->SetDirty(false);
    }, 380);
  }

  WDL_String mLabel;
  float mVisualOn = 0.f;
  float mAnimFromOn = 0.f;
  float mTargetOn = 0.f;
  float mPressAmount = 0.f;
  float mHoverAmt = 0.f;
  float mHoverTarget = 0.f;
  float mHoverFadeFrom = 0.f;
  IRECT mButtonBounds;
  IRECT mTitleBounds;
};

class ZoneCaptureUI final : public UI {
public:
  explicit ZoneCaptureUI(std::unordered_map<std::string, FAUSTFLOAT*>& zones)
    : mZones(zones) {
  }

  void addCheckButton(const char* label, FAUSTFLOAT* zone) override { mZones[label] = zone; }
  void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }
  void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override {
    mZones[label] = zone;
  }

private:
  std::unordered_map<std::string, FAUSTFLOAT*>& mZones;
};

}  // namespace

Freeze95::Freeze95(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kParamPower)->InitBool("Effect On", true);
  GetParam(kParamChaos)->InitDouble("Chaos Amount", 0.55, 0.0, 1.0, 0.01, "%");
  GetParam(kParamLoFi)->InitDouble("Lo-Fi Amount", 0.30, 0.0, 1.0, 0.01, "%");
  GetParam(kParamBpm)->InitDouble("Tempo (BPM)", 120.0, 20.0, 300.0, 0.1, "bpm");
  GetParam(kParamSync)->InitBool("Sync to Host", true);
  // Dry/Wet is applied in C++ ProcessBlock (post-Faust mix), not in the Faust DSP itself.
  GetParam(kParamDryWet)->InitDouble("Dry/Wet", 100.0, 0.0, 100.0, 1.0, "%");

  // Factory presets - these work with iPlug2's MakePreset API.
  // NOTE: Some DAWs (e.g., certain VST3 hosts) do not expose factory presets in their
  // preset browser. Presets can still be saved/loaded via chunk state (host project files).
  // The preset visibility depends on the DAW's VST3 preset support implementation.

  mDSP = std::make_unique<Freeze95DSP>();

  // Initialize DSP early with a plausible default sample rate so the internal
  // state (~48 MB of delay buffers, filter coeffs) is valid even if the host
  // calls ProcessBlock() before OnReset().  OnReset() re-initialises with the
  // real sample rate when it fires.
  mDSP->init(44100.0f);

  ZoneCaptureUI captureUI(mFaustZones);
  mDSP->buildUserInterface(&captureUI);
  SyncParamsToDSP();

  // Pre-allocate processing buffers to the maximum expected block size.
  // This avoids real-time unsafe std::vector::resize() calls in OnReset().
  mInL.resize(kMaxBlockFrames);
  mInR.resize(kMaxBlockFrames);
  mOutL.resize(kMaxBlockFrames);
  mOutR.resize(kMaxBlockFrames);

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    // Use GetScaleForScreen() to compute the initial scale from the screen DPI.
    // The DPI awareness manifest (Freeze95.manifest) declares PerMonitorV2 so
    // Windows passes the correct DPI; GetScaleForScreen() translates that to
    // the proper initial scale. OnParentWindowResize() then adjusts if the
    // host provides a different window size.
    auto* graphics = MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS,
                                  GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
    return graphics;
  };

  mLayoutFunc = [&](IGraphics* g) { LayoutUI(g); };
#endif
}

#if IPLUG_EDITOR

class CoolFilterOverlayControl final : public IControl {
public:
  CoolFilterOverlayControl(const IRECT& bounds) : IControl(bounds) { mIgnoreMouse = true; }
  void Draw(IGraphics& g) override { ApplyCoolFilter(g, mRECT); }
};
void Freeze95::LayoutUI(IGraphics* g) {
  // Use live dimensions so this function works for both initial layout and
  // any future re-layout calls.  In Scale mode Width()/Height() equal the
  // original PLUG_WIDTH/PLUG_HEIGHT; in Size mode they reflect the new size.
  const float w = static_cast<float>(g->Width());
  const float h = static_cast<float>(g->Height());

  // Allow user to scale the window between 50 % (small laptop DAW) and 200 %
  // (4K display comfort), snapping the entire vector scene.
  g->SetScaleConstraints(0.65f, 2.0f);

  const float outerMargin = Snap8(ClampValue(w * 0.029f, 24.f, 32.f));

  // Plates position and height: larger and aligned with the SVG computer screen bezel
  const float brandPlateTop = Snap8(ClampValue(h * 0.200f, 48.f, 72.f));
  const float brandPlateHeight = Snap8(ClampValue(h * 0.120f, 36.f, 48.f));
  const float badgePlateTop = brandPlateTop;
  const float badgePlateHeight = brandPlateHeight; // Identical height for perfect symmetry

  const float headerGap = Snap8(ClampValue(h * 0.035f, 4.f, 16.f));
  const float majorTop = brandPlateTop + brandPlateHeight + headerGap;
  const float lowerMargin = Snap8(ClampValue(h * 0.08f, 24.f, 36.f));
  const float majorBottom = h - lowerMargin;
  const float majorHeight = std::max(152.f, majorBottom - majorTop);

  const float knobGap = Snap8(16.f);
  const float transportGap = Snap8(24.f);
  const float powerGap = Snap8(40.f);  // Restored to original — the Dry/Wet knob lives in the footer now

  // Equal raw gaps looked uneven because the knob faces occupy much less of
  // their bounds than the transport panel and power button. Weight the row by
  // visible mass instead.
  float chaosWidth = Snap8(ClampValue(w * 0.175f, 144.f, 152.f));
  float loFiWidth = chaosWidth;
  float powerWidth = Snap8(ClampValue(w * 0.175f, 144.f, 152.f));

  const float logoPlateWidth = chaosWidth;
  const float badgePlateWidth = powerWidth;

  const float bpmPanelTop = majorTop + Snap8(ClampValue(majorHeight * 0.20f, 24.f, 40.f));
  const float bpmPanelBottom = majorBottom - Snap8(ClampValue(majorHeight * 0.18f, 24.f, 40.f));
  const float bpmPanelHeight = std::max(64.f, bpmPanelBottom - bpmPanelTop);
  const float toggleSize = Snap8(ClampValue(bpmPanelHeight * 0.54f, 48.f, 64.f));
  const float toggleFieldGap = Snap8(16.f);

  if (g->NControls()) {
    return;
  }

  g->EnableMouseOver(true);
  const bool fallbackFontLoaded = LoadFallbackUIFont(g);
  const bool utilityFontLoaded = LoadUtilityUIFont(g);
  const bool tertiaryFontLoaded = LoadTertiaryUIFont(g);
  const char* activeFontID = LoadActiveDOSFont(g);
  bool usePixelTextFallback = false;

  if (!activeFontID) {
    activeFontID = fallbackFontLoaded ? kFallbackFontID : kDOSFontID;
    usePixelTextFallback = !fallbackFontLoaded;
  }

  SetActiveFontID(activeFontID);
  SetUsePixelTextFallback(usePixelTextFallback);
  SetUseUtilityFont(utilityFontLoaded);
  SetUseTertiaryFont(tertiaryFontLoaded);

  // Load monitor background SVGs with multiple fallback paths
  // First try: direct filename (bundle resource)
  // Second try: resources subdirectory
  // Third try: relative to working directory
  auto LoadSVGWithFallback = [g](const char* fileName, const char* subPath) -> ISVG {
    ISVG result = g->LoadSVG(fileName);
    if (!result.IsValid() && subPath) {
      WDL_String fallbackPath;
      fallbackPath.SetFormatted(256, "%s/%s", subPath, fileName);
      result = g->LoadSVG(fallbackPath.Get());
    }
    return result;
  };

  ISVG monitorBackgroundOn = LoadSVGWithFallback(MONITOR_BG_ON_FN, SHARED_RESOURCES_SUBPATH);
  ISVG monitorBackgroundOff = LoadSVGWithFallback(MONITOR_BG_OFF_FN, SHARED_RESOURCES_SUBPATH);
  ISVG brandLogoFace = LoadSVGWithFallback(BRAND_LOGO_FACE_FN, SHARED_RESOURCES_SUBPATH);
  ISVG brandLogoHighlight = LoadSVGWithFallback(BRAND_LOGO_HIGHLIGHT_FN, SHARED_RESOURCES_SUBPATH);
  ISVG brandLogoShadow = LoadSVGWithFallback(BRAND_LOGO_SHADOW_FN, SHARED_RESOURCES_SUBPATH);

  g->AttachPanelBackground(kShellBg);
  g->AttachTextEntryControl();
  g->AttachControl(new MonitorBezelControl(
    IRECT(0.f, 0.f, w, h),
    kParamPower,
    monitorBackgroundOn,
    monitorBackgroundOff));

  const float macroTopInset = Snap8(ClampValue(majorHeight * 0.03f, 8.f, 16.f));
  const float macroBottomInset = Snap8(ClampValue(majorHeight * 0.04f, 8.f, 16.f));
  const float loFiTopOffset = 0.f;

  const float transportInsetX = Snap8(16.f);
  const float transportInsetY = Snap8(16.f);

  const float totalTransportWidth = Snap8(ClampValue(w * 0.27f, 216.f, 224.f));
  const float contentWidth = chaosWidth + knobGap + loFiWidth + transportGap + totalTransportWidth + powerGap + powerWidth;
  const float rowLeft = std::max(outerMargin, 0.5f * (w - contentWidth));

  const IRECT chaosBounds(rowLeft,
                          majorTop + macroTopInset,
                          rowLeft + chaosWidth,
                          majorBottom - macroBottomInset);

  const IRECT loFiBounds(chaosBounds.R + knobGap,
                         majorTop + macroTopInset + loFiTopOffset,
                         chaosBounds.R + knobGap + loFiWidth,
                         majorBottom - macroBottomInset);

  const float transportPanelL = loFiBounds.R + transportGap;
  const IRECT transportPanelBounds(transportPanelL,
                                   bpmPanelTop - transportInsetY,
                                   transportPanelL + totalTransportWidth,
                                   bpmPanelBottom + transportInsetY);

  const float powerTop = majorTop + macroTopInset;
  const float powerBottom = majorBottom - macroBottomInset;

  const float powerPanelL = transportPanelBounds.R + powerGap;
  const IRECT powerBounds(powerPanelL, powerTop, powerPanelL + powerWidth, powerBottom);

  // Tiny Dry/Wet mix knob — fits in the lower-margin space below the power
  // button.  Diameter ~23 px (radius ~11.5).  Right-edge aligned with the
  // power button so it reads as a continuation of the power hardware.
  // Slightly up (knob top sits at the power button's bottom edge).  A
  // "MIX" label sits in the same control's bounds, below the knob, aligned
  // to the knob's width (not the full control rect).
  // Geometry: lower margin is 32 px (PLUG_HEIGHT - powerBounds.B).  We budget
  // it as 2 * radius for the knob + 9 px for the label band = exactly 32 px.
  const float dryWetR = ClampValue(h * 0.0375f, 11.f, 12.f);   // radius, raw
  const float dryWetD = dryWetR * 2.f;
  const float labelBandH = 9.f;  // height reserved for the MIX label band
  // Horizontal: place the knob in the empty gap between the transport
  // panel's right edge and the power button's left edge, nudged slightly
  // to the right (toward the power button) so it reads as part of the
  // power hardware group rather than drifting toward the transport.
  const float transportRight = transportPanelBounds.R;
  const float powerLeft = powerBounds.L;
  const float gapCenterX = 0.5f * (transportRight + powerLeft);
  // "Slightly to the right" → 60% across the gap from the transport side.
  const float dryWetCenterX = transportRight + 0.60f * (powerLeft - transportRight);
  // Vertical: nudge the knob slightly above the gap's midpoint, toward
  // the transport panel.  The knob's diameter exceeds the 16-px gap, so
  // it straddles both panel edges (no visual overlap because the knob
  // sits in the horizontal gap between the two controls).
  const float dryWetCenterY = 252.f;
  const float dryWetLeft = dryWetCenterX - dryWetR;
  const float dryWetTop = dryWetCenterY - dryWetR;
  // The control's bounds include the knob and the label band.  The label
  // rect inside the control is aligned to the knob's width (see Draw()).
  const IRECT dryWetBounds(dryWetLeft, dryWetTop,
                           dryWetLeft + dryWetD, dryWetTop + dryWetD + labelBandH);

  // Plates anchored to align exactly with inner bezels.
  const IRECT logoPlateBounds(chaosBounds.L, badgePlateTop,
                              chaosBounds.L + logoPlateWidth, badgePlateTop + badgePlateHeight);
  // Align badge plate precisely on the right margin just like the Power bounds below it
  const IRECT badgePlateBounds(powerBounds.L, badgePlateTop,
                               powerBounds.L + badgePlateWidth, badgePlateTop + badgePlateHeight);

  g->AttachControl(new MonitorLogoPlateControl(
    logoPlateBounds,
    brandLogoFace,
    brandLogoHighlight,
    brandLogoShadow));
  g->AttachControl(new MonitorBadgePlateControl(badgePlateBounds, "Freeze95"));
  const IRECT transportInner = transportPanelBounds.GetPadded(-transportInsetX);

  const float syncTop = Snap8(transportInner.MH() - toggleSize * 0.5f);
  const IRECT syncBounds(transportInner.L, syncTop, transportInner.L + toggleSize, syncTop + toggleSize);

  const float bpmPanelLeft = syncBounds.R + toggleFieldGap;
  const IRECT bpmBounds(bpmPanelLeft,
                        transportInner.T,
                        transportInner.R,
                        transportInner.B);

  g->AttachControl(new TransportGroupPanelControl(
    transportPanelBounds,
    kParamSync));

  g->AttachControl(new SpeakerKnobControl(
    chaosBounds,
    kParamChaos, "CHAOS"));

  g->AttachControl(new SpeakerKnobControl(
    loFiBounds,
    kParamLoFi, "LO-FI"));

  g->AttachControl(new MiniManualToggleControl(
    syncBounds,
    kParamSync));

  g->AttachControl(new ManualBpmTextControl(
    bpmBounds,
    kParamBpm));

  g->AttachControl(new MonitorPowerButtonControl(
    powerBounds,
    kParamPower, ""));

  // Tiny Dry/Wet mix knob — sits in the lower margin just below the power
  // button.  Same design language as SpeakerKnobControl, just much smaller,
  // with a "MIX" label drawn inside the same control's bounds.
  g->AttachControl(new TinyKnobControl(dryWetBounds, kParamDryWet, "MIX"));

  // Bypass wash — covers the full panel so power-off reads like the monitor went dark
  const IRECT bypassCoverBounds(0.f, 0.f, w, h);
  g->AttachControl(new BypassOverlayControl(bypassCoverBounds, kParamPower));
  g->AttachControl(new CoolFilterOverlayControl(IRECT(0.f, 0.f, w, h)));
  // Corner resizer - Scale mode zooms the entire vector scene without
  // changing logical coordinates. layoutOnResize MUST be false.
  g->AttachCornerResizer(EUIResizerMode::Scale, false,
                         IColor(60, 255, 255, 255),             // visible at rest
                         WithAlpha(kShellDark, 120),     // visible on hover
                         WithAlpha(kShellText, 210),     // solid while dragging
                         24.f);
}

void Freeze95::OnParentWindowResize(int width, int height) {
  if (!GetUI()) {
    return;
  }

  const float scaleX = static_cast<float>(width) / static_cast<float>(PLUG_WIDTH);
  const float scaleY = static_cast<float>(height) / static_cast<float>(PLUG_HEIGHT);
  const float scale = ClampValue(std::min(scaleX, scaleY), 0.65f, 2.0f);

  // Strip and re-layout at the new scale so every control keeps its
  // proportional relationship to the others.  Without this, the canvas
  // is scaled but the control bounds are stale, so different controls
  // (text, knobs, SVGs that were rasterised at the old scale) end up
  // at different visual scales.  LayoutUI's early-return-on-non-empty
  // guard is satisfied because we just cleared the controls above.
  GetUI()->RemoveAllControls();
  GetUI()->Resize(PLUG_WIDTH, PLUG_HEIGHT, scale, false);
  LayoutUI(GetUI());
}
#endif

void Freeze95::SetFaustParam(const char* label, float value) {
  auto it = mFaustZones.find(label);
  if (it != mFaustZones.end() && it->second) {
    *(it->second) = value;
  }
}

void Freeze95::SyncParamsToDSP() {
  if (!mDSP) {
    return;
  }

  const float powerOn = static_cast<float>(GetParam(kParamPower)->Value() > 0.5);
  SetFaustParam("Bypass", 1.0f - powerOn);
  SetFaustParam("Chaos", static_cast<float>(GetParam(kParamChaos)->Value()));
  SetFaustParam("Lo-Fi", static_cast<float>(GetParam(kParamLoFi)->Value()));
  SetFaustParam("transport/bpm", static_cast<float>(GetParam(kParamBpm)->Value()));
  SetFaustParam("Sync BPM", static_cast<float>(GetParam(kParamSync)->Value() > 0.5));
}

void Freeze95::OnParamChange(int paramIdx) {
  switch (paramIdx) {
    case kParamPower:
    case kParamChaos:
    case kParamLoFi:
    case kParamBpm:
    case kParamSync:
      SyncParamsToDSP();
      if (paramIdx == kParamSync && GetUI()) {
        GetUI()->SetAllControlsDirty();
      }
      break;
    default:
      break;
  }
}

void Freeze95::OnReset() {
#ifdef _WIN32
  // Ensure FTZ/DAZ is active during DSP reinit so denormals don't accumulate
  // in delay-line state while the host is reconfiguring.
  _mm_setcsr(_mm_getcsr() | 0x8040);
#endif

  if (!mDSP) {
    return;
  }

  mDSP->init(static_cast<int>(GetSampleRate()));

  // Track the host-reported block size for bounds checking in ProcessBlock().
  // Buffers were pre-allocated in the constructor to avoid real-time allocation.
  mBufferSize = std::max(16384, GetBlockSize());

  SyncParamsToDSP();
}

void Freeze95::OnActivate(bool active) {
  DBGMSG("OnActivate: active=%d\n", active);
  if (active) {
#if defined IPLUG_VST3
    // When the plugin is reactivated (e.g., track re-enabled, session reopened),
    // the VST3 parameter container (mParameters) may have stale values because
    // setComponentState is a no-op in non-distributed VST3. Sync it here.
    // This ensures the GUI controls show correct values when the UI opens.
    UpdateParams(this, GetBypassed() ? 1 : 0);
#endif
    
    // Signal OnIdle to update UI controls with current parameter values
    mSendUpdate = true;
    
    // Log current parameter values to help diagnose the issue
    for (int i = 0; i < kNumParams; ++i) {
      DBGMSG("  Param %d: value=%.3f normalized=%.3f\n", 
             i, GetParam(i)->Value(), GetParam(i)->GetNormalized());
    }
  }
}

void Freeze95::OnIdle() {
#if IPLUG_EDITOR
  if (mSendUpdate) {
    if (GetUI()) {
      // Send current parameter values to UI controls
      SendCurrentParamValuesFromDelegate();
    }
    mSendUpdate = false;
  }
#endif
}

void Freeze95::OnUIOpen() {
  // Call base implementation which sends current parameter values to UI controls
  IEditorDelegate::OnUIOpen();
}

void Freeze95::OnRestoreState() {
  // Call base implementation which sends current parameter values to UI controls
  IEditorDelegate::OnRestoreState();
}

void Freeze95::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
  // FTZ/DAZ MUST be set BEFORE any early-return paths (host bypass, transport
  // gate, null DSP) so denormals are flushed even when the DSP path is skipped.
  // Otherwise the large delay-line state (~48 MB) can accumulate denormals
  // during paused/bypassed periods, causing CPU spikes on resume.
#ifdef _WIN32
  _mm_setcsr(_mm_getcsr() | 0x8040);
#elif defined(__APPLE__) && defined(__x86_64__)
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

  const int nIn = NInChansConnected();
  const int nOut = NOutChansConnected();
  const int originalNFrames = nFrames;

  auto outputDry = [&](int startFrame, int frameCount) {
    const int nChans = std::min(nIn, nOut);
    for (int s = 0; s < frameCount; ++s) {
      for (int c = 0; c < nChans; ++c) {
        outputs[c][startFrame + s] = inputs[c][startFrame + s];
      }
      for (int c = nChans; c < nOut; ++c) {
        outputs[c][startFrame + s] = 0.0;
      }
    }
  };

  if (!mDSP || nFrames <= 0) {
    outputDry(0, originalNFrames);
    return;
  }

  // Respect host VST3 bypass — output dry when bypassed so the power button
  // and host bypass stay functionally equivalent (both pass dry signal).
  if (GetBypassed()) {
    outputDry(0, originalNFrames);
    return;
  }

  const bool syncToHost = GetParam(kParamSync)->Value() > 0.5;
  const bool transportRunning = mTimeInfo.mTransportIsRunning;
  const bool shouldGateForPause = syncToHost && !GetRenderingOffline();

  mHasTransportState = true;
  mLastTransportRunning = transportRunning;

  if (shouldGateForPause && !transportRunning) {
    outputDry(0, originalNFrames);
    return;
  }

  if (mBufferSize < nFrames) {
    // Host sent a larger block than expected. Clamp to our pre-allocated buffer
    // size and output dry for the remaining tail so the host doesn't read garbage.
    nFrames = mBufferSize;
  }

  // Hoist channel-count branches out of the hot sample loop
  if (nIn >= 2) {
    for (int s = 0; s < nFrames; ++s) {
      mInL[s] = static_cast<float>(inputs[0][s]);
      mInR[s] = static_cast<float>(inputs[1][s]);
    }
  } else if (nIn == 1) {
    for (int s = 0; s < nFrames; ++s) {
      mInL[s] = static_cast<float>(inputs[0][s]);
      mInR[s] = mInL[s];
    }
  } else {
    std::fill(mInL.begin(), mInL.begin() + nFrames, 0.f);
    std::fill(mInR.begin(), mInR.begin() + nFrames, 0.f);
  }

  mInPtrs[0] = mInL.data();
  mInPtrs[1] = mInR.data();
  mOutPtrs[0] = mOutL.data();
  mOutPtrs[1] = mOutR.data();

  mDSP->control();
  mDSP->compute(nFrames, mInPtrs, mOutPtrs);

  // Ramp power gain to target for click-free on/off toggling.
  // The Faust DSP already has an 8 ms internal smooth, but a longer C++ ramp
  // prevents discontinuities when the host bypasses or the user toggles rapidly.
  mTargetPowerGain = GetParam(kParamPower)->Value() > 0.5f ? 1.0f : 0.0f;
  // Dry/Wet: param range 0..100, normalised to 0..1 here.
  mTargetDryWet = static_cast<float>(GetParam(kParamDryWet)->Value() / 100.0);
  const float gainRamp = 0.04f; // ~25 samples to converge
  const float dryWetRamp = 0.02f; // ~50 samples — gentler for smooth knob tweaks

  if (nOut >= 2) {
    for (int s = 0; s < nFrames; ++s) {
      mPowerGain += (mTargetPowerGain - mPowerGain) * gainRamp;
      mDryWet += (mTargetDryWet - mDryWet) * dryWetRamp;
      // Power scales how much of the processed signal is heard; dry/wet then
      // blends that with the unprocessed input.  Net gain is wet = mPowerGain*mDryWet.
      const float wet = mPowerGain * mDryWet;
      const float dry = 1.0f - wet;
      outputs[0][s] = static_cast<sample>(mOutL[s] * wet + inputs[0][s] * dry);
      outputs[1][s] = static_cast<sample>(mOutR[s] * wet + inputs[1][s] * dry);
    }
  } else if (nOut == 1) {
    for (int s = 0; s < nFrames; ++s) {
      mPowerGain += (mTargetPowerGain - mPowerGain) * gainRamp;
      mDryWet += (mTargetDryWet - mDryWet) * dryWetRamp;
      const float wet = mPowerGain * mDryWet;
      const float dry = 1.0f - wet;
      outputs[0][s] = static_cast<sample>(mOutL[s] * wet + inputs[0][s] * dry);
    }
  }

  // Zero-fill any extra output channels beyond what we processed
  const int firstExtraChan = (nOut >= 2) ? 2 : 1;
  for (int c = firstExtraChan; c < nOut; ++c) {
    std::fill(outputs[c], outputs[c] + nFrames, static_cast<sample>(0.0));
  }

  // If we had to clamp nFrames, output dry for the remaining tail frames
  // so the host doesn't read uninitialized memory.
  if (nFrames < originalNFrames) {
    outputDry(nFrames, originalNFrames - nFrames);
  }
}

bool Freeze95::SerializeState(IByteChunk& chunk) const {
  return SerializeParams(chunk);
}

int Freeze95::UnserializeState(const IByteChunk& chunk, int startPos) {
  const int result = UnserializeParams(chunk, startPos);
  SyncParamsToDSP();
  return result;
}
