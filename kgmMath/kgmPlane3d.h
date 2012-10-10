#pragma once

//PLANE 3D
template <class T> class kgmPlane3d{
public:
 T x, y, z, w;
public:
 kgmPlane3d(){
  x = y = z = w = 0;
 }
 kgmPlane3d(T a, T b, T c, T d){
  x = a, y = b, z = c, w = d;
 }
 kgmPlane3d(const kgmVector3d<T>& n, const kgmVector3d<T>& v){
  w = -(n.x * v.x + n.y * v.y + n.z * v.z);
  x = n.x, y = n.y, z = n.z;
 }
 kgmPlane3d(const kgmVector3d<T> &a, const kgmVector3d<T> &b, const kgmVector3d<T> &c){
  kgmVector3d<T> v1, v2, nor;
  v1 = b - a;
  v2 = c - a; //c - b;
  nor = v1.cross(v2);
//  nor.normalize();
  w = -(nor.x*a.x + nor.y*a.y + nor.z*a.z);
  x = nor.x, y = nor.y, z = nor.z;
 }
 kgmVector3d<T> normal(){
  kgmVector3d<T> n(x, y, z);
  n.normalize();
  return n;
 }
 T distance(kgmVector3d<T> v){
  T mr = (x*v.x + y*v.y + z*v.z + w);
  T mn = (T)sqrt(x*x + y*y + z*z);
  if(mn == 0){
    return 1;
  }
  return  (mr / mn);
 }
 kgmVector3d<T> projection(kgmVector3d<T> v){
  T dist = distance(v);
  kgmVector3d<T> pn(x, y, z);
  pn.normalize();
  return kgmVector3d<T>(v.x - dist * pn.x, v.y - dist * pn.y, v.z - dist * pn.z);
 }
 bool intersect(kgmLine3d<T> &l, kgmVector3d<T> &v){
  kgmVector3d<T> dl = l.e - l.s;
  T t, numer, denom;

  numer = -(x * l.s.x + y * l.s.y + z * l.s.z + w);
  denom = (x * dl.x + y * dl.y + z * dl.z);
  if(denom == 0)
   return false;

  t = numer / denom;
  if((t < 0.0) || (t > 1.0))
   return false;

  v.x = l.s.x + t * dl.x;
   v.y = l.s.y + t * dl.y;
    v.z = l.s.z + t * dl.z;
  return true;
 }
 bool intersect(kgmRay3d<T> &r, kgmVector3d<T> &v){
  T numer, denom, dist; 
  numer = r.s.x*x + r.s.y*y + r.s.z*z + w;
  denom = r.d.x*x + r.d.y*y + r.d.z*z;
  if(denom == 0)
	 return false;

  dist = -(numer / denom);
  if(dist < 0)
	 return false;

  v.x = r.s.x + dist * r.d.x;
   v.y = r.s.y + dist * r.d.y;
    v.z = r.s.z + dist * r.d.z;
  return true;
 }
};

typedef kgmPlane3d<float> plane;