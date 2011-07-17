#pragma once
#include "../kgmMain/kgmObject.h"
#include "../kgmMain/kgmTab.h"

class kgmFog: public kgmObject{
 KGM_OBJECT(kgmFog)

public:
 uint  m_type;
 uint  m_mode;
 float m_density;
 float m_start;
 float m_end;
 vec4  m_color;

public:
 kgmFog(){
  m_mode = 0;
  m_density = 0.5f;
  m_start = 10.0f;
  m_end = 100.0f;
 }
};