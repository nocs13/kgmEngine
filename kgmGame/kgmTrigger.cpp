#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

KGMOBJECT_IMPLEMENT(kgmTrigger, kgmUnit);

kgmTrigger::kgmTrigger(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";
}

void kgmTrigger::sendToTarget(kgmString e)
{
  kgmUnit* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
