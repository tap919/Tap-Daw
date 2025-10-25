# Sonnet Composer

## Overview
MIDI composition engine that transforms 8 bars of input into a complete 2-minute musical piece.

## Type
MIDI to Full Composition Engine

## Priority
9

## Description
Intelligent composition system that analyzes 8-bar MIDI input and generates complete arrangements with verse, chorus, bridge, and dynamic structure.

## Architecture

### Input Analysis
- MIDI input: 8 bars of melody/harmony
- Analysis parameters: Chord progression, rhythm patterns, melodic contour, harmonic density

### Expansion Engine
- Section generation: Verse, chorus, bridge, breakdown
- Variation algorithms: Transposition, rhythm variation, harmonic development
- Instrumentation logic: Based on input character and genre cues

### Arrangement System
- Dynamic arc: Builds, drops, and transitions
- Length control: Precise 2-minute output
- Export options: MIDI, audio stems, project file

### Style Presets
- Electronic: EDM-style build-ups and drops
- Cinematic: Orchestral development
- Ambient: Evolving textures
- Pop: Verse-chorus structure

## File Structure
```
sonnet_composer/
├── src/
│   ├── SonnetComposer.cpp     # Main plugin
│   ├── InputAnalyzer.cpp       # MIDI analysis
│   ├── ExpansionEngine.cpp     # Section generation
│   └── Arranger.cpp            # Structure builder
├── include/
│   ├── SonnetComposer.h
│   ├── InputAnalyzer.h
│   ├── ExpansionEngine.h
│   └── Arranger.h
└── docs/
    └── README.md (this file)
```

## Usage
1. Record or input 8 bars of MIDI
2. Choose style preset (electronic, cinematic, etc.)
3. Adjust arrangement parameters
4. Generate complete composition
5. Fine-tune sections manually if desired
6. Export as MIDI or audio stems
