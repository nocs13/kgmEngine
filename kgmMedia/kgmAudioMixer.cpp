#include "kgmAudioMixer.h"

kgmAudioMixer::kgmAudioMixer()
{
  frames = 0;
  channels = 0;
  bytes_per_sample = 0;
  samples_per_second = 0;
}

kgmAudioMixer::~kgmAudioMixer()
{
  buffer.clear();
}

void kgmAudioMixer::clean()
{
  if(buffer.length() > 0)
    memset(buffer.data(), 0, buffer.length());
}

bool kgmAudioMixer::prepare(u32 chn, u32 bps, u32 sps)
{
  if(chn == 0 || bps == 0 || sps == 0)
    return false;

  if(chn < 1|| chn > 2)
    return false;

  channels = chn;

  bytes_per_sample = bps / 8;

  if(bytes_per_sample < 1 || bytes_per_sample > 2)
    return false;

  //8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz

  samples_per_second = sps;

  if(samples_per_second > 45000)
    return false;

  u32 size = channels * bytes_per_sample * samples_per_second;

  return buffer.alloc(size);
}

u32  kgmAudioMixer::mixdata(void* data, u32 len, u32 chn, u32 bps, u32 sps)
{
  if(data == null || len == 0)
    return 0;

  if(buffer.length() < 1)
    return 0;
}
