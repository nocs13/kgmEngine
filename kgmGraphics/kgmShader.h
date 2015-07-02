#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmMath/kgmMath.h"

class kgmIGC;

class kgmShader: public kgmResource
{
 KGM_OBJECT(kgmShader)

public:
  enum Input
  {
    IN_MTX4_PROJ      = 1 << 0,
    IN_MTX4_VIEW      = 1 << 1,
    IN_MTX4_TRAN      = 1 << 2,
    IN_MTX3_NORM      = 1 << 3,
    IN_VEC3_EYEPOS    = 1 << 4,
    IN_VEC3_EYEDIR    = 1 << 5,
    IN_VEC4_COLOR     = 1 << 6,
    IN_VEC4_LIGHT     = 1 << 7,
    IN_VEC4_LIGHTS    = 1 << 8,
    IN_VEC1_TIME      = 1 << 9,
    IN_VEC1_RANDOM    = 1 << 10,
    IN_MAP_COLOR      = 1 << 16,
    IN_MAP_NORMAL     = 1 << 17,
    IN_MAP_SPECULAR   = 1 << 18
  };

public:
  enum Shader
  {
    TypeNone,
    TypeBase,
    TypeSkin,
    TypePoor,
    TypeBlend,
    TypeWater,
    TypeRiver,
    TypeOcean,
    TypeGlass,
    TypeFlame,
    TypeMirror,
    TypeLights,
    TypeIce,
    TypeExtend = 0x1f
  };

protected:
 kgmIGC* m_gc;
 u8      m_maps;

public:
 u32   m_input;
 void* m_shader;

protected:
 ~kgmShader();

public:
 kgmShader(kgmIGC* g = 0);

 void start();
 void stop();

 void set(const char*, float&, int count = 1);
 void set(const char*, vec2&,  int count = 1);
 void set(const char*, vec3&,  int count = 1);
 void set(const char*, vec4&,  int count = 1);
 void set(const char*, mtx4&,  int count = 1);
 void set(const char*, mtx3&,  int count = 1);
 void set(const char*, int,    int count = 1);
 void attr(int, const char*);

 static const char* toString(Shader);
 static Shader toType(const char*);

 void addInput(Input i)
 {
   m_input |= i;
 }

 void delInput(Input i)
 {
   m_input &= (~i);
 }

 bool hasInpup(Input i) const
 {
   return (bool)(m_input & (~i));
 }

 kgmIGC* gc() const { return m_gc; }
};
