#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmGameObject.h"

class kgmSensor : public kgmGameObject
{
  KGM_OBJECT(kgmSensor);

public:
  kgmSensor();

  virtual void sense(){}
};

#endif // KGMSENSOR_H
