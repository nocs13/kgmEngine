#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

KGMOBJECT_IMPLEMENT(kgmTrigger, kgmGameObject);

kgmTrigger::kgmTrigger()
{
  target = "";
}

void kgmTrigger::sendToTarget(kgmString e)
{
  kgmGameObject* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
