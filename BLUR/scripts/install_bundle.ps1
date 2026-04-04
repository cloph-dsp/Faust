$src = 'd:\Github\VST3 Developments\BLUR\iPlug2\Examples\IPlugEffect\build-win\vst3\x64\Release\SpectralProtocol.vst3'
$dest = 'C:\Program Files\Common Files\VST3\SpectralProtocol.vst3'
try {
    Remove-Item -Force -Recurse $dest -ErrorAction SilentlyContinue
    $target = Join-Path $dest 'Contents\x86_64-win'
    New-Item -ItemType Directory -Force -Path $target | Out-Null
    Copy-Item -Force $src (Join-Path $target 'SpectralProtocol.vst3')
    Write-Output 'BundleCopied'
} catch {
    Write-Output "ERROR: $_"
}
