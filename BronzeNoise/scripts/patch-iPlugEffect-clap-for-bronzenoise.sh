#!/bin/bash
# ============================================================================
# patch-iPlugEffect-clap-for-bronzenoise.sh
#
# Adds WDL/fft.c (path: ../../../WDL/fft.c) as a ClCompile entry to the
# iPlug2 IPlugEffect-clap.vcxproj. BronzeNoise uses WDL FFT symbols
# (WDL_fft_init, WDL_fft, WDL_fft_permute) in ProcessHop and OnReset,
# but the upstream IPlugEffect-clap project does not compile fft.c,
# causing LNK2001 unresolved externals at link time.
#
# Mirrors the Freeze95 scripts/patch-iPlug2-for-freeze95.sh pattern:
#   - one-shot idempotent post-clone patcher
#   - .bak backup of the original vcxproj preserved
#   - clean restoration if sanity checks fail
#
# Reference (vcxproj that DOES compile fft.c): BronzeNoise-vst3.vcxproj has
#   <ClCompile Include="..\..\iPlug2\WDL\fft.c" />
#
# Usage:
#   bash BronzeNoise/scripts/patch-iPlugEffect-clap-for-bronzenoise.sh
# ============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
# CI layout: workspace-root contains both BronzeNoise/ and iPlug2/. From
# BronzeNoise/scripts the iPlug2 path resolves to BronzeNoise/../../iPlug2.
VCXPROJ="${VCXPROJ_OVERRIDE:-$WORKSPACE_ROOT/iPlug2/Examples/IPlugEffect/projects/IPlugEffect-clap.vcxproj}"
FFT_REL_PATH="..\..\..\WDL\fft.c"
# Deterministic marker GUID (24 hex chars). Same pattern as macOS patcher.
MARKER="4FBAA1012D1588B8009EF4AA"

echo "=== Patching IPlugEffect-clap.vcxproj for BronzeNoise (WDL fft.c) ==="
echo "  Target vcxproj: $VCXPROJ"

if [ ! -f "$VCXPROJ" ]; then
  echo "ERROR: vcxproj not found. Run from a checkout where iPlug2/ is a sibling of BronzeNoise/scripts/."
  exit 1
fi

if grep -q "$MARKER" "$VCXPROJ"; then
  echo "  Already patched (found $MARKER). Nothing to do."
  exit 0
fi

cp "$VCXPROJ" "$VCXPROJ.bak"
chmod u+w "$VCXPROJ"

# Use Python for safe multi-line XML mutation (awk can struggle with attributes).
python3 - "$VCXPROJ" "$MARKER" "$FFT_REL_PATH" <<'PYEOF'
import sys, re
vcxproj, marker, fft_rel = sys.argv[1:4]
src = open(vcxproj).read()
# Locate a stable anchor in <ClCompile Include=...> form. Use the IPlugCLAP.cpp
# entry as our insertion site (last iPlug-path ClCompile, followed by the
# IPlugVST3_ProcessorBase.cpp, IPlugEditorDelegate.cpp entries etc.).
needle = '<ClCompile Include="..\\..\\..\\IPlug\\CLAP\\IPlugCLAP.cpp" />'
patch = (
    f'<ClCompile Include="{fft_rel}">\n'
    f'      <PreprocessorDefinitions>%(PreprocessorDefinitions)</PreprocessorDefinitions>\n'
    f'    </ClCompile>\n'
    f'    <ClCompile Include="{fft_rel}" />\n'
    f'    <!-- BronzeNoiseMarker:{marker} fft.c added for BronzeNoise (WDL_fft symbols) -->\n'
    f'    {needle}'
)
if needle not in src:
    print('ERROR: anchor needle not found in vcxproj')
    sys.exit(1)
src = src.replace(needle, patch, 1)
open(vcxproj, 'w').write(src)
print(f'patch: inserted fft.c ClCompile entry with marker {marker}')
PYEOF

if ! grep -q "$MARKER" "$VCXPROJ"; then
  echo "ERROR: marker $MARKER missing after patch. Restoring backup."
  mv "$VCXPROJ.bak" "$VCXPROJ"
  exit 1
fi

echo "  Inserted <ClCompile Include=\"$FFT_REL_PATH\" /> at IPlugCLAP.cpp anchor."
echo "  Backup: $VCXPROJ.bak"
echo "=== Done ==="
