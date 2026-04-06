import re

with open('resources/monitor-bg-on.svg', 'r') as f:
    content = f.read()

scale_y = 306/1248
scale_x = 840/3424

# Extract all path data with fill colors
paths = re.findall(r'fill="#([A-F0-9]+)" d="([^"]+)"', content)
print(f'Total paths: {len(paths)}')

# Parse bounding boxes from path 'd' attributes
def path_bounds(d):
    coords = re.findall(r'M\s*([\d.]+)\s+([\d.]+)', d)
    all_nums = re.findall(r'[-]?\d+\.?\d*', d[1:])  # skip first M
    all_floats = [float(n) for n in all_nums]
    # Rough x,y min/max from all numbers alternating x,y
    xs = all_floats[0::2]
    ys = all_floats[1::2]
    if not xs or not ys: return None
    return min(xs), min(ys), max(xs), max(ys)

# Classify paths by color to find the monitor screen vs body areas
print("\nColor zones in SVG:")
color_bounds = {}
for fill, d in paths:
    b = path_bounds(d)
    if b is None: continue
    if fill not in color_bounds:
        color_bounds[fill] = {'minY': b[1], 'maxY': b[3], 'minX': b[0], 'maxX': b[2], 'count': 0}
    color_bounds[fill]['minY'] = min(color_bounds[fill]['minY'], b[1])
    color_bounds[fill]['maxY'] = max(color_bounds[fill]['maxY'], b[3])
    color_bounds[fill]['count'] += 1

# Sort by count desc, show top 20
top = sorted(color_bounds.items(), key=lambda x: -x[1]['count'])
print("Top colors by path count:")
for fill, info in top[:20]:
    print(f"  #{fill} ({info['count']} paths): Y {info['minY']:.0f}-{info['maxY']:.0f} -> rendered {info['minY']*scale_y:.1f}-{info['maxY']*scale_y:.1f}px")

# Find the dominant top-strip area (the monitor screen) - likely a horizontal band
print("\n\nVertical bands analysis:")
for y_start in range(0, 450, 25):
    y_end = y_start + 25
    band = [fill for fill,d in paths if any(
        float(y) >= y_start and float(y) < y_end 
        for y in re.findall(r'M[\d.]+\s+([\d.]+)', d[:50])
    )]
    if band:
        rendered_start = y_start * scale_y
        rendered_end = y_end * scale_y
        print(f"  SVG Y {y_start:4d}-{y_end:4d} (rendered {rendered_start:.0f}-{rendered_end:.0f}px): {len(band)} paths start here")
