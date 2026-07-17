#!/usr/bin/env python3
"""Fix the 3BeatProblem.cpp file:
1. Remove duplicate MUSIC-THEORY comments after kChordTypes };
2. Fix constructor to have proper member-initializer on same block
3. Ensure blank line between }; and constructor
"""

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'r') as f:
    lines = f.readlines()

new_lines = []
i = 0
while i < len(lines):
    # Skip duplicate comments
    if '// MUSIC-THEORY DATABASE' in lines[i]:
        # Skip this line and any following comment lines until a blank or code
        while i < len(lines) and ('// MUSIC-THEORY' in lines[i] or
                                    '// ScaleType is defined' in lines[i] or
                                    '// kScales defined here' in lines[i] or
                                    '// ===' in lines[i] or
                                    lines[i].strip() == ''):
            i += 1
        continue

    # Fix: after ThreeBeatProblem constructor declaration, the next non-blank
    # should be the member initializer on the next line
    if 'ThreeBeatProblem::ThreeBeatProblem(const InstanceInfo& info)' in lines[i]:
        new_lines.append(lines[i])  # constructor declaration
        i += 1
        # Skip any blanks
        while i < len(lines) and lines[i].strip() == '':
            i += 1
        # Next should be the member initializer - add it directly after declaration
        # with proper indentation
        if i < len(lines) and lines[i].strip().startswith(':'):
            new_lines.append('\n')
            new_lines.append(lines[i])
            i += 1
        continue

    new_lines.append(lines[i])
    i += 1

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'w') as f:
    f.writelines(new_lines)

print(f'Done. New line count: {len(new_lines)}')
