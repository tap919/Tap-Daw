#include "../include/NostalgiaTron.h"
#include <cmath>
#include <algorithm>
#include <random>

namespace mydaw::plugins::nostalgia_tron {

NostalgiaTron::NostalgiaTron() {
    // Initialize voices
    for (auto& voice : voices_) {
        voice.active = false;
        voice.noteNumber = -1;
    }
}

NostalgiaTron::~NostalgiaTron() = default;

void NostalgiaTron::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
    maxBlockSize_ = maxBlockSize;
    updateEnvelopeRates();
    
    // Load default sample set
    loadSampleSet(currentSampleSet_);
}

void NostalgiaTron::process(const AudioBlock& block) {
    // Zero output buffers
    for (int ch = 0; ch < 2; ++ch) {
        for (int i = 0; i < block.frames; ++i) {
            block.out[ch][i] = 0.0f;
        }
    }
    
    // Process each active voice
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices_[v];
        if (!voice.active) continue;
        
        for (int i = 0; i < block.frames; ++i) {
            // Get tape pitch modulation
            float time = static_cast<float>(i) / static_cast<float>(sampleRate_);
            float pitchMod = getTapePitchModulation(time) + pitchBend_;
            
            // Calculate sample playback rate with modulation
            float rate = std::pow(2.0f, pitchMod / 12.0f);
            voice.phase += rate / sampleRate_;
            
            // Get sample value
            float sample = getSample(voice.noteNumber, voice.phase);
            
            // Apply ADSR envelope
            float env = processADSR(voice, true);
            sample *= env * voice.velocity * volume_;
            
            // Apply tone filter
            sample = applyToneFilter(sample, filterStates_[v]);
            
            // Add tape hiss
            sample += getTapeHiss() * 0.01f;
            
            // Output to both channels (stereo)
            block.out[0][i] += sample;
            block.out[1][i] += sample;
        }
        
        // Stop voice if phase exceeds sample duration
        if (voice.phase >= SAMPLE_DURATION_SECONDS * sampleRate_) {
            stopVoice(v);
        }
    }
    
    // Update tape effect phases
    wowPhase_ += tapeEffects_.wowRate * block.frames / sampleRate_;
    flutterPhase_ += tapeEffects_.flutterRate * block.frames / sampleRate_;
}

void NostalgiaTron::noteOn(int noteNumber, float velocity) {
    int voiceIndex = findFreeVoice();
    if (voiceIndex == -1) {
        voiceIndex = findOldestVoice();
    }
    
    Voice& voice = voices_[voiceIndex];
    voice.noteNumber = noteNumber;
    voice.velocity = velocity;
    voice.phase = 0.0f;
    voice.adsrEnv = 0.0f;
    voice.active = true;
    voice.startTime = static_cast<uint64_t>(wowPhase_ * 1000.0f); // Simple timestamp
}

void NostalgiaTron::noteOff(int noteNumber) {
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (voices_[i].active && voices_[i].noteNumber == noteNumber) {
            // Begin release phase (simplified - would need proper state machine)
            voices_[i].active = false;
        }
    }
}

void NostalgiaTron::pitchBend(float amount) {
    pitchBend_ = amount * 2.0f; // +/- 2 semitones
}

void NostalgiaTron::setVolume(float volume) {
    volume_ = std::clamp(volume, 0.0f, 1.0f);
}

void NostalgiaTron::setTone(float tone) {
    tone_ = std::clamp(tone, 0.0f, 1.0f);
}

void NostalgiaTron::setVibrato(float amount) {
    vibrato_ = std::clamp(amount, 0.0f, 1.0f);
}

void NostalgiaTron::setAttack(float timeMs) {
    attackTime_ = std::clamp(timeMs, 0.0f, 2000.0f);
    updateEnvelopeRates();
}

void NostalgiaTron::setRelease(float timeMs) {
    releaseTime_ = std::clamp(timeMs, 0.0f, 5000.0f);
    updateEnvelopeRates();
}

void NostalgiaTron::setSampleSet(SampleSet set) {
    currentSampleSet_ = set;
    loadSampleSet(set);
}

void NostalgiaTron::setTapeEffects(const TapeEffects& effects) {
    tapeEffects_ = effects;
}

int NostalgiaTron::findFreeVoice() {
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (!voices_[i].active) {
            return i;
        }
    }
    return -1;
}

int NostalgiaTron::findOldestVoice() {
    int oldest = 0;
    uint64_t oldestTime = voices_[0].startTime;
    
    for (int i = 1; i < MAX_VOICES; ++i) {
        if (voices_[i].startTime < oldestTime) {
            oldest = i;
            oldestTime = voices_[i].startTime;
        }
    }
    
    return oldest;
}

void NostalgiaTron::stopVoice(int voiceIndex) {
    voices_[voiceIndex].active = false;
    voices_[voiceIndex].noteNumber = -1;
}

void NostalgiaTron::loadSampleSet(SampleSet set) {
    // Placeholder: In real implementation, load samples from disk
    // For now, just reserve space
    sampleData_.clear();
    sampleData_.resize(128); // One sample per MIDI note
    
    for (auto& sample : sampleData_) {
        sample.resize(static_cast<size_t>(SAMPLE_DURATION_SECONDS * sampleRate_));
        // Fill with silence or placeholder sine wave
        for (size_t i = 0; i < sample.size(); ++i) {
            sample[i] = 0.0f;
        }
    }
}

float NostalgiaTron::getSample(int noteNumber, float phase) {
    if (noteNumber < 0 || noteNumber >= static_cast<int>(sampleData_.size())) {
        return 0.0f;
    }
    
    const auto& sample = sampleData_[noteNumber];
    if (sample.empty()) {
        return 0.0f;
    }
    
    // Simple linear interpolation
    float index = phase * sampleRate_;
    int idx0 = static_cast<int>(index);
    int idx1 = idx0 + 1;
    
    if (idx0 >= static_cast<int>(sample.size()) - 1) {
        return 0.0f;
    }
    
    float frac = index - static_cast<float>(idx0);
    return sample[idx0] * (1.0f - frac) + sample[idx1] * frac;
}

float NostalgiaTron::getTapePitchModulation(float time) {
    float wow = std::sin(wowPhase_ * 2.0f * M_PI) * tapeEffects_.wowAmount * 0.1f;
    float flutter = std::sin(flutterPhase_ * 2.0f * M_PI) * tapeEffects_.flutterAmount * 0.05f;
    return wow + flutter;
}

float NostalgiaTron::getTapeHiss() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    return dis(gen) * tapeEffects_.tapeHiss;
}

float NostalgiaTron::processADSR(Voice& voice, bool noteHeld) {
    if (noteHeld) {
        // Attack phase
        voice.adsrEnv += attackRate_;
        if (voice.adsrEnv > 1.0f) {
            voice.adsrEnv = 1.0f;
        }
    } else {
        // Release phase
        voice.adsrEnv -= releaseRate_;
        if (voice.adsrEnv < 0.0f) {
            voice.adsrEnv = 0.0f;
        }
    }
    
    return voice.adsrEnv;
}

void NostalgiaTron::updateEnvelopeRates() {
    // Calculate rates based on time in ms
    attackRate_ = attackTime_ > 0.0f ? 1.0f / (attackTime_ * 0.001f * sampleRate_) : 1.0f;
    releaseRate_ = releaseTime_ > 0.0f ? 1.0f / (releaseTime_ * 0.001f * sampleRate_) : 1.0f;
}

float NostalgiaTron::applyToneFilter(float input, FilterState& state) {
    // Simple one-pole low-pass filter
    float cutoff = tone_ * 0.5f + 0.1f; // Map 0-1 to reasonable cutoff range
    float a = 1.0f - cutoff;
    
    float output = input * cutoff + state.z1 * a;
    state.z1 = output;
    
    return output;
}

} // namespace mydaw::plugins::nostalgia_tron
