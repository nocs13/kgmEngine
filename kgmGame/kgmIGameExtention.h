#ifndef KGMIGAMEEXTENTION_H
#define KGMIGAMEEXTENTION_H

#include "../kgmBase/kgmInterface.h"

#define KGMFN_GAMEEXTENTION "kgmGameExtention"

typedef kgmIGameExtention* (*KGMGAMEEXTENTION) ();

class kgmIGame;

class kgmIGameExtention: public kgmInterface
{
public:
  virtual bool prepare(kgmIGame*) = 0;
};

#endif // KGMIGAMEEXTENTION_H
