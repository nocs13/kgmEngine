#ifndef KGMSNINPUTLISTENER_H
#define KGMSNINPUTLISTENER_H

#include "../kgmSensor.h"

class kgmSnInputListener : public kgmSensor
{
  KGM_OBJECT(kgmSnInputListener);

public:
  kgmSnInputListener(kgmIGame* g)
  {

  }

  ~kgmSnInputListener()
  {

  }

  virtual void input(u32, int)
  {

  }

public:
  static kgmSnInputListener* New(kgmIGame* g)
  {
    return new kgmSnInputListener(g);
  }
};
#endif // KGMSNINPUTLISTENER_H
