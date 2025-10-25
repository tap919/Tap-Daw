# MyDAW Plugins

This directory contains plugin implementations for the MyDAW project. Each plugin is a self-contained audio processing module that can be used within the DAW.

## Plugin List

### 1. Nostalgia-Tron (Priority 2)
**Type**: Simplified Mellotron Emulation

A streamlined tape replay synthesizer focusing on essential Mellotron sounds with authentic tape effects.

**Key Features**:
- 4 classic sample sets (strings, choir, flute, cellos)
- 8-second sample playback
- 3-voice polyphony with oldest-note stealing
- Tape effects (wow, flutter, tape hiss)
- ADSR envelope with release trail

**Directory**: `nostalgia_tron/`

---

### 2. Quantum-80 (Priority 3)
**Type**: Simplified CX-80 Style Hybrid

Digital FM meets virtual analog in this 80s-inspired hybrid synthesizer.

**Key Features**:
- 2-operator FM engine with 6 algorithms
- 2 analog-style VCOs (saw/square/triangle)
- 4-pole ladder filter emulation
- Digital chorus and analog-style delay
- Flexible FM-to-analog modulation routing

**Directory**: `quantum_80/`

---

### 3. Rhythm Composer (Priority 4)
**Type**: Sampling Drum Machine + Bass Integration

16-pad sampler with integrated bass synth and advanced pattern sequencing.

**Key Features**:
- 16 sample pads with per-pad processing
- 8-voice polyphony with priority system
- 808-style bass synthesizer
- 16-step sequencer with probability and flam
- Pattern chaining and sidechain compression

**Directory**: `rhythm_composer/`

---

### 4. Velocity EQ (Priority 5)
**Type**: Speed-Based Intelligent Equalizer

FabFilter-style EQ with intelligent presets and real-time analysis.

**Key Features**:
- 8 parametric bands with multiple filter types
- Real-time FFT analysis (4096-point)
- Dynamic processing on 4 bands
- Intelligent preset system
- AI-assisted frequency recommendations

**Directory**: `velocity_eq/`

---

### 5. Momentum Delay (Priority 6)
**Type**: Speed-Based Tempo Intelligent Delay

Tempo-synced delay with rhythmic feels and modulation.

**Key Features**:
- Tap tempo with beat detection
- Multiple note divisions (1/4, 1/8, 1/16, triplets)
- Rhythmic feels (straight, swing, shuffle, dotted)
- True stereo ping-pong operation
- LFO modulation for chorus/flange effects

**Directory**: `momentum_delay/`

---

### 6. Analytica Vaccine (Priority 7)
**Type**: Metric A/B Analysis Hybrid Plugin

A/B comparison with intelligent problem detection and correction.

**Key Features**:
- True bypass A/B comparison with level matching
- Automatic problem detection (clipping, phase, mud, harshness)
- Auto de-esser and dynamic rumble filter
- LUFS metering and phase correlation analysis
- Conservative automatic correction with manual override

**Directory**: `analytica_vaccine/`

---

### 7. Fractal Remixer (Priority 8)
**Type**: Sound Kit Granular Resynthesizer

20-sample granular remixer for creative sound transformation.

**Key Features**:
- Up to 20 samples (5 seconds each)
- Automatic transient, pitch, and texture detection
- Granular synthesis with adjustable parameters
- Scale-aware auto-pitching
- Pattern-based grain triggering
- Tempo-synced grain timing

**Directory**: `fractal_remixer/`

---

### 8. Sonnet Composer (Priority 9)
**Type**: MIDI to Full Composition Engine

Transforms 8-bar MIDI input into complete 2-minute compositions.

**Key Features**:
- Analyzes chord progression and melodic contour
- Automatic section generation (verse, chorus, bridge)
- Style presets (electronic, cinematic, ambient, pop)
- Intelligent variation algorithms
- Exports MIDI, audio stems, or project files

**Directory**: `sonnet_composer/`

---

## Building the Plugins

### Prerequisites
- CMake 3.24 or higher
- C++20 compatible compiler (GCC 10+, Clang 11+, MSVC 2019+)

### Build Instructions

```bash
cd MyDAW
mkdir -p build
cd build
cmake ..
make
```

This will build the main DAW executable and all plugins as shared libraries.

### Individual Plugin Build

To build a specific plugin:

```bash
cd MyDAW/plugins/nostalgia_tron
mkdir -p build
cd build
cmake ..
make
```

## Plugin Architecture

All plugins inherit from the `Node` base class defined in `engine/Node.h`:

```cpp
struct Node {
    virtual ~Node(){};
    virtual void prepare(double sampleRate, int maxBlockSize) = 0;
    virtual void process(const AudioBlock& block) = 0;
    virtual int latencySamples() const = 0;
};
```

### AudioBlock Structure

```cpp
struct AudioBlock {
    float** in;     // Input audio buffers
    float** out;    // Output audio buffers
    int frames;     // Number of frames to process
    double sr;      // Sample rate
};
```

## Development Guidelines

### Adding a New Plugin

1. Create a new directory under `plugins/`
2. Create subdirectories: `src/`, `include/`, `docs/`
3. Implement the `Node` interface in your main plugin class
4. Create a `CMakeLists.txt` file
5. Add the plugin to `plugins/CMakeLists.txt`
6. Document the plugin in `docs/README.md`

### Code Style

- Use C++20 features where appropriate
- Follow the existing naming conventions
- Keep implementations header-only where performance is not critical
- Use `namespace mydaw::plugins::<plugin_name>` for each plugin

### Testing

Each plugin should include:
- Unit tests for core algorithms
- Integration tests with the DAW
- Performance benchmarks
- Audio quality validation

## Documentation

Each plugin directory contains:
- `docs/README.md`: Comprehensive plugin documentation
- Implementation details and architecture
- Usage instructions
- Technical specifications

## License

See the main MyDAW LICENSE file for licensing information.

## Contributing

Contributions are welcome! Please follow the development guidelines and ensure your code builds cleanly before submitting.
