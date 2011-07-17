#pragma once
#include "../kgmMath/kgmMath.h"

// DOT PRODUCT
float Dot(vec3 a, vec3 b){
 return (a.x*b.x + a.y*b.y + a.z*b.z);
}

// CROSS PRODUCT
vec3 Cross(vec3 a, vec3 b){
 return vec3(a.y * b.z - a.z * b.y,
             a.z * b.x - a.x * b.z,
             a.x * b.y - a.y * b.x);
}

//POINT IN TRIANGLE
bool ptInTriangle(vec3 pt, vec3 a, vec3 b, vec3 c){
 vec3 va = a - pt,
	   vb = b - pt,
	    vc = c - pt;
 float an = 0.0f;

 va.normalize();
  vb.normalize();
   vc.normalize();

 an += (float)acos(Dot(va, vb));
 an += (float)acos(Dot(vb, vc));
 an += (float)acos(Dot(vc, va));

 if(an > (2 * PI * .9f))
  return true;

 return false;
}

// Clasify point
int ClassifyPoint(vec3 pn, float pd, vec3 pt){
 float s = Dot(pn, pt) + pd;
 if(s > 0.0f) 
	 return 1;
 if(s < 0.0f) 
	 return -1;
 return 0;
}

/* 
 Point is object with start 
 Line is object with start and end 
 Vector is object with start and direction 
*/
bool isPointOnLine(vec3 ls, vec3 le, vec3 pt){
 vec3 dir = le - ls;
 float tx = (pt.x - ls.x) / dir.x;
 float ty = (pt.y - ls.y) / dir.y;
 float tz = (pt.z - ls.z) / dir.z;
 if((tx - ty + tz) != tx)
	 return false;
 if((tx < 0.0f) || (tx > 1.0f))
	 return false;
 return true;
}
float distancePointVector(vec3 vs, vec3 vd, vec3 pt){
 vec3 vas  = pt - vs;
 vec3 vab  = vd;
 vec3 nas = vas.normal();
 vec3 nab = vab.normal();
 float alpha = (float)acos(Dot(nas, nab));
 return vas.length() * (float)sin(alpha);
}
float distancePointLine(vec3 ls, vec3 le, vec3 pt){
 vec3 vas  = pt - ls; 
 vec3 vab  = le - ls;
 vec3 nas = vas.normal();
 vec3 nab = vab.normal();
 float alpha = (float)acos(Dot(nas, nab));
 return vas.length() * (float)sin(alpha);
}
float distancePointPlane(vec3 pt, vec3 pn, vec3 po){
 float d = - (Dot(pn, po));
 float nom = pt.x*pn.x + pt.y*pn.y + pt.z*pn.z + d;
 float denom = (float)sqrt(SQR(pn.x) + SQR(pn.y) + SQR(pn.z));
 return nom / denom;
}
bool intersectLineSphere(vec3 ls, vec3 le, vec3 so, float sr){
 vec3 vas  = so - ls;
 vec3 vab  = le - ls;
 vec3 nas = vas.normal();
 vec3 nab = vab.normal();
 float vcs = Dot(nas, nab);
 vec3 vpr = ls + nab * (vas.length() * vcs);

 if(!isPointOnLine(ls, le, vpr))
  return false;

 vec3 so_vpr = vpr - so;
 if(so_vpr.length() > sr)
  return false;

 return true;
}
bool intersectVectorBox(vec3 vs, vec3 vd, vec3 bmin, vec3 bmax){
 vec3 bpos = (bmin + bmax) * 0.5f; 
 vec3 bpar = (bmax - bmin) * 0.5f; 
 vec3 vsb = bpos - vs;
 vec3 vnsb = vsb.normal();
 float alpha = (float)acos(Dot(vd, vnsb));
 float dist = (float)fabs(vsb.length() * sin(alpha));
 if(dist > bpar.length())
  return false;
 return true;
}

bool intersectLineTriangle(vec3& ls, vec3& le, vec3& a, vec3& b, vec3& c){
 kgmLine3d<float>     ln(ls, le);
 kgmTriangle3d<float> trn(a, b, c);

 return false;
}
bool intersectSphereTriangle(vec3& sc, float sr, vec3& a, vec3& b, vec3& c){
 kgmTriangle3d<float> trn(a, b, c);

 if(trn.isin(sc))
	 return true;
 return false;
}

float intersectElipsoidPlane(vec3 rO, vec3 rV, vec3 pN, vec3 pO, float eR){
 float distPoly = -(pN.dot(pO)); 
 float nom = eR * 0.99999999f * pN.length() - distPoly - pN.dot(rO);
 float denom = pN.dot(rV);
 if(denom == 0.0f)
  return -1.0f;
 return nom / denom;
}

float intersectVectorPlane(vec3 rO, vec3 rV, vec3 pN, vec3 pO){
 float distPoly = -(Dot(pN, pO)); 
 float nom = -(Dot(pN, rO) + distPoly);
 float denom = Dot(pN, rV);
 if(denom == 0.0f)
  return -1.0f;
 return nom / denom;
}

