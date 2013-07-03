#pragma once

// VECTOR 3D
template <class T> class kgmVector3d{
public:
  T x, y, z;
public:
  kgmVector3d(){
    x = y = z = 0;
  }
  kgmVector3d(T a, T b, T c){
    x = a, y = b, z = c;
  }
  kgmVector3d(T* v){
    x = v[0], y = v[1], z = v[2];
  }
  kgmVector3d(const kgmVector3d<T>& v){
    x = v.x, y = v.y, z = v.z;
  }


  kgmVector3d<T> operator+(const kgmVector3d<T>& v) const{
    return kgmVector3d<T>(x + v.x, y + v.y, z + v.z);
  }

  kgmVector3d<T> operator+=(const kgmVector3d<T>& v) const{
    x += v.x, y += v.y, z += v.z;
    return *this;
  }

  kgmVector3d<T> operator-(const kgmVector3d<T>& v) const{
    return kgmVector3d<T>(x - v.x, y - v.y, z - v.z);
  }

  kgmVector3d<T> operator-=(const kgmVector3d<T>& v) const{
    x -= v.x, y -= v.y, z -= v.z;
    return *this;
  }

  kgmVector3d<T> operator*(T v) const{
    return kgmVector3d<T>(x * v, y * v, z * v);
  }

  kgmVector3d<T> operator*=(T v) const{
    x *= v, y *= v, z *= v;
    return *this;
  }

  bool operator==(const kgmVector3d<T>& v){
    return ((x == v.x) && (y == v.y) && (z == v.z));
  }

  void set(T a, T b, T c){
   x = a, y = b, z = c;
  }

  T length(){
    return (T)sqrt(x*x + y*y + z*z);
  }

  kgmVector3d<T> inverse(){
    return kgmVector3d<T>(-x, -y, -z);
  }

  void invert(){
    x = -x, y = -y, z = -z;
  }

  void normalize(){
    T l = length();
    if(l != 0.0){
      x /= l, y /= l, z /= l;
    }
  }

  kgmVector3d<T> normal(){
    kgmVector3d<T> v;
    T l = length();
    if(l != 0.0){
      v.x = x / l, v.y = y / l, v.z = z / l;
    }
    return v;
  }

  T distance(const kgmVector3d<T>& v){
    kgmVector3d<T> dv(v.x - x, v.y - y, v.z - z);
    return dv.length();
  }

  T dot(const kgmVector3d<T>& v){
    return (x*v.x + y*v.y + z*v.z);
  }

  kgmVector3d<T> cross(const kgmVector3d<T>& v) const{
    return kgmVector3d<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
  }

  kgmVector3d<T> slerp(const kgmVector3d<T>& v, T t){
    return kgmVector3d<T>(x + (v.x - x) * t, y + (v.y - y) * t, z + (v.z - z) * t);
  }

  T angle(const kgmVector3d<T>& v){
    T smul = (T)(x * v.x + y * v.y + z * v.z);
    T denm = (T)(sqrt(x*x + y*y + z*z) *
                 sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
    if(denm == 0)
      return 0;
    T cosa = smul / denm;
    return (T)acos(cosa);
  }
};
