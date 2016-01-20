#include "kgmParticles.h"
#include "kgmMaterial.h"
#include "kgmCamera.h"
#include "kgmMesh.h"
#include "../kgmBase/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmParticles, kgmObject);

inline void init_point(kgmMesh::Vertex_P_C_T& v, vec3 pos, u32 col, vec2 uv)
{
  v.pos = pos;
  v.col = col;
  v.uv  = uv;
}

kgmParticles::kgmParticles()
{
  m_count = 10;
  m_life = 2000;
  m_speed = 0.5f;
  m_angle = 0.0f;
  m_color.color = 0xffffffff;
  m_fade  = true;
  m_fall  = false;
  m_loop  = false;

  force     = vec3(0, 0, 0);
  volume    = vec3(0, 0, 0);
  location  = vec3(0, 0, 0);
  direction = vec3(0.0f, 0.0f, 1.0f);

  div_life     = 0.0f;
  div_speed    = 0.0f;
  div_location = 0.0f;
  div_direction = 0.0f;

  st_size  = 0.1f;
  en_size  = 1.0f;

  tex_slide_rows = 0;
  tex_slide_cols = 0;

  m_particles  = null;
  m_typerender = RTypeBillboard;

  m_camera = null;
  m_mesh   = null;
}

kgmParticles::~kgmParticles()
{
  delete [] m_particles;
  delete m_mesh;
}

void kgmParticles::build()
{
  int i = 0;

  if(m_particles)
  {
    delete [] m_particles;

    m_particles = null;
  }

  if(m_mesh)
  {
    delete m_mesh;

    m_mesh = null;
  }

  m_particles = new Particle[m_count];

  if(div_life < 0)     div_life = 0;
  if(div_speed < 0)    div_speed = 0;
  if(div_life > 1.0)   div_life = 1.0;
  if(div_speed > 1.0)  div_speed = 1.0;

  if(m_typerender != RTypeMesh)
  {
    m_mesh = new kgmMesh();

    u32 count;

    if(m_typerender == RTypeBillboard)
      count = m_count * 6;
    else if(m_typerender == RTypePoint)
      count = m_count * 18;

    kgmMesh::Vertex_P_C_T* v = (kgmMesh::Vertex_P_C_T*)m_mesh->vAlloc(count, kgmMesh::FVF_P_C_T);
  }

  for(i = 0; i < m_count; i++)
  {
    init(&m_particles[i]);
  }
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

  float r1    = (float)rand() / RAND_MAX;
  float alpha = m_angle * r1 + m_angle * (1 - r1);

  float r2 = (float)rand()/RAND_MAX;
  float beta = m_angle * r2 + m_angle * (1 - r2);
  pr->dir.x = cos(beta) * cos(alpha);
  pr->dir.y = sin(beta) * cos(alpha);
  pr->dir.z = sin(alpha);

  //pr->dir   = direction + direction * (div_direction / (1 + rand() % m_count));
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

  if(m_particles == null)
    return;

  for(i = m_count; i > 0; i--)
  {
    Particle* pr = &m_particles[i - 1];

    pr->pos = pr->pos + pr->dir * (pr->speed * t * 0.001f);

    if(st_size != en_size)
    {
      float d_size = (st_size - en_size) / pr->life;

      pr->scale = st_size - (d_size * pr->time);
    }

    if(m_fade && pr->col.a > 1)
    {
      uchar a = (uchar)(255.0 - 255.0 * pr->time / pr->life);
      //pr->col.r = pr->col.g = pr->col.b =
      if(a < pr->col.a)
        pr->col.a = a;
    }

    if(m_loop && (pr->time > pr->life))
    {
      init(pr);
    }

    pr->time += t;
  }

  if(m_mesh && m_typerender != RTypeMesh)
  {
    kgmMesh::Vertex_P_C_T* parts = (kgmMesh::Vertex_P_C_T*)m_mesh->vertices();

    if(m_typerender == RTypeBillboard)
    {

    }
    else
    {
      for (s32 i = 0; i < m_count; i++)
      {
        u32   col   = m_particles[i].col.color;
        f32   scale = m_particles[i].scale;
        vec3  pos   = m_particles[i].pos;

        init_point(parts[18 * i + 0],  pos + vec3(-scale,  scale, 0), col, vec2(0, 0));
        init_point(parts[18 * i + 1],  pos + vec3(-scale, -scale, 0), col, vec2(0, 1));
        init_point(parts[18 * i + 2],  pos + vec3(scale, scale, 0),   col, vec2(1, 0));
        init_point(parts[18 * i + 3],  pos + vec3(scale, scale, 0),   col, vec2(1, 0));
        init_point(parts[18 * i + 4],  pos + vec3(-scale, -scale, 0), col, vec2(0, 1));
        init_point(parts[18 * i + 5],  pos + vec3(scale, -scale, 0),  col, vec2(1, 1));
        init_point(parts[18 * i + 6],  pos + vec3(-scale,  0, scale), col, vec2(0, 0));
        init_point(parts[18 * i + 7],  pos + vec3(-scale, 0, -scale), col, vec2(0, 1));
        init_point(parts[18 * i + 8],  pos + vec3(scale, 0, scale),   col, vec2(1, 0));
        init_point(parts[18 * i + 9],  pos + vec3(scale, 0, scale),   col, vec2(1, 0));
        init_point(parts[18 * i + 10], pos + vec3(-scale, 0, -scale), col, vec2(0, 1));
        init_point(parts[18 * i + 11], pos + vec3(scale, 0, -scale),  col, vec2(1, 1));
        init_point(parts[18 * i + 12], pos + vec3(0, -scale,  scale), col, vec2(0, 0));
        init_point(parts[18 * i + 13], pos + vec3(0, -scale, -scale), col, vec2(0, 1));
        init_point(parts[18 * i + 14], pos + vec3(0, scale, scale),   col, vec2(1, 0));
        init_point(parts[18 * i + 15], pos + vec3(0, scale, scale),   col, vec2(1, 0));
        init_point(parts[18 * i + 16], pos + vec3(0, -scale, -scale), col, vec2(0, 1));
        init_point(parts[18 * i + 17], pos + vec3(0, scale, -scale),  col, vec2(1, 1));
      }
    }
  }
}
