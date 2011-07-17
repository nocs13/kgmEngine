#pragma once

//BOX 2D
template <class T> class kgmRect2d{
public:
 T m_x, m_y, m_w, m_h;

public:
 kgmRect2d(){
 }
 kgmRect2d(T x, T y, T w, T h){
  m_x = x, m_y = y, m_w = w, m_h = h;
 }
 T Width(){
  return m_w;
 }
 T Height(){
  return m_h;
 }
 kgmVector2d<T> Center(){
  return kgmVector2d<T>(m_x +  m_w / 2, m_y + m_h / 2);
 }
 bool InRect(kgmVector2d<T> pt){
  if((pt.x >= m_x) && (pt.x <= (m_x + m_w)) &&
     (pt.y >= m_y) && (pt.y <= (m_y + m_h)))
   return true;
  return false;
 }
 bool EntyreIn(kgmRect2d<T> rc){
  if((rc.m_x > m_x) && ((rc.m_x + rc.m_w) < (m_x + m_w)) &&
	 (rc.m_y > m_y) && ((rc.m_x + rc.m_w) < (m_x + m_w)))
	 return true;
  return false;
 }
};


