$paths = @(
  "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
  "${env:ProgramFiles}\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
  "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe",
  "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
)
$found = $null
foreach ($p in $paths) {
  if (Test-Path $p) { $found = $p; break }
}
if (-not $found) {
  Write-Host "Searching registry..."
  $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
  if (Test-Path $vswhere) {
    Write-Host "vswhere exists: $vswhere"
    $all = & $vswhere -all -prerelease -format json 2>&1
    Write-Host $all
  } else {
    Write-Host "vswhere NOT found"
  }
  Write-Host "Searching filesystem..."
  Get-ChildItem -Path "${env:ProgramFiles}" -Filter "MSBuild.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 5 -ExpandProperty FullName
  Get-ChildItem -Path "${env:ProgramFiles(x86)}" -Filter "MSBuild.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 5 -ExpandProperty FullName
} else {
  Write-Host "FOUND: $found"
}
