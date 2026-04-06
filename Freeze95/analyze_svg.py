import re

with open('resources/monitor-bg-on.svg', 'r') as f:
    content = f.read()

# Extract fill colors and first M coordinates from path d attributes
paths = re.findall(r'fill="#([A-F0-9]+)" d="M([\d.]+) ([\d.]+)', content)
ys = [float(y) for _,_,y in paths]
print(f'Total paths: {len(paths)}')
print(f'SVG dimensions: 3424 x 1248')
print(f'Rendered at: 840 x 306 (scale = {840/3424:.4f} x = {306/1248:.4f} y)')
scale_x = 840/3424
scale_y = 306/1248

# The label plate indentations in the SVG should be in the top area
# Look at all paths with Y < 200 in SVG coords (< ~49px rendered)
top_paths = [(c,float(x),float(y)) for c,x,y in paths if float(y) < 200]
top_paths.sort(key=lambda t: t[2])
print(f'\nPaths starting in Y<200 SVG coords ({200*scale_y:.0f}px rendered): {len(top_paths)}')
print('Y distribution:')
buckets = {}
for c,x,y in top_paths:
    bucket = int(y/20)*20
    buckets[bucket] = buckets.get(bucket, 0) + 1
for b in sorted(buckets):
    rendered = b * scale_y
    print(f'  SVG Y {b:4d}-{b+20:4d} (rendered {rendered:.0f}-{(b+20)*scale_y:.0f}px): {buckets[b]} paths')

# Also check what's happening in specific Y bands that might be the label plates
print('\nX distribution of top paths (to find left vs right plate):')
left_top = [(c,x,y) for c,x,y in top_paths if x < 1000]
right_top = [(c,x,y) for c,x,y in top_paths if x > 2400]
print(f'  Left zone (X<1000): {len(left_top)} paths, Y range {min(y for _,_,y in left_top) if left_top else "N/A":.1f} - {max(y for _,_,y in left_top) if left_top else "N/A":.1f}')
print(f'  Right zone (X>2400): {len(right_top)} paths, Y range {min(y for _,_,y in right_top) if right_top else "N/A":.1f} - {max(y for _,_,y in right_top) if right_top else "N/A":.1f}')

# The very first path in the SVG usually sets the background - let's see the first 5
print('\nFirst 5 paths:')
for c,x,y in paths[:5]:
    print(f'  fill=#{c} start=({x},{y}) rendered=({float(x)*scale_x:.1f},{float(y)*scale_y:.1f})')
