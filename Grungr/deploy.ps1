$src = "D:\Github\VST3 Developments\Grungr\build-win\vst3\x64\Release\Grungr.vst3"
$dst = "C:\Program Files\Common Files\VST3\Grungr.vst3"

# Kill Bitwig processes
$procs = @("Bitwig Studio","BitwigAudioEngine-X64-AVX2","BitwigPluginHost-X64-AVX2","BitwigStudioApp")
foreach ($p in $procs) {
  $running = Get-Process -Name $p -ErrorAction SilentlyContinue
  if ($running) {
    Write-Host "Killing: $p"
    $running | Stop-Process -Force
  }
}
Start-Sleep -Seconds 2

# Delete old bundle directory if it exists
if (Test-Path $dst) {
  Write-Host "Removing old: $dst (IsDir=$(Test-Path $dst -PathType Container))"
  Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
  Start-Sleep -Seconds 1
}

# Copy single-file VST3
Write-Host "Copying: $src -> $dst"
Copy-Item -Force $src $dst
Start-Sleep -Seconds 1

# Verify
if (Test-Path $dst) {
  $item = Get-Item $dst
  Write-Host "Deployed: $dst"
  Write-Host "  Mode: $($item.Mode)"
  Write-Host "  Size: $($item.Length)"
  Write-Host "  Modified: $($item.LastWriteTime)"
} else {
  Write-Host "FAILED: $dst not found after copy"
}
