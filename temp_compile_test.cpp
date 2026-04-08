#include <cmath>
struct IKeyPress { int VK; bool S; bool C; };
class IControl { public: bool OnKeyDown(float x,float y,const IKeyPress& k) { return false; }};
class MetallicKnobs {
public:
  class ClophSVGKnobControl {
  public:
    bool OnKeyDown(float x, float y, const IKeyPress& key) {
      // VK codes (Windows) - numeric fallbacks to avoid platform headers
      const int VK_LEFT = 0x25;
      const int VK_UP = 0x26;
      const int VK_RIGHT = 0x27;
      const int VK_DOWN = 0x28;
      const int VK_PRIOR = 0x21; // PageUp
      const int VK_NEXT = 0x22;  // PageDown
      const int VK_HOME = 0x24;
      const int VK_END = 0x23;
      const int VK_RETURN = 0x0D;
      const int VK_SPACE = 0x20;
      const int VK_TAB = 0x09;
      return true;
    }
  };
};
int main() { return 0; }
