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

  Sound* create(FMT fmt, u16 freq, u32 size, void* data);
  void   listener(vec3& pos, vec3& vel, vec3& ort);
  void   clear();
};

#endif // KGMALSA_H
