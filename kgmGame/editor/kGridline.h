#ifndef KGMGRIDLINE_H
#define KGMGRIDLINE_H

#include "../../kgmGraphics/kgmMesh.h"

namespace kgmGameEditor
{

class kGridline: public kgmMesh
{
public:
  kGridline(u8 count = 10, f32 space = 1000.0f, u32 color = 0xff888888);

  ~kGridline();
};

}

#endif // KGMGRIDLINE_H
