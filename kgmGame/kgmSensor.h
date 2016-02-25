#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmUnit.h"

class kgmSensor : public kgmUnit
{
  KGM_OBJECT(kgmSensor);

  kgmString target;

public:
  kgmSensor(kgmIGame* g = null);

  virtual void sense(){}

  void init() {}
  void exit() {}

  void setTarget(kgmString s)
  {
    target = s;
  }

  kgmString getTarget()
  {
    return target;
  }

private:
  void event(kgmString s);
};

#endif // KGMSENSOR_H
