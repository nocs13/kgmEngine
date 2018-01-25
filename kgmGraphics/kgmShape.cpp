#include "kgmShape.h"

kgmShape::kgmShape()
{

}

kgmShape::kgmShape(f32 w, f32 l)
{
  kgmMesh::Vertex_P_N_T* v = (kgmMesh::Vertex_P_N_T*) vAlloc(4, kgmMesh::FVF_P_N_T);

  v[0].pos = vec3(-.5f * w,  .5f * l, 0.0f), v[0].nor = vec3(0, 0, 1), v[0].uv = vec2(0, 0);
  v[1].pos = vec3(-.5f * w, -.5f * l, 0.0f), v[1].nor = vec3(0, 0, 1), v[1].uv = vec2(0, 1);
  v[2].pos = vec3( .5f * w,  .5f * l, 0.0f), v[2].nor = vec3(0, 0, 1), v[2].uv = vec2(1, 0);
  v[3].pos = vec3( .5f * w, -.5f * l, 0.0f), v[3].nor = vec3(0, 0, 1), v[3].uv = vec2(1, 1);

  kgmMesh::Face_16* f = (kgmMesh::Face_16*) fAlloc(2);

  f[0].a = 0, f[0].b = 1, f[0].c = 2;
  f[0].a = 3, f[0].b = 2, f[0].c = 1;

  m_rtype = RT_TRIANGLESTRIP;

  rebound();
}

kgmShape::kgmShape(f32 w, f32 l, f32 h)
{
  kgmMesh::Vertex_P_N_T* v = (kgmMesh::Vertex_P_N_T*) vAlloc(8, kgmMesh::FVF_P_N_T);

  v[0].pos = vec3(-.5f * w,  .5f * l, -.5f * h), v[0].nor = vec3(0, 0, -1), v[0].uv = vec2(0, 0);
  v[1].pos = vec3(-.5f * w, -.5f * l, -.5f * h), v[1].nor = vec3(0, 0, -1), v[1].uv = vec2(0, 1);
  v[2].pos = vec3( .5f * w,  .5f * l, -.5f * h), v[2].nor = vec3(0, 0, -1), v[2].uv = vec2(1, 0);
  v[3].pos = vec3( .5f * w, -.5f * l, -.5f * h), v[3].nor = vec3(0, 0, -1), v[3].uv = vec2(1, 1);

  v[4].pos = vec3(-.5f * w,  .5f * l, .5f * h), v[4].nor = vec3(0, 0, 1), v[4].uv = vec2(0, 0);
  v[5].pos = vec3(-.5f * w, -.5f * l, .5f * h), v[5].nor = vec3(0, 0, 1), v[5].uv = vec2(0, 1);
  v[6].pos = vec3( .5f * w,  .5f * l, .5f * h), v[6].nor = vec3(0, 0, 1), v[6].uv = vec2(1, 0);
  v[7].pos = vec3( .5f * w, -.5f * l, .5f * h), v[7].nor = vec3(0, 0, 1), v[7].uv = vec2(1, 1);

  kgmMesh::Face_16* f = (kgmMesh::Face_16*) fAlloc(12);

  f[0].a = 2, f[0].b = 1, f[0].c = 0;
  f[1].a = 3, f[1].b = 1, f[1].c = 2;

  f[2].a = 4, f[2].b = 5, f[2].c = 6;
  f[3].a = 7, f[3].b = 6, f[3].c = 5;

  f[4].a = 0, f[2].b = 5, f[2].c = 4;
  f[5].a = 0, f[3].b = 1, f[3].c = 5;

  f[6].a = 1, f[2].b = 7, f[2].c = 5;
  f[7].a = 1, f[3].b = 3, f[3].c = 7;

  f[8].a = 3, f[2].b = 6, f[2].c = 7;
  f[9].a = 3, f[3].b = 2, f[3].c = 6;

  f[10].a = 2, f[2].b = 4, f[2].c = 6;
  f[11].a = 2, f[3].b = 0, f[3].c = 4;

  m_rtype = RT_TRIANGLE;

  rebound();
}

kgmMesh::Vertex* kgmShape::vAlloc(u32 count, FVF f)
{
  return kgmMesh::vAlloc(count, f);
}

kgmMesh::Face*   kgmShape::fAlloc(u32 count, FFF f)
{
  return kgmMesh::fAlloc(count, f);
}
