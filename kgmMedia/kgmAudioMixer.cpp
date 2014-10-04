#include "kgmAudioMixer.h"
#include <math.h>

#define TEST

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

  u32 size = channels * bytes_per_sample * rate;

  frames = size / (channels *bytes_per_sample);

  bool res = buffer.alloc(size);

  clean();

  return res;
}

u32  kgmAudioMixer::mixdata(void* data, u32 size, u32 chn, u32 bps, u32 rate)
{
#ifdef TEST

  for(int i = 0; i < frames; i+=2)
  {
    s32 a = 300 * i;//0 * sin(18 * i);
    ((s16*)buffer.data())[i]     = a;
    ((s16*)buffer.data())[i + 1] = a;
  }

  return true;
#endif

  if(data == null || size < 1)
    return 0;

  if(buffer.data() == null || buffer.length() < 1)
    return 0;

  u32 bpf = chn * bps / 8;

  u32 byps = bps / 8;

  u32 mframes = size / bpf;

  u32 rframes = (mframes < frames) ? (mframes) : (frames);

  u32 readsize = bpf * rframes;

  if(readsize > size)
    readsize = size;

  for(int i = 0; i < rframes; i++)
  {
    char* lsample = buffer.data() + bytes_per_sample * i;

    s16 s1 = 0, s2 = 0;

    if(chn == 2)
    {
      memcpy(&s1, data + bpf * i, byps);
      memcpy(&s2, data + bpf * i + byps, byps);
    }
    else
    {
      memcpy(&s1, data + bpf * i, byps);
      s2 = s1;
    }

    ((s16*)lsample)[0] += s1;
    ((s16*)lsample)[1] += s2;
  }

  return readsize;
}
