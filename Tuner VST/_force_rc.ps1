$ErrorActionPreference = "Stop"
$root = "D:\Github\VST3 Developments\Tuner VST"
Set-ItemProperty -Path "$root\resources\main.rc" -Name "LastWriteTime" -Value (Get-Date)
$res = Get-ChildItem -Path "$root\build-win" -Recurse -Filter "main.res" -ErrorAction SilentlyContinue
foreach ($r in $res) {
    Remove-Item $r.FullName -Force
    Write-Host ("Deleted " + $r.FullName)
}
Write-Host "RC timestamp bumped, main.res cleared."