@echo off
REM Post-build script for Bronze Noise - single .vst3 file, no bundle
set FORMAT=%~1
set NAME=%~2
set PLATFORM=%~3
set COPY_VST2=%~4
set BUILT_BINARY=%~5
set VST2_32_PATH=%~6
set VST2_64_PATH=%~7
set VST3_32_PATH=%~8
set VST3_64_PATH=%~9
shift
shift
shift
shift
shift
shift
set AAX_32_PATH=%~4
set AAX_64_PATH=%~5
set BUILD_DIR=%~6
set VST_ICON=%~7
set AAX_ICON=%~8
set CREATE_BUNDLE_SCRIPT=%~9

if "%PLATFORM%"=="x64" (
  if ".vst3"=="%FORMAT%" (
    if exist "%BUILD_DIR%\%NAME%.vst3\" rd /s /q "%BUILD_DIR%\%NAME%.vst3"
    copy /y "%BUILT_BINARY%" "%BUILD_DIR%\%NAME%.vst3"
    if exist "%VST3_64_PATH%\" (
      if exist "%VST3_64_PATH%\%NAME%.vst3\" rd /s /q "%VST3_64_PATH%\%NAME%.vst3"
      copy /y "%BUILT_BINARY%" "%VST3_64_PATH%\%NAME%.vst3"
    )
  )
)
exit /b 0
