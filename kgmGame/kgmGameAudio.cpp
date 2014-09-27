#include "kgmGameAudio.h"

KGMOBJECT_IMPLEMENT(kgmGameAudio, kgmIAudio);

kgmGameAudio::kgmGameAudio()
{
#ifdef OAL
  m_audio = new kgmOAL();
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
