#pragma once
#include "../kgmPhysics/kgmPhysics.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"

#include "../kgmMath/kgmTriangle3d.h"


class kgmGamePhysics: public kgmPhysics{
public:
 kgmGamePhysics();
 ~kgmGamePhysics();

 void update();
 void collision(kgmBody*, kgmBody*);
};
