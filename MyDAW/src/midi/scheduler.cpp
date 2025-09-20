#include "midi/scheduler.hpp"
#include <algorithm>
#include <cmath>
namespace mydaw::midi {
void Scheduler::gather(const Pattern& pat, Tick clipStartTick, Tick loopLenTick, int64_t blockSample, uint32_t frames, std::vector<Ev>& out) const{
  const Tick blkBeg = tb_.samples_to_ticks(blockSample);
  const Tick blkEnd = tb_.samples_to_ticks(blockSample + frames);
  for (const auto& ch : pat.channels){
    if (ch.gen==0) continue;
    for (const auto& note : ch.notes){
      const Tick patternLength = pat.length; if (patternLength<=0) continue;
      const Tick searchStart = std::max<Tick>(0, blkBeg - patternLength);
      const Tick searchEnd = blkEnd + patternLength;
      for (Tick loopOffset = 0; loopOffset <= searchEnd; loopOffset += patternLength){
        const Tick absoluteStart = clipStartTick + loopOffset + note.start;
        const Tick absoluteEnd = absoluteStart + note.len;
        if (absoluteStart >= searchEnd || absoluteEnd <= searchStart) continue;
        if (loopLenTick > 0 && loopOffset >= loopLenTick) break;
        if (absoluteStart >= blkBeg && absoluteStart < blkEnd){
          const int64_t sp = tb_.tick_to_samples(absoluteStart);
          const uint32_t off = (uint32_t)std::clamp<int64_t>(sp - blockSample, 0LL, (int64_t)frames - 1);
          Ev e{ch.gen, EvType::NoteOn, off, note.pitch, note.vel, 0}; out.push_back(e);
        }
        if (absoluteEnd >= blkBeg && absoluteEnd < blkEnd){
          const int64_t sp = tb_.tick_to_samples(absoluteEnd);
          const uint32_t off = (uint32_t)std::clamp<int64_t>(sp - blockSample, 0LL, (int64_t)frames - 1);
          Ev e{ch.gen, EvType::NoteOff, off, note.pitch, note.rel, 0}; out.push_back(e);
        }
        if (note.slide && note.fine!=0){
          const Tick bendStart = std::max<Tick>(absoluteStart, blkBeg);
          const Tick bendEnd = std::min<Tick>(absoluteEnd, blkEnd);
          for (Tick t=bendStart; t<bendEnd; t += tb_.samples_to_ticks(64)){
            const int64_t sp = tb_.tick_to_samples(t);
            const uint32_t off = (uint32_t)std::clamp<int64_t>(sp - blockSample, 0LL, (int64_t)frames - 1);
            const float progress = (float)(t - absoluteStart) / (float)note.len;
            const int bend = (int)(note.fine * progress * 64);
            Ev e{ch.gen, EvType::PitchBend, off, note.pitch, 0, std::clamp(bend, -8192, 8191)}; out.push_back(e);
          }
        }
      }
    }
  }
  std::sort(out.begin(), out.end(), [](const Ev& a, const Ev& b){
    if (a.sampOff != b.sampOff) return a.sampOff < b.sampOff;
    if (a.type != b.type) return a.type == EvType::NoteOff;
    return false;
  });
}
void Scheduler::gather_realtime(const std::vector<PatternInstance>& instances, int64_t blockSample, uint32_t frames, std::vector<Ev>& out) const{
  out.clear();
  for (const auto& inst : instances){
    if (!inst.pattern || !inst.enabled) continue;
    const Tick loopLen = inst.loopLengthTicks > 0 ? inst.loopLengthTicks : inst.pattern->length;
    gather(*inst.pattern, inst.startTick, loopLen, blockSample, frames, out);
  }
  auto last = std::unique(out.begin(), out.end(), [](const Ev& a, const Ev& b){
    return a.gen==b.gen && a.type==b.type && a.sampOff==b.sampOff && a.pitch==b.pitch && a.vel==b.vel;
  });
  out.erase(last, out.end());
}
void Scheduler::apply_swing(std::vector<Ev>& events, float swingPercent, Tick swingResolution) const{
  if (swingPercent <= 50.0f || swingResolution <= 0) return;
  const float swingRatio = swingPercent / 100.0f;
  const float offset = (swingRatio - 0.5f) * 2.0f;
  for (auto& ev : events){
    if (ev.type != EvType::NoteOn) continue;
    const int64_t samplePos = tb_.tick_to_samples(0) + ev.sampOff;
    const Tick eventTick = tb_.samples_to_ticks(samplePos);
    const Tick beatTick = eventTick % swingResolution;
    const Tick halfRes = swingResolution / 2;
    if (beatTick == halfRes){
      const Tick swingAmount = (Tick)(halfRes * offset * 0.25f);
      const int64_t newSample = tb_.tick_to_samples(eventTick + swingAmount);
      const int64_t base = tb_.tick_to_samples(0);
      ev.sampOff = (uint32_t)std::max<int64_t>(0LL, newSample - base);
    }
  }
  std::sort(events.begin(), events.end(), [](const Ev& a, const Ev& b){ return a.sampOff < b.sampOff; });
}
void Scheduler::apply_humanize(std::vector<Ev>& events, float timingVariation, float velocityVariation) const{
  static thread_local std::mt19937 rng(std::random_device{}());
  const float maxTiming = timingVariation * (float)tb_.tick_to_samples(kPPQ/16);
  const float maxVelVar = velocityVariation * 127.0f;
  std::uniform_real_distribution<float> td(-maxTiming, maxTiming);
  std::uniform_real_distribution<float> vd(-maxVelVar, maxVelVar);
  for (auto& ev : events){
    if (ev.type == EvType::NoteOn){
      const float dt = td(rng);
      int32_t newOff = (int32_t)((float)ev.sampOff + dt);
      ev.sampOff = (uint32_t)std::max(0, newOff);
      const float dv = vd(rng);
      int nv = (int)ev.vel + (int)dv;
      ev.vel = (uint8_t)std::clamp(nv, 1, 127);
    }
  }
  std::sort(events.begin(), events.end(), [](const Ev& a, const Ev& b){ return a.sampOff < b.sampOff; });
}
void Scheduler::quantize_events(std::vector<Ev>& events, Tick quantizeGrid) const{
  if (quantizeGrid <= 0) return;
  for (auto& ev : events){
    const int64_t samplePos = tb_.tick_to_samples(0) + ev.sampOff;
    const Tick eventTick = tb_.samples_to_ticks(samplePos);
    const Tick q = ((eventTick + quantizeGrid/2) / quantizeGrid) * quantizeGrid;
    const int64_t qSamples = tb_.tick_to_samples(q);
    const int64_t base = tb_.tick_to_samples(0);
    ev.sampOff = (uint32_t)std::max<int64_t>(0LL, qSamples - base);
  }
  std::sort(events.begin(), events.end(), [](const Ev& a, const Ev& b){ return a.sampOff < b.sampOff; });
}
Tick Scheduler::get_next_event_tick(const std::vector<PatternInstance>& instances, Tick currentTick) const{
  Tick nextTick = currentTick + kPPQ*4; bool found=false;
  for (const auto& inst : instances){
    if (!inst.pattern || !inst.enabled) continue;
    for (const auto& ch : inst.pattern->channels){
      for (const auto& note : ch.notes){
        const Tick abs = inst.startTick + note.start;
        if (abs > currentTick){ nextTick = std::min(nextTick, abs); found=true; }
      }
    }
  }
  return found ? nextTick : currentTick + kPPQ;
}
bool Scheduler::has_events_in_range(const std::vector<PatternInstance>& instances, Tick startTick, Tick endTick) const{
  for (const auto& inst : instances){
    if (!inst.pattern || !inst.enabled) continue;
    for (const auto& ch : inst.pattern->channels){
      for (const auto& note : ch.notes){
        const Tick a = inst.startTick + note.start;
        const Tick b = a + note.len;
        if (a < endTick && b > startTick) return true;
      }
    }
  }
  return false;
}
} // namespace
