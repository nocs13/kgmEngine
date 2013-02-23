#pragma once

template <class T> class kgmRay3d{
public:
 kgmVector3d<T> s, d;
public:
 kgmRay3d(){
 }
 kgmRay3d(kgmVector3d<T> &a, kgmVector3d<T> &b){
  s = a, d = b;
 }
 kgmRay3d(kgmLine3d<T> l){
  s = l.s, d = l.e - l.s;
  d.normalize();
 }
};