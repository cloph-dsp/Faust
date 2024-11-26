
# **Faust DSP Experiments and Plugins**

This repository features DSP experiments and creative audio tools built with [Faust](https://faust.grame.fr/), a language for high-performance real-time audio processing.

---

## **Pandemonium**

**Pandemonium** is a feedback delay DSP with integrated panning, designed to create dynamic and evolving spatial effects.

Automate the panning parameter for dynamic effects. Remember to use **internal panning**, not DAW/host panning.  

### **Panning Modes**
Pandemonium offers five distinct panning styles:
- **Simple Motion**: A single left-to-right pan.  
- **Recursive Motion**: Repeated panning across feedbacks.  
- **Reverse Motion**: Mirrors the pan direction (e.g., left-to-right becomes right-to-left).  
- **Asynchronous Ping-Pong**: Uneven, offbeat stereo shifts.  
- **Randomized Panning**: Includes three variations:  
  - Fixed intervals.  
  - Synced to delay time.  
  - Noise-driven smooth variation.

---

### **Usage**
- Test, tweak it and compile it directly in the [Faust IDE](https://faustide.grame.fr).

---

Stay tuned for more creative DSPs and experiments!
