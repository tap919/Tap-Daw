#pragma once
#include "engine/Node.h"
#include <vector>
#include <string>

namespace mydaw::plugins::fractal_remixer {

struct GrainParams {
    float size = 0.1f;      // seconds
    float density = 0.5f;   // grains per second
    float pitch = 1.0f;     // pitch multiplier
    float pan = 0.5f;       // 0.0 to 1.0
    float volume = 1.0f;    // 0.0 to 1.0
};

class FractalRemixer : public Node {
public:
    FractalRemixer();
    ~FractalRemixer() override;

    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    void loadSample(int slotIndex, const std::string& filepath);
    void clearSlot(int slotIndex);
    
    void setGrainParams(const GrainParams& params);
    void setVariationIntensity(float intensity);
    void setPatternLength(int bars);
    void setTempo(float bpm);

private:
    static constexpr int MAX_SAMPLES = 20;
    double sampleRate_ = 44100.0;
    
    struct Sample {
        std::vector<float> data;
        bool loaded = false;
    };
    
    std::vector<Sample> samples_;
    GrainParams grainParams_;
    float variationIntensity_ = 0.5f;
};

} // namespace mydaw::plugins::fractal_remixer
