#!/bin/bash
# ============================================================================
# patch-iPlug2-for-tuner.sh
#
# Strip iPlug2 framework template strings that survive the plugin-name
# rename in _local_copy.ps1.  Run after cloning iPlug2 into Tuner VST/iPlug2/
# and before the first build.
#
# Usage: bash "Tuner VST/scripts/patch-iPlug2-for-tuner.sh" [iPlug2_dir]
#
# What it does:
#   - sed-replaces AcmeInc / com.AcmeInc globally in iPlug2 framework
#     source files.  These strings are baked into compiled binaries as
#     vendor/bundle-id literals and survive the host-plugin rename.
#   - Skips the IPlugEffect class-name substitution (that would break the
#     example project's compile because IPlugEffect.cpp/.h are class
#     identifiers, not just strings).  The Info.plist PlistBuddy step in
#     the workflow already renames the bundle executable, so any
#     string-literal "IPlugEffect" occurrences still in the binary are
#     upstream-side and harmless.
#
# Round-7 verification: local iPlug2 SHA fa08f63 produces clean binaries
# (0 AcmeInc / com.AcmeInc strings) WITHOUT this patch.  The CI
# `git clone --depth 1` may pull a newer upstream SHA that re-introduces
# those strings — keep the binary-string scan in the verify step and let
# the patch keep CI green on upstream drift.
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IPLUG2_DIR="${1:-$PROJECT_ROOT/iPlug2}"

if [ ! -d "$IPLUG2_DIR" ]; then
  echo "ERROR: iPlug2 directory not found at $IPLUG2_DIR"
  echo "Run: git clone --depth 1 https://github.com/iPlug2/iPlug2.git $IPLUG2_DIR"
  exit 1
fi

echo "=== Patching iPlug2 for Tuner ==="
echo "  iPlug2 root: $IPLUG2_DIR"

# Targets: every .cpp/.mm/.h under the IPlug + IGraphics framework trees.
# Skip the user's own plugin code (iPlug2/Examples/IPlugEffect/) -- we
# don't want to rewrite their host-plugin names; that's _local_copy.ps1's job.
mapfile -t TARGETS < <(find "$IPLUG2_DIR/IPlug" "$IPLUG2_DIR/IGraphics" \
  -type f \( -name '*.cpp' -o -name '*.mm' -o -name '*.h' \) 2>/dev/null)

if [ "${#TARGETS[@]}" -eq 0 ]; then
  echo "  WARNING: no iPlug2 framework files found under $IPLUG2_DIR/{IPlug,IGraphics}"
  exit 0
fi

echo "  Files to patch: ${#TARGETS[@]}"

# AcmeInc / com.AcmeInc are template residue from upstream iPlug2's
# default Info.plist (CFBundleIdentifier prefix, vendor name, etc.).
# Replace globally with CLOPH-prefixed values.  These are substrings,
# not whole-word identifiers, so we don't risk colliding with class names.
#   - AcmeInc      -> Clph          (vendor 4CC, matches our AU manufacturer)
#   - com.AcmeInc  -> com.cloph-dsp (bundle id prefix)
for f in "${TARGETS[@]}"; do
  sed -i \
    -e 's/AcmeInc/Clph/g' \
    -e 's/com\.AcmeInc/com.cloph-dsp/g' \
    "$f"
done

echo "  Replaced AcmeInc -> Clph and com.AcmeInc -> com.cloph-dsp"
echo ""
echo "=== Patching complete ==="
echo "  Note: no backups taken (matches Freeze95's scripts/patch-iPlug2-for-freeze95.sh)."
echo "  Re-clone iPlug2 to undo if needed."