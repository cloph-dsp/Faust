$src = "D:\Github\VST3 Developments\Tuner VST"
$dst = "D:\Github\VST3 Developments\iPlug2\Examples\IPlugEffect"
Copy-Item -Force "$src\Tuner.cpp"          "$dst\IPlugEffect.cpp"
Copy-Item -Force "$src\Tuner.h"            "$dst\IPlugEffect.h"
Copy-Item -Force "$src\Tuner.h"            "$dst\Tuner.h"
Copy-Item -Force "$src\config.h"           "$dst\config.h"
Copy-Item -Force "$src\TunerDSP.h"         "$dst\TunerDSP.h"
Copy-Item -Force "$src\TunerDSP.dsp"       "$dst\TunerDSP.dsp"
Copy-Item -Force "$src\TunerDSP_gen.h"     "$dst\TunerDSP_gen.h"
Copy-Item -Force "$src\FaustCompat.h"      "$dst\FaustCompat.h"
Copy-Item -Force "$src\config\IPlugEffect-win.props" "$dst\config\IPlugEffect-win.props"
Copy-Item -Force "$src\scripts\prebuild-win.bat"  "$dst\scripts\prebuild-win.bat"
Copy-Item -Force "$src\scripts\postbuild-win.bat" "$dst\scripts\postbuild-win.bat"
Remove-Item -Force -ErrorAction SilentlyContinue "$dst\TunerDSP.cpp"
# Copy resources next to main.rc so the vcxproj picks them up via
# <ResourceCompile Include="..\resources\main.rc" />.  main.rc itself
# embeds CLOPH.svg + TEXASLED.TTF into the DLL at build time, so the
# deployed single-file .vst3 is self-contained -- no sidecar copies
# of the SVG/TTF next to it required.
foreach ($asset in @("main.rc", "CLOPH.svg", "TEXASLED.TTF")) {
    $a = Join-Path "$src\resources" $asset
    $b = Join-Path "$dst\resources" $asset
    New-Item -ItemType Directory -Force -Path (Split-Path $b) | Out-Null
    Copy-Item -Force $a $b
}
Write-Host "OK"