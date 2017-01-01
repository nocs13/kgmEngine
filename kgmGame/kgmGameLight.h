#ifndef KGMGAMELIGHT_H
#define KGMGAMELIGHT_H

#include "kgmGameNode.h"
#include "../kgmGraphics/kgmLight.h"

class kgmGameLight : public kgmGameNode
{
  kgmLight* m_light;

public:
  kgmGameLight(kgmIGame*);

  kgmLight* light() const
  {
    return m_light;
  }
};

#endif // KGMGAMELIGHT_H
