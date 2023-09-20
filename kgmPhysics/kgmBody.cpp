#include "kgmBody.h"

kgmBody::kgmBody()
{
  m_type = 0;
  m_mass = 1.0f;
  m_friction = 0.0f;
//  m_force = 0.0f;
  m_velocity      = 0.0f;
  //m_speed_forward = 0.0f;
  //m_speed_side    = 0.0f;
  //m_speed_up      = 0.0f;

  m_valid     = true;
  m_remove    = false;
  m_collision = true;
  m_gravity   = true;
  m_falling   = false;
#ifdef DEBUG
  m_intersect = false;
#endif

  m_position.set(0, 0, 0);
  m_direction.set(1, 0, 0);

  m_shape = ShapeBox;

  //m_extra = 0;
  m_udata = 0;
}

kgmBody::~kgmBody()
{
  m_convex.clear();
}

void kgmBody::collision(kgmBody* body)
{
}

void kgmBody::addShapeSide(vec3 v[])
{
  triangle3 tr(v[0], v[1], v[2]);

  m_convex.add(tr);
}

//void kgmBody::setDirection(vec3& d){
//  m_direction = d;
//  m_direction.normalize();
//  m_rotation.z = (float)asin(m_direction.x);
//}
