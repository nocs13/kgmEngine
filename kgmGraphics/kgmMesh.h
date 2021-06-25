#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"
#include "kgmIMesh.h"

#define KGM_MESH_MAP_CHANNELS 4

class kgmMesh: public kgmIMesh, public kgmResource
{
  KGM_OBJECT(kgmMesh);

public:
  struct LOD
  {
    //kgmArray vertices;
    //kgmArray faces;
  };

public:
  RenderType m_rtype;

  Vertex* m_vertices; // vertex buffer
  u32     m_vcount;   // vertex count
  Face*   m_faces;    // face buffer
  u32     m_fcount;   // face count
  u32     m_fvf;      // flexible vertex format
  u32     m_fff;      // flexible face format

  kgmList<LOD*> m_lods;

  u32     m_group;    // object group id
  box3    m_bound;
  vec3    m_normal;   // average normal.

  kgmString m_mtlId;  // base material name

  kgmMesh* m_linked = null;

public:
  kgmMesh();
  kgmMesh(kgmMesh*);
  kgmMesh(const kgmMesh&);
  ~kgmMesh();

  void rebuild();
  int  genLods(int);

  Vertex* vAlloc(u32 count, FVF f=FVF_P);
  Face*   fAlloc(u32 count, FFF f=FFF_16);
  u32     vsize();
  u32     fsize();
  u32     fvf();
  u32     fff();

  box3 bound()
  {
    return m_bound;
  }

  vec3 normal()
  {
    return m_normal;
  }

  Vertex* vertices()
  {
    if (m_linked)
      return m_linked->vertices();

    return m_vertices;
  }

  Face* faces()
  {
    if (m_linked)
      return m_linked->faces();

    return m_faces;
  }

  u32 vcount()
  {
    if (m_linked)
      return m_linked->vcount();

    return m_vcount;
  }

  u32 fcount()
  {
    if (m_linked)
      return m_linked->fcount();

    return m_fcount;
  }

  void setMtlId(kgmString s)
  {
    m_mtlId = s;
  }

  kgmString getMtlId() const
  {
    return m_mtlId;
  }

  RenderType rtype()
  {
    return m_rtype;
  }
};

typedef kgmList<kgmMesh*> kgmMeshList;
