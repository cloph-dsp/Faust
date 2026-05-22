@echo off

set FORMAT=%1
set NAME=%2
set PLATFORM=%3
set COPY_VST2=%4
set BUILT_BINARY=%5
set VST2_32_PATH=%6
set VST2_64_PATH=%7
set VST3_32_PATH=%8
set VST3_64_PATH=%9
shift
shift
shift
shift
shift
set AAX_32_PATH=%4
set AAX_64_PATH=%5
set BUILD_DIR=%6
set VST_ICON=%7
set AAX_ICON=%8
set CREATE_BUNDLE_SCRIPT=%9

if %PLATFORM% == "x64" (
  if %FORMAT% == ".vst3" (
    call %CREATE_BUNDLE_SCRIPT% %BUILD_DIR%\%NAME%.vst3 %VST_ICON% %FORMAT%
    copy /y %BUILT_BINARY% %BUILD_DIR%\%NAME%.vst3\Contents\x86_64-win
    if exist %VST3_64_PATH% (
      call %CREATE_BUNDLE_SCRIPT% %VST3_64_PATH%\%NAME%.vst3 %VST_ICON% %FORMAT%
      xcopy /E /H /Y %BUILD_DIR%\%NAME%.vst3\Contents\* %VST3_64_PATH%\%NAME%.vst3\Contents\
    )
  )
)