#pragma once
#include "../kgmMath/kgmMath.h"

class kgmIntersection{
public:
 bool m_intersection;
 vec3 m_normal;
 vec3 m_point;
public:
 kgmIntersection();
 virtual ~kgmIntersection();
  
 void reset();
 bool intersection(vec3& start, vec3& end, vec3& tra, vec3& trb, vec3& trc, vec3& pt_insect);
 bool intersection(box3& a, box3& b);
 bool intersection(sphere3& a, sphere3& b);
 bool intersection(box3& a, sphere3& b);
 bool intersection(line3& ln, box3& bx);
 bool intersection(line3& ln, cylinder3& cyl);
 bool intersection(box3& ob, mtx4& btran, line3& ln, float radius);
};