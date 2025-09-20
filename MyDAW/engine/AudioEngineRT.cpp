#include "AudioEngineRT.h"
namespace mydaw {
void AudioEngineRT::process(const float* /*in*/, float* /*out*/, int frames){
  events_.clear();
  sched_.gather_realtime(active_, samplePos_, (uint32_t)frames, events_);
  for (const auto& ev : events_){
    auto bp = GenIdMapper::gen_to_pad(ev.gen);
    switch (ev.type){
      case mydaw::midi::EvType::NoteOn:  sampler_.note_on({bp.first,bp.second,ev.vel}); break;
      case mydaw::midi::EvType::NoteOff: sampler_.note_off({bp.first,bp.second,ev.vel}); break;
      case mydaw::midi::EvType::PitchBend: sampler_.pitch_bend(bp.first,bp.second,ev.pb); break;
    }
  }
  samplePos_ += frames;
}
} // namespace
