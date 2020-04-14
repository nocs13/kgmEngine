#include "kgmMesh.h"
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmBase/kgmIGC.h"

kgmMesh::kgmMesh()
{
  m_vertices = null;
  m_faces = null;
  m_vcount = m_fcount = 0;
  m_fvf = m_fff = 0;
  m_rtype = RT_TRIANGLE;
  m_group = 0;
  m_bound = box3();
  m_normal = vec3(0, 0, 0);
}

kgmMesh::kgmMesh(kgmMesh* m)
{
  m_linked = m;

  m_vertices = null;
  m_faces = null;

  m_vcount = m_fcount = 0;
  m_fvf = m_linked->m_fvf;
  m_fff = m_linked->m_fff;
  m_rtype = m_linked->m_rtype;
  m_bound = m_linked->m_bound;
  m_normal = m_linked->m_normal;
}

kgmMesh::kgmMesh(const kgmMesh& msh)
{
  m_rtype = msh.m_rtype;

  m_vcount = msh.m_vcount;
  m_fcount = msh.m_fcount;
  m_fvf    = msh.m_fvf;
  m_fff    = msh.m_fff;
  m_group  = msh.m_group;
  m_bound  = msh.m_bound;
  m_normal = msh.m_normal;

  vAlloc(m_vcount, (kgmMesh::FVF)m_fvf);
  fAlloc(m_fcount, (kgmMesh::FFF)m_fff);

  memcpy(m_vertices, msh.m_vertices, m_vcount * vsize());
  memcpy(m_faces, msh.m_faces, m_fcount * fsize());
}

kgmMesh::~kgmMesh()
{
  if(m_vertices)
    free(m_vertices);

  if(m_faces)
    free(m_faces);
}

void kgmMesh::rebuild()
{
  if (m_linked)
    return;

  if(!m_vertices)
    return;

  u8 *v = (u8*) m_vertices;
  vec3 max = ((Vertex*)v)->pos,
       min = ((Vertex*)v)->pos;

  u32 i    = 0;
  u32 size = vsize();

  for(i = 1; i < m_vcount; i++)
  {
    v += size;

    Vertex* vt = (Vertex*)v;

    min.x = MIN(min.x, ((Vertex*)v)->pos.x);
    min.y = MIN(min.y, ((Vertex*)v)->pos.y);
    min.z = MIN(min.z, ((Vertex*)v)->pos.z);
    max.x = MAX(max.x, ((Vertex*)v)->pos.x);
    max.y = MAX(max.y, ((Vertex*)v)->pos.y);
    max.z = MAX(max.z, ((Vertex*)v)->pos.z);
  }

  m_bound.min = min;
  m_bound.max = max;

  u8* f     = (u8*)m_faces;

  if (!f)
    return;

  v = (u8*) m_vertices;

  if (!v)
    return;

  m_normal = vec3(0, 0, 0);

  i = 0;

  for (i = 0; i < m_fcount; i++)
  {
    triangle tr;
    Face*    fc = (Face*)f;

    Vertex *v1, *v2, *v3;
    u32     f1,  f2,  f3;

    if (m_fff == FFF_16) {
      f1 = ((Face_16*)fc)->a;
      f2 = ((Face_16*)fc)->b;
      f3 = ((Face_16*)fc)->c;
    } else {
      f1 = ((Face_32*)fc)->a;
      f2 = ((Face_32*)fc)->b;
      f3 = ((Face_32*)fc)->c;
    }

    v1 = (Vertex*) ((u8*)m_vertices + vsize() * f1);
    v2 = (Vertex*) ((u8*)m_vertices + vsize() * f2);
    v3 = (Vertex*) ((u8*)m_vertices + vsize() * f3);

    tr = triangle(v1->pos, v2->pos, v3->pos);

    vec3 n = tr.normal();

    n.normalize();
    m_normal = m_normal + n;

    f += fsize();
  }

  m_normal.normalize();
}

kgmMesh::Vertex* kgmMesh::vAlloc(u32 count, FVF f)
{
  if(m_vertices)
    free(m_vertices);

  u32 v_size = 0;

  switch(f)
  {
  case FVF_P_N_T_BW_BI:
    v_size = sizeof(Vertex_P_N_T_BW_BI) * count;
    m_fvf = FVF_P_N_T_BW_BI;
    break;
  case FVF_P_N_T2:
    v_size = sizeof(Vertex_P_N_T2) * count;
    m_fvf = FVF_P_N_T2;
    break;
  case FVF_P_N_T:
    v_size = sizeof(Vertex_P_N_T) * count;
    m_fvf = FVF_P_N_T;
    break;
  case FVF_P_C_T:
    v_size = sizeof(Vertex_P_C_T) * count;
    m_fvf = FVF_P_C_T;
    break;
  case FVF_P_T:
    v_size = sizeof(Vertex_P_T) * count;
    m_fvf = FVF_P_T;
    break;
  case FVF_P_N:
    v_size = sizeof(Vertex_P_N) * count;
    m_fvf = FVF_P_N;
    break;
  case FVF_P_C:
    v_size = sizeof(Vertex_P_C) * count;
    m_fvf = FVF_P_C;
    break;
  case FVF_P:
    v_size = sizeof(Vertex) * count;
    m_fvf = FVF_P;
    break;
  default:
    v_size = sizeof(Vertex) * count;
    m_fvf = FVF_P;
  }

  m_vertices = (Vertex*) malloc(v_size);

  m_vcount = count;

  return m_vertices;
}

kgmMesh::Face*	kgmMesh::fAlloc(u32 count, FFF f)
{
  if(m_faces)
    free(m_faces);

  switch(f)
  {
  case FFF_32:
    m_faces = (Face*)malloc(sizeof(Face_32) * count);
    m_fff = FFF_32;
    break;
  default:
    m_faces = (Face*)malloc(sizeof(Face_16) * count);
    m_fff = FFF_16;
  }

  m_fcount = count;

  return m_faces;
}

u32 kgmMesh::fvf()
{
  switch(m_fvf)
  {
  case FVF_P_N_T2_BW_BI:
    return (gcv_xyz|gcv_nor|gcv_uv0|gcv_uv1|gcv_bn0);
    break;
  case FVF_P_N_T_BW_BI:
    return (gcv_xyz|gcv_nor|gcv_uv0|gcv_bn0);
    break;
  case FVF_P_N_T2:
    return (gcv_xyz|gcv_nor|gcv_uv0|gcv_uv1);
    break;
  case FVF_P_N_T:
    return (gcv_xyz|gcv_nor|gcv_uv0);
    break;
  case FVF_P_N:
    return (gcv_xyz|gcv_nor);
    break;
  case FVF_P_C_T:
    return (gcv_xyz|gcv_col|gcv_uv0);
  case FVF_P_T:
    return (gcv_xyz|gcv_uv0);
    break;
  case FVF_P_C:
    return (gcv_xyz|gcv_col);
    break;
  case FVF_P:
    return (gcv_xyz);
    break;
  }
  return 0;
}

u32 kgmMesh::fff()
{
  switch(m_fvf)
  {
  case FFF_16:
    return 16;
    break;
  case FFF_32:
    return 32;
    break;
  }

  return 0;
}

u32 kgmMesh::vsize()
{
  switch(m_fvf)
  {
  case FVF_P_N_T2_BW_BI:
    return sizeof(Vertex_P_N_T2_BW_BI);
  case FVF_P_N_T_BW_BI:
    return sizeof(Vertex_P_N_T_BW_BI);
  case FVF_P_N_T2:
    return sizeof(Vertex_P_N_T2);
  case FVF_P_N_T:
    return sizeof(Vertex_P_N_T);
  case FVF_P_C_T:
    return sizeof(Vertex_P_C_T);
  case FVF_P_T:
    return sizeof(Vertex_P_T);
  case FVF_P_N:
    return sizeof(Vertex_P_N);
  case FVF_P_C:
    return sizeof(Vertex_P_C);
  case FVF_P:
    return sizeof(Vertex);
  }

  return 0;
}

u32 kgmMesh::fsize()
{
  switch(m_fff)
  {
  case FFF_16:
    return sizeof(Face_16);
    break;
  case FFF_32:
    return  sizeof(Face_32);
    break;
  }

  return 0;
}
