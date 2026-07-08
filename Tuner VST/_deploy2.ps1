$ErrorActionPreference = "Continue"
$built = "D:\Github\VST3 Developments\Tuner VST\build-win\vst3\x64\Release\Tuner.vst3"

$commonDir = "D:\Github\VST3 Developments\common files\vst3"
if (-not (Test-Path $commonDir)) { New-Item -ItemType Directory -Force -Path $commonDir | Out-Null }
try { Copy-Item -Force $built (Join-Path $commonDir "Tuner.vst3"); Write-Host ("OK common: " + (Join-Path $commonDir "Tuner.vst3")) -ForegroundColor Green }
catch { Write-Host ("FAIL common: " + $_.Exception.Message) -ForegroundColor Red }

$sysVst3 = Join-Path $env:CommonProgramW6432 "VST3"
$target  = Join-Path $sysVst3 "Tuner.vst3"
if (-not (Test-Path $sysVst3)) { New-Item -ItemType Directory -Force -Path $sysVst3 | Out-Null }
try {
    Copy-Item -Force $built $target
    Write-Host ("OK system: " + $target) -ForegroundColor Green
} catch {
    Write-Host ("Direct copy failed: " + $_.Exception.Message) -ForegroundColor Yellow
    $bak = "$target.bak"
    try {
        if (Test-Path $bak) { Remove-Item -Force $bak -ErrorAction SilentlyContinue }
        Move-Item $target $bak -ErrorAction Stop
        Copy-Item -Force $built $target
        Write-Host ("OK system (move+replace): " + $target) -ForegroundColor Green
    } catch {
        Write-Host ("FAIL system: " + $_.Exception.Message) -ForegroundColor Red
    }
}