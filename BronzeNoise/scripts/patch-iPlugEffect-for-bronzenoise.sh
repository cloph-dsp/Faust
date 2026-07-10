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
# Mirrors the Freeze95 scripts/patch-iPlug2-for-freeze95.sh pattern:
#   - one-shot idempotent post-clone patcher
#   - .bak backup of the original pbxproj preserved
#   - clean restoration if sanity checks fail
#
# Reference implementation that DOES compile fft.c:
#   iPlug2/Examples/IPlugVisualizer/projects/IPlugVisualizer-macOS.xcodeproj
# We mirror its PBXFileReference + PBXBuildFile + PBXSourcesBuildPhase entries
# but scope to the three schemes BronzeNoise actually builds (VST3/CLAP/AUv2).
#
# Usage:
#   bash BronzeNoise/scripts/patch-iPlugEffect-for-bronzenoise.sh
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
# CI layout: workspace-root contains both BronzeNoise/ and iPlug2/. From
# BronzeNoise/scripts the iPlug2 path resolves to BronzeNoise/../../iPlug2.
# Allow PBXPROJ_OVERRIDE env override for ad-hoc local layouts.
PBXPROJ="${PBXPROJ_OVERRIDE:-$PROJECT_ROOT/../../iPlug2/Examples/IPlugEffect/projects/IPlugEffect-macOS.xcodeproj/project.pbxproj}"

# BronzeNoise-distinguishable IDs (4FBAAxxx prefix avoids collision with
# existing iPlug2 IDs which use 4Fxxxxxx patterns).
FFT_FILE_REF="4FBAA0012D1588B8009EF4AA"
FFT_BUILD_VST3="4FBAA0022D1588B8009EF4AA"
FFT_BUILD_CLAP="4FBAA0032D1588B8009EF4AA"
FFT_BUILD_AUV2="4FBAA0042D1588B8009EF4AA"
FFT_REL_PATH="../../../WDL/fft.c"

echo "=== Patching IPlugEffect for BronzeNoise (WDL fft.c) ==="
echo "  Target pbxproj: $PBXPROJ"

if [ ! -f "$PBXPROJ" ]; then
  echo "ERROR: pbxproj not found. Clone iPlug2 into BronzeNoise/iPlug2/ first."
  exit 1
fi

# Idempotency: if our marker ID is already present, assume the patch is applied.
if grep -q "$FFT_FILE_REF" "$PBXPROJ"; then
  echo "  Already patched (found $FFT_FILE_REF). Nothing to do."
  exit 0
fi

cp "$PBXPROJ" "$PBXPROJ.bak"

TMP="$(mktemp)"
awk -v FILE_REF="$FFT_FILE_REF" \
    -v BUILD_VST3="$FFT_BUILD_VST3" \
    -v BUILD_CLAP="$FFT_BUILD_CLAP" \
    -v BUILD_AUV2="$FFT_BUILD_AUV2" \
    -v REL_PATH="$FFT_REL_PATH" '
    # 1. PBXFileReference: insert fft.c ref right after the IPlugPaths.mm ref.
    /4F5F344020C0226200487201 \/\* IPlugPaths\.mm \*\/ = \{isa = PBXFileReference;/ {
      print
      print "\t\t" FILE_REF " /* fft.c */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.c.c; name = fft.c; path = " REL_PATH "; sourceTree = \"<group>\"; };"
      next
    }
    # 2. PBXBuildFile: insert 3 entries right before the PBXBuildFile section
    #    closes (after the IPlugVST3_ProcessorBase.cpp entry, which is the last
    #    PBXBuildFile entry in this pbxproj).
    /B8E22A0C220268C4007CBF4C \/\* IPlugVST3_ProcessorBase\.cpp in Sources \*\/ = \{isa = PBXBuildFile;/ {
      print
      print "\t\t" BUILD_VST3 " /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = " FILE_REF " /* fft.c */; };"
      print "\t\t" BUILD_CLAP " /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = " FILE_REF " /* fft.c */; };"
      print "\t\t" BUILD_AUV2 " /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = " FILE_REF " /* fft.c */; };"
      next
    }
    # 3. PBXSourcesBuildPhase entries: append fft.c to each of the 3 target
    #    schemes. The trailing ",$" anchor restricts the match to the
    #    files = (...) list entries (not the PBXBuildFile declarations).
    /4F1A527E205D911A00CF2908 \/\* IPlugVST3\.cpp in Sources \*\/,$/ {
      print
      print "\t\t\t" BUILD_VST3 " /* fft.c in Sources */,"
      next
    }
    /4FD869DA27206169005A5F28 \/\* IPlugCLAP\.cpp in Sources \*\/,$/ {
      print
      print "\t\t\t" BUILD_CLAP " /* fft.c in Sources */,"
      next
    }
    /4F1A528C205D916F00CF2908 \/\* IPlugAU\.cpp in Sources \*\/,$/ {
      print
      print "\t\t\t" BUILD_AUV2 " /* fft.c in Sources */,"
      next
    }
    { print }
' "$PBXPROJ" > "$TMP"

mv "$TMP" "$PBXPROJ"
rm -f "$TMP"

# Sanity check: each new ID must now be present.
MISSING=0
for ID in "$FFT_FILE_REF" "$FFT_BUILD_VST3" "$FFT_BUILD_CLAP" "$FFT_BUILD_AUV2"; do
  if ! grep -q "$ID" "$PBXPROJ"; then
    echo "ERROR: expected ID $ID missing after patch."
    MISSING=1
  fi
done
if [ "$MISSING" -eq 1 ]; then
  echo "Restoring backup."
  mv "$PBXPROJ.bak" "$PBXPROJ"
  exit 1
fi

echo "  Inserted PBXFileReference + 3 PBXBuildFile entries."
echo "  Linked fft.c into macOS-VST3, macOS-CLAP, macOS-AUv2 Sources phases."
echo "  Backup: $PBXPROJ.bak"
echo "=== Done ==="