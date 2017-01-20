#ifndef KGMGAMELIGHT_H
#define KGMGAMELIGHT_H

#include "kgmGameNode.h"
#include "../kgmGraphics/kgmLight.h"

class kgmGameLight : public kgmGameNode
{
  KGM_OBJECT(kgmGameLight);

private:
  kgmLight* m_light;

public:
  kgmGameLight(kgmIGame*);

  kgmLight* light() const
  {
    return m_light;
  }
};

#endif // KGMGAMELIGHT_H
