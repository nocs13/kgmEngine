#pragma once
#include "../kgmMain/kgmResource.h"
#include "../kgmMain/kgmList.h"
#include "../kgmMain/kgmTab.h"
#include "../kgmMath/kgmMath.h"

class kgmIGraphics;

class kgmShader: public kgmResource{
 KGM_OBJECT(kgmShader)
public:
  enum Type{
    fTime,
    fRandom,
    vEyePosition,
    vEyeDirection,
    vLight,
    mView,
    mProjection,
    mViewProjection,
    tDiffuse,
    tNormal,
    tSpecular,
  };

protected:
 kgmIGraphics*  m_gc;
public:
 void*		             m_shader;
 kgmTab<Type, kgmString> m_input;
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

 kgmIGraphics* gc(){ return m_gc; }
};
