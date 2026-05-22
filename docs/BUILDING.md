# Building & Deployment

## Prerequisites
- Visual Studio 2022 (v143 toolset)
- VST3 SDK (via iPlug2)
- Windows 10/11 x64
- Python virtual environment (`.venv/`) — optional, used by some tools

## Building a Plugin

### Method 1: Visual Studio (Recommended)
```cmd
# Open the plugin's .vcxproj in Visual Studio
cd D:\Github\VST3 Developments\DataBend\projects
start DataBend-vst3.vcxproj

# Build → Configuration Manager → Select "Release" and "x64"
# Build → Build Solution (F7)
```

**Output:**  
`build-win/vst3/x64/Release/` → contains `.vst3` directory

Post-build scripts automatically:
1. Create `.vst3` bundle structure
2. Copy compiled binary to `Contents/x86_64-win/`
3. Copy VST3 to system VST3 folder (if configured)
4. Handle VST2/AAX deployment for multi-format builds

### Method 2: Command Line (MSBuild)
```cmd
cd D:\Github\VST3 Developments\DataBend\projects
msbuild DataBend-vst3.vcxproj /p:Configuration=Release /p:Platform=x64
```

## Build Targets

| Plugin | Config.h Defines | Output | Format |
|--------|------------------|--------|--------|
| **DataBend** | PLUG_CHANNEL_IO="2-2", PLUG_WIDTH=980, PLUG_HEIGHT=560 | `build-win/vst3/x64/Release/DataBend.vst3` | VST364 |
| **Freeze95** | Stereo I/O, UI=1, shared resources=0 | `Freeze95/IPlugEffect.vst3` | VST364 |
| **Grungr** | 4 I/O, UI=1 | Varies by toolchain | VST364 |
| **SpectrumMatch** | 2-2 I/O | `build-win/vst3/x64/Release/RussetNoise.vst3` | VST364 |
| **MetallicSnare** | 2-2 I/O | Varies | VST364 |
| **Sore Throat** | 2-2 I/O, multi-target | Multiple formats | VST2/VST3/AAX |

## Post-Build Scripts

### Standard (postbuild-win.bat)
Handles VST3 bundling and deployment:

**Parameters (position-shifted after 6× `shift`):**
```bat
%1 = TargetExt (e.g., ".vst3")
%2 = Binary name (e.g., "DataBend")
%3 = Platform (e.g., "x64")
%4 = CopyVST2 flag
%5 = BuiltBinary path
%6-9 = VST2_32, VST2_64, VST3_32, VST3_64 paths
%4-9 (after shift) = AAX_32, AAX_64, BUILD_DIR, VST_ICON, AAX_ICON, CREATE_BUNDLE_SCRIPT
```

**What it does:**
1. For x64 + .vst3 format:
   - Calls `CREATE_BUNDLE_SCRIPT` to create `.vst3` bundle
   - Copies built binary to `PluginName.vst3/Contents/x86_64-win/`
   - Deploys to `%VST3_64_PATH%` if exists

### Freeze95-Specific (postbuild-freeze95-resources.bat)
Handles font embedding:
- SVGs embedded in compiled DLL via `main.rc`
- `.ttf` fonts copied to `Contents/Resources/` as fallback

**Parameters:**
```bat
%1 = VST3_BUILD_BUNDLE
%2 = VST3_INSTALL_BUNDLE  
%3 = RESOURCES_DIR
%4 = FONT_FILE
```

## Common Build Issues

### "CREATE_BUNDLE_SCRIPT not found"
**Cause:** MSBuild variable not set (running scripts standalone)
**Fix:** Build via Visual Studio — do not run `.bat` files directly

### "Failed to copy to VST3 path"
**Cause:** Permission issue or path doesn't exist
**Fix:** Ensure target VST3 folder exists (e.g., `C:\Program Files\Common Files\VST3\`)

### "Undefined VST3 symbols"
**Cause:** VST3 SDK not found
**Fix:** Check iPlug2 path in project files — ensure `IPLUG2_ROOT` environment variable set

## Resource Embedding

### Freeze95
- SVGs in `resources/` → embedded in DLL via `resources/main.rc`
- Fonts (`.ttf`) → copied to `Contents/Resources/` by postbuild script (`postbuild-freeze95-resources.bat`)

### Grungr
- SVGs → C++ via `tools/generate_embedded_assets.py`
- Output: `tools/iplug2-ui/EmbeddedAssets.h`

### DataBend/SpectrumMatch  
- No custom resource embedding
- Icons via `.ico` files

### BLUR (SpectralProtocol)
- SVGs compiled into DLL via `.rc` + `Spectral_blend.bat`
- Post-build: `deploy-vst3.ps1` / manual copy
- Note: Post-build bundler may fail intermittently (MSB3073 exit code 4) — manual deployment works reliably

## Testing After Build

### Standalone Test (Freeze95)
```cmd
cd D:\Github\VST3 Developments\Freeze95
# Compile test.cpp with generated DSP code
# Compare output against reference waveforms
```

### DAW Testing
1. Install plugin to system VST3 folder
2. Open DAW (Reaper, Logic, Studio One, etc.)
3. Verify:
   - Plugin loads without errors
   - Parameters respond correctly
   - Bypass works
   - No artifacts at extreme parameter values

## Build Configurations

All plugins support:
- **Debug|x64** — Debugging symbols, no optimizations
- **Release|x64** — Full optimizations, `/GL` (whole program)

Output directories:
```
build-win/vst3/
└── x64/
    ├── Debug/
    │   ├── int/          # Intermediate files
    │   └── *.vst3/       # Plugin bundle
    └── Release/
        ├── int/
        └── *.vst3/
```