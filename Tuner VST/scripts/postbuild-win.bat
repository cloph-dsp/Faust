@echo off
REM postbuild-win.bat - wraps the built DLL into a VST3 bundle and deploys
REM the bundle to local build output + the system VST3 folder.
REM
REM iPlug2's IPlugEffect-vst3.vcxproj emits a single-file DLL named with a
REM .vst3 extension.  The VST3 spec REQUIRES a bundle (folder) layout, so
REM we have to call create_bundle.bat which moves the DLL into:
REM   <Name>.vst3/Contents/x86_64-win/<Name>.vst3
REM and copies it next to any resources from <SolutionDir>/resources/.
REM
REM Without this step the host's VST3 scanner ignores the plugin entirely --
REM Bitwig, Reaper, Cubase, Live all expect the bundle form.
REM
REM Args (per iPlug2 convention):
REM   %1  = FORMAT (vst3)
REM   %2  = NAME (Tuner)
REM   %3  = PLATFORM (x64)
REM   %4  = COPY_VST2 (0/1)
REM   %5  = BUILT_BINARY path  (the single-file DLL)
REM   %6  = VST2_32_PATH
REM   %7  = VST2_64_PATH
REM   %8  = VST3_32_PATH
REM   %9  = VST3_64_PATH
REM   shift /6 then continue.
REM   ... then AAX_32_PATH, AAX_64_PATH, BUILD_DIR, VST_ICON, AAX_ICON,
REM   CREATE_BUNDLE_SCRIPT.

setlocal
set FORMAT=%~1
set NAME=%~2
set PLATFORM=%~3
set COPY_VST2=%~4
set BUILT_BINARY=%~5
set VST2_32_PATH=%~6
set VST2_64_PATH=%~7
set VST3_32_PATH=%~8
set VST3_64_PATH=%~9
shift /6
set AAX_32_PATH=%~4
set AAX_64_PATH=%~5
set BUILD_DIR=%~6
set VST_ICON=%~7
set AAX_ICON=%~8
set CREATE_BUNDLE_SCRIPT=%~9

if "%PLATFORM%"=="x64" (
  if "%FORMAT%"=="vst3" (
    REM Invoke iPlug2's bundle wrapper.  This is THE critical step: it
    REM converts build-win/vst3/x64/Release/Tuner.vst3 (single DLL) into
    REM build-win/vst3/x64/Release/Tuner.vst3/Contents/x86_64-win/Tuner.vst3
    REM (proper VST3 bundle).
    if exist "%CREATE_BUNDLE_SCRIPT%" (
      call "%CREATE_BUNDLE_SCRIPT%" "%NAME%" "%BUILT_BINARY%" "%BUILD_DIR%" "%VST3_64_PATH%"
    ) else (
      echo WARNING: create_bundle.bat not found at "%CREATE_BUNDLE_SCRIPT%"
      echo          Falling back to single-file copy -- VST3 hosts will likely
      echo          refuse to load this plugin.
      if exist "%BUILD_DIR%\%NAME%.vst3" del /f /q "%BUILD_DIR%\%NAME%.vst3"
      if exist "%BUILD_DIR%\%NAME%.vst3\" rd /s /q "%BUILD_DIR%\%NAME%.vst3"
      copy /y "%BUILT_BINARY%" "%BUILD_DIR%\%NAME%.vst3"
    )
  )
)

endlocal
exit /b 0