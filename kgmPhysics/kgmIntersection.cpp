#include "kgmIntersection.h"

kgmIntersection::kgmIntersection(){

}
kgmIntersection::~kgmIntersection(){

}

void kgmIntersection::reset(){
 m_intersection = false;
 m_normal = vec3(0, 0, 0);
 m_point = vec3(0, 0, 0);
}
bool kgmIntersection::intersection(vec3& start, vec3& end, vec3& tra, vec3& trb, vec3& trc, vec3& pt_insect){
 triangle3 triangle(tra, trb, trc);
 plane3    plane(tra, trb, trc);
 line3     line(start, end);

 if(plane.intersect(line, pt_insect) && triangle.isin(pt_insect))
	 return true;
 return false;
}
bool kgmIntersection::intersection(box3& a, box3& b){
 if((a.max.x < b.min.x) || (a.max.y < b.min.y) || (a.max.z < b.min.z))
	 return false;
 if((a.min.x > b.max.x) || (a.min.y > b.max.y) || (a.min.z > b.max.z))
	 return false;
 return true;
}
bool kgmIntersection::intersection(sphere3& a, sphere3& b){
 float dist = a.center.distance(b.center);
 if(dist < (a.radius + b.radius))
	 return true;
 return false;
}
bool kgmIntersection::intersection(box3& a, sphere3& b){
 vec3 bparams;
 vec3 bcenter;
 vec3 delta;
 bparams.x = a.max.x - a.min.x;
  bparams.y = a.max.y - a.min.y;
  bparams.z = a.max.z - a.min.z;
 bcenter.x = (a.min.x + a.max.x) * 0.5f;
  bcenter.y = (a.min.y + a.max.y) * 0.5f;
  bcenter.z = (a.min.z + a.max.z) * 0.5f;
 delta.x = fabs(b.center.x - bcenter.x);
  delta.y = fabs(b.center.y - bcenter.y);
  delta.z = fabs(b.center.z - bcenter.z);
 if(delta.x > (0.5f * bparams.x + b.radius))
	 return false;
 if(delta.y > (0.5f * bparams.y + b.radius))
	 return false;
 if(delta.z > (0.5f * bparams.z + b.radius))
	 return false;
 return true;
}
bool kgmIntersection::intersection(line3& ln, box3& bx){
 return false;
}
bool kgmIntersection::intersection(line3& ln, cylinder3& cyl){
 line ln_proj; 
	  ln_proj.s = ln.s; 
	  ln_proj.e = ln.e;
	  ln_proj.s.z = ln_proj.e.z = cyl.position.z;
 vec3 v_proj = ln_proj.projection(cyl.position);
 if(!ln_proj.ison(v_proj))
	 return false;
 float c_dist = v_proj.distance(cyl.position);
 if(c_dist > cyl.radius)
	 return false;
 float z_min = cyl.position.z;
 float z_max = cyl.position.z + cyl.height;
 if((z_min > ln.s.z) && (z_min > ln.e.z))
	 return false;
 if((z_max < ln.s.z) && (z_max < ln.e.z))
	 return false;
 return true;
}
bool kgmIntersection::intersection(box3& ob, mtx4& btran, line3& ln, float radius){
 vec3 v[8];
 return false;
}
