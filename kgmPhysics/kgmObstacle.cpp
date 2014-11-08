#include "kgmObstacle.h"

KGMOBJECT_IMPLEMENT(kgmObstacle, kgmObject);

kgmObstacle::kgmObstacle()
{
  sx = sy = sz = 0;
}

kgmObstacle::kgmObstacle(float x, float y, float z)
{
  sx = x;
  sy = y;
  sz = z;
}
