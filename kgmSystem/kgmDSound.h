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
#include "inc/DX/dsound.h"

class kgmDSound: public kgmIAudio
{
  LPDIRECTSOUND m_pSnd;

public:
  kgmDSound();
  virtual ~kgmDSound();

  Sound create(FMT fmt, u16 freq, u32 size, void* data);
  void  remove(Sound snd);

  void  channel(Sound snd, s16 pan);
  void  volume(Sound snd, u16 vol);
  void  pause(Sound snd, bool stat);
  void  play(Sound snd, bool loop);
  void  pan(Sound  snd, s16 pan);
  void  stop(Sound snd);
};

#endif
