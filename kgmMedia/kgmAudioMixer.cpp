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
    memset(buffer.data(), 0x7f, buffer.length());
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

  if(samples_per_second > 44100)
    return false;

  u32 size = channels * bytes_per_sample * samples_per_second;

  frames = size / bytes_per_sample;

  bool res = buffer.alloc(size);

  clean();

  return res;
}

u32  kgmAudioMixer::mixdata(void* data, u32 size, u32 chn, u32 bps, u32 rate)
{
  if(data == null || size < 1)
    return 0;

  if(buffer.data() == null || buffer.length() < 1)
    return 0;

  u32 bytespersample = chn * bps / 8;

  u32 readsize = bytespersample * frames;

  if(readsize > size)
    readsize = size;

  for(int i = 0; i < readsize; i += bytespersample)
  {
    for(int j = 0; j < bytespersample; j++)
    {
      buffer[i + j] += ((s8*)data)[i + j];
    }
  }

  return readsize;
}
