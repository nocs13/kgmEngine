#pragma once
#include "kgmBox3d.h"

template <class T> class kgmSphere3d{
public:
  kgmVector3d<T> center;
  T            radius;
public:
  kgmSphere3d()
  {
    radius = 0;
  }

  kgmSphere3d(kgmVector3d<T>& v, T r)
  {
    center = v;
    radius = r;
  }

  kgmSphere3d(kgmBox3d<T>& b)\
  {
    kgmVector3d<T> d = b.max - b.min;
    center.x = (b.min.x + b.max.x) / 2;
    center.y = (b.min.y + b.max.y) / 2;
    center.z = (b.min.z + b.max.z) / 2;
    radius = d.length() / 2;
  }

  kgmSphere3d(kgmVector3d<T>* points, int count)
  {
    kgmVector3d<T> min,
                   max;

    min = max = points[0];
    center = kgmVector3d<T>(0, 0, 0);

    for(int i = 1; i < count; i++)
    {
      center = center + points[i];

      if(min.x > points[i].x)
        min.x = points[i].x;
      else if(max.x < points[i].x)
        max.x = points[i].x;

      if(min.y > points[i].y)
        min.y = points[i].y;
      else if(max.y < points[i].y)
        max.y = points[i].y;

      if(min.z > points[i].z)
        min.z = points[i].z;
      else if(max.z < points[i].z)
        max.z = points[i].z;
    }

    center.x /= count;
    center.y /= count;
    center.z /= count;

    kgmVector3d<T> d = max - min;

    radius = 0.5 * d.length();
  }

  bool isin(kgmVector3d<T>& v)
  {
    kgmVector3d<T> d = v - center;

    if(d.length() <= radius)
      return true;

    return false;
  }
};
