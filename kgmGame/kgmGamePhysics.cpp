#include "kgmGamePhysics.h"

#include "../kgmPhysics/kgmCollision.h"
#include "../kgmPhysics/kgmIntersection.h"


#include "kgmIGame.h"
#include "kgmGameBase.h"

#ifdef WIN32
#include <windows.h>
#endif

kgmGamePhysics::kgmGamePhysics()
{
}

kgmGamePhysics::~kgmGamePhysics()
{
}

void kgmGamePhysics::update(float dtime)
{
  kgmPhysics::update(dtime);
}

void kgmGamePhysics::collision(kgmBody* body, kgmBody* tobody)
{
  if(!body || !tobody || !body->m_udata || !tobody->m_udata)
    return;

  kgmUnit* go_body   = (kgmUnit*)body->m_udata;
  kgmUnit* go_tobody = (kgmUnit*)tobody->m_udata;

  kgmIGame::getGame()->getLogic()->collide(go_body, go_tobody);
}

