#!/usr/bin/env python3
"""Move kScales/kChordTypes data from after constructor to before it."""

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'r') as f:
    lines = f.readlines()

# Find: data starts at line index 9 (after the blank from insertion + // comment block)
# Find where kScales ends (line with "};")
data_start = None
data_end = None
for i, line in enumerate(lines):
    if 'const ScaleType kScales[80]' in line:
        data_start = i
    if data_start is not None and data_end is None and line.strip() == '};':
        data_end = i
        break

# kChordTypes starts after kScales }; and ends at next };
for i in range(data_end + 1, len(lines)):
    if 'const ChordType kChordTypes[100]' in lines[i]:
        # skip to the next };
        for j in range(i, len(lines)):
            if lines[j].strip() == '};':
                data_end = j
                break
        break

print(f"Data: lines {data_start+1} to {data_end+1}")
print(f"Constructor at line: {next(i+1 for i,l in enumerate(lines) if 'ThreeBeatProblem::ThreeBeatProblem' in l)}")

# Extract data
data_lines = lines[data_start:data_end+1]

# Remove data from current position
new_lines = lines[:data_start] + lines[data_end+1:]

# Find insertion point: before the constructor
# The constructor declaration spans lines 7-8 (0-indexed): line 7 is blank, line 8 is "ThreeBeatProblem::ThreeBeatProblem..."
for i, line in enumerate(new_lines):
    if 'ThreeBeatProblem::ThreeBeatProblem' in line:
        insert_at = i  # insert BEFORE this line
        break

# Insert data before constructor
result = new_lines[:insert_at] + data_lines + ['\n'] + new_lines[insert_at:]

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'w') as f:
    f.writelines(result)

print(f"Done. Inserted at line {insert_at+1}")
