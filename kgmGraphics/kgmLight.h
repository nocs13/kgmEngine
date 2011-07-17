#pragma once
#include "../kgmMain/kgmObject.h"
#include "../kgmMain/kgmList.h"
#include "kgmTexture.h"

class kgmLight: public kgmObject{
 KGM_OBJECT(kgmLight)
public:
 vec3  position;
 vec4  ambient, 
       diffuse, 
       specular;
 float range;
 float intensity;


 vec3  direction;
 uint  type;
 bool  ison;		//light switched on
 bool  shadows;
 bool  shadesmooth;
 
 kgmTexture shadowmap;

 kgmString m_id;
public:
 kgmLight(){
  position = vec3(0, 0, 0);
  ambient  = vec4(.7, .7, .7, 1);
  diffuse  = vec4(.7, .7, .7, 1);
  specular = vec4(.7, .7, .7, 1);
  range = 1000.0f;
  intensity = 1.0f;
  direction = vec3(0, 0, -1);
  shadesmooth = true;
  shadows = true;
  ison = true;

  shadowmap.m_type = kgmTexture::Type_RT_Depth;
 }
 virtual ~kgmLight(){
 }
};


typedef kgmList<kgmLight*> kgmLightList;



