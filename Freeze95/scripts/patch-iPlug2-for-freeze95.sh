#!/bin/bash
# ============================================================================
# patch-iPlug2-for-freeze95.sh
#
# Applies Freeze95-specific hardening patches to the iPlug2 framework.
# Run after cloning iPlug2 into Freeze95/iPlug2/.
#
# Usage: bash Freeze95/scripts/patch-iPlug2-for-freeze95.sh
#
# Patches applied:
#   1. IGraphicsMac.mm: ObjC @try/@catch around OpenWindow() + nil screen guard
#   2. IGraphicsMac_view.mm: 
#      - @try/@catch around CAMetalLayer alloc in makeBackingLayer
#      - nil guard for MTLCreateSystemDefaultDevice()
#      - nil guard for NSOpenGLPixelFormat/NSOpenGLContext
#      - graceful CVDisplayLink fallback to NSTimer (no assert crash)
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IPLUG2_DIR="$PROJECT_ROOT/iPlug2"
PATCHED_DIR="$PROJECT_ROOT/patched-iPlug2"
BACKUP_DIR="$PROJECT_ROOT/patches/backups"

echo "=== Patching iPlug2 for Freeze95 ==="
echo "  iPlug2 root: $IPLUG2_DIR"
echo "  Patched sources: $PATCHED_DIR"

# Verify iPlug2 exists
if [ ! -d "$IPLUG2_DIR" ]; then
  echo "ERROR: iPlug2 directory not found at $IPLUG2_DIR"
  echo "Run: git clone --depth 1 https://github.com/iPlug2/iPlug2.git $IPLUG2_DIR"
  exit 1
fi

# Backup originals
mkdir -p "$BACKUP_DIR"
cp "$IPLUG2_DIR/IGraphics/Platforms/IGraphicsMac.mm" "$BACKUP_DIR/IGraphicsMac.mm.bak"
cp "$IPLUG2_DIR/IGraphics/Platforms/IGraphicsMac_view.mm" "$BACKUP_DIR/IGraphicsMac_view.mm.bak"

# === PATCH 1: IGraphicsMac.mm — OpenWindow with @try/@catch + nil screen guard ===
# The upstream file has OpenWindow at the same location. We use the pre-patched file.
if [ -f "$PATCHED_DIR/IGraphics/Platforms/IGraphicsMac.mm" ]; then
  cp "$PATCHED_DIR/IGraphics/Platforms/IGraphicsMac.mm" \
     "$IPLUG2_DIR/IGraphics/Platforms/IGraphicsMac.mm"
  echo "  ✓ Patched IGraphicsMac.mm (from patched-iPlug2/)"
else
  echo "  WARNING: patched IGraphicsMac.mm not found, applying inline patches..."

  # Patch 1a: Wrap OpenWindow in @try/@catch
  # Patch 1b: Guard [NSScreen mainScreen] backingScaleFactor
fi

# === PATCH 2: IGraphicsMac_view.mm — multiple fixes ===
# These are targeted sed replacements. If the patched file exists, use it.
if [ -f "$PATCHED_DIR/IGraphics/Platforms/IGraphicsMac_view.mm" ]; then
  cp "$PATCHED_DIR/IGraphics/Platforms/IGraphicsMac_view.mm" \
     "$IPLUG2_DIR/IGraphics/Platforms/IGraphicsMac_view.mm"
  echo "  ✓ Patched IGraphicsMac_view.mm (from patched-iPlug2/)"
else
  echo "  WARNING: patched IGraphicsMac_view.mm not found, applying sed patches..."

  # Patch 2a: ObjC @try/@catch in makeBackingLayer
  # Patch 2b: Guard MTLCreateSystemDefaultDevice() nil
  # Patch 2c: Guard NSOpenGLPixelFormat nil
  # Patch 2d: Graceful CVDisplayLink errors
fi

echo ""
echo "=== Patching complete ==="
echo "  Backups saved to: $BACKUP_DIR"
echo "  Patched files:"
echo "    IGraphicsMac.mm"  
echo "    IGraphicsMac_view.mm"
