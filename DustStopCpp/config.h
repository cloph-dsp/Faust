#define PLUG_NAME "DustStop"
#define PLUG_MFR "CLOPH"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'Dstp'
#define PLUG_MFR_ID 'Clph'
#define PLUG_URL_STR "https://cloph.audio"
#define PLUG_EMAIL_STR "hello@cloph.audio"
#define PLUG_COPYRIGHT_STR "Copyright 2026 CLOPH"
#define PLUG_CLASS_NAME DustStop

#define BUNDLE_NAME "DustStop"
#define BUNDLE_MFR "CLOPH"
#define BUNDLE_DOMAIN "audio"

#define SHARED_RESOURCES_SUBPATH "DustStop"

#define PLUG_CHANNEL_IO "2-2"

#define PLUG_LATENCY 0
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 600
#define PLUG_HEIGHT 400
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0

#define AUV2_ENTRY DustStop_Entry
#define AUV2_ENTRY_STR "DustStop_Entry"
#define AUV2_FACTORY DustStop_Factory
#define AUV2_VIEW_CLASS DustStop_View
#define AUV2_VIEW_CLASS_STR "DustStop_View"

#define AAX_TYPE_IDS 'DSt1', 'DSt2'
#define AAX_TYPE_IDS_AUDIOSUITE 'DSa1', 'DSa2'
#define AAX_PLUG_MFR_STR "CLOPH"
#define AAX_PLUG_NAME_STR "DustStop\nDSt1"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 0

#define VST3_SUBCATEGORY "Fx|Tools|Filter"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64
