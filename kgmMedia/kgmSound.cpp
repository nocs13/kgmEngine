#include "kgmSound.h"

KGMOBJECT_IMPLEMENT(kgmSound, kgmResource);

kgmSound::kgmSound()
:kgmResource(){
 m_sound = null;
}

kgmSound::kgmSound(kgmIAudio::Sound* s)
:kgmResource(){
 m_sound = s;
}

kgmSound::~kgmSound()
{
}
