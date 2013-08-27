#pragma once

//TRIANGLE 3D
template <class T> class kgmTriangle3d{
public:
 kgmVector3d<T> a, b, c;
public:
 kgmTriangle3d(){
 }
 kgmTriangle3d(kgmVector3d<T> &aa, kgmVector3d<T> &bb, kgmVector3d<T> &cc){
  a = aa, b = bb, c = cc;
 }

 bool isin(kgmVector3d<T> v){
  kgmVector3d<T> v0, v1, v2;
  T a0, a1, a2, an;
  v0 = a - v;
  v1 = b - v;
  v2 = c - v;
  a0 = v0.angle(v1);
  a1 = v1.angle(v2);
  a2 = v2.angle(v0);
  an = a0 + a1 + a2;

  if(an > (0.9f * 2.0f * PI))
   return true;

  return false;
 }

 bool intersect(kgmVector3d<T> v, float r){
  return false;
 }
};
