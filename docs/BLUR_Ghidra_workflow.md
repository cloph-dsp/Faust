# Ghidra Workflow

This folder holds a reproducible Ghidra setup for analyzing `References/MNSpectralBlurring.component` without relying on a system-wide Java install.

## Layout

- `Invoke-HeadlessImport.ps1`: Imports the AU binary into a local Ghidra project, runs analysis, and exports machine-readable summaries.
- `Open-Ghidra.ps1`: Launches the GUI with the same repo-local Java runtime.
- `scripts/ExportFunctionSummary.java`: Writes a JSON inventory of imported functions.
- `scripts/ExportTargetDecomp.java`: Decompiles the offsets already identified as important in the x86_64 hot path.
- `project/`: Local Ghidra project artifacts.
- `exports/`: Generated JSON and decompilation outputs.
- `logs/`: Headless analyzer and script logs.
- `tools/`: Repo-local Java runtime used by Ghidra.

## Targets

- Binary: `References/MNSpectralBlurring.component/Contents/MacOS/MNSpectralBlurring`
- Project name: `MNSpectralBlurring`

## Commands

From the repo root:

```powershell
powershell -ExecutionPolicy Bypass -File .\ReverseEngineering\Ghidra\Invoke-HeadlessImport.ps1
```

To skip auto-analysis and only import plus run scripts:

```powershell
powershell -ExecutionPolicy Bypass -File .\ReverseEngineering\Ghidra\Invoke-HeadlessImport.ps1 -NoAnalysis
```

To open the GUI:

```powershell
powershell -ExecutionPolicy Bypass -File .\ReverseEngineering\Ghidra\Open-Ghidra.ps1
```

## Outputs

Headless runs write:

- Function inventory JSON to `exports/`
- Targeted decompilation `.c` files to `exports/`
- Analyzer log to `logs/analyze-*.log`
- Script log to `logs/scripts-*.log`

The checked-in scripts are the durable part of the workflow. The Java runtime, logs, and Ghidra project state are intentionally ignored by git.
