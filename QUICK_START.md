# Quick Start Guide - MyDAW Plugins

## Building All Plugins

```bash
cd MyDAW
mkdir -p build
cd build
cmake ..
make -j4
```

## Plugin Locations

After building, plugins are located at:
```
MyDAW/build/plugins/
├── nostalgia_tron/libnostalgia_tron.so
├── quantum_80/libquantum_80.so
├── rhythm_composer/librhythm_composer.so
├── velocity_eq/libvelocity_eq.so
├── momentum_delay/libmomentum_delay.so
├── analytica_vaccine/libanalytica_vaccine.so
├── fractal_remixer/libfractal_remixer.so
└── sonnet_composer/libsonnet_composer.so
```

## Building Individual Plugins

To build a single plugin:

```bash
cd MyDAW/plugins/nostalgia_tron
mkdir -p build
cd build
cmake ..
make
```

## Using Plugins

All plugins implement the `Node` interface:

```cpp
#include "plugins/nostalgia_tron/include/NostalgiaTron.h"

using namespace mydaw::plugins::nostalgia_tron;

// Create plugin instance
auto plugin = std::make_unique<NostalgiaTron>();

// Prepare for audio processing
plugin->prepare(44100.0, 512);

// Process audio
AudioBlock block = { inputBuffers, outputBuffers, numFrames, sampleRate };
plugin->process(block);
```

## Plugin API Examples

### Nostalgia-Tron
```cpp
plugin->setSampleSet(SampleSet::STRINGS);
plugin->setVolume(0.8f);
plugin->setTone(0.5f);
plugin->noteOn(60, 0.8f);  // Middle C
```

### Quantum-80
```cpp
plugin->setFMAlgorithm(FMAlgorithm::SIMPLE_STACK);
plugin->setModulationIndex(2.0f);
plugin->setFilterCutoff(1000.0f);
plugin->noteOn(60, 0.8f);
```

### Rhythm Composer
```cpp
plugin->loadSample(0, "kick.wav");
plugin->setTempo(120.0f);
plugin->setStep(0, 0, true, 1.0f);  // Kick on step 1
plugin->start();
```

### Velocity EQ
```cpp
EQBand band;
band.enabled = true;
band.type = BandType::PARAMETRIC;
band.frequency = 1000.0f;
band.gain = 3.0f;
band.q = 1.0f;
plugin->setBand(0, band);
```

### Momentum Delay
```cpp
plugin->setTempo(120.0f);
plugin->setNoteDivision(NoteDivision::EIGHTH);
plugin->setFeedback(0.5f);
plugin->setPingPong(true);
```

### Analytica Vaccine
```cpp
plugin->enableABMode(true);
plugin->enableAutoDeEsser(true);
auto problems = plugin->detectProblems();
float lufs = plugin->getLUFS();
```

### Fractal Remixer
```cpp
plugin->loadSample(0, "sample1.wav");
GrainParams params;
params.size = 0.1f;
params.density = 0.5f;
plugin->setGrainParams(params);
```

### Sonnet Composer
```cpp
std::vector<MIDINote> inputNotes = { /* 8 bars */ };
plugin->setInputMIDI(inputNotes);
plugin->setStyle(Style::ELECTRONIC);
plugin->generateComposition();
auto output = plugin->getOutputMIDI();
```

## Documentation

- **Main Overview**: `MyDAW/plugins/README.md`
- **Plugin Summary**: `PLUGIN_SUMMARY.md`
- **Implementation Report**: `IMPLEMENTATION_REPORT.md`
- **Individual Docs**: `MyDAW/plugins/*/docs/README.md`

## Troubleshooting

### Build Errors

If you encounter build errors:

1. Check CMake version: `cmake --version` (need 3.24+)
2. Check compiler: `g++ --version` (need C++20 support)
3. Clean build: `rm -rf build && mkdir build`

### Missing Headers

If you get "Node.h not found":
- Ensure you're building from the MyDAW directory
- Check that `engine/Node.h` exists

### Linking Errors

If plugins fail to link:
- Verify all source files are in the src/ directory
- Check CMakeLists.txt has correct file globs

## Next Steps

1. **Integration**: Load plugins dynamically in DAW
2. **UI**: Create parameter control interfaces
3. **Presets**: Implement save/load system
4. **Testing**: Add unit and integration tests
5. **Optimization**: Profile and optimize DSP code

## Support

For issues or questions:
- Check individual plugin documentation
- Review implementation report
- Consult main plugin README
