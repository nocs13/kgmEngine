#ifndef KGMOBSTACLE_H
#define KGMOBSTACLE_H

#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmObstacle : public kgmObject
{
  KGM_OBJECT(kgmObstacle);

  kgmList<triangle3> triangles;

  mtx4 transform;

public:
  kgmObstacle();
  ~kgmObstacle();

  void set(mtx4& m)
  {
    transform = m;
  }

  void add(vec3& a, vec3& b, vec3& c)
  {
    triangle3 trn(a, b, c);

    triangles.add(trn);
  }

  u32 length()
  {
    return triangles.length();
  }

  triangle3 get(u32 i)
  {
    triangle3 t = triangles[i];

    t.pt[0] = transform * t.pt[0];
    t.pt[1] = transform * t.pt[1];
    t.pt[2] = transform * t.pt[2];

    return t;
  }
};

#endif // KGMOBSTACLE_H
