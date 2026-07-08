$src = "D:\Github\VST3 Developments\common files\vst3\Tuner.vst3"
$dst = "C:\Program Files\Common Files\VST3\Tuner.vst3"
try {
  Copy-Item -Force $src $dst -ErrorAction Stop
  $f = Get-Item $dst
  Write-Host "OK: $($f.Length) bytes, $($f.LastWriteTime)"
} catch {
  Write-Host "FAIL: $($_.Exception.Message)"
  Get-Process -Name 'Bitwig*','reaper*' -ErrorAction SilentlyContinue | ForEach-Object { Write-Host "  holding: $($_.Name) PID=$($_.Id)" }
}