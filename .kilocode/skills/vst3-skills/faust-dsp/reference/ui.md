# Faust UI Elements Reference

## Input Controls

### button("name")
Momentary button, outputs 1 when pressed, 0 otherwise.
```
gate = button("Gate");
process = osc(440) * gate;
```

### checkbox("name")
Toggle switch, outputs 0 or 1.
```
bypass = checkbox("Bypass");
process = no.noise * (1 - bypass);
```

### hslider("name", default, min, max, step)
Horizontal slider with continuous values.
```
freq = hslider("Frequency", 440, 20, 20000, 1);
process = os.osc(freq);
```

### vslider("name", default, min, max, step)
Vertical slider - same syntax as hslider.

### nentry("name", default, min, max, step)
Numeric entry for discrete values.

## Grouping Controls

### hgroup("name", x)
Groups controls horizontally in UI.
```
process = hgroup("Oscillator", os.osc(freq) * gain)
with {
  freq = hslider("[0]Freq", 440, 20, 2000, 1);
  gain = hslider("[1]Gain", 0.5, 0, 1, 0.01);
};
```

### vgroup("name", x)
Groups controls vertically.

### tgroup("name", x)
Tab-based grouping.

## Output Displays

### vbargraph("name", min, max, signal)
Vertical meter/bargraph.
```
level = vbargraph("Level", 0, 1, abs(x));
```

### hbargraph("name", min, max, signal)
Horizontal meter.

## Labels and Metadata

### Ordering with [n] Prefix
Controls appear in order of bracket number.
```
freq = hslider("[0]Frequency", 440, ...);
gain = hslider("[1]Gain", 0.5, ...);
```

### Style Metadata
- `[style:knob]` - Display as knob
- `[style:menu]` - Display as dropdown
- `[style:radio]` - Display as radio buttons
- `[style:led]` - Display as LED
- `[style:numerical]` - Display as number

### Unit Metadata
- `[unit:dB]` - Decibel display
- `[unit:Hz]` - Hertz display
- `[unit:s]` - Seconds display
- `[unit:%]` - Percentage display

### Scale Metadata
- `[scale:log]` - Logarithmic scale
- `[scale:exp]` - Exponential scale

### Tooltip
- `[tooltip:description]` - Help text

### MIDI Mapping
- `[midi:ctrl n]` - MIDI controller mapping

## Complete Example

```
import("stdfaust.lib");

freq = hslider("[0]Freq [unit:Hz] [scale:log]", 440, 20, 20000, 1);
gain = hslider("[1]Gain [unit:dB] [style:knob]", 0, -60, 12, 0.1) : ba.db2linear;
gate = button("[2]Gate");
attack = hslider("[3]Attack [unit:s]", 0.01, 0.001, 1, 0.001);
decay = hslider("[4]Decay [unit:s]", 0.1, 0.001, 1, 0.001);
sustain = hslider("[5]Sustain", 0.5, 0, 1, 0.01);
release = hslider("[6]Release [unit:s]", 0.3, 0.001, 2, 0.001);

env = en.adsr(attack, decay, sustain, release, gate);

process = os.osc(freq) * env * gain;
```

## Smoothing

UI elements run at control rate. Use smoothing to prevent clicks:

```
smooth(c) = *(1-c) : +~*(c);  // Simple smoothing

// Or use library function
freq = hslider("freq", 440, 20, 2000, 1) : si.smoo;
```

## Attach

Use `attach` to connect UI to signal path without affecting signal:
```
freq = hslider("freq", 440, 20, 2000, 1);
meter = hbargraph("meter", 0, 1, abs(freq));
process = attach(freq, os.osc(freq) * meter);
```