#!/usr/bin/env python3
"""Insert generated kScales and kChordTypes into 3BeatProblem.cpp after line 6."""

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'r') as f:
    lines = f.readlines()

with open(r'D:\Github\VST3 Developments\3BeatProblem\kData_gen.txt', 'r') as g:
    data = g.read()

# Insert after line 7 (0-indexed), which is #include <cstring>
insert_after = 7  # 0-indexed; insert AFTER this line
new_lines = lines[:insert_after+1] + ['\n', data + '\n'] + lines[insert_after+1:]

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'w') as f:
    f.writelines(new_lines)

print('Done')
