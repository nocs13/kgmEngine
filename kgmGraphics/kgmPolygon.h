#pragma once
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmArray.h"
#include "kgmMaterial.h"

class kgmPolygon: public kgmResource
{
  KGM_OBJECT(kgmPolygon);

public:
  struct Vertex
  {
    vec3 pos,
    nor;
    u32 col;
    vec2 uv[2];
  };

protected:
  ~kgmPolygon()
  {
    verts.clear();
  }

public:
  kgmArray<Vertex> verts;

  vec3  nor;
  box3  m_box;

  kgm_ptr<kgmMaterial> material;

public:
  kgmPolygon()
  {
  }

  kgmPolygon(int cnt)
  {
    verts.realloc(cnt);
    material = 0;
  }

  void calBounds()
  {
    vec3 max, min;
    int i = 0;

    if(!verts)
      return;

    min = max = verts[(u32)0].pos;

    for(i = 0; i < verts.length(); i++)
    {
      min.x = MIN(min.x, verts[(u32)i].pos.x);
      min.y = MIN(min.y, verts[(u32)i].pos.y);
      min.z = MIN(min.z, verts[(u32)i].pos.z);
      max.x = MAX(max.x, verts[(u32)i].pos.x);
      max.y = MAX(max.y, verts[(u32)i].pos.y);
      max.z = MAX(max.z, verts[(u32)i].pos.z);
    }

    m_box.min = min;
    m_box.max = max;
  }
};

typedef kgmList<kgmPolygon*> kgmPolygonList;
