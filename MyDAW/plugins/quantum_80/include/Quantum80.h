#pragma once
#include "engine/Node.h"
#include <vector>
#include <array>

namespace mydaw::plugins::quantum_80 {

// FM Algorithm types
enum class FMAlgorithm {
    SIMPLE_STACK = 0,    // Carrier <- Modulator
    PARALLEL = 1,        // Both direct to output
    FEEDBACK = 2,        // Modulator with feedback
    RING_MOD = 3,        // Ring modulation style
    DOUBLE_MOD = 4,      // Both modulate each other
    SPLIT = 5            // Modulator splits to carrier
};

// Oscillator waveform types
enum class OscWaveform {
    SAW,
    SQUARE,
    TRIANGLE
};

// FM Operator state
struct FMOperator {
    float phase = 0.0f;
    float frequency = 440.0f;
    float amplitude = 1.0f;
    float feedback = 0.0f;
    float lastOutput = 0.0f;
};

// Analog VCO state
struct AnalogOscillator {
    float phase = 0.0f;
    float frequency = 440.0f;
    float amplitude = 1.0f;
    OscWaveform waveform = OscWaveform::SAW;
    float pwm = 0.5f;  // Pulse width for square wave
};

// Filter state
struct FilterState {
    float stage1 = 0.0f;
    float stage2 = 0.0f;
    float stage3 = 0.0f;
    float stage4 = 0.0f;
};

// Main Quantum-80 plugin class
class Quantum80 : public Node {
public:
    Quantum80();
    ~Quantum80() override;

    // Node interface
    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    // MIDI interface
    void noteOn(int noteNumber, float velocity);
    void noteOff(int noteNumber);
    void pitchBend(float amount);
    void modWheel(float amount);

    // Digital section controls
    void setFMAlgorithm(FMAlgorithm algo);
    void setModulationIndex(float index);    // 0.0 to 10.0
    void setOperatorRatio(float ratio);      // 0.5 to 8.0
    void setFeedback(float amount);          // 0.0 to 1.0

    // Analog section controls
    void setOsc1Waveform(OscWaveform wave);
    void setOsc2Waveform(OscWaveform wave);
    void setOsc1Level(float level);          // 0.0 to 1.0
    void setOsc2Level(float level);          // 0.0 to 1.0
    void setOsc2Detune(float cents);         // -100 to +100

    // Filter controls
    void setFilterCutoff(float cutoff);      // 20Hz to 20kHz
    void setFilterResonance(float resonance); // 0.0 to 1.0
    void setFilterType(int type);            // 0=LP, 1=HP, 2=BP

    // Effects
    void setChorusDepth(float depth);        // 0.0 to 1.0
    void setDelayTime(float timeMs);         // 0 to 1000ms
    void setDelayFeedback(float feedback);   // 0.0 to 0.9

private:
    double sampleRate_ = 44100.0;
    int maxBlockSize_ = 512;
    
    // FM Engine
    FMAlgorithm fmAlgorithm_ = FMAlgorithm::SIMPLE_STACK;
    FMOperator carrier_;
    FMOperator modulator_;
    float modulationIndex_ = 1.0f;
    float operatorRatio_ = 1.0f;
    
    // Analog Oscillators
    AnalogOscillator osc1_;
    AnalogOscillator osc2_;
    float osc1Level_ = 0.5f;
    float osc2Level_ = 0.5f;
    float osc2Detune_ = 0.0f;
    
    // Filter
    FilterState filterState_;
    float filterCutoff_ = 1000.0f;
    float filterResonance_ = 0.0f;
    int filterType_ = 0;
    
    // Effects
    float chorusDepth_ = 0.0f;
    float delayTime_ = 250.0f;
    float delayFeedback_ = 0.3f;
    std::vector<float> delayBuffer_;
    int delayWritePos_ = 0;
    
    // Envelope
    float envelope_ = 0.0f;
    float attack_ = 10.0f;
    float decay_ = 100.0f;
    float sustain_ = 0.7f;
    float release_ = 300.0f;
    bool noteActive_ = false;
    
    // Processing methods
    float processFM(float deltaPhase);
    float processAnalog(float deltaPhase);
    float processFilter(float input);
    float processEffects(float input);
    void updateEnvelope();
    
    // Waveform generators
    float generateSaw(float phase);
    float generateSquare(float phase, float pwm);
    float generateTriangle(float phase);
};

} // namespace mydaw::plugins::quantum_80
