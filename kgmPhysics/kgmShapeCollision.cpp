#include "kgmShapeCollision.h"

KGMOBJECT_IMPLEMENT(kgmShapeCollision, kgmResource);

kgmShapeCollision::kgmShapeCollision()
{
}

kgmShapeCollision::~kgmShapeCollision()
{
  triangles.clear();
}
