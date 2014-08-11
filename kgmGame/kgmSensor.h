#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmGameObject.h"

class kgmSensor : public kgmGameObject
{
  KGM_OBJECT(kgmSensor);

  kgmString target;

#ifdef EDITOR
  static kgmList<kgmString> g_typ_sensors;
#endif

public:
  kgmSensor(kgmIGame* g = null);

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
