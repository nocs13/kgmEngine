#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmGameObject.h"

class kgmSensor : public kgmGameObject
{
  KGM_GO_OBJECT(kgmSensor);

  kgmString target;

public:
  kgmSensor(kgmIGame* g = null);

  virtual void sense(){}

  void init() {}

  void setTarget(kgmString s)
  {
    target = s;
  }

  kgmString getTarget()
  {
    return target;
  }

private:
  void sendToTarget(kgmString s);
};

#endif // KGMSENSOR_H
