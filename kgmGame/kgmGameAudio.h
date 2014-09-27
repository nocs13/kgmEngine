#ifndef KGMGAMEAUDIO_H
#define KGMGAMEAUDIO_H

#include "../kgmMedia/kgmNullAudio.h"

#ifdef OAL
#include "../kgmSystem/kgmOAL.h"
#elif defined(OSL)
#include "../kgmSystem/kgmOSL.h"
#elif defined(D3DS)
#include "../kgmSystem/kgmD3ds.h"
#endif

class kgmGameAudio
#ifdef OAL
  :public kgmOAL
#elif defined(OSL)
  :public kgmOSL
#elif defined(D3DS)
    :public kgmD3ds
#else
    :public kgmNullAudio
#endif
{
private:
  kgmIAudio*  m_audio;

};
#endif // KGMGAMEAUDIO_H
