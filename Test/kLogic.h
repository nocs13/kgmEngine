#ifndef KLOGIC_H
#define KLOGIC_H

#include "../kgmGame/kgmIGame.h"
#include "../kgmGame/kgmGameLogic.h"

class kLogic: public kgmGameLogic
{
  kgmIGame* m_game;
public:
  kLogic(kgmIGame* g);
};

#endif // KLOGIC_H
