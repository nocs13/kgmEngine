#include "kgmSensor.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

kgmSensor::kgmSensor(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";
}

void kgmSensor::event(kgmString e)
{
  kgmUnit* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
