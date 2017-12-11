#include "kgmGameAudio.h"
#include "../kgmBase/kgmLog.h"
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

kgmGameAudio::kgmGameAudio()
{
#if defined(ALSA)
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
#ifdef DEBUG
  kgm_log() << "kgmGameAudio::~kgmGameAudio.\n";
#endif

  if(m_audio)
    delete m_audio;

  m_audio = null;
}
