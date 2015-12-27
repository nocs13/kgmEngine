#pragma once
#include "kgmIAudio.h"
#include "../kgmBase/kgmResource.h"

class kgmSound: public kgmResource
{
  KGM_OBJECT(kgmSound)

  private:
    kgmIAudio::Sound m_sound;

public:
  kgmSound();
  kgmSound(kgmIAudio::Sound s);

  kgmIAudio::Sound  getSound()
  {
    return m_sound;
  }

  ~kgmSound();
};
