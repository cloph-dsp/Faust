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
# Detector split: refactor extracted host-free detector into a HEADER-ONLY
# module (TunerAnalysis.h pulls in TunerAnalysis_impl.h).  No separate .cpp
# means no vcxproj / xcodeproj edit to add a new TU -- the iPlugEffect
# example build sees only a header dependency.
Copy-Item -Force "$src\TunerAnalysis.h"      "$dst\TunerAnalysis.h"
Copy-Item -Force "$src\TunerAnalysis_impl.h" "$dst\TunerAnalysis_impl.h"
Copy-Item -Force "$src\config\IPlugEffect-win.props" "$dst\config\IPlugEffect-win.props"
Copy-Item -Force "$src\scripts\prebuild-win.bat"  "$dst\scripts\prebuild-win.bat"
Copy-Item -Force "$src\scripts\postbuild-win.bat" "$dst\scripts\postbuild-win.bat"
Remove-Item -Force -ErrorAction SilentlyContinue "$dst\TunerDSP.cpp"
# Resources next to main.rc: the vcxproj picks them up via
# <ResourceCompile Include="..\resources\main.rc" />.  main.rc itself
# embeds CLOPH.svg + TEXASLED.TTF into the DLL at build time, so the
# deployed single-file plugin is self-contained -- no sidecar copies
# of the SVG/TTF next to it required.
foreach ($asset in @("main.rc", "CLOPH.svg", "TEXASLED.TTF")) {
    $a = Join-Path "$src\resources" $asset
    $b = Join-Path "$dst\resources" $asset
    New-Item -ItemType Directory -Force -Path (Split-Path $b) | Out-Null
    Copy-Item -Force $a $b
}
# Format-specific Info.plist templates for macOS bundles.  iPlug2 ships
# them as IPlugEffect-{VST3,CLAP,AU,...}-Info.plist; the xcodeproj schemes
# look for <BinaryName>-<Format>-Info.plist.  We copy with the Tuner
# prefix to match Freeze95's pattern -- the CI macOS job then runs
# PlistBuddy over the resulting bundles to overwrite placeholder values
# (CFBundleExecutable, CFBundleIdentifier, AudioComponents:0:subtype, etc.)
foreach ($tpl in @("IPlugEffect-VST3-Info.plist",
                  "IPlugEffect-CLAP-Info.plist",
                  "IPlugEffect-AU-Info.plist")) {
    $dstPath = Join-Path "$dst\resources" ($tpl -replace '^IPlugEffect-', 'Tuner-')
    Copy-Item -Force (Join-Path "$dst\resources" $tpl) $dstPath
}
Write-Host "OK"