# **Faust DSP Experiments and Plugins**

Welcome to my repository of audio experiments and plugins created with [Faust](https://faust.grame.fr/), a functional programming language for real-time audio signal processing. This collection explores innovative effects, tools, and ideas for sound design, mixing, and creative audio applications.

## **About Faust**
Faust (Functional Audio Stream) is a powerful language for developing high-performance digital signal processing (DSP) applications. It enables rapid prototyping of effects, synthesizers, and audio tools that can be deployed across various platforms, from standalone applications to plugins and web audio.

## **Projects**
This repository includes both experimental DSPs and more polished designs. Each project includes its source code and documentation for testing or adapting the effects.

# **Pandemonium: Feedback Delay DSP**

Pandemonium is a Faust DSP offering creative delay effects with advanced panning.

## **Panning Modes**
- **Simple Motion**: A single left-to-right pan.  
- **Recursive Motion**: Repeats panning across feedbacks.  
- **Reverse Motion**: Mirrors the pan direction.  
- **Asynchronous Ping-Pong**: Uneven stereo shifts.  
- **Randomized Panning**: Fixed intervals, synced to delay, or noise-driven.

## **Usage**
- Use **internal panning**, not DAW/host panning.  
- Automate the panning parameter for dynamic effects.  
- Test it in the [Faust IDE](https://faustide.grame.fr).
