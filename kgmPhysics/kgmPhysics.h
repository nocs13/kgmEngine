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

class kgmPhysics: public kgmObject, public kgmIPhysics
{
  KGM_OBJECT(kgmPhysics);

public:
  float  m_gravity;
  bool   m_isgravity;

  kgmList<kgmBody*>       m_bodies;
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

public:
  kgmPhysics();
  ~kgmPhysics();

  //virtual
  virtual void clear();
  virtual void build();
  virtual void update();
  virtual void collision(kgmBody* cbody, kgmBody* tobody);

  // static objects
  void add(vec3& a, vec3& b, vec3& c);
  void add(kgmCollision::Shape* shape);

  void add(kgmObstacle*);
  void remove(kgmObstacle*);

  // dynamic objects
  void add(kgmBody*);
  void remove(kgmBody*);

  //collision detection
  bool checkCollision(vec3& spos, vec3& epos, float& rad, vec3& rpos);
  // bool checkCollision(vec3& spos, vec3& epos, box& bound, vec3& rpos);

protected:
  void doCollision(f32);
  void getTriangles(kgmList<triangle3>& triangles, sphere& s);
  void getBodies(kgmList<kgmBody*>& bodies, sphere& s);

  u32 getTriangles(triangle3 triangles[], u32 max, sphere& s);
  u32 getBodies(kgmBody* bodies[], u32 max, sphere& s);
};
