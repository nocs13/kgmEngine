#include "kgmSound.h"

kgmSound::kgmSound()
  :kgmResource()
{
  m_sound = null;
}

kgmSound::kgmSound(kgmIAudio::Sound s)
  :kgmResource()
{
  m_sound = s;
}

kgmSound::~kgmSound()
{
}
