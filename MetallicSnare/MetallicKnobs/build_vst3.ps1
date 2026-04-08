$project = Join-Path $PSScriptRoot "projects\MetallicKnobs-vst3.vcxproj"
$solutionDir = (Resolve-Path $PSScriptRoot).Path.TrimEnd('\\')
$solutionDirArgument = $solutionDir + "\\"

$msbuildCandidates = @(
  "$env:ProgramFiles\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
)

$msbuild = $msbuildCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $msbuild) {
  $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"

  if (Test-Path $vswhere) {
    $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
  }
}

if (-not $msbuild) {
  throw "MSBuild was not found."
}

& $msbuild $project /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solutionDirArgument" /v:m

if ($LASTEXITCODE -ne 0) {
  exit $LASTEXITCODE
}

$localBundle = Join-Path $PSScriptRoot "build-win\MetallicKnobs.vst3"
$installedBundle = Join-Path $env:CommonProgramW6432 "VST3\MetallicKnobs.vst3"

Write-Host "Local bundle:" $localBundle
Write-Host "Installed bundle:" $installedBundle

if (Test-Path $localBundle) {
  try {
    Copy-Item $localBundle $installedBundle -Recurse -Force -ErrorAction Stop
    Write-Host "Installed bundle:" $installedBundle
  } catch {
    Write-Host "Warning: could not copy VST3 bundle to system location. It may be in use by a host."
    Write-Host "Local bundle remains at:" $localBundle
  }
}
