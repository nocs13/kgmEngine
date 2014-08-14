#include "kgmSensor.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

KGMOBJECT_IMPLEMENT(kgmSensor, kgmGameObject);

kgmSensor::kgmSensor(kgmIGame* g)
  :kgmGameObject(g)
{
  target = "";
}

void kgmSensor::sendToTarget(kgmString e)
{
  kgmGameObject* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
