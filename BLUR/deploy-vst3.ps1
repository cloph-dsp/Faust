# Deploy SpectralProtocol VST3 to system folder
# Run this script after closing your DAW (to unlock the VST3 file)

$src = 'D:\Github\VST3 Developments\BLUR\iPlug2\Examples\IPlugEffect\build-win\vst3\x64\Release\SpectralProtocol.vst3'
$dst = 'C:\Program Files\Common Files\VST3\SpectralProtocol.vst3'

Write-Host "Deploying SpectralProtocol.vst3..." -ForegroundColor Cyan

if (-not (Test-Path $src)) {
    Write-Host "ERROR: Source file not found: $src" -ForegroundColor Red
    exit 1
}

try {
    # Remove old copy if it exists
    if (Test-Path $dst) {
        Remove-Item $dst -Force -Recurse -ErrorAction Stop
        Write-Host "Removed old VST3 file" -ForegroundColor Yellow
    }
    
    # Copy new binary
    Copy-Item -Path $src -Destination $dst -Recurse -Force -ErrorAction Stop
    
    # Verify deployment
    $deployed = Get-Item $dst
    Write-Host "✓ Deployment successful!" -ForegroundColor Green
    Write-Host "  Location: $($deployed.FullName)" -ForegroundColor Green
    Write-Host "  Timestamp: $($deployed.LastWriteTime)" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next step: Restart your DAW and rescan VST3 plugins" -ForegroundColor Cyan
}
catch {
    Write-Host "ERROR: Deployment failed" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    exit 1
}
