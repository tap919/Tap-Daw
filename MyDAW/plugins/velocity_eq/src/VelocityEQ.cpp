#include "../include/VelocityEQ.h"

namespace mydaw::plugins::velocity_eq {

VelocityEQ::VelocityEQ() = default;
VelocityEQ::~VelocityEQ() = default;

void VelocityEQ::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
}

void VelocityEQ::process(const AudioBlock& block) {
    // Pass-through for now
    for (int ch = 0; ch < 2; ++ch) {
        for (int i = 0; i < block.frames; ++i) {
            block.out[ch][i] = block.in[ch][i];
        }
    }
}

void VelocityEQ::setBand(int index, const EQBand& band) {
    if (index >= 0 && index < NUM_BANDS) {
        bands_[index] = band;
    }
}

void VelocityEQ::enableFFT(bool enable) {
    fftEnabled_ = enable;
}

void VelocityEQ::loadPreset(const std::string& presetName) {
    // Preset loading placeholder
}

} // namespace mydaw::plugins::velocity_eq
