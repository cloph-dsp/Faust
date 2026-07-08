#pragma once

#define PLUG_NAME "L-R8"
#define PLUG_MFR "cloph"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'LR8S'
#define PLUG_MFR_ID 'Clph'
#define PLUG_URL_STR "https://cloph.audio"
#define PLUG_EMAIL_STR "pedrocastromusic@gmail.com"
#define PLUG_COPYRIGHT_STR "Copyright 2026 cloph"
#define PLUG_CLASS_NAME LR8

#define BUNDLE_NAME "L-R8"
#define BUNDLE_MFR "cloph"
#define BUNDLE_DOMAIN "com"

#define SHARED_RESOURCES_SUBPATH "L-R8"

#define PLUG_CHANNEL_IO "2-2"

#define PLUG_LATENCY 2048
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 780
#define PLUG_HEIGHT 420
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 1
#define PLUG_MIN_WIDTH 390
#define PLUG_MIN_HEIGHT 210
#define PLUG_MAX_WIDTH 1560
#define PLUG_MAX_HEIGHT 840

#define KNOB_CAP_FN "knob-cap.svg"

#define AUV2_ENTRY LR8_Entry
#define AUV2_ENTRY_STR "LR8_Entry"
#define AUV2_FACTORY LR8_Factory
#define AUV2_VIEW_CLASS LR8_View
#define AUV2_VIEW_CLASS_STR "LR8_View"

#define AAX_TYPE_IDS 'LR81', 'LR82'
#define AAX_TYPE_IDS_AUDIOSUITE 'LR8A', 'LR8B'
#define AAX_PLUG_MFR_STR "cloph"
#define AAX_PLUG_NAME_STR "L-R8\nLR8S"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 0

#define VST3_SUBCATEGORY "Fx|Modulation|Stereo"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64
