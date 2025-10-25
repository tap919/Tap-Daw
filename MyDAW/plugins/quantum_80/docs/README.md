# Quantum-80

## Overview
Quantum-80 is a simplified CX-80 style hybrid synthesizer combining digital FM with virtual analog synthesis.

## Type
Simplified CX-80 Style Hybrid

## Priority
3

## Description
Digital FM meets virtual analog in this 80s-inspired hybrid synthesizer, offering the best of both worlds with simplified 2-operator FM and dual analog-style oscillators.

## Architecture

### Digital Section
- **FM Engine**: 2-operator (carrier + modulator)
- **Algorithms**: 6 simplified configurations
- **Operators**: Sine wave only for simplicity

### Analog Section
- **Oscillators**: 2 VCOs (saw/square/triangle)
- **Filter**: 4-pole ladder emulation
- **Routing**: FM can modulate analog VCOs

### Shared Components
- **Filter**: Single multimode filter post-mixer
- **Amplifier**: Shared ADSR envelope
- **Effects**: Digital chorus + analog-style delay

## Coding Directives

### FM Implementation
- Phase modulation for cleaner sound
- Simplified algorithm matrix

### Hybrid Routing
- Flexible matrix for digital->analog modulation
- Clean signal path

### Character
- Emphasis on 80s digital artifacts and warmth
- Authentic vintage sound

## Implementation Notes

### File Structure
```
quantum_80/
├── src/
│   ├── Quantum80.cpp          # Main plugin
│   ├── FMEngine.cpp            # FM synthesis
│   ├── AnalogOscillators.cpp  # VCO emulation
│   └── LadderFilter.cpp       # Filter emulation
├── include/
│   ├── Quantum80.h
│   ├── FMEngine.h
│   ├── AnalogOscillators.h
│   └── LadderFilter.h
└── docs/
    └── README.md (this file)
```

## Technical Specifications

### Audio Format
- Sample Rate: 44.1kHz - 192kHz
- Bit Depth: 32-bit float internal
- Channels: Stereo output

### MIDI Implementation
- Full MIDI note range
- Velocity sensitive
- Pitch bend
- Mod wheel for FM depth

## Usage
1. Choose FM algorithm (1-6)
2. Adjust FM modulation index
3. Mix in analog oscillators
4. Shape with filter
5. Apply effects
