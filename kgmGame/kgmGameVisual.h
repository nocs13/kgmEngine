#ifndef KGMGAMEVISUAL_H
#define KGMGAMEVISUAL_H

#include "kgmGameNode.h"

class kgmGameVisual : public kgmGameNode
{
  KGM_OBJECT(kgmGameVisual);

public:
  kgmGameVisual(kgmIGame*);
};

#endif // KGMGAMEVISUAL_H
