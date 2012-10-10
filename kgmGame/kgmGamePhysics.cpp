#include "kgmGamePhysics.h"
#include "kgmGameCollision.inl"

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
 this->doCollision(dtime);
}

void kgmGamePhysics::collision(kgmBody* body, kgmBody* tobody){
#ifdef WIN32
#endif
 kgmGameBase* game = (kgmGameBase*)kgmIGame::getGame();
}

