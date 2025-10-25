# Velocity EQ

## Overview
Speed-based intelligent equalizer with FabFilter-style analysis and smart presets.

## Type
Speed-Based Intelligent Equalizer

## Priority
5

## Description
Advanced EQ plugin with real-time FFT analysis, 8 parametric bands, dynamic processing, and AI-assisted frequency recommendations.

## Architecture

### Analysis Engine
- Real-time FFT: 4096-point with overlapping
- Frequency tracking: Peak and average detection
- Transient analysis: Attack and sustain separation

### EQ Bands
- Count: 8
- Types: Parametric, shelving, notch, high-pass, low-pass
- Dynamic processing: 4 bands with threshold control

### Preset System
- Vocal clarity: Auto-attenuates 300-500Hz mud
- Kick punch: Boosts 60-80Hz, cuts boxiness
- Master sweetener: Gentle high-shelf air boost
- Guitar presence: Mid-range enhancement

### Visualization
- Real-time spectrum with processing overlay
- Suggestion engine: AI-assisted frequency recommendations

## File Structure
```
velocity_eq/
├── src/
│   ├── VelocityEQ.cpp         # Main plugin
│   ├── FFTAnalyzer.cpp         # Spectral analysis
│   ├── EQBand.cpp              # Band processing
│   └── PresetEngine.cpp        # Smart presets
├── include/
│   ├── VelocityEQ.h
│   ├── FFTAnalyzer.h
│   ├── EQBand.h
│   └── PresetEngine.h
└── docs/
    └── README.md (this file)
```

## Usage
1. Enable real-time spectrum analyzer
2. Add EQ bands by clicking frequency graph
3. Choose band type (parametric, shelf, etc.)
4. Use smart presets for quick starting points
5. Fine-tune with dynamic processing on key bands
6. View before/after processing overlay
