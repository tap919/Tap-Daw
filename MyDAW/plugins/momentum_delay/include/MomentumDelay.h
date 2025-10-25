#pragma once
#include "engine/Node.h"
#include <vector>

namespace mydaw::plugins::momentum_delay {

enum class NoteDivision {
    QUARTER,
    QUARTER_TRIPLET,
    EIGHTH,
    EIGHTH_TRIPLET,
    SIXTEENTH,
    SIXTEENTH_TRIPLET
};

enum class RhythmicFeel {
    STRAIGHT,
    SWING,
    SHUFFLE,
    DOTTED
};

class MomentumDelay : public Node {
public:
    MomentumDelay();
    ~MomentumDelay() override;

    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    void setTempo(float bpm);
    void setNoteDivision(NoteDivision division);
    void setRhythmicFeel(RhythmicFeel feel);
    void setFeedback(float feedback);
    void setPingPong(bool enable);
    void setModulation(float depth, float rate);

private:
    double sampleRate_ = 44100.0;
    float tempo_ = 120.0f;
    NoteDivision division_ = NoteDivision::QUARTER;
    RhythmicFeel feel_ = RhythmicFeel::STRAIGHT;
    float feedback_ = 0.3f;
    bool pingPong_ = false;
    
    std::vector<float> delayBufferL_;
    std::vector<float> delayBufferR_;
    int writePos_ = 0;
};

} // namespace mydaw::plugins::momentum_delay
