#include "kGridline.h"

using namespace kgmGameEditor;

kGridline::kGridline(u8 count, f32 space, u32 color)
{
  Vertex_P_C* v = (Vertex_P_C*)vAlloc(4 * count, FVF_P_C);

  f32 step = space / count;
  f32 min  = -0.5f * space;
  f32 max  =  0.5f * space;

  for(int i = 0; i < count; i++)
  {
    f32 start = min + step * i;

    v[i * 4 + 0].pos = vec3(start, min,   0);
    v[i * 4 + 1].pos = vec3(start, max,   0);
    v[i * 4 + 2].pos = vec3(min,   start, 0);
    v[i * 4 + 3].pos = vec3(max,   start, 0);

    v[i * 4 + 0].col = color;
    v[i * 4 + 1].col = color;
    v[i * 4 + 2].col = color;
    v[i * 4 + 3].col = color;
  }

  m_rtype = kgmMesh::RT_LINE;

  m_color = color;
}

kGridline::~kGridline()
{

}
