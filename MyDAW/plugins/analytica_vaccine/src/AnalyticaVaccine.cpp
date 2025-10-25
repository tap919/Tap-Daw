#include "../include/AnalyticaVaccine.h"

namespace mydaw::plugins::analytica_vaccine {

AnalyticaVaccine::AnalyticaVaccine() = default;
AnalyticaVaccine::~AnalyticaVaccine() = default;

void AnalyticaVaccine::prepare(double sampleRate, int maxBlockSize) {
    sampleRate_ = sampleRate;
}

void AnalyticaVaccine::process(const AudioBlock& block) {
    for (int ch = 0; ch < 2; ++ch) {
        for (int i = 0; i < block.frames; ++i) {
            block.out[ch][i] = block.in[ch][i];
        }
    }
}

void AnalyticaVaccine::enableABMode(bool enable) { abMode_ = enable; }
void AnalyticaVaccine::selectA() { isA_ = true; }
void AnalyticaVaccine::selectB() { isA_ = false; }

ProblemReport AnalyticaVaccine::detectProblems() {
    return ProblemReport{};
}

void AnalyticaVaccine::enableAutoDeEsser(bool enable) { autoDeEsser_ = enable; }
void AnalyticaVaccine::enableRumbleFilter(bool enable) { rumbleFilter_ = enable; }
void AnalyticaVaccine::enableClippingPrevention(bool enable) { (void)enable; }
void AnalyticaVaccine::enablePhaseCorrection(bool enable) { (void)enable; }

float AnalyticaVaccine::getLUFS() { return -14.0f; }
float AnalyticaVaccine::getPhaseCorrelation() { return 1.0f; }
float AnalyticaVaccine::getPeakToRMS() { return 10.0f; }

} // namespace mydaw::plugins::analytica_vaccine
