# Grungr — Production Fix Plan

## TL;DR

> **Quick Summary**: Fix all issues identified in the Grungr audit — real-time safety, performance, hardening, GUI robustness, test infrastructure, and CI workflow. Based on proven patterns from Freeze95.

> **Deliverables**:
> - Fixed Grungr.cpp with pre-allocated buffers, FTZ/DAZ, NaN/Inf guard
> - Recompiled Faust DSP (-single -ftz 2 -vec) for ~40-50% CPU reduction
> - Cleaned up build config (no PDB in release)
> - SVG asset fallback handling in UI
> - Factory presets added
> - Standalone test executable for DSP verification
> - GitHub CI workflow for Grungr builds

> **Estimated Effort**: Medium
> **Parallel Execution**: YES — 3 waves
> **Critical Path**: Build baseline → Core DSP fixes → Test + CI → Final verification

---

## Context

### Original Request
Create a comprehensive plan to fix all issues from Grungr's production audit, including CI workflow and test infrastructure.

### Interview Summary
**Key Decisions**:
- **Scope**: Grungr-only fixes + CI workflow (no Freeze95 changes)
- **Faust source**: Available at `Grungr/Grungr.dsp` v2.7, 210 lines
- **Faust CLI**: Available at v2.85.5 (newer than the v2.81.2 used for current DSP)
- **Test strategy**: Standalone test executable (doctest, DSP-only, no iPlug2 mocking)
- **Build baseline**: Verify current source compiles before fixes

**Research Findings**:
- **Freeze95 buffer pattern**: Pre-allocates `std::max(16384, GetBlockSize())` in OnReset — proven RT-safe
- **Freeze95 FTZ/DAZ**: `_mm_setcsr(_mm_getcsr() | 0x8040)` at ProcessBlock start
- **Freeze95 Faust flags**: `-single -ftz 2` + additional optimization flags
- **iPlug2 API**: `GetBlockSize()` returns max block size — available in OnReset
- **Grungr.dsp**: Already uses `si.smoo` on all continuous controls (parameter smoothing built-in)

### Metis Review
**Identified Gaps** (addressed):
- Build baseline must be verified first (Task 1)
- Faust recompilation dependencies must be explicit (Task 3)
- Each fix needs clear QA scenarios (included per task)
- Scope guardrails must be explicit (see "Must NOT Have")

---

## Work Objectives

### Core Objective
Fix all Grungr audit issues, add test infrastructure and CI.

### Concrete Deliverables
- `Grungr/Grungr.cpp` — pre-allocated buffers, FTZ/DAZ, NaN guard, factory presets
- `Grungr/Grungr-win.props` — no PDB in release builds
- `Grungr/DSP/GrungrFaustDSP.h` — recompiled with `-single -ftz 2 -vec`
- `Grungr/GrungrIPlug2UI.cpp` — graceful SVG fallback
- `Grungr/tests/doctest.h` — single-header test framework
- `Grungr/tests/test_dsp.cpp` — standalone DSP test executable
- `.github/workflows/grungr-build.yml` — CI workflow

### Definition of Done
- [ ] `dotnet test Grungr/tests/` (or equivalent) passes
- [ ] MSBuild produces working Grungr.vst3
- [ ] CI workflow builds Grungr on push
- [ ] All QA scenarios pass

### Must Have
- Real-time safety: no allocations in audio thread
- Faust recompiled with -single precision
- FTZ/DAZ denormal protection in ProcessBlock
- Standalone test executable with working DSP tests
- CI workflow that builds and uploads artifacts

### Must NOT Have (Guardrails)
- No changes to Freeze95, DataBend, or any other plugin
- No modifications to iPlug2 framework files
- No Faust version upgrade (use current v2.85.5)
- No GUI modifications beyond SVG fallback fix
- No new parameters — only add presets
- No refactoring of DSP architecture

---

## Verification Strategy

> **ZERO HUMAN INTERVENTION** — ALL verification is agent-executed.
> Acceptance criteria requiring "user manually tests/confirms" are FORBIDDEN.

### Test Decision
- **Infrastructure exists**: NO (new setup)
- **Automated tests**: YES (standalone test executable)
- **Framework**: doctest (header-only, single .h file)
- **Test structure**: DSP output verification + parameter mapping tests

### QA Policy
Every task MUST include agent-executed QA scenarios. Evidence saved to `.sisyphus/evidence/task-{N}-{scenario-slug}.{ext}`.

- **DSP/Code changes**: Compile the plugin or test executable, verify output
- **Build/config changes**: Run `build_vst3.ps1`, check artifact exists
- **CI changes**: Verify file was created with correct YAML syntax

---

## Execution Strategy

### Parallel Execution Waves

```
Wave 0 (Pre-flight — establish baseline):
├── Task 1: Build baseline verification + download doctest
├── Task 1.5: Fix pre-existing compilation errors in GrungrIPlug2UI.cpp

Wave 1 (Core fixes — MAX PARALLEL):
├── Task 2: Fix real-time safety (OnReset pre-alloc, FTZ/DAZ, NaN guard) [Grungr.cpp]
├── Task 3: Recompile Faust DSP (-single -ftz 2 -vec)
├── Task 4: Remove PDB from release build config [Grungr-win.props]
├── Task 5: Add SVG asset fallback in UI [GrungrIPlug2UI.cpp]
├── Task 6: Add factory presets [Grungr.cpp]
└── Task 7: Create standalone test executable

Wave 2 (Integration + CI):
├── Task 8: Create Grungr CI workflow [.github/workflows/]
└── Task 9: Full build + test suite run (integration verification)

Wave FINAL (Review + sign-off):
├── Task F1: Plan compliance audit (oracle)
├── Task F2: Code quality review (unspecified-high)
├── Task F3: Real manual QA (unspecified-high)
└── Task F4: Scope fidelity check (deep)
```

### Dependency Matrix
- **1**: None (start immediately) → Blocks: 1.5
- **1.5**: 1 → Blocks: 2-9
- **2-7**: 1.5 → 8, 9
- **8-9**: 2-7 → F1-F4
- **F1-F4**: 9 → user approval

### Agent Dispatch Summary
- **Wave 0**: 2 tasks → `deep` (build verification), `quick` (compilation fix)
- **Wave 1**: 6 tasks → varied (see per task)
- **Wave 2**: 2 tasks → `unspecified-high`, `quick`
- **FINAL**: 4 agents → `oracle`, `unspecified-high`, `unspecified-high`, `deep`

---

## TODOs

- [x] 1. Build Baseline Verification + Download Doctest

  **What to do**:
  - Run `Grungr/build_vst3.ps1` to verify the current source compiles
  - Capture any build warnings/errors
  - Document the current build state (compiles? warnings?)
  - Download `doctest.h` from `https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h` to `Grungr/tests/doctest.h`

  **Must NOT do**:
  - Do not fix any issues yet — this is purely baseline measurement

  **Recommended Agent Profile**:
  - **Category**: `deep`
    - Reason: Build verification requires understanding MSBuild output and diagnosing potential failures
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: NO (baseline — blocks everything)
  - **Parallel Group**: Wave 0
  - **Blocks**: Tasks 2-7
  - **Blocked By**: None (start immediately)

  **References**:
  - `Grungr/build_vst3.ps1` — Build script with hardcoded MSBuild path
  - `Grungr/projects/Grungr-vst3.vcxproj` — Project file
  - `Freeze95/build_vst3.ps1` — Reference for how Freeze95 handles similar build

  **Acceptance Criteria**:
  - [ ] Build completes with "Build succeeded" message
  - [ ] `Grungr/build-win/vst3/x64/Release/Grungr.vst3` exists
  - [ ] `Grungr/tests/doctest.h` exists (downloaded)

  **QA Scenarios**:
  ```
  Scenario: Baseline build succeeds
    Tool: Bash
    Preconditions: iPlug2 cloned at repo root, Faust SDK at C:\Program Files\Faust
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
      2. Wait for build completion
      3. Check exit code
    Expected Result: Exit code 0, "Build succeeded" in output
    Failure Indicators: MSBuild error, missing framework include
    Evidence: .sisyphus/evidence/task-1-build-output.txt

  Scenario: Doctest header downloaded
    Tool: Bash
    Preconditions: Grungr/tests/ directory exists
    Steps:
      1. Run: Test-Path Grungr/tests/doctest.h
    Expected Result: True
    Evidence: .sisyphus/evidence/task-1-doctest-exists.txt
  ```

  **Commit**: NO (baseline only, no changes)

- [ ] 1.5. Fix Pre-Existing Compilation Errors in GrungrIPlug2UI.cpp

  **What to do**:
  **CRITICAL**: Baseline build revealed pre-existing compilation errors in `GrungrIPlug2UI.cpp`. Root cause: line 305 has a premature `}` that closes `Draw()` early, and line 307 has a spurious `void OnMouseDown(...) override` function declaration without an opening `{`, which orphanes ~90 lines of `Draw()` body code (lines 308-397).

  Fix: Remove exactly 2 lines — line 305 (`}`) and line 307 (`void OnMouseDown(float x, float y, const igraphics::IMouseMod& mod) override`). This allows the remaining code (alternative specular highlight, badge rendering, ripple effect, focus ring, engage pulse) to stay inside `Draw()` where it belongs.

  The correct `OnMouseDown` at line 399-407 is already properly formed and should not be touched.

  Before/After:
  ```
  // Before (lines 303-308):
    g.FillCircle(hiColor, ledX - ledInnerR * 0.3f, ledY - ledInnerR * 0.3f, hiR, &mBlend);
  }                                        ← line 305: ERRONEOUS
                                           ← line 306: blank
  void OnMouseDown(...) override           ← line 307: ERRONEOUS
      const igraphics::IColor hiColor(...) ← line 308: should be inside Draw()
  
  // After:
    g.FillCircle(hiColor, ledX - ledInnerR * 0.3f, ledY - ledInnerR * 0.3f, hiR, &mBlend);
                                           ← lines 305+307 removed
    const igraphics::IColor hiColor(...)   ← now correctly inside Draw()
  ```

  **Must NOT do**:
  - Do not modify any other part of the file
  - Do not change the correct `OnMouseDown` at line 399
  - Do not touch Grungr.cpp or any other file

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: 2-line deletion, trivially verified
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: NO (build verification prerequisite)
  - **Parallel Group**: Wave 0b (post-baseline)
  - **Blocks**: Tasks 2-9 (build must work to verify changes)
  - **Blocked By**: Task 1

  **References**:
  - `Grungr/GrungrIPlug2UI.cpp:303-308` — The bug location
  - `Grungr/GrungrIPlug2UI.cpp:399-407` — The correct OnMouseDown (don't touch)

  **Acceptance Criteria**:
  - [ ] Build succeeds after fix
  - [ ] No changes to any other file
  - [ ] Exactly 2 lines removed (305, 307)

  **QA Scenarios**:
  ```
  Scenario: Build succeeds after fix
    Tool: Bash
    Preconditions: Lines 305 and 307 removed from GrungrIPlug2UI.cpp
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
      2. Check for GrungrIPlug2UI.cpp errors
    Expected Result: Build succeeds, no errors from GrungrIPlug2UI.cpp
    Evidence: .sisyphus/evidence/task-1.5-build.txt

  Scenario: Verify only 2 lines changed
    Tool: Bash
    Preconditions: Fix applied
    Steps:
      1. Run: git diff Grungr/GrungrIPlug2UI.cpp | Select-String "^[+-]" | Select-Object -First 10
    Expected Result: Only 2 deletions (- lines), no additions (+ lines)
    Evidence: .sisyphus/evidence/task-1.5-diff.txt
  ```

  **Commit**: YES (part of Task 2 commit)

- [ ] 2. Fix Real-Time Safety and Hardening (OnReset + FTZ/DAZ + NaN Guard)

  **What to do**:
  These three changes are in the same file (`Grungr.cpp`) and form one logical concern (audio thread safety):

  **a) Pre-allocate buffers in OnReset (CRITICAL)** — replace the resize-in-audio-thread pattern:
  - In `OnReset()`, add pre-allocation using Freeze95's proven pattern:
    ```cpp
    const int preallocFrames = std::max(16384, GetBlockSize());
    mSilentInput.assign(preallocFrames, 0.0);
    mScratchOutput.assign(preallocFrames, 0.0);
    ```
  - Remove the conditional `resize()` from `ProcessBlock()` — replace with a failsafe clamp matching Freeze95's pattern:
    ```cpp
    if (static_cast<int>(mSilentInput.size()) < nFrames) {
      nFrames = static_cast<int>(mSilentInput.size());  // clamp instead of resize
    }
    ```
  - Also clear the vectors at start of ProcessBlock: `std::fill(mSilentInput.begin(), mSilentInput.begin() + nFrames, 0.0);`

  **b) Add FTZ/DAZ denormal protection** — add at the top of `ProcessBlock()`, after the nFrames check:
    ```cpp
    #ifdef _WIN32
    _mm_setcsr(_mm_getcsr() | 0x8040);
    #endif
    ```
    Add `#include <intrin.h>` or `#include <xmmintrin.h>` at top of file if not present.

  **c) Add NaN/Inf guard** — after `mFaustDSP.compute(nFrames, faustInputs, faustOutputs);`:
    ```cpp
    for (int s = 0; s < nFrames; ++s) {
      if (!std::isfinite(out0[s])) out0[s] = 0.0;
      if (nOut > 1 && !std::isfinite(out1[s])) out1[s] = 0.0;
    }
    ```
    This only runs if `nOut > 0` (already guaranteed by early return).

  **Must NOT do**:
  - Do not change the audio processing logic (same Faust compute call)
  - Do not modify any file outside Grungr/
  - Do not change the Faust DSP header

  **Recommended Agent Profile**:
  - **Category**: `deep`
    - Reason: Real-time safety violations are subtle — must understand iPlug2 lifecycle and audio thread constraints
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 3-7)
  - **Parallel Group**: Wave 1
  - **Blocks**: Task 9
  - **Blocked By**: Task 1 (baseline)

  **References**:
  - `Freeze95/Freeze95.cpp:2513-2528` — OnReset pre-allocation pattern
  - `Freeze95/Freeze95.cpp:2529-2532` — FTZ/DAZ at start of ProcessBlock
  - `Grungr/Grungr.cpp:98-151` — Current OnReset/ProcessBlock (to be modified)
  - `iPlug2/IPlug/IPlugProcessor.h:111-112` — `GetBlockSize()` API
  - `Grungr/Grungr.h:57-58` — `mSilentInput`, `mScratchOutput` member declarations

  **Acceptance Criteria**:
  - [ ] No `std::vector::resize()` calls in ProcessBlock
  - [ ] `OnReset()` pre-allocates with `std::max(16384, GetBlockSize())`
  - [ ] FTZ/DAZ enabled at start of ProcessBlock
  - [ ] NaN/Inf guard after compute()
  - [ ] Build succeeds

  **QA Scenarios**:
  ```
  Scenario: Code compiles without warnings
    Tool: Bash
    Preconditions: Changes applied to Grungr.cpp
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
      2. Check output for compile errors
    Expected Result: Build succeeds
    Evidence: .sisyphus/evidence/task-2-build.txt

  Scenario: Verify no resize calls in ProcessBlock
    Tool: Bash
    Preconditions: Grungr.cpp modified
    Steps:
      1. Run: Select-String -Path Grungr/Grungr.cpp -Pattern "resize\(|\.resize\(" -Context 0,0
    Expected Result: resize() not found inside void Grungr::ProcessBlock (only in OnReset)
    Evidence: .sisyphus/evidence/task-2-no-resize.txt

  Scenario: Verify FTZ/DAZ present
    Tool: Bash
    Preconditions: Grungr.cpp modified
    Steps:
      1. Run: Select-String -Path Grungr/Grungr.cpp -Pattern "_mm_setcsr"
    Expected Result: Match found
    Evidence: .sisyphus/evidence/task-2-ftz.txt

  Scenario: Verify NaN guard present
    Tool: Bash
    Preconditions: Grungr.cpp modified
    Steps:
      1. Run: Select-String -Path Grungr/Grungr.cpp -Pattern "isfinite"
    Expected Result: Match found
    Evidence: .sisyphus/evidence/task-2-nan-guard.txt
  ```

  **Commit**: YES (groups with Task 4)
  - Message: `fix(grungr): real-time safety hardening — pre-alloc buffers, FTZ/DAZ, NaN guard`
  - Files: `Grungr/Grungr.cpp`

- [x] 3. Recompile Faust DSP with Optimized Options

  **What to do**:
  Recompile `Grungr/Grungr.dsp` (v2.7, 210 lines) to `Grungr/DSP/GrungrFaustDSP.h` with improved flags:
  - Change from `-double -ftz 0` to **`-single -ftz 2`**
  - Keep existing flags: `-lang cpp -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33`

  **RESULT: BLOCKED** — Faust v2.85.5 generates code that takes `&` of temporary rvalues, which MSVC rejects (`error C2102: '&' requires l-value`). Tried `-vec`, `-os`, and other flags — all fail. Faust v2.81.2 (original) didn't have this issue. Reverted to original DSP header. The `-single` precision upgrade is blocked until either:
  - An MSVC-compatible Faust version is available
  - The generated code is manually patched
  - The CI environment has a different Faust version

  **Must NOT do**:
  - Do NOT change the Faust source (.dsp) — only compile flags
  - Do NOT upgrade Faust version
  - Do NOT change class name (must remain `GrungrFaustDSP`)

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: Single command execution, well-defined input/output
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 2, 4-7)
  - **Parallel Group**: Wave 1
  - **Blocks**: Task 9
  - **Blocked By**: Task 1

  **References**:
  - `Grungr/Grungr.dsp` — Faust source (210 lines, v2.7)
  - `Grungr/DSP/GrungrFaustDSP.h:5` — Current compile options (baseline)
  - `Freeze95/Freeze95DSP.h:5` — Freeze95 compile options (`-single -ftz 2`)
  - `Grungr/Grungr.cpp:19` — `mFaustDSP.buildUserInterface(&mFaustUI);` — class name `GrungrFaustDSP` must match

  **Acceptance Criteria**:
  - [ ] `Grungr/DSP/GrungrFaustDSP.h` is regenerated
  - [ ] Generated file uses `float` types (not `double`)
  - [ ] Class name remains `GrungrFaustDSP`
  - [ ] Plugin builds and works

  **QA Scenarios**:
  ```
  Scenario: Faust recompiles successfully
    Tool: Bash
    Preconditions: Faust CLI available, Grungr.dsp exists
    Steps:
      1. Run: faust -lang cpp -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 2 -vec -o Grungr/DSP/GrungrFaustDSP_new.h Grungr/Grungr.dsp
      2. Check exit code
    Expected Result: Exit code 0, no errors
    Failure Indicators: Faust syntax error, file not found
    Evidence: .sisyphus/evidence/task-3-faust-compile.txt

  Scenario: Verify single precision (no 'double' in key types)
    Tool: Bash
    Preconditions: New DSP .h generated
    Steps:
      1. Run: Select-String -Path Grungr/DSP/GrungrFaustDSP_new.h -Pattern "double"
      2. Check for double usage
    Expected Result: 'double' only in method return types/non-audio code, FAUSTFLOAT is float
    Evidence: .sisyphus/evidence/task-3-precision.txt

  Scenario: Verify class name preserved
    Tool: Bash
    Preconditions: New DSP .h generated
    Steps:
      1. Run: Select-String -Path Grungr/DSP/GrungrFaustDSP_new.h -Pattern "class GrungrFaustDSP"
    Expected Result: "class GrungrFaustDSP" found
    Evidence: .sisyphus/evidence/task-3-classname.txt
  ```

  **Commit**: YES (groups with Task 4)
  - Message: `perf(grungr): recompile Faust with -single -ftz 2 -vec for ~40% CPU reduction`
  - Files: `Grungr/DSP/GrungrFaustDSP.h`

- [ ] 4. Remove PDB from Release Build Config

  **What to do**:
  In `Grungr/config/Grungr-win.props`, remove or comment out the PDB file path so release builds don't generate debug symbols:
  - Find the line: `<PDB_FILE>$(SolutionDir)build-win\pdbs\$(TargetName)_$(Platform).pdb</PDB_FILE>`
  - Either remove the line or set it to empty: `<PDB_FILE></PDB_FILE>`
  - Also consider adding a `Release|*` condition to the `Link` section: `<GenerateDebugInformation>false</GenerateDebugInformation>` or removing the PDB reference for Release configurations only

  Actually, the simplest fix: the PDB line is a property macro used in `<Link><ProgramDatabaseFile>$(PDB_FILE)</ProgramDatabaseFile></Link>`. For Release builds, set the PDB to empty:
  ```xml
  <PDB_FILE Condition="'$(Configuration)'=='Release'"></PDB_FILE>
  ```
  Or just remove it entirely if PDBs aren't wanted for any config.

  **Must NOT do**:
  - Do not change any build paths or IPLUG2_ROOT
  - Do not remove PDB config for Debug builds (if desired)

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: Single-line XML change, trivial
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 2, 3, 5-7)
  - **Parallel Group**: Wave 1
  - **Blocks**: Task 9
  - **Blocked By**: Task 1

  **References**:
  - `Grungr/config/Grungr-win.props:11` — PDB_FILE property
  - `Grungr/config/Grungr-win.props:27-28` — ProgramDatabaseFile link setting

  **Acceptance Criteria**:
  - [ ] Release build does not produce a .pdb file
  - [ ] Debug builds (if used) can still produce .pdb

  **QA Scenarios**:
  ```
  Scenario: PDB removed from release config
    Tool: Bash
    Preconditions: Grungr-win.props modified
    Steps:
      1. Run: Select-String -Path Grungr/config/Grungr-win.props -Pattern "PDB_FILE"
      2. Check if PDB_FILE line is empty or conditional for Release
    Expected Result: PDB_FILE is empty or conditional for non-Release configs
    Evidence: .sisyphus/evidence/task-4-pdb-check.txt
  ```

  **Commit**: YES (groups with Tasks 2, 3)
  - Message: `fix(grungr): real-time safety hardening — pre-alloc buffers, FTZ/DAZ, NaN guard`
  - Files: `Grungr/config/Grungr-win.props`

- [ ] 5. Add SVG Asset Fallback in UI

  **What to do**:
  The `GrungrIPlug2UI.cpp` currently loads SVGs by filename from resources. If an SVG fails to load, the plugin may crash or show blank UI. Add graceful fallback:

  1. In `GrungrIPlug2UI.cpp`, find where background SVG and knob SVG are loaded
  2. Wrap SVG loading with a check — if loading fails, fall back to vector-drawn controls (the `kForceSafeVectorUIMode` constant already exists at line 26)
  3. Set the fallback mode: add a try/check around the SVG asset loading and if it fails, set `kForceSafeVectorUIMode` behavior programmatically
  4. Alternatively: check file existence before attempting to load, and provide a clear error message (tooltip or status)

  **Must NOT do**:
  - Do not redesign the UI — only add failure handling
  - Do not change SVG positioning or layout

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
    - Reason: iPlug2's SVG loading API needs careful handling to avoid crashes
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 2, 3, 4, 6, 7)
  - **Parallel Group**: Wave 1
  - **Blocks**: Task 9
  - **Blocked By**: Task 1

  **References**:
  - `Grungr/GrungrIPlug2UI.cpp:26` — `kForceSafeVectorUIMode` constant
  - `Grungr/GrungrIPlug2UI.cpp:50-84` — Design tokens for vector fallback rendering
  - `Grungr/config.h:63-64` — SVG filenames: `GRUNGE_BG_SVG_FN`, `POWERBUTTON_SVG_FN`

  **Acceptance Criteria**:
  - [ ] UI doesn't crash if SVG file is missing
  - [ ] Vector fallback renders controls when SVG is unavailable

  **QA Scenarios**:
  ```
  Scenario: UI handles missing SVG gracefully
    Tool: Code review + build
    Preconditions: Fallback logic applied
    Steps:
      1. Review code for SVG loading guard
      2. Build plugin
    Expected Result: Code has fallback path, build succeeds
    Evidence: .sisyphus/evidence/task-5-fallback-review.txt
  ```

  **Commit**: YES
  - Message: `fix(grungr): add SVG asset fallback in UI`
  - Files: `Grungr/GrungrIPlug2UI.cpp`

- [ ] 6. Add Factory Presets

  **What to do**:
  Currently `kNumPresets = 1` (default only). Add meaningful factory presets:

  1. Change `kNumPresets` in `Grungr.h` to a larger number (e.g., 5-8)
  2. In `Grungr.cpp` constructor, add preset definitions using iPlug2's `MakePreset` or `AddPreset` API:
     ```cpp
     // iPlug2 preset API: GetParam(kParamX)->InitDouble(...) then set presets
     // Example: AddPreset("Clean-ish", ...) — Grunge 0.3, Butt 0.5, Face 0.4, Loud 0.7, RAW
     //          AddPreset("Crunch", ...) — Grunge 0.6, Butt 0.6, Face 0.5, Loud 0.7, ENHANCED
     //          AddPreset("Full Grunge", ...) — Grunge 1.0, Butt 0.8, Face 0.8, Loud 0.7, ENHANCED
     //          AddPreset("Bass Boost", ...) — Grunge 0.5, Butt 1.0, Face 0.3, Loud 0.7, RAW
     //          AddPreset("Bright", ...) — Grunge 0.4, Butt 0.3, Face 1.0, Loud 0.7, RAW
     ```
  3. Check iPlug2's `IPlugParameter.h` for `AddPreset` API signature — it's typically `MakePreset(const char* name, double p1, double p2, ...)`

  **Must NOT do**:
  - Do not change existing parameter order or IDs
  - Do not remove the factory default

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
    - Reason: Requires understanding iPlug2 preset API and crafting musical preset values
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 2, 3, 4, 5, 7)
  - **Parallel Group**: Wave 1
  - **Blocks**: Task 9
  - **Blocked By**: Task 1

  **References**:
  - `Grungr/Grungr.h:19` — `kNumPresets = 1` (to increase)
  - `Grungr/Grungr.cpp:11-16` — Parameter initialization (Grunger, Butt, Face, Loud, RAW, Bypass)
  - `iPlug2/IPlug/IPlugParameter.h` — `MakePreset()` API
  - `Freeze95/Freeze95.cpp` — Search for how Freeze95 handles presets (if any)

  **Acceptance Criteria**:
  - [ ] `kNumPresets` increased (5+ presets)
  - [ ] Each preset has distinct, musically-useful parameter values
  - [ ] Presets can be loaded in DAW (iPlug2 handles this automatically)
  - [ ] Build succeeds

  **QA Scenarios**:
  ```
  Scenario: Presets compile correctly
    Tool: Bash
    Preconditions: Grungr.cpp + Grungr.h modified
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
    Expected Result: Build succeeds
    Evidence: .sisyphus/evidence/task-6-build.txt

  Scenario: Preset count is correct
    Tool: Bash
    Preconditions: Grungr.h modified
    Steps:
      1. Run: Select-String -Path Grungr/Grungr.h -Pattern "kNumPresets"
    Expected Result: kNumPresets > 1
    Evidence: .sisyphus/evidence/task-6-preset-count.txt
  ```

  **Commit**: YES
  - Message: `feat(grungr): add factory presets`
  - Files: `Grungr/Grungr.h`, `Grungr/Grungr.cpp`

- [ ] 7. Create Standalone Test Executable

  **What to do**:
  Create a standalone test executable for the Faust DSP. This is separate from the iPlug2 plugin — it compiles the Faust DSP directly and tests it:

  1. Create directory `Grungr/tests/` (if not already done in Task 1)
  2. Create `Grungr/tests/test_dsp.cpp` with doctest tests:
     ```cpp
     #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
     #include "doctest.h"
     
     // Include Faust DSP directly (not through iPlug2)
     #ifndef FAUSTFLOAT
     #define FAUSTFLOAT float
     #endif
     #include "faust/dsp/dsp.h"
     #include "faust/gui/MapUI.h"
     #include "../DSP/GrungrFaustDSP.h"
     
     TEST_CASE("DSP initializes without error") {
       GrungrFaustDSP dsp;
       CHECK_NOTHROW(dsp.init(44100));
       CHECK(dsp.getNumInputs() == 2);
       CHECK(dsp.getNumOutputs() == 2);
     }
     
     TEST_CASE("DSP processes silence without NaN/Inf") {
       GrungrFaustDSP dsp;
       dsp.init(44100);
       
       float inputs[2][64] = {};
       float outputs[2][64] = {};
       float* in[2] = {inputs[0], inputs[1]};
       float* out[2] = {outputs[0], outputs[1]};
       dsp.compute(64, in, out);
       
       for (int s = 0; s < 64; s++) {
         CHECK(std::isfinite(out[0][s]));
         CHECK(std::isfinite(out[1][s]));
       }
     }
     
     TEST_CASE("DSP defaults match config.h defaults") {
       GrungrFaustDSP dsp;
       dsp.init(44100);
       MapUI ui;
       dsp.buildUserInterface(&ui);
       
       CHECK(ui.getParamValue("Grunge") == doctest::Approx(0.70f));
       CHECK(ui.getParamValue("Butt") == doctest::Approx(0.62f));
       CHECK(ui.getParamValue("Face") == doctest::Approx(0.56f));
       CHECK(ui.getParamValue("Loud") == doctest::Approx(0.72f));
     }
     
     TEST_CASE("Parameter mapping works through MapUI") {
       GrungrFaustDSP dsp;
       dsp.init(44100);
       MapUI ui;
       dsp.buildUserInterface(&ui);
       
       ui.setParamValue("Grunge", 0.5f);
       CHECK(ui.getParamValue("Grunge") == doctest::Approx(0.5f));
       
       ui.setParamValue("Enhanced", 1.0f);
       CHECK(ui.getParamValue("Enhanced") == doctest::Approx(1.0f));
     }
     
     TEST_CASE("DSP handles various block sizes") {
       GrungrFaustDSP dsp;
       dsp.init(44100);
       
       int blockSizes[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
       for (int i = 0; i < 11; i++) {
         int n = blockSizes[i];
         std::vector<float> inL(n, 0.5f), inR(n, 0.5f);
         std::vector<float> outL(n, 0.0f), outR(n, 0.0f);
         float* in[2] = {inL.data(), inR.data()};
         float* out[2] = {outL.data(), outR.data()};
         CHECK_NOTHROW(dsp.compute(n, in, out));
         for (int s = 0; s < n; s++) {
           CHECK(std::isfinite(out[0][s]));
         }
       }
     }
     ```
  3. Create `Grungr/tests/build_tests.ps1` to compile and run:
     ```powershell
     # Locate MSVC compiler
     $cl = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.x\bin\Hostx64\x64\cl.exe"
     # Or use vcvarsall to set up environment
     
     # Compile test executable
     cl /EHsc /std:c++17 /I"..\..\iPlug2\Dependencies\IPlug\Faust\include" /I"..\DSP" test_dsp.cpp
     
     # Run tests
     .\test_dsp.exe
     ```
     Note: The exact include paths need to match where Faust SDK headers are installed. Update based on your system.

  **Must NOT do**:
  - Do not include iPlug2 headers (tests should be standalone DSP-only)
  - Do not create a .vcxproj — use simple CL invocation

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
    - Reason: Need to understand Faust SDK include paths, MSVC compilation, and doctest integration
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: YES (with Tasks 2-6)
  - **Parallel Group**: Wave 1
  - **Blocks**: None (independent)
  - **Blocked By**: Task 1 (need doctest.h)

  **References**:
  - `Grungr/DSP/GrungrFaustDSP.h` — DSP class to test
  - `Grungr/Grungr.h:7-17` — Faust include path conventions used in plugin
  - `Grungr/config/Grungr-win.props:6` — Faust include: `C:\Program Files\Faust\include`
  - `Freeze95/build_vst3.ps1` — Freeze95 build script as reference for MSVC compilation

  **Acceptance Criteria**:
  - [ ] `Grungr/tests/test_dsp.cpp` created with 5+ test cases
  - [ ] `Grungr/tests/build_tests.ps1` created
  - [ ] Test executable compiles and runs
  - [ ] All tests pass

  **QA Scenarios**:
  ```
  Scenario: Test executable compiles
    Tool: Bash
    Preconditions: test_dsp.cpp + build_tests.ps1 exist, doctest.h exists
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/tests/build_tests.ps1
    Expected Result: Compilation succeeds, test_dsp.exe created
    Evidence: .sisyphus/evidence/task-7-test-compile.txt

  Scenario: All tests pass
    Tool: Bash
    Preconditions: test_dsp.exe exists
    Steps:
      1. Run: .\Grungr\tests\test_dsp.exe
    Expected Result: "All tests passed" or "0 failures"
    Failure Indicators: Test failures, crashes
    Evidence: .sisyphus/evidence/task-7-test-results.txt
  ```

  **Commit**: YES
  - Message: `test(grungr): add standalone DSP test executable`
  - Files: `Grungr/tests/test_dsp.cpp`, `Grungr/tests/build_tests.ps1`, `Grungr/tests/doctest.h`

- [ ] 8. Create Grungr CI Workflow

  **What to do**:
  Create `.github/workflows/grungr-build.yml` modeled after Freeze95's `build.yml`:

  ```yaml
  name: Build Grungr
  
  on:
    push:
      paths:
        - 'Grungr/**'
        - '.github/workflows/grungr-build.yml'
    pull_request:
      paths:
        - 'Grungr/**'
    workflow_dispatch:
  
  jobs:
    build-windows:
      runs-on: windows-latest
      steps:
        - uses: actions/checkout@v4
          with:
            submodules: recursive
        
        - name: Clone iPlug2
          shell: bash
          run: |
            git clone --depth 1 https://github.com/iPlug2/iPlug2.git ../iPlug2
        
        - name: Download iPlug2 SDKs
          shell: bash
          working-directory: ../iPlug2
          run: bash download-iplug-sdks.sh
        
        - name: Add MSBuild to PATH
          uses: microsoft/setup-msbuild@v2
        
        - name: Build Grungr VST3 (Windows)
          shell: pwsh
          run: |
            cd Grungr
            & "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" projects\Grungr-vst3.vcxproj /p:Configuration=Release /p:Platform=x64 /p:SolutionDir=$env:GITHUB_WORKSPACE\Grungr\ /v:m
        
        - name: Upload VST3 Artifact
          uses: actions/upload-artifact@v4
          with:
            name: Grungr-Windows
            path: Grungr/build-win/vst3/x64/Release/Grungr.vst3
  ```

  **Must NOT do**:
  - Do not modify Freeze95's existing build.yml
  - Do not add macOS build unless we verify xcodebuild commands

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: Well-defined pattern to copy from Freeze95
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: NO (after Wave 1 completed)
  - **Parallel Group**: Wave 2
  - **Blocks**: Task 9
  - **Blocked By**: Tasks 2-7

  **References**:
  - `.github/workflows/build.yml` — Freeze95's workflow (model)
  - `Grungr/build_vst3.ps1` — Build script (MSBuild command)

  **Acceptance Criteria**:
  - [ ] `.github/workflows/grungr-build.yml` created
  - [ ] YAML syntax is valid
  - [ ] Paths reference Grungr/ correctly
  - [ ] Path filter narrows to Grungr/** changes

  **QA Scenarios**:
  ```
  Scenario: CI workflow file created with valid YAML
    Tool: Bash
    Preconditions: File created
    Steps:
      1. Run: Select-String -Path .github/workflows/grungr-build.yml -Pattern "^on:|^jobs:" -Context 0,2
    Expected Result: Valid YAML structure, proper indentation
    Evidence: .sisyphus/evidence/task-8-ci-structure.txt

  Scenario: Path references are correct
    Tool: Bash
    Preconditions: File created
    Steps:
      1. Run: Select-String -Path .github/workflows/grungr-build.yml -Pattern "Grungr/"
    Expected Result: All path references use correct relative paths from repo root
    Evidence: .sisyphus/evidence/task-8-ci-paths.txt
  ```

  **Commit**: YES
  - Message: `ci(grungr): add GitHub Actions build workflow`
  - Files: `.github/workflows/grungr-build.yml`

- [ ] 9. Integration Build and Test Run

  **What to do**:
  After all Wave 1 + 2 tasks are complete, do a full integration check:
  1. Run `Grungr/build_vst3.ps1` to verify everything compiles together
  2. Run the test executable: `Grungr/tests/test_dsp.exe`
  3. Verify the built `.vst3` artifact exists at `Grungr/build-win/vst3/x64/Release/Grungr.vst3`
  4. Check file size to ensure it's reasonable (a few hundred KB)
  5. Verify no unintended changes by checking git diff

  **Must NOT do**:
  - Do not modify any files — pure verification
  - Do not load in DAW (no host available in CI/agent)

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
    - Reason: Integration verification requires understanding the full build chain
  - **Skills**: `[]`

  **Parallelization**:
  - **Can Run In Parallel**: NO (integration verification)
  - **Parallel Group**: Wave 2
  - **Blocks**: F1-F4
  - **Blocked By**: Tasks 2-8

  **Acceptance Criteria**:
  - [ ] Full build succeeds
  - [ ] Test executable runs and all tests pass
  - [ ] `.vst3` artifact exists
  - [ ] Git diff shows only expected changes

  **QA Scenarios**:
  ```
  Scenario: Full build succeeds with all fixes
    Tool: Bash
    Preconditions: All fixes applied
    Steps:
      1. Run: powershell -ExecutionPolicy Bypass -File Grungr/build_vst3.ps1
    Expected Result: Build succeeded, no errors
    Evidence: .sisyphus/evidence/task-9-build.txt

  Scenario: All tests pass
    Tool: Bash
    Preconditions: Test executable built
    Steps:
      1. Run: .\Grungr\tests\test_dsp.exe
    Expected Result: All tests pass (0 failures)
    Evidence: .sisyphus/evidence/task-9-test-results.txt

  Scenario: VST3 artifact exists with expected size
    Tool: Bash
    Preconditions: Build completed
    Steps:
      1. Run: Get-Item Grungr/build-win/vst3/x64/Release/Grungr.vst3 | Select-Object Length
    Expected Result: File exists, size > 100KB
    Evidence: .sisyphus/evidence/task-9-artifact.txt
  ```

  **Commit**: NO (verification only)

---

## Final Verification Wave

> 4 review agents run in PARALLEL. ALL must APPROVE.

- [ ] F1. **Plan Compliance Audit** — `oracle`
  Read the plan end-to-end. For each "Must Have": verify implementation exists. For each "Must NOT Have": search codebase for forbidden patterns. Check evidence files exist. Compare deliverables against plan.
  Output: `Must Have [N/N] | Must NOT Have [N/N] | Tasks [N/N] | VERDICT: APPROVE/REJECT`

- [ ] F2. **Code Quality Review** — `unspecified-high`
  Review all changed files for: `as any`/`@ts-ignore` (C++ equivalent), empty catches, console.log in prod, commented-out code, unused imports. Check AI slop.
  Output: `Build [PASS/FAIL] | Files [N clean/N issues] | VERDICT`

- [ ] F3. **Real Manual QA** — `unspecified-high` (+ `playwright` skill if UI)
  Execute EVERY QA scenario from EVERY task. Test cross-task integration. Save to `.sisyphus/evidence/final-qa/`.
  Output: `Scenarios [N/N pass] | VERDICT`

- [ ] F4. **Scope Fidelity Check** — `deep`
  For each task: read "What to do", read actual diff. Verify 1:1 — everything built, nothing beyond spec. Check "Must NOT do" compliance.
  Output: `Tasks [N/N compliant] | Contamination [CLEAN/N issues] | VERDICT`

---

## Commit Strategy

- **1.5-2**: `fix(grungr): fix UI compilation errors, real-time safety hardening` — GrungrIPlug2UI.cpp, Grungr.cpp, Grungr-win.props
- **3-4**: `perf(grungr): recompile Faust with -single -ftz 2 -vec` — DSP/GrungrFaustDSP.h
- **5**: `fix(grungr): add SVG asset fallback in UI` — GrungrIPlug2UI.cpp
- **6**: `feat(grungr): add factory presets` — Grungr.cpp
- **7**: `test(grungr): add standalone DSP test executable` — Grungr/tests/
- **8**: `ci(grungr): add GitHub Actions build workflow` — .github/workflows/
- **F**: `chore: final review fixes` — (if needed)

---

## Success Criteria

### Verification Commands
```powershell
# Build test
& "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" projects\Grungr-vst3.vcxproj /p:Configuration=Release /p:Platform=x64 /p:SolutionDir=..\ /v:m
# Expected: Build succeeded, Grungr.vst3 created at build-win\vst3\x64\Release\

# Run tests
# (compile test executable first, then run)
# Expected: All tests pass (0 failures)

# CI syntax check
# (verify YAML is valid)
```

### Final Checklist
- [x] Task 1: Build baseline verified, VST3 SDK initialized, doctest downloaded
- [x] Task 1.5: GrungrIPlug2UI.cpp compilation errors fixed
- [x] Audio thread allocation fix verified (no resize in ProcessBlock)
- [ ] Task 3: Faust DSP recompilation — BLOCKED (Faust v2.85.5 MSVC incompatibility)
- [x] FTZ/DAZ enabled in ProcessBlock
- [x] No PDB in release build output
- [x] SVG fallback: UI code was already defensive (hasBackgroundSVG/hasKnobSVG checks)
- [x] Factory presets added (5 presets, build verified)
- [x] Test executable compiles and all 5 tests pass (1170 assertions)
- [x] CI workflow file created (`.github/workflows/grungr-build.yml`)
- [x] Grungr.cpp real-time safety: pre-alloc in OnReset, clamp in ProcessBlock
- [x] NaN/Inf guard after Faust compute
