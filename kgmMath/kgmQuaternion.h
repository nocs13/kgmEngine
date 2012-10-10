//QUATERNION
#pragma once

template <class T> class kgmQuaternion{
public:
 T x, y, z, w;
public:
 kgmQuaternion(){
  x = y = z = 0, w = 1;
 }
 kgmQuaternion(T a, T b, T c, T d){
  x = a, y = b, z = c, w = d;
 }
 kgmQuaternion(kgmVector3d<T> &v, T a){
  T l = v.length();
  if(l != 0)
  {
   l = 1 / l;
   T sina = (T)sin(a * 0.5f);
   x = v.x * l * sina,
   y = v.y * l * sina,
   z = v.z * l * sina,
   w = (T)cos(a * 0.5f);
  }
  else{
	 x = y = z = 0, w = 1;
  }
 }

 kgmQuaternion<T> operator+(const kgmQuaternion<T>& q) const{
  return kgmQuaternion<T>(x + q.x, y + q.y, z + q.z, w + q.w);
 }
 kgmQuaternion<T> operator*(T v) const{
  return kgmQuaternion<T>(x * v, y * v, z * v, w * v);
 }
 kgmQuaternion<T> operator*(kgmQuaternion<T>& q) const{
  kgmQuaternion<T> r;
  r.x = w * q.x + x * q.x + y * q.z - z * q.y;
  r.y = w * q.y + y * q.w + z * q.x - x * q.z;
  r.z = w * q.z + z * q.w + x * q.y - y * q.x;
  r.w = w * q.w - x * q.x - y * q.y - z * q.z;
  return r;
 }


 void euler(kgmVector3d<T> &r){
  kgmVector3d<T> vx(1, 0, 0), vy(0, 1, 0), vz(0, 0, 1);
  kgmQuaternion<T> qx(vx, r.x), qy(vy, r.y), qz(vz, r.z);
  *this = qz * qy * qx;
  /*
        float cr, cp, cy, sr, sp, sy, cpcy, spsy;  // calculate trig identities
        cr = cos(roll/2);
        cp = cos(pitch/2);
        cy = cos(yaw/2);
        sr = sin(roll/2);
        sp = sin(pitch/2);
        sy = sin(yaw/2);
        cpcy = cp * cy;
        spsy = sp * sy;
        w = cr * cpcy + sr * spsy;
        x = sr * cpcy - cr * spsy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;
  */
 }
 kgmQuaternion<T> slerp(kgmQuaternion<T> &q, T t){
  kgmQuaternion b, r;
  T k0, k1, 
	   cos_om = x*q.x + y*q.y + z*q.z + w*q.w;

  if(cos_om < 0.0f){
   cos_om = -cos_om;
   b.x = -q.x;
   b.y = -q.y;
   b.z = -q.z;
   b.w = -q.w;
  }
  else
   b = q;

  if(1.0f - cos_om > 1e-6){
   T om = (T)acos(cos_om);
   T sin_om = (T)sin(om);
   k0 = (T)sin((1.0f - t) * om) / sin_om;
   k1 = (T)sin(t * om) / sin_om;
  }
  else{
   k0 = 1 - t;
   k1 = t;
  }

  r.x = x*k0 + b.x*k1;
  r.y = y*k0 + b.y*k1;
  r.z = z*k0 + b.z*k1;
  r.w = w*k0 + b.w*k1;

  return r;
 }
};

