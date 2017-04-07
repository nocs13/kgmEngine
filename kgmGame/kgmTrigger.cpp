#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"

kgmTrigger::kgmTrigger(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";

  m_type = Trigger;
}

void kgmTrigger::sendToTarget(kgmString e)
{
  kgmUnit* o = kgmIGame::getGame()->getLogic()->getObjectById(target);

  if(o)
    o->event(this, e);
}
