#ifndef KGMSENSOR_H
#define KGMSENSOR_H

#include "kgmUnit.h"

class kgmSensor : public kgmUnit
{
  KGM_OBJECT(kgmSensor);

  kgmString source;
  kgmString target;
  kgmString action;

public:
  kgmSensor(kgmIGame* g = null);

  virtual void sense() {}

  void start();
  void finish();
  void update();

  void setSource(kgmString s)
  {
    source = s;
  }

  kgmString getSource()
  {
    return source;
  }

  void setTarget(kgmString s)
  {
    target = s;
  }

  kgmString getTarget()
  {
    return target;
  }

  void setAction(kgmString s)
  {
    action = s;
  }

  kgmString getAction()
  {
    return action;
  }

private:
  void event(kgmObject* o, kgmString s);
};

#endif // KGMSENSOR_H
