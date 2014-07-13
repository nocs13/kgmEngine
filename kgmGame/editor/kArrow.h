#ifndef KARROW_H
#define KARROW_H

#include "../../kgmGraphics/kgmMesh.h"

namespace kgmGameEditor
{

class kArrow : public kgmMesh
{
public:
  kArrow(f32 length = 1.0, u32 color = 0xff7777bb);
};

};

#endif // KARROW_H
