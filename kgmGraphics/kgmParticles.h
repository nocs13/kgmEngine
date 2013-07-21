#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmMaterial;

class kgmParticles: public kgmObject
{
  KGM_OBJECT(kgmParticles);
public:
  enum TypeRender
  {
    RTypeBillboard,
    RTypeParticle,
    RTypeMesh
  };

  struct Particle
  {
    vec3   pos;   //current position
    vec3   dir;   //move direction
    color  col;   //color
    float  speed; //speed of particle
    float  scale; //particle geometry size
    float  mass;  //mass of particle
    u32    life;  //life time
    u32    time;  //current time
  };

public:
  Particle*  m_particles;
  u32        m_count;

  bool       m_loop;
  bool       m_fade;
  bool       m_fall;
  bool       m_alpha;
  bool       m_blend;
  bool       m_depth;

  float      m_speed;
  float      m_life;

  color      m_color;

  vec3       location;
  vec3       direction;

  float      div_location;
  float      div_direction;

  float      mass;
  float      st_size;
  float      en_size;

  u32        time_start;
  u32        time_update;

  kgmMaterial* m_material;
  TypeRender   m_typerender;
public:
  kgmParticles();
  ~kgmParticles();

  void build();
  void init(Particle*);
  void update(u32 t);
  void set(kgmMaterial*);
};


#pragma once
