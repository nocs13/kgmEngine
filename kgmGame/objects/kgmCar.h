#ifndef KGMCAR_H
#define KGMCAR_H

#include "../kgmUnit.h"


class kgmCar: public kgmUnit
{
  KGM_OBJECT(kgmCar);
  KGM_UNIT(kgmCar);

public:
  kgmCar(kgmIGame*);
};

#endif // KGMCAR_H
