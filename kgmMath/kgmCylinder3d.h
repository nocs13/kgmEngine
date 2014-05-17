#pragma once
#include "kgmVector3d.h"

template <class T> class kgmCylinder3d
{
public:
 kgmVector3d<T> position;
 T            radius, height;

public:
 kgmCylinder3d()
 {
  radius = height = 0;
 }

 kgmCylinder3d(kgmVector3d<T>& pos, T rad, T h)
 {
  position = pos, radius = rad, height = h;
 }

 bool isInside(kgmVector3d<T>& v)
 {
  kgmVector3d<T> v_proj = v; 
  v_proj.z = position.z;
  T v_dist = position.distance(v_proj);

  if(v_dist > radius)
   return false;

  if((v.z < position.z) || (v.z > (position.z + height)))
   return false;

  return true;
 }
};
