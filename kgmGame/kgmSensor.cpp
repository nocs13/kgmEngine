#include "kgmSensor.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

kgmSensor::kgmSensor(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";

  m_type = Sensor;
}

void kgmSensor::event(kgmObject* a, kgmString e)
{
  kgmUnit* o = kgmIGame::getGame()->getLogic()->getObject(target);

  if(o)
    o->event(this, e);
}
