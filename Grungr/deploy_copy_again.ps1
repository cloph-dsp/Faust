$src = 'D:\Github\VST3 Developments\Grungr\iPlug2\Examples\DODGrunge\build-win\Grungr.vst3'
$dst1 = 'D:\Github\VST3 Developments\common files\vst3'
$dst2 = Join-Path $env:CommonProgramW6432 'VST3'

Write-Host "Source: $src"
if (-not (Test-Path $src)) {
  Write-Error "Source not found: $src"
  exit 1
}

Write-Host "Copying to $dst1 ..."
New-Item -ItemType Directory -Path $dst1 -Force | Out-Null
try {
  Remove-Item -Recurse -Force (Join-Path $dst1 'Grungr.vst3') -ErrorAction Stop
} catch {}
try {
  Copy-Item -Path $src -Destination $dst1 -Recurse -Force
  Write-Host "Copied to $dst1"
} catch {
  Write-Warning "Copy to common files failed: $_"
  exit 2
}

Write-Host "Copying to $dst2 ..."
New-Item -ItemType Directory -Path $dst2 -Force | Out-Null
try {
  Remove-Item -Recurse -Force (Join-Path $dst2 'Grungr.vst3') -ErrorAction Stop
} catch {}
try {
  Copy-Item -Path $src -Destination $dst2 -Recurse -Force
  Write-Host "Copied to $dst2"
} catch {
  Write-Warning "Copy to system path failed: $_"
  exit 3
}

Write-Host "Done"
exit 0
