#!/usr/bin/env python3
"""Generate C++ headers with embedded SVG assets."""

import argparse
from pathlib import Path


def svg_to_c_array_header(svg_path: Path, array_name: str, output_path: Path):
    """Convert SVG to C++ header with embedded data."""
    data = svg_path.read_bytes()
    hex_lines = []
    for i in range(0, len(data), 16):
        chunk = data[i:i + 16]
        hex_values = ','.join(f"0x{b:02x}" for b in chunk)
        hex_lines.append(f"  {hex_values}")
    
    content = f"""// Auto-generated - do not edit
const uint8_t k{array_name}Data[] = {{
{chr(10).join(hex_lines)}
}};
const uint32_t k{array_name}Size = {len(data)};
"""
    output_path.write_text(content)
    print(f"Generated: {output_path.name} ({len(data)} bytes)")


def main():
    parser = argparse.ArgumentParser(description="Generate embedded C++ assets")
    parser.add_argument("--output-dir", type=Path, default=Path(__file__).parent / "iplug2-ui")
    args = parser.parse_args()
    
    resources = Path(args.output_dir).parent.parent / "resources"
    
    svg_to_c_array_header(
        resources / "GrungrFaceplate.svg",
        "GrungrFaceplate",
        args.output_dir / "DODGrungeIPlug2Assets_Faceplate.inc"
    )
    svg_to_c_array_header(
        resources / "knob-cropped.svg",
        "KnobCropped",
        args.output_dir / "DODGrungeIPlug2Assets_Knob.inc"
    )


if __name__ == "__main__":
    main()