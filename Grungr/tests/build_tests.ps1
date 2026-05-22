$faustInclude = "C:\Program Files\Faust\include"
$doctestDir = Split-Path -Parent $PSCommandPath
$dspDir = Join-Path $doctestDir "..\DSP"
$sourceFile = Join-Path $doctestDir "test_dsp.cpp"
$outputExe = Join-Path $doctestDir "test_dsp.exe"

Write-Host "Building DSP test executable..."
Write-Host "Faust include: $faustInclude"
Write-Host "Source: $sourceFile"
Write-Host "Output: $outputExe"

# Find MSVC compiler
$vcvars = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if (Test-Path $vcvars) {
    cmd /c "`"$vcvars`" && cl /EHsc /std:c++17 /I`"$faustInclude`" /I`"$dspDir`" /I`"$doctestDir`" `"$sourceFile`" /Fe:`"$outputExe`" 2>&1"
} else {
    $cl = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe"
    if (-not (Test-Path $cl)) {
        # Search for cl.exe
        $cl = Get-ChildItem -Path "C:\Program Files\Microsoft Visual Studio\2022" -Recurse -Filter "cl.exe" -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
    }
    if (Test-Path $cl) {
        & $cl /EHsc /std:c++17 /I"$faustInclude" /I"$dspDir" /I"$doctestDir" "$sourceFile" /Fe:"$outputExe"
    } else {
        Write-Error "MSVC compiler not found"
        exit 1
    }
}

if ($LASTEXITCODE -eq 0 -and (Test-Path $outputExe)) {
    Write-Host "Build succeeded. Running tests..."
    & $outputExe
} else {
    Write-Error "Build failed"
    exit 1
}
