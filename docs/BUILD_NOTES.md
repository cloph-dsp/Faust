# Build & Release Notes

## Freeze95 CI Pipeline

### Trigger
- CI only builds on pushes to `main` branch
- Path filter: only `Freeze95/**` or `.github/workflows/build.yml` changes trigger it
- Manual trigger via `workflow_dispatch` from any branch

### Build Artifacts

| Format | Platform | File Type | Upload Method |
|--------|----------|-----------|---------------|
| VST3   | Windows  | Single `.vst3` file | Direct upload |
| CLAP   | Windows  | Single `.clap` file | Direct upload |
| VST3   | macOS    | Directory bundle | Stage in wrapper dir, then upload |
| CLAP   | macOS    | Directory bundle | Stage in wrapper dir, then upload |

### macOS Bundle Upload (Critical!)
On macOS, `.vst3` and `.clap` are **directory bundles** (like `.app` folders).  
**Do NOT upload them directly** — `upload-artifact@v4` strips the root folder name.  
**Fix:** Copy the bundle into a staging directory and upload the staging directory:

```yaml
- name: Stage VST3 artifact
  run: |
    mkdir -p stage-vst3
    cp -R build-mac/Products/Freeze95.vst3 stage-vst3/

- name: Upload
  uses: actions/upload-artifact@v4
  with:
    name: Freeze95-macOS-VST3
    path: stage-vst3/
```

This gives users a single unzip → `Freeze95.vst3/` folder.

**Note:** AU (Audio Unit / `.component`) is not built — requires paid Apple Developer ID signing ($99/yr), not viable for CI currently. GarageBand is AU-only, so Freeze95 won't work there.

### Binary Magic Numbers
- `4D 5A` = Windows PE executable (valid)
- `CA FE BA BE` with `00 00 00 02` = macOS fat/universal binary (valid, 2 arch)
  - This is NOT "Java bytecode" — it's the same magic number used for fat Mach-O
  - Verify with `file` and `lipo -info` on macOS

---

## Freeze95 Known Issues & Fixes

### 1. Cakewalk Sonar / VirtualDAW — DPI Rendering Issue
**Root cause:** Forcing initial scale to 1.0 on Windows contradicted the DPI awareness manifest (`dpiAware: true` + `PerMonitorV2`). Windows expected a DPI-aware plugin to scale itself, but scale=1.0 made the physical window too small on high-DPI displays.

**Fix (Freeze95.cpp, constructor):** Use `GetScaleForScreen()` universally on all platforms. The DPI awareness manifest tells Windows the plugin handles DPI; `GetScaleForScreen()` reads the actual screen DPI and returns the correct initial scale. `OnParentWindowResize()` then adjusts if the host provides different dimensions.

```cpp
auto* graphics = MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS,
                              GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
```

**Required:** Windows DPI awareness manifest (`Freeze95/resources/Freeze95.manifest`) embedded via `RT_MANIFEST` in `main.rc`.

### 2. FL Studio — Crashes After ~Minutes (fixed v1.3)
**Root cause:** FTZ/DAZ (`_mm_setcsr`) was placed AFTER the bypass/transport-gate early returns. When transport was paused, denormals accumulated in the 6× 2M-sample delay buffers (~48 MB). On resume, the denormals caused CPU spikes → audio dropout.

**Fix (Freeze95.cpp):**
- Moved `_mm_setcsr(_mm_getcsr() | 0x8040)` to the **very top** of `ProcessBlock()` — before any `return` statement.
- Also added it to `OnReset()` so denormals are flushed during sample rate changes.

### 3. macOS Bundle Missing Binary (fixed v1.3)
**Root cause:** CI built from `main` branch (old code) instead of `gui-rework` branch (which had the universal binary fix). Fixed by merging `gui-rework` → `main`.

**Root cause 2:** Artifact upload stripped the `.vst3` wrapper folder (see "macOS Bundle Upload" above). Fixed by staging in wrapper dir.

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0   | -    | Initial release (macOS missing binary) |
| 1.1.0   | -    | DSP hardening, GUI tint overlay (washed out) |
| 1.2.0   | -    | Universal binary, GUI color fix, CI verification |
| 1.3.0   | May 2026 | DPI fix, FL Studio crash fix, macOS bundle fix |

## Config Reference
- Version hex: `PLUG_VERSION_HEX 0x00010300` = v1.3.0
- Version string: `PLUG_VERSION_STR "1.3.0"`
- Located in `Freeze95/config.h`
