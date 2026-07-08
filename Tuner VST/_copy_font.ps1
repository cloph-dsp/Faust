$src = "D:\Downloads\intodotmatrix.ttf"
foreach ($dst in @(
    "D:\Github\VST3 Developments\Tuner VST\resources\fonts\TEXASLED.TTF",
    "D:\Github\VST3 Developments\iPlug2\Examples\IPlugEffect\resources\fonts\TEXASLED.TTF",
    "D:\Github\VST3 Developments\Tuner VST\build-win\vst3\x64\Release\TEXASLED.TTF",
    "D:\Github\VST3 Developments\common files\vst3\TEXASLED.TTF",
    "C:\Program Files\Common Files\VST3\TEXASLED.TTF"
)) {
    New-Item -ItemType Directory -Force -Path (Split-Path $dst) | Out-Null
    Copy-Item -Force $src $dst
    Write-Host "Wrote $dst"
}
Get-ChildItem "D:\Github\VST3 Developments\Tuner VST\resources\fonts\TEXASLED.TTF",
              "C:\Program Files\Common Files\VST3\TEXASLED.TTF" |
    Format-Table FullName,Length,LastWriteTime