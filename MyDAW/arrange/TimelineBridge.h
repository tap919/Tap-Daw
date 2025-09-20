#pragma once
#include <vector>
#include <unordered_map>
#include "arrange/Playlist.h"
#include "midi/pattern.hpp"
#include "midi/scheduler.hpp"
namespace mydaw::arrange {
struct PatternRegistry{ std::unordered_map<int,const mydaw::midi::Pattern*> byId; };
inline std::vector<mydaw::midi::PatternInstance>
BuildPatternInstances(const Playlist& pl, const PatternRegistry& reg, mydaw::midi::Tick /*playheadTick*/){
  std::vector<mydaw::midi::PatternInstance> out;
  for (const auto& c: pl.clips){
    auto it = reg.byId.find(c.id);
    if (it==reg.byId.end()) continue;
    mydaw::midi::PatternInstance pi;
    pi.pattern = it->second;
    pi.startTick = (mydaw::midi::Tick)c.start;
    pi.loopLengthTicks = (mydaw::midi::Tick)c.len;
    out.push_back(pi);
  }
  return out;
}
} // namespace
