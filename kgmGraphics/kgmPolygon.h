#pragma once
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmArray.h"
#include "kgmMaterial.h"

class kgmPolygon: public kgmResource{
 KGM_OBJECT(kgmPolygon)
public:
struct Vertex{
 vec3 pos, 
      nor;
 uint col;
 vec2 uv[2];
}; 
public:

 kgmArray<Vertex> verts;
 vec3		  nor;	
 kgmMaterial*     material;
 box3		  m_box;

 kgmPolygon(){
  material = 0;
 }
 kgmPolygon(int cnt){
  verts.realloc(cnt);
  material = 0;
 }
 ~kgmPolygon(){
  verts.clear();
  if(material) material->release();
 }
 void calBounds(){
  vec3 max, min;
  int i = 0;
  if(!verts)
   return;
  min = max = verts[(uint)0].pos;
  for(i = 0; i < verts.length(); i++){
   min.x = MIN(min.x, verts[(uint)i].pos.x);
    min.y = MIN(min.y, verts[(uint)i].pos.y);
     min.z = MIN(min.z, verts[(uint)i].pos.z);
   max.x = MAX(max.x, verts[(uint)i].pos.x);
    max.y = MAX(max.y, verts[(uint)i].pos.y);
     max.z = MAX(max.z, verts[(uint)i].pos.z);
  }
  m_box.min = min;
  m_box.max = max;
 }
};

typedef kgmList<kgmPolygon*> kgmPolygonList;
