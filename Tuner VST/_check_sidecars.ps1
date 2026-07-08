# Compare what Freeze95 ships alongside its .vst3 (font, SVG, etc.) vs what Tuner ships.
# This tells us whether Freeze95 actually embeds the font OR uses a sidecar file.
$srcDir = "D:\Github\VST3 Developments\Freeze95\build-win\vst3\x64\Release"
$dstDir = "D:\Github\VST3 Developments\Tuner VST\build-win\vst3\x64\Release"
$srcDir2 = "C:\Program Files\Common Files\VST3"
$dstDir2 = "D:\Github\VST3 Developments\common files\vst3"

Write-Host "=== Freeze95 build output dir ==="
Get-ChildItem $srcDir | Select-Object Name,Length | Format-Table

Write-Host "=== Tuner build output dir ==="
Get-ChildItem $dstDir | Select-Object Name,Length | Format-Table

Write-Host "=== Freeze95 deployed to system VST3 ==="
Get-ChildItem $srcDir2 -Filter "*Freeze95*" | ForEach-Object {
    if ($_.PSIsContainer) {
        Get-ChildItem -Recurse $_.FullName | Select-Object FullName,Length
    } else {
        $_
    }
} | Format-Table

Write-Host "=== Tuner deployed to system VST3 ==="
Get-ChildItem $dstDir2 -Filter "*Tuner*" | Select-Object Name,Length | Format-Table