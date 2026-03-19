[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$workflowDir = $PSScriptRoot
$javaHome = Join-Path $workflowDir 'tools\jdk-21.0.9+10'
$ghidraHome = 'C:\Users\Razer\AppData\Local\UniGetUI\Chocolatey\lib\ghidra\tools\ghidra_12.0.4_PUBLIC'
$ghidraRun = Join-Path $ghidraHome 'ghidraRun.bat'
$projectFile = Join-Path $workflowDir 'project\MNSpectralBlurring.gpr'

if (!(Test-Path $javaHome)) {
    throw "Portable JDK not found at $javaHome"
}

if (!(Test-Path $ghidraRun)) {
    throw "Ghidra launcher not found at $ghidraRun"
}

$env:JAVA_HOME = $javaHome

if (Test-Path $projectFile) {
    & $ghidraRun $projectFile
}
else {
    & $ghidraRun
}
