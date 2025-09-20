#pragma once
#include <cstdint>
namespace mydaw::midi {
using Tick = int64_t;
constexpr Tick kPPQ = 960;
struct TempoSig{ double bpm{120.0}; int num{4}; int den{4}; };
class TimeBase{
  double sr_{48000.0}; TempoSig t_{};
public:
  TimeBase()=default; TimeBase(double sr, TempoSig t):sr_(sr),t_(t){}
  void set(double sr, TempoSig t){ sr_=sr; t_=t; }
  double sr() const { return sr_; }
  TempoSig tempo() const { return t_; }
  inline Tick samples_to_ticks(int64_t samples) const {
    const double ticks = (double)samples * (t_.bpm/60.0) * (double)kPPQ / sr_;
    return (Tick)(ticks + 0.5);
  }
  inline int64_t tick_to_samples(Tick ticks) const {
    const double samples = (double)ticks * (60.0/t_.bpm) * (sr_/(double)kPPQ);
    return (int64_t)(samples + 0.5);
  }
};
} // namespace
