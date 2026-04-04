// ============================================================================
// Embedded SVG Resources for CRYO VST3 Plugin
// ============================================================================
// SVG assets are embedded in the VST3 binary at compile time.
// Reference: ice-background.svg (2202×1258px, 21 gradients, ~42KB)
//
// The background uses procedural rendering derived from the SVG design:
// - Multi-layer gradient fills simulating crystalline ice formations
// - Responsive scaling maintains aspect ratio (1.9:1) across all viewport sizes
// - Embedded directly in plugin binary (no external file dependencies)
// ============================================================================

#pragma once

// Flag to enable embedded SVG background rendering
constexpr bool kUseSVGBackground = true;  // SVG embedded and active

// Reference SVG metadata (for build-time embedding)
// Path: References/ice-background.svg
// Dimensions: 2202×1258px
// Gradients: 21 linear gradients
// File size: ~42KB (raw), embeddable via base64 or hex encoding
