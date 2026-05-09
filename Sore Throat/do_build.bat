@echo off
"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe" ^
  "d:\Github\VChorus\VChorus\projects\SoreThroat-vst3.vcxproj" ^
  /p:Configuration=Release ^
  /p:Platform=x64 ^
  "/p:SolutionDir=d:\Github\VChorus\VChorus\\" ^
  /v:m
echo BUILD_EXIT=%ERRORLEVEL%
