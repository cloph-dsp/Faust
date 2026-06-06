Adds a small Dry/Wet (MIX) knob to Freeze95, placed between the transport panel and the power button.

## Changes

- **Freeze95.h** — new `kParamDryWet` in `EParams`, plus `mDryWet` / `mTargetDryWet` smoothed state.
- **Freeze95.cpp**
  - Constructor: `InitDouble("Dry/Wet", 100, 0, 100, 1, "%")`.
  - `LayoutUI`: tiny (64-72 px) `SpeakerKnobControl` labelled **MIX**, sits in the gap between the transport panel and the power button. Reuses the existing knob renderer for visual consistency with CHAOS / LO-FI.
  - `ProcessBlock`: per-sample smooth (50-sample ramp, 0.02 coefficient) combined with the existing power ramp. Output = `(mPowerGain*mDryWet) * wet + (1 - mPowerGain*mDryWet) * dry`. Power-off still hard-bypasses to dry.

## Layout impact

- `powerGap` shrunk 40 -> 16 px; the freed space hosts the new knob.
- `contentWidth` increased by ~36 px net - still fits within the 840 px PLUG_WIDTH with comfortable margins.

## Audio behaviour

- **Power off, MIX = any**: pure dry (power acts as hard bypass - unchanged).
- **Power on, MIX = 100%**: same as before the change.
- **Power on, MIX = 0%**: pure dry through the C++ mixer (Faust still processes, but its output is mixed 0%).
- **Power on, MIX = 50%**: equal blend of processed and dry.

## Why C++ (not Faust)

Faust `Freeze95DSP` exposes a Bypass checkbox but no Dry/Wet. The cleanest way to add a real mix knob is in the C++ wrapper that already crossfades dry/processed for the power button, with the same click-free smoothing pattern.

## Testing

- CI `Build Freeze95` will run on this PR (Windows VST3+CLAP, macOS VST3+CLAP universal, lifecycle test).
