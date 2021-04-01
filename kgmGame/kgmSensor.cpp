#include "kgmSensor.h"
#include "kgmIGame.h"
#include "kgmILogic.h"
#include "kgmGameApp.h"

kgmSensor::kgmSensor(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";
}

void kgmSensor::start()
{

}

void kgmSensor::finish()
{

}

void kgmSensor::update()
{
  sense();
}

void kgmSensor::event(kgmObject* a, kgmString e)
{
  kgmUnit* o = kgmGameApp::gameApp()->game()->gUnit(target);

  if(o)
    o->event(this, e);
}
