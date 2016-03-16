#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmMesh;
class kgmCamera;
class kgmMaterial;

class kgmParticles: public kgmObject
{
  KGM_OBJECT(kgmParticles);

public:
  enum TypeRender
  {
    RTypeBillboard,   //camera oriented
    RTypePoint,       //3 intersect flat
    RTypeMesh         //point has own mesh refference
  };

  struct Particle
  {
    vec3     pos;   //current position
    vec3     dir;   //move direction
    color    col;   //color
    float    speed; //speed of particle
    float    scale; //particle geometry size
    float    mass;  //mass of particle
    u32      life;  //life time
    u32      time;  //current time

    kgmMesh* mesh = null;  //ref mesh
  };

public:
  Particle*  m_particles = null;

  bool       m_fall;

  color      m_color;

  vec3       force;
  vec3       volume;

  u32        time_start;
  u32        time_update;
  u32        tex_slide_rows;
  u32        tex_slide_cols;

  TypeRender m_typerender;

private:
  u32        m_count;

  bool       m_loop;
  bool       m_fade;

  float      m_life;
  float      m_mass;
  float      m_size;
  float      m_esize;
  float      m_speed;
  float      m_angle;
  float      m_divlife;
  float      m_divspeed;

  mtx4       m_transform;

  kgmMesh*    m_mesh   = null;
  kgmCamera*  m_camera = null;

public:
  kgmParticles();
  ~kgmParticles();

  void build();
  void init(Particle*);

  u32 count() const
  {
    return m_count;
  }

  void count(u32 c)
  {
    m_count = c;
  }

  bool loop() const
  {
    return m_loop;
  }

  void loop(bool l)
  {
    m_loop = l;
  }

  bool fade() const
  {
    return m_fade;
  }

  void fade(bool f)
  {
    m_fade = f;
  }

  f32 life() const
  {
    return m_life;
  }

  void life(f32 a)
  {
    m_life = a;
  }

  f32 mass() const
  {
    return m_mass;
  }

  void mass(f32 a)
  {
    m_mass = a;
  }

  f32 size() const
  {
    return m_size;
  }

  void size(f32 a)
  {
    m_size = a;
  }

  f32 speed() const
  {
    return m_speed;
  }

  void speed(f32 a)
  {
    m_speed = a;
  }

  f32 esize() const
  {
    return m_esize;
  }

  void esize(f32 a)
  {
    m_esize = a;
  }

  f32 angle() const
  {
    return m_angle;
  }

  void angle(f32 a)
  {
    m_angle = a;
  }

  f32 divlife() const
  {
    return m_divlife;
  }

  void divlife(f32 s)
  {
    if(s > 1.0f) s = 1.0f;
    else if(s < 0.0f) s = 0.0f;

    m_divlife = s;
  }

  f32 divspeed() const
  {
    return m_divspeed;
  }

  void divspeed(f32 s)
  {
    if(s > 1.0f) s = 1.0f;
    else if(s < 0.0f) s = 0.0f;

    m_divspeed = s;
  }

  void camera(kgmCamera* c)
  {
    m_camera = c;
  }

  kgmCamera* camera() const
  {
    return m_camera;
  }

  kgmMesh* getMesh()
  {
    return m_mesh;
  }

  void typeRender(TypeRender t)
  {
    m_typerender = t;
  }

  TypeRender typeRender() const
  {
    return m_typerender;
  }

  void transform(mtx4& m)
  {
    m_transform = m;
  }

  virtual void update(u32 t);
};
