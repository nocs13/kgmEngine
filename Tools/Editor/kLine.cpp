#include "kLine.h"
#include "../../kgmGraphics/kgmColor.h"

kLine::kLine(u32 color)
{
  v = (Vertex_P_C*)vAlloc(2, FVF_P_C);

  v[0].pos = vec3(0, 0, 0);
  v[1].pos = vec3(0, 0, 0);

  v[0].col = v[1].col = kgmColor::toVector(color);

  m_rtype = kgmMesh::RT_LINE;
}

kLine::~kLine()
{
}

void kLine::set(vec3 v1, vec3 v2)
{
  v[0].pos = v1;
  v[1].pos = v2;
}
