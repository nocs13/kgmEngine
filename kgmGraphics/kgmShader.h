#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmMath/kgmMath.h"

class kgmIGraphics;

class kgmShader: public kgmResource{
 KGM_OBJECT(kgmShader)
public:
  enum Input
  {
    IN_MTX4_VIEW         = 1 << 0,
    IN_MTX4_PROJ         = 1 << 1,
    IN_MTX4_TRAN         = 1 << 2,
    IN_VEC3_EYEPOS       = 1 << 3,
    IN_VEC3_EYEDIR       = 1 << 4,
    IN_VEC4_COLOR        = 1 << 5,
    IN_VEC4_LIGHTPOS     = 1 << 6,
    IN_VEC4_LIGHTDIR     = 1 << 7,
    IN_VEC4_LIGHTSPOS    = 1 << 8,
    IN_VEC4_LIGHTSDIR    = 1 << 9,
    IN_VEC1_TIME         = 1 << 10,
    IN_VEC1_RANDOM       = 1 << 11,
  };

  enum Map
  {
    MAP_COLOR      = 1 << 0,
    MAP_NORMAL     = 1 << 1,
    MAP_SPECULAR   = 1 << 2,
  };
protected:
 kgmIGraphics*  m_gc;
 u32            m_input;
 u8             m_maps;
public:
 void*          m_shader;

public:
 kgmShader(kgmIGraphics* g = 0);
 ~kgmShader();

 void start();
 void stop();

 void set(const char*, float&);
 void set(const char*, vec2&);
 void set(const char*, vec3&);
 void set(const char*, vec4&);
 void set(const char*, mtx4&);
 void set(const char*, int);
 void set(const char*, void*);
 void attr(int, const char*);

 kgmIGraphics* gc(){ return m_gc; }
};
