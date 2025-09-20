#pragma once
#include "midi/pattern.hpp"
#include "midi/timing.hpp"
#include <vector>
#include <random>
namespace mydaw::midi {
enum class EvType{ NoteOn, NoteOff, PitchBend };
struct Ev{ GenId gen; EvType type; uint32_t sampOff; uint8_t pitch; uint8_t vel; int pb{0}; };
struct PatternInstance{
  const Pattern* pattern{nullptr}; Tick startTick{0}; Tick loopLengthTicks{0};
  bool enabled{true}; bool muted{false}; float gain{1.0f}; int transpose{0};
};
class Scheduler{
  TimeBase tb_;
public:
  explicit Scheduler(TimeBase tb):tb_(tb){}
  void set_timebase(TimeBase tb){ tb_=tb; }
  const TimeBase& get_timebase() const { return tb_; }
  void gather(const Pattern& pat, Tick clipStartTick, Tick loopLenTick, int64_t blockSample, uint32_t frames, std::vector<Ev>& out) const;
  void gather_realtime(const std::vector<PatternInstance>& instances, int64_t blockSample, uint32_t frames, std::vector<Ev>& out) const;
  void apply_swing(std::vector<Ev>& events, float swingPercent, Tick swingResolution) const;
  void apply_humanize(std::vector<Ev>& events, float timingVariation, float velocityVariation) const;
  void quantize_events(std::vector<Ev>& events, Tick quantizeGrid) const;
  Tick get_next_event_tick(const std::vector<PatternInstance>& instances, Tick currentTick) const;
  bool has_events_in_range(const std::vector<PatternInstance>& instances, Tick startTick, Tick endTick) const;
};
} // namespace
