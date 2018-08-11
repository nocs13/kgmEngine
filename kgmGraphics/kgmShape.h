#ifndef KGMSHAPE_H
#define KGMSHAPE_H

#include "kgmMesh.h"

class kgmShape: public kgmMesh
{
  KGM_OBJECT(kgmShape);

public:
  kgmShape();
  kgmShape(f32 w, f32 l);
  kgmShape(f32 w, f32 l, f32 h);
  kgmShape(f32 r, u32 s);
  kgmShape(f32 r, f32 h, u32 s);

private:
  Vertex* vAlloc(u32 count, FVF f = FVF_P_N_C);
  Face*   fAlloc(u32 count, FFF f = FFF_16);
};

#endif // KGMSHAPE_H
