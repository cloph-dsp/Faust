# Freeze95 — macOS Install

1. Unzip the downloaded file
2. Drag `Freeze95.vst3` to `~/Library/Audio/Plug-Ins/VST3/`
   (NOT the entire unzipped folder — just the `.vst3` bundle)
3. Open Terminal and run:
   ```bash
   xattr -rd com.apple.quarantine ~/Library/Audio/Plug-Ins/VST3/Freeze95.vst3
   ```
4. In Ableton Live: hold **Option** and click **Rescan**
5. Find Freeze95 under VST3 plugins

## If it still doesn't show up

Check the install path:
```bash
ls ~/Library/Audio/Plug-Ins/VST3/Freeze95.vst3/Contents/MacOS/Freeze95
```
If that says "No such file", you may have installed the wrapper folder.
Delete it and reinstall by dragging ONLY the `.vst3` bundle.

## Verify signature
```bash
codesign -dvv ~/Library/Audio/Plug-Ins/VST3/Freeze95.vst3
```
Expected: `Signature=adhoc` and `flags=0x10002(adhoc,runtime)`
