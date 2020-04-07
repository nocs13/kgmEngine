#include "kArrow.h"
#include "../../kgmGraphics/kgmColor.h"

using namespace kgmGameEditor;

kArrow::kArrow(f32 length, u32 color)
{
  float al      = length * 0.1;

  Vertex_P_FC* v = (Vertex_P_FC*)vAlloc(10, FVF_P_FC);

  v[0].pos = vec3(0, 0, 0);
  v[1].pos = vec3(length, 0, 0);

  v[2].pos = vec3(length, 0, 0);
  v[3].pos = vec3(length - al, al, 0);

  v[4].pos = vec3(length, 0, 0);
  v[5].pos = vec3(length - al, -al, 0);

  v[6].pos = vec3(length, 0, 0);
  v[7].pos = vec3(length - al, 0, al);

  v[8].pos = vec3(length, 0, 0);
  v[9].pos = vec3(length - al, 0, -al);

  v[0].col = v[1].col = v[2].col =
  v[3].col = v[4].col = v[5].col =
  v[6].col = v[7].col = v[8].col =
  v[9].col = kgmColor::toVector(color);

  m_rtype = kgmMesh::RT_LINE;
}
