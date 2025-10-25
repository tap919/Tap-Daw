# Rhythm Composer

## Overview
16-pad sampling drum machine with integrated bass synth and pattern sequencing.

## Type
Sampling Drum Machine + Bass Integration

## Priority
4

## Description
Comprehensive drum machine featuring 16 sample pads, built-in bass synthesizer, and advanced pattern sequencer with probability, flam, and parameter lock features.

## Architecture

### Drum Section
- **Pads**: 16
- **Sample Format**: WAV/MP3 import with time-stretching
- **Per-Pad Processing**: Tuning, decay, filter, pan, drive
- **Voice Allocation**: 8-voice polyphony with priority system

### Bass Integration
- **Synth Engine**: 808-style with sub-oscillator
- **Pattern Sequencer**: 32 patterns, 16 steps each
- **Sync**: Tight integration with drum sequencer
- **Sidechain**: Automatic ducking from kick drum

### Sequencer
- **Steps**: 16
- **Advanced Features**: Probability, flam, parameter locks, pattern chaining
- **Resolution**: 16th notes with swing

## File Structure
```
rhythm_composer/
├── src/
│   ├── RhythmComposer.cpp     # Main plugin
│   ├── SamplePad.cpp           # Sample playback
│   ├── BassEngine.cpp          # 808-style bass
│   └── PatternSequencer.cpp    # Step sequencer
├── include/
│   ├── RhythmComposer.h
│   ├── SamplePad.h
│   ├── BassEngine.h
│   └── PatternSequencer.h
├── samples/
│   └── (drum samples directory)
└── docs/
    └── README.md (this file)
```

## Usage
1. Load samples to 16 pads via drag-and-drop
2. Program drum patterns using step sequencer
3. Layer bass patterns on integrated synth
4. Apply per-pad processing (filter, tuning, etc.)
5. Use probability and flam for variation
6. Chain patterns for full arrangements
