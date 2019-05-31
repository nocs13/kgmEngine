#pragma once

//BOX 2D

template <class T> class kgmBox2d
{
public:
  kgmVector2d<T> min, max;

public:
  kgmBox2d(){
  }
  kgmBox2d(kgmVector2d<T> a, kgmVector2d<T> b){
    min = a, max = b;
  }
  kgmBox2d(T a, T b, T c, T d){
    min.x = a, min.y = b, max.x = c, max.y = d;
  }

  T Width(){
    return max.x - min.x;
  }

  T Height(){
    return max.y - min.y;
  }

  kgmVector2d<T> Center()
  {
    return kgmVector2d<T>((max.x - min.x) / 2, (max.y - min.y) / 2);
  }

  bool intersect(kgmBox2d<T>& b)
  {
    return !( (min.x > b.max.x) || (min.y > b.max.y) ||
              (max.x < b.min.x) || (max.y < b.min.y));
  }

  bool InRect(kgmVector2d<T> pt){
    if((pt.x >= min.x) && (pt.x <= max.x) &&
       (pt.y >= min.y) && (pt.y <= max.y))
      return true;
    return false;
  }

  bool EntyreIn(kgmBox2d<T> rc){
    if((rc.min.x > min.x) && (rc.max.x < max.x) &&
       (rc.min.y > min.y) && (rc.max.y < max.y))
      return true;
    return false;
  }
};

