#ifndef KLINE_H
#define KLINE_H

#include "../../kgmGraphics/kgmMesh.h"

class kLine : public kgmMesh
{
  Vertex_P_C* v;

public:
  kLine(u32 color = 0xff7777bb);
  ~kLine();

  void set(vec3 v1, vec3 v2);
};

#endif // KLINE_H
