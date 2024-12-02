#pragma once
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmOctTree.h"

#include "kgmIPhysics.h"
#include "kgmBody.h"
#include "kgmObstacle.h"
#include "kgmCollision.h"
#include "kgmIntersection.h"

#include "../kgmSystem/kgmThread.h"

class kgmPhysics: public kgmIPhysics, public kgmObject
{
  KGM_OBJECT(kgmPhysics);

public:
  float  m_gravity;
  bool   m_isgravity;

  kgmList<IBody*>         m_bodies;
  kgmList<triangle3>      m_triangles;
  kgmList<kgmObstacle*>   m_obstacles;
  kgmOctTree<triangle3*>  m_treestatic;

  vec3  m_ptCurrent;
  vec3  m_ptDestination;
  vec3  m_ptCollision;
  vec3  m_nrCollision;
  box3  m_space;

  kgmCollision    m_collision;
  kgmIntersection m_intersection;

  u32 m_time_update;

  kgmThread::Thread m_worker;
  kgmThread::Mutex  m_mutex;
  bool              m_active;

public:
  kgmPhysics();
  ~kgmPhysics();

  //virtual
  virtual void clear();
  virtual void build();
  virtual void update(u32 ms);
  virtual void collision(IBody* cbody, IBody* tobody);

  // static objects
  void add(vec3& a, vec3& b, vec3& c);
  void add(kgmCollision::Shape* shape);

  void add(kgmObstacle*);
  void remove(kgmObstacle*);

  // dynamic objects
  void add(IBody*);
  void remove(IBody*);

  //collision detection
  bool checkCollision(vec3& spos, vec3& epos, float& rad, vec3& rpos);
  // bool checkCollision(vec3& spos, vec3& epos, box& bound, vec3& rpos);

protected:
  void doCollision(f32);
  void getTriangles(kgmList<triangle3>& triangles, sphere& s);
  void getBodies(kgmList<IBody*>& bodies, sphere& s);

  u32 getTriangles(triangle3 triangles[], u32 max, sphere& s);
  u32 getBodies(IBody* bodies[], u32 max, sphere& s);

private:
  static int __worker(kgmPhysics*);
};
