#include "kPivot.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmGraphics/kgmColor.h"

kPivot::kPivot()
{
  Vertex_P_C* v = (Vertex_P_C*) vAlloc(6, FVF_P_C);

  v[0].pos = vec3(0, 0, 0);
  v[1].pos = vec3(1, 0, 0);

  v[2].pos = vec3(0, 0, 0);
  v[3].pos = vec3(0, 1, 0);

  v[4].pos = vec3(0, 0, 0);
  v[5].pos = vec3(0, 0, 1);

  v[0].col =
  v[1].col = kgmColor::toVector(0x000000ff);

  v[2].col =
  v[3].col = kgmColor::toVector(0x0000ff00);

  v[4].col =
  v[5].col = kgmColor::toVector(0x00ff0000);

  m_rtype = kgmMesh::RT_LINE;

  axis = AXIS_NONE;
}

u32 kPivot::peekAxis(ray3 r)
{
  mtx4 mtr, mrt, mmv;
  vec3 pos(0, 0, 0), rot(0, 0, 0);
  
  mmv.translate(pos);
  mrt.rotate(rot);
  mtr = mrt * mmv;

  vec3 v[4], vpt;

  vpt.set(0, 0, 0);
  v[0] = mtr * vpt;
  vpt.set(1, 0, 0);
  v[1] = mtr * vpt;
  vpt.set(0, 1, 0);
  v[2] = mtr * vpt;
  vpt.set(0, 0, 1);
  v[3] = mtr * vpt;

  vec3 vnor, vins;
  plane3 pln;

  vnor = v[3] - v[0];
  vnor.normalize();

  pln = plane3(vnor, v[0]);

  if(pln.intersect(r, vins))
  {
    line3 l(v[0], v[1]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_X;
      vprj = vins;

      return axis;
    }

    l.set(v[0], v[2]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_Y;
      vprj = vins;

      return axis;
    }
  }

  vnor = v[1] - v[0];
  vnor.normalize();

  pln = plane3(vnor, v[0]);

  if(pln.intersect(r, vins))
  {
    line3 l(v[0], v[2]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_Y;
      vprj = vins;

      return axis;
    }

    l.set(v[0], v[3]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_Z;
      vprj = vins;

      return axis;
    }
  }

  vnor = v[2] - v[0];
  vnor.normalize();

  pln = plane3(vnor, v[0]);

  if(pln.intersect(r, vins))
  {
    line3 l(v[0], v[3]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_Z;
      vprj = vins;

      return axis;
    }

    l.set(v[0], v[1]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_X;
      vprj = vins;

      return axis;
    }
  }

  axis = AXIS_NONE;

  return AXIS_NONE;
}
