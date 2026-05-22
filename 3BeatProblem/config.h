#define PLUG_NAME "3BeatProblem"
#define PLUG_MFR "cloph"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID '3BPM'
#define PLUG_MFR_ID 'Clph'
#define PLUG_URL_STR "https://cloph.audio"
#define PLUG_EMAIL_STR "hello@cloph.audio"
#define PLUG_COPYRIGHT_STR "Copyright 2026 cloph"
#define PLUG_CLASS_NAME ThreeBeatProblem

#define BUNDLE_NAME "3BeatProblem"
#define BUNDLE_MFR "cloph"
#define BUNDLE_DOMAIN "com"

#define SHARED_RESOURCES_SUBPATH "3BeatProblem"

#define PLUG_CHANNEL_IO "0-2"

#define PLUG_LATENCY 0
#define PLUG_TYPE 1
#define PLUG_DOES_MIDI_IN 1
#define PLUG_DOES_MIDI_OUT 1
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 680
#define PLUG_HEIGHT 480
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0

#define AUV2_ENTRY ThreeBeatProblem_Entry
#define AUV2_ENTRY_STR "ThreeBeatProblem_Entry"
#define AUV2_FACTORY ThreeBeatProblem_Factory
#define AUV2_VIEW_CLASS ThreeBeatProblem_View
#define AUV2_VIEW_CLASS_STR "ThreeBeatProblem_View"

#define AAX_TYPE_IDS '3BP1', '3BP2'
#define AAX_TYPE_IDS_AUDIOSUITE '3BA1', '3BA2'
#define AAX_PLUG_MFR_STR "cloph"
#define AAX_PLUG_NAME_STR "3BeatProblem\n3BPM"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 0

#define VST3_SUBCATEGORY "Instrument|Synth"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64