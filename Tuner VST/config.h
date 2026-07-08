#pragma once

// =============================================================================
// config.h - iPlug2 plugin identity
//
// 4-char manufacturer + plugin codes MUST be unique across all VST/AU plugins
// in the world.  CLPH is the maker code from AGENTS.md / L-R8 etc.  TnR1 is
// the Tuner code, distinct from L-R8 (LR81), Freeze95 (F951) and the others.
// =============================================================================

#define PLUG_NAME "Tuner"
#define PLUG_MFR "CLOPH"
#define PLUG_VERSION_HEX      0x00010100
#define PLUG_VERSION_STR "1.1.0"
#define PLUG_UNIQUE_ID        'TnR1'
#define PLUG_MFR_ID           'CLPH'

// I/O.  A tuner is stereo input (we don't have to be, but matches the
// source Cmajor patch which was stereo-in / stereo-out pass-through).
#define PLUG_CHANNEL_IO "2-2"
#define PLUG_LATENCY          0

// Plugin class / behaviour.
#define PLUG_TYPE             0
#define PLUG_DOES_MIDI_IN     0
#define PLUG_DOES_MIDI_OUT    0
#define PLUG_DOES_MPE         0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI           1

// UI dimensions.  Taller than before (was 260) to give the tuning
// meter and controls breathing room.  540 wide stays comfortable in any DAW.
#define PLUG_WIDTH            540
#define PLUG_HEIGHT           300
#define PLUG_FPS              30

// Host resize support.  We don't shrink/grow dynamically -- fixed size keeps
// the UI layout simple -- but we report the canonical size.
#define PLUG_HOST_RESIZE      0
#define PLUG_MIN_WIDTH        PLUG_WIDTH
#define PLUG_MIN_HEIGHT       PLUG_HEIGHT
#define PLUG_MAX_WIDTH        PLUG_WIDTH
#define PLUG_MAX_HEIGHT       PLUG_HEIGHT

// AUv2 entry points (compiled by iPlug2).  The first three are required
// by IPlug_include_in_plug_hdr.h -- without AUV2_FACTORY and AUV2_VIEW_CLASS
// the AU build fails with `#error AUV2_FACTORY not defined`.  The naming
// convention matches Freeze95's config.h: `<Name>_Entry`, `<Name>_Factory`,
// `<Name>_View`.  PLUG_ENTRY above is the iPlug class symbol, not the
// AUv2 component entry, so we duplicate the suffix pattern here.
#define AUV2_ENTRY            Tuner_Entry
#define AUV2_ENTRY_STR "Tuner_Entry"
#define AUV2_FACTORY          Tuner_Factory
#define AUV2_VIEW_CLASS       Tuner_View
#define AUV2_VIEW_CLASS_STR "Tuner_View"

// AU AudioComponent identifiers.  Mirrors Freeze95's AAX/PLUG_NAME macros.
// `manufacturer` is a 4-char code that the system registry keys on; we
// reuse `CLPH` (the iPlug MFR id) so AudioComponents matches what the host
// sees when scanning VST3 bundles.  `subtype` is the per-plugin 4-char
// code, distinct from any other CLPH plugin.  The CI macOS job rewrites
// the same keys inside the .component/Contents/Info.plist at build time
// (the iPlug2 template uses placeholder values).
#define AUV2_MANUFACTURER "Clph"
#define AUV2_SUBTYPE "TnR1"
#define AUV2_DESCRIPTION "Tuner"

// AAX identity codes (split to avoid commas in RC preprocessor).
#define AAX_TYPE_ID_1         'TnR1'
#define AAX_TYPE_ID_2         'TnR2'
#define AAX_TYPE_IDS          AAX_TYPE_ID_1, AAX_TYPE_ID_2
#define AAX_TYPE_IDS_AUDIOSUITE_1 'TnR3'
#define AAX_TYPE_IDS_AUDIOSUITE_2 'TnR4'
#define AAX_TYPE_IDS_AUDIOSUITE AAX_TYPE_IDS_AUDIOSUITE_1, AAX_TYPE_IDS_AUDIOSUITE_2
#define AAX_PLUGIN_ID         'CLPT'

// VST3 category.  Fx|Tools is the right slot for a tuner.
#define VST3_SUBCATEGORY "Fx|Tools"

// iPlug2 internals.
#define APP_NUM_CHANNELS      2
#define APP_N_VECTOR_WAIT     0
#define APP_MULT              1
#define APP_SIGNAL_VECTOR_SIZE 64

// Plugin description string surfaced to the host.
#define PLUG_DESCRIPTION "High-precision YIN+MPM chromatic tuner with 4-point Neville interpolation and adaptive asymmetric smoothing."

// Entry point symbol (matches the AUV2_ENTRY above).
#define PLUG_ENTRY            Tuner

// Plugin class name -- iPlug2's IPlug_include_in_plug_hdr.h needs this
// to expand PLUG_*_ENTRY macros.
#define PLUG_CLASS_NAME       Tuner

// Bundle name -- used in CFBundleIdentifier on macOS / iOS.
#define BUNDLE_NAME "Tuner"
#define BUNDLE_MFR "CLOPH"
#define BUNDLE_DOMAIN "com.cloph"

#define PLUG_SHARED_RESOURCES 0
#define PLUG_URL_STR          ""
#define PLUG_EMAIL_STR        ""
#define PLUG_COPYRIGHT_STR "Copyright (C) CLOPH"
#define SHARED_RESOURCES_SUBPATH PLUG_NAME

// Resource compiler substitution.  resources/main.rc contains lines like
//   TEXASLED_FN  TTF  TEXASLED_FN
// which expand to:
//   TEXASLED.TTF  TTF  TEXASLED.TTF
// declaring a TTF resource with NAME="TEXASLED.TTF" pointing at the file
// TEXASLED.TTF in this resources/ directory.  The Tuner UI calls
// g.LoadFont("TexasLED", TEXASLED_FN) -- iPlug2's LoadPlatformFont calls
// FindResource(0, "TEXASLED.TTF", "TTF") which finds the embedded
// resource inside the .vst3 single-file DLL (no sidecar copy needed).
//
// Same pattern for the CLOPH brand logo SVG.  Mirrors Freeze95's
// resources/main.rc exactly.
#define TEXASLED_FN  "TEXASLED.TTF"
#define CLOPH_LOGO_FN "CLOPH.svg"
