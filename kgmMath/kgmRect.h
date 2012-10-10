#pragma once

//Rectangle
template <class T> class kgmRect{
public:
  T x, y, w, h;
public:
  kgmRect(){
  }
  kgmRect(T a, T b, T c, T d){
    x = a, y = b;
    w = c, h = d;
  }
  T width(){
    return w;
  }
  T height(){
    return h;
  }
  void center(T& cx, T& cy){
    cx = x + w / 2;
    cy = y + h / 2;
  }
  bool inside(T cx, T cy){
    if((cx >= x) && (cx <= (x + w)) &&
       (cy >= y) && (cy <= (y + h)))
      return true;
    return false;
  }
  bool inside(kgmRect<T> rc){
    if((rc.x > x) && ((rc.x + rc.w) < (x + w)) &&
       (rc.y > y) && ((rc.y + rc.h) < (y + h)))
      return true;
    return false;
  }
};

