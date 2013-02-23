#pragma once
#include "kgmBox3d.h"

template <class T> class kgmSphere3d{
public:
 kgmVector3d<T> center;
 T            radius;
public:
 kgmSphere3d(){
  radius = 0;
 }
 kgmSphere3d(kgmVector3d<T>& v, T r){
  center = v;
  radius = r;
 }
 kgmSphere3d(kgmBox3d<T>& b){
  kgmVector3d<T> d = b.max - b.min;
  center.x = (b.min.x + b.max.x) / 2;
  center.y = (b.min.y + b.max.y) / 2;
  center.z = (b.min.z + b.max.z) / 2;
  radius = d.length() / 2;
 }
 bool isin(kgmVector3d<T>& v){
  kgmVector3d<T> d = v - center;
  if(d.length() <= radius)
	  return true;
  return false;
 }
};