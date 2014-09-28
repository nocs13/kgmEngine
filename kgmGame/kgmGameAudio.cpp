#include "kgmGameAudio.h"

#ifdef OAL
#include "../kgmSystem/kgmOAL.h"
#elif defined(ALSA)
#include "../kgmSystem/kgmAlsa.h"
#elif defined(OSL)
#include "../kgmSystem/kgmOSL.h"
#elif defined(DSOUND)
#include "../kgmSystem/kgmDSound.h"
#endif

KGMOBJECT_IMPLEMENT(kgmGameAudio, kgmIAudio);

kgmGameAudio::kgmGameAudio()
{
#ifdef OAL
  m_audio = new kgmOAL();
#elif defined(ALSA)
  m_audio = new kgmAlsa();
#elif defined(OSL)
  m_audio = new kgmOSL();
#elif defined(DSOUND)
  m_audio = new kgmDSound();
#else
  m_audio = new kgmNullAudio();
#endif
}

kgmGameAudio::~kgmGameAudio()
{

}
