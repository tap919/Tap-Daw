# Momentum Delay

## Overview
Tempo-synced delay with rhythmic feels, modulation, and intelligent timing.

## Type
Speed-Based Tempo Intelligent Delay

## Priority
6

## Description
Advanced delay plugin with tap tempo, beat detection, multiple note divisions, rhythmic feels, and modulation capabilities.

## Architecture

### Timing Engine
- Tap tempo with beat detection
- Note divisions: 1/4, 1/4T, 1/8, 1/8T, 1/16, 1/16T
- Rhythmic feels: Straight, swing, shuffle, dotted

### Delay Lines
- Count: 2 (true stereo)
- Ping-pong: True stereo operation
- Maximum time: 2000ms unsynced
- Modulation: LFO on delay time for chorus/flange effects

### Filter System
- Frequency-dependent decay
- High-cut and low-cut per repeat
- Separate filtering in feedback path

### Presets
- Dub echo: Long decays with filtering
- Slapback: Short, single repeat
- Ping-pong: Stereo bouncing
- Rhythmic: Complex timing patterns

## File Structure
```
momentum_delay/
├── src/
│   ├── MomentumDelay.cpp      # Main plugin
│   ├── DelayLine.cpp           # Delay processing
│   ├── TapTempo.cpp            # Tempo detection
│   └── Modulation.cpp          # LFO modulation
├── include/
│   ├── MomentumDelay.h
│   ├── DelayLine.h
│   ├── TapTempo.h
│   └── Modulation.h
└── docs/
    └── README.md (this file)
```

## Usage
1. Enable tempo sync or set manual delay time
2. Choose note division and rhythmic feel
3. Adjust feedback for number of repeats
4. Apply filtering for character
5. Add modulation for movement
6. Use ping-pong for stereo width
