#ifndef KGMSKY_H
#define KGMSKY_H

#include "../kgmUnit.h"

class kgmSky : public kgmUnit
{
  KGM_UNIT(kgmSky);

public:
  kgmSky(kgmIGame*);

  void init(){}
};

#endif // KGMSKY_H
