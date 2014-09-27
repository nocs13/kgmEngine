#ifndef KGMALSA_H
#define KGMALSA_H

#ifdef ALSA

#include <alsa/asoundlib.h>

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmBase/kgmObject.h"

class kgmAlsa : public kgmIAudio
{
  KGM_OBJECT(kgmAlsa);

public:
  kgmAlsa();
  ~kgmAlsa();
};

#endif //ALSA
#endif // KGMALSA_H
