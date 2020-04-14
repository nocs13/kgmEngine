#include "kgmParticles.h"
#include "kgmMaterial.h"
#include "kgmCamera.h"
#include "kgmMesh.h"
#include "../kgmBase/kgmTime.h"

inline void init_point(kgmMesh::Vertex_P_C_T& v, vec3 pos, u32 col, vec2 uv)
{
  v.pos = pos;
  v.col = kgmColor::toVector(col);
  v.uv  = uv;
}

inline float p_rand()
{
  return (float) rand() / (float) RAND_MAX;
}

kgmParticles::kgmParticles()
{
  m_count = 100;
  m_life  = 2000;
  m_speed = 1.0;
  m_noise = 0.1;
  m_gravity = 9.8;

  m_color.color = 0xffffffff;
  m_fade  = true;
  m_lock  = false;
  m_fall  = false;
  m_loop  = true;

  m_force  = vec3(0, 0, 0);
  m_volume = vec3(0, 0, 0);

  m_divlife     = 0.5f;
  m_divspeed    = 0.5f;

  m_size  = .1f;
  m_esize  = .1f;

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
  u32 i = 0;

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

  if(m_typerender != RTypeMesh)
  {
    m_mesh = new kgmMesh();

    u32 count = 0;

    if(m_typerender == RTypeBillboard)
      count = m_count * 6;
    else if(m_typerender == RTypePoint)
      count = m_count * 18;

    m_mesh->vAlloc(count, kgmMesh::FVF_P_C_T);
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

  pr->pos.x = m_position.x + -0.5f * m_volume.x + m_volume.x * p_rand();
  pr->pos.y = m_position.y + -0.5f * m_volume.y + m_volume.y * p_rand();
  pr->pos.z = m_position.z + -0.5f * m_volume.z + m_volume.z * p_rand();

  pr->dir.x = m_direction.x - (m_noise * m_direction.x * p_rand());
  pr->dir.y = m_direction.y - (m_noise * m_direction.y * p_rand());
  pr->dir.z = m_direction.z - (m_noise * m_direction.z * p_rand());

  pr->dir.normalize();

  pr->speed = m_speed - m_speed * m_divspeed * p_rand();
  pr->life  = 1000 * (m_life  - m_life  * m_divlife  * p_rand());
  pr->col   = m_color;
  pr->scale = m_size;
  pr->time  = 0;
  pr->mass  = m_mass * pr->scale;

  pr->speed *= 0.001;

  if(pr->speed < 0)
    pr->speed = 0.0f;
}

void kgmParticles::update(u32 t)
{
  int i = 0;

  if(m_particles == null)
  {
    return;
  }

  float ftime = t * 0.001f;

  for(i = m_count; i > 0; i--)
  {
    Particle* pr = &m_particles[i - 1];

    if(pr->time > pr->life)
    {
      if(m_loop)
        init(pr);
      else
        continue;
    }

    pr->pos = pr->pos + pr->dir * (pr->speed * ftime);

    pr->speed -= (m_gravity * ftime * 0.001f);

    if(m_size != m_esize)
    {
      float d_size = (m_size - m_esize) / (1.0f + pr->life);

      pr->scale = m_size - (d_size * pr->time);
    }

    if(m_fade && pr->col.a > 1)
    {
      uchar a = (uchar)(255.0 - 255.0 * pr->time / pr->life);

      if(a < pr->col.a)
        pr->col.a = a;
    }

    pr->time += t;
  }

  if(m_mesh && m_typerender != RTypeMesh)
  {
    kgmMesh::Vertex_P_C_T* parts = (kgmMesh::Vertex_P_C_T*)m_mesh->vertices();

    if(m_typerender == RTypeBillboard)
    {
      vec3    rv, uv;

      if (!m_camera)
        return;

      rv = vec3(m_camera->mView.m[0], m_camera->mView.m[2], m_camera->mView.m[1]);
      rv.normalize();
      uv = rv.cross(m_camera->mDir);
      uv.normalize();

      kgmMesh::Vertex_P_C_T* points = (kgmMesh::Vertex_P_C_T*)m_mesh->vertices();

      for(u32 i = 0; i < m_count; i++)
      {
        s32    vi = i * 6;
        vec3   pos   = m_particles[i].pos;
        float  scale = m_particles[i].scale;
        float  time  = m_particles[i].time;
        float  life  = m_particles[i].life;
        vec3   crv = rv * scale,
               cuv = uv * scale;

        float   txu_s = 0.0f, txv_s = 0.0f;
        float   txu_e = 1.0f, txv_e = 1.0f;

        if(tex_slide_cols > 1 || tex_slide_rows > 1)
        {
          u32 frames = tex_slide_cols * tex_slide_rows;
          u32 frame  = (u32)((float)frames * time / life);

          float sw = (tex_slide_cols > 1) ? (1.0f / tex_slide_cols) : (1.0f);
          float sh = (tex_slide_rows > 1) ? (1.0f / tex_slide_rows) : (1.0f);

          u32   ir = frame / tex_slide_cols;
          u32   ic = frame % tex_slide_cols;

          txu_s = sw * ic;
          txv_s = sh * ir;
          txu_e = sw * (ic + 1);
          txv_e = sh * (ir + 1);
        }

        points[vi].pos = (pos - crv + cuv);
        points[vi].uv = vec2(txu_s, txv_s);
        points[vi + 1].pos = (pos - crv - cuv);
        points[vi + 1].uv = vec2(txu_s, txv_e);
        points[vi + 2].pos = (pos + crv + cuv);
        points[vi + 2].uv = vec2(txu_e, txv_s);

        points[vi + 3].pos = (pos + crv - cuv);
        points[vi + 3].uv = vec2(txu_e, txv_e);
        points[vi + 4].pos = (pos + crv + cuv);
        points[vi + 4].uv = vec2(txu_e, txv_s);
        points[vi + 5].pos = (pos - crv - cuv);
        points[vi + 5].uv = vec2(txu_s, txv_e);

        points[vi].col = points[vi + 1].col =
        points[vi + 2].col = points[vi + 3].col =
        points[vi + 4].col = points[vi + 5].col = kgmColor::toVector(m_particles[i].col.color);
      }
    }
    else
    {
      for (u32 i = 0; i < m_count; i++)
      {
        s32   vi = i * 18;
        u32   col   = m_particles[i].col.color;
        f32   scale = m_particles[i].scale;
        vec3  pos   = m_particles[i].pos;

        init_point(parts[vi + 0],  pos + vec3(-scale,  scale, 0), col, vec2(0, 0));
        init_point(parts[vi + 1],  pos + vec3(-scale, -scale, 0), col, vec2(0, 1));
        init_point(parts[vi + 2],  pos + vec3(scale, scale, 0),   col, vec2(1, 0));
        init_point(parts[vi + 3],  pos + vec3(scale, scale, 0),   col, vec2(1, 0));
        init_point(parts[vi + 4],  pos + vec3(-scale, -scale, 0), col, vec2(0, 1));
        init_point(parts[vi + 5],  pos + vec3(scale, -scale, 0),  col, vec2(1, 1));
        init_point(parts[vi + 6],  pos + vec3(-scale,  0, scale), col, vec2(0, 0));
        init_point(parts[vi + 7],  pos + vec3(-scale, 0, -scale), col, vec2(0, 1));
        init_point(parts[vi + 8],  pos + vec3(scale, 0, scale),   col, vec2(1, 0));
        init_point(parts[vi + 9],  pos + vec3(scale, 0, scale),   col, vec2(1, 0));
        init_point(parts[vi + 10], pos + vec3(-scale, 0, -scale), col, vec2(0, 1));
        init_point(parts[vi + 11], pos + vec3(scale, 0, -scale),  col, vec2(1, 1));
        init_point(parts[vi + 12], pos + vec3(0, -scale,  scale), col, vec2(0, 0));
        init_point(parts[vi + 13], pos + vec3(0, -scale, -scale), col, vec2(0, 1));
        init_point(parts[vi + 14], pos + vec3(0, scale, scale),   col, vec2(1, 0));
        init_point(parts[vi + 15], pos + vec3(0, scale, scale),   col, vec2(1, 0));
        init_point(parts[vi + 16], pos + vec3(0, -scale, -scale), col, vec2(0, 1));
        init_point(parts[vi + 17], pos + vec3(0, scale, -scale),  col, vec2(1, 1));
      }
    }
  }
}
