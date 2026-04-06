import re

with open('resources/monitor-bg-on.svg', 'r') as f:
    content = f.read()

scale_y = 306/1248
scale_x = 840/3424

# Extract all path data with fill and full d attribute
paths = re.findall(r'fill="#([A-F0-9]+)" d="([^"]+)"', content)

def get_start_coord(d):
    m = re.search(r'M\s*([\d.]+)\s+([\d.]+)', d)
    if m:
        return float(m.group(1)), float(m.group(2))
    return None, None

# Find paths that start in specific zones that correspond to label plate areas
# Look for left zone (X < 1000) and right zone (X > 2400) in upper region
# In the SVG, the plate should be somewhere in the bezel below the screen

print("=== Paths in LEFT ZONE (SVG X<1000) and UPPER REGION (SVG Y 100-400) ===")
print("(rendered Y = 25-98px)")
for fill, d in paths:
    x, y = get_start_coord(d)
    if x is None: continue
    if x < 1000 and 100 < y < 400:
        print(f"  #{fill} start=({x:.0f},{y:.0f}) -> rendered ({x*scale_x:.1f}, {y*scale_y:.1f})")

print("\n=== Paths in RIGHT ZONE (SVG X>2400) and UPPER REGION (SVG Y 100-400) ===")
for fill, d in paths:
    x, y = get_start_coord(d)
    if x is None: continue
    if x > 2400 and 100 < y < 400:
        print(f"  #{fill} start=({x:.0f},{y:.0f}) -> rendered ({x*scale_x:.1f}, {y*scale_y:.1f})")

# Also look at what falls in the exact zone where brandPlateTop is (Y=28-64 rendered = SVG Y 114-261)
print("\n=== ALL paths in UPPER ZONE SVG Y 60-300 (rendered 15-74px) ===")
for fill, d in paths:
    x, y = get_start_coord(d)
    if x is None: continue
    if 60 < y < 300:
        print(f"  #{fill} start=({x:.0f},{y:.0f}) -> rendered ({x*scale_x:.1f}, {y*scale_y:.1f})")
