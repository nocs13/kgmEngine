#pragma once

template <class T> class kgmVector2d{
public:
 T x, y;
public:
 kgmVector2d(){
  x = y = 0;
 }
 kgmVector2d(T a, T b){
  x = a, y = b;
 }
};

