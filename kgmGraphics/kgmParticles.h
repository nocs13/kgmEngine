#include "../kgmMain/kgmObject.h"
#include "../kgmMain/kgmList.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmParticles: public kgmObject
{
 KGM_OBJECT(kgmParticles)
public:
 struct Particle{
 public: 
  vec3   pos; //current position
  vec3   dir; //move direction
  color  col;   //color
  float  speed; //speed of particle
  float  mass;  //mass of particle
  float  size;	//particle geometry size	
  float  life;  //life time
  float  time;  //current time
  Particle(){   
   memset(this, 0, sizeof(Particle));   
  }
 };

public:
 kgmList<Particle*> m_particles;
 uint				m_count;
 bool				m_loop;
 bool				m_fade;
 bool				m_alpha;
 bool				m_blend;
 bool				m_depth;
 float				m_speed;
 float				m_life;
 color				m_color;

 vec3  location;
 vec3  direction;
 float div_location;
 float div_direction;

 

 float mass;
 float st_size;
 float en_size;

 float time_start;
 float time_update;
public:
 kgmParticles();
 ~kgmParticles();

 void build();
 void init(Particle*);

 void simulate(float t);
};


#pragma once
