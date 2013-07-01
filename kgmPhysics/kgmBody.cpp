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
  for(i = 0; i < m_convex.size(); i++)
    delete m_convex[i];
  m_convex.clear();
}

void kgmBody::collision(kgmBody* body){
}

void kgmBody::translate(float x, float y, float z){
  m_position = vec3(x, y, z);
}

void kgmBody::rotate(float x, float y, float z){
  m_rotation.x = x;
  m_rotation.y = y;
  m_rotation.z = z;
  m_quaternion.euler(m_rotation);
  mtx4 m(m_quaternion);
  vec3 v(1, 0, 0);
  m_direction = m * v;
  m_direction.normalize();
  //m_direction = vec3(-sin(m_rotation.z), cos(m_rotation.z), 0);
}

void kgmBody::rotate(quat& q){
  m_quaternion = q;
  mtx4 m(m_quaternion);
  m.angles(m_rotation);
  //m_direction = vec3(sin(m_rotation.z), cos(m_rotation.z), 0);
}

void kgmBody::transform(mtx4& mtr){
  mtx4 tr(m_position), rt(m_quaternion);
  mtr = rt * tr;
}

void kgmBody::setPosition(vec3& v){
  m_position = v;
  translate(v.x, v.y, v.z);
}

void kgmBody::setDirection(vec3& d){
  m_direction = d;
  m_direction.normalize();
  m_rotation.z = (float)asin(m_direction.x);
}
