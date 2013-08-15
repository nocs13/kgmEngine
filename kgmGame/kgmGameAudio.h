#ifndef KGMGAMEAUDIO_H
#define KGMGAMEAUDIO_H
#ifdef OAL
#include "../kgmSystem/kgmOAL.h"
#elif defined(OSL)
#include "../kgmSystem/kgmOSL.h"
#endif

class kgmGameAudio:
#ifdef OAL
  public kgmOAL
#elif defined(OSL)
  public kgmOSL
//#elif defined(D3DS)
//#else
#endif
{

};
#endif // KGMGAMEAUDIO_H
