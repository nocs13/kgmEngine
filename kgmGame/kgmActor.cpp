#include "kgmActor.h"
KGMOBJECT_IMPLEMENT(kgmActor,	kgmObject);

kgmActor::kgmActor()
{
 m_enable = true;
 m_visible = true;
 m_active = true;
 m_remove = false;
 m_culled = false;

 m_visual = new kgmVisual();

 m_body = new kgmBody();
 m_body->m_udata = this;
 m_body->m_mass = 1.0f;
 m_body->m_position = vec3(0, 0, 1);
 m_body->m_rotation = vec3(0, 0, 0);
 m_body->m_direction = vec3(1, 0, 0);
 m_body->m_bound.min = vec3(-1, -1, 0);
 m_body->m_bound.max = vec3(1, 1, 2);

 m_transform.identity();
 m_birth = kgmTime::getTicks();
 m_health = 1.0f;
 m_parent = null;
}

kgmActor::~kgmActor(){
  for(int i = 0; i < m_childs.length(); i++)
    m_childs[i]->release();

  m_childs.clear();

  delete m_body;
  m_visual->release();
}

void kgmActor::exit(){
}

void kgmActor::init(){
 m_transform.translate(m_body->m_position);
}

void kgmActor::tick(u32 time){
}

void kgmActor::remove(){
 m_remove = true;
}

void kgmActor::enable(bool s){
 m_enable = s;
}

void kgmActor::active(bool a){
 m_active = a;
}

void kgmActor::visible(bool a){
 m_visible = a;
}

bool kgmActor::enabled(){
 return m_enable;
}

bool kgmActor::removed(){
 return m_remove;
}

void kgmActor::setPosition(vec3& v){
 m_body->translate(v.x, v.y, v.z);
}

void kgmActor::setRotation(vec3& r){
 m_body->rotate(r.x, r.y, r.z);
}

void kgmActor::setRotation(quat& r){
 m_body->rotate(r);
}

void kgmActor::setDirection(vec3& d){
 m_body->m_direction = d;
 m_body->m_direction.normalize();
 m_body->m_rotation.z = (float)acos(m_body->m_direction.x);
}

void kgmActor::setForce(float f, vec3& d){
  if(d.length() == 0.0f)
    m_body->m_EF = m_body->m_direction * f;
  else
    m_body->m_EF = d * f;
}

void kgmActor::setParent(kgmActor* a){
 m_parent = a;
}

void kgmActor::addChild(kgmActor* a){
 if(!a)
  return;
 m_childs.add(a);
 a->increment();
}

void kgmActor::delChild(kgmActor* a){
 if(!a)
  return;
 for(int i = 0; i < m_childs.length(); i++)
  if(a == m_childs[i]){
   a->release();
   m_childs.erase(i);
   return;
  }
}
