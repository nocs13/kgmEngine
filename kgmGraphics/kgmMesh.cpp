#include "kgmMesh.h"
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmBase/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmMesh, kgmResource);

kgmMesh::kgmMesh(){
  m_vertices = 0;
  m_faces = 0;
  m_vcount = m_fcount = 0;
  m_fvf = m_fff = 0;
}

kgmMesh::~kgmMesh(){
 }

box3 kgmMesh::bound(){
 box3   m_bound;

 if(!m_vertices)
  return m_bound;

 Vertex v   = m_vertices[(u32)0];
 vec3   max = v.pos, 
        min = v.pos;

 u32 i    = 0;
 int  size = 0;

 for(i = 0; i < m_vcount; i++){
  v = m_vertices[i];
  min.x = MIN(min.x, v.pos.x);
   min.y = MIN(min.y, v.pos.y);
    min.z = MIN(min.z, v.pos.z);
  max.x = MAX(max.x, v.pos.x);
   max.y = MAX(max.y, v.pos.y);
    max.z = MAX(max.z, v.pos.z);
 }
 
 m_bound.min = min;
 m_bound.max = max;

 return m_bound;
}


kgmMesh::Vertex* kgmMesh::vAlloc(u32 count, FVF f){
 if(m_vertices)
  delete [] m_vertices;
 switch(f){
 case FVF_P_N_C_T_BW_BI:
   m_vertices = new Vertex_P_N_C_T_BW_BI[count];
   m_fvf = FVF_P_N_C_T_BW_BI;
   break;
 case FVF_P_N_C_T2:
   m_vertices = new Vertex_P_N_C_T2[count];
   m_fvf = FVF_P_N_C_T2;
   break;
 case FVF_P_N_C_T:
   m_vertices = new Vertex_P_N_C_T[count];
   m_fvf = FVF_P_N_C_T;
   break;
 default:
   m_vertices = new Vertex[count];
   m_fvf = FVF_P_N_C;
 }

 m_vcount = count;
 return m_vertices;
}

kgmMesh::Face*	kgmMesh::fAlloc(u32 count, FFF f){
 if(m_faces)
  delete [] m_faces;
 switch(f){
 case FFF_32:
   m_faces = new Face_32[count];
   m_fff = FFF_32;
   break;
 default:
   m_faces = new Face_16[count];
   m_fff = FFF_16;
 }

 m_fcount = count;
 return m_faces;
}

u32 kgmMesh::fvf(){
 switch(m_fvf){
 case FVF_P_N_C_T2_BW_BI:
   return (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_bn0);
   break;
 case FVF_P_N_C_T_BW_BI:
   return (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_bn0);
   break;
 case FVF_P_N_C_T2:
   return (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1);
   break;
 case FVF_P_N_C_T:
   return (gcv_xyz|gcv_nor|gcv_col|gcv_uv0);
   break;
 case FVF_P_N_C:
   return (gcv_xyz|gcv_nor|gcv_col);
   break;
 }
 return 0;
}

u32 kgmMesh::fff(){
 switch(m_fvf){
 case FFF_16:
   return 16;
   break;
 case FFF_32:
   return 32;
   break;
 }
 return 0;
}

u32 kgmMesh::vsize(){
  switch(m_fvf){
  case FVF_P_N_C_T2_BW_BI:
    return sizeof(Vertex_P_N_C_T2_BW_BI);
    break;
  case FVF_P_N_C_T_BW_BI:
    return sizeof(Vertex_P_N_C_T_BW_BI);
    break;
  case FVF_P_N_C_T2:
    return sizeof(Vertex_P_N_C_T2);
    break;
  case FVF_P_N_C_T:
    return sizeof(Vertex_P_N_C_T);
    break;
  case FVF_P_N_C:
    return sizeof(Vertex);
    break;
  }
  return 0;
}

u32 kgmMesh::fsize(){
  switch(m_fvf){
  case FFF_16:
    return 16;
    break;
  case FFF_32:
    return 32;
    break;
  }
  return 0;
 }
