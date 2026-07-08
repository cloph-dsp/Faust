@echo off
REM prebuild-win.bat - runs before compilation.  We don't need to do anything
REM because TunerDSP_gen.h is committed alongside TunerDSP.dsp, but the
REM iPlug2 .vcxproj wires this script in via the win.props file, so it has
REM to exist and return 0.
exit /b 0