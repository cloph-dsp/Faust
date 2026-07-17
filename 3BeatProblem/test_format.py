import sys
# Test the exact format string
name = "Chromatic"
n = 12
ivals = [0,1,2,3,4,5,6,7,8,9,10,11]
elems = ", ".join(str(x) for x in ivals)
result = '    { "%s", %2d, {%s}} },' % (name, n, elems)
print("Result repr:", repr(result))
print("Length:", len(result))
print("Result:", result)

# Check trailing chars
for i, c in enumerate(result):
    if c == '}':
        print(f"  }} at pos {i+1}")
    if c == ',':
        print(f"  , at pos {i+1}")
    if c == ' ' and i > 50:
        print(f"  SP at pos {i+1}")
