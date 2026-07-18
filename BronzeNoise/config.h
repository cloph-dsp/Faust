#define PLUG_NAME "Bronze Noise"
#define PLUG_MFR "CLOPH"
#define PLUG_VERSION_HEX 0x00010100
#define PLUG_VERSION_STR "1.1.0"
#define PLUG_UNIQUE_ID 'BrNz'
#define PLUG_MFR_ID 'Clph'
#define PLUG_URL_STR "https://cloph.audio"
#define PLUG_EMAIL_STR "hello@cloph.audio"
#define PLUG_COPYRIGHT_STR "Copyright 2026 CLOPH"
#define PLUG_CLASS_NAME BronzeNoise

#define BUNDLE_NAME "BronzeNoise"
#define BUNDLE_MFR "CLOPH"
#define BUNDLE_DOMAIN "audio"

#define SHARED_RESOURCES_SUBPATH "BronzeNoise"

#define PLUG_CHANNEL_IO "1-1 2-2"

#define PLUG_LATENCY 4095
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 1080
#define PLUG_HEIGHT 720
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 1
#define PLUG_MIN_WIDTH 756
#define PLUG_MIN_HEIGHT 504
#define PLUG_MAX_WIDTH 1600
#define PLUG_MAX_HEIGHT 1120

#define AUV2_ENTRY BronzeNoise_Entry
#define AUV2_ENTRY_STR "BronzeNoise_Entry"
#define AUV2_FACTORY BronzeNoise_Factory
#define AUV2_VIEW_CLASS BronzeNoise_View
#define AUV2_VIEW_CLASS_STR "BronzeNoise_View"

#define AAX_TYPE_IDS 'BNF1', 'BNF2'
#define AAX_TYPE_IDS_AUDIOSUITE 'BNA1', 'BNA2'
#define AAX_PLUG_MFR_STR "CLOPH"
#define AAX_PLUG_NAME_STR "Bronze Noise\nBNZ1"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 1

#define VST3_SUBCATEGORY "Fx|EQ|Tools"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64

// Embedded IronSans font for the plugin title
#define IRON_SANS_FN "IronSans.ttf"

// Embedded Kenyan Coffee Rg font resource for consistent cross-platform rendering
// TTF (not OTF) — GDI on Windows rejects CFF-based OTF files via AddFontMemResourceEx
#define BODY_FONT_FN "KenyanCoffeeRg.ttf"

// Roboto-Regular is iPlug2's DEFAULT_FONT: any control that draws text without
// an explicit font ID resolves to it in NanoVG. Embed + register it first so
// a missing-font handle can never crash the first draw (Grungr/Freeze95 pattern).
#define ROBOTO_FN "Roboto-Regular.ttf"

// Embedded knob SVG (Freeze95 pattern: macro name == resource name == filename so iPlug2 can locate via kWinBinary)
#define KNOB_SVG_FN "knob.svg"

// Embedded iPlug2 logo SVG, drawn opposite the title (right side of title bar)
#define CLOPH_LOGO_FN "cloph_logo.svg"

