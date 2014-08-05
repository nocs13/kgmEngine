#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmGameObject.h"

class kgmSensor : public kgmGameObject
{
  KGM_OBJECT(kgmSensor);

  kgmString target;

public:
  kgmSensor();

  virtual void sense(){}

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
