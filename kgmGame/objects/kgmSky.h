#ifndef KGMSKY_H
#define KGMSKY_H

#include "../kgmUnit.h"

class kgmSky : public kgmUnit
{
  KGM_OBJECT(kgmSky);

public:
  kgmSky(kgmIGame*);

  void init(){}
  void exit(){}
};

#endif // KGMSKY_H
