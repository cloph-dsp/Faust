# SVG Workflow for VST3 Plugins

Guide for creating, optimizing, and implementing SVG assets in iPlug2, JUCE, and Faust.

## SVG Creation Best Practices

### Design Tools
- **Figma**: Best for collaborative workflows, easy SVG export
- **Adobe Illustrator**: Industry standard, excellent export options
- **Affinity Designer**: Affordable, good SVG output
- **Inkscape**: Free option, manual optimization needed

### Export Settings
```
Format: SVG
- Remove text converted to paths (if you want editable text)
- Flatten transforms
- Minify output
- Decimal precision: 2
- ID naming: semantic (knob-body, knob-indicator)
```

### SVG Structure for Controls
```xml
<svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
  <g id="knob-body">
    <circle cx="50" cy="50" r="45" fill="#2a2a2a" />
  </g>
  <g id="knob-indicator">
    <rect x="48" y="10" width="4" height="20" rx="2" fill="#5a9fd4" />
  </g>
  <g id="knob-scale">
    <!-- Optional scale markings -->
  </g>
</svg>
```

## SVG Optimization

### SVGO Configuration
```json
{
  "plugins": [
    "removeDoctype",
    "removeXMLProcInst",
    "removeComments",
    "removeMetadata",
    "removeEditorsNSData",
    "cleanupAttrs",
    "mergeStyles",
    "inlineStyles",
    "minifyStyles",
    "cleanupIds",
    "removeUselessDefs",
    "cleanupNumericValues": {
      "floatPrecision": 2
    },
    "convertColors",
    "removeUnknownsAndDefaults",
    "removeNonInheritableGroupAttrs",
    "removeUselessStrokeAndFill",
    "cleanupEnableBackground",
    "removeHiddenElems",
    "removeEmptyText",
    "convertShapeToPath",
    "convertEllipseToCircle",
    "moveElemsAttrsToGroup",
    "moveGroupAttrsToElems",
    "collapseGroups",
    "convertPathData",
    "removeEmptyAttrs",
    "removeEmptyContainers",
    "removeUnusedNS",
    "sortAttrs"
  ]
}
```

### Optimization Command
```bash
npx svgo input.svg -o output.svg --config svgo.config.js
```

### Size Targets
- **Simple icons**: < 1 KB
- **Knob graphics**: 2-5 KB
- **Complex illustrations**: 10-20 KB
- **Full panel backgrounds**: 30-50 KB

## iPlug2 Implementation

### Loading SVGs
```cpp
// In plugin constructor or mMakeGraphicsFunc
auto knobSvg = LoadSVG("knob.svg");
auto bodySvg = LoadSVG("knob-body.svg");
auto indicatorSvg = LoadSVG("knob-indicator.svg");
```

### ISVGBase Custom Control
```cpp
class SVGKnob : public ISVGBase {
public:
  SVGKnob(const IRECT& bounds, int paramIdx, 
           const ISVG& body, const ISVG& indicator)
    : ISVGBase(bounds, paramIdx, body)
    , mIndicator(indicator) {}
  
  void Draw(IGraphics& g) override {
    // Draw body
    g.DrawSVG(mSvg, *this);
    
    // Draw rotated indicator
    double normalizedValue = GetValue(); // 0-1
    double angle = -135 + (normalizedValue * 270); // degrees
    
    IGraphics::Transform t(*this, 
      AffineTransform::Rotation(angle * (kPI / 180.0), 
                               GetBounds().GetCenter().x,
                               GetBounds().GetCenter().y));
    
    g.SaveTransform(t);
    g.DrawSVG(mIndicator, *this);
    g.RestoreTransform();
  }
  
private:
  ISVG mIndicator;
};
```

### SVG Knob Factory Pattern
```cpp
// Register SVG-based controls
pGraphics->AttachControl(new SVGKnob(
  IRECT(50, 50, 120, 120),
  kParamGain,
  LoadSVG("knob-body.svg"),
  LoadSVG("knob-indicator.svg")
));
```

### Performance Tips
- Pre-render static SVGs to bitmaps on load
- Use `SetDirty(false)` during batch updates
- Cache rotation transforms

## JUCE Implementation

### Loading SVGs
```cpp
// Load SVG file
File svgFile("knob.svg");
SVG svgDocument = SVG::parse(svgFile);

// Convert to Drawable
std::unique_ptr<Drawable> knobDrawable = svgDocument.createDrawable();
```

### Custom Component with SVG
```cpp
class SVGKnob : public Component {
public:
  SVGKnob()
    : knobBody(SVG::parse(File::getCurrentWorkingDirectory()
           .getChildFile("knob.svg")))
  {}
  
  void paint(Graphics& g) override {
    knobBody->drawWithin(g, getLocalBounds().toFloat(),
                         RectanglePlacement::fillDestination);
  }
  
  void mouseDrag(const MouseEvent& event) override {
    double newValue = calculateValueFromDrag(event);
    setValue(newValue);
    repaint();
  }
  
private:
  std::unique_ptr<Drawable> knobBody;
};
```

### Performance Tips
- Render complex SVGs to `Image` once, then draw image
- Use `juce::Image::BitmapData` for pixel manipulation if needed
- JUCE 8+: Direct2D handles SVGs efficiently on Windows

### Caching Strategy
```cpp
class CachedSVGKnob : public Component {
public:
  CachedSVGKnob() {
    // Pre-render to image on construction
    auto svg = SVG::parse(File("knob.svg"));
    auto drawable = svg.createDrawable();
    
    RenderEngine engine(*this);
    engine.setBackgroundColour(Colours::transparentBlack);
    cachedImage = engine.render(drawable->getWidth(),
                               drawable->getHeight());
  }
  
  void paint(Graphics& g) override {
    g.drawImageAt(cachedImage, 0, 0);
  }
  
private:
  Image cachedImage;
};
```

## Faust UI Customization

### Architecture File Approach
```cpp
// In your VST architecture file
// Custom UI function that loads SVG
class SVGKnobUI : public uiBasicItem {
public:
  SVGKnobUI(const char* label, FAUSTFUNC callback, 
             const std::string& svgPath)
    : uiBasicItem(label, callback)
    , svgPath(svgPath) {}
    
  void updateMesh() override {
    // Load and render SVG knob
    // This is framework-specific
  }
  
private:
  std::string svgPath;
};
```

### Hybrid Approach (Recommended)
```cpp
// Generate Faust DSP
// Wrap in iPlug2 or JUCE for custom UI

// mydsp.h - generated by Faust
#include "mydsp.h"

// Plugin wrapper with custom SVG UI
class MyFaustPlugin : public iplug::DelegatePlugin {
public:
  MyFaustPlugin()
    : DelegatePlugin(2, 2)
    , fDsp(new mydsp()) {
    
    SetDelegateDsp(fDsp.get());
    
    // Custom SVG-based UI
    mMakeGraphicsFunc = MakeSVGGraphics;
    mAttachFunc = AttachSVGControls;
  }
};
```

## Asset Organization

### Directory Structure
```
resources/
├── svg/
│   ├── controls/
│   │   ├── knob-rotary.svg
│   │   ├── knob-linear.svg
│   │   ├── button-toggle.svg
│   │   ├── meter-vu.svg
│   │   └── ...
│   ├── icons/
│   │   ├── ic-bypass.svg
│   │   ├── ic-menu.svg
│   │   └── ...
│   └── backgrounds/
│       ├── panel-dark.svg
│       └── panel-light.svg
├── fonts/
│   └── custom-display.ttf
└── themes/
    └── dark-theme.json
```

### Bundling with Plugin
- iPlug2: Add to `resources` folder, use `LoadSVG()`
- JUCE: Add to BinaryData, use `createFromData()`
- Manual: Embed as resource, load at runtime

## Common Issues

### Issue: SVG renders differently across platforms
**Solution**: Test on Windows, macOS. Use path-based graphics over complex effects.

### Issue: SVG file size is too large
**Solution**: Run through SVGO, simplify paths, reduce decimal precision.

### Issue: Gradients render incorrectly
**Solution**: Avoid gradients in SVG. Use solid fills or implement in framework.

### Issue: Text is blurry
**Solution**: Convert text to paths OR use system fonts with native rendering.

### Issue: Control not responding to clicks
**Solution**: Ensure hit areas are defined. Use invisible rect overlays for hit detection.
