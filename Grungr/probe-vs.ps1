$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
Write-Host "vswhere: $vswhere exists=$(Test-Path $vswhere)"
$all = & $vswhere -all -prerelease -property installationPath
Write-Host "VS installations:`n$all"
$msb = & $vswhere -latest -prerelease -find MSBuild\**\Bin\MSBuild.exe
Write-Host "MSBuild finds (no -requires):`n$msb"
$vsdev = & $vswhere -latest -prerelease -find **\VsDevCmd.bat
Write-Host "VsDevCmd:`n$vsdev"
$vcvars = & $vswhere -latest -prerelease -find VC\Auxiliary\Build\vcvarsall.bat
Write-Host "vcvarsall:`n$vcvars"
