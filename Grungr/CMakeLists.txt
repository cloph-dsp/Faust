cmake_minimum_required(VERSION 3.14)
project(Grungr VERSION 2.7.0)

# This is an optional developer build. The maintained release build uses the
# Visual Studio projects in projects/, but keeping this target accurate makes
# local configuration useful and deliberately avoids deploying anywhere.
if(NOT DEFINED IPLUG2_DIR)
  set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../iPlug2" CACHE PATH "iPlug2 root directory")
endif()

include(${IPLUG2_DIR}/iPlug2.cmake)
find_package(iPlug2 REQUIRED)

set(GRUNGR_RESOURCES
  resources/GrungrFaceplate.svg
  resources/knob-cropped.svg
  resources/cloph-logo.svg
  resources/logo-grungr.svg
  resources/stomp-sole.svg
  resources/NoMoreHeroV2-Regular.ttf
  resources/Future2097Eroded-Regular.otf
  resources/boxpot1.ttf
  resources/Roboto-Regular.ttf
)

set(GRUNGR_SOURCES
  Grungr.cpp
  Grungr.h
  GrungrIPlug2UI.cpp
  GrungrIPlug2UI.h
  GrungrFaustCompat.h
  DSP/GrungrFaustDSP.h
  resources/resource.h
)

if(WIN32)
  list(APPEND GRUNGR_SOURCES resources/main.rc)
endif()

iplug_add_plugin(${PROJECT_NAME}
  SOURCES ${GRUNGR_SOURCES}
  RESOURCES ${GRUNGR_RESOURCES}
  FORMATS VST3
)
