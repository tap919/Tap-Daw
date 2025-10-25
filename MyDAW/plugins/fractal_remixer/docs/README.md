# Fractal Remixer

## Overview
Granular resynthesis plugin for creative sound kit transformation with 20-sample capacity.

## Type
Sound Kit Granular Resynthesizer

## Priority
8

## Description
Advanced granular synthesis engine that transforms up to 20 samples into evolving textures, rhythmic patterns, and creative soundscapes.

## Architecture

### Sample Management
- Maximum files: 20
- File types: WAV, MP3
- Maximum length: 5 seconds per sample
- Analysis: Automatic transient, pitch, and texture detection

### Granular Engine
- Grain generation: Random selection from source pool
- Grain parameters: Size, density, pitch, pan, volume
- Playback modes: Random, sequential, probability-based

### Remix Algorithms
- Auto-pitching: Scale-aware transposition
- Rhythm generation: Pattern-based grain triggering
- Texture evolution: Parameter automation over time

### Output Controls
- Variation intensity: From subtle to extreme transformation
- Pattern length: 1 to 16 bars
- Sync: Tempo-synced grain timing

## File Structure
```
fractal_remixer/
├── src/
│   ├── FractalRemixer.cpp     # Main plugin
│   ├── GranularEngine.cpp      # Grain synthesis
│   ├── SampleAnalyzer.cpp      # Sample analysis
│   └── RemixAlgorithms.cpp     # Creative algorithms
├── include/
│   ├── FractalRemixer.h
│   ├── GranularEngine.h
│   ├── SampleAnalyzer.h
│   └── RemixAlgorithms.h
└── docs/
    └── README.md (this file)
```

## Usage
1. Load up to 20 samples into kit slots
2. Automatic analysis extracts key features
3. Choose remix algorithm (texture, rhythm, etc.)
4. Adjust grain parameters for character
5. Set variation intensity
6. Sync to tempo for rhythmic output
7. Export new sound kit or use live
