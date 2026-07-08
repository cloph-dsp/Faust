$faustInclude = "C:\Program Files\Faust\include"
$dir          = Split-Path -Parent $PSCommandPath
$dspDir       = Join-Path $dir "..\DSP"
$src          = Join-Path $dir "sanity_check.cpp"
$out          = Join-Path $dir "sanity_check.exe"
$vcvars = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cmd /c "`"$vcvars`" && cl /EHsc /std:c++17 /I`"$faustInclude`" /I`"$dspDir`" `"$src`" /Fe:`"$out`" 2>&1"
if ($LASTEXITCODE -eq 0 -and (Test-Path $out)) { & $out } else { Write-Error "Build failed"; exit 1 }
