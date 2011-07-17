#pragma once
#include "../kgmMain/kgmResource.h"

class kgmSound: public kgmResource{
 KGM_OBJECT(kgmSound)
public:
 void*	m_sound;
 float	m_volume, 
		m_pan;
public:
 kgmSound();
 kgmSound(void* s);
private:
 ~kgmSound();
};
