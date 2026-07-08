$ErrorActionPreference = 'Stop'
$src = 'D:\Github\VST3 Developments\Grungr\build-win\Grungr.vst3'
$dest = 'C:\Program Files\Common Files\VST3\Grungr.vst3'
$start = Get-Date
Write-Host "Removing old bundle..."
Remove-Item -Path $dest -Recurse -Force -ErrorAction SilentlyContinue
Write-Host "Copying bundle..."
Copy-Item -Path $src -Destination $dest -Recurse -Force
$copied = Get-ChildItem -Path $dest -Recurse -File | Measure-Object -Property Length -Sum
Write-Host "Deployed $($copied.Count) files, $($copied.Sum) bytes in $((Get-Date).Subtract($start).TotalSeconds)s"
Write-Host "Contents:"
Get-ChildItem -Path $dest -Recurse -File | ForEach-Object { Write-Host "  $($_.FullName.Replace($dest, '$dest').Replace('C:\Program Files\Common Files\VST3\', ''))" }
