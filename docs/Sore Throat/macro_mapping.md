# Sore Throat - Distilled Version (12 Knobs + Selector)

## Simplified Design for Power Users

Based on simplification analysis, here's the final distilled design:

---

## Controls Summary

| # | Control | Type | Details |
|---|---------|------|---------|
| 1 | MIX | Knob | 0-100% |
| 2 | VOICES | Knob | 1-200, logarithmic |
| 3 | MIN DELAY | Knob | Switchable via global SYNC |
| 4 | DELAY SPREAD | Knob | Switchable via global SYNC |
| 5 | FEEDBACK | Knob | -100% to +100% (bipolar) |
| 6 | SCATTER | Knob | 0-10 |
| 7 | STEREO | Knob | Rotation + Width combined |
| 8 | SPEED | Knob | Switchable via global SYNC |
| 9 | RANDOM | Knob | Drift amount |
| 10 | TONE | Knob | Tilt EQ style |
| 11 | MOD | Knob | LFO Mod + Env Mod combined |
| 12 | LFO SHAPE | Selector | 10 shapes (dropdown) |

**Total: 12 knobs + 1 selector + 1 SYNC toggle = 14 controls**

---

## Detailed Definitions

### Knob 1: MIX
- Range: 0-100%
- Purpose: Dry/wet balance

### Knob 2: VOICES
- Range: 1-200
- Scaling: Logarithmic
- Purpose: Number of chorus voices

### Knob 3: MIN DELAY
- Range: Switchable via global SYNC toggle
  - SYNC OFF: 0-250ms
  - SYNC ON: Note values (1/64 to 1/1)
- Purpose: Base delay time

### Knob 4: DELAY SPREAD
- Range: Switchable via global SYNC toggle
  - SYNC OFF: 0-1000ms
  - SYNC ON: Multiplier (1x to 10x Min Delay)
- Purpose: Delay range between voices

### Knob 5: FEEDBACK
- Range: -100% to +100%
- Purpose: Regeneration (positive = normal, negative = phase-cancelled)

### Knob 6: SCATTER (formerly Damper)
- Range: 0-10
- Purpose: Controls how "scattered" voices sound by attenuating voices at the edge of the delay range
- Low = all voices equal volume (wide/scattered sound)
- High = edge voices quieter (focused/contained sound)

### Knob 7: STEREO
- Range: Dual-function
  - Center position = normal stereo width
  - Turn right = increase Width (100-200%)
  - Turn left = Rotation (-180° to +180°)
- Purpose: Stereo field control

### Knob 8: SPEED
- Range: Switchable via global SYNC toggle
  - SYNC OFF: Hz (0.01-20 Hz)
  - SYNC ON: Note values (1/64 to 1/1)
- Purpose: Modulation rate (LFO + Drift base)

### Knob 9: RANDOM
- Range: 0-100%
- Purpose: Amount of drift/randomization in voice rates
- Works with SPEED to control how much voices vary

### Knob 10: TONE
- Range: Tilt EQ style
  - Center = flat
  - Right = brighter
  - Left = darker
- Purpose: Overall brightness control

### Knob 11: MOD
- Range: Combined LFO + Env modulation
  - Controls: LFO→LP, LFO→HP, LFO→Delay, Env→LP, Env→HP, Env→Delay, Input Follow
- Purpose: All modulation depth amounts
- Single knob that affects all modulation targets equally

### Selector: LFO SHAPE
10 options:
1. Sine
2. Triangle
3. Square
4. Ramp Up
5. Ramp Down
6. S&H (Sample & Hold)
7. Smooth S&H
8. Sawtooth
9. Reverse Saw
10. Bitcrush

---

## Global Controls

### SYNC Toggle
- Single toggle button in header
- When ON: MIN DELAY, DELAY SPREAD, and SPEED switch to note-synced values
- When OFF: MIN DELAY, DELAY SPREAD, and SPEED use Hz/ms values
- Visual indicator shows current mode

### Auto-Makeup Gain
- Built-in (no knob needed)
- Ensures consistent output level regardless of settings

---

## Design Philosophy

**Distillation principles applied:**

1. **Combined related controls**: STEREO (Rotation + Width), MOD (LFO + Env), RANDOM nested into SPEED interaction
2. **Global sync**: Single toggle for all time-based knobs instead of individual buttons
3. **Tilt EQ**: Single TONE knob instead of separate HP/LP
4. **Bipolar feedback**: Single knob covers both normal and inverted feedback
5. **Dropdown kept simple**: LFO SHAPE stays as selector (not all shapes need rotary control)

---

## Comparison

| Aspect | Original | Simplified |
|-------|----------|------------|
| Total Parameters | 30+ | 14 |
| Knobs | ~25 | 12 |
| Selectors | 5+ | 1 |
| Toggles | 3 | 1 (SYNC) |
| Mode switches | Complex | Simple global SYNC |

This keeps the essential character of Sore Throat while dramatically reducing complexity for power users who want quick, intuitive control.
