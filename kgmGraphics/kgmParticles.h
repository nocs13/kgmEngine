#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmMesh;
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
    kgmMesh* mesh;  //ref mesh
  };

public:
  Particle*  m_particles;
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

  float      div_life;
  float      div_speed;
  float      div_location;
  float      div_direction;

  float      mass;
  float      st_size;
  float      en_size;

  u32        time_start;
  u32        time_update;
  u32        tex_slide_rows;
  u32        tex_slide_cols;

  TypeRender m_typerender;

public:
  kgmParticles();
  ~kgmParticles();

  void build();
  void init(Particle*);

  virtual void update(u32 t);
};


#pragma once
