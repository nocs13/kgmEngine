#include "kPivot.h"
#include "../../kgmBase/kgmLog.h"

using namespace kgmGameEditor;

kPivot::kPivot()
{
  Vertex_P_C* v = (Vertex_P_C*)vAlloc(6, FVF_P_C);

  v[0].pos = vec3(0, 0, -0.1);
  v[1].pos = vec3(1, 0, 0);

  v[2].pos = vec3(0, 0, -0.1);
  v[3].pos = vec3(0, 1, 0);

  v[4].pos = vec3(0, 0, -0.1);
  v[5].pos = vec3(0, 0, 1);

  v[0].col = 0xff0000ff;
  v[1].col = 0xff0000ff;

  v[2].col = 0xff00ff00;
  v[3].col = 0xff00ff00;

  v[4].col = 0xffff0000;
  v[5].col = 0xffff0000;

  m_rtype = kgmMesh::RT_LINE;

  pos = vec3(0, 0, 0);
  rot = vec3(0, 0, 0);

  axis = AXIS_NONE;
}

u32 kPivot::peekAxis(ray3 r)
{
  mtx4 mtr, mrt, mmv;
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

  //kgm_log() << "Line S: " << v[0].x << " " << v[0].y << " " << v[0].z << "\n";
  //kgm_log() << "Line E: " << v[1].x << " " << v[1].y << " " << v[1].z << "\n";
  //kgm_log() << "Line E: " << v[2].x << " " << v[2].y << " " << v[2].z << "\n";

  if(pln.intersect(r, vins))
  {
    line3 l(v[0], v[1]);

    //kgm_log() << "\nZ Ins: " << vins.x << " " << vins.y << " " << vins.z << "\n";

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_X;
      vprj = vins;
      //kgm_log() << "AXIS X: selected\n";
      return axis;
    }

    l.set(v[0], v[2]);

    if(l.ison(vins, 0.1))
    {
      axis = AXIS_Y;
      vprj = vins;
      //kgm_log() << "AXIS Y: selected\n";
      return axis;
    }
  }

  vnor = v[1] - v[0];
  vnor.normalize();

  pln = plane3(vnor, v[0]);

  if(pln.intersect(r, vins))
  {
    //kgm_log() << "\nX Ins: " << vins.x << " " << vins.y << " " << vins.z << "\n";

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
      //kgm_log() << "AXIS Z: selected\n";
      return axis;
    }
  }

  vnor = v[2] - v[0];
  vnor.normalize();

  pln = plane3(vnor, v[0]);

  if(pln.intersect(r, vins))
  {
    //kgm_log() << "\nY Ins: " << vins.x << " " << vins.y << " " << vins.z << "\n";

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
      //kgm_log() << "AXIS X: selected\n";
      return axis;
    }
  }

  axis = AXIS_NONE;

  return AXIS_NONE;
}

mtx4 kPivot::getTransform()
{
  mtx4 m;
  m.identity();
  m.translate(pos);

  return m;
}
