#pragma once
#include <cstdint>
#include "../engine/Node.h"
namespace mydaw::pads {
struct PadHit{ uint8_t bank, pad, vel; };
class PadSampler : public Node{
public:
  void prepare(double sr,int block) override {(void)sr;(void)block;}
  void process(const AudioBlock&) override {}
  int latencySamples() const override { return 0; }
  void note_on(PadHit){ }
  void note_off(PadHit){ }
  void pitch_bend(uint8_t,uint8_t,int){ }
};
} // namespace
