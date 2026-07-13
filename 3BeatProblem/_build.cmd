@echo off
REM Build wrapper - sidesteps bash snip filter by avoiding forward-slash switches in MSBuild args
"C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" "D:\Github\VST3 Developments\3BeatProblem\projects\3BeatProblem-vst3.vcxproj" -p:Configuration=Release -p:Platform=x64 -v:normal
