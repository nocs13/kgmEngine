#include "kgmCollision.h"
#include "kgmIntersection.h"

inline bool crossLineSphere(line3& ln, sphere3& sp)
{
  if(sp.isin(ln.s) || sp.isin(ln.e))
    return true;
  vec3 proj = ln.projection(sp.center);
  vec3 ctpr = proj - sp.center;
  if((ctpr.length() < sp.radius) && (ln.ison(proj)))
    return true;
  return false;
}

inline bool ptInPolygon(vec3& pt, vec3* poly, u32 points)
{
  double angle = 0.0;
  for(int i = 0; i < points; i++){
    vec3 vs;
    vec3 ve;

    vs = poly[i] - pt;
    vs.normalize();
    if(i == (points - 1))
      ve = poly[0] - pt;
    else
      ve = poly[i + 1] - pt;
    ve.normalize();
    angle += vs.angle(ve); //acos(vs.dot(ve));
  }
  //if(angle < (1.5 * PI))
  if(angle < (1.99999 * PI))
    return false;
  return true;
}

inline bool crossEllipticPolygon(vec3& pt, float rx, float ry, float rz, vec3* poly, u32 points){
  for(int i = 0; i < points; i++){
    vec3  vs;
    vec3  ve;
    vec3  pr;
    line3 ln;

    vs = poly[i];

    if(i == (points - 1))
      ve = poly[0];
    else
      ve = poly[i + 1];

    if((SQR(vs.x - pt.x) < SQR(rx)) &&
       (SQR(vs.y - pt.y) < SQR(ry)) &&
       (SQR(vs.z - pt.z) < SQR(rz)))
      return true;

    ln.set(vs, ve);
    pr = ln.projection(pt);

    if(ln.ison(pt)){
      if((SQR(pr.x - pt.x) < SQR(rx)) &&
         (SQR(pr.y - pt.y) < SQR(ry)) &&
         (SQR(pr.z - pt.z) < SQR(rz)))
        return true;
    }
  }

  return false;
}

/*		***		***		***		***		*/
//Constructor & Desctructor
kgmCollision::kgmCollision()
{
}

kgmCollision::~kgmCollision()
{
}

//reset
void kgmCollision::reset()
{
  m_collision = false;
  m_point     = m_normal = vec3(0, 0, 0);
}

//collision move sphere with triangle
bool kgmCollision::collision(vec3& start, vec3& end, float radius, vec3& tra, vec3& trb, vec3& trc, vec3& pt_insect){
  triangle3 triangle(tra, trb, trc);
  plane3    plane(tra, trb, trc);
  line3     line(start, end);

  float s_dist = plane.distance(start),
      e_dist = plane.distance(end);

  bool b_plinsect = false;
  bool b_plnear = false;
  if(s_dist < 0.0) return false;
  if(plane.intersect(line, pt_insect)){
    b_plinsect = true;
  }
  if((e_dist >= 0.0f) && (e_dist < radius)){
    pt_insect = plane.projection(end);
    b_plnear = true;
  }
  if(!b_plinsect && !b_plnear){
    return false;
  }
  if(b_plinsect || b_plnear){
    //  return true;
  }

  if(triangle.isin(pt_insect)){
    m_point = pt_insect;
    m_normal = plane.normal();
    return true;
  }

  sphere3 sphere(pt_insect, radius);
  /* if(sphere.isin(tra) || sphere.isin(trb) || sphere.isin(trc)){
   m_point = pt_insect;
   m_normal = plane.normal();
   return true;
 }
//*/

  line3 lna(tra, trb), lnb(trb, trc), lnc(trc, tra);
  if(crossLineSphere(lna, sphere) ||
     crossLineSphere(lnb, sphere) ||
     crossLineSphere(lnc, sphere)){
    m_point = pt_insect;
    m_normal = plane.normal();
    return true;
  }
  return false;
}

//collision move sphere with polygon
bool kgmCollision::collision(vec3& start, vec3& end, float radius, vec3* poly, u32 points)
{
  plane3    plane(poly[0], poly[1], poly[2]);
  line3     line(start, end);
  vec3				  pt_insect;

  float s_dist = plane.distance(start),
      e_dist = plane.distance(end);

  bool b_plinsect = false;
  bool b_plnear = false;
  if(s_dist < 0.0) return false;
  if(e_dist > radius) return false;
  if(plane.intersect(line, pt_insect)){
    b_plinsect = true;
  }

  if((e_dist > 0.0f) && (e_dist < radius)){
    pt_insect = plane.projection(end);
    b_plnear = true;
  }

  if(!b_plinsect && !b_plnear){
    return false;
  }
  ///*
  if(!ptInPolygon(pt_insect, poly, points)){
    sphere3 sphere(pt_insect, radius);
    for(int i = 0; i < points ; i++){
      line3 line;
      if(i == (points - 1))
        line.set(poly[0], poly[i]);
      else
        line.set(poly[i], poly[i + 1]);
      if(crossLineSphere(line, sphere)){
        m_normal = plane.normal();
        m_point = pt_insect;
        return true;;
      }
    }
    return false;
  }
  //*/
  m_normal = plane.normal();
  m_point = pt_insect;
  return true;
}
//collision move elipsoid with polygon
bool kgmCollision::collision(vec3& start, vec3& end, float rx, float ry, float rz, vec3* poly, u32 points){
  kgmIntersection	  intersection;
  plane3    plane(poly[0], poly[1], poly[2]);
  line3     line(start, end);
  vec3				  pt_insect;
  vec3				  pr_start, pr_end, pr_delta;
  bool				  b_plinsect = false;

  float	s_dist = plane.distance(start),
      e_dist = plane.distance(end);

  // if(s_dist < 0.0) return false;;
  pr_start	= plane.projection(start);
  pr_end		= plane.projection(end);
  pr_delta	= pr_end - end;
  if(plane.intersect(line, pt_insect)){
    if(ptInPolygon(pt_insect, poly, points) ||
       crossEllipticPolygon(pt_insect, rx, ry, rz, poly, points)){
      m_normal = plane.normal();
      m_point  = pt_insect = pr_end;
      m_collision = true;
      return true;
    }
  }


  if((SQR(pr_end.x - end.x) < SQR(rx)) &&
     (SQR(pr_end.y - end.y) < SQR(ry)) &&
     (SQR(pr_end.z - end.z) < SQR(rz))){
    if(ptInPolygon(pr_end, poly, points) ||
       crossEllipticPolygon(pr_end, rx, ry, rz, poly, points)){
      m_normal = plane.normal();
      m_point  = pr_end;
      m_collision = true;
      return true;
    }

    /*  if((SQR(pr_start.x - start.x) < SQR(rx)) &&
   (SQR(pr_start.y - start.y) < SQR(ry)) &&
   (SQR(pr_start.z - start.z) < SQR(rz))){
   if(ptInPolygon(pr_start, poly, points) ||
      crossEllipticPolygon(pr_start, rx, ry, rz, poly, points)){
    m_normal = plane.normal();
    m_point  = pr_end;
    m_collision = true;
    return true;
   }
  }//*/
  }

  m_normal = plane.normal();
  m_point  = pt_insect;
  return m_collision;
}
//collision move elipsoid with box
bool kgmCollision::collision(vec3& start, vec3& end, float rx, float ry, float rz, box3& b, mtx4& btr){
  int i = 0;
  vec3 box_points[8];
  vec3 box_sides[6][4];
  b.points(box_points);
  for(i = 0; i < 8; i++)
    box_points[i] = btr * box_points[i];
  box_sides[0][0] = box_points[0];
  box_sides[0][1] = box_points[1];
  box_sides[0][2] = box_points[5];
  box_sides[0][3] = box_points[4];
  box_sides[1][0] = box_points[1];
  box_sides[1][1] = box_points[3];
  box_sides[1][2] = box_points[7];
  box_sides[1][3] = box_points[5];
  box_sides[2][0] = box_points[3];
  box_sides[2][1] = box_points[2];
  box_sides[2][2] = box_points[6];
  box_sides[2][3] = box_points[7];
  box_sides[3][0] = box_points[2];
  box_sides[3][1] = box_points[0];
  box_sides[3][2] = box_points[4];
  box_sides[3][3] = box_points[6];
  box_sides[4][0] = box_points[0];
  box_sides[4][1] = box_points[2];
  box_sides[4][2] = box_points[3];
  box_sides[4][3] = box_points[1];
  box_sides[5][0] = box_points[4];
  box_sides[5][1] = box_points[5];
  box_sides[5][2] = box_points[7];
  box_sides[5][3] = box_points[6];

  float dist = -1.0f;
  vec3  ptins;
  m_collision = false;
  for(i = 0; i < 6; i++){
    plane	pln(box_sides[i][0], box_sides[i][1], box_sides[i][2]);
    float edist = pln.distance(end);
    if(collision(start, end, rx, ry, rz, box_sides[i], 4)){
      ptins = m_point;
      m_collision = true;
      if(edist > 0.0f){
        break;
      }
    }
  }
  return m_collision;
}

//collision move sphere with cylinder
bool kgmCollision::collision(vec3& start, vec3& end, float radius, 
                             kgmCylinder3d<float>& cyl, vec3& rpt){
  kgmLine3d<float>	line(start, end);
  vec3				pt_insect;
  vec3				pt_proj;
  vec3				pt_temp;
  float				ln_proj;

  pt_proj = line.projection(cyl.position);
  pt_temp = vec3(pt_proj.x, pt_proj.y, cyl.position.z);
  ln_proj = pt_temp.distance(cyl.position);
  if(ln_proj > cyl.radius)
    return false;
  if((pt_proj.z < cyl.position.z) ||
     (pt_proj.z > (cyl.position.z + cyl.height)))
    return false;
  return true;
}
//collision move cylinder to static cylinder
bool kgmCollision::collision(vec3& start, vec3& end, 
                             kgmCylinder3d<float>& cyl,
                             kgmCylinder3d<float>& scyl,
                             vec3& rpt){
  kgmLine3d<float>	line(start, end);
  vec3				pt_proj;
  vec3				pt_temp;
  float				dist_start_proj;
  float				dist_scyl_proj;

  pt_proj = line.projection(scyl.position);
  dist_start_proj = start.distance(pt_proj);
  if(dist_start_proj > (start.distance(end) + cyl.radius))
    return false;

  pt_temp = vec3(pt_proj.x, pt_proj.y, scyl.position.z);
  dist_scyl_proj = pt_temp.distance(scyl.position);
  if(dist_scyl_proj > (cyl.radius + scyl.radius))
    return false;

  if((pt_proj.z < (scyl.position.z - cyl.height)) ||
     (pt_proj.z > (scyl.position.z + scyl.height)))
    return false;
  rpt = pt_proj;
  return true;
}
//collision move sphere with box
bool kgmCollision::collision(vec3& start, vec3& end, float radius, 
                             box& b, mtx4& btr){
  int i = 0;
  vec3 box_points[8];
  vec3 box_sides[6][4];
  b.points(box_points);
  for(i = 0; i < 8; i++)
    box_points[i] = btr * box_points[i];
  box_sides[0][0] = box_points[0];
  box_sides[0][1] = box_points[1];
  box_sides[0][2] = box_points[5];
  box_sides[0][3] = box_points[4];
  box_sides[1][0] = box_points[1];
  box_sides[1][1] = box_points[3];
  box_sides[1][2] = box_points[7];
  box_sides[1][3] = box_points[5];
  box_sides[2][0] = box_points[3];
  box_sides[2][1] = box_points[2];
  box_sides[2][2] = box_points[6];
  box_sides[2][3] = box_points[7];
  box_sides[3][0] = box_points[2];
  box_sides[3][1] = box_points[0];
  box_sides[3][2] = box_points[4];
  box_sides[3][3] = box_points[6];
  box_sides[4][0] = box_points[0];
  box_sides[4][1] = box_points[2];
  box_sides[4][2] = box_points[3];
  box_sides[4][3] = box_points[1];
  box_sides[5][0] = box_points[4];
  box_sides[5][1] = box_points[5];
  box_sides[5][2] = box_points[7];
  box_sides[5][3] = box_points[6];

  float dist = -1.0f;
  vec3  ptins;
  m_collision = false;
  for(i = 0; i < 6; i++){
    if(collision(start, end, radius, box_sides[i], 4)){
      //  if(collision(start, end, radius, box_sides[i][0], box_sides[i][1],box_sides[i][2], ptins) ||
      //     collision(start, end, radius, box_sides[i][0], box_sides[i][2],box_sides[i][3], ptins)){
      ptins = m_point;
      m_collision = true;
      break;
      if(m_collision){
        if(start.distance(m_point) > dist)
          m_point = ptins;
        else
          dist = start.distance(m_point);
      }else{
        dist = start.distance(m_point);
      }
      m_collision = true;
    }
  }
  return m_collision;
}
//collision move box to static box
bool kgmCollision::collision(vec3& start, vec3& end, 
                             kgmBox3d<float>& mbox,
                             kgmBox3d<float>& sbox,
                             vec3& rpt){
  kgmLine3d<float>	line(start, end);
  vec3				pt_proj;
  vec3				pt_temp;
  float				dist_proj;
  float				dist_sbox_proj;

  vec3	pt_mbox = (mbox.min + mbox.max) * 0.5f;
  pt_mbox.z = mbox.min.z;
  vec3	pr_mbox = mbox.max - mbox.min;
  vec3	pt_sbox = (sbox.min + sbox.max) * 0.5f;
  pt_sbox.z = sbox.min.z;
  vec3	pr_sbox = sbox.max - sbox.min;

  pt_proj = line.projection(pt_sbox);
  dist_proj = start.distance(pt_proj);
  if(dist_proj > (start.distance(end)))
    return false;

  pt_temp = vec3(pt_proj.x, pt_proj.y, pt_sbox.z);
  if((fabs(pt_temp.x - pt_sbox.x)) > (pr_sbox.x * 0.5f + pr_mbox.x * 0.5f))
    return false;
  if((fabs(pt_temp.y - pt_sbox.y)) > (pr_sbox.y * 0.5f + pr_mbox.y * 0.5f))
    return false;
  if((pt_proj.z < (pt_sbox.z - pr_mbox.z)) ||
     (pt_proj.z > (pt_sbox.z + pr_sbox.z)))
    return false;
  rpt = pt_proj;
  return true;
}

bool kgmCollision::ob_collision(obox3& s_box, obox3& d_box)
{
  if(s_box.intersect(d_box) && d_box.intersect(s_box))
    return true;

  return true;
}

bool kgmCollision::ob_collision(box3& s_box, vec3& s_start, vec3& s_end, box3& d_box, vec3& d_start, vec3& d_end)
{
  vec3 s_dir = s_end - s_start;
  vec3 d_dir = d_end - d_start;
  vec3 s_nor = s_dir.normal();
  vec3 d_nor = d_dir.normal();

  //mtx4 s_rot;
  //mtx4 d_rot;

  f32  s_len = s_dir.length();
  f32  d_len = d_dir.length();

  vec3 s_pos = s_start + s_nor * s_len * 0.5;
  vec3 d_pos = d_start + d_nor * d_len * 0.5;

  vec3 s_dim = s_box.dimension();
  vec3 d_dim = d_box.dimension();

  s_dim.x += s_len;
  d_dim.x += d_len;

  //s_rot.rotate(0, s_nor);
  //d_rot.rotate(0, d_nor);

  if(s_nor.length() == 0.0)
    s_nor = vec3(1, 0, 0);

  if(d_nor.length() == 0.0)
    d_nor = vec3(1, 0, 0);

  kgmOBox3d<f32> s_obox(s_pos, s_nor, s_dim);
  kgmOBox3d<f32> d_obox(d_pos, d_nor, d_dim);

  if(s_obox.intersect(d_obox) && d_obox.intersect(s_obox))
    return true;

  return false;
}

bool kgmCollision::ob_collision(box3& s_box, vec3& s_start, vec3& s_rot, float s_len,
                                 box3& d_box, vec3& d_start, vec3& d_rot, float d_len)
{
  box3 sbox = s_box;
  sbox.max.x += s_len;

  box3 dbox = d_box;
  dbox.max.x += d_len;

  vec3 sdim = sbox.max - sbox.min;
  vec3 ddim = dbox.max - dbox.min;

  kgmOBox3d<f32> s_obox(s_start, s_rot, sdim);
  kgmOBox3d<f32> d_obox(d_start, d_rot, ddim);

  if(!s_obox.intersect(d_obox) || !d_obox.intersect(s_obox))
    return false;

  /*
  vec3 spts[8];
  vec3 dpts[8];

  sbox.points(spts);
  dbox.points(dpts);

  for(int i = 0; i < 8; i++)
  {
    spts[i] = smtr * spts[i];
    dpts[i] = dmtr * dpts[i];
  }

  plane3 splanes[3];

  splanes[0] = plane3(spts[0], spts[3], spts[1]);
  splanes[1] = plane3(spts[0], spts[1], spts[4]);
  splanes[2] = plane3(spts[0], spts[4], spts[3]);

  sphere3 dsphere(d_start, d_box.dimension().length() + d_len);
  float   sdim[] = {sbox.dimension().z, sbox.dimension().y, sbox.dimension().x + s_len};

  for(int i = 0; i < 3; i++)
  {
    float distance = splanes[i].distance(dsphere.center);

    if((distance > dsphere.radius) || (fabs(distance) > (dsphere.radius + sdim[i])))
    {
      return false;
    }
    else
    {
      int  sides = 0;
      bool cross = false;

      for(int j = 0; j < 8; j++)
      {
        distance = splanes[i].distance(dpts[j]);

        if(distance < 0.0)
          sides--;
        else
          sides++;

        if((distance < 0.0) && (fabs(distance) < sdim[i]))
        {
          cross = true;

          break;
        }
      }

      if(!cross && (abs(sides) == 8))
      {
        return false;
      }
    }
  }
*/

  return true;
}

bool kgmCollision::ob_collision(obox3& s_box, kgmList<polygon3*>& d_poly, mtx4& d_tran)
{
  vec3               s_points[8];
  kgmPlane3d<float>  s_planes[3];

  s_box.points(s_points);

  s_planes[0] = kgmPlane3d<float>(s_points[0], s_points[2], s_points[1]);
  s_planes[1] = kgmPlane3d<float>(s_points[0], s_points[4], s_points[3]);
  s_planes[2] = kgmPlane3d<float>(s_points[0], s_points[1], s_points[5]);

  float    dims[3] = {s_box.dimension.z, s_box.dimension.x, s_box.dimension.y};

  int  sides = 0;
  bool cross = false;

  for(kgmList<polygon3*>::iterator i = d_poly.begin(); i != d_poly.end(); ++i)
  {
    polygon3* poly = (*i);
    vec3*     points = new vec3[poly->m_count];

    for(int j = 0; j < poly->m_count; j++)
    {
      points[j] = d_tran * poly->m_points[j];
    }

    plane3 d_plane(points[0], points[1], points[2]);

    sides = 0;

    for(int j = 0; j < 8; j++)
    {
      float dist = d_plane.distance(s_points[j]);

      if(dist < 0)
        sides--;
      else
        sides++;
    }

    if(abs(sides) == 8)
    {
      continue;
    }
    else
    {
      cross = false;

      for(int j = 0; j < 3; j++)
      {
        sides = 0;
        cross = false;

        for(int k = 0; k < poly->m_count; k++)
        {
          float dist = s_planes[j].distance(s_points[j]);

          if(dist < 0 && dist > (dims[j]))
            cross = true;

          if(dist < 0)
            sides--;
          else
            sides++;
        }
      }

      if(cross || (abs(sides) != 8))
      {
        return true;
      }
    }
  }

  return false;
}

bool kgmCollision::ob_collision(obox3& s_box, kgmList<triangle3>& d_poly, mtx4& d_tran)
{
  vec3               s_points[8];
  kgmPlane3d<float>  s_planes[3];

  s_box.points(s_points);

  s_planes[0] = kgmPlane3d<float>(s_points[0], s_points[2], s_points[1]);
  s_planes[1] = kgmPlane3d<float>(s_points[0], s_points[4], s_points[3]);
  s_planes[2] = kgmPlane3d<float>(s_points[0], s_points[1], s_points[5]);

  float    dims[3] = {s_box.dimension.z, s_box.dimension.x, s_box.dimension.y};

  int  sides = 0;
  bool cross = false;

  for(kgmList<triangle3>::iterator i = d_poly.begin(); i != d_poly.end(); ++i)
  {
    triangle3 poly = (*i);
    vec3     points[3];

    for(int j = 0; j < 3; j++)
    {
      points[j] = d_tran * poly.pt[j];
    }

    plane3 d_plane(points[0], points[1], points[2]);

    sides = 0;

    for(int j = 0; j < 8; j++)
    {
      float dist = d_plane.distance(s_points[j]);

      if(dist < 0)
        sides--;
      else
        sides++;
    }

    if(abs(sides) == 8)
    {
      continue;
    }
    else
    {
      cross = false;

      for(int j = 0; j < 3; j++)
      {
        sides = 0;
        cross = false;

        for(int k = 0; k < 3; k++)
        {
          float dist = s_planes[j].distance(s_points[j]);

          if(dist < 0 && dist > (dims[j]))
            cross = true;

          if(dist < 0)
            sides--;
          else
            sides++;
        }
      }

      if(cross || (abs(sides) != 8))
      {
        return true;
      }
    }
  }

  return false;
}
