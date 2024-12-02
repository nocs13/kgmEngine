#include "kgmPhysics.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmSystem/kgmTime.h"

#ifdef WIN32
#include <windows.h>
#endif

kgmPhysics::kgmPhysics()
{
  m_gravity = 0.0;
  m_active = false;
  m_isgravity = false;

  m_time_update = kgmTime::getTicks();
  m_worker = kgmThread::Thread(0);
  m_mutex  = kgmThread::mutex_create();
}

kgmPhysics::~kgmPhysics()
{
  clear();

  kgmThread::mutex_free(m_mutex);
}

int kgmPhysics::__worker(kgmPhysics *ph)
{
  bool active = true;

  while(active) {
    ph->update(20);

    kgmThread::mutex_lock(ph->m_mutex);
    active = ph->m_active;
    kgmThread::mutex_unlock(ph->m_mutex);
    kgmThread::sleep(50);
  }

  return 0;
}

void kgmPhysics::update(u32 ms)
{
  doCollision((f32) ms / (f32) 1000.0f);
}

void kgmPhysics::collision(IBody* cbody, IBody* tobody)
{
}

void kgmPhysics::clear()
{
  kgmThread::mutex_lock(m_mutex);
  m_active = false;
  kgmThread::mutex_unlock(m_mutex);

  if (m_worker) {
    kgmThread::thread_join(m_worker);
    m_worker = kgmThread::Thread(0);
  }

  m_intersection.reset();
  m_collision.reset();

  m_obstacles.clear();
  m_triangles.clear();
  m_bodies.clear();
}

void kgmPhysics::build()
{
  if (m_worker) {
    m_active = false;
    kgmThread::thread_join(m_worker);

    m_worker = kgmThread::Thread(0);
  }

  m_active = true;
  m_worker = kgmThread::thread_create((kgmThread::Thread_Function) kgmPhysics::__worker, this);
}

void kgmPhysics::add(vec3& a, vec3& b, vec3& c)
{
  triangle3 trn(a, b, c);
  m_triangles.add(trn);
}

void kgmPhysics::add(kgmObstacle *o)
{
  if(!o)
    return;

  m_obstacles.add(o);
}

void kgmPhysics::remove(kgmObstacle* o)
{
  for(kgmList<kgmObstacle*>::iterator i = m_obstacles.begin(); i != m_obstacles.end(); i++)
  {
    if(o == (*i))
    {
      (*i) = null;

      break;
    }
  }
}

// dynamic objects
void kgmPhysics::add(kgmPhysics::IBody* body)
{
  if(!body)
    return;

  m_bodies.add(body);
}

void kgmPhysics::remove(kgmPhysics::IBody* body)
{
  for(kgmList<IBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); i++)
  {
    if(body == (*i))
    {
      (*i) = null;

      break;
    }
  }
}

//collision
bool kgmPhysics::checkCollision(vec3& spos, vec3& epos, float& rad, vec3& rpos)
{
  bool  insect = false;
  float dist = 0.0f;
  vec3  pt_ins;
  vec3  n = vec3(0, 0, 0);

  for(int j = 0; j < m_triangles.size(); j++)
  {
    plane pln(m_triangles[j].pt[0],  m_triangles[j].pt[1],  m_triangles[j].pt[2]);
    triangle3  trn(m_triangles[j].pt[0],  m_triangles[j].pt[1],  m_triangles[j].pt[2]);

    if(m_collision.collision(spos, epos, rad, trn.pt[0], trn.pt[1], trn.pt[2], pt_ins))
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

void kgmPhysics::doCollision(f32 dtime)
{
  u16 max_triangles = 5000;
  u16 max_bodies = 50;

  triangle3  triangles[max_triangles];
  IBody*     bodies[max_bodies];

  for(kgmList<IBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); i++)
  {
    if(*i == null)
    {
      m_bodies.erase(i);

      continue;
    }

    bool  holddown = false;
    bool  upstare  = false;
    //bool  insect   = false;
    u32   ccount   = 0;

    box       bbound;
    cylinder  cbound;
    sphere    sbound;
    sphere    sinteract;

    float ctime = dtime;

    IBody* body = (*i);

    if(!body->bodyIsValid())
      continue;

    float mass = body->bodyMass();

    if (mass < 0.5)
      mass = 0.5;

    float gdist = 5 * ctime * mass;

    vec3  spos = body->bodyPosition();

    vec3 force = body->bodyForce();

    vec3 accel = force * (1.0 / mass);

    vec3 speed = accel;

    vec3  epos = spos + speed * ctime;

    if(!body->bodyUseCollision())
    {
      body->bodyPosition(epos);

      continue;
    }

#ifdef DEBUG
    //body->m_intersect = false;
#endif

    //body->m_V.z = 0.0f;

    bbound = body->bodyBound();

    float rx = 0.5f * (bbound.max.x - bbound.min.x),//crad,
          ry = 0.5f * (bbound.max.y - bbound.min.y),//crad,
          rz = 0.5f * (bbound.max.z - bbound.min.z);//10.0f;

    //body->m_V = body->m_P = body->m_F = vec3(0, 0, 0);

    if(m_gravity && body->bodyGravity() && !upstare)
    {
      epos.z -= gdist;
    }

    vec3  pt_ins;

    sinteract.center = epos;
    sinteract.radius = bbound.min.distance(bbound.max) + spos.distance(epos);

    u32 cbodies = getBodies(bodies, max_bodies, sinteract);

    for(u32 k = 0; k < cbodies; k++)
    {
      IBody* cbody = bodies[k];

      if(cbody == body)
        continue;

      if(!cbody || !cbody->bodyIsValid())
        continue;

      vec3 pt_ins, nr_ins;
      vec3 cs = cbody->bodyPosition();
      vec3 cd = cs + cbody->bodyForce().normal();
      vec3 s = body->bodyPosition();
      vec3 d = epos;

      box cbbound= cbody->bodyBound();

      //npos.z = zpos;

      bool  binsect = false;

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

      box_body.min.x -= 0.5 * bbound.dimension().x;
      box_body.min.y -= 0.5 * bbound.dimension().y;
      box_body.min.z -= 0.5 * bbound.dimension().z;
      box_body.max.x += 0.5 * bbound.dimension().x;
      box_body.max.y += 0.5 * bbound.dimension().y;
      box_body.max.z += 0.5 * bbound.dimension().z;

      box_cbody.min.x -= 0.5 * cbbound.dimension().x;
      box_cbody.min.y -= 0.5 * cbbound.dimension().y;
      box_cbody.min.z -= 0.5 * cbbound.dimension().z;
      box_cbody.max.x += 0.5 * cbbound.dimension().x;
      box_cbody.max.y += 0.5 * cbbound.dimension().y;
      box_cbody.max.z += 0.5 * cbbound.dimension().z;

      vec3 bdir  = body->bodyForce().normal();
      vec3 cbdir = cbody->bodyForce().normal();
      vec3 bangles  = bdir.angles();
      vec3 cbangles = cbdir.angles();
      obox3 ob_body  = obox3(body->bodyPosition(), bdir, body->bodyBound().dimension());
      obox3 ob_cbody = obox3(cbody->bodyPosition(), cbdir, cbody->bodyBound().dimension());
      quat  cbquat = quat(cbangles);
      mtx4  mtx_cbody(cbquat, cs);

      if(box_body.intersect(box_cbody) &&
         ((cbody->bodyShape() == BodyShapeBox && m_collision.ob_collision(ob_body, ob_cbody))
          || (cbody->bodyShape() == BodyShapePolyhedron && m_collision.ob_collision(ob_body, ob_cbody)))
         //&& ob_body.intersect(ob_cbody)
         //&& ob_cbody.intersect(ob_body)
         //m_collision.ob_collision(body->m_bound, s, r, 2.0f, cbody->m_bound, s, cr, 1.0f)
         //&& m_collision.ob_collision(body->m_bound, s, d, cbody->m_bound, cbody->m_position, cd)
         )
      {
#ifdef DEBUG
        //body->m_intersect = true;
#endif
        if(cbody->bodyShape() == BodyShapePolyhedron)
        {

        }

        binsect = true;
        pt_ins = m_collision.m_point;
        nr_ins = m_collision.m_normal;
        kgmPlane3d<float> pln(nr_ins, pt_ins);
        vec3 pr_end = pln.projection(d);
        ccount++;

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

        if(body->bodyMass() > 0.0 && cbody->bodyMass() > 0.0)
        {
          n.normalize();
          epos.x = pr_end.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_end.y + n.y * ry;//fabs(pr_end.y - d.y);
          float z = pr_end.z + n.z * rz;//fabs(pr_end.z - d.z);;

          if((z - rz) > (epos.z))
            epos.z = z - rz;
        }
      }

      if(binsect)
      {
        collision(body, cbody);
      }
    }

    sinteract.center = body->bodyPosition();
    sinteract.radius = body->bodyBound().dimension().length() +
                       body->bodyPosition().distance(epos);

    u32 ctriangles = getTriangles(triangles, max_triangles, sinteract);

    for(u32 j = 0; j < ctriangles; j++)
    {
      triangle3  trn = triangles[j];
      plane pln(trn.pt[0], trn.pt[1], trn.pt[2]);
      vec3 n = vec3(0, 0, 0);
      vec3 s = body->bodyPosition();
      vec3 d = epos;
      d.z = s.z += rz;

      //   d.z += rz;
      /*(s.distance(d) > 0.0000001f) && */
      m_collision.reset();

      if(m_collision.collision(s, d, rx, ry, rz, &trn.pt[0], 3))
      {
        vec3 pr_e = m_collision.m_point;
        n = m_collision.m_normal;
        float z = 0.0;
        ccount++;

        if(n.z < 0.0f){
          if(n.z < -0.5f || n.z > 0.5f)
            continue;
          n.z = 0.0f;
        }

        if(n.length() > 0.001f)
        {
          n.normalize();
          epos.x = pr_e.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_e.y + n.y * ry;//fabs(pr_end.y - d.y);
          z      = pr_e.z + n.z * rz;//fabs(pr_end.z - d.z);;
          z += 0.01;

          if((n.z > 0.95) && ((z) > epos.z))
          {
            epos.z = z - rz;
            upstare = true;
          }
        }
      }

      if(m_gravity && body->bodyGravity() && (!upstare))
      {
        d.z -= gdist;
        m_collision.reset();

        if(m_collision.collision(s, d, rx, ry, rz, &trn.pt[0], 3))
        {
          vec3 pr_e = m_collision.m_point;
          float z = 0.0f;
          n = m_collision.m_normal;

          if(n.z < 0.0)
            continue;

          epos.x = pr_e.x + n.x * rx;//fabs(pr_end.x - d.x);
          epos.y = pr_e.y + n.y * ry;//fabs(pr_end.y - d.y);
          z      = pr_e.z + n.z * rz;//fabs(pr_end.z - d.z);
          z += 0.01;

          if((n.z > 0.0) && ((z - rz) > epos.z))
          {
            epos.z = z - rz;
            holddown  = true;
          }
        }
      }
    }

    /*if(!upstare)
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

    body->m_falling = false;*/
    body->bodyPosition(epos);
  }
}

void kgmPhysics::getTriangles(kgmList<triangle3>& triangles, sphere& s)
{
  int i = 0;
  int count = m_triangles.size();

  for(; i < count; i++)
  {
    triangle3 t = m_triangles[i];
    vec3 ct;
    ct.x = (t.pt[0].x + t.pt[1].x + t.pt[2].x) * 0.3333333;
    ct.y = (t.pt[0].y + t.pt[1].y + t.pt[2].y) * 0.3333333;
    ct.z = (t.pt[0].z + t.pt[1].z + t.pt[2].z) * 0.3333333;

    float a = SQR(t.pt[0].x - ct.x) + SQR(t.pt[0].y - ct.y) + SQR(t.pt[0].z - ct.z);
    float b = SQR(t.pt[1].x - ct.x) + SQR(t.pt[1].y - ct.y) + SQR(t.pt[1].z - ct.z);
    float c = SQR(t.pt[2].x - ct.x) + SQR(t.pt[2].y - ct.y) + SQR(t.pt[2].z - ct.z);
    float d = SQR(ct.x - s.center.x) + SQR(ct.y - s.center.y) + SQR(ct.z - s.center.z);

    if(b > a) a = b;
    if(c > a) a = c;

    if((d) < (a + SQR(s.radius)))
      triangles.add(t);
  }
}

void kgmPhysics::getBodies(kgmList<IBody*>& bodies, sphere& s)
{
  int i = 0;
  int count = m_bodies.size();

  for(; i < count; i++)
  {
    box     bb = m_bodies[i]->bodyBound();
    sphere3 bs(bb);

    bs.center = m_bodies[i]->bodyPosition();

    vec3	  ds = s.center - bs.center;
    float	  mgn = SQR(ds.x) + SQR(ds.y) + SQR(ds.z);

    if(mgn < (SQR(s.radius) + SQR(bs.radius)))
      bodies.add(m_bodies[i]);
  }
}

u32 kgmPhysics::getTriangles(triangle3 triangles[], u32 max, sphere& s)
{
  u32 count = 0;

  for(kgmList<triangle3>::iterator i = m_triangles.begin(); i != m_triangles.end(); i++)
  {
    if(count >= max)
      break;

    triangle3 t = (*i);

    vec3 ct;

    ct.x = (t.pt[0].x + t.pt[1].x + t.pt[2].x) * 0.3333333;
    ct.y = (t.pt[0].y + t.pt[1].y + t.pt[2].y) * 0.3333333;
    ct.z = (t.pt[0].z + t.pt[1].z + t.pt[2].z) * 0.3333333;

    float a = SQR(t.pt[0].x - ct.x) + SQR(t.pt[0].y - ct.y) + SQR(t.pt[0].z - ct.z);
    float b = SQR(t.pt[1].x - ct.x) + SQR(t.pt[1].y - ct.y) + SQR(t.pt[1].z - ct.z);
    float c = SQR(t.pt[2].x - ct.x) + SQR(t.pt[2].y - ct.y) + SQR(t.pt[2].z - ct.z);
    float d = SQR(ct.x - s.center.x) + SQR(ct.y - s.center.y) + SQR(ct.z - s.center.z);

    if(b > a) a = b;
    if(c > a) a = c;

    if((d) < (a + SQR(s.radius)))
      triangles[count++] = t;
  }

  for(kgmList<kgmObstacle*>::iterator i = m_obstacles.begin(); i != m_obstacles.end(); i++)
  {
    if(count >= max)
      break;

    for(u32 j = 0; j < (*i)->length(); j++)
    {
      if(count >= max)
        break;

      triangle3 t = (*i)->get(j);

      vec3 ct;

      ct.x = (t.pt[0].x + t.pt[1].x + t.pt[2].x) * 0.3333333;
      ct.y = (t.pt[0].y + t.pt[1].y + t.pt[2].y) * 0.3333333;
      ct.z = (t.pt[0].z + t.pt[1].z + t.pt[2].z) * 0.3333333;

      float a = SQR(t.pt[0].x - ct.x) + SQR(t.pt[0].y - ct.y) + SQR(t.pt[0].z - ct.z);
      float b = SQR(t.pt[1].x - ct.x) + SQR(t.pt[1].y - ct.y) + SQR(t.pt[1].z - ct.z);
      float c = SQR(t.pt[2].x - ct.x) + SQR(t.pt[2].y - ct.y) + SQR(t.pt[2].z - ct.z);
      float d = SQR(ct.x - s.center.x) + SQR(ct.y - s.center.y) + SQR(ct.z - s.center.z);

      if(b > a) a = b;
      if(c > a) a = c;

      if((d) < (a + SQR(s.radius)))
        triangles[count++] = t;
    }
  }

  return count;
}

u32 kgmPhysics::getBodies(IBody* bodies[], u32 max, sphere& s)
{
  u32 count = 0;

  for(kgmList<IBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); i++)
  {
    if(count >= max)
      break;

    box bb = (*i)->bodyBound();

    sphere3 bs(bb);

    bs.center = (*i)->bodyPosition();

    vec3 ds = s.center - bs.center;

    float mgn = SQR(ds.x) + SQR(ds.y) + SQR(ds.z);

    if(mgn < (SQR(s.radius) + SQR(bs.radius)))
      bodies[count++] = (*i);
  }

  return count;
}
