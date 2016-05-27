#include "kgmAudioMixer.h"
#include "kgmIAudio.h"
#include "../kgmBase/kgmLog.h"
#include <math.h>

inline s16 snd_normalize(s16 val)
{
  s16 max = 32000;

  if(val > max)
    val = max;
  else if(val < -max)
    val = -max;
}

kgmAudioMixer::kgmAudioMixer()
{
  rate = 0;
  frames = 0;
  format = 0;
  channels = 0;
  bytes_per_sample = 0;
  bytes_per_frame  = 0;
  active_size = 0;
}

kgmAudioMixer::~kgmAudioMixer()
{
  buffer.clear();

#ifdef DEBUG
  kgm_log() << "kgmAudioMixer::~kgmAudioMixer.\n";
#endif
}

void kgmAudioMixer::clean()
{
  if(buffer.length() > 0)
    memset(buffer.data(), 0x00, buffer.length());

  active_size = 0;
}

bool kgmAudioMixer::prepare(u32 chn, u32 bps, u32 fr)
{
  if(chn == 0 || bps == 0 || fr == 0)
    return false;

  if(chn < 1 || chn > 2)
    return false;

  channels = chn;

  bytes_per_sample = bps / 8;

  bytes_per_frame = channels * bytes_per_sample;

  if(bytes_per_sample < 1 || bytes_per_sample > 2)
    return false;

  //8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz

  rate = fr;

  if(rate < 8000 || rate > 48000)
    return false;

  u32 size = channels * bytes_per_sample * rate;

  size /= 10;
  //size *= 2;

  time = 100 * 1;

  frames = size / (channels * bytes_per_sample);

  if(channels == 2 && bytes_per_sample == 2)
    format = kgmIAudio::FMT_STEREO16;
  else if(channels == 2 && bytes_per_sample == 1)
    format = kgmIAudio::FMT_STEREO8;
  else if(channels == 1 && bytes_per_sample == 2)
    format = kgmIAudio::FMT_MONO16;
  else if(channels == 1 && bytes_per_sample == 1)
    format = kgmIAudio::FMT_MONO8;
  else
    format = kgmIAudio::FMT_NONE;

  bool res = buffer.alloc(size);

  clean();

  return res;
}

u32  kgmAudioMixer::mixdata(void *data, u32 size, u32 chn, u32 bps, u32 sps, u16 vol, s16 pan)
{
  if(data == null || size < 1)
    return -1;

  if(buffer.data() == null || buffer.length() < 1)
    return -1;

  u32 divs = (u32)ceil(rate / sps);

  u32 bpf = chn * bps / 8;

  u32 byps = bps / 8;

  if(byps > 4)
    return -1;

  u32 mframes = size / bpf;

  u32 rframes = (mframes < (frames / divs)) ? (mframes) : (frames / divs);

  u32 divfb = (u32)ceil(bytes_per_frame / bpf);

  u32 readsize = bpf * rframes;

  if(readsize > size)
    readsize = size;

  u32 writesize = readsize * divfb;

  if(writesize > active_size)
    active_size = writesize;

  float vol_div[2] = {1};

  vol_div[0] = vol_div[1] = (vol >= kgmIAudio::VolMax) ? (1.0f) : ((float)vol / (float)kgmIAudio::VolMax);

  if(channels == 2)
  {
    if(pan < kgmIAudio::PanLeft)
      pan = kgmIAudio::PanLeft;
    else if(pan > kgmIAudio::PanRight)
      pan = kgmIAudio::PanRight;

    float pan1 = 1.0f;// + (float)pan / (float)kgmIAudio::PanRight;
    float pan2 = 1.0f;// - pan1;

    if(pan > 0)
    {
      pan1 = 1.0f - (float)pan / (float)kgmIAudio::PanRight;
    }
    else
    {
      pan2 = 1.0f - (float)pan / (float)kgmIAudio::PanLeft;
    }

    vol_div[0] *= pan1;
    vol_div[1] *= pan2;
  }

  for(int i = 0; i < rframes; i++)
  {
    s32 ref = bytes_per_frame * i * divs;

    if(ref > buffer.length())
    {
      int k = 0;

      break;
    }

    void* lsample = buffer.data() + ref;

    s16 s1 = 0;
    s16 s2 = 0;

    if(chn == 2)
    {
      memcpy(&s1, data + bpf * i, byps);
      memcpy(&s2, data + bpf * i + byps, byps);

      if(byps == 1)
      {
        s1 =  (s16)(s1 - 0x80) << 8;
        s2 =  (s16)(s2 - 0x80) << 8;
      }
    }
    else
    {
      memcpy(&s1, data + bpf * i, byps);

      if(byps == 1)
        s1 =  (s16)(s1 - 0x80) << 8;

      s2 = s1;
    }

    s1 = (s16)((float)s1 * (float)vol_div[0]);
    s2 = (s16)((float)s2 * (float)vol_div[1]);

    //a + b - (a * b) - 65535;
    //a + b - (a * b) / 65535;

    for(int j = 0; j < divs; j++)
    {
      u32 ref1 = bytes_per_frame * j;
      u32 ref2 = ref1 + bytes_per_sample;

      s16 r1 = 0;
      s16 r2 = 0;

      //r1 = ((s16*)lsample)[bytes_per_frame * j];
      //r2 = ((s16*)lsample)[bytes_per_frame * j + 1];

      memcpy(&r1, lsample + ref1, bytes_per_sample);
      memcpy(&r2, lsample + ref2, bytes_per_sample);

      r1 = ((r1 + s1) >> 1);
      r2 = ((r2 + s2) >> 1);
      //r1 = snd_normalize(r1 + s1 >> 1);
      //r2 = snd_normalize(r2 + s2 >> 1);
      //r1 = snd_normalize(r1 + s1 - (r1 * s1) / 0xffff);
      //r2 = snd_normalize(r2 + s2 - (r2 * s2) / 0xffff);
      //r1 = snd_normalize((s1));
      //r2 = snd_normalize((s2));
      //r1 = 15000;
      //r2 = 15000;

      //((s16*)lsample)[0] = r1;
      //((s16*)lsample)[1] = r2;
      //((s16*)lsample)[bytes_per_frame * j]     = r1;
      //((s16*)lsample)[bytes_per_frame * j + 1] = r2;
      memcpy(lsample + ref1, &r1, bytes_per_sample);
      memcpy(lsample + ref2, &r2, bytes_per_sample);
    }
  }

  return readsize;
}
