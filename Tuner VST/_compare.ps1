$dumpbin = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe"
$freeze = "C:\Program Files\Common Files\VST3\Freeze95.vst3"
$tuner  = "C:\Program Files\Common Files\VST3\Tuner.vst3"

Write-Host "=== FREEZE95.vst3 EXPORTS ==="
& $dumpbin /exports $freeze 2>&1 | Select-String -Pattern 'InitDll|ExitDll|GetPluginFactory'

Write-Host ""
Write-Host "=== TUNER.vst3 EXPORTS ==="
& $dumpbin /exports $tuner 2>&1 | Select-String -Pattern 'InitDll|ExitDll|GetPluginFactory'

Write-Host ""
Write-Host "=== FREEZE95.vst3 PE HEADERS (DLL/EXE, machine) ==="
& $dumpbin /headers $freeze 2>&1 | Select-String -Pattern 'machine|FILE HEADER|MAGIC NUMBER|file type' | Select-Object -First 5

Write-Host ""
Write-Host "=== TUNER.vst3 PE HEADERS (DLL/EXE, machine) ==="
& $dumpbin /headers $tuner 2>&1 | Select-String -Pattern 'machine|FILE HEADER|MAGIC NUMBER|file type' | Select-Object -First 5