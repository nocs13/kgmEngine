#include "kgmTrigger.h"
#include "kgmIGame.h"
#include "kgmILogic.h"
#include "kgmGameApp.h"

kgmTrigger::kgmTrigger(kgmIGame* g)
  :kgmUnit(g)
{
  target = "";

  m_type = Trigger;
}

void kgmTrigger::sendToTarget(kgmString e)
{
  kgmUnit* o = kgmGameApp::gameApp()->game()->gUnit(target);

  if(o)
    o->event(this, e);
}
