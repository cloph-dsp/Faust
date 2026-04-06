@echo off
REM Post-build resources script for Freeze95
REM SVGs are now embedded in the DLL via main.rc.
REM This script only copies fonts into Contents/Resources as a fallback.
REM Parameters: %1=VST3_BUILD_BUNDLE %2=VST3_INSTALL_BUNDLE %3=RESOURCES_DIR %4=FONT_FILE

set BUNDLE=%~1
set INSTALL_BUNDLE=%~2
set RES_SRC=%~3
set FONT_FILE=%~4

REM Ensure Resources folder exists inside build bundle
if not exist "%BUNDLE%\Contents\Resources" mkdir "%BUNDLE%\Contents\Resources"

REM Copy font files only (SVGs are embedded in the DLL)
for %%F in ("%RES_SRC%\*.ttf") do (
  copy /y "%%F" "%BUNDLE%\Contents\Resources\" >nul
)
if exist "%FONT_FILE%" copy /y "%FONT_FILE%" "%BUNDLE%\Contents\Resources\" >nul

exit /b 0
