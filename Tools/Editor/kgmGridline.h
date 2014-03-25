#ifndef KGMGRIDLINE_H
#define KGMGRIDLINE_H

#include "../../kgmGraphics/kgmMesh.h"

class kgmGridline: public kgmMesh
{
public:
  kgmGridline(u8 count = 10, f32 space = 1000.0f, u32 color = 0xffaaaaaa);
};

#endif // KGMGRIDLINE_H
