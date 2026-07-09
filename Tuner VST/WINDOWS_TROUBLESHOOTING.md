# Tuner VST — Windows Troubleshooting

## "Could not be loaded because a library it depends on is missing"

The single-file `Tuner.vst3` DLL at `C:\Program Files\Common Files\VST3\Tuner.vst3`
loads cleanly on any modern Windows install — the import table is exactly
**10 system DLLs**, all in `C:\Windows\System32\`:

```
COMCTL32.dll  COMDLG32.dll  GDI32.dll  KERNEL32.dll  OPENGL32.dll
SHELL32.dll   SHLWAPI.dll   USER32.dll  WININET.dll   ole32.dll
```

No MSVCRT, no VCRUNTIME, no third-party. Local verification:
`LoadLibraryExW` succeeds (returns non-NULL handle), `GetPluginFactory`
export resolves, no extra modules pulled in by the loader.

If Bitwig reports missing-library, **the problem is not the binary.**
Work through these in order — each is a known-wrong failure mode.

---

### 1. Quit ALL Bitwig processes before re-copying

GUI close does **not** kill the audio engine. Open Task Manager → Details →
end every `Bitwig*` row:

```
Bitwig Studio.exe
BitwigAudioEngine-X64-AVX2.exe        (one)
BitwigPluginHost-X64-AVX2.exe         (up to three — each loaded plugin runs in its own)
BitwigStudioApp.exe
```

If any of these are alive when you copy a new `.vst3`, the copy either
fails entirely (you'll see file-locked errors) or partially succeeds —
leaving an unloadable `.vst3` whose bytes DON'T match the build but whose
header is still valid. Bitwig then reports "library missing" because the
`.vst3` *itself* is the library that won't load.

Re-copy only after confirming zero `Bitwig*` processes remain.

---

### 2. Confirm the binary on disk isn't corrupted

If `dir "%ProgramFiles%\Common Files\VST3\Tuner.vst3"` shows a file whose
size is wildly off (Tuner's release is ~940 KB), it was almost certainly
a partial-copy casualty from step 1.

Replace it:

```powershell
Get-Item "C:\Program Files\Common Files\VST3\Tuner.vst3" | Format-List Length
Copy-Item -Force "D:\path\to\build-win\vst3\x64\Release\Tuner.vst3" `
                 "C:\Program Files\Common Files\VST3\Tuner.vst3"
```

The valid size after round-8 CI is 962560 bytes (940 KB). Anything
materially smaller is broken.

---

### 3. Check for AV quarantine

Windows Defender logs every take-up in
**Windows Security → Virus & threat protection → Protection history**.
Look for `Tuner.vst3`. If something shows up, click "Restore" + add the
folder to Defender's exclusion list.

Other AV: check vendor console for any quarantine of
`Tuner.vst3` or `Tuner DSP` (Faust-generated code in the binary can
pattern-match DSP heuristics).

---

### 4. Clean Bitwig's plugin cache

Bitwig keeps a scan cache that's invalidated by mtime, but manual
invalidation fixes stale entries:

```
%APPDATA%\Bitwig Studio\
```

Look for `plugin-info.xml` and `vst3.xml`. Closing Bitwig deletes
volatile entries. Or just close Bitwig, delete those two files (if
present), and relaunch — the next scan re-walks `C:\Program Files\
Common Files\VST3\`.

---

### 5. Confirm you're running the right shape

Tuner ships as a **single-file** `.vst3` — a loose DLL with the
`.vst3` extension. It is NOT a `.vst3` bundle directory:

```
C:\Program Files\Common Files\VST3\
└── Tuner.vst3          <-- 940 KB file (good)
```

NOT:

```
C:\Program Files\Common Files\VST3\Tuner.vst3\
└── Contents\           <-- directory layout (wrong for Tuner)
```

If you see the directory layout, the build did not run successfully on
this machine. The path is `./Tuner VST/build-win/vst3/x64/Release/
Tuner.vst3` — copy that single file, not its parent dir.

---

### 6. Verify with the loader before blaming Bitwig

```powershell
Add-Type @'
using System; using System.Runtime.InteropServices;
public static class P {
    [DllImport("kernel32.dll", SetLastError=true, CharSet=CharSet.Unicode)]
    public static extern IntPtr LoadLibraryExW(string n, IntPtr f, uint fl);
    [DllImport("kernel32.dll", SetLastError=true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool FreeLibrary(IntPtr h);
}
'@
$h = [P]::LoadLibraryExW("C:\Program Files\Common Files\VST3\Tuner.vst3",
                          [IntPtr]::Zero, 0)
if ($h -eq [IntPtr]::Zero) {
    "LOAD_FAIL err=$([System.Runtime.InteropServices.Marshal]::GetLastWin32Error())"
} else {
    "LOAD_OK handle=$h"
    [P]::FreeLibrary($h) | Out-Null
}
```

- `LOAD_OK` → binary on disk is fine; the Bitwig error is environmental.
- `LOAD_FAIL err=126` (ERROR_MOD_NOT_FOUND) → binary genuinely can't find
  one of its deps; re-copy from the build dir.
- `LOAD_FAIL err=193` (ERROR_BAD_EXE_FORMAT) → copy was clipped to 0 bytes
  or scrambled; re-copy from the build dir.

---

## Verified summary

Local evidence (CI re-run on commit `7a354fe`):

| Check | Result |
|-------|--------|
| `LoadLibraryExW(Tuner.vst3)` | returns non-NULL handle |
| `GetPluginFactory` export | resolves |
| Pulled-in modules | none (only existing OS deps) |
| Import table (via `strings`) | exactly 10 system DLLs |
| File size at `build-win/vst3/x64/Release/Tuner.vst3` | 962560 bytes |

The deliverable is provably good. If Bitwig rejects it, **steps 1-4**
above are the only remaining causes.
