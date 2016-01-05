#include "kgmObstacle.h"

KGMOBJECT_IMPLEMENT(kgmObstacle, kgmObject);

kgmObstacle::kgmObstacle()
{
  transform.identity();
}

kgmObstacle::~kgmObstacle()
{
  triangles.clear();
}
