# Overlay Integration Guide

This document describes how to integrate an overlay system with MyDAW's MIDI and audio engine architecture.

## Overview

An overlay system provides real-time visual feedback about the DAW's internal state, including:
- Active pattern playback
- MIDI event scheduling
- Timeline position and clip information
- Audio engine status

## Architecture

### Core Components

The overlay system integrates with these key MyDAW components:

1. **MIDI System** (`mydaw::midi`)
   - `TimeBase` - Converts between samples and ticks
   - `Scheduler` - Schedules and gathers MIDI events
   - `Pattern` - Contains note data for playback

2. **Audio Engine** (`mydaw::AudioEngineRT`)
   - Real-time audio processing
   - MIDI event dispatch to samplers
   - Tempo and sample rate management

3. **Arrangement** (`mydaw::arrange`)
   - `Playlist` - Timeline clips
   - `TimelineBridge` - Converts playlist to pattern instances

## Integration Points

### 1. TimeBase Integration

The `TimeBase` class provides essential time conversion utilities:

```cpp
// Get TimeBase from AudioEngineRT
const mydaw::midi::TimeBase& tb = engine.scheduler().get_timebase();

// Convert sample position to tick
int64_t samplePos = /* current playhead position */;
mydaw::midi::Tick currentTick = tb.samples_to_ticks(samplePos);

// Convert tick to samples for visual positioning
mydaw::midi::Tick tick = 960; // 1 beat at 960 PPQ
int64_t samples = tb.tick_to_samples(tick);
```

**Key Constants:**
- `mydaw::midi::kPPQ = 960` - Pulses Per Quarter note

### 2. Pattern and Event Visualization

To display active patterns and their MIDI events:

```cpp
// Access pattern data
const mydaw::midi::Pattern* pattern = /* from pattern registry */;

// Pattern properties
mydaw::midi::Tick length = pattern->length;
const std::vector<mydaw::midi::ChannelPattern>& channels = pattern->channels;

// Iterate through notes for visualization
for (const auto& ch : channels) {
    uint32_t genId = ch.gen;
    for (const auto& note : ch.notes) {
        mydaw::midi::Tick start = note.start;
        mydaw::midi::Tick len = note.len;
        uint8_t pitch = note.pitch;    // MIDI pitch (0-127)
        uint8_t vel = note.vel;         // Velocity (0-127)
        // Draw note overlay at tick position
    }
}
```

### 3. Real-Time Event Monitoring

Monitor MIDI events as they're scheduled:

```cpp
// Gather events for current audio block
std::vector<mydaw::midi::Ev> events;
engine.scheduler().gather_realtime(activePatterns, samplePos, frames, events);

// Process events for overlay display
for (const auto& ev : events) {
    switch (ev.type) {
        case mydaw::midi::EvType::NoteOn:
            // Extract pad info
            auto [bank, pad] = mydaw::GenIdMapper::gen_to_pad(ev.gen);
            // Display note-on overlay at bank/pad with velocity ev.vel
            break;
        case mydaw::midi::EvType::NoteOff:
            // Display note-off overlay
            break;
        case mydaw::midi::EvType::PitchBend:
            // Display pitch bend amount: ev.pb
            break;
    }
}
```

### 4. Timeline Clip Overlay

Display timeline clips with accurate positioning:

```cpp
// Convert playlist to pattern instances
mydaw::arrange::PatternRegistry registry;
// Populate registry with pattern pointers...

std::vector<mydaw::midi::PatternInstance> instances = 
    mydaw::arrange::BuildPatternInstances(playlist, registry, playheadTick);

// Render each clip
for (const auto& inst : instances) {
    if (!inst.enabled || inst.muted) continue;
    
    mydaw::midi::Tick startTick = inst.startTick;
    mydaw::midi::Tick loopLen = inst.loopLengthTicks;
    
    // Convert to pixel coordinates for rendering
    int64_t startSample = tb.tick_to_samples(startTick);
    int64_t endSample = tb.tick_to_samples(startTick + loopLen);
    
    // Draw clip overlay from startSample to endSample
}
```

### 5. Playhead Position

Synchronize overlay playhead with audio engine:

```cpp
class OverlayManager {
    int64_t samplePos_ = 0;
    
    void onAudioCallback(int frames) {
        samplePos_ += frames;
        
        // Convert to tick for musical positioning
        mydaw::midi::Tick tick = tb_.samples_to_ticks(samplePos_);
        
        // Update overlay playhead position
        updatePlayhead(tick);
    }
};
```

## Advanced Features

### Swing Visualization

Display swing-quantized note positions:

```cpp
// Apply swing to events for visualization
std::vector<mydaw::midi::Ev> events = /* gathered events */;
float swingPercent = 60.0f;  // 50-100
mydaw::midi::Tick swingRes = mydaw::midi::kPPQ / 2;  // 8th notes

scheduler.apply_swing(events, swingPercent, swingRes);

// Events now have adjusted sampOff values for display
for (const auto& ev : events) {
    // Render at ev.sampOff position
}
```

### Quantize Grid Overlay

Show quantization grid:

```cpp
// Define quantize grid (e.g., 16th notes)
mydaw::midi::Tick gridSize = mydaw::midi::kPPQ / 4;

// Calculate grid lines for current view
mydaw::midi::Tick viewStart = tb.samples_to_ticks(viewStartSample);
mydaw::midi::Tick viewEnd = tb.samples_to_ticks(viewEndSample);

for (mydaw::midi::Tick tick = 0; tick < viewEnd; tick += gridSize) {
    if (tick >= viewStart) {
        int64_t sample = tb.tick_to_samples(tick);
        // Draw vertical grid line at sample position
    }
}
```

### Multi-Pattern Event Detection

Check for overlapping events across patterns:

```cpp
mydaw::midi::Tick startTick = /* current window start */;
mydaw::midi::Tick endTick = /* current window end */;

bool hasEvents = scheduler.has_events_in_range(instances, startTick, endTick);
if (hasEvents) {
    // Highlight region with event activity
}
```

## Implementation Example

Complete overlay manager implementation:

```cpp
#pragma once
#include "engine/AudioEngineRT.h"
#include "arrange/TimelineBridge.h"

class OverlayManager {
    mydaw::AudioEngineRT& engine_;
    mydaw::arrange::PatternRegistry& registry_;
    int64_t samplePos_ = 0;
    
public:
    OverlayManager(mydaw::AudioEngineRT& eng, 
                   mydaw::arrange::PatternRegistry& reg)
        : engine_(eng), registry_(reg) {}
    
    void update(int frames, const mydaw::arrange::Playlist& playlist) {
        // Update sample position
        samplePos_ += frames;
        
        // Get current timebase
        const auto& tb = engine_.scheduler().get_timebase();
        
        // Convert to tick
        mydaw::midi::Tick currentTick = tb.samples_to_ticks(samplePos_);
        
        // Build active pattern instances
        auto instances = mydaw::arrange::BuildPatternInstances(
            playlist, registry_, currentTick);
        
        // Gather MIDI events for this frame
        std::vector<mydaw::midi::Ev> events;
        engine_.scheduler().gather_realtime(instances, samplePos_, 
                                          frames, events);
        
        // Update overlay visualization
        renderPlayhead(currentTick);
        renderClips(instances, tb);
        renderEvents(events);
    }
    
private:
    void renderPlayhead(mydaw::midi::Tick tick) {
        // Display playhead at tick position
    }
    
    void renderClips(const std::vector<mydaw::midi::PatternInstance>& instances,
                    const mydaw::midi::TimeBase& tb) {
        for (const auto& inst : instances) {
            if (!inst.enabled || inst.muted) continue;
            
            int64_t startSample = tb.tick_to_samples(inst.startTick);
            int64_t endSample = tb.tick_to_samples(
                inst.startTick + inst.loopLengthTicks);
            
            // Render clip rectangle
        }
    }
    
    void renderEvents(const std::vector<mydaw::midi::Ev>& events) {
        for (const auto& ev : events) {
            auto [bank, pad] = mydaw::GenIdMapper::gen_to_pad(ev.gen);
            
            // Flash pad or show event indicator
            if (ev.type == mydaw::midi::EvType::NoteOn) {
                // Highlight pad with velocity intensity
                float intensity = ev.vel / 127.0f;
                flashPad(bank, pad, intensity);
            }
        }
    }
    
    void flashPad(uint8_t bank, uint8_t pad, float intensity) {
        // Visual feedback implementation
    }
};
```

## Performance Considerations

1. **Event Gathering**: Only gather events for the visible time range
2. **Update Rate**: Synchronize overlay updates with audio callback or v-sync
3. **Batch Rendering**: Group similar overlay elements for efficient rendering
4. **Tick Caching**: Cache tick-to-sample conversions for frequently accessed positions

## Thread Safety

- `AudioEngineRT::process()` runs on the audio thread
- Overlay rendering typically runs on the UI thread
- Use lock-free data structures or atomic operations for shared state
- Consider using a ring buffer for event passing from audio to UI thread

## Example Use Cases

### 1. Piano Roll Overlay
Display notes from patterns aligned with timeline:
```cpp
// For each visible pattern
for (const auto& ch : pattern->channels) {
    for (const auto& note : ch.notes) {
        int64_t noteSample = tb.tick_to_samples(clipStart + note.start);
        int64_t noteLen = tb.tick_to_samples(note.len);
        // Draw note rectangle at (noteSample, note.pitch)
    }
}
```

### 2. Pad Matrix Overlay
Show real-time pad triggers:
```cpp
// In audio callback
for (const auto& ev : events) {
    if (ev.type == mydaw::midi::EvType::NoteOn) {
        auto [bank, pad] = mydaw::GenIdMapper::gen_to_pad(ev.gen);
        // Post to UI thread: illuminate pad[bank][pad]
    }
}
```

### 3. Waveform Timeline
Align clips with audio waveforms:
```cpp
for (const auto& clip : playlist.clips) {
    mydaw::midi::Tick startTick = clip.start;
    mydaw::midi::Tick len = clip.len;
    
    int64_t x = tb.tick_to_samples(startTick);
    int64_t width = tb.tick_to_samples(len);
    
    // Draw waveform or clip outline at x with width
}
```

## Testing Overlay Integration

To verify overlay integration:

1. **Timing Accuracy**: Verify tick-to-sample conversions at various tempos
2. **Event Synchronization**: Confirm events trigger overlays at correct times
3. **Pattern Looping**: Test overlay updates during pattern loops
4. **Multi-Pattern**: Verify overlay with multiple overlapping patterns
5. **Swing/Quantize**: Test groove-modified event display

## See Also

- `MyDAW/include/midi/scheduler.hpp` - Event scheduling
- `MyDAW/include/midi/timing.hpp` - Time conversion utilities
- `MyDAW/engine/AudioEngineRT.h` - Real-time audio engine
- `MyDAW/arrange/TimelineBridge.h` - Timeline/pattern bridge
