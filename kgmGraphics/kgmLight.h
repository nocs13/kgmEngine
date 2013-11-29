#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "kgmTexture.h"

class kgmLight: public kgmObject{
 KGM_OBJECT(kgmLight)
public:
  static float LIGHT_RANGE;
public:
 vec3  position;   //light position
 vec3  direction;  //light direction, if 0 then light is omni
 vec4  color, specular;
 float intensity;  //light intensity, 1 mean 10 meter range
 float angle;      //light spot angle if directional
 bool  ison;       //light switched on
 bool  shadows;    //light drop shadows, on shadow map
 
 kgmTexture shadowmap;  //light shadow map depth texture

 kgmString m_id;
 u32       m_group;     // object group id

public:
 kgmLight(){
  position  = vec3(0, 0, 0);
  direction = vec3(0, 0, 0);

  color     = vec4(.7, .7, .7, 1);
  specular  = vec4(.7, .7, .7, 1);

  intensity = 1.0f;
  angle = 0.0f;

  shadows = false;
  ison = true;

  shadowmap.m_type = kgmTexture::Type_RT_Depth;
  m_group = 0;
 }

 virtual ~kgmLight()
 {
 }
};


typedef kgmList<kgmLight*> kgmLights;



