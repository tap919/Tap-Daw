#pragma once
#include "engine/Node.h"
#include <vector>
#include <array>
#include <memory>

namespace mydaw::plugins::nostalgia_tron {

// Sample set enumeration
enum class SampleSet {
    STRINGS = 0,
    CHOIR = 1,
    FLUTE = 2,
    CELLOS = 3
};

// Tape effect parameters
struct TapeEffects {
    float wowAmount = 0.0f;        // 0.0 to 1.0
    float wowRate = 0.5f;          // Hz
    float flutterAmount = 0.0f;    // 0.0 to 1.0
    float flutterRate = 6.0f;      // Hz
    float tapeHiss = 0.2f;         // 0.0 to 1.0
};

// Voice state for polyphony management
struct Voice {
    int noteNumber = -1;
    float phase = 0.0f;
    float velocity = 0.0f;
    float adsrEnv = 0.0f;
    bool active = false;
    uint64_t startTime = 0;        // For oldest-note stealing
};

// Main Nostalgia-Tron plugin class
class NostalgiaTron : public Node {
public:
    NostalgiaTron();
    ~NostalgiaTron() override;

    // Node interface implementation
    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    // MIDI interface
    void noteOn(int noteNumber, float velocity);
    void noteOff(int noteNumber);
    void pitchBend(float amount); // -1.0 to 1.0

    // Parameter controls
    void setVolume(float volume);           // 0.0 to 1.0
    void setTone(float tone);               // 0.0 to 1.0 (low-pass filter)
    void setVibrato(float amount);          // 0.0 to 1.0
    void setAttack(float timeMs);           // 0 to 2000ms
    void setRelease(float timeMs);          // 0 to 5000ms
    void setSampleSet(SampleSet set);
    void setTapeEffects(const TapeEffects& effects);

private:
    static constexpr int MAX_VOICES = 3;
    static constexpr int SAMPLE_DURATION_SECONDS = 8;
    
    double sampleRate_ = 44100.0;
    int maxBlockSize_ = 512;
    
    // Voice management
    std::array<Voice, MAX_VOICES> voices_;
    int findFreeVoice();
    int findOldestVoice();
    void stopVoice(int voiceIndex);
    
    // Sample playback
    SampleSet currentSampleSet_ = SampleSet::STRINGS;
    std::vector<std::vector<float>> sampleData_; // Pre-loaded samples
    void loadSampleSet(SampleSet set);
    float getSample(int noteNumber, float phase);
    
    // Controls
    float volume_ = 0.8f;
    float tone_ = 0.7f;
    float vibrato_ = 0.0f;
    float attackTime_ = 10.0f;  // ms
    float releaseTime_ = 500.0f; // ms
    float pitchBend_ = 0.0f;
    
    // Tape emulation
    TapeEffects tapeEffects_;
    float wowPhase_ = 0.0f;
    float flutterPhase_ = 0.0f;
    float getTapePitchModulation(float time);
    float getTapeHiss();
    
    // ADSR envelope
    float processADSR(Voice& voice, bool noteHeld);
    float attackRate_ = 0.0f;
    float releaseRate_ = 0.0f;
    void updateEnvelopeRates();
    
    // Low-pass filter for tone control
    struct FilterState {
        float z1 = 0.0f;
        float z2 = 0.0f;
    };
    std::array<FilterState, MAX_VOICES> filterStates_;
    float applyToneFilter(float input, FilterState& state);
};

} // namespace mydaw::plugins::nostalgia_tron
