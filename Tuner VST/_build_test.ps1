$vcvars = & "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" 2>&1
$env:VSCMD_ARG_TGT_ARCH = "x64"
# Source vcvars64 and capture the env it sets.
$cmd = "`"$PSCommandPath`" -- bypass -- norestart"
# Easiest: just call cl.exe with full path.
$cl = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe"
$link = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\link.exe"
$msvc = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\lib\x64"
$ucrt = "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64"
$um = "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"

& $cl /nologo /EHsc /O2 /std:c++17 /Fe_vst3_test.exe /Fo_vst3_test.obj `
    /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" `
    /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" `
    /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" `
    /I "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include" `
    "D:\Github\VST3 Developments\Tuner VST\_vst3_test.cpp" `
    /link /LIBPATH:"$msvc" /LIBPATH:"$ucrt" /LIBPATH:"$um" kernel32.lib user32.lib

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "=== Running test on TUNER ==="
    & ".\vst3_test.exe" "C:\Program Files\Common Files\VST3\Tuner.vst3"
    Write-Host ""
    Write-Host "=== Running test on FREEZE95 (control) ==="
    & ".\vst3_test.exe" "C:\Program Files\Common Files\VST3\Freeze95.vst3"
}