#include "../include/MomentumDelay.h"

namespace mydaw::plugins::momentum_delay {

MomentumDelay::MomentumDelay() = default;
MomentumDelay::~MomentumDelay() = default;

void MomentumDelay::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
    delayBufferL_.resize(static_cast<size_t>(sampleRate * 2.0), 0.0f);
    delayBufferR_.resize(static_cast<size_t>(sampleRate * 2.0), 0.0f);
}

void MomentumDelay::process(const AudioBlock& block) {
    for (int i = 0; i < block.frames; ++i) {
        block.out[0][i] = block.in[0][i];
        block.out[1][i] = block.in[1][i];
    }
}

void MomentumDelay::setTempo(float bpm) { tempo_ = bpm; }
void MomentumDelay::setNoteDivision(NoteDivision division) { division_ = division; }
void MomentumDelay::setRhythmicFeel(RhythmicFeel feel) { feel_ = feel; }
void MomentumDelay::setFeedback(float feedback) { feedback_ = feedback; }
void MomentumDelay::setPingPong(bool enable) { pingPong_ = enable; }
void MomentumDelay::setModulation(float depth, float rate) { (void)depth; (void)rate; }

} // namespace mydaw::plugins::momentum_delay
