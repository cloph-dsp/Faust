$dumpbin = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe"

Write-Host "=== FREEZE95 .rsrc (raw) ==="
& $dumpbin /rawdata:1 C:\Program Files\Common Files\VST3\Freeze95.vst3 2>&1 | Select-String -Pattern 'VS_VERSION_INFO|FileVersion|ProductVersion|StringFileInfo' | Select-Object -First 10

Write-Host ""
Write-Host "=== TUNER .rsrc (raw) ==="
& $dumpbin /rawdata:1 C:\Program Files\Common Files\VST3\Tuner.vst3 2>&1 | Select-String -Pattern 'VS_VERSION_INFO|FileVersion|ProductVersion|StringFileInfo' | Select-Object -First 10