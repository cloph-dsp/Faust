$project = Join-Path $PSScriptRoot "iPlug2/Examples/IPlugEffect/projects/IPlugEffect-vst3.vcxproj"
$solutionDir = (Resolve-Path $PSScriptRoot).Path
$solutionDirArgument = $solutionDir + "\\"

Write-Host "Building Freeze95 VST3 plugin..."
Write-Host "Project: $project"
Write-Host "Solution Dir: $solutionDir"

# Find MSBuild
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

Write-Host "Using MSBuild: $msbuild"

# Build Release x64
& $msbuild $project /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solutionDirArgument" /v:m

if ($LASTEXITCODE -ne 0) {
  Write-Error "Build failed with exit code $LASTEXITCODE"
  exit $LASTEXITCODE
}

Write-Host "Build completed successfully"

# Copy to common files/vst3 — use the fully-structured bundle created by postbuild
$sourceBundle = Join-Path $PSScriptRoot "build-win\Freeze95.vst3"
$commonFilesDir = Join-Path $PSScriptRoot "..\common files\vst3"

if (-not (Test-Path $sourceBundle)) {
  Write-Error "Build output not found: $sourceBundle"
  exit 1
}

Write-Host "Deploying VST3..."

# 1 – Workspace copy
if (-not (Test-Path $commonFilesDir)) {
  New-Item -ItemType Directory -Path $commonFilesDir -Force | Out-Null
}
Remove-Item -Path (Join-Path $commonFilesDir "Freeze95.vst3") -Force -ErrorAction SilentlyContinue
Copy-Item -Path $sourceBundle -Destination $commonFilesDir -Force
Write-Host "  -> $commonFilesDir"

# 2 – System VST3 folder (C:\Program Files\Common Files\VST3)
$systemVst3 = Join-Path $env:CommonProgramW6432 "VST3"
$systemBundle = Join-Path $systemVst3 "Freeze95.vst3"

try {
  Remove-Item -Force $systemBundle -ErrorAction SilentlyContinue
  Copy-Item -Path $sourceBundle -Destination $systemVst3 -Force
  Write-Host "  -> $systemVst3"
} catch {
  Write-Warning "Could not write to system VST3 folder (need admin?): $_"
  Write-Warning "Run this script as Administrator to install to $systemVst3"
}

Write-Host "Build and deployment complete!"
