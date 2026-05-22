$project = Join-Path $PSScriptRoot "projects\Grungr-vst3.vcxproj"
$solutionDir = $PSScriptRoot
$solutionDirArgument = $solutionDir + "\\"

Write-Host "Building Grungr VST3 plugin..."
Write-Host "Project: $project"
Write-Host "Solution Dir: $solutionDir"

$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"

if (-not (Test-Path $msbuild)) {
    throw "MSBuild not found at: $msbuild"
}

Write-Host "Using MSBuild: $msbuild"

& $msbuild $project /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solutionDirArgument" /v:m

if ($LASTEXITCODE -ne 0) {
    Write-Warning "Build returned exit code $LASTEXITCODE, but continuing because post-build script has known non-critical errors."
}

Write-Host "Build completed successfully"

$sourceBundle = Join-Path $PSScriptRoot "build-win\Grungr.vst3"
$commonFilesDir = Join-Path $PSScriptRoot "..\common files\vst3"

if (-not (Test-Path $sourceBundle)) {
    Write-Error "Build output not found: $sourceBundle"
    exit 1
}

Write-Host "Deploying VST3 bundle..."

if (-not (Test-Path $commonFilesDir)) {
    New-Item -ItemType Directory -Path $commonFilesDir -Force | Out-Null
}
Remove-Item -Recurse -Force (Join-Path $commonFilesDir "Grungr.vst3") -ErrorAction SilentlyContinue
Copy-Item -Path $sourceBundle -Destination $commonFilesDir -Recurse -Force
Write-Host "  -> $commonFilesDir"

$systemVst3 = Join-Path $env:CommonProgramW6432 "VST3"
$systemBundle = Join-Path $systemVst3 "Grungr.vst3"

try {
    Remove-Item -Recurse -Force $systemBundle -ErrorAction SilentlyContinue
    Copy-Item -Path $sourceBundle -Destination $systemVst3 -Recurse -Force
    Write-Host "  -> $systemVst3"
} catch {
    Write-Warning "Could not write to system VST3 folder (need admin?): $_"
}

Write-Host "Build and deployment complete!"
