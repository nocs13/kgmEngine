#pragma once
#include "../kgmPhysics/kgmPhysics.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"

#include "../kgmMath/kgmTriangle3d.h"


class kgmGamePhysics: public kgmPhysics{
public:
 kgmGamePhysics();
 ~kgmGamePhysics();

 void update(u32 ms);
 void collision(kgmBody*, kgmBody*);
};
