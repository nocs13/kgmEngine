#pragma once

template <class T>
class kgmVector2d
{
public:
  T x, y;

public:
  kgmVector2d()
  {
    set(T(0), T(0));
  }

  kgmVector2d(T a, T b)
  {
    set(a, b);
  }

  kgmVector2d<T> operator+(const kgmVector2d<T> &v) const
  {
    return kgmVector2d<T>(x + v.x, y + v.y);
  }

  kgmVector2d<T> operator+=(const kgmVector2d<T> &v) const
  {
    x += v.x, y += v.y;
    return *this;
  }

  kgmVector2d<T> operator-(const kgmVector2d<T> &v) const
  {
    return kgmVector2d<T>(x - v.x, y - v.y);
  }

  kgmVector2d<T> operator-=(const kgmVector2d<T> &v) const
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

  bool operator==(const kgmVector2d<T> &v)
  {
    return ((x == v.x) && (y == v.y));
  }

  void set(T a, T b)
  {
    x = a;
    y = b;
  }

  T length()
  {
    return (T)sqrt(x * x + y * y);
  }

  kgmVector2d<T> inverse()
  {
    return kgmVector2d<T>(-x, -y);
  }

  void invert()
  {
    x = -x, y = -y;
  }

  void normalize()
  {
    T l = length();

    if (l != 0.0)
    {
      x /= l, y /= l;
    }
  }

  kgmVector2d<T> normal()
  {
    kgmVector2d<T> v;
    T l = length();

    if (l != 0.0)
    {
      v.x = x / l, v.y = y / l;
    }

    return v;
  }

  T distance(const kgmVector2d<T> &v)
  {
    kgmVector2d<T> dv(v.x - x, v.y - y);
    return dv.length();
  }
};
