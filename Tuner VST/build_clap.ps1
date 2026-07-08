# =============================================================================
# build_clap.ps1 - Build Tuner.clap on Windows via msbuild + iPlug2.
#
# Mirrors build_vst3.ps1 exactly except for the vcxproj and the output
# path.  The IPlugEffect-clap.vcxproj is a parallel example in
# iPlug2/Examples/IPlugEffect/projects/ that links the same IPlugEffect.cpp
# we already stage via _local_copy.ps1 -- no source-file changes are
# required to add CLAP support.
#
# Output: build-win/clap/x64/Release/Tuner.clap
# Deployed to:
#   1) ../common files/clap/Tuner.clap    (workspace-local mirror)
#   2) %CommonProgramW6432%\CLAP\Tuner.clap (system CLAP folder)
#
# Usage:  powershell -ExecutionPolicy Bypass -File build_clap.ps1
# =============================================================================

$ErrorActionPreference = "Stop"

$scriptDir   = (Resolve-Path $PSScriptRoot).Path
$solutionDir = $scriptDir.TrimEnd('\')
# Win.props uses "$(SolutionDir)scripts\..." (no extra backslash between
# SolutionDir and scripts), so SolutionDir needs a trailing separator.
# PowerShell "\\" expands to a single "\" inside the substituted value,
# and the surrounding "..." form the literal quote characters MSBuild
# consumes as the property-quoting pair.  Same trick as build_vst3.ps1.
$solutionDirArgument = $solutionDir + "\\"

# Find the iPlug2 checkout.  Two valid layouts (sibling or in-tree),
# matching the build_vst3.ps1 logic.
$projectCandidates = @(
    (Join-Path (Split-Path $solutionDir -Parent) "iPlug2/Examples/IPlugEffect/projects/IPlugEffect-clap.vcxproj"),
    (Join-Path $solutionDir "iPlug2/Examples/IPlugEffect/projects/IPlugEffect-clap.vcxproj")
)
$project = $null
foreach ($c in $projectCandidates) {
    if (Test-Path $c) { $project = $c; break }
}
if (-not $project) {
    Write-Error "iPlug2 clap project not found in either layout. Tried:`n  $($projectCandidates -join "`n  ")"
    exit 1
}

# Locate msbuild.  Same candidate list as build_vst3.ps1.
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

# Build Release x64.  OutputName is set to $(BINARY_NAME) via the win.props
# file, so the resulting DLL is Tuner.dll -- we then rename to .clap below.
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

# iPlug2's CLAP build emits a single-file plugin at
# build-win/clap/x64/Release/Tuner.clap (the vcxproj's postbuild
# wraps the compiled .dll into the .clap extension; we don't need
# to do that here).  Older iPlug2 versions emitted a bare .dll -- if
# that's the case, fall back to renaming it.
$clapBinary = Join-Path $solutionDir "build-win\clap\x64\Release\Tuner.clap"
if (-not (Test-Path $clapBinary)) {
    $builtBinary = Join-Path $solutionDir "build-win\clap\x64\Release\Tuner.dll"
    if (Test-Path $builtBinary) {
        Copy-Item -Force $builtBinary $clapBinary
        Write-Host "Renamed DLL to CLAP" -ForegroundColor Cyan
    } else {
        Write-Error "Expected build output not found at $clapBinary (or $builtBinary)"
        exit 1
    }
}

# Copy into ../common files/clap (sibling to the project folder) so other
# builds can pick it up, and into %CommonProgramW6432%\CLAP so the host DAW
# loads it.  CLAP is a single-file format (not a bundle), so the file goes
# straight into the directory -- no Contents/MacOS/ wrapper.
$commonDir = Join-Path (Split-Path $solutionDir -Parent) "common files\clap"
if (-not (Test-Path $commonDir)) { New-Item -ItemType Directory -Force -Path $commonDir | Out-Null }
Copy-Item -Force $clapBinary (Join-Path $commonDir "Tuner.clap")
Write-Host "Copied to $commonDir\Tuner.clap" -ForegroundColor Green

if (Test-Path env:CommonProgramW6432) {
    $sysClap = Join-Path $env:CommonProgramW6432 "CLAP"
    if (-not (Test-Path $sysClap)) {
        try {
            New-Item -ItemType Directory -Force -Path $sysClap | Out-Null
        } catch {
            Write-Warning "Could not create system CLAP folder (need admin?): $_"
            Write-Warning "Run this script as Administrator to install to $sysClap"
        }
    }
    if (Test-Path $sysClap) {
        try {
            Copy-Item -Force $clapBinary (Join-Path $sysClap "Tuner.clap")
            Write-Host "Copied to $sysClap\Tuner.clap" -ForegroundColor Green
        } catch {
            Write-Warning "Could not write to system CLAP folder (need admin?): $_"
            Write-Warning "Run this script as Administrator to install to $sysClap"
        }
    }
}

Write-Host "Tuner CLAP build complete." -ForegroundColor Green
exit 0
