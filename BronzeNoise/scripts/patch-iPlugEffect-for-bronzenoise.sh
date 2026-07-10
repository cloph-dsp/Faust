#!/bin/bash
# ============================================================================
# patch-iPlugEffect-for-bronzenoise.sh
#
# Adds WDL/fft.c (path: ../../../WDL/fft.c) as a Sources build entry to the
# iPlug2 IPlugEffect-macOS.xcodeproj's project.pbxproj. BronzeNoise uses
# WDL FFT symbols (WDL_fft_init, WDL_fft, WDL_fft_permute) in ProcessHop
# and OnReset, but the upstream IPlugEffect project does not compile fft.c,
# causing link errors when building the macOS-VST3 / macOS-CLAP / macOS-AUv2
# schemes.
#
# Mirrors the Freeze95 scripts/patch-iPlug2-for-freeze95.sh pattern.
# Idempotent (skips when marker UUID is present), .bak preserved.
#
# Reference implementation that DOES compile fft.c:
#   iPlug2/Examples/IPlugVisualizer/projects/IPlugVisualizer-macOS.xcodeproj
# ============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PBXPROJ="$PROJECT_ROOT/iPlug2/Examples/IPlugEffect/projects/IPlugEffect-macOS.xcodeproj/project.pbxproj"

# BronzeNoise-distinguishable IDs (4FBAAxxx prefix avoids collision with
# iPlug2 IDs which are 4Fxxxxxx patterns).
FFT_FILE_REF="4FBAA0012D1588B8009EF4AA"
FFT_BUILD_VST3="4FBAA0022D1588B8009EF4AA"
FFT_BUILD_CLAP="4FBAA0032D1588B8009EF4AA"
FFT_BUILD_AUV2="4FBAA0042D1588B8009EF4AA"
FFT_REL_PATH="../../../WDL/fft.c"

echo "=== Patching IPlugEffect for BronzeNoise (WDL fft.c) ==="
echo "  Target pbxproj: $PBXPROJ"

if [ ! -f "$PBXPROJ" ]; then
  echo "ERROR: pbxproj not found. Run from a checkout where iPlug2/ is a sibling of BronzeNoise/scripts/."
  exit 1
fi

if grep -q "$FFT_FILE_REF" "$PBXPROJ"; then
  echo "  Already patched (found $FFT_FILE_REF). Nothing to do."
  exit 0
fi

cp "$PBXPROJ" "$PBXPROJ.bak"
chmod u+w "$PBXPROJ"

python3 - "$PBXPROJ" "$FFT_FILE_REF" "$FFT_BUILD_VST3" "$FFT_BUILD_CLAP" "$FFT_BUILD_AUV2" "$FFT_REL_PATH" <<'PYEOF'
import sys, re
pbp, ff, vst3, clap, auv2, relpath = sys.argv[1:7]
src = open(pbp).read()

# 1. PBXFileReference: insert fft.c ref right after the IPlugPaths.mm ref
needle = "4F5F344020C0226200487201 /* IPlugPaths.mm */ = {isa = PBXFileReference;"
patch  = (
    f"\t\t{ff} /* fft.c */ = {{isa = PBXFileReference; "
    f"lastKnownFileType = sourcecode.c.c; name = fft.c; path = "
    f"{relpath}; sourceTree = \"<group>\"; }};\n"
)
src = src.replace(needle, needle + "\n" + patch.rstrip("\n"), 1)

# 2. PBXBuildFile (3 entries) before the End PBXBuildFile marker.
build_block = (
    f"\t\t{vst3} /* fft.c in Sources */ = {{isa = PBXBuildFile; fileRef = {ff} /* fft.c */; }};\n"
    f"\t\t{clap} /* fft.c in Sources */ = {{isa = PBXBuildFile; fileRef = {ff} /* fft.c */; }};\n"
    f"\t\t{auv2} /* fft.c in Sources */ = {{isa = PBXBuildFile; fileRef = {ff} /* fft.c */; }};\n"
)
src = src.replace("/* End PBXBuildFile section */",
                  build_block + "/* End PBXBuildFile section */", 1)

# 3. Append fft.c to each of the 3 PBXSourcesBuildPhase files = ( ... ) lists.
#    Each scheme's Sources-phase block contains a unique identifier of its
#    last file entry (we use IPlugVST3.cpp / IPlugCLAP.cpp / IPlugAU.cpp).
markers = [
    (vst3,  "4F1A527E205D911A00CF2908 /* IPlugVST3.cpp in Sources */,"),
    (clap,  "4FD869DA27206169005A5F28 /* IPlugCLAP.cpp in Sources */,"),
    (auv2,  "4F1A528C205D916F00CF2908 /* IPlugAU.cpp in Sources */,"),
]

# But the same string can appear in BOTH the PBXBuildFile section AND the
# PBXSourcesBuildPhase files = (...) list. We must only patch the SECOND
# occurrence (the files list entry), not the PBXBuildFile declaration.
patches = 0
for bfid, marker in markers:
    occurrences = [i for i in range(len(src)) if src.startswith(marker, i)]
    if len(occurrences) < 2:
        print(f"WARN: only {len(occurrences)} occurrence(s) of {marker[:30]}..., skipping")
        continue
    target_pos = occurrences[1]  # second occurrence = inside files = (...) list
    end_of_marker = target_pos + len(marker)
    insert_text = f"\n\t\t\t\t{bfid} /* fft.c in Sources */,"
    src = src[:end_of_marker] + insert_text + src[end_of_marker:]
    patches += 1

open(pbp, 'w').write(src)
print(f"patch: file ref + 3 build files + {patches}/{len(markers)} sources phases")
PYEOF

for ID in "$FFT_FILE_REF" "$FFT_BUILD_VST3" "$FFT_BUILD_CLAP" "$FFT_BUILD_AUV2"; do
  if ! grep -q "$ID" "$PBXPROJ"; then
    echo "ERROR: expected ID $ID missing after patch. Restoring backup."
    mv "$PBXPROJ.bak" "$PBXPROJ"
    exit 1
  fi
done

echo "  Inserted PBXFileReference + 3 PBXBuildFile entries."
echo "  Linked fft.c into macOS-VST3, macOS-CLAP, macOS-AUv2 Sources phases."
echo "  Backup: $PBXPROJ.bak"
echo "=== Done ==="