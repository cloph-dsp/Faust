param(
  [switch]$Deploy
)

$project = Join-Path $PSScriptRoot "projects\Grungr-vst3.vcxproj"
$solutionDir = $PSScriptRoot
$solutionDirArgument = $solutionDir + "\"
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$sourcePlugin = Join-Path $PSScriptRoot "build-win\vst3\x64\Release\Grungr.vst3"

if (-not (Test-Path -LiteralPath $msbuild)) {
  throw "MSBuild not found at: $msbuild"
}

Write-Host "Building Grungr VST3 (Release x64)..."
& $msbuild $project /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solutionDirArgument" /p:PostBuildEventUseInBuild=false /v:minimal /nologo
if ($LASTEXITCODE -ne 0) {
  throw "MSBuild failed with exit code $LASTEXITCODE. No existing artifact was deployed."
}

if (-not (Test-Path -LiteralPath $sourcePlugin -PathType Leaf)) {
  throw "Expected VST3 artifact was not produced: $sourcePlugin"
}

Write-Host "Build complete: $sourcePlugin"
if (-not $Deploy) {
  Write-Host "Use -Deploy to copy this verified artifact to the test locations."
  exit 0
}

function Copy-PluginWithRetry {
  param(
    [Parameter(Mandatory = $true)][string]$DestinationDirectory
  )

  New-Item -ItemType Directory -Path $DestinationDirectory -Force | Out-Null
  $destination = Join-Path $DestinationDirectory (Split-Path -Leaf $sourcePlugin)

  for ($attempt = 1; $attempt -le 5; $attempt++) {
    try {
      Copy-Item -LiteralPath $sourcePlugin -Destination $destination -Force -ErrorAction Stop
      Write-Host "  -> $destination"
      return
    }
    catch {
      if ($attempt -eq 5) {
        throw "Could not deploy to $destination after $attempt attempts: $($_.Exception.Message)"
      }
      Start-Sleep -Seconds 2
    }
  }
}

$workspaceVst3 = Join-Path $PSScriptRoot "..\common files\vst3"
Copy-PluginWithRetry -DestinationDirectory $workspaceVst3

$systemVst3 = Join-Path $env:CommonProgramW6432 "VST3"
Copy-PluginWithRetry -DestinationDirectory $systemVst3

Write-Host "Deployment complete."
