#ifndef KGMCAR_H
#define KGMCAR_H

#include "../kgmActor.h"


class kgmCar: public kgmActor
{
  KGM_UNIT(kgmCar);

public:
  kgmCar(kgmIGame*);

};

#endif // KGMCAR_H
