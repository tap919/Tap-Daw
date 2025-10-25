#include "../include/RhythmComposer.h"
#include <cmath>

namespace mydaw::plugins::rhythm_composer {

RhythmComposer::RhythmComposer() = default;
RhythmComposer::~RhythmComposer() = default;

void RhythmComposer::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
    samplesPerStep_ = (60.0 / tempo_) * sampleRate / 4.0; // 16th notes
}

void RhythmComposer::process(const AudioBlock& block) {
    // Clear output
    for (int ch = 0; ch < 2; ++ch) {
        for (int i = 0; i < block.frames; ++i) {
            block.out[ch][i] = 0.0f;
        }
    }
    
    if (playing_) {
        processSequencer();
    }
    
    processVoices(block);
    processBass(block);
}

void RhythmComposer::loadSample(int padIndex, const std::string& filepath) {
    // Placeholder for sample loading
}

void RhythmComposer::clearPad(int padIndex) {
    if (padIndex >= 0 && padIndex < NUM_PADS) {
        pads_[padIndex].sample.clear();
        pads_[padIndex].active = false;
    }
}

void RhythmComposer::triggerPad(int padIndex, float velocity) {
    allocateVoice(padIndex, velocity);
}

void RhythmComposer::start() { playing_ = true; }
void RhythmComposer::stop() { playing_ = false; }
void RhythmComposer::reset() { currentStep_ = 0; sampleCounter_ = 0.0; }

void RhythmComposer::processSequencer() {
    sampleCounter_ += 1.0;
    if (sampleCounter_ >= samplesPerStep_) {
        sampleCounter_ -= samplesPerStep_;
        currentStep_ = (currentStep_ + 1) % 16;
        
        // Trigger active steps
        for (int pad = 0; pad < NUM_PADS; ++pad) {
            const auto& step = patterns_[currentPattern_].steps[pad][currentStep_];
            if (step.active) {
                // Check probability
                if (static_cast<float>(rand()) / RAND_MAX < step.probability) {
                    triggerPad(pad, step.velocity);
                }
            }
        }
    }
}

void RhythmComposer::processVoices(const AudioBlock& block) {
    for (auto& voice : voices_) {
        if (!voice.active) continue;
        
        for (int i = 0; i < block.frames; ++i) {
            float sample = processPad(voice.padIndex, voice.position, voice.velocity);
            
            const auto& pad = pads_[voice.padIndex];
            float panL = 1.0f - pad.pan;
            float panR = pad.pan;
            
            block.out[0][i] += sample * panL;
            block.out[1][i] += sample * panR;
            
            voice.position += 1.0f;
            
            if (voice.position >= pads_[voice.padIndex].sample.size()) {
                voice.active = false;
                break;
            }
        }
    }
}

void RhythmComposer::processBass(const AudioBlock& block) {
    // Simple bass synthesis placeholder
}

int RhythmComposer::allocateVoice(int padIndex, float velocity) {
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (!voices_[i].active) {
            voices_[i].padIndex = padIndex;
            voices_[i].position = 0.0f;
            voices_[i].velocity = velocity;
            voices_[i].active = true;
            return i;
        }
    }
    return -1;
}

float RhythmComposer::processPad(int padIndex, float position, float velocity) {
    if (padIndex < 0 || padIndex >= NUM_PADS) return 0.0f;
    
    const auto& pad = pads_[padIndex];
    if (pad.sample.empty()) return 0.0f;
    
    int idx = static_cast<int>(position);
    if (idx >= static_cast<int>(pad.sample.size())) return 0.0f;
    
    return pad.sample[idx] * velocity;
}

void RhythmComposer::setPadTuning(int padIndex, float semitones) {}
void RhythmComposer::setPadDecay(int padIndex, float decay) {}
void RhythmComposer::setPadFilter(int padIndex, float cutoff) {}
void RhythmComposer::setPadPan(int padIndex, float pan) {
    if (padIndex >= 0 && padIndex < NUM_PADS) {
        pads_[padIndex].pan = pan;
    }
}
void RhythmComposer::setPadDrive(int padIndex, float drive) {}
void RhythmComposer::setPattern(int patternIndex) { currentPattern_ = patternIndex; }
void RhythmComposer::setStep(int padIndex, int stepIndex, bool active, float velocity) {}
void RhythmComposer::setStepProbability(int padIndex, int stepIndex, float prob) {}
void RhythmComposer::setStepFlam(int padIndex, int stepIndex, bool flam) {}
void RhythmComposer::setSwing(float amount) {}
void RhythmComposer::setTempo(float bpm) { tempo_ = bpm; }
void RhythmComposer::setBassNote(int noteNumber) {}
void RhythmComposer::setBassDecay(float decay) {}
void RhythmComposer::setBassFilter(float cutoff) {}
void RhythmComposer::setSidechainAmount(float amount) {}

} // namespace mydaw::plugins::rhythm_composer
