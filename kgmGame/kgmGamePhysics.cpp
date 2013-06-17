#include "kgmGamePhysics.h"

#include "../kgmPhysics/kgmCollision.h"
#include "../kgmPhysics/kgmIntersection.h"


#include "kgmIGame.h"
#include "kgmGameBase.h"

#ifdef WIN32
#include <windows.h>
#endif

kgmGamePhysics::kgmGamePhysics(){
}

kgmGamePhysics::~kgmGamePhysics(){
}

void kgmGamePhysics::update(float dtime){
  kgmPhysics::update(dtime);
}

void kgmGamePhysics::collision(kgmBody* body, kgmBody* tobody){
  kgmGameBase* game = (kgmGameBase*)kgmIGame::getGame();

  int k = 0;
}

