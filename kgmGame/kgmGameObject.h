#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"

#include "../kgmGraphics/kgmIGraphics.h"

#include "../kgmPhysics/kgmIPhysics.h"



class kgmGameObject: public kgmIGraphics::INode, public kgmIPhysics::IBody, public kgmObject
{
public:
  kgmGameObject();
};

#endif // KGMGAMEOBJECT_H
