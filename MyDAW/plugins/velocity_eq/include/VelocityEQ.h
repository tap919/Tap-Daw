#pragma once
#include "engine/Node.h"
#include <array>
#include <vector>
#include <string>

namespace mydaw::plugins::velocity_eq {

enum class BandType {
    PARAMETRIC,
    LOW_SHELF,
    HIGH_SHELF,
    NOTCH,
    HIGH_PASS,
    LOW_PASS
};

struct EQBand {
    bool enabled = false;
    BandType type = BandType::PARAMETRIC;
    float frequency = 1000.0f;
    float gain = 0.0f;
    float q = 1.0f;
    bool dynamic = false;
    float threshold = -20.0f;
};

class VelocityEQ : public Node {
public:
    VelocityEQ();
    ~VelocityEQ() override;

    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    void setBand(int index, const EQBand& band);
    void enableFFT(bool enable);
    void loadPreset(const std::string& presetName);

private:
    static constexpr int NUM_BANDS = 8;
    double sampleRate_ = 44100.0;
    std::array<EQBand, NUM_BANDS> bands_;
    bool fftEnabled_ = true;
};

} // namespace mydaw::plugins::velocity_eq
