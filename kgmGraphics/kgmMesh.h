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


class kgmMesh: public kgmResource{
 KGM_OBJECT(kgmMesh)
public:
 enum FVF{
  FVF_P_N_C,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
  FVF_P_N_C_T,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
  FVF_P_N_C_T2,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
  FVF_P_N_C_T_BW_BI      // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
 };

 enum FFF{
  FFF_16,
  FFF_32
 };

 struct Vertex{
   vec3 pos;
   vec3 nor;
   uint col;
 };

 struct Vertex_P_N_C_T: public Vertex{
  vec2 uv;
 };

 struct Vertex_P_N_C_T2: public Vertex{
  vec2 uv[2];
 };

 struct Vertex_P_N_C_T_BW_BI: public Vertex{
  vec4 uv;
  vec4 bw,
       bi;
 };

 struct Face{
 };

 struct Face_16: public Face{
  union{
   struct{ ushort a, b, c; };
   ushort f[3];
  };
 };

 struct Face_32: public Face{
  union{
   struct{ uint a, b, c; };
   uint f[3];
  };
 };

public:
 Vertex*	        	m_vertices;		//vertex buffer
 uint                           m_vcount;               //vertex count
 Face*  			m_faces;		//face buffer
 uint                           m_fcount;               //face count
 uint				m_fvf;			//flexible vertex format
 uint                           m_fff;                  //flexible face format

public:
 kgmMesh();
 virtual ~kgmMesh();

 box3 bound();		//recalculate boundes(box, sphere, ...)

 Vertex*	vAlloc(uint count, FVF f=FVF_P_N_C);
 Face*		fAlloc(uint count, FFF f=FFF_16);
 uint		vsize();
 uint		fsize();
 uint		fvf();
 uint           fff();

 ////
 Vertex*	vertices(){ return m_vertices; }
 Face*		faces(){ return m_faces; }
 uint           vcount(){ return m_vcount; }
 uint           fcount(){ return m_fcount; }
};

typedef kgmList<kgmMesh*> kgmMeshList;


