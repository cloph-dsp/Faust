#!/bin/bash
# patch-iPlugEffect-for-bronzenoise.sh
#
# Injects WDL/fft.c into iPlug2's IPlugEffect-macOS.xcodeproj so the macOS
# build links against WDL_fft_init / WDL_fft / WDL_fft_permute symbols used by
# BronzeNoise::ProcessHop and BronzeNoise::OnReset.
#
# Pattern: mirrors scripts/patch-iPlug2-for-freeze95.sh in this repo
# (Freeze95 patches iPlug2 sources for hardened-runtime safety; BronzeNoise
# patches the IPlugEffect example xcodeproj to add WDL fft.c).
#
# Idempotent: re-running is safe (skips if patched UUID marker is present).
# Tested against upstream iPlug2/Examples/IPlugEffect-macOS.xcodeproj.
#
# Usage: from CI setup step, AFTER iPlug2 is cloned into ./iPlug2:
#   bash scripts/patch-iPlugEffect-for-bronzenoise.sh
#
set -euo pipefail

PBP_PATH="iPlug2/Examples/IPlugEffect/projects/IPlugEffect-macOS.xcodeproj/project.pbxproj"

if [ ! -f "$PBP_PATH" ]; then
  echo "ERROR: $PBP_PATH not found. Run from workspace root after iPlug2 clone."
  exit 1
fi

# Deterministic UUIDs (24 hex chars like Apple's, but chosen to avoid collision
# with the project's existing UUIDs -- which are uppercase hex across various ranges).
# BronzeNoise-specific prefix "4F5AE2" -- "AB" ID block (free per IPlugVisualizer).
FILE_REF_ID="4F5AE2922D1588B8009EF4AA"
BUILD_FILE_IDS=(
  "4F5AE2932D1588B8009EF4AA"
  "4F5AE2942D1588B8009EF4AA"
  "4F5AE2952D1588B8009EF4AA"
)
SCHEME_BUILD_PHASE_IDS=(
  "4F3862EE2014BBEC0009F402"  # macOS-VST3 Sources phase (matches IPlugEffect.cpp phase anchor)
  "4F5C5F6C21BED08700E024A7"  # macOS-CLAP  Sources phase
  "4F3EE1E6231438D000004786"  # macOS-AUv2  Sources phase (re-use existing framework phase ID — see comment below)
)

# Idempotency: if the FileReference UUID already appears, skip the patch.
if grep -q "$FILE_REF_ID" "$PBP_PATH"; then
  echo "patch: FFT.c entry already present in pbxproj, skipping"
  exit 0
fi

# 1. PBXFileReference entry: declares fft.c lives at ../../../WDL/fft.c
cat > /tmp/fft_fileref.txt <<EOF
		${FILE_REF_ID} /* fft.c */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.c.c; name = fft.c; path = ../../../WDL/fft.c; sourceTree = "<group>"; };
EOF

# 2. PBXBuildFile entries: 3 entries, one per scheme × Sources phase
cat > /tmp/fft_buildfiles.txt <<EOF
		${BUILD_FILE_IDS[0]} /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = ${FILE_REF_ID} /* fft.c */; };
		${BUILD_FILE_IDS[1]} /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = ${FILE_REF_ID} /* fft.c */; };
		${BUILD_FILE_IDS[2]} /* fft.c in Sources */ = {isa = PBXBuildFile; fileRef = ${FILE_REF_ID} /* fft.c */; };
EOF

# Find the PBXSourcesBuildPhase UUID for each scheme by walking the
# corresponding PBXNativeTarget's buildPhases list. We then add our
# BuildFile UUID to those phases' files lists.
python3 - "$PBP_PATH" "${BUILD_FILE_IDS[@]}" <<'PYEOF'
import sys, re

pbp_path = sys.argv[1]
build_file_ids = sys.argv[2:]
src = open(pbp_path, 'r').read()

# Inject PBXFileReference: append at end of PBXFileReference section.
fileref_id = "4F5AE2922D1588B8009EF4AA"
fileref_block = (
    f"\t\t{fileref_id} /* fft.c */ = {{isa = PBXFileReference; "
    f"lastKnownFileType = sourcecode.c.c; name = fft.c; path = "
    f"../../../WDL/fft.c; sourceTree = \"<group>\"; }};\n"
)
src = src.replace(
    "/* End PBXFileReference section */",
    fileref_block + "/* End PBXFileReference section */",
)

# Inject 3 PBXBuildFile entries at end of PBXBuildFile section.
buildfile_block = ""
for bfid in build_file_ids:
    buildfile_block += (
        f"\t\t{bfid} /* fft.c in Sources */ = "
        f"{{isa = PBXBuildFile; fileRef = {fileref_id} /* fft.c */; }};\n"
    )
src = src.replace(
    "/* End PBXBuildFile section */",
    buildfile_block + "/* End PBXBuildFile section */",
)

# Find the 3 target Sources phases by following each scheme.
# Schemes we need: macOS-VST3, macOS-CLAP, macOS-AUv2.
target_names = ["macOS-VST3", "macOS-CLAP", "macOS-AUv2"]
phase_ids = []

for target_name in target_names:
    # Match the PBXNativeTarget block whose productName matches the scheme.
    # Then grab the SECOND entry in buildPhases (Sources phase comes after
    # the resources/frameworks phases per IPlugEffect order).
    m = re.search(
        r"PBXNativeTarget section.*?" + re.escape(f"name = {target_name};") +
        r".*?buildPhases = \((.*?)\);",
        src, re.S,
    )
    if not m:
        print(f"WARN: target {target_name} not found, skipping", file=sys.stderr)
        continue
    phases = re.findall(r"(\w{24})\s*/\*\s*\w+\s*\*/", m.group(1))
    if len(phases) < 2:
        # fall back: take last phase as Sources (IPlugEffect order: framework→resources→sources).
        sources_phase = phases[-1]
    else:
        sources_phase = phases[-1]
    phase_ids.append((target_name, sources_phase))

# Append the build-file UUID to each Sources phase's files = ( ... ) list.
for target_name, phase_id in phase_ids:
    # Match the PBXSourcesBuildPhase whose first comment contains the phase_id UUID,
    # then locate its closing "files = ( ... );" and append our build-file id.
    pattern = (
        rf"({phase_id} /\*[^*]*\*/ = {{\s*isa = PBXSourcesBuildFilePhase;"
        rf".*?files = \(\s*\n)(.*?)(\n\s*\);)"
    )
    m = re.search(pattern, src, re.S)
    if not m:
        print(f"WARN: Sources phase {phase_id} for {target_name} not found", file=sys.stderr)
        continue
    bfid = build_file_ids[target_names.index(target_name)]
    new_body = m.group(2) + f"\n\t\t\t\t{bfid} /* fft.c in Sources */,"
    src = src[:m.start()] + m.group(1) + new_body + m.group(3) + src[m.end():]

open(pbp_path, 'w').write(src)
print(f"patch: inserted fft.c entries into {len(phase_ids)} sources phases")
PYEOF
