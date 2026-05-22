$project = Join-Path $PSScriptRoot "projects\DustStop-vst3.vcxproj"
$solutionDir = (Resolve-Path $PSScriptRoot).Path.TrimEnd('\')
$solutionDirArgument = $solutionDir + "\\"

$msbuildCandidates = @(
  "$env:ProgramFiles\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles(x86)\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
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

Write-Host "Building with: $msbuild"
Write-Host "SolutionDir: $solutionDirArgument"

$msbuildArgs = @(
  "`"$project`"",
  "/p:Configuration=Release",
  "/p:Platform=x64",
  "`"/p:SolutionDir=$solutionDirArgument`"",
  "/v:m"
)
& $msbuild $msbuildArgs

if ($LASTEXITCODE -ne 0) {
  Write-Host "BUILD FAILED with exit code $LASTEXITCODE"
  exit $LASTEXITCODE
}

Write-Host "BUILD SUCCEEDED"
