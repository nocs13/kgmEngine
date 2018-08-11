#ifndef KGMALSA_H
#define KGMALSA_H

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmAudioMixer.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmFunction.h"
#include "../kgmSystem/kgmLib.h"
#include "../kgmSystem/kgmThread.h"

struct _Sound;

class kgmAlsa: public kgmIAudio
{
  KGM_OBJECT(kgmAlsa);

private:
  void*         m_handle;

  kgmLib        m_lib;

  kgmThread::Thread m_thread;
  kgmThread::Thread m_render;
  kgmThread::Mutex  m_mutex;

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

  int   render();
  int   proceed();

  static int render_a(kgmAlsa* a)
  {
    a->render();
  }

  static int proceed_a(kgmAlsa* a)
  {
    a->proceed();
  }
};

#endif // KGMALSA_H
