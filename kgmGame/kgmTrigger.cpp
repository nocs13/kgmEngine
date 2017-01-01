#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

kgmTrigger::kgmTrigger(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";
}

void kgmTrigger::sendToTarget(kgmString e)
{
  kgmGameNode* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
