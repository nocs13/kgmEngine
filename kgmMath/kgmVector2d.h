#pragma once

template <class T> class kgmVector2d{
public:
 T x, y;
public:
 kgmVector2d()
 {
  x = y = 0;
 }

 kgmVector2d(T a, T b)
 {
  x = a, y = b;
 }

  kgmVector2d<T> operator+(const kgmVector2d<T>& v) const
  {
    return kgmVector2d<T>(x + v.x, y + v.y);
  }

  kgmVector2d<T> operator+=(const kgmVector2d<T>& v) const
  {
    x += v.x, y += v.y;
    return *this;
  }

  kgmVector2d<T> operator-(const kgmVector2d<T>& v) const
  {
    return kgmVector2d<T>(x - v.x, y - v.y);
  }

  kgmVector2d<T> operator-=(const kgmVector2d<T>& v) const
  {
    x -= v.x, y -= v.y;
    return *this;
  }

  kgmVector2d<T> operator*(T v) const
  {
    return kgmVector2d<T>(x * v, y * v);
  }

  kgmVector2d<T> operator*=(T v) const
  {
    x *= v, y *= v;
    return *this;
  }

  bool operator==(const kgmVector2d<T>& v)
  {
    return ((x == v.x) && (y == v.y));
  }
};

