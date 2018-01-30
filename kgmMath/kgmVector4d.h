#pragma once


// VECTOR4D
template <class T> class kgmVector4d
{
public:
  T x, y, z, w;

public:
  kgmVector4d()
  {
    x = y = z = w = 0;
  }

  kgmVector4d(T a, T b, T c, T d)
  {
    x = a, y = b, z = c, w = d;
  }

  kgmVector4d(const kgmVector4d<T>& v)
  {
    x = v.x, y = v.y, z = v.z, w = v.w;
  }

  kgmVector4d<T> normalize()
  {
    T l = (T)sqrt(x * x + y * y + z * z);

    x /= l, y /= l, z /= l, w /= l;

    return *this;
  }

  T dot(kgmVector4d<T>& v)
  {
    return (T)(x * v.x + y * v.y + z * v.z + w * v.w);
  }

  T dot3(kgmVector4d<T>& v)
  {
    return (T)(x * v.x + y * v.y + z * v.z);
  }
};

