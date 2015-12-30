#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"
//#include "../kgmMath/kgmVector3d.h"
//#include "../kgmMath/kgmVector2d.h"
//#include "../kgmMath/kgmBox3d.h"
//#include "../kgmMath/kgmQuaternion.h"
//#include "../kgmMath/kgmMatrix3d.h"

#include "kgmMaterial.h"
#include "kgmSkeleton.h"
#include "kgmAnimation.h"

#define KGM_MESH_MAP_CHANNELS 4


class kgmMesh: public kgmResource
{
  KGM_OBJECT(kgmMesh);

public:
  enum RenderType
  {
    RT_TRIANGLE,
    RT_POINT,
    RT_LINE
  };

  enum FVF
  {
    FVF_P,        // = (gcv_xyz) ,
    FVF_P_C,        // = (gcv_xyz|gcv_col) ,
    FVF_P_N,        // = (gcv_xyz|gcv_nor) ,
    FVF_P_T,        // = (gcv_xyz|gcv_uv0) ,
    FVF_P_C_T,      // = (gcv_xyz|gcv_col|gcv_uv0) ,
    FVF_P_N_C,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
    FVF_P_N_C_T,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
    FVF_P_N_C_T2,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
    FVF_P_N_C_T_BW_BI,      // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
    FVF_P_N_C_T2_BW_BI      // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
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
    u32 col;
  };

  struct Vertex_P_T: public Vertex
  {
    vec2 uv;
  };

  struct Vertex_P_N: public Vertex
  {
    vec3 nor;
  };

  struct Vertex_P_C_T: public Vertex
  {
    u32  col;
    vec2 uv;
  };

  struct Vertex_P_N_C: public Vertex_P_N
  {
    u32  col;
  };

  struct Vertex_P_N_C_T: public Vertex_P_N_C
  {
    vec2 uv;
  };

  struct Vertex_P_N_C_T2: public Vertex_P_N_C
  {
    vec2 uv[2];
  };

  struct Vertex_P_N_C_T_BW_BI: public Vertex_P_N_C_T
  {
    float bw[4];
    int   bi[4];
  };

  struct Vertex_P_N_C_T2_BW_BI: public Vertex_P_N_C_T2
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

  struct LOD
  {
    //kgmArray vertices;
    //kgmArray faces;
  };

public:
  RenderType m_rtype;

  Vertex* m_vertices; //vertex buffer
  u32     m_vcount;   //vertex count
  Face*   m_faces;    //face buffer
  u32     m_fcount;   //face count
  u32     m_fvf;      //flexible vertex format
  u32     m_fff;      //flexible face format
  u8*     m_maps;     //material id by face

  kgmList<LOD*> m_lods;

  u32     m_group;    // object group id
  box3    m_bound;

public:
  kgmMesh();
  ~kgmMesh();

  kgmMesh* clone();


  void rebound();  //recalculate boundes(box, sphere, ...)
  box3 bound();

  Vertex* vAlloc(u32 count, FVF f=FVF_P_N_C);
  Face*   fAlloc(u32 count, FFF f=FFF_16);
  u32     vsize();
  u32     fsize();
  u32     fvf();
  u32     fff();

  Vertex* vertices(){ return m_vertices; }
  Face*   faces()   { return m_faces; }
  u32     vcount()  { return m_vcount; }
  u32     fcount()  { return m_fcount; }

  int     genLods(int);
};

typedef kgmList<kgmMesh*> kgmMeshList;
