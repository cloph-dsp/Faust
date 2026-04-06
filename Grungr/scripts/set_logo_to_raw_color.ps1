$src = 'D:\Github\VST3 Developments\Grungr\iPlug2\Examples\DODGrunge\resources\cloph-logo.svg'
if (-not (Test-Path $src)) { Write-Error "Source not found: $src"; exit 1 }

$content = Get-Content -Raw -Path $src
$content = $content -replace 'fill="rgb\(255,152,118\)"','fill="rgb(233,214,202)"'
$content = $content -replace 'fill-opacity="0.48"','fill-opacity="0.85"'
Set-Content -Path $src -Value $content -Encoding UTF8
Write-Host 'Updated source SVG to RAW-text color'

$bundleRes = 'D:\Github\VST3 Developments\Grungr\iPlug2\Examples\DODGrunge\build-win\Grungr.vst3\Contents\Resources'
if (Test-Path $bundleRes) {
  Copy-Item -Path $src -Destination (Join-Path $bundleRes 'cloph-logo.svg') -Force
  Write-Host 'Copied updated SVG to bundle resources'
} else {
  Write-Warning "Bundle resources not found: $bundleRes"
}

# Attempt deploy to both locations
& 'D:\Github\VST3 Developments\Grungr\deploy_copy_again.ps1'