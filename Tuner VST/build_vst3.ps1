# =============================================================================
# build_vst3.ps1 - Build Tuner.vst3 on Windows via msbuild + iPlug2.
#
# Follows the Freeze95/build_vst3.ps1 convention exactly:
#   * References the iPlug2 example VST3 project directly
#   * Sets SolutionDir to the current directory so the win.props file paths
#     resolve correctly
#   * Tries multiple MSBuild locations in order (2022 Build Tools first,
#     then Community, then 2019, then VS Preview)
#   * Copies the resulting build-win/Tuner.vst3 into ../common files/vst3
#     AND %CommonProgramW6432%\VST3 (so the DAW picks it up)
#
# Usage:  powershell -ExecutionPolicy Bypass -File build_vst3.ps1
# =============================================================================

$ErrorActionPreference = "Stop"

$scriptDir   = (Resolve-Path $PSScriptRoot).Path
$solutionDir = $scriptDir.TrimEnd('\')
# Freeze95's win.props uses "$(SolutionDir)scripts\..." (no extra backslash
# between SolutionDir and scripts), so SolutionDir needs a trailing
# separator.  PowerShell's "\\" expands to a single "\" inside the
# substituted value, and the surrounding "..." form the literal quote
# characters MSBuild consumes as the property-quoting pair.  This is
# exactly how Freeze95/build_vst3.ps1 does it.
$solutionDirArgument = $solutionDir + "\\"

# Find the iPlug2 checkout.  Two valid layouts:
#   1. <workspace>/iPlug2/  -- sibling of the plugin folder (local dev)
#   2. <plugin>/iPlug2/     -- cloned in place by CI (workflow_dispatch)
# We try both so the same script works for "developer on workstation" and
# "fresh CI checkout".
$projectCandidates = @(
    (Join-Path (Split-Path $solutionDir -Parent) "iPlug2/Examples/IPlugEffect/projects/IPlugEffect-vst3.vcxproj"),
    (Join-Path $solutionDir "iPlug2/Examples/IPlugEffect/projects/IPlugEffect-vst3.vcxproj")
)
$project = $null
foreach ($c in $projectCandidates) {
    if (Test-Path $c) { $project = $c; break }
}
if (-not $project) {
    Write-Error "iPlug2 project not found in either layout. Tried:`n  $($projectCandidates -join "`n  ")"
    exit 1
}

# Locate msbuild.  Try 2022 Build Tools (most common on CI), then Community,
# then 2019, then Preview.  Each candidate is verified to actually exist.
$msbuildCandidates = @(
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Preview\MSBuild\Current\Bin\MSBuild.exe"
)
$msbuild = $null
foreach ($c in $msbuildCandidates) {
    if (Test-Path $c) { $msbuild = $c; break }
}
if (-not $msbuild) {
    Write-Error "No MSBuild found. Install Visual Studio 2022 (with C++ workload) or Build Tools 2022."
    exit 1
}

Write-Host "Using MSBuild: $msbuild" -ForegroundColor Cyan
Write-Host "Project:       $project" -ForegroundColor Cyan
Write-Host "SolutionDir:   $solutionDirArgument" -ForegroundColor Cyan

# Build Release x64.  iPlug2's win.props defines TARGET_NAME via BINARY_NAME,
# but the project file is the generic IPlugEffect one which sets OutputName
# to $(BINARY_NAME) at the props level -- so the resulting DLL is Tuner.dll.
& $msbuild $project `
    /p:Configuration=Release `
    /p:Platform=x64 `
    /p:SolutionDir="$solutionDirArgument" `
    /p:BinaryName=Tuner `
    /v:minimal

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

# iPlug2's VST3 build emits at build-win/vst3/x64/Release/Tuner.vst3 (a
# single .vst3 file, not a bundle -- matches the Freeze95 convention).  Look
# there first, then fall back to a flat layout for older iPlug2 versions.
$builtBinary = Join-Path $solutionDir "build-win\vst3\x64\Release\Tuner.vst3"
if (-not (Test-Path $builtBinary)) {
    $builtBinary = Join-Path $solutionDir "build-win\Tuner.vst3"
}
if (-not (Test-Path $builtBinary)) {
    Write-Error "Expected build output not found. Looked for:"
    Write-Error "  $builtBinary"
    exit 1
}

# Copy into ../common files/vst3 (sibling to the project folder) so other
# builds can pick it up, and into %CommonProgramW6432%\VST3 so the host DAW
# loads it.
$commonDir = Join-Path (Split-Path $solutionDir -Parent) "common files\vst3"
if (-not (Test-Path $commonDir)) { New-Item -ItemType Directory -Force -Path $commonDir | Out-Null }
Copy-Item -Force $builtBinary (Join-Path $commonDir "Tuner.vst3")
Write-Host "Copied to $commonDir\Tuner.vst3" -ForegroundColor Green

if (Test-Path env:CommonProgramW6432) {
    $sysVst3 = Join-Path $env:CommonProgramW6432 "VST3"
    if (-not (Test-Path $sysVst3)) { New-Item -ItemType Directory -Force -Path $sysVst3 | Out-Null }
    Copy-Item -Force $builtBinary (Join-Path $sysVst3 "Tuner.vst3")
    Write-Host "Copied to $sysVst3\Tuner.vst3" -ForegroundColor Green
}

Write-Host "Tuner VST3 build complete." -ForegroundColor Green
exit 0