$ErrorActionPreference = "SilentlyContinue"
$src = "D:\Github\VST3 Developments\3BeatProblem\build-win\vst3\x64\Release\3BeatProblem.vst3"
$dst = "C:\Program Files\Common Files\VST3\3BeatProblem.vst3"
$maxRetries = 10
$retryDelay = 500

for ($i = 1; $i -le $maxRetries; $i++) {
    try {
        Copy-Item -Path $src -Destination $dst -Force -ErrorAction Stop
        $deployed = Get-Item $dst
        $srcSize = (Get-Item $src).Length
        Write-Host "Deploy OK: $($deployed.Length) bytes"
        if ($deployed.Length -eq $srcSize) {
            Write-Host "Size match: OK"
        } else {
            Write-Host "Size mismatch: src=$srcSize deployed=$($deployed.Length)"
        }
        exit 0
    } catch {
        $err = $_.Exception.Message
        if ($err -match "Access|ACCESS|denied|DENIED|locked|LOCKED|sharing|Sharing") {
            Start-Sleep -Milliseconds $retryDelay
        } else {
            Write-Host "Deploy failed: $err"
            exit 1
        }
    }
}
Write-Host "Max retries exceeded"
exit 1