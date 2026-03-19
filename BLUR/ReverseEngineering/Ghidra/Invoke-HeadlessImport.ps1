[CmdletBinding()]
param(
    [switch]$NoAnalysis,
    [switch]$SkipDecompile,
    [int]$AnalysisTimeoutPerFile = 900,
    [int]$MaxCpu = 4
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$workflowDir = $PSScriptRoot
$repoRoot = (Resolve-Path (Join-Path $workflowDir '..\..')).Path
$binaryPath = (Resolve-Path (Join-Path $repoRoot 'References\MNSpectralBlurring.component\Contents\MacOS\MNSpectralBlurring')).Path

$projectDir = Join-Path $workflowDir 'project'
$projectName = 'MNSpectralBlurring'
$scriptsDir = Join-Path $workflowDir 'scripts'
$exportsDir = Join-Path $workflowDir 'exports'
$logsDir = Join-Path $workflowDir 'logs'

$javaHome = Join-Path $workflowDir 'tools\jdk-21.0.9+10'
$ghidraHome = 'C:\Users\Razer\AppData\Local\UniGetUI\Chocolatey\lib\ghidra\tools\ghidra_12.0.4_PUBLIC'
$analyzeHeadless = Join-Path $ghidraHome 'support\analyzeHeadless.bat'

if (!(Test-Path $javaHome)) {
    throw "Portable JDK not found at $javaHome"
}

if (!(Test-Path $analyzeHeadless)) {
    throw "Ghidra headless launcher not found at $analyzeHeadless"
}

New-Item -ItemType Directory -Path $projectDir -Force | Out-Null
New-Item -ItemType Directory -Path $exportsDir -Force | Out-Null
New-Item -ItemType Directory -Path $logsDir -Force | Out-Null

$env:JAVA_HOME = $javaHome

$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$logPath = Join-Path $logsDir "analyze-$timestamp.log"
$scriptLogPath = Join-Path $logsDir "scripts-$timestamp.log"

$arguments = @(
    $projectDir,
    $projectName,
    '-import', $binaryPath,
    '-overwrite',
    '-max-cpu', $MaxCpu,
    '-analysisTimeoutPerFile', $AnalysisTimeoutPerFile,
    '-scriptPath', $scriptsDir,
    '-postScript', 'ExportFunctionSummary.java', $exportsDir,
    '-log', $logPath,
    '-scriptlog', $scriptLogPath
)

if (!$SkipDecompile) {
    $arguments += @('-postScript', 'ExportTargetDecomp.java', $exportsDir)
}

if ($NoAnalysis) {
    $arguments += '-noanalysis'
}

& $analyzeHeadless @arguments

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Project directory: $projectDir"
Write-Host "Project name: $projectName"
Write-Host "Imported binary: $binaryPath"
Write-Host "Exports directory: $exportsDir"
Write-Host "Analyzer log: $logPath"
Write-Host "Script log: $scriptLogPath"
