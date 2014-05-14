#ifndef KGMTRIGGER_H
#define KGMTRIGGER_H

#include "kgmGameObject.h"

class kgmTrigger : public kgmGameObject
{
  KGM_OBJECT(kgmTrigger);

public:
  kgmTrigger();

  virtual void triggering(){}
};

#endif // KGMTRIGGER_H
