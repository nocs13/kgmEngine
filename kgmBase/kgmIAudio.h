#pragma once
#include "kgmInterface.h"
#include "../kgmMath/kgmMath.h"

//class kgmWave;
//class kgmSound;

class kgmIAudio: public kgmInterface{
public: 
  enum FMT{
    FMT_NONE,
    FMT_MONO8,
    FMT_MONO16,
    FMT_STEREO8,
    FMT_STEREO16,
  };
 
  class Sound{
  public:
    virtual void release()                  = 0;
    virtual void stop()                     = 0;
    virtual void play(bool loop)            = 0;
    virtual void pause()                    = 0;
    virtual void volume(float vol)          = 0;
    virtual void emit(vec3& pos, vec3& vel) = 0;
  }; 
public:
  virtual Sound* create(FMT fmt, u16 freq, u32 size, void* data) = 0; //create sound object from .wav ...
  virtual void   listener(vec3& pos, vec3& vel, vec3& ort)       = 0;
  virtual void   release()                                       = 0;
};
