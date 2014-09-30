#ifndef KGMALSA_H
#define KGMALSA_H

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmAudioMixer.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmSystem/kgmLib.h"
#include "../kgmSystem/kgmThread.h"

class _Sound;

class kgmAlsa : public kgmIAudio
{
  KGM_OBJECT(kgmAlsa);

  class Thread: public kgmThread
  {
    kgmAlsa* object;

    int (*callback)(kgmAlsa*);
  public:

    Thread()
    {
      object   = null;
      callback = null;
    }

    bool start(kgmAlsa* obj, int (*call)(kgmAlsa*))
    {
      object   = obj;
      callback = call;
    }

    void run()
    {
      if(object && callback)
        callback(object);
    }
  };

private:
  void*  m_handle;

  kgmLib        m_lib;

  Thread        m_thread;

  kgmList<_Sound*> m_sounds;

  kgmAudioMixer    m_mixer;
public:
  kgmAlsa();
  ~kgmAlsa();

  Sound create(FMT fmt, u16 freq, u32 size, void* data);
  void  remove(Sound snd);

  void  channel(Sound snd, s16 pan);
  void  volume(Sound snd, u16 vol);
  void  pause(Sound snd, bool stat);
  void  play(Sound snd, bool loop);
  void  stop(Sound snd);

  int   proceed();
};

#endif // KGMALSA_H
