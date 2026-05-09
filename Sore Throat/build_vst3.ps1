# Create temporary drive mapping to work around spaces in path issue
$mapDrive = "Z:"
$mappedPath = "d:\Github\VST3 Developments\Sore Throat"

# Remove existing mapping if it exists
subst Z: /d 2>$null

# Create new mapping
subst Z: "$mappedPath"

$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$project = "Z:\VChorus\projects\SoreThroat-vst3.vcxproj"
$solutionDir = "Z:\VChorus\"

& $msbuild $project /p:Configuration=Release /p:Platform=x64 /p:SolutionDir=$solutionDir /v:m
$exitCode = $LASTEXITCODE

# Clean up mapping
subst Z: /d 2>$null

Write-Host "Build exit code: $exitCode"
exit $exitCode
