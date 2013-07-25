#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmSprite: public kgmObject{
  KGM_OBJECT(kgmSprite);
public:
  uRect  m_rect;
  bool   m_alpha;
  color  m_color;

public:
 kgmSprite();
 virtual ~kgmSprite();
};
