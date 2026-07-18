$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$workspaceRoot = Split-Path -Parent $projectRoot
$sdkRoot = Join-Path $workspaceRoot "iPlug2\Dependencies\IPlug\VST3_SDK"
$source = Join-Path $PSScriptRoot "vst3_smoke.cpp"
$output = Join-Path $PSScriptRoot "vst3_smoke.exe"
$objectDir = Join-Path $PSScriptRoot "obj"
$coreIids = Join-Path $sdkRoot "pluginterfaces\base\coreiids.cpp"
$vstIids = Join-Path $sdkRoot "public.sdk\source\vst\vstinitiids.cpp"
$vsDevCmd = @(
  "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat",
  "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat",
  "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat",
  "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
) | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $vsDevCmd) {
  $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
  if (Test-Path $vswhere) {
    $installation = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($installation) { $vsDevCmd = Join-Path $installation "Common7\Tools\VsDevCmd.bat" }
  }
}
if (-not $vsDevCmd -or -not (Test-Path $vsDevCmd)) { throw "VsDevCmd.bat not found" }
New-Item -ItemType Directory -Force -Path $objectDir | Out-Null

$command = "`"$vsDevCmd`" -arch=x64 -host_arch=x64 >nul && cl /nologo /std:c++17 /EHsc /O2 /I`"$sdkRoot`" `"$source`" `"$coreIids`" `"$vstIids`" /Fo`"$objectDir\\`" /Fe`"$output`""
& cmd.exe /d /s /c $command
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& $output (Join-Path $projectRoot "build-win\vst3\x64\Release\BronzeNoise.vst3")
exit $LASTEXITCODE
