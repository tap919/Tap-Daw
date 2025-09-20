#pragma once
#include <vector>
#include <cstdint>
#include "midi/scheduler.hpp"
#include "../pads/PadSampler.h"
namespace mydaw {
struct GenIdMapper {
  static std::pair<uint8_t,uint8_t> gen_to_pad(uint32_t gen){
    return { (uint8_t)((gen>>8)&0xFF), (uint8_t)(gen & 0xFF) };
  }
  static uint32_t pad_to_gen(uint8_t bank, uint8_t pad){ return (uint32_t)((bank<<8)|pad); }
};
class AudioEngineRT{
  mydaw::midi::TimeBase tb_;
  mydaw::midi::Scheduler sched_;
  mydaw::pads::PadSampler sampler_;
  std::vector<mydaw::midi::PatternInstance> active_;
  std::vector<midi::Ev> events_;
  int64_t samplePos_{0};
public:
  AudioEngineRT(double sr): tb_{sr,{120.0,4,4}}, sched_{tb_} {}
  void setTempo(double bpm){ tb_.set(tb_.sr(), {bpm,4,4}); }
  void setSampleRate(double sr){ tb_.set(sr, tb_.tempo()); }
  void setActivePatterns(std::vector<mydaw::midi::PatternInstance> v){ active_.swap(v); }
  void process(const float* in, float* out, int frames);
  mydaw::pads::PadSampler& sampler(){ return sampler_; }
  mydaw::midi::Scheduler& scheduler(){ return sched_; }
};
} // namespace
