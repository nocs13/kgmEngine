#include "kgmActor.h"
KGMOBJECT_IMPLEMENT(kgmActor,	kgmObject);


kgmActor::kgmActor(){
 m_enable = true;
 m_visible = true;
 m_active = true;
 m_remove = false;
 m_culled = false;

 m_udata = this;

 m_mesh = 0;

 m_mass = 1.0f;
 m_life = 1.0f;
 m_position = vec3(0, 0, 1);
 m_rotation = vec3(0, 0, 0);
 m_direction = vec3(1, 0, 0);
 m_bound.min = vec3(-1, -1, 0);
 m_bound.max = vec3(1, 1, 2);

 m_birth = kgmTime::getTicks();

 m_parent = 0;
}
kgmActor::~kgmActor(){
  for(int i = 0; i < m_childs.length(); i++)
    m_childs[i]->release();

  for(int i = 0; i < m_sections.length(); i++){
    m_sections[i].clear();
  }

  m_childs.clear();
}

void kgmActor::update(float time){
}

void kgmActor::prepare(){
 m_transform.translate(m_position);
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
 m_position = v;
 translate(v.x, v.y, v.z);
}
void kgmActor::setRotation(vec3& r){
 rotate(r.x, r.y, r.z);
}
void kgmActor::setRotation(quat& r){
 //Rotation = r.;
 //rotate(r.x, r.y, r.z);
 //rotate(r);
}
void kgmActor::setDirection(vec3& d){
 m_direction = d;
 m_direction.normalize();
 m_rotation.z = (float)acos(m_direction.x);
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



