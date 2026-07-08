$src = @{
    "D:\Github\VST3 Developments\Tuner VST\resources\fonts\TEXASLED.TTF" = "TEXASLED.TTF"
    "D:\GithubVST3 Developments\Tuner VST\resources\CLOPH.svg"       = "CLOPH.svg"
}
foreach ($dstDir in @(
    "D:\GithubVST3 Developments\Tuner VST\build-win\vst3\x64\Release",
    "D:\GithubVST3 Developments\common files\vst3",
    "C:\Program Files\Common Files\VST3"
)) {
    foreach ($src in $src.Keys) {
        $dst = Join-Path $dstDir $src[$src]
        try {
            Copy-Item -Force $src $dst -ErrorAction Stop
            Write-Host "Copied $($src[$src]) -> $dst"
        } catch {
            Write-Host "Could not copy to $dst ($($_.Exception.Message))"
        }
    }
}
Get-ChildItem "C:\Program Files\Common Files\VST3\Tuner.vst3",
              "C:\Program Files\Common Files\VST3\TEXASLED.TTF",
              "C:\Program Files\Common Files\VST3\CLOPH.svg" |
    Format-Table FullName,Length,LastWriteTime