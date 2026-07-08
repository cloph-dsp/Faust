$project = Join-Path $PSScriptRoot "projects\BronzeNoise-vst3.vcxproj"
$solutionDir = (Resolve-Path $PSScriptRoot).Path.TrimEnd('\\')
$solutionDirArgument = $solutionDir + "\\"

Write-Host "Building Bronze Noise VST3 (Release x64)..."
Write-Host "Project: $project"
Write-Host "Solution Dir: $solutionDir"

# Find MSBuild
$msbuildCandidates = @(
  "$env:ProgramFiles\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
  "$env:ProgramFiles\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
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

# Clean previous build artifacts to avoid stale object file conflicts with the rebrand
$intDir = Join-Path $PSScriptRoot "build-win\vst3\x64\Release\int"
if (Test-Path $intDir) {
  Write-Host "Cleaning stale int directory..."
  Remove-Item -Path $intDir -Recurse -Force -ErrorAction SilentlyContinue
}

# Build Release x64
& $msbuild $project /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solutionDirArgument" /v:m /t:Rebuild

if ($LASTEXITCODE -ne 0) {
  Write-Error "Build failed with exit code $LASTEXITCODE"
  exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build completed successfully."
Write-Host ""

# Locate the produced .vst3 (single file, not bundle)
$builtVst3 = Join-Path $PSScriptRoot "build-win\vst3\x64\Release\BronzeNoise.vst3"

if (-not (Test-Path $builtVst3)) {
  Write-Error "Build output not found: $builtVst3"
  exit 1
}

$fileInfo = Get-Item $builtVst3
Write-Host ("Build output: {0}" -f $builtVst3)
Write-Host ("Size: {0:N0} bytes" -f $fileInfo.Length)
Write-Host ("PSIsContainer: {0}" -f $fileInfo.PSIsContainer)
Write-Host ""

# Copy to system VST3 folder (C:\Program Files\Common Files\VST3)
$systemVst3 = Join-Path $env:CommonProgramW6432 "VST3"
$systemBundle = Join-Path $systemVst3 "BronzeNoise.vst3"

if (-not (Test-Path $systemVst3)) {
  Write-Error "System VST3 folder not found: $systemVst3"
  exit 1
}

Write-Host "Deploying to system VST3 folder..."
Write-Host "Target: $systemBundle"

try {
  # If existing destination is a directory (bundle), remove it first
  if (Test-Path $systemBundle) {
    if ((Get-Item $systemBundle).PSIsContainer) {
      Write-Host "Removing existing bundle directory..."
      Remove-Item -Path $systemBundle -Recurse -Force
    } else {
      Remove-Item -Path $systemBundle -Force
    }
  }
  Copy-Item -Path $builtVst3 -Destination $systemBundle -Force
  Write-Host ""
  Write-Host "Deployed successfully!"
  Write-Host ("  -> {0}" -f $systemBundle)
  $deployedInfo = Get-Item $systemBundle
  Write-Host ("     Size: {0:N0} bytes" -f $deployedInfo.Length)
  Write-Host ("     Container: {0}" -f $deployedInfo.PSIsContainer)
} catch {
  Write-Warning "Could not write to system VST3 folder (need admin?): $_"
  Write-Warning "Run this script as Administrator to install to $systemBundle"
  exit 1
}