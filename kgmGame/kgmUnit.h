#ifndef KGMUNIT_H
#define KGMUNIT_H

#include "kgmGameObject.h"

class kgmUnit : public kgmGameObject
{
  KGM_GO_OBJECT(kgmUnit);

public:
  kgmUnit(kgmIGame*);

  void init();
};

#endif // KGMUNIT_H
