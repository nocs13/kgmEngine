#include "kgmTools.h"

namespace kgmGraphics
{

kgmMesh* Tools::genPlane(f32 w, f32 l)
{
  kgmMesh* mesh = new kgmMesh();

  kgmMesh::Vertex_P_N_T* v = (kgmMesh::Vertex_P_N_T*) mesh->vAlloc(4, kgmMesh::FVF_P_N_T);

  v[0].pos = vec3(-.5f,  .5f, 0.0f), v[0].nor = vec3(0, 0, 1), v[0].uv = vec2(0, 0);
  v[1].pos = vec3(-.5f, -.5f, 0.0f), v[0].nor = vec3(0, 0, 1), v[0].uv = vec2(0, 1);
  v[0].pos = vec3( .5f,  .5f, 0.0f), v[0].nor = vec3(0, 0, 1), v[0].uv = vec2(1, 0);
  v[0].pos = vec3( .5f, -.5f, 0.0f), v[0].nor = vec3(0, 0, 1), v[0].uv = vec2(1, 1);
  return mesh;
}

}

