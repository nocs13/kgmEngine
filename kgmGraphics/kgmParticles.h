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
  u32        m_count;

  bool       m_loop;
  bool       m_fade;
  bool       m_fall;

  color      m_color;

  vec3       force;
  vec3       volume;
  vec3       location;
  vec3       direction;

  float      m_life;
  float      m_speed;
  float      m_angle;

  float      mass;
  float      st_size;
  float      en_size;

  u32        time_start;
  u32        time_update;
  u32        tex_slide_rows;
  u32        tex_slide_cols;

  TypeRender m_typerender;

private:
  float      m_divlife;
  float      m_divspeed;

  kgmMesh*    m_mesh   = null;
  kgmCamera*  m_camera = null;

public:
  kgmParticles();
  ~kgmParticles();

  void build();
  void init(Particle*);

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

  virtual void update(u32 t);
};
