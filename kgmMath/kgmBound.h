#pragma once

//Bound of 3D objects.

template <class T> class kgmBound3d
{
  kgmVector3d<T> min, max;

public:
  kgmBound3d()
  {
    min = max = kgmVector3d<T>(0, 0, 0);
  }

  kgmBound3d(kgmVector3d<T> *pt, u32 count)
  {
    if (count < 1)
      return;

    min = max = pt[0];

    for (u32 i = 1; i < count; i++)
    {
      if (pt[i].x < min.x)
	min.x = pt[i].x;
      if (pt[i].y < min.y)
	min.y = pt[i].y;
      if (pt[i].x > max.x)
	max.x = pt[i].x;
      if (pt[i].y > max.y)
	max.y = pt[i].y;
    }
  }

  kgmBox3d<T> box()
  {
    return kgmBox3d<T>(min, max);
  }

  kgmSphere3d<T> sphere()
  {
    T radius = (T) 0.5 * min.distance(max);
    
    kgmVector3d<T> center = (min + max) * 0.5;

    return kgmSphere3d<T>(center, radius);
  }
};
