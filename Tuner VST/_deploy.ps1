$ErrorActionPreference = "Continue"
$built = "D:\Github\VST3 Developments\Tuner VST\build-win\vst3\x64\Release\Tuner.vst3"

$commonDir = "D:\Github\VST3 Developments\common files\vst3"
if (-not (Test-Path $commonDir)) { New-Item -ItemType Directory -Force -Path $commonDir | Out-Null }
try { Copy-Item -Force $built (Join-Path $commonDir "Tuner.vst3"); Write-Host ("OK: " + (Join-Path $commonDir "Tuner.vst3")) -ForegroundColor Green }
catch { Write-Host ("FAIL common-files: " + $_.Exception.Message) -ForegroundColor Red }

$sysVst3 = Join-Path $env:CommonProgramW6432 "VST3"
if (-not (Test-Path $sysVst3)) { New-Item -ItemType Directory -Force -Path $sysVst3 | Out-Null }
try { Copy-Item -Force $built (Join-Path $sysVst3 "Tuner.vst3"); Write-Host ("OK: " + (Join-Path $sysVst3 "Tuner.vst3")) -ForegroundColor Green }
catch { Write-Host ("FAIL system VST3: " + $_.Exception.Message) -ForegroundColor Red }