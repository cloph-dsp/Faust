// rasterize_logo.js - converts resources/CLOPH.svg into a raster PNG for
// pre-rasterized IBitmap caching in the Tuner VST.  Run as:
//   node rasterize_logo.js
// Produces resources/CLOPH.png (width=384px, scaled aspect ratio from the SVG).
// Re-run this whenever resources/CLOPH.svg changes.

const fs = require('fs');
const path = require('path');
const { Resvg } = require('@resvg/resvg-js');

const svgPath = path.join(__dirname, '..', 'resources', 'CLOPH.svg');
const pngPath = path.join(__dirname, '..', 'resources', 'CLOPH.png');

const svg = fs.readFileSync(svgPath, 'utf-8');
const resvg = new Resvg(svg, { fitTo: { mode: 'width', value: 384 } });
const pngBuf = resvg.render().asPng();
fs.writeFileSync(pngPath, pngBuf);
console.log(`Wrote ${pngPath} (${pngBuf.length} bytes)`);