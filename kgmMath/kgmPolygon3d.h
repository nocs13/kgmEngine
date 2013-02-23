#pragma once

//TRIANGLE 3D
template <class T> class kgmPolygon3d{
 kgmVector3d<T>*	m_points;
 unsigned int		m_count;
public:
 kgmPolygon3d(){
	 m_points = 0;
	 m_count  = 0;
 }

 kgmPolygon3d(int cnt){
	 m_points = 0;
	 if(cnt > 0)
	   m_points = new kgmVector3d<T>[cnt];
	 m_count  = cnt;
 }

 kgmPolygon3d(int cnt, kgmVector3d<T>* pts){
	 m_points = 0;
	 if(cnt > 0)
		 m_points = new kgmVector3d<T>[cnt];
	 if(pts)
		 memcpy(m_points, pts, cnt * sizeof(kgmVector3d<T>));
	 m_count  = cnt;
 }

 ~kgmPolygon3d(){
	 if(m_points)
		 delete [] m_points;
 }

 kgmVector3d<T>& operator[](unsigned int i){
  return m_points[i];
 }

 void clear(){
	 if(m_points) 
		 delete [] m_points;
	 m_points = 0;
	 m_count = 0;
  
 }

 bool isin(kgmVector3d<T> v, float pres = 0.1){
  kgmVector3d<T> v0, v1, v2;
  T a0, a1, a2, an;
  an = (T)0.0;
//  v0 = a - v;
//  v1 = b - v;
//  v2 = c - v;
//  a0 = v0.angle(v1);
//  a1 = v1.angle(v2);
//  a2 = v2.angle(v0);
//  an = a0 + a1 + a2;
  int i;
  for(int i = 0; i < m_count; i++){
   v0 = m_points[i] - v;
   if((i + 1) >= m_count)
    v1 = m_points[0] - v;
   else
    v1 = m_points[i + 1] - v;
   an += v0.angle(v1);
  }
  if(fabs(an - (2.0 * PI)) < pres)
   return true;
  return false;
 }

 bool intersect(kgmVector3d<T> v, float r){
  return false;
 }
};
