$dumpbin = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe"
$freeze = "C:\Program Files\Common Files\VST3\Freeze95.vst3"
$tuner  = "C:\Program Files\Common Files\VST3\Tuner.vst3"

Write-Host "=== FREEZE95 .rsrc SECTION ==="
& $dumpbin /headers $freeze 2>&1 | Select-String -Pattern '\.rsrc|VERSION|VS_VERSION_INFO' | Select-Object -First 10

Write-Host ""
Write-Host "=== TUNER .rsrc SECTION ==="
& $dumpbin /headers $tuner 2>&1 | Select-String -Pattern '\.rsrc|VERSION|VS_VERSION_INFO' | Select-Object -First 10

Write-Host ""
Write-Host "=== FREEZE95 VS_VERSION_INFO (versioninfo) ==="
& $dumpbin /versioninfo $freeze 2>&1 | Select-Object -First 30

Write-Host ""
Write-Host "=== TUNER VS_VERSION_INFO ==="
& $dumpbin /versioninfo $tuner 2>&1 | Select-Object -First 30

Write-Host ""
Write-Host "=== FREEZE95 SECTION SIZES ==="
& $dumpbin /headers $freeze 2>&1 | Select-String -Pattern '\.\w+ ' | Select-Object -First 15

Write-Host ""
Write-Host "=== TUNER SECTION SIZES ==="
& $dumpbin /headers $tuner 2>&1 | Select-String -Pattern '\.\w+ ' | Select-Object -First 15