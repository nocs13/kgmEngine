#ifndef KGMSKY_H
#define KGMSKY_H

#include "../kgmUnit.h"

class kgmSky : public kgmUnit
{
  KGM_GO_OBJECT(kgmSky);

public:
  kgmSky(kgmIGame*);
};

#endif // KGMSKY_H
