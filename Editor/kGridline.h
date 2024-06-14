#ifndef KGMGRIDLINE_H
#define KGMGRIDLINE_H

#include "../kgmGraphics/kgmMesh.h"

class kGridline: public kgmMesh
{
  u32 m_color;

public:
  kGridline(u8 count = 10, f32 space = 1000.0f, u32 color = 0xff888888);

  ~kGridline();

  u32 color()
  {
    return m_color;
  }
};

#endif // KGMGRIDLINE_H
