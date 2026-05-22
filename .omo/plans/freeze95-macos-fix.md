# Fix Freeze95 macOS VST3/CLAP Plugin Loading in DAWs

## TL;DR

> **Quick Summary**: Fix macOS build configuration so Freeze95 loads correctly in Ableton Live, Reaper, and Logic Pro. Root causes: ad-hoc signing incompatible with `--timestamp` flag, missing entitlements for hardened runtime, IPlugEffect template residue in Info.plist, and nested artifact zip structure confusing users.
> 
> **Deliverables**:
> - Fixed `.github/workflows/build.yml` (xcconfig, signing, entitlements, plist, staging)
> - macOS entitlements file `Freeze95/Freeze95.entitlements`
> - Flat artifact distribution (zip contains `.vst3`/`.clap` directly)
> - Updated user install instructions with quarantine removal
> - CI verification step for bundle structure + signature + Gatekeeper
> 
> **Estimated Effort**: Medium
> **Parallel Execution**: YES — 2 waves
> **Critical Path**: Task 1 (fix xcconfig/signing) → Task 5 (flatten zip) → Task 6 (CI verification) → Task F1-F4 (final verification)

---

## Context

### Original Request
User reports Freeze95 still not loading in Ableton Live on macOS. Error messages:
- `codesign -vvv` returns "No such file or directory" despite file visible in Finder
- Ableton: "Freeze95.vst is not recognised or not scannable for viruses"
- Users already ran quarantine removal script

### Interview Summary
**Key Discussions**:
- No Mac available for local testing — rely on CI + user feedback
- No paid Apple Developer ID — ad-hoc signing only
- Must work on Intel and Apple Silicon (universal binary)
- Distribution via Google Drive (because Gmail blocks zip attachments)

**Research Findings**:
- Ad-hoc signing (`codesign -s -`) is incompatible with `--timestamp` — known failure mode
- Hardened runtime (`--options=runtime`) requires entitlements file
- VST3 on macOS is a bundle (directory), not a single file
- `com.apple.quarantine` attribute blocks Gatekeeper even on signed code
- `Freeze95-macOS-VST3.zip` contains nested folder `Freeze95-macOS-VST3/Freeze95.vst3/` — users may install the wrapper folder
- Info.plist still has IPlugEffect template values (version 1.0.0, copyright "Acme Inc", signature `Ipef`)

### Metis Review
**Identified Gaps** (addressed in plan):
- Need to verify current artifact structure before fixing
- Need to check CLAP has same issues (it does — shared build steps)
- Need to test on clean macOS VM (covered in QA scenarios)
- Need explicit guardrails against scope creep (Developer ID, notarization, JUCE switch)
- Need acceptance criteria for Gatekeeper (`spctl --assess`)

---

## Work Objectives

### Core Objective
Fix the macOS CI build so Freeze95 VST3 and CLAP plugins load correctly in DAWs after user download and install.

### Concrete Deliverables
1. Fixed `build.yml` with correct signing flags, entitlements, plist updates, and flat staging
2. `Freeze95/Freeze95.entitlements` entitlement file committed to repo
3. CI verification step that fails the build if signature/bundle structure is wrong
4. Updated install instructions for users (succinct, with exact commands)

### Definition of Done
- [ ] CI build passes and produces `Freeze95-macOS-VST3.zip` with `Freeze95.vst3` at root
- [ ] `codesign -dvv` on extracted bundle shows `Signature=adhoc` and `flags=0x10002(adhoc,runtime)`
- [ ] `spctl --assess --type install` returns `accepted` (or equivalent for ad-hoc)
- [ ] User can install by: unzip → drag `.vst3` to `~/Library/Audio/Plug-Ins/VST3/` → run `xattr -rd com.apple.quarantine` → rescan DAW

### Must Have
- Ad-hoc signing (no Developer ID)
- Universal binary (x86_64 + arm64)
- Hardened runtime enabled
- Quarantine removal instructions
- Both VST3 and CLAP fixed

### Must NOT Have (Guardrails)
- Apple Developer ID signing
- Notarization (requires Developer ID)
- AU format (requires Developer ID)
- JUCE framework switch
- DSP/audio code changes
- Linux/Windows build breakage
- Complex installer app/DMG

---

## Verification Strategy

> **ZERO HUMAN INTERVENTION** — ALL verification is agent-executed via CI logs and bash commands.

### Test Decision
- **Infrastructure exists**: YES — GitHub Actions CI with macOS runner
- **Automated tests**: Tests-after (no unit test framework for VST3 loading)
- **Agent-Executed QA**: MANDATORY for every task

### QA Policy
Every task includes agent-executed QA scenarios. Evidence saved to `.sisyphus/evidence/`.

- **CI/Build**: Bash — run commands on CI logs, verify outputs
- **Bundle inspection**: Bash — extract zip, inspect structure, run `codesign`, `lipo`, `plutil`
- **No human DAW testing** — we don't have a Mac; rely on CI verification + user feedback loop

---

## Execution Strategy

### Parallel Execution Waves

```
Wave 1 (Foundation — Build Config Fixes):
├── Task 1: Fix xcconfig signing flags (remove --timestamp)
├── Task 2: Create entitlements file and integrate into CI
├── Task 3: Fix Info.plist generation (all fields, not just 3)
└── Task 4: Add CLAP entitlements + moduleinfo consistency

Wave 2 (Integration + Distribution + Verification):
├── Task 5: Flatten artifact zip structure (VST3 + CLAP)
├── Task 6: Add CI verification step (signature, bundle, Gatekeeper)
├── Task 7: Update user install instructions
└── Task 8: Build test run + evidence capture

Wave FINAL (After ALL tasks — 4 parallel reviews):
├── Task F1: Plan compliance audit (oracle)
├── Task F2: Code quality review (unspecified-high)
├── Task F3: Bundle structure + signature QA (unspecified-high)
└── Task F4: Scope fidelity check (deep)
-> Present results -> Get explicit user okay

Critical Path: Task 1 → Task 2 → Task 3 → Task 5 → Task 6 → Task 8 → F1-F4
```

### Dependency Matrix
- **1**: None → blocks 2, 3, 4, 5, 6
- **2**: None → blocks 4, 5, 6
- **3**: None → blocks 5, 6
- **4**: 1, 2 → blocks 5, 6
- **5**: 1, 2, 3, 4 → blocks 6, 7, 8
- **6**: 5 → blocks 8
- **7**: None → blocks 8 (can run in parallel with 5, 6)
- **8**: 5, 6, 7 → blocks F1-F4

---

## TODOs

- [ ] 1. Fix xcconfig: remove `--timestamp` from ad-hoc signing

  **What to do**:
  - In `.github/workflows/build.yml`, line 121: change `OTHER_CODE_SIGN_FLAGS = --timestamp` to `OTHER_CODE_SIGN_FLAGS =` (empty)
  - Also remove `--timestamp` from any explicit `codesign` commands in the workflow
  - Add comment explaining why: ad-hoc signing (`-`) cannot use timestamps

  **Must NOT do**:
  - Do NOT change `CODE_SIGN_IDENTITY = -` (keep ad-hoc)
  - Do NOT add `--timestamp` back anywhere
  - Do NOT touch Windows build steps

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: Simple string replacement in YAML

  **Parallelization**:
  - **Can Run In Parallel**: YES
  - **Parallel Group**: Wave 1 (with Tasks 2, 3, 4)
  - **Blocks**: Tasks 5, 6, 8
  - **Blocked By**: None

  **References**:
  - `build.yml:121` — current `OTHER_CODE_SIGN_FLAGS = --timestamp`
  - `build.yml:160` — explicit `codesign` step (verify no `--timestamp` here either)
  - Apple docs: Ad-hoc signing (`-s -`) does not support `--timestamp` flag

  **Acceptance Criteria**:
  - [ ] `grep -n "timestamp" .github/workflows/build.yml` returns 0 matches
  - [ ] CI build passes on macOS runner

  **QA Scenarios**:
  ```
  Scenario: Verify no timestamp flag in build config
    Tool: Bash
    Preconditions: None
    Steps:
      1. grep -n "timestamp" .github/workflows/build.yml
      2. Count matches
    Expected Result: 0 matches
    Evidence: .sisyphus/evidence/task-1-no-timestamp.txt
  ```

  **Commit**: YES
  - Message: `fix(macOS): remove incompatible --timestamp from ad-hoc signing`
  - Files: `.github/workflows/build.yml`

- [ ] 2. Create entitlements file and integrate into CI signing

  **What to do**:
  - Create `Freeze95/Freeze95.entitlements`:
    ```xml
    <?xml version="1.0" encoding="UTF-8"?>
    <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
    <plist version="1.0">
    <dict>
      <key>com.apple.security.cs.disable-library-validation</key>
      <true/>
    </dict>
    </plist>
    ```
  - Copy entitlements file into iPlug2 example resources during CI setup (same as config.h copy)
  - Update CI `codesign` commands to include `--entitlements Freeze95.entitlements`
  - Ensure entitlements are referenced by absolute path or copied to build workspace

  **Must NOT do**:
  - Do NOT add unnecessary entitlements (keep minimal)
  - Do NOT commit entitlements inside iPlug2 directory (it's cloned dynamically)

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: File creation + YAML edit

  **Parallelization**:
  - **Can Run In Parallel**: YES
  - **Parallel Group**: Wave 1 (with Tasks 1, 3, 4)
  - **Blocks**: Tasks 4, 5, 6
  - **Blocked By**: None

  **References**:
  - `build.yml:92-106` — resource copying pattern (copy entitlements same way)
  - `build.yml:155-163` — ad-hoc signing step (add `--entitlements` flag)
  - Apple docs: `com.apple.security.cs.disable-library-validation` required for unsigned dylibs

  **Acceptance Criteria**:
  - [ ] `Freeze95/Freeze95.entitlements` exists in repo
  - [ ] CI signing step includes `--entitlements` flag
  - [ ] `codesign -d --entitlements -` on built bundle shows the entitlement

  **QA Scenarios**:
  ```
  Scenario: Verify entitlements in built bundle
    Tool: Bash (on CI artifact)
    Preconditions: CI build completed
    Steps:
      1. Download artifact, extract
      2. codesign -d --entitlements - Freeze95.vst3
    Expected Result: Output contains "com.apple.security.cs.disable-library-validation"
    Evidence: .sisyphus/evidence/task-2-entitlements.txt
  ```

  **Commit**: YES
  - Message: `fix(macOS): add entitlements for hardened runtime`
  - Files: `Freeze95/Freeze95.entitlements`, `.github/workflows/build.yml`

- [ ] 3. Fix Info.plist generation: update ALL template fields

  **What to do**:
  - In CI "Fix Info.plist" step (`build.yml:144-153`), add PlistBuddy commands for ALL remaining IPlugEffect fields:
    ```bash
    /usr/libexec/PlistBuddy -c "Set :CFBundleGetInfoString 'Freeze95 v1.3.0 Copyright 2026 cloph'" "$PLIST"
    /usr/libexec/PlistBuddy -c "Set :CFBundleSignature 'Frz5'" "$PLIST"
    /usr/libexec/PlistBuddy -c "Set :CFBundleShortVersionString '1.3.0'" "$PLIST"
    /usr/libexec/PlistBuddy -c "Set :CFBundleVersion '1.3.0'" "$PLIST"
    /usr/libexec/PlistBuddy -c "Set :LSMinimumSystemVersion '11.0'" "$PLIST"
    ```
  - Read `config.h` to confirm version is `1.3.0` and manufacturer is `cloph`
  - If any field doesn't exist in template, use `Add` instead of `Set`, or delete+add

  **Must NOT do**:
  - Do NOT leave any "IPlugEffect" or "Acme Inc" strings in plist
  - Do NOT change `CFBundlePackageType` (must remain `BNDL`)
  - Do NOT change `CFBundleIdentifier` (already correct)

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: YAML edit + string verification

  **Parallelization**:
  - **Can Run In Parallel**: YES
  - **Parallel Group**: Wave 1 (with Tasks 1, 2, 4)
  - **Blocks**: Tasks 5, 6
  - **Blocked By**: None

  **References**:
  - `build.yml:144-153` — current plist fix step
  - `Freeze95/config.h:5-6` — version strings
  - `Freeze95/config.h:4` — manufacturer
  - Extracted plist from artifact — current values

  **Acceptance Criteria**:
  - [ ] CI step updated with all 5 new PlistBuddy commands
  - [ ] Built artifact's Info.plist contains NO "IPlugEffect" or "Acme" strings
  - [ ] Version is "1.3.0" in plist

  **QA Scenarios**:
  ```
  Scenario: Verify no template residue in Info.plist
    Tool: Bash (on CI artifact)
    Preconditions: CI build completed
    Steps:
      1. Download artifact, extract
      2. grep -i "iplugeffect\|acme" Freeze95.vst3/Contents/Info.plist
    Expected Result: 0 matches
    Evidence: .sisyphus/evidence/task-3-plist-clean.txt

  Scenario: Verify correct version in Info.plist
    Tool: Bash
    Steps:
      1. plutil -p Freeze95.vst3/Contents/Info.plist | grep CFBundleShortVersionString
    Expected Result: Contains "1.3.0"
    Evidence: .sisyphus/evidence/task-3-plist-version.txt
  ```

  **Commit**: YES
  - Message: `fix(macOS): update all Info.plist fields to Freeze95 branding`
  - Files: `.github/workflows/build.yml`

- [ ] 4. Apply signing fixes to CLAP build + ensure moduleinfo consistency

  **What to do**:
  - CLAP build uses same xcconfig and signing steps as VST3 — verify it gets all fixes from Tasks 1-3
  - Ensure CLAP codesign step also uses `--entitlements` flag
  - The moduleinfo.json step only applies to VST3 — ensure CLAP doesn't need equivalent
  - Verify CLAP artifact staging mirrors VST3 staging pattern

  **Must NOT do**:
  - Do NOT add moduleinfo.json to CLAP (CLAP spec doesn't use it)
  - Do NOT change CLAP build commands unless needed for consistency

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: Verification task — ensure CLAP inherits fixes

  **Parallelization**:
  - **Can Run In Parallel**: YES
  - **Parallel Group**: Wave 1 (with Tasks 1, 2, 3)
  - **Blocks**: Tasks 5, 6
  - **Blocked By**: None

  **References**:
  - `build.yml:134-143` — CLAP build step
  - `build.yml:155-163` — signing step (applies to both bundles)

  **Acceptance Criteria**:
  - [ ] CLAP bundle is signed with same flags as VST3
  - [ ] CLAP bundle has entitlements
  - [ ] No CLAP-specific regressions

  **QA Scenarios**:
  ```
  Scenario: Verify CLAP signature matches VST3
    Tool: Bash
    Steps:
      1. codesign -dvv Freeze95.clap
      2. Compare flags with VST3
    Expected Result: Same signature flags (adhoc, runtime)
    Evidence: .sisyphus/evidence/task-4-clap-signature.txt
  ```

  **Commit**: NO (groups with Task 1-3 commit)

- [ ] 5. Flatten artifact zip structure: remove wrapper folder

  **What to do**:
  - In CI "Stage VST3 artifact" step (`build.yml:230-239`), change from:
    ```yaml
    - name: Stage VST3 artifact
      run: |
        mkdir -p Freeze95/build-mac/stage-vst3
        cp -R Freeze95/build-mac/Products/Freeze95.vst3 Freeze95/build-mac/stage-vst3/
    ```
    To:
    ```yaml
    - name: Stage VST3 artifact
      run: |
        mkdir -p Freeze95/build-mac/stage-vst3
        cp -R Freeze95/build-mac/Products/Freeze95.vst3 Freeze95/build-mac/stage-vst3/
        mv Freeze95/build-mac/stage-vst3/Freeze95.vst3 Freeze95/build-mac/stage-vst3-temp/
        rm -rf Freeze95/build-mac/stage-vst3
        mv Freeze95/build-mac/stage-vst3-temp Freeze95/build-mac/stage-vst3
        # Now stage-vst3 contains Freeze95.vst3 directly, not nested in a folder
    ```
  - Actually simpler: just change upload path to `Freeze95/build-mac/Products/Freeze95.vst3` directly, or use a temp folder approach
  - Best approach: set upload-artifact `path` to the `.vst3` bundle directly instead of the parent folder
  - Do same for CLAP artifact

  **Must NOT do**:
  - Do NOT change artifact name (keep `Freeze95-macOS-VST3`)
  - Do NOT break the upload-artifact action

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: Path change in YAML

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Wave 2 (after Wave 1)
  - **Blocks**: Tasks 6, 8
  - **Blocked By**: Tasks 1, 2, 3, 4

  **References**:
  - `build.yml:230-239` — VST3 staging
  - `build.yml:241-250` — CLAP staging
  - GitHub Actions docs: `upload-artifact` path behavior

  **Acceptance Criteria**:
  - [ ] Downloaded zip extracts to `Freeze95.vst3/` directly (no wrapper folder)
  - [ ] `ls Freeze95-macOS-VST3.zip` root contains only `Freeze95.vst3`

  **QA Scenarios**:
  ```
  Scenario: Verify flat zip structure
    Tool: Bash
    Steps:
      1. Download CI artifact
      2. unzip -l Freeze95-macOS-VST3.zip | head -5
    Expected Result: First file is "Freeze95.vst3/Contents/..." not "Freeze95-macOS-VST3/Freeze95.vst3/..."
    Evidence: .sisyphus/evidence/task-5-flat-zip.txt
  ```

  **Commit**: YES
  - Message: `fix(macOS): flatten artifact zip structure for correct install`
  - Files: `.github/workflows/build.yml`

- [ ] 6. Add CI verification step: signature, bundle, Gatekeeper

  **What to do**:
  - Add a new CI step after "Validate code signatures" called "Final macOS plugin validation":
    ```yaml
    - name: Final macOS plugin validation
      run: |
        VST3="Freeze95/build-mac/Products/Freeze95.vst3"
        CLAP="Freeze95/build-mac/Products/Freeze95.clap"
        
        # 1. Verify bundle structure
        test -d "$VST3/Contents/MacOS/Freeze95" || exit 1
        test -f "$VST3/Contents/Info.plist" || exit 1
        test -f "$VST3/Contents/Resources/moduleinfo.json" || exit 1
        
        # 2. Verify signature
        codesign -vvv --deep "$VST3" || exit 1
        codesign -vvv --deep "$CLAP" || exit 1
        
        # 3. Verify entitlements
        codesign -d --entitlements - "$VST3" | grep -q "disable-library-validation" || exit 1
        
        # 4. Verify universal binary
        lipo -info "$VST3/Contents/MacOS/Freeze95" | grep -q "x86_64" || exit 1
        lipo -info "$VST3/Contents/MacOS/Freeze95" | grep -q "arm64" || exit 1
        
        # 5. Gatekeeper assessment (ad-hoc may warn but should not hard-fail)
        spctl --assess --type install "$VST3" || true
        
        echo "All validations passed"
    ```
  - This step must FAIL the build if any critical check fails

  **Must NOT do**:
  - Do NOT make Gatekeeper assessment fail the build (ad-hoc will always warn)
  - Do NOT remove existing validation steps

  **Recommended Agent Profile**:
  - **Category**: `quick`
  - **Skills**: []
  - Reason: YAML step addition with bash commands

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Wave 2 (after Wave 1)
  - **Blocks**: Task 8
  - **Blocked By**: Tasks 1, 2, 3, 4, 5

  **References**:
  - `build.yml:183-190` — existing validation
  - Apple docs: `codesign -vvv`, `lipo -info`, `spctl --assess`

  **Acceptance Criteria**:
  - [ ] New CI step added
  - [ ] Step fails build on signature/bundle/entitlements failure
  - [ ] CI log shows "All validations passed"

  **QA Scenarios**:
  ```
  Scenario: Verify CI validation passes
    Tool: Bash (read CI logs)
    Steps:
      1. Trigger CI build
      2. Read "Final macOS plugin validation" step log
    Expected Result: "All validations passed" and green checkmark
    Evidence: .sisyphus/evidence/task-6-ci-validation.txt
  ```

  **Commit**: YES
  - Message: `ci(macOS): add comprehensive plugin validation step`
  - Files: `.github/workflows/build.yml`

- [ ] 7. Update user install instructions (succinct, direct)

  **What to do**:
  - Create/update `docs/INSTALL_MACOS.md` with exact steps:
    ```markdown
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
    ```
  - Also update README.md or create a short section
  - Keep it under 20 lines for email format

  **Must NOT do**:
  - Do NOT include Windows instructions here
  - Do NOT mention Developer ID or notarization
  - Do NOT use technical jargon without explanation

  **Recommended Agent Profile**:
  - **Category**: `writing`
  - **Skills**: []
  - Reason: Technical documentation

  **Parallelization**:
  - **Can Run In Parallel**: YES
  - **Parallel Group**: Wave 2 (with Tasks 5, 6)
  - **Blocks**: Task 8
  - **Blocked By**: None

  **References**:
  - Current user feedback (Tom, Pierre, Andre)
  - Common macOS plugin install patterns

  **Acceptance Criteria**:
  - [ ] `docs/INSTALL_MACOS.md` exists and is under 30 lines
  - [ ] Instructions include exact Terminal command
  - [ ] Includes troubleshooting for nested folder issue
  - [ ] Includes signature verification command

  **QA Scenarios**:
  ```
  Scenario: Verify install doc readability
    Tool: Read tool
    Steps:
      1. Read docs/INSTALL_MACOS.md
      2. Count lines
      3. Verify all commands are copy-pasteable
    Expected Result: Under 30 lines, clear steps, no ambiguity
    Evidence: .sisyphus/evidence/task-7-install-doc.txt
  ```

  **Commit**: YES
  - Message: `docs(macOS): add succinct install instructions with troubleshooting`
  - Files: `docs/INSTALL_MACOS.md`

- [ ] 8. Trigger CI test build and capture evidence

  **What to do**:
  - Push all commits to main (or create a PR)
  - Trigger CI workflow manually or via push
  - Download the resulting artifacts
  - Run verification commands locally and capture output:
    ```bash
    # Extract
    unzip Freeze95-macOS-VST3.zip -d /tmp/freeze95-test
    
    # Verify structure
    ls /tmp/freeze95-test/
    
    # Verify signature
    codesign -dvv /tmp/freeze95-test/Freeze95.vst3
    
    # Verify entitlements
    codesign -d --entitlements - /tmp/freeze95-test/Freeze95.vst3
    
    # Verify binary
    lipo -info /tmp/freeze95-test/Freeze95.vst3/Contents/MacOS/Freeze95
    
    # Verify plist
    plutil -lint /tmp/freeze95-test/Freeze95.vst3/Contents/Info.plist
    grep -i "iplugeffect\|acme" /tmp/freeze95-test/Freeze95.vst3/Contents/Info.plist || echo "Clean"
    ```
  - Save all output to `.sisyphus/evidence/`
  - If any verification fails, create a follow-up task

  **Must NOT do**:
  - Do NOT test on a Mac (we don't have one — rely on CI + bash verification)
  - Do NOT release to users until evidence shows all checks pass

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
  - **Skills**: []
  - Reason: Multi-step verification requiring CI coordination

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Final integration
  - **Blocks**: F1-F4
  - **Blocked By**: Tasks 5, 6, 7

  **References**:
  - All previous tasks' acceptance criteria
  - `docs/INSTALL_MACOS.md` for user-facing verification

  **Acceptance Criteria**:
  - [ ] CI build passes completely
  - [ ] All verification commands produce expected output
  - [ ] Evidence files saved to `.sisyphus/evidence/`
  - [ ] No "IPlugEffect" or "Acme" in plist
  - [ ] Signature shows `adhoc,runtime`
  - [ ] Entitlements show `disable-library-validation`
  - [ ] Zip is flat (no wrapper folder)

  **QA Scenarios**:
  ```
  Scenario: Full end-to-end CI verification
    Tool: Bash + CI artifact download
    Steps:
      1. Download artifact from CI run
      2. Run all verification commands
      3. Check evidence files exist
    Expected Result: All checks pass, evidence captured
    Evidence: .sisyphus/evidence/task-8-e2e-verification.txt
  ```

  **Commit**: NO (this is verification, not code change)

---

## Final Verification Wave

> 4 review agents run in PARALLEL. ALL must APPROVE.

- [ ] F1. **Plan Compliance Audit** — `oracle`
  Read the plan end-to-end. Verify each task matches the actual diff. Check evidence files exist.
  Output: `Tasks [N/N compliant] | Evidence [N/N] | VERDICT: APPROVE/REJECT`

- [ ] F2. **Code Quality Review** — `unspecified-high`
  Verify build.yml syntax is valid. Check no YAML indentation errors. Ensure no secrets in plaintext.
  Output: `YAML [PASS/FAIL] | Secrets [CLEAN/N] | VERDICT`

- [ ] F3. **Bundle Structure + Signature QA** — `unspecified-high`
  Download CI artifact, extract, verify: flat structure, `codesign -dvv` passes, `lipo -info` shows universal, Info.plist has correct values.
  Output: `Structure [PASS/FAIL] | Signature [PASS/FAIL] | Binary [PASS/FAIL] | VERDICT`

- [ ] F4. **Scope Fidelity Check** — `deep`
  For each task: read "What to do", read actual diff. Verify 1:1 — no missing features, no scope creep.
  Output: `Tasks [N/N compliant] | Contamination [CLEAN/N] | VERDICT`

---

## Commit Strategy

- **Wave 1**: `fix(macOS): correct signing flags, add entitlements, fix Info.plist`
- **Wave 2**: `fix(macOS): flatten artifact zip, add CI verification, update docs`

---

## Success Criteria

### Verification Commands
```bash
# After CI build completes:
# 1. Download artifact
# 2. Extract
# 3. Verify flat structure
ls Freeze95-macOS-VST3.zip  # Should contain Freeze95.vst3 at root, not nested folder

# 4. Verify signature
codesign -dvv Freeze95.vst3
# Expected: Signature=adhoc, flags=0x10002(adhoc,runtime)

# 5. Verify hardened runtime + entitlements
codesign -d --entitlements - Freeze95.vst3
# Expected: Contains com.apple.security.cs.disable-library-validation

# 6. Verify universal binary
lipo -info Freeze95.vst3/Contents/MacOS/Freeze95
# Expected: Architectures in the fat file: x86_64 arm64

# 7. Verify Info.plist
plutil -lint Freeze95.vst3/Contents/Info.plist
# Expected: OK
# And: CFBundleGetInfoString should NOT contain "IPlugEffect" or "Acme Inc"

# 8. Gatekeeper assessment
spctl --assess --type install Freeze95.vst3
# Expected: Accepted (or equivalent for ad-hoc)
```

### Final Checklist
- [ ] All "Must Have" present
- [ ] All "Must NOT Have" absent
- [ ] CI build passes on macOS runner
- [ ] Artifact zip has flat structure
- [ ] Signature valid (adhoc + runtime)
- [ ] Entitlements present
- [ ] Info.plist corrected
- [ ] Install instructions updated
