#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"

#define KGM_MESH_MAP_CHANNELS 4

class kgmMesh: public kgmResource
{
  KGM_OBJECT(kgmMesh)

public:
  enum RenderType
  {
    RT_TRIANGLE,
    RT_POINT,
    RT_LINE,
    RT_TRIANGLESTRIP
  };

  enum FVF
  {
    FVF_P,                  // = (gcv_xyz) ,
    FVF_P_C,                // = (gcv_xyz|gcv_col) ,
    FVF_P_N,                // = (gcv_xyz|gcv_nor) ,
    FVF_P_T,                // = (gcv_xyz|gcv_uv0) ,
    FVF_P_N_T,              // = (gcv_xyz|gcv_nor|gcv_uv0) ,
    FVF_P_C_T,              // = (gcv_xyz|gcv_col|gcv_uv0) ,
    FVF_P_N_T2,             // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
    FVF_P_N_T_BW_BI,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
    FVF_P_N_T2_BW_BI,       // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
  };

  enum FFF
  {
    FFF_16,
    FFF_32
  };

  struct Vertex
  {
    vec3 pos;
  };

  struct Vertex_P_C: public Vertex
  {
    vec4 col;
  };

  struct Vertex_P_T: public Vertex
  {
    vec2 uv;
  };

  struct Vertex_P_N: public Vertex
  {
    vec3 nor;
  };

  struct Vertex_P_N_T: public Vertex_P_N
  {
    vec2 uv;
  };

  struct Vertex_P_N_T2: public Vertex_P_N
  {
    vec2 uv[2];
  };

  struct Vertex_P_C_T: public Vertex_P_C
  {
    vec2 uv;
  };

  struct Vertex_P_N_T_BW_BI: public Vertex_P_N_T
  {
    float bw[4];
    int   bi[4];
  };

  struct Vertex_P_N_T2_BW_BI: public Vertex_P_N_T2
  {
    float bw[4];
    int   bi[4];
  };

  struct Face
  {
  };

  struct Face_16: public Face
  {
    union
    {
      struct{ u16 a, b, c; };
      u16 f[3];
    };
  };

  struct Face_32: public Face
  {
    union
    {
      struct{ u32 a, b, c; };
      u32 f[3];
    };
  };

public:
  RenderType m_rtype;

  Vertex* m_vertices; // vertex buffer
  u32     m_vcount;   // vertex count
  Face*   m_faces;    // face buffer
  u32     m_fcount;   // face count
  u32     m_fvf;      // flexible vertex format
  u32     m_fff;      // flexible face format

  //kgmList<LOD*> m_lods;

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

  Vertex* vertices();
  Face*   faces();

  u32     vcount();
  u32     fcount();
  u32     vsize();
  u32     fsize();
  u32     fvf();
  u32     fff();

  RenderType rtype();
  box3       bound();

  vec3 normal()
  {
    return m_normal;
  }

  void setMtlId(kgmString s)
  {
    m_mtlId = s;
  }

  kgmString getMtlId() const
  {
    return m_mtlId;
  }
};

typedef kgmList<kgmMesh*> kgmMeshList;
