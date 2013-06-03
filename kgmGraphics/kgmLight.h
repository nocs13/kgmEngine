#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "kgmTexture.h"

class kgmLight: public kgmObject{
 KGM_OBJECT(kgmLight)
public:
 vec3  position;
 vec4  color,
       specular;
 float intensity;


 vec3  direction;
 u32   type;
 bool  ison;		//light switched on
 bool  shadows;
 
 kgmTexture shadowmap;

 kgmString m_id;
public:
 kgmLight(){
  position  = vec3(0, 0, 0);
  color     = vec4(.7, .7, .7, 1);
  specular  = vec4(.7, .7, .7, 1);
  intensity = 1.0f;
  direction = vec3(0, 0, -1);
  shadows = true;
  ison = true;

  shadowmap.m_type = kgmTexture::Type_RT_Depth;
 }
 virtual ~kgmLight(){
 }
};


typedef kgmList<kgmLight*> kgmLightList;



