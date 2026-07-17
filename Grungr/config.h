#define PLUG_NAME "Grungr"
#define PLUG_MFR "cloph"
#define PLUG_VERSION_HEX 0x00020700
#define PLUG_VERSION_STR "2.7.0"
#define PLUG_UNIQUE_ID 'GRNR'
#define PLUG_MFR_ID 'CLPH'
#define PLUG_URL_STR "https://cloph.gumroad.com"
#define PLUG_EMAIL_STR "pedrocastromusic@gmail.com"
#define PLUG_COPYRIGHT_STR "Copyright 2026 cloph"
#define PLUG_CLASS_NAME Grungr

#define BUNDLE_NAME "Grungr"
#define BUNDLE_MFR "cloph"
#define BUNDLE_DOMAIN "com"

#define SHARED_RESOURCES_SUBPATH "Grungr"

#define PLUG_CHANNEL_IO "1-1 2-2"

// The signal path uses only zero-latency filters and saturation. Keeping this
// at zero avoids inaccurate PDC for hosts and parallel effect chains.
#define PLUG_LATENCY 0
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 480
#define PLUG_HEIGHT 811
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 1
#define PLUG_MIN_WIDTH 360
#define PLUG_MIN_HEIGHT 608
#define PLUG_MAX_WIDTH 960
#define PLUG_MAX_HEIGHT 1621

#define AUV2_ENTRY Grungr_Entry
#define AUV2_ENTRY_STR "Grungr_Entry"
#define AUV2_FACTORY Grungr_Factory
#define AUV2_VIEW_CLASS Grungr_View
#define AUV2_VIEW_CLASS_STR "Grungr_View"

#define AAX_TYPE_IDS 'GR01', 'GR02'
#define AAX_TYPE_IDS_AUDIOSUITE 'GR11', 'GR12'
#define AAX_PLUG_MFR_STR "clph"
#define AAX_PLUG_NAME_STR "Grungr\nGRNR"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 1

#define VST3_SUBCATEGORY "Fx"

#define CLAP_MANUAL_URL "https://cloph.gumroad.com"
#define CLAP_SUPPORT_URL "mailto:pedrocastromusic@gmail.com"
#define CLAP_DESCRIPTION "Grungr analog-style drive effect with RAW, MOD, and BASS voicings"
#define CLAP_FEATURES "audio-effect"//, "utility"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64

#define GRUNGE_BG_SVG_FN "GrungrFaceplate.svg"
#define POWERBUTTON_SVG_FN "knob-cropped.svg"
#define cloph_logo_svg_fn "cloph-logo.svg"
#define GRUNGR_LOGO_SVG_FN "logo-grungr.svg"
#define NOMOREHERO_FN "NoMoreHeroV2-Regular.ttf"
#define FUTUREERODED_FN "Future2097Eroded-Regular.otf"
#define ROBOTO_FN "Roboto-Regular.ttf"
#define BOXPOT_FN "boxpot1.ttf"
#define STOMP_SOLE_SVG_FN "stomp-sole.svg"
