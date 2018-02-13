#pragma once

//BOX 2D
template <class T> class kgmRect2d{
public:
  T x, y, w, h;

public:
  kgmRect2d()
  {
  }

  kgmRect2d(T x, T y, T w, T h)
  {
    this->x = x, this->y = y,
    this->w = w, this->h = h;
  }

  T width()
  {
    return w;
  }

  T height()
  {
    return h;
  }

  kgmVector2d<T> center()
  {
    return kgmVector2d<T>(x +  w / 2, y + h / 2);
  }

  void center(T& cx, T& cy)
  {
    cx = x + w / 2;
    cy = y + h / 2;
  }

  bool inside(kgmVector2d<T> pt)
  {
    if((pt.x >= x) && (pt.x <= (x + w)) &&
       (pt.y >= y) && (pt.y <= (y + h)))
      return true;

    return false;
  }

  bool inside(T cx, T cy)
  {
    if((cx >= x) && (cx <= (x + w)) &&
       (cy >= y) && (cy <= (y + h)))
      return true;

    return false;
  }

  bool inside(kgmRect2d<T> rc)
  {
    if((rc.x >= x) && ((rc.x + rc.w) <= (x + w)) &&
       (rc.y >= y) && ((rc.y + rc.h) <= (y + h)))
      return true;

    return false;
  }
};
