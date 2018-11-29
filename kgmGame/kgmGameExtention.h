#ifndef KGMGAMEEXTENTION_H
#define KGMGAMEEXTENTION_H

#include "../kgmBase/kgmInterface.h"

typedef kgmGameExtention* (*KGMGETGAMEEXTENTION)();

class kgmIGame;

class kgmGameExtention: public kgmInterface
{
public:

  void prepare(kgmIGame*) = 0;
};

#endif // KGMGAMEEXTENTION_H
