#include "kgmBody.h"
KGMOBJECT_IMPLEMENT(kgmBody, kgmObject);

kgmBody::kgmBody()
{
  m_mass = 1.0f;
  m_friction = 0.0f;
//  m_force = 0.0f;
  m_velocity      = 0.0f;
  m_speed_forward = 0.0f;
  m_speed_side    = 0.0f;
  m_speed_up      = 0.0f;

  m_valid     = true;
  m_remove    = false;
  m_collision = true;
  m_gravity   = true;
  m_falling   = false;
#ifdef TEST
  m_intersect = false;
#endif

  m_position = vec3(0, 0, 0);
  m_direction = vec3(1, 0, 0);

  m_shape = ShapeBox;

  //m_extra = 0;
  m_udata = 0;
}

kgmBody::kgmBody(void* udata)
{
  m_mass = 1.0f;
  m_friction = 0.0f;
//  m_force = 0.0f;
  m_velocity      = 0.0f;
  m_speed_forward = 0.0f;
  m_speed_side    = 0.0f;
  m_speed_up      = 0.0f;

  m_valid     = true;
  m_remove    = false;
  m_collision = true;
  m_gravity   = true;
  m_falling   = false;

  m_position = vec3(0, 0, 0);
  m_direction = vec3(0, 1, 0);

  m_shape = ShapeBox;

  m_udata = udata;
}

kgmBody::~kgmBody(){
  int i;

  m_convex.clear();
}

void kgmBody::collision(kgmBody* body){
}

void kgmBody::addShapeSide(vec3 v[])
{
  kgmPhysicTypes::Triangle tr(v[0], v[1], v[2]);

  m_convex.add(tr);
}

//void kgmBody::setDirection(vec3& d){
//  m_direction = d;
//  m_direction.normalize();
//  m_rotation.z = (float)asin(m_direction.x);
//}
