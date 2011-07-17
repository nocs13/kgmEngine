#pragma once
#include "../kgmMain/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmFrustum: public kgmObject{
 KGM_OBJECT(kgmFrustum)

public:
//union{
// struct{ plane plnR, plnL, plnB, plnT, plnF, plnN; };
// struct{ 
 plane3 frs[6]; 
//};
//};
public:
kgmFrustum(){
}
void Frustum(mtx4& mvp)
{
 plane3   pln;
 float    len;

 pln.x = mvp.m[3] - mvp.m[0];
  pln.y = mvp.m[7] - mvp.m[4];
   pln.z = mvp.m[11] - mvp.m[8];
    pln.w = mvp.m[15] - mvp.m[12];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[0] = pln;

 pln.x = mvp.m[3] + mvp.m[0];
  pln.y = mvp.m[7] + mvp.m[4];
   pln.z = mvp.m[11] + mvp.m[8];
    pln.w = mvp.m[15] + mvp.m[12];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[1] = pln;

 pln.x = mvp.m[3] + mvp.m[1];
  pln.y = mvp.m[7] + mvp.m[5];
   pln.z = mvp.m[11] + mvp.m[9];
    pln.w = mvp.m[15] + mvp.m[13];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[2] = pln;

 pln.x = mvp.m[3] - mvp.m[1];
  pln.y = mvp.m[7] - mvp.m[5];
   pln.z = mvp.m[11] - mvp.m[9];
    pln.w = mvp.m[15] - mvp.m[13];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[3] = pln;

 pln.x = mvp.m[3] - mvp.m[2];
  pln.y = mvp.m[7] - mvp.m[6];
   pln.z = mvp.m[11] - mvp.m[10];
    pln.w = mvp.m[15] - mvp.m[14];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[4] = pln;

 pln.x = mvp.m[3] + mvp.m[2];
  pln.y = mvp.m[7] + mvp.m[6];
   pln.z = mvp.m[11] + mvp.m[10];
    pln.w = mvp.m[15] + mvp.m[14];
	len = (float)sqrt(pln.x*pln.x + pln.y*pln.y + pln.z*pln.z);
	pln.x /= len, pln.y /= len, pln.z /= len, pln.w /= len;
	frs[5] = pln;

// mPos = vec3(mvp.m[12], mvp.m[13], mvp.m[14]);
}
bool isPointIn(vec3 pt){
 for(int i = 0; i < 6; i++){
  float s = frs[i].distance(pt);
  if(s < 0) 
	  return false;
 }
 return true;
} 
bool isSphereCross(vec3 p, float r){
 for(int i = 0; i < 6; i++){
  float d = frs[i].distance(p);
  if((d + r) < 0.0f)
	  return false;
 }
 return true;
} 
bool toCross(box3 b){

 return (isPointIn(b.min) && isPointIn(b.max));
} 
};
