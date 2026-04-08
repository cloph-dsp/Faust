import sys
p = r"D:\Github\VST3 Developments\MetallicSnare\MetallicKnobs\build-win\MetallicKnobs.vst3\Contents\x86_64-win\MetallicKnobs.vst3"
try:
    with open(p, 'rb') as f:
        data = f.read()
except Exception as e:
    print('ERROR_OPENING_BINARY', e)
    sys.exit(2)

print('HAS_KNOB' if b'metallic_knob.svg' in data else 'NO_KNOB')
print('HAS_SCREW' if b'metallic_screw_head.svg' in data else 'NO_SCREW')
print('HAS_KNOB_SHORT' if b'metallic_knob' in data else 'NO_KNOB_SHORT')
print('HAS_SCREW_SHORT' if b'metallic_screw' in data else 'NO_SCREW_SHORT')
