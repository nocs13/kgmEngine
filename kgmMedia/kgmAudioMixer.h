#ifndef KGMAUDIOMIXER_H
#define KGMAUDIOMIXER_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMemory.h"

class kgmAudioMixer : public kgmObject
{
  kgmMemory<s8> buffer;

  u32       frames;
  u32       channels;
  u32       bytes_per_sample;
  u32       samples_per_second;
public:
  kgmAudioMixer();
  ~kgmAudioMixer();

  void clean();
  bool prepare(u32 chn, u32 bps, u32 sps);
  u32  mixdata(void* data, u32 len, u32 chn, u32 bps, u32 sps);

  void* getBuffer()
  {
    return buffer.data();
  }

  u32 getFrames()
  {
    return frames;
  }

  u32 getChannels()
  {
    return channels;
  }

  u32 getBitsPerSample()
  {
    return bytes_per_sample * 8;
  }

  u32 getSamplesPerSecond()
  {
    return samples_per_second;
  }

  u32 getBytesPerFrame()
  {
    return (channels * bytes_per_sample);
  }
};

#endif // KGMAUDIOMIXER_H
