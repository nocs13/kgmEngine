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
};

#endif // KGMALSA_H
