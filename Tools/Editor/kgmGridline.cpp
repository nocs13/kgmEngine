#include "kgmGridline.h"

kgmGridline::kgmGridline(u8 count, f32 space, u32 color)
{
  Vertex_P_C* v = (Vertex_P_C*)vAlloc(4 * (count + 2), FVF_P_C);

  f32 step = space / count;
  f32 min  = -0.5f * space;

  for(int i = 0; i < (count + 1); i++)
  {
    f32 start = min + step * i;
    f32 end   = min + step * (i + 1);

    v[i * 4 + 0].pos = vec3(start, start, 0);
    v[i * 4 + 1].pos = vec3(start, end,   0);
    v[i * 4 + 2].pos = vec3(start, start, 0);
    v[i * 4 + 3].pos = vec3(end,   start, 0);

    v[i * 4 + 0].col = color;
    v[i * 4 + 1].col = color;
    v[i * 4 + 2].col = color;
    v[i * 4 + 3].col = color;
  }
}
