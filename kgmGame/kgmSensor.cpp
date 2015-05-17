#include "kgmSensor.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

KGMOBJECT_IMPLEMENT(kgmSensor, kgmUnit);

kgmSensor::kgmSensor(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";
}

void kgmSensor::sendToTarget(kgmString e)
{
  kgmUnit* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
