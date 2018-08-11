#ifndef KGMOBSTACLE_H
#define KGMOBSTACLE_H

#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmObstacle : public kgmObject
{
  KGM_OBJECT(kgmObstacle);

  kgmList<triangle3> triangles;

  mtx4 transform;

  box3 bound;

  f32  scale;

public:
  kgmObstacle();
  ~kgmObstacle();

  void set(mtx4& m)
  {
    transform = m;
  }

  void set(float s)
  {
    scale = s;
  }

  void add(vec3& a, vec3& b, vec3& c)
  {
    triangle3 trn(a, b, c);

    triangles.add(trn);

    if(triangles.length() > 1)
    {
      bound.extend(a);
      bound.extend(b);
      bound.extend(c);
    }
    else
    {
      bound = box3(&a, 1);
      bound.extend(b);
      bound.extend(c);
    }
  }

  u32 length()
  {
    return triangles.length();
  }

  triangle3 get(u32 i)
  {
    triangle3 t = triangles[i];

    t.pt[0] = transform * t.pt[0] * scale;
    t.pt[1] = transform * t.pt[1] * scale;
    t.pt[2] = transform * t.pt[2] * scale;

    return t;
  }

  float getScale()
  {
    return scale;
  }

  mtx4 getTransfom() const
  {
    return transform;
  }

  triangle3 getTriangle(u32 i)
  {
    return triangles[i];
  }

  void fromBox(vec3 min, vec3 max);
  void fromRect(vec2 min, vec2 max);
};

#endif // KGMOBSTACLE_H
