#pragma once
#include <vector>
#include <cstdint>
#include "midi/timing.hpp"
namespace mydaw::midi {
using GenId = uint32_t;
struct Note{ Tick start{0}; Tick len{0}; uint8_t pitch{60}; uint8_t vel{100}; uint8_t rel{64}; bool slide{false}; int fine{0}; };
struct ChannelPattern{ GenId gen{0}; std::vector<Note> notes; };
struct Pattern{ Tick length{kPPQ*4}; std::vector<ChannelPattern> channels; };
} // namespace
