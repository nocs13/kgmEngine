#pragma once
#include "kgmVector3d.h"

template <class T> class kgmLine3d{
public:
 kgmVector3d<T> s, e;
public:
 kgmLine3d()
 {
 }

 kgmLine3d(kgmVector3d<T> &a, kgmVector3d<T> &b)
 {
  s = a, e = b;
 }

 void set(kgmVector3d<T> &a, kgmVector3d<T> &b)
 {
  s = a, e = b;
 }

 T length()
 {
  kgmVector3d<T> v = e - s;

  return v.length();
 }

 bool ison(kgmVector3d<T>& v, T presision = T(0.01))
 {
  kgmVector3d<T> vT, vTV;
  kgmVector3d<T> vL = e - s;

  T t = T(0);

  if(vL.x != 0)
    t = (v.x - s.x) / vL.x;
  else if(vL.y != 0)
    t = (v.y - s.y) / vL.y;
  else if(vL.z != 0)
    t = (v.z - s.z) / vL.z;

  if((t < 0.0) || (t > 1.0))
    return false;

  vT.x = s.x + vL.x * t;
  vT.y = s.y + vL.y * t;
  vT.z = s.z + vL.z * t;
  vTV = vT - v;

  if(vTV.length() < presision)
    return true;

  return false;
 }

 kgmVector3d<T> projection(kgmVector3d<T>& v)
 {
  kgmVector3d<T> vL = e - s;
  kgmVector3d<T> vV = v - s;

  T lL = vL.length();
  T lV = vV.length();
  vL.normalize();
  vV.normalize();
  T dot = vL.dot(vV);
  kgmVector3d<T> vPr = s + vL * (lV * dot);

  return vPr;
 }
};
