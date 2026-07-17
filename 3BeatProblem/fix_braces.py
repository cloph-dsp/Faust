import re

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'r') as f:
    content = f.read()

def transform_section(content, marker):
    start_idx = content.find(marker)
    if start_idx == -1:
        return content
    brace_open = content.find('{', start_idx)
    end_idx = content.find('};', brace_open)
    if end_idx == -1:
        return content
    section = content[brace_open:end_idx + 2]
    new_section = re.sub(
        r',\s*\{\s*([^{}]+?)\s*\}',
        lambda m: ', ' + m.group(1).strip(),
        section
    )
    return content[:brace_open] + new_section + content[end_idx + 2:]

content = transform_section(content, 'const ScaleType kScales[80]')
content = transform_section(content, 'const ChordType kChordTypes[100]')

with open(r'D:\Github\VST3 Developments\3BeatProblem\3BeatProblem.cpp', 'w') as f:
    f.write(content)

print('Brace-elision applied')
