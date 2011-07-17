#include "kgmParticles.h"
#include "../kgmMain/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmParticles,	kgmObject);

kgmParticles::kgmParticles(){
 m_count = 10;
 m_life = 2000.0f;
 m_speed = 10.7f;
 m_color.color = 0xffffffff;
 m_fade  = true;
 m_alpha = false;
 m_blend = true;
 m_depth = false;
 m_loop  = true;

 direction = vec3(0.0f, 0.0f, 1.0f);

 st_size = 1.1f;
 en_size = 30.f;
}

kgmParticles::~kgmParticles(){
}

void kgmParticles::build(){
 int i = 0;
 float ctime = kgmTime::getTicks();
 for(i = 0; i < m_count; i++){
  Particle*  pr = new Particle();
  m_particles.add(pr);
  init(pr);
 }
}

void kgmParticles::init(Particle* pr){
 if(!pr)
	 return;
 pr->pos = location;
 pr->pos.x += 0.01f * pow(-1.0, rand()%2) * (rand() % 100); 
 pr->pos.y += 0.01f * pow(-1.0, rand()%2) * (rand() % 100); 
 pr->pos.z += 0.01f * pow(-1.0, rand()%2) * (rand() % 100); 
 pr->dir.x = 0.01f * pow(-1.0, rand()%2) * direction.x * (rand() % 100);
 pr->dir.y = 0.01f * pow(-1.0, rand()%2) * direction.y * (rand() % 100);
 pr->dir.z = 0.01f * pow( 1.0, rand()%2) * direction.z * (rand() % 100);
 pr->dir.normalize();
 //pr->dir = direction;

 pr->speed = 0.01f * m_speed * (rand() % 100);
 pr->life  = 0.01f * m_life  * (rand() % 100);
 pr->col = m_color;
 pr->time = 0;
 pr->size = st_size;
}

/*void kgmParticles::doUpdate(float t){
 int i = 0;
 if(m_duration > 0.0f){
  float ltime = kgmTime::getTicks() - m_birth;
  if(ltime > m_duration){
   m_remove = true;
   return;
  }
 }
 for(i = (m_particles.size() - 1); i >= 0; i--){
  Particle*  pr =  m_particles[i];

  pr->pos = pr->pos + pr->dir * (pr->speed * t * 0.001f);
  pr->time += t;

  if(st_size != en_size){
   float d_size = (st_size - en_size) / pr->life;
   pr->size = st_size - (d_size * pr->time);
  }
  if(m_fade){
   uchar a = (uchar)(255.0 - 255.0 * pr->time / pr->life);
   if(m_alpha){
    pr->col.a = a;
   }else{
    pr->col.r = pr->col.g = pr->col.b = a;
   }
  }
  if(pr->time > pr->life){
   if(m_loop){
    init(pr);
   }
  }
 }
}
*/
