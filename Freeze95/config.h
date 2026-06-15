#pragma once

#define PLUG_NAME "Freeze95"
#define PLUG_MFR "cloph"
#define PLUG_VERSION_HEX 0x00010300
#define PLUG_VERSION_STR "1.3.0"
#define PLUG_UNIQUE_ID 'F95G'
#define PLUG_MFR_ID 'Clph'
#define PLUG_URL_STR "https://cloph.gumroad.com"
#define PLUG_EMAIL_STR "pedrocastromusic@gmail.com"
#define PLUG_COPYRIGHT_STR "Copyright 2026 cloph"
#define PLUG_CLASS_NAME Freeze95

#define BUNDLE_NAME "Freeze95"
#define BUNDLE_MFR "cloph-dsp"
#define BUNDLE_DOMAIN "com"

#define SHARED_RESOURCES_SUBPATH "Freeze95"

#define PLUG_CHANNEL_IO "2-2"

#define PLUG_LATENCY 0
// Memory: ~48 MB static delay buffer allocation (6x float[2097152] for 3-band stutter processing)
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 840
#define PLUG_HEIGHT 306
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 1
#define PLUG_MIN_WIDTH 420
#define PLUG_MIN_HEIGHT 153
#define PLUG_MAX_WIDTH 1680
#define PLUG_MAX_HEIGHT 612

#define AUV2_ENTRY Freeze95_Entry
#define AUV2_ENTRY_STR "Freeze95_Entry"
#define AUV2_FACTORY Freeze95_Factory
#define AUV2_VIEW_CLASS Freeze95_View
#define AUV2_VIEW_CLASS_STR "Freeze95_View"

#define AAX_TYPE_IDS 'F951', 'F952'
#define AAX_TYPE_IDS_AUDIOSUITE 'F95A', 'F95B'
#define AAX_PLUG_MFR_STR "cloph"
#define AAX_PLUG_NAME_STR "Freeze95\nF95G"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 0

#define VST3_SUBCATEGORY "Fx|Modulation"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64

#define TERTIARY_UI_FN "citaro_zij_ds.ttf"
#define UTILITY_UI_FN "Trigram-Light.ttf"
#define GOOD_OLD_DOS_FN "Freeze95DOS.ttf"
#define GOOD_OLD_DOS_RES "FREEZE95DOS"
#define MONITOR_BG_ON_FN "monitor-bg-on.svg"
#define MONITOR_BG_OFF_FN "monitor-bg-off.svg"
#define BRAND_LOGO_FACE_FN "cloph-logo-face.svg"
#define BRAND_LOGO_HIGHLIGHT_FN "cloph-logo-highlight.svg"
#define BRAND_LOGO_SHADOW_FN "cloph-logo-shadow.svg"
