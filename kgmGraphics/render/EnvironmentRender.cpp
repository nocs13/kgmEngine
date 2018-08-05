#include "EnvironmentRender.h"
#include "../kgmCamera.h"
#include "../kgmGraphics.h"

EnvironmentRender::EnvironmentRender(kgmGraphics* g)
  :BaseRender(g)
{
  m_target = gc->gcGenTarget(512, 512, true, false);
}

EnvironmentRender::~EnvironmentRender()
{
  gc->gcFreeTarget(m_target);
}

void EnvironmentRender::render()
{
}

void EnvironmentRender::render(vec3 pos, box bnd, gchandle tex)
{
  kgmCamera cam;

  f32 shift = 0.5 * bnd.min.distance(bnd.max);

  kgmGraphics::Options o;

  for (u32 i = 0; i < 6; i++)
  {
    prepare(cam, pos, shift, i);
    gc->gcTexTarget(m_target, tex, gctype_texcube);
    gr->render(m_target, cam, o);
  }
}

void EnvironmentRender::prepare(kgmCamera& cam, vec3 pos, f32 shift, u32 face)
{
  vec3 cpos, cdir;

  switch(face)
  {
  case 0:
    cpos = pos;
    cpos.x += shift;
    cdir = vec3(1, 0, 0);
    break;
  case 1:
    cpos = pos;
    cpos.x -= shift;
    cdir = vec3(-1, 0, 0);
    break;
  case 2:
    cpos = pos;
    cpos.y += shift;
    cdir = vec3(0, 1, 0);
    break;
  case 3:
    cpos = pos;
    cpos.y -= shift;
    cdir = vec3(0, -1, 0);
    break;
  case 4:
    cpos = pos;
    cpos.z += shift;
    cdir = vec3(0, 0, 1);
    break;
  case 5:
    cpos = pos;
    cpos.z -= shift;
    cdir = vec3(0, 0, -1);
    break;
  }

  cam.set(PI / 6, 1.0, 0.1, 1000, cpos, cdir, vec3(0, 0, 1));
}
