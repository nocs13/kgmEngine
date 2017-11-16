#ifndef KGMTOOLS_H
#define KGMTOOLS_H

#include "kgmMesh.h"

namespace kgmGraphics
{

class Tools
{
private:
  Tools(){}

public:
  static kgmMesh* genPlane(f32 w, f32 l);
  static kgmMesh* genBox(f32 w, f32 l, f32 h);
  static kgmMesh* genSphere(f32 r, f32 s);
  static kgmMesh* genCylinder(f32 r, f32 h, f32 s);
};

}

#endif // KGMTOOLS_H
