#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmIGraphics.h"

#include "kgmMesh.h"
#include "kgmText.h"
#include "kgmLight.h"
#include "kgmSprite.h"
#include "kgmSkeleton.h"
#include "kgmParticles.h"

//Graphical Object
class kgmVisual: public kgmObject
{
  KGM_OBJECT(kgmVisual);

public:
  enum TypeRender
  {
    RenderNone,
    RenderMesh,
    RenderText,
    RenderSprite,
    RenderParticles
  };

  enum  TypeShadow
  {
    ShadowNone,
    ShadowCast,
    ShadowRecv,
    ShadowFull
  };

  class Visual
  {
    friend class  kgmVisual;

    kgmMaterial*  material;
    kgmMesh*      mesh;
    mtx4*         transform;
    bool          skin;

    kgmMesh::Vertex*     vertices;
  public:

    Visual()
    {
      mesh      = null;
      material  = null;

      skin      = false;
      vertices  = null;
    }

    Visual(kgmMesh* msh, kgmMaterial* mtl)
    {
      mesh      = null;
      material  = null;

      if(msh)  { mesh = msh; msh->increment(); }
      if(mtl)  { material = mtl; mtl->increment(); }

      skin      = false;
      vertices  = null;

      if(mesh)
      {
        if(mesh->fvf() & gcv_bn0)
        {
          vertices = new kgmMesh::Vertex_P_N_C_T_BW_BI[mesh->vcount()];
          memcpy(vertices, mesh->vertices(),
                 sizeof(kgmMesh::Vertex_P_N_C_T_BW_BI) * mesh->vcount());
          skin = true;
        }
      }
    }

    virtual ~Visual()
    {
      if(vertices)  delete [] vertices;

      if(mesh)      mesh->release();
      if(material)  material->release();
    }

    kgmMaterial* getMaterial()
    {
      return material;
    }

    kgmMesh::Vertex* getVertices()
    {
      if(vertices)
        return vertices;

      return mesh->vertices();
    }

    kgmMesh::Face* getFaces()
    {
      return mesh->faces();
    }

    u32 getVcount()
    {
      return mesh->vcount();
    }

    u32 getFcount()
    {
      return mesh->fcount();
    }

    u32 getVsize()
    {
      return mesh->vsize();
    }

    u32 getFvf()
    {
      return mesh->fvf();
    }

    u32 getFff()
    {
      return mesh->fff();
    }
    kgmMesh::RenderType getRenderType()
    {
      return mesh->m_rtype;
    }
  };

  bool                   m_valid;
  bool                   m_remove;

public:
  bool                   m_visible;
  mtx4                   m_transform;
  TypeShadow             m_typeshadow;
  TypeRender             m_typerender;

  kgmList<kgmMaterial*>  m_materials;   //material maps
  kgmList<Visual*>       m_visuals;     //render geometry
  kgmParticles*          m_particles;   //particle effect
  kgmSprite*             m_sprite;      //render sprite
  kgmText*               m_text;        //render text


  kgmSkeleton*           m_skeleton;
  kgmAnimation*          m_animation;
  bool                   m_floop;
  u32                    m_fstart;
  u32                    m_fend;
  u32                    m_fset;

//private:
  box3                   m_bound;
  mtx4*                  m_tm_joints;
  u32                    m_last_update;

  static  bool           AnimateVertices;
public:
  kgmVisual()
  {
    m_valid   = true;
    m_remove  = false;
    m_visible = true;

    m_typeshadow = ShadowNone;
    m_typerender = RenderNone;

    m_skeleton  = null;
    m_animation = null;
    m_fstart    = m_fend = 0;
    m_floop     = false;

    m_bound       = box3(vec3(-1, -1, -1), vec3(1, 1, 1));
    m_tm_joints   = null;
    m_last_update = kgmTime::getTicks();

    m_text = null;
    m_sprite = null;
    m_particles = null;
  }

  virtual ~kgmVisual()
  {
    int i = 0;

    for(int i = 0; i < m_materials.size(); i++)
      m_materials[i]->release();
    m_materials.clear();

    for(int i = 0; i < m_visuals.size(); i++)
      delete m_visuals[i];
    m_visuals.clear();

    if(m_tm_joints)
      delete [] m_tm_joints;

    if(m_particles)
      m_particles->release();

    if(m_text)
      m_text->release();

    if(m_sprite)
      m_sprite->release();

    if(m_skeleton)
      m_skeleton->release();

    if(m_animation)
      m_animation->release();
  }

  void enable(){
    m_valid = false;
  }

  void disable(){
    m_valid = false;
  }

  bool valid(){
    return m_valid;
  }

  void remove(){
    m_remove = true;
  }

  bool removed(){
    return m_remove;
  }

  //Materials
  int numMaterials(){
    return m_materials.size();
  }

  bool add(kgmMaterial* m)
  {
    if(m == 0)
      return false;

    m_materials.add(m);
    m->increment();

    return true;
  }

  void set(kgmParticles* par)
  {
    if(!par)
      return;

    if(m_typerender == RenderNone || m_typerender == RenderParticles)
    {

      if(m_particles)
        m_particles->release();

      par->increment();
      m_particles = par;
      m_typerender = RenderParticles;
    }
  }

  kgmParticles* getParticles()
  {
    return m_particles;
  }

  void set(kgmText* text)
  {
    if(!text)
      return;

    if(m_typerender == RenderNone || m_typerender == RenderText)
    {

      if(m_text)
        m_text->release();

      text->increment();
      m_text = text;
      m_typerender = RenderText;
    }
  }

  kgmText* getText()
  {
    return m_text;
  }

  void set(kgmSprite* sprite)
  {
    if(!sprite)
      return;

    if(m_typerender == RenderNone || m_typerender == RenderSprite)
    {

      if(m_sprite)
        m_sprite->release();

      sprite->increment();
      m_sprite = sprite;
      m_typerender = RenderSprite;
    }
  }

  kgmSprite* getSprite()
  {
    return m_sprite;
  }

  void setAnimation(kgmAnimation* a, u32 start = 0, u32 end = 0, bool loop = false)
  {
    if(m_animation)
      m_animation->release();

    if(a)
    {
      a->increment();
      m_animation = a;
    }

    m_fstart    = start;
    m_fset      = start;
    m_fend      = end;
    m_floop     = loop;
  }

  void setSkeleton(kgmSkeleton* skel)
  {
    if(skel)
    {
      if(m_skeleton)
      {
        m_skeleton->release();

        if(m_tm_joints)
        {
          delete [] m_tm_joints;
          m_tm_joints = null;
        }
      }

      skel->increment();
      m_skeleton = skel;
      m_tm_joints = new mtx4[skel->m_joints.size()];
    }
  }

  void addVisual(kgmMesh* msh, kgmMaterial* mtl)
  {
    if(msh)
    {
      m_visuals.add(new Visual(msh, mtl));

      box3 b = msh->bound();

      m_bound.extend(b);
    }
  }

  box3 getBound()
  {
    return m_bound;
  }

  void update()
  {
    u32 dtick = kgmTime::getTicks() - m_last_update;

    if(dtick < 50)
      return;

    switch(m_typerender)
    {
    case RenderMesh:
      animate();
    break;
    case RenderParticles:
      if(m_particles)
      {
        m_particles->update(dtick);
      }
      break;
    case RenderSprite:
      if(m_sprite)
      {

      }
    }

    m_last_update = kgmTime::getTicks();
  }

private:
  void animate()
  {
    if(!m_animation || !m_skeleton)
      return;

    if(!m_floop && m_fset >= m_fend)
      return;

    if(m_floop && m_fset >= m_fend)
      m_fset = m_fstart;

    for(int i = 0; i < m_skeleton->m_joints.size(); i++)
    {
      kgmSkeleton::Joint* joint = m_skeleton->m_joints[i];
      kgmAnimation::Animation* a = m_animation->getNode(joint->n);

      if(!a)
      {
        m_tm_joints[i].identity();

        continue;
      }

      mtx4 jframe;
      a->getFrame(m_fset, jframe);

      m_tm_joints[i] = m_skeleton->m_imatrices[i] * jframe;
      //m_tm_joints[i] = jframe;
    }

    for(int j = 0; j < m_visuals.size(); j++)
    {
      Visual* v = m_visuals[j];

      if(!v->skin)
        continue;

      kgmMesh::Vertex_P_N_C_T_BW_BI* vbase = (kgmMesh::Vertex_P_N_C_T_BW_BI*)v->mesh->vertices();
      kgmMesh::Vertex_P_N_C_T_BW_BI* verts = (kgmMesh::Vertex_P_N_C_T_BW_BI*)v->vertices;

      for(int i = 0; i < v->mesh->vcount(); i++)
      {
        vec3   pos(0, 0, 0);
        vec3   bpos = vbase[i].pos;
        float* wght = (float*)(vbase[i].bw);
        int*   indx = (int*)(vbase[i].bi);

        for(int j = 0; j < 4; j++)
        {
          int   bi = (int)indx[j];
          float w  = wght[j];

          if(bi < 0)
          {
            if(j < 1)
              pos = bpos;
            break;
          }

          pos = pos + m_tm_joints[bi] * vbase[i].pos * wght[j];
        }

        verts[i].pos = pos;
      }
    }

    m_fset++;
  }
};

