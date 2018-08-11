// kgmOALDevice.h: interface for the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmAudioMixer.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmMedia/kgmWave.h"

#include "../kgmMath/kgmMath.h"

#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmFunction.h"

#include "../kgmSystem/kgmThread.h"

#ifdef DSOUND
#include <windows.h>
#include "inc/DX/dsound.h"

class _Sound;

class kgmDSound: public kgmIAudio
{
  KGM_OBJECT(kgmDSound);

  /*class Thread: public kgmThread
  {
    kgmDSound* object;

    //kgmFunction<int(kgmDSound*, int)> callback;

  public:

    Thread()
    {
      object   = null;
    }

    bool start(kgmDSound* obj, kgmFunction<int(kgmDSound*, int)> fn)
    {
      object   = obj;
      callback = fn;

      //exec();
    }

    void run()
    {
      callback(*object, 0);
    }
  };*/

  LPDIRECTSOUND       m_pSnd;
  LPDIRECTSOUNDBUFFER m_pSbuf;

  kgmAudioMixer m_mixer;

  kgmList<_Sound*> m_sounds;

  u32 m_timer;

  kgmThread::Thread m_thread;
  kgmThread::Mutex  m_mutex;

  bool     m_proceed;

  friend void CALLBACK DirectSoundProc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

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

  void  clear();

  int proceed(int);
};

#endif
