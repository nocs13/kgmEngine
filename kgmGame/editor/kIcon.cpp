#include "kIcon.h"

using namespace kgmGameEditor;

kIcon::kIcon()
{
  Vertex_P_C* v = (Vertex_P_C*)vAlloc(6, FVF_P_C);

  v[0].pos = vec3(-1, 0, 0);
  v[1].pos = vec3(1,  0, 0);
  v[2].pos = vec3(1,  0, 1);

  v[3].pos = vec3(-1, 0, -1);
  v[4].pos = vec3(-1, 0, 0);
  v[5].pos = vec3(1, 0, 1);

  v[0].col = kgmColor::toVector(0xff0000ff);
  v[1].col = kgmColor::toVector(0xff0000ff);

  v[2].col = kgmColor::toVector(0xff00ff00);
  v[3].col = kgmColor::toVector(0xff00ff00);

  v[4].col = kgmColor::toVector(0xffff0000);
  v[5].col = kgmColor::toVector(0xffff0000);

  m_rtype = kgmMesh::RT_TRIANGLE;
}
