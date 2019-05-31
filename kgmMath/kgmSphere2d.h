#pragma once
#include "kgmBox2d.h"

template <class T> class kgmSphere2d{
public:
  kgmVector2d<T> center;
  T            radius;
public:
  kgmSphere2d()
  {
    radius = 0;
  }

  kgmSphere2d(kgmVector2d<T>& v, T r)
  {
    center = v;
    radius = r;
  }

  kgmSphere2d(kgmBox2d<T>& b)
  {
    kgmVector2d<T> d = b.max - b.min;
    center.x = 0.5 * (b.min.x + b.max.x);
    center.y = 0.5 * (b.min.y + b.max.y);
    radius = 0.5 * d.length();
  }

  kgmSphere2d(kgmVector2d<T>* points, int count)
  {
    kgmVector2d<T> min,
                   max;

    min = max = points[0];

    center = min;

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
    }

    center.x /= count;
    center.y /= count;

    kgmVector2d<T> d = max - min;

    radius = 0.5 * d.length();
  }

  bool isin(kgmVector2d<T>& v)
  {
    kgmVector2d<T> d = v - center;

    if(d.length() <= radius)
      return true;

    return false;
  }

  T distance(kgmVector2d<T>& v)
  {
    kgmVector2d<T> d = v - center;

    return d.length() - radius;
  }
};
