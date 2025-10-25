#pragma once
#include "engine/Node.h"
#include <vector>
#include <string>

namespace mydaw::plugins::sonnet_composer {

enum class Style {
    ELECTRONIC,
    CINEMATIC,
    AMBIENT,
    POP
};

struct MIDINote {
    int noteNumber;
    float velocity;
    float startTime;
    float duration;
};

class SonnetComposer : public Node {
public:
    SonnetComposer();
    ~SonnetComposer() override;

    void prepare(double sampleRate, int maxBlockSize) override;
    void process(const AudioBlock& block) override;
    int latencySamples() const override { return 0; }

    void setInputMIDI(const std::vector<MIDINote>& notes);
    void setStyle(Style style);
    void setTargetLength(float seconds);
    
    void generateComposition();
    std::vector<MIDINote> getOutputMIDI();
    void exportMIDI(const std::string& filepath);

private:
    double sampleRate_ = 44100.0;
    std::vector<MIDINote> inputNotes_;
    std::vector<MIDINote> outputNotes_;
    Style style_ = Style::ELECTRONIC;
    float targetLength_ = 120.0f;
    
    void analyzeInput();
    void generateSections();
    void createArrangement();
};

} // namespace mydaw::plugins::sonnet_composer
