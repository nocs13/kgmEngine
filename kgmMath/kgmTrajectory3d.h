#pragma once
#include "kgmVector3d.h"

template <class T> class kgmTrajectory3d{
 kgmVector3d<T>*	m_points;
 unsigned int		m_count;
 public:
 kgmTrajectory3d(){
	 m_points = 0;
	 m_count = 0;
 }
 ~kgmTrajectory3d(){
	 if(m_points)
		 delete [] m_points;
 }
 
 bool set(unsigned int count){
  if(count == 0)	
	  return false;
  if(m_points)
	  delete [] m_points;
  m_points = new kgmVector3d<T>[count];
  m_count = count;	
  return true;
 } 
 bool set(unsigned int count, kgmVector3d<T>* points){
  if(count > m_count)
	  return false;
  for(int i = 0; i < count; i++)
	  m_points[i] = points[i];
  return true;
 }
 bool set(unsigned int id, kgmVector3d<T>& point){
  if(id > (m_count - 1))
	  return false;
  m_points[id] = point;
  return true;
 }
};