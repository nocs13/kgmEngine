#pragma once
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmObject.h"

//class kgmWave;
//class kgmSound;

/*
  Supports 1 or 2 channel stream with
  1 or 2 byte sample. 8000 till 48000
  frequency.
*/

class kgmIAudio: public kgmObject
{
public: 
  enum Volume
  {
    VolMin = 0,
    VolMax = 10000
  };

  enum Pan
  {
    PanLeft    = -10000,
    PanBalance =  0,
    PanRight   =  10000
  };

  enum FMT
  {
    FMT_NONE,
    FMT_MONO8,
    FMT_MONO16,
    FMT_STEREO8,
    FMT_STEREO16
  };

  typedef void* Sound;

public:
  virtual Sound  create(FMT fmt, u16 freq, u32 size, void* data) = 0;
  virtual void   remove(Sound snd)                               = 0;

  virtual void   channel(Sound snd, s16 rel)                     = 0;
  virtual void   volume(Sound snd, u16 vol)                      = 0;
  virtual void   pause(Sound snd, bool stat)                     = 0;
  virtual void   play(Sound snd, bool loop)                      = 0;
  virtual void   stop(Sound snd)                                 = 0;
};
