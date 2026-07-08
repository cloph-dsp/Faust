$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$proj = "D:\Github\VST3 Developments\Grungr\projects\Grungr-vst3.vcxproj"
Write-Host "Building: $proj"
& $msbuild $proj /p:Configuration=Release /p:Platform=x64 /p:SolutionDir="D:\Github\VST3 Developments\Grungr\\" /m /v:minimal 2>&1
Write-Host "Exit code: $LASTEXITCODE"
