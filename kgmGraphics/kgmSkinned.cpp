#include "kgmSkinned.h"
#include "kgmSkeleton.h"
#include "kgmAnimation.h"

kgmSkinned::kgmSkinned(kgmMesh* m, kgmSkeleton* s, kgmAnimation* a)
{
  set(s);
  set(a);

  if (m && m->fvf() == kgmMesh::FVF_P_N_C_T_BW_BI)
  {
    kgmMesh::vAlloc(m->vcount(), kgmMesh::FVF_P_N_C_T_BW_BI);

    memcpy(vertices(), m->vertices(), m->vcount() * sizeof(kgmMesh::FVF_P_N_C_T_BW_BI));
  }
}

void kgmSkinned::set(kgmSkeleton* s)
{
  m_skeleton  = s;

  if (s)
  {
    m_tm_joints.clear();

    m_tm_joints.alloc(s->m_joints.size());
  }
}

void kgmSkinned::set(kgmAnimation* a)
{
  m_animation = a;
}

void kgmSkinned::update(u32 ms)
{
  if(!m_vertices || !m_animation || !m_skeleton)
    return;

  if(!m_loop && m_set >= m_end)
    return;

  u32 fset = m_set + ms;

  if(m_loop && fset >= m_end)
    fset = m_start;

  for(s32 i = 0; i <  m_skeleton->m_joints.size(); i++)
  {
    kgmSkeleton::Joint* joint = m_skeleton->m_joints[i];
    kgmAnimation::Animation* a = m_animation->getNode(joint->n);

    if(!a)
    {
      m_tm_joints[i].identity();

      continue;
    }

    mtx4 jframe;

    a->getFrame(fset, jframe);

    m_tm_joints[i] = m_skeleton->m_imatrices[i] * jframe;
    //m_tm_joints[i] = jframe;
  }

  kgmMesh::Vertex_P_N_C_T_BW_BI* verts = (kgmMesh::Vertex_P_N_C_T_BW_BI*) m_vertices;

  if (m_basic == null)
  {
    m_basic = (Vertex_P_N*) malloc(vcount() * sizeof(Vertex_P_N));

    for(u32 i = 0; i < vcount(); i++)
    {
      m_basic[i].pos = verts[i].pos;
      m_basic[i].nor = verts[i].nor;
    }
  }

  kgmMesh::Vertex_P_N* vbase = (kgmMesh::Vertex_P_N*) m_basic;


  for(u32 i = 0; i < vcount(); i++)
  {
    vec3   pos(0, 0, 0);
    vec3   bpos = vbase[i].pos;

    float* wght = (float*)(verts[i].bw);
    int*   indx = (int*)(verts[i].bi);

    for(u32 j = 0; j < 4; j++)
    {
      int bi = (int)indx[j];

      if(bi < 0)
      {
        if(j < 1)
          pos = bpos;

        break;
      }

      pos = pos + m_tm_joints[(u32)bi] * vbase[i].pos * wght[j];
    }

    verts[i].pos = pos;
  }

  m_set = fset;
}
