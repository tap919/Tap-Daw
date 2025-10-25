#pragma once
#include "engine/Node.h"
#include <vector>
#include <array>
#include <string>

namespace mydaw::plugins::rhythm_composer {

// Pad configuration
struct PadConfig {
    std::vector<float> sample;
    float tuning = 0.0f;        // -12 to +12 semitones
    float decay = 1.0f;         // 0.0 to 1.0
    float filterCutoff = 1.0f;  // 0.0 to 1.0
    float pan = 0.5f;           // 0.0 (L) to 1.0 (R)
    float drive = 0.0f;         // 0.0 to 1.0
    bool active = false;
};

// Step data
struct Step {
    bool active = false;
    float velocity = 1.0f;
    float probability = 1.0f;   // 0.0 to 1.0
    bool flam = false;
};

// Pattern data
struct Pattern {
    std::array<std::array<Step, 16>, 16> steps; // 16 pads x 16 steps
    float swing = 0.0f;         // 0.0 to 1.0
    int length = 16;            // 1 to 16 steps
};

// Main Rhythm Composer class
class RhythmComposer : public Node {
public:
    RhythmComposer();
    ~RhythmComposer() override;

    // Node interface
    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    // Sample management
    void loadSample(int padIndex, const std::string& filepath);
    void clearPad(int padIndex);
    
    // Pad controls
    void setPadTuning(int padIndex, float semitones);
    void setPadDecay(int padIndex, float decay);
    void setPadFilter(int padIndex, float cutoff);
    void setPadPan(int padIndex, float pan);
    void setPadDrive(int padIndex, float drive);
    
    // Manual trigger
    void triggerPad(int padIndex, float velocity);
    
    // Pattern sequencer
    void setPattern(int patternIndex);
    void setStep(int padIndex, int stepIndex, bool active, float velocity);
    void setStepProbability(int padIndex, int stepIndex, float prob);
    void setStepFlam(int padIndex, int stepIndex, bool flam);
    void setSwing(float amount);
    void setTempo(float bpm);
    
    // Bass synth controls
    void setBassNote(int noteNumber);
    void setBassDecay(float decay);
    void setBassFilter(float cutoff);
    void setSidechainAmount(float amount);
    
    // Playback control
    void start();
    void stop();
    void reset();

private:
    static constexpr int NUM_PADS = 16;
    static constexpr int NUM_PATTERNS = 32;
    static constexpr int MAX_VOICES = 8;
    
    double sampleRate_ = 44100.0;
    
    // Pads
    std::array<PadConfig, NUM_PADS> pads_;
    
    // Patterns
    std::array<Pattern, NUM_PATTERNS> patterns_;
    int currentPattern_ = 0;
    
    // Sequencer state
    bool playing_ = false;
    float tempo_ = 120.0f;
    int currentStep_ = 0;
    double sampleCounter_ = 0.0;
    double samplesPerStep_ = 0.0;
    
    // Voice management
    struct Voice {
        int padIndex = -1;
        float position = 0.0f;
        float velocity = 1.0f;
        bool active = false;
    };
    std::array<Voice, MAX_VOICES> voices_;
    
    // Bass synth
    float bassPhase_ = 0.0f;
    float bassFrequency_ = 0.0f;
    float bassEnvelope_ = 0.0f;
    float bassDecay_ = 0.3f;
    float sidechainAmount_ = 0.5f;
    float sidechainEnv_ = 0.0f;
    
    // Processing methods
    void processSequencer();
    void processVoices(const AudioBlock& block);
    void processBass(const AudioBlock& block);
    int allocateVoice(int padIndex, float velocity);
    float processPad(int padIndex, float position, float velocity);
};

} // namespace mydaw::plugins::rhythm_composer
