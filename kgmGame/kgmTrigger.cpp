#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

KGMOBJECT_IMPLEMENT(kgmTrigger, kgmGameObject);

kgmTrigger::kgmTrigger()
{
  destination = null;
}

void kgmTrigger::sendDestination(kgmString e)
{
  kgmGameObject* o = kgmIGame::getGame()->getLogic()->getObjectById(destination);

  if(o)
    o->event(this, e);
}
