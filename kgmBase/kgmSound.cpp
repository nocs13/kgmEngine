#include "kgmSound.h"

KGMOBJECT_IMPLEMENT(kgmSound, kgmResource);

kgmSound::kgmSound()
:kgmResource(){
 m_sound = 0;
 m_volume = 0.0f;
 m_pan = 0.0f;
}

kgmSound::kgmSound(void* s)
:kgmResource(){
 m_volume = 0.0f;
 m_pan = 0.0f;
 m_sound = s;
}

kgmSound::~kgmSound(){
}
