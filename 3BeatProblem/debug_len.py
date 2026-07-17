with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'rb') as f:
    lines = f.readlines()

# Chromatic (index 12)
line = lines[12]
print('Chromatic line length:', len(line))
print('Last 15 bytes:', repr(line[-15:]))
# Count chars
asc = line.decode('ascii', errors='replace')
# Find } positions
for i, c in enumerate(asc):
    if c == '}':
        print(f'  }} at pos {i+1} (col {i+1})')
# Print the whole line
print('Full line:', asc)
