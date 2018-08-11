#pragma once
#ifndef KGMAUDIOMIXER_H
#define KGMAUDIOMIXER_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMemory.h"
#include "kgmIAudio.h"

class kgmAudioMixer : public kgmObject
{
  KGM_OBJECT(kgmAudioMixer);
private:
  kgmMemory<s8> buffer;

  u32       time;
  u32       rate;
  u32       frames;
  u32       format;
  u32       channels;
  u32       bytes_per_sample;
  u32       bytes_per_frame;
  
  u32       active_size;

public:
  kgmAudioMixer();
  ~kgmAudioMixer();

  void clean();
  bool prepare(u32 chn, u32 bps, u32 sps);
  u32  mixdata(void* data, u32 len, u32 chn, u32 bps, u32 sps, u16 vol = kgmIAudio::VolMax,
               s16 pan = kgmIAudio::PanBalance);

  void* getBuffer()
  {
    return buffer.data();
  }

  u32 getFrames()
  {
    return active_size / getBytesPerFrame();
    //return frames;
  }

  u32 getFormat()
  {
    return format;
  }

  u32 getLength()
  {
    return buffer.length();
  }

  u32 getChannels()
  {
    return channels;
  }

  u32 getBytesPerSample()
  {
    return bytes_per_sample;
  }

  u32 getRate()
  {
    return rate;
  }

  u32 getMsTime()
  {
    //time in miliseconds

    return time;
  }

  u32 getBytesPerFrame()
  {
    return (channels * bytes_per_sample);
  }
};

#endif // KGMAUDIOMIXER_H
