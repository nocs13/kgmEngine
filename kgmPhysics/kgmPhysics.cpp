#include "kgmPhysics.h"
#include "../kgmBase/kgmTime.h"

#ifdef WIN32
#include <windows.h>
#endif

//kgmPhysics
kgmPhysics::kgmPhysics(){
}

kgmPhysics::~kgmPhysics(){
}

//virtual 
void kgmPhysics::update(float time){
  doCollision(time);

  for(kgmList<kgmBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_bodies.erase(i);
    }
  }
}

void kgmPhysics::collision(kgmBody* cbody, kgmBody* tobody)
{
}

///////////////////////// 
void kgmPhysics::clear(){
  for(kgmList<kgmBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); ++i)
  {
    (*i)->release();
  }

  m_bodies.clear();
  m_triangles.clear();
  m_collision.reset();
  m_intersection.reset();
}

void kgmPhysics::build(){
}

// static objects
void kgmPhysics::add(vec3& a, vec3& b, vec3& c){
  Triangle trn(a, b, c);
  m_triangles.add(trn);
}

// dynamic objects
void kgmPhysics::add(kgmBody* body){
  if(!body)
    return;

  m_bodies.add(body);
  body->increment();
}

//remove body
bool kgmPhysics::remove(kgmBody* body){
  for(int i = 0; i < m_bodies.size(); i++){
    if(m_bodies[i] == body){
      m_bodies.erase(i);
      break;
    }
  }
  return true;
}

//collision
bool kgmPhysics::checkCollision(vec3& spos, vec3& epos, float& rad, vec3& rpos){
  bool  insect = false;
  float dist = 0.0f;
  vec3  pt_ins;
  vec3  n = vec3(0, 0, 0);

  for(int j = 0; j < m_triangles.size(); j++)
  {
    plane pln(m_triangles[j].a,
    m_triangles[j].b,
    m_triangles[j].c);
    Triangle  trn(m_triangles[j].a,
    m_triangles[j].b,
    m_triangles[j].c);

    if(m_collision.collision(spos, epos, rad, trn.a, trn.b, trn.c, pt_ins))
    {
      insect = true;
      dist = pln.distance(epos);
      n = pln.normal();

      break;
    }
  }

  if(insect)
  {
    float len = 0.0f;

    if(dist >= 0.0)
    {
      len = (float)fabs(rad - dist);
    }
    else
    {
      len = rad + (float)fabs(dist);
    }

    if(n.z < 0.0f)
    {
      n.z = 0.0f;
    }

    n.normalize();
    rpos = epos + n * (len + 0.01f);
  }

  return insect;
}

void kgmPhysics::doCollision(float dtime){
  float G = 1.0f;
  u32  stime = kgmTime::getTicks();

  //active collisions
  kgmList<Triangle>	triangles;
  kgmList<kgmBody*>	bodies;



  //iterate by all dynamic objects
  for(int i = 0; i < m_bodies.size(); i++)
  {
    bool  holddown = false;
    bool  upstare  = false;
    bool  insect   = false;
    u32   ccount   = 0;
    cylinder  cbound;
    sphere    sbound;
    sphere    sinteract;

    float ctime  = dtime;// + kgmTime::getTicks() - stime;

    kgmBody* body = m_bodies[i];

    if(!body->m_valid)
      continue;

    if(!body->m_collision)
    {
      body->m_position = body->m_position + body->direction() * body->m_velocity * dtime;

      continue;
    }

#ifdef TEST
    body->m_intersect = false;
#endif

    //body->m_V.z = 0.0f;
    vec3 bdPos = m_ptCurrent = body->m_position;
    float gdist = 5 * ctime * body->m_mass;
    vec3  spos = body->m_position;
    vec3  epos = body->m_position + body->direction() * (body->m_velocity * ctime);

    vec3 b_min = body->m_bound.min;
    vec3 b_max = body->m_bound.max;
    float crad = b_min.distance(b_max) * 0.5f;
    float rx = 0.5f * (body->m_bound.max.x - body->m_bound.min.x),//crad,
    ry = 0.5f * (body->m_bound.max.y - body->m_bound.min.y),//crad,
    rz = 0.5f * (body->m_bound.max.z - body->m_bound.min.z);//10.0f;

    //body->m_V = body->m_P = body->m_F = vec3(0, 0, 0);
    if(body->m_speed_up > 0.0f)
    {
      upstare = true;
      epos.z  = spos.z + body->m_speed_up * ctime;
      body->m_speed_up -= (body->m_speed_up * 0.2);

      if(body->m_speed_up < 1.0f)
        body->m_speed_up = 0.0f;
    }

    if(m_gravity && body->m_gravity && !upstare)
    {
      epos.z -= gdist;
    }

    vec3  pt_ins;
    float len = 0.0f;
    //**********************************

    sinteract.center = body->m_position;
    sinteract.radius = body->m_bound.min.distance(body->m_bound.max) +
    body->m_position.distance(epos);
    //getBodies(bodies, sinteract);

    //check collision to dynamic objects
    for(int k = 0; k < m_bodies.size(); k++)
    {
      kgmBody* cbody = m_bodies[k];

      if(cbody == body)
        continue;

      if(!cbody || !cbody->m_valid)
        continue;

      //continue;

      /*
     box	 a = body->m_bound;
     a.min = a.min + body->m_position; a.max = a.max + body->m_position;
     box   b = cbody->m_bound;
     b.min = b.min + cbody->m_position;
     b.max = b.max + cbody->m_position;
     cylinder cyla(body->m_position, 3, body->m_bound.max.z - body->m_bound.min.z),
     cylb(cbody->m_position, 10, cbody->m_bound.max.z - cbody->m_bound.min.z);
     */

      vec3 pt_ins, nr_ins;
      vec3 cs = cbody->m_position;
      vec3 cd = cbody->m_position + cbody->direction();
      vec3 cr = cbody->rotation();
      vec3 s = body->m_position;
      vec3 d = epos;
      vec3 r = body->rotation();

      //		npos.z = zpos;

      bool  binsect = false;
      mtx4  mtr;
      cbody->transform(mtr);

      s.z += rz;
      d.z += rz;
      //   s.z += crad;
      //   d.z += crad;
      if(d.z < s.z)
        d.z = s.z;

      m_collision.reset();

      //build bodies move bounds
      box3 box_body;
      box3 box_cbody;

      box_body.min.x = (s.x < d.x) ? (s.x) : (d.x);
      box_body.min.y = (s.y < d.y) ? (s.y) : (d.y);
      box_body.min.z = (s.z < d.z) ? (s.z) : (d.z);
      box_body.max.x = (s.x > d.x) ? (s.x) : (d.x);
      box_body.max.y = (s.y > d.y) ? (s.y) : (d.y);
      box_body.max.z = (s.z > d.z) ? (s.z) : (d.z);
      box_cbody.min.x = (cs.x < cd.x) ? (cs.x) : (cd.x);
      box_cbody.min.y = (cs.y < cd.y) ? (cs.y) : (cd.y);
      box_cbody.min.z = (cs.z < cd.z) ? (cs.z) : (cd.z);
      box_cbody.max.x = (cs.x > cd.x) ? (cs.x) : (cd.x);
      box_cbody.max.y = (cs.y > cd.y) ? (cs.y) : (cd.y);
      box_cbody.max.z = (cs.z > cd.z) ? (cs.z) : (cd.z);

      box_body.min.x -= 0.5 * body->m_bound.dimension().x;
      box_body.min.y -= 0.5 * body->m_bound.dimension().y;
      box_body.min.z -= 0.5 * body->m_bound.dimension().z;
      box_body.max.x += 0.5 * body->m_bound.dimension().x;
      box_body.max.y += 0.5 * body->m_bound.dimension().y;
      box_body.max.z += 0.5 * body->m_bound.dimension().z;
      box_cbody.min.x -= 0.5 * cbody->m_bound.dimension().x;
      box_cbody.min.y -= 0.5 * cbody->m_bound.dimension().y;
      box_cbody.min.z -= 0.5 * cbody->m_bound.dimension().z;
      box_cbody.max.x += 0.5 * cbody->m_bound.dimension().x;
      box_cbody.max.y += 0.5 * cbody->m_bound.dimension().y;
      box_cbody.max.z += 0.5 * cbody->m_bound.dimension().z;
      //   if((s.distance(s) > 0.1f) && (m_collision.collision(s, d, crad, b, mtr))){

      if(
      box_body.intersect(box_cbody) &&
      m_collision.ob_collision(body->m_bound, s, r, 2.0f, cbody->m_bound, s, cr, 0.0f)
      //&& m_collision.ob_collision(body->m_bound, s, d, cbody->m_bound, cbody->m_position, cd)
      )
      {
#ifdef TEST
        body->m_intersect = true;
#endif
        int  k = 0;
        insect = binsect = true;
        pt_ins = m_collision.m_point;
        nr_ins = m_collision.m_normal;
        kgmPlane3d<float> pln(nr_ins, pt_ins);
        vec3 pr_end = pln.projection(d);
        float dist = pln.distance(d);
        ccount++;

        if(dist >= 0.0)
        {
          len = (float)fabs(crad - dist);
        }
        else
        {
          len = crad + (float)fabs(dist);
        }

        vec3 n = m_collision.m_normal;
        n = pln.normal();
        n.normalize();

        if(n.z < 0.0f)
        {
          n.z = 0.0f;
        }

        if(n.z > 0.01f)
        {
          upstare = true;
        }

        if(body->m_mass > 0.0 && cbody->m_mass > 0.0)
        {
          n.normalize();
          epos.x = pr_end.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_end.y + n.y * ry;//fabs(pr_end.y - d.y);
          float z = pr_end.z + n.z * rz;//fabs(pr_end.z - d.z);;

          if((z - rz) > (epos.z))
            epos.z = z - rz;
        }
      }

      ///*
      if(m_gravity && body->m_gravity && (!upstare) && (body->m_speed_up <= 0.0f))
      {
        d.z -= gdist;
        //if(m_collision.collision(d, g, 0.1f, b, mtr)){
        m_collision.reset();

        if(m_collision.collision(s, d, rx, ry, rz, cbody->m_bound, mtr))
        {
          if((m_collision.m_normal.z > 0.01f) && (m_collision.m_point.z > epos.z))
            epos.z = m_collision.m_point.z + 0.1f;

          upstare = true;
          binsect = true;
        }
      }//*/

      if(binsect)
      {
        collision(body, cbody);

        if(cbody->m_velocity > 0.0f)
        {
          //body->m_V = body->m_V + cbody->m_direction * cbody->m_velocity;
        }
      }
    }
    bodies.clear();
    //**********************************

    sinteract.center = body->m_position;
    sinteract.radius = body->m_bound.min.distance(body->m_bound.max) +
    body->m_position.distance(epos);
    getTriangles(triangles, sinteract);

    //check collision to static objects
    for(int j = 0; j < triangles.size(); j++)
    {
      Triangle  trn = triangles[j];
      plane pln(trn.a, trn.b, trn.c);
      bool iscollision = false;
      vec3 n = vec3(0, 0, 0);
      vec3 s = body->m_position;
      vec3 d = epos;
      d.z = s.z += rz;

      //   d.z += rz;
      /*(s.distance(d) > 0.0000001f) && */
      m_collision.reset();
      if(m_collision.collision(s, d, rx, ry, rz, &trn.a, 3)){
        vec3 pr_e = m_collision.m_point;
        vec3 pr_s = pln.projection(s);
        n = m_collision.m_normal;
        float z = 0.0;
        insect = true;
        ccount++;

        if(n.z < 0.0f){
          if(n.z < -0.5f || n.z > 0.5f)
            continue;
          n.z = 0.0f;
        }

        if(n.length() > 0.001f){
          n.normalize();
          epos.x = pr_e.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_e.y + n.y * ry;//fabs(pr_end.y - d.y);
          z      = pr_e.z + n.z * rz;//fabs(pr_end.z - d.z);;
          z += 0.01;
          if((n.z > 0.95) && ((z) > epos.z)){
            epos.z = z - rz;
            upstare = true;
          }
        }
      }

      if(m_gravity && body->m_gravity && (!upstare)){
        d.z -= gdist;
        m_collision.reset();
        if(m_collision.collision(s, d, rx, ry, rz, &trn.a, 3)){
          vec3 pr_e = m_collision.m_point;
          float z = 0.0f;
          n = m_collision.m_normal;
          if(n.z < 0.0) continue;
          epos.x = pr_e.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_e.y + n.y * ry;//fabs(pr_end.y - d.y);
          z      = pr_e.z + n.z * rz;//fabs(pr_end.z - d.z);
          z += 0.01;
          if((n.z > 0.0) && ((z - rz) > epos.z)){
            epos.z = z - rz;
            holddown  = true;
          }
        }
      }
    }
    //**********************************

    triangles.clear();

    //select position
    if(!upstare)
    {
      if(holddown)
        body->m_falling = false;
      else
        body->m_falling = true;
    }
    else
    {
      body->m_falling = false;
    }
    if(epos.z <= (spos.z - gdist))
      body->m_falling = true;
    body->m_falling = false;
    body->m_position = epos;
  }
}

void kgmPhysics::getTriangles(kgmList<Triangle>& triangles, sphere& s){
  int i = 0;
  int count = m_triangles.size();
  for(i; i < count; i++){
    Triangle t = m_triangles[i];
    vec3 mn = t.a;
    vec3 ct;
    ct.x = (t.a.x + t.b.x + t.c.x) * 0.3333333;
    ct.y = (t.a.y + t.b.y + t.c.y) * 0.3333333;
    ct.z = (t.a.z + t.b.z + t.c.z) * 0.3333333;

    float a = SQR(t.a.x - ct.x) + SQR(t.a.y - ct.y) + SQR(t.a.z - ct.z);
    float b = SQR(t.b.x - ct.x) + SQR(t.b.y - ct.y) + SQR(t.b.z - ct.z);
    float c = SQR(t.c.x - ct.x) + SQR(t.c.y - ct.y) + SQR(t.c.z - ct.z);
    float d = SQR(ct.x - s.center.x) + SQR(ct.y - s.center.y) + SQR(ct.z - s.center.z);

    if(b > a) a = b;
    if(c > a) a = c;

    if((d) < (a + SQR(s.radius)))
      triangles.add(t);
  }
}

void kgmPhysics::getBodies(kgmList<kgmBody*>& bodies, sphere& s){
  int i = 0;
  int count = m_bodies.size();
  for(i; i < count; i++){
    sphere3 bs(m_bodies[i]->m_bound);	bs.center = m_bodies[i]->m_position;
    vec3	  ds = s.center - bs.center;
    float	  mgn = SQR(ds.x) + SQR(ds.y) + SQR(ds.z);
    if(mgn < (SQR(s.radius) + SQR(bs.radius)))
      bodies.add(m_bodies[i]);
  }
}


