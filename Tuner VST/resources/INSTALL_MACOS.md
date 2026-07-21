# Tuner — macOS Installation

## 1. Which file do I need?

| Format | File | Use in |
|--------|------|--------|
| **Audio Unit (AU)** | `Tuner.component` | **Logic Pro, GarageBand, MainStage** |
| VST3 | `Tuner.vst3` | Ableton Live, FL Studio, Reaper, Bitwig, Cubase |
| CLAP | `Tuner.clap` | Bitwig, Reaper, Studio One |

> **Logic Pro and GarageBand only load Audio Units.** They cannot use `.vst3` or
> `.clap` files at all — you must install **`Tuner.component`**.

## 2. Easiest install — double-click the helper

This download includes **`Fix-Security-Block.command`**.

1. **Right-click** it → **Open** → **Open**. (A plain double-click is blocked the
   first time because the file came from the internet.)
2. It copies every plugin in this folder to the right place, removes the macOS
   "quarantine" flag, and clears Logic's plug-in cache. No password needed.
3. Restart your DAW.

Prefer to do it by hand? See below.

## 3. Manual install

Copy the bundle you need into the matching per-user folder (no admin password):

```bash
# Audio Unit — Logic / GarageBand / MainStage
cp -R Tuner.component ~/Library/Audio/Plug-Ins/Components/

# VST3
cp -R Tuner.vst3 ~/Library/Audio/Plug-Ins/VST3/

# CLAP
cp -R Tuner.clap ~/Library/Audio/Plug-Ins/CLAP/
```

Then remove the quarantine flag — this is the step that "Allow Anyway" in System
Settings only half does. Run it on whichever bundle you installed, e.g.:

```bash
xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/Components/Tuner.component
```

> `~/Library` is your **user** Library (Finder → Go → hold ⌥ Option → Library),
> not the top-level `/Library`. Both locations work; if you install into the
> system-wide `/Library/Audio/Plug-Ins/...`, put `sudo` in front of the commands.

## 4. Make Logic rescan

Quit Logic, then either:

- delete the AU cache so Logic rebuilds it on launch —
  ```bash
  rm -rf ~/Library/Caches/AudioUnitCache
  ```
  and reopen Logic, **or**
- in Logic: **Logic Pro → Settings → Plug-in Manager**, select **Tuner**, and
  click **Reset & Rescan Selection**.

Tuner then appears under **Audio Units → cloph-dsp**.

## 5. Still not showing up?

Run this in Terminal and read the last lines — it tells you exactly why macOS is
rejecting the AU:

```bash
auval -v aufx TnR1 Clph
```

- `AU VALIDATION SUCCEEDED` → the plugin is fine; just rescan in Logic (step 4).
- "not found" / nothing listed → the `.component` isn't in
  `~/Library/Audio/Plug-Ins/Components/` (or `/Library/...`). Re-check step 3.
- A load/architecture error → make sure you downloaded the latest version.

Still stuck? Email the full output of `auval -v aufx TnR1 Clph` and we'll sort it.
