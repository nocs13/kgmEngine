// kgmOALDevice.h: interface for the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmMedia/kgmWave.h"
#include "../kgmMath/kgmMath.h"

#ifdef DSOUND
#include <windows.h>
#include <dsound.h>

class kgmDSound: public kgmIAudio
{
  class _Sound: public Sound
  {
    LPDIRECTSOUNDBUFFER pSb;

    virtual ~_Sound();
  public:
    _Sound(LPDIRECTSOUNDBUFFER sb);
    void release();
    void stop();
    void play(bool loop);
    void pause();
    void volume(float vol);
    void emit(vec3& pos, vec3& vel);

    void drop();
  };

  LPDIRECTSOUND m_pSnd;

public:
  kgmDSound();
  virtual ~kgmDSound();

  Sound* create(FMT fmt, u16 freq, u32 size, void* data);
  void   listener(vec3& pos, vec3& vel, vec3& ort);
  void   clear();
};

#endif
