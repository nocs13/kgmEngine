#include "kgmAudioMixer.h"

kgmAudioMixer::kgmAudioMixer()
{
  rate = 0;
  frames = 0;
  channels = 0;
  bytes_per_sample = 0;
}

kgmAudioMixer::~kgmAudioMixer()
{
  buffer.clear();
}

void kgmAudioMixer::clean()
{
  if(buffer.length() > 0)
    memset(buffer.data(), 0x00, buffer.length());
}

bool kgmAudioMixer::prepare(u32 chn, u32 bps, u32 fr)
{
  if(chn == 0 || bps == 0 || fr == 0)
    return false;

  if(chn < 1 || chn > 2)
    return false;

  channels = chn;

  bytes_per_sample = bps / 8;

  if(bytes_per_sample < 1 || bytes_per_sample > 2)
    return false;

  //8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz

  rate = fr;

  if(rate > 44100)
    return false;

  u32 size = channels * bytes_per_sample * rate * 10;

  frames = size / (channels *bytes_per_sample);

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

  u32 bytps = chn * bps / 8;

  u32 mframes = size / bytps;

  u32 rframes = (mframes < frames) ? (mframes) : (frames);

  u32 readsize = bytps * rframes;

  if(readsize > size)
    readsize = size;

  char* sample = new char[bytps];

  for(int i = 0; i < rframes; i++)
  {
    char* lsample = buffer.data() + bytes_per_sample * i;

    memcpy(sample, data + bytps * i, bytps);

    ((s16*)lsample)[0] += ((s16*)sample)[0];
    ((s16*)lsample)[1] += ((s16*)sample)[0];
  }

  delete [] sample;

  return readsize;
}
