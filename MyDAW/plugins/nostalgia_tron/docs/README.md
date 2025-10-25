# Nostalgia-Tron

## Overview
Nostalgia-Tron is a streamlined Mellotron emulation plugin focusing on essential tape replay synthesis.

## Type
Simplified Mellotron Emulation

## Priority
2

## Description
Streamlined tape replay synth focusing on essential Mellotron sounds with 8-second sample playback, tape effects, and classic voice management.

## Architecture

### Sample Sets
- **Included Sets**: Strings, Choir, Flute, Cellos
- **Duration**: 8 seconds per sample
- **Format**: SFZ with lossless compression
- **Loading**: On-demand streaming

### Playback
- **Polyphony**: 3 voices
- **Voice Management**: Oldest note stealing
- **Tape Effects**: Wow, flutter, tape hiss
- **Envelope**: ADSR with release trail

### Controls
- **Essential**: Volume, Tone, Vibrato, Attack, Release
- **Simplified Omissions**: Multiple tape heads, complex mechanics

## Coding Directives

### Sample Management
- Memory-mapped file loading for efficient access
- Pre-loading of frequently used samples

### Tape Emulation
- Simple pitch modulation for wow and flutter effects
- Noise layer for authentic tape hiss

### Performance Optimization
- Pre-calculated sample offsets
- Efficient voice allocation
- Minimal CPU overhead

## Implementation Notes

### File Structure
```
nostalgia_tron/
├── src/
│   ├── NostalgiaTron.cpp      # Main plugin implementation
│   ├── SamplePlayer.cpp        # Sample playback engine
│   ├── TapeEmulation.cpp       # Tape effect simulation
│   └── VoiceManager.cpp        # Voice allocation
├── include/
│   ├── NostalgiaTron.h
│   ├── SamplePlayer.h
│   ├── TapeEmulation.h
│   └── VoiceManager.h
├── samples/
│   ├── strings/
│   ├── choir/
│   ├── flute/
│   └── cellos/
└── docs/
    └── README.md (this file)
```

### Key Classes

#### NostalgiaTron (Main Plugin Class)
- Inherits from Node base class
- Manages overall plugin state
- Coordinates sample playback and effects

#### SamplePlayer
- Handles sample loading and streaming
- Memory-mapped file access
- Sample offset calculation

#### TapeEmulation
- Wow and flutter simulation
- Tape hiss generation
- Pitch modulation

#### VoiceManager
- 3-voice polyphony management
- Oldest note stealing algorithm
- Voice state tracking

## Usage

### Basic Operation
1. Load the plugin in your DAW
2. Select desired sample set (strings, choir, flute, or cellos)
3. Play notes via MIDI input
4. Adjust tone, vibrato, attack, and release controls
5. Fine-tune with volume control

### Tape Effects
- **Wow**: Slow periodic pitch variation
- **Flutter**: Fast periodic pitch variation
- **Tape Hiss**: Constant background noise characteristic

### ADSR Envelope
- **Attack**: Time to reach full volume
- **Decay**: Time to reach sustain level
- **Sustain**: Held volume level
- **Release**: Time to fade out after note off

## Technical Specifications

### Audio Format
- Sample Rate: 44.1kHz - 192kHz (host-dependent)
- Bit Depth: 24-bit internal processing
- Channels: Stereo output

### MIDI Implementation
- Note On/Off (full velocity range)
- Pitch Bend
- Sustain Pedal (CC64)
- Expression (CC11)

### CPU Usage
- Typical: 2-5% (single core, modern CPU)
- Maximum: 10% (3 voices active with all effects)

## Future Enhancements
- Additional sample sets
- Per-sample fine-tuning
- Enhanced tape mechanics simulation
- User sample import
