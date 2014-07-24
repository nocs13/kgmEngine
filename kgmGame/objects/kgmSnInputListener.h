#ifndef KGMSNINPUTLISTENER_H
#define KGMSNINPUTLISTENER_H

#include "../kgmSensor.h"

class kgmSnInputListener : public kgmSensor
{
  KGM_OBJECT(kgmSnInputListener);

public:
  kgmSnInputListener()
  {

  }

  ~kgmSnInputListener()
  {

  }

  virtual void input(u32, int)
  {

  }

public:
  static kgmSnInputListener* New()
  {
    return new kgmSnInputListener();
  }
};
#endif // KGMSNINPUTLISTENER_H
