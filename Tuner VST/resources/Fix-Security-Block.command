#!/bin/bash
#
#  cloph-dsp — macOS install & "unblock" helper
#  ------------------------------------------------------------------
#  macOS tags files downloaded from the internet with a "quarantine"
#  flag. Because these plugins are code-signed but not notarised by
#  Apple, that flag makes your DAW silently skip them. This helper
#  installs every plugin sitting next to it into your personal
#  plug-in folders and removes the quarantine flag. No password needed.
#
#  HOW TO RUN: right-click this file > Open > Open. (A plain double-click
#  may be blocked the first time, because the script itself was
#  downloaded from the internet.)
#
echo "cloph-dsp macOS installer"
echo "========================="

HERE="$(cd "$(dirname "$0")" && pwd)"
COMP="$HOME/Library/Audio/Plug-Ins/Components"
VST3="$HOME/Library/Audio/Plug-Ins/VST3"
CLAP="$HOME/Library/Audio/Plug-Ins/CLAP"
mkdir -p "$COMP" "$VST3" "$CLAP"

found=0
install_bundle() {   # $1 = source bundle  $2 = destination dir  $3 = label
  local name; name="$(basename "$1")"
  rm -rf "$2/$name"
  if ! cp -R "$1" "$2/"; then echo "  ! failed to copy $name"; return; fi
  xattr -dr com.apple.quarantine "$2/$name" 2>/dev/null
  echo "  installed $3: $name"
  found=1
}

for b in "$HERE"/*.component; do [ -e "$b" ] && install_bundle "$b" "$COMP" "Audio Unit"; done
for b in "$HERE"/*.vst3;      do [ -e "$b" ] && install_bundle "$b" "$VST3" "VST3"; done
for b in "$HERE"/*.clap;      do [ -e "$b" ] && install_bundle "$b" "$CLAP" "CLAP"; done

# Make Logic / GarageBand rebuild their Audio Unit list on next launch.
rm -rf "$HOME/Library/Caches/AudioUnitCache" 2>/dev/null

echo ""
if [ "$found" -eq 1 ]; then
  echo "All done. Now open (or restart) your DAW."
  echo "In Logic Pro the Audio Unit appears under the cloph-dsp / CLOPH manufacturer."
else
  echo "No .component / .vst3 / .clap plugins were found next to this script."
  echo "Keep this file in the same folder as the plugin bundle when you run it."
fi
echo ""
echo "You can close this window."
