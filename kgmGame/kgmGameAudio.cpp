#include "kgmGameAudio.h"
#include "../kgmMedia/kgmNullAudio.h"

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
  m_audio = null;

#ifdef OAL
  m_audio = new kgmOAL();
#elif defined(ALSA)
  m_audio = new kgmAlsa();
  //m_audio = new kgmNullAudio();
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
  if(m_audio)
  {
    m_audio->release();
  }
}
