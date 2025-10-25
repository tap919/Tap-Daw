#include "../include/Quantum80.h"
#include <cmath>
#include <algorithm>

namespace mydaw::plugins::quantum_80 {

Quantum80::Quantum80() {
    carrier_.frequency = 440.0f;
    modulator_.frequency = 440.0f;
    osc1_.frequency = 440.0f;
    osc2_.frequency = 440.0f;
}

Quantum80::~Quantum80() = default;

void Quantum80::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
    maxBlockSize_ = maxBlockSize;
    
    // Allocate delay buffer (1 second max)
    delayBuffer_.resize(static_cast<size_t>(sampleRate), 0.0f);
    delayWritePos_ = 0;
}

void Quantum80::process(const AudioBlock& block) {
    float deltaPhase = 1.0f / static_cast<float>(sampleRate_);
    
    for (int i = 0; i < block.frames; ++i) {
        updateEnvelope();
        
        // Process FM section
        float fmOutput = processFM(deltaPhase);
        
        // Process analog section
        float analogOutput = processAnalog(deltaPhase);
        
        // Mix sections
        float mixed = (fmOutput + analogOutput) * 0.5f * envelope_;
        
        // Apply filter
        mixed = processFilter(mixed);
        
        // Apply effects
        mixed = processEffects(mixed);
        
        // Output to both channels
        block.out[0][i] = mixed;
        block.out[1][i] = mixed;
    }
}

void Quantum80::noteOn(int noteNumber, float velocity) {
    float frequency = 440.0f * std::pow(2.0f, (noteNumber - 69) / 12.0f);
    
    carrier_.frequency = frequency;
    modulator_.frequency = frequency * operatorRatio_;
    osc1_.frequency = frequency;
    osc2_.frequency = frequency * std::pow(2.0f, osc2Detune_ / 1200.0f);
    
    noteActive_ = true;
    envelope_ = 0.0f;
}

void Quantum80::noteOff(int noteNumber) {
    noteActive_ = false;
}

void Quantum80::pitchBend(float amount) {
    // Pitch bend implementation
}

void Quantum80::modWheel(float amount) {
    // Modulation wheel affects FM depth
    modulationIndex_ = amount * 10.0f;
}

void Quantum80::setFMAlgorithm(FMAlgorithm algo) {
    fmAlgorithm_ = algo;
}

void Quantum80::setModulationIndex(float index) {
    modulationIndex_ = std::clamp(index, 0.0f, 10.0f);
}

void Quantum80::setOperatorRatio(float ratio) {
    operatorRatio_ = std::clamp(ratio, 0.5f, 8.0f);
}

void Quantum80::setFeedback(float amount) {
    carrier_.feedback = std::clamp(amount, 0.0f, 1.0f);
}

void Quantum80::setOsc1Waveform(OscWaveform wave) {
    osc1_.waveform = wave;
}

void Quantum80::setOsc2Waveform(OscWaveform wave) {
    osc2_.waveform = wave;
}

void Quantum80::setOsc1Level(float level) {
    osc1Level_ = std::clamp(level, 0.0f, 1.0f);
}

void Quantum80::setOsc2Level(float level) {
    osc2Level_ = std::clamp(level, 0.0f, 1.0f);
}

void Quantum80::setOsc2Detune(float cents) {
    osc2Detune_ = std::clamp(cents, -100.0f, 100.0f);
}

void Quantum80::setFilterCutoff(float cutoff) {
    filterCutoff_ = std::clamp(cutoff, 20.0f, 20000.0f);
}

void Quantum80::setFilterResonance(float resonance) {
    filterResonance_ = std::clamp(resonance, 0.0f, 1.0f);
}

void Quantum80::setFilterType(int type) {
    filterType_ = std::clamp(type, 0, 2);
}

void Quantum80::setChorusDepth(float depth) {
    chorusDepth_ = std::clamp(depth, 0.0f, 1.0f);
}

void Quantum80::setDelayTime(float timeMs) {
    delayTime_ = std::clamp(timeMs, 0.0f, 1000.0f);
}

void Quantum80::setDelayFeedback(float feedback) {
    delayFeedback_ = std::clamp(feedback, 0.0f, 0.9f);
}

float Quantum80::processFM(float deltaPhase) {
    // Phase modulation synthesis
    float modulatorOutput = std::sin(modulator_.phase * 2.0f * M_PI);
    modulatorOutput *= modulationIndex_;
    
    float carrierPhase = carrier_.phase + modulatorOutput * 0.1f;
    float carrierOutput = std::sin(carrierPhase * 2.0f * M_PI);
    
    carrier_.phase += carrier_.frequency * deltaPhase;
    modulator_.phase += modulator_.frequency * deltaPhase;
    
    // Wrap phases
    while (carrier_.phase > 1.0f) carrier_.phase -= 1.0f;
    while (modulator_.phase > 1.0f) modulator_.phase -= 1.0f;
    
    return carrierOutput;
}

float Quantum80::processAnalog(float deltaPhase) {
    float output1 = 0.0f;
    float output2 = 0.0f;
    
    // Oscillator 1
    switch (osc1_.waveform) {
        case OscWaveform::SAW:
            output1 = generateSaw(osc1_.phase);
            break;
        case OscWaveform::SQUARE:
            output1 = generateSquare(osc1_.phase, osc1_.pwm);
            break;
        case OscWaveform::TRIANGLE:
            output1 = generateTriangle(osc1_.phase);
            break;
    }
    
    // Oscillator 2
    switch (osc2_.waveform) {
        case OscWaveform::SAW:
            output2 = generateSaw(osc2_.phase);
            break;
        case OscWaveform::SQUARE:
            output2 = generateSquare(osc2_.phase, osc2_.pwm);
            break;
        case OscWaveform::TRIANGLE:
            output2 = generateTriangle(osc2_.phase);
            break;
    }
    
    osc1_.phase += osc1_.frequency * deltaPhase;
    osc2_.phase += osc2_.frequency * deltaPhase;
    
    while (osc1_.phase > 1.0f) osc1_.phase -= 1.0f;
    while (osc2_.phase > 1.0f) osc2_.phase -= 1.0f;
    
    return output1 * osc1Level_ + output2 * osc2Level_;
}

float Quantum80::processFilter(float input) {
    // Simple 4-pole ladder filter emulation
    float cutoffNorm = filterCutoff_ / static_cast<float>(sampleRate_);
    float resonanceAmount = filterResonance_ * 4.0f;
    
    filterState_.stage1 += cutoffNorm * (input - filterState_.stage1 - resonanceAmount * filterState_.stage4);
    filterState_.stage2 += cutoffNorm * (filterState_.stage1 - filterState_.stage2);
    filterState_.stage3 += cutoffNorm * (filterState_.stage2 - filterState_.stage3);
    filterState_.stage4 += cutoffNorm * (filterState_.stage3 - filterState_.stage4);
    
    return filterState_.stage4;
}

float Quantum80::processEffects(float input) {
    // Simple delay
    int readPos = delayWritePos_ - static_cast<int>(delayTime_ * sampleRate_ / 1000.0f);
    if (readPos < 0) readPos += delayBuffer_.size();
    
    float delayed = delayBuffer_[readPos];
    delayBuffer_[delayWritePos_] = input + delayed * delayFeedback_;
    
    delayWritePos_ = (delayWritePos_ + 1) % delayBuffer_.size();
    
    return input + delayed * 0.3f;
}

void Quantum80::updateEnvelope() {
    // Simple ADSR envelope
    if (noteActive_) {
        envelope_ += 1.0f / (attack_ * sampleRate_ / 1000.0f);
        if (envelope_ > 1.0f) envelope_ = 1.0f;
    } else {
        envelope_ -= 1.0f / (release_ * sampleRate_ / 1000.0f);
        if (envelope_ < 0.0f) envelope_ = 0.0f;
    }
}

float Quantum80::generateSaw(float phase) {
    return 2.0f * phase - 1.0f;
}

float Quantum80::generateSquare(float phase, float pwm) {
    return phase < pwm ? 1.0f : -1.0f;
}

float Quantum80::generateTriangle(float phase) {
    return phase < 0.5f ? 4.0f * phase - 1.0f : 3.0f - 4.0f * phase;
}

} // namespace mydaw::plugins::quantum_80
