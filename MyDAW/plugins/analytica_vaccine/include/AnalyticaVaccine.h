#pragma once
#include "engine/Node.h"
#include <string>

namespace mydaw::plugins::analytica_vaccine {

struct ProblemReport {
    bool clipping = false;
    bool phaseIssues = false;
    bool mud = false;
    bool harshness = false;
};

class AnalyticaVaccine : public Node {
public:
    AnalyticaVaccine();
    ~AnalyticaVaccine() override;

    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    void enableABMode(bool enable);
    void selectA();
    void selectB();
    
    ProblemReport detectProblems();
    void enableAutoDeEsser(bool enable);
    void enableRumbleFilter(bool enable);
    void enableClippingPrevention(bool enable);
    void enablePhaseCorrection(bool enable);
    
    float getLUFS();
    float getPhaseCorrelation();
    float getPeakToRMS();

private:
    double sampleRate_ = 44100.0;
    bool abMode_ = false;
    bool isA_ = true;
    bool autoDeEsser_ = false;
    bool rumbleFilter_ = false;
};

} // namespace mydaw::plugins::analytica_vaccine
