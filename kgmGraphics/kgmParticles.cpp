#include "kgmParticles.h"
#include "kgmMaterial.h"
#include "../kgmBase/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmParticles, kgmObject);

kgmParticles::kgmParticles()
{
  m_count = 10;
  m_life = 2000;
  m_speed = 0.5f;
  m_color.color = 0xffffffff;
  m_fade  = true;
  m_fall  = false;
  m_loop  = true;

  force     = vec3(0, 0, 0);
  volume    = vec3(0, 0, 0);
  location  = vec3(0, 0, 0);
  direction = vec3(0.0f, 0.0f, 1.0f);

  div_life  = 0.0f;
  div_speed = 0.0f;
  div_location = 0.0f;
  div_direction = 0.0f;

  st_size   = 0.1f;
  en_size   = 1.0f;

  tex_slide_rows = 0;
  tex_slide_cols = 0;

  m_particles  = null;
  m_typerender = RTypeBillboard;
}

kgmParticles::~kgmParticles()
{
  if(m_particles)
    delete [] m_particles;
}

void kgmParticles::build(){
  int i = 0;

  if(m_particles)
    delete [] m_particles;

  m_particles = new Particle[m_count];

  if(div_life < 0) div_life = 0;
  if(div_speed < 0) div_speed = 0;
  if(div_life > 1.0) div_life = 1.0;
  if(div_speed > 1.0) div_speed = 1.0;

  for(i = 0; i < m_count; i++)
    init(&m_particles[i]);
}

void kgmParticles::init(Particle* pr)
{
  if(!pr)
    return;

  //srand (time(NULL));
  float s = m_speed / (1 + rand() % (10 * m_count));
  float l = m_life  / (1 + rand() % (10 * m_count));

  //if(l > m_life) l = m_life;

  pr->pos.x = 0.5f * volume.x * pow(-1.0, rand() % 2) / (1 + rand() % m_count);
  pr->pos.y = 0.5f * volume.y * pow(-1.0, rand() % 2) / (1 + rand() % m_count);
  pr->pos.z = 0.5f * volume.z * pow(-1.0, rand() % 2) / (1 + rand() % m_count);
  pr->dir   = direction + direction * (div_direction / (1 + rand() % m_count));
  pr->dir.normalize();

  pr->speed = m_speed - div_speed * s;
  pr->life  = m_life  - div_life  * l;
  pr->col   = m_color;
  pr->scale = st_size;
  pr->time  = 0;
  pr->mass  = 0;
  pr->mesh  = null;

  if(pr->life < 0) pr->life = 0;
  if(pr->speed < 0) pr->speed = 0.0f;
}

void kgmParticles::update(u32 t)
{
  int i = 0;

  for(i = m_count; i > 0; i--)
  {
    Particle* pr = &m_particles[i - 1];

    pr->pos = pr->pos + pr->dir * (pr->speed * t * 0.001f);
    pr->time += t;

    if(st_size != en_size)
    {
      float d_size = (st_size - en_size) / pr->life;

      pr->scale = st_size - (d_size * pr->time);
    }

    if(m_fade)
    {
      uchar a = (uchar)(255.0 - 255.0 * pr->time / pr->life);

      pr->col.r = pr->col.g = pr->col.b = pr->col.a = a;
    }

    if(m_loop && (pr->time > pr->life))
    {
        init(pr);
    }
  }
}
