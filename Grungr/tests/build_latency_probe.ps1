# build_latency_probe.ps1 — ponytail: minimal, mirrors build_tests.ps1 structure
$faustInclude = "C:\Program Files\Faust\include"
$doctestDir   = Split-Path -Parent $PSCommandPath
$dspDir       = Join-Path $doctestDir "..\DSP"
$sourceFile   = Join-Path $doctestDir "latency_probe.cpp"
$outputExe    = Join-Path $doctestDir "latency_probe.exe"

$vcvars = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if (Test-Path $vcvars) {
    cmd /c "`"$vcvars`" && cl /EHsc /std:c++17 /I`"$faustInclude`" /I`"$dspDir`" `"$sourceFile`" /Fe:`"$outputExe`" 2>&1"
} else {
    Write-Error "vcvars64.bat not found at expected location"
    exit 1
}

if ($LASTEXITCODE -eq 0 -and (Test-Path $outputExe)) {
    Write-Host "=== Latency probe ===" -ForegroundColor Cyan
    & $outputExe
} else {
    Write-Error "Build failed"
    exit 1
}
