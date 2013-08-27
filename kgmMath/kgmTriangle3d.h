#pragma once

//TRIANGLE 3D
template <class T> class kgmTriangle3d{
public:
  kgmVector3d<T> pt[3];
  kgmPlane3d<T> plane;
public:
  kgmTriangle3d(){
  }
  kgmTriangle3d(kgmVector3d<T> &aa, kgmVector3d<T> &bb, kgmVector3d<T> &cc){
    pt[0] = aa, pt[1] = bb, pt[2] = cc;
    plane = kgmPlane3d<T>(aa, bb, cc);
  }

  bool isin(kgmVector3d<T> v){
    kgmVector3d<T> v0, v1, v2;
    T a0, a1, a2, an;

    v0 = pt[0] - v;
    v1 = pt[1] - v;
    v2 = pt[2] - v;
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
