#pragma once
#include "../kgmBase//kgmList.h"
#include "../kgmMath/kgmMath.h"
#include "kgmTypes.h"


class kgmCollision{
public:
  bool m_collision;    //collision occured
  vec3 m_point;		      //intersection point
  vec3 m_normal;       //intersection pormal

public:
  //constructor
  kgmCollision();
  ~kgmCollision();

  //reset
  void reset();

  //collision move sphere with triangle
  bool collision(vec3& start, vec3& end, float radius, vec3& tra, vec3& trb, vec3& trc, vec3& pt_ins);

  //collision move sphere with polygon
  bool collision(vec3& start, vec3& end, float radius, vec3* poly, u32 points);

  //collision move elipsoid with polygon
  bool collision(vec3& start, vec3& end, float rx, float ry, float rz, vec3* poly, u32 points);

  //collision move elipsoid with box
  bool collision(vec3& start, vec3& end, float rx, float ry, float rz, box3& b, mtx4& btr);

  //collision move sphere with cylinder
  bool collision(vec3& start, vec3& end, float radius, cylinder3& cyl, vec3& rpt);

  //collision move sphere with box
  bool collision(vec3& start, vec3& end, float radius, box3& b, mtx4& btr);

  //collision move cylinder to static cylinder
  bool collision(vec3& start, vec3& end, cylinder3& cyl, cylinder3& scyl, vec3& rpt);

  //collision move box to static box
  bool collision(vec3& start, vec3& end, box3& box, box3& sbox, vec3& rpt);

  //collision oobb boxes
  bool ob_collision(obox3& s_box, obox3& d_box);

  bool ob_collision(box3& s_box, vec3& s_start, vec3& s_end, box3& d_box, vec3& d_start, vec3& d_end);

  bool ob_collision(box3& s_box, vec3& s_start, vec3& s_rot, float s_len,
                      box3& d_box, vec3& d_start, vec3& d_rot, float d_len);

  bool ob_collision(obox3& s_box, kgmList<polygon3*>& d_poly, mtx4& d_tran);
  bool ob_collision(obox3& s_box, kgmList<triangle3>& d_poly, mtx4& d_tran);
};
