#ifndef KGMSHAPECOLLISION_H
#define KGMSHAPECOLLISION_H

#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmMath/kgmMath.h"

class kgmShapeCollision: public kgmResource
{
  KGM_OBJECT(kgmShapeCollision);

public:
  kgmList<triangle3> triangles;

public:
  kgmShapeCollision();
  virtual ~kgmShapeCollision();
};

#endif // KGMSHAPECOLLISION_H
