#pragma once
#include "kgmVector3d.h"
#include "kgmPlane3d.h"
#include "kgmMatrix4x4.h"

template <class T> class kgmFrustum3d{
public:
union{
 struct{ kgmPlane3d<T> plnR, plnL, plnB, plnT, plnF, plnN; };
 struct{ kgmPlane3d<T> frs[6]; };
};
 kgmVector3d<T> pos, dir;
 T mFov, mAspect, mNear, mFar;
public:
kgmFrustum3d(){
}
kgmFrustum3d(kgmMatrix4x4<T> &mvp)
{
 kgmPlane3d<T>   pln;
 T    len;

 pln.x = mvp.m[3] - mvp.m[0];
  pln.y = mvp.m[7] - mvp.m[4];
   pln.z = mvp.m[11] - mvp.m[8];
    pln.w = mvp.m[15] - mvp.m[12];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnR = pln;

 pln.x = mvp.m[3] + mvp.m[0];
  pln.y = mvp.m[7] + mvp.m[4];
   pln.z = mvp.m[11] + mvp.m[8];
    pln.w = mvp.m[15] + mvp.m[12];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnL = pln;

 pln.x = mvp.m[3] + mvp.m[1];
  pln.y = mvp.m[7] + mvp.m[5];
   pln.z = mvp.m[11] + mvp.m[9];
    pln.w = mvp.m[15] + mvp.m[13];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnB = pln;

 pln.x = mvp.m[3] - mvp.m[1];
  pln.y = mvp.m[7] - mvp.m[5];
   pln.z = mvp.m[11] - mvp.m[9];
    pln.w = mvp.m[15] - mvp.m[13];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnT = pln;

 pln.x = mvp.m[3] - mvp.m[2];
  pln.y = mvp.m[7] - mvp.m[6];
   pln.z = mvp.m[11] - mvp.m[10];
    pln.w = mvp.m[15] - mvp.m[14];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnF = pln;

 pln.x = mvp.m[3] + mvp.m[2];
  pln.y = mvp.m[7] + mvp.m[6];
   pln.z = mvp.m[11] + mvp.m[10];
    pln.w = mvp.m[15] + mvp.m[14];
	len = (T)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	plnN = pln;

 pos = kgmVector3d<T>(mvp.m[12], mvp.m[13], mvp.m[14]);
}
bool isPointIn(kgmVector3d<T> pt){
 for(int i = 0; i < 6; i++){
  int s = sidePtPln(pt, frs[i]);
  if(s < 0) 
	  return false;
 }
 return true;
} 
bool isSphereCross(kgmVector3d<T> p, float r){
 for(int i = 0; i < 6; i++){
  float d;
  distPtPln(d, p, frs[i]);
  if((d + r) < 0.0f)
	  return false;
 }
 return true;
} 
bool isBoxCross(kgmVector3d<T> p[8]){
 for(int i = 0; i < 6; i++){
  for(int j = 0; j < 8; j++){
   float d = frs[i].distanse(p[j]);
   if(d < 0.0f)
	  return false;
  }
 }
 return true;
} 
};