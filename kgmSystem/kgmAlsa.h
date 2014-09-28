#ifndef KGMALSA_H
#define KGMALSA_H

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmLib.h"

class kgmAlsa : public kgmIAudio
{
  KGM_OBJECT(kgmAlsa);

  void*  m_handle;
  kgmLib m_lib;
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
};

#endif // KGMALSA_H
