$msbuild = 'C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe'
$proj = 'D:\Github\VST3 Developments\3BeatProblem\projects\3BeatProblem-vst3.vcxproj'
& $msbuild $proj /p:Configuration=Release /p:Platform=x64 /v:minimal /nologo /m:1 2>&1 | Out-File -FilePath 'D:\Github\VST3 Developments\.omo\evidence\task-5-brace-elision-build.log'
$ec = $LASTEXITCODE
Write-Output "EXIT=$ec"
$log = Get-Content 'D:\Github\VST3 Developments\.omo\evidence\task-5-brace-elision-build.log'
$errors = Select-String -InputObject $log -Pattern 'error\s+C\d+' -AllMatches
Write-Output "ERRORS=$($errors.Matches.Count)"
$errors | Select-Object -First 5
