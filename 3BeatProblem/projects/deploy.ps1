$src = 'D:\Github\VST3 Developments\3BeatProblem\build-win\vst3\x64\Release\3BeatProblem.vst3'
$dst = 'C:\Program Files\Common Files\VST3\3BeatProblem.vst3'
$timeout = 10
$elapsed = 0
$interval = 1
while ($elapsed -lt $timeout) {
  try {
    Copy-Item -Force $src $dst -ErrorAction Stop
    Write-Host "Copy succeeded at $elapsed seconds"
    exit 0
  } catch {
    Start-Sleep -Seconds $interval
    $elapsed += $interval
  }
}
Write-Host "Copy failed after $timeout seconds"
exit 1
