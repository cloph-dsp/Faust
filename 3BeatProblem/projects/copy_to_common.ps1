$src = 'D:\Github\VST3 Developments\3BeatProblem\build-win\vst3\x64\Release\3BeatProblem.vst3'
$dst = 'C:\Program Files\Common Files\VST3\3BeatProblem.vst3'
$srcSize = (Get-Item $src).Length
Write-Host "Source: $src ($srcSize bytes)"

$ok = $false
for ($i = 1; $i -le 30; $i++) {
  try {
    Copy-Item -LiteralPath $src -Destination $dst -Force -ErrorAction Stop
    $ok = $true
    break
  } catch [System.UnauthorizedAccessException] {
    Start-Sleep -Seconds 2
  } catch [System.IO.IOException] {
    Start-Sleep -Seconds 2
  } catch {
    Write-Host "Error: $($_.Exception.Message)"
    Start-Sleep -Seconds 2
  }
}

if ($ok) {
  $dstItem = Get-Item $dst
  Write-Host "OK: copied to $dst ($($dstItem.Length) bytes, $($dstItem.LastWriteTime))"
  if ($dstItem.Length -ne $srcSize) {
    Write-Host "WARNING: size mismatch!"
  } else {
    Write-Host "Size matches source."
  }
} else {
  Write-Host "FAILED after 30 retries (60s) - DAW still holding lock?"
}
