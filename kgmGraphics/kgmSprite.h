#pragma once

class kgmSprite: public kgmObject{
  KGM_OBJECT(kgmSprite);
public:
  uRect  m_rect;
  bool   m_alpha;
public:
 kgmSprite();
 ~kgmSprite();
};
