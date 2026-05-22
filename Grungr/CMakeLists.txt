cmake_minimum_required(VERSION 3.14)
project(Grungr VERSION 2.6.0)

# Discover IPLUG2_DIR if not already set (for independent builds)
if(NOT DEFINED IPLUG2_DIR)
  set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "iPlug2 root directory")
endif()

# Include iPlug2 CMake configuration
include(${IPLUG2_DIR}/iPlug2.cmake)

# We deploy manually after copying custom resources to ensure the installed
# VST3 bundle always contains the SVG/font assets.
set(IPLUG_DEPLOY_PLUGINS OFF CACHE BOOL "Disable default iPlug deploy" FORCE)

# IGraphics backend/renderer can be set via command line:
#   cmake -DIGRAPHICS_BACKEND=SKIA -DIGRAPHICS_RENDERER=GL3 ..
# Defaults: NANOVG/METAL on macOS, NANOVG/GL2 on Windows
# See iPlug2/Scripts/cmake/IGraphics.cmake for all options

find_package(iPlug2 REQUIRED)

find_path(FAUST_INCLUDE_DIR
  faust/dsp/dsp.h
  HINTS
    "$ENV{FAUST_INCLUDE_DIR}"
    "$ENV{FAUST_HOME}/include"
    "C:/Program Files/Faust/include"
)

if(NOT FAUST_INCLUDE_DIR)
  message(FATAL_ERROR "Faust headers not found. Set FAUST_INCLUDE_DIR to your Faust include directory.")
endif()

include_directories(${FAUST_INCLUDE_DIR})

set(DOD_GRUNGE_RESOURCES
  resources/GrungrFaceplate.svg
  resources/GrungrFaceplateRefined.svg
  resources/knob-cropped.svg
  resources/NoMoreHeroV2-Regular.ttf
  resources/Future2097Eroded-Regular.otf
  resources/fonts/Roboto-Regular.ttf
)

set(DOD_GRUNGE_SOURCES
  Grungr.cpp
  Grungr.h
  GrungrIPlug2UI.cpp
  GrungrIPlug2UI.h
  DSP/GrungrFaustDSP.h
  resources/resource.h
)

if(WIN32)
  list(APPEND DOD_GRUNGE_SOURCES resources/main.rc)

  # main.rc references files by short names (e.g. Roboto-Regular.ttf)
  # so RC needs include search paths for fonts/images/resources.
  set_source_files_properties(resources/main.rc PROPERTIES
    COMPILE_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts\" /I\"${CMAKE_CURRENT_SOURCE_DIR}/resources/img\" /I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\""
  )
endif()

# Build VST3 only for this project.
iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    ${DOD_GRUNGE_SOURCES}
  RESOURCES
    ${DOD_GRUNGE_RESOURCES}
  FORMATS
    VST3
)

if(WIN32 AND TARGET ${PROJECT_NAME}-vst3)
  foreach(_res ${DOD_GRUNGE_RESOURCES})
    add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}-vst3>/../Resources/"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${_res}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}-vst3>/../Resources/"
    )
  endforeach()

  set(DOD_GRUNGE_VST3_DEPLOY_DIR "$ENV{LOCALAPPDATA}/Programs/Common/VST3")

  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DOD_GRUNGE_VST3_DEPLOY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${DOD_GRUNGE_VST3_DEPLOY_DIR}/${PROJECT_NAME}.vst3"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_BINARY_DIR}/out/${PROJECT_NAME}.vst3"
      "${DOD_GRUNGE_VST3_DEPLOY_DIR}/${PROJECT_NAME}.vst3"
  )
endif()
