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
  kgmGameBase* game = (kgmGameBase*)kgmIGame::getGame();

  if(!body || !tobody || !body->m_udata || !tobody->m_udata)
    return;

  kgmGameObject* go_body   = (kgmGameObject*)body->m_udata;
  kgmGameObject* go_tobody = (kgmGameObject*)tobody->m_udata;

  if(go_body->getParent() == go_tobody ||
     go_tobody->getParent() == go_body)
  {
    int k = 0;
  }
  else
  {
    //go_body->remove();
    //go_body->getBody()->remove();
    //go_body->getVisual()->remove();
  }
}

