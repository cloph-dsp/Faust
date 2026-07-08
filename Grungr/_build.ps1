$ErrorActionPreference = 'Stop'
$env:Path = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\amd64;$env:Path"
$proj = "D:\Github\VST3 Developments\Grungr\projects\Grungr-vst3.vcxproj"
$solDir = 'D:\Github\VST3 Developments\Grungr\\'
$start = Get-Date
Write-Host "Building Grungr-vst3..."
$output = & msbuild.exe $proj /p:Configuration=Release /p:Platform=x64 "/p:SolutionDir=$solDir" /t:Build /v:m 2>&1
$exit = $LASTEXITCODE
$tail = ($output | Select-Object -Last 40)
Write-Host $tail
if ($exit -ne 0) { Write-Host "BUILD FAILED ($exit)"; exit $exit }
Write-Host "BUILD OK in $((Get-Date).Subtract($start).TotalSeconds)s"
exit 0
