# Faust MIDI and OSC Reference

## MIDI Support

### Automatic MIDI Mapping

Faust automatically maps certain UI elements to MIDI:

```
// MIDI controller mapping
ctrl = hslider("[midi:ctrl 1] volume", 0.5, 0, 1, 0.01);

// MIDI note input
freq = hslider("freq", 60, 0, 127, 1) : ba.midikey2hz;

// MIDI velocity
velocity = hslider("velocity", 1, 0, 1, 0.01);

// MIDI gate
gate = button("gate");
```

### MIDI Control Numbers

| Controller | Description |
|------------|-------------|
| `[midi:ctrl 0-127]` | MIDI controller |
| `[midi:pitch]` | Pitch wheel |
| `[midi:mod]` | Mod wheel |
| `[midi:vol]` | Volume |
| `[midi:pan]` | Pan |

### MIDI Notes

Convert MIDI note to frequency:
```
key = hslider("key", 60, 0, 127, 1);
freq = key : ba.midikey2hz;
process = os.osc(freq);
```

## OSC Support

### OSC Metadata

Add OSC addresses to controls:
```
freq = hslider("freq [osc:/synth/freq]", 440, 20, 2000, 1);
gain = hslider("gain [osc:/synth/gain][unit:dB]", 0, -60, 12, 0.1);
```

### OSC Options

Compile with OSC support:
```
faust -osc mydsp.dsp
```

### OSC Range

Specify OSC range:
```
value = hslider("value [osc:/path/min 0/max 1]" , 0.5, 0, 1, 0.01);
```

## Smart Keyboard

### Basic Smart Keyboard

```
import("stdfaust.lib");
process = os.osc(freq) * env
with {
  freq = hslider("[0]freq", 440, 20, 2000, 1) : ba.midikey2hz;
  gate = button("[1]gate");
  attack = hslider("[2]attack", 0.01, 0.001, 0.5, 0.001);
  decay = hslider("[3]decay", 0.1, 0.001, 1, 0.001);
  sustain = hslider("[4]sustain", 0.7, 0, 1, 0.01);
  release = hslider("[5]release", 0.3, 0.001, 2, 0.001);
  env = en.adsr(attack, decay, sustain, release, gate);
};
```

### Smart Keyboard with Polyphony

Enable polyphony in architecture:
```
// Compile: faust -poly mydsp.dsp
```

## Compile Options for MIDI/OSC

### Enable OSC
```
faust -osc mydsp.dsp
```

### Enable MIDI
```
faust -midi mydsp.dsp
```

### Enable Both
```
faust -osc -midi mydsp.dsp
```

### Smart Keyboard
```
faust -smartkey mydsp.dsp
```

## Complete Example with MIDI/OSC

```
declare name "Synth";
declare version "1.0";
import("stdfaust.lib");

// Smart keyboard inputs
freq = hslider("[0]freq [midi:key] [scale:log]", 440, 20, 2000, 1) : ba.midikey2hz;
velocity = hslider("[1]velocity [midi:vel]", 1, 0, 1, 0.01);
gate = button("[2]gate [midi:gate]");

// Controls with OSC
cutoff = hslider("[3]cutoff [osc:/synth/cutoff] [scale:log]", 2000, 100, 10000, 1);
res = hslider("[4]resonance [osc:/synth/res]", 5, 0.1, 20, 0.1);
gain = hslider("[5]gain [osc:/synth/gain][unit:dB]" , 0, -60, 12, 0.1) : ba.db2linear;

// ADSR envelope
attack = hslider("[6]attack", 0.01, 0.001, 1, 0.001);
decay = hslider("[7]decay", 0.1, 0.001, 1, 0.001);
sustain = hslider("[8]sustain", 0.7, 0, 1, 0.01);
release = hslider("[9]release", 0.3, 0.001, 2, 0.001);

env = en.adsr(attack, decay, sustain, release, gate);

// Synth voice
voice = os.osc(freq) : fi.resonlp(cutoff, res, 1) : *(env * velocity);

// Master output
process = voice : *(gain);
```

## MIDI Learn

Some architectures support MIDI learn - right-click a control to assign MIDI controller.

## Declare Options for MIDI/OSC

```
declare options "[osc:on]";
declare options "[midi:on]";
declare options "[midi:ctrl]";
declare options "[smartkey:on]";
```