#pragma once
#include "../kgmPhysics/kgmPhysics.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"

#include "../kgmMath/kgmTriangle3d.h"

#include <vector>


class kgmGamePhysics: public kgmPhysics{
public:
 kgmGamePhysics();
 ~kgmGamePhysics();

 void update(float);
 void collision(kgmBody*, kgmBody*);
};
