#ifndef KGMALSA_H
#define KGMALSA_H

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmAudioMixer.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmFunction.h"
#include "../kgmSystem/kgmLib.h"
#include "../kgmSystem/kgmThread.h"

class _Sound;

class kgmAlsa: public kgmIAudio
{
  KGM_OBJECT(kgmAlsa);

  class Thread: public kgmThread
  {
    kgmAlsa* object;

    kgmFunction<int(kgmAlsa*, int)> callback;
    
  public:

    Thread()
    {
      object   = null;
    }

    bool start(kgmAlsa* obj, kgmFunction<int(kgmAlsa*, int)> fn)
    {
      object   = obj;
      callback = fn;

      return exec();
    }

    void run()
    {
      callback(*object, 0);
    }
  };

private:
  void*         m_handle;

  kgmLib        m_lib;

  Thread        m_thread;
  Thread        m_render;
  Thread::Mutex m_mutex;

  kgmList<_Sound*> m_sounds;

  kgmAudioMixer    m_mixer;

  bool             m_proceed;

public:
  kgmAlsa();
  ~kgmAlsa();

  void clear();

  Sound create(FMT fmt, u16 freq, u32 size, void* data);
  void  remove(Sound snd);

  void  channel(Sound snd, s16 pan);
  void  volume(Sound snd, u16 vol);
  void  pause(Sound snd, bool stat);
  void  play(Sound snd, bool loop);
  void  stop(Sound snd);

  int   render(int);
  int   proceed(int);
};

#endif // KGMALSA_H
