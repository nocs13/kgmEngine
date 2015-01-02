#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmIGC.h"

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
  enum Type
  {
    TypeNone,
    TypeMesh,
    TypeText,
    TypeSprite,
    TypeBillboard,
    TypeParticles
  };

  enum  TypeShadow
  {
    ShadowNone,
    ShadowCast,
    ShadowRecv,
    ShadowFull
  };

  class Mesh: public kgmObject
  {
    friend class  kgmVisual;

    kgmMesh*      mesh;
    bool          skin;

    kgmMesh::Vertex*     vertices;
  public:

    Mesh()
    {
      mesh      = null;

      skin      = false;
      vertices  = null;
    }

    Mesh(kgmMesh* msh)
    {
      mesh      = null;

      if(msh)  { mesh = msh; msh->increment(); }

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

    virtual ~Mesh()
    {
      if(vertices)  delete [] vertices;

      if(mesh)      mesh->release();
    }

    bool hasSkin()
    {
      return skin;
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

    kgmMesh* getMesh() const
    {
      return mesh;
    }
  };

  bool                   m_valid;
  bool                   m_remove;
  bool                   m_visible;

private:
  mtx4                   m_transform;

public:
  Type                   m_type;
  TypeShadow             m_typeshadow;

  kgmMaterial*           m_material;

  kgmObject*             m_visual;

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

    m_type       = TypeNone;
    m_typeshadow = ShadowNone;

    m_visual    = null;
    m_material  = null;
    m_skeleton  = null;
    m_animation = null;
    m_tm_joints = null;

    m_fstart    = m_fend = 0;
    m_floop     = false;

    m_bound       = box3(vec3(-1, -1, -1), vec3(1, 1, 1));
    m_last_update = kgmTime::getTicks();

    m_transform.identity();
  }

  kgmVisual(const kgmVisual &v)
  {
    m_valid   = v.m_valid;
    m_remove  = v.m_remove;
    m_visible = v.m_visible;

    m_type       = v.m_type;
    m_typeshadow = v.m_typeshadow;

    m_visual    = null;
    m_material  = v.m_material;
    m_skeleton  = v.m_skeleton;
    m_animation = v.m_animation;
    m_tm_joints = null;

    if(m_material)
      m_material->increment();

    if(m_animation)
      m_animation->increment();

    if(m_skeleton)
    {
      m_skeleton->increment();
      m_tm_joints = new mtx4[m_skeleton->m_joints.size()];
    }

    if(v.m_visual)
    {
      if(m_type == TypeMesh)
      {
        m_visual = new Mesh(((Mesh*)v.m_visual)->mesh);
      }
      else
      {
        m_visual = v.m_visual;
        m_visual->increment();
      }
    }

    m_fstart = v.m_fstart;
    m_fend   = v.m_fend;
    m_floop  = v.m_floop;

    m_bound       = v.m_bound;
    m_last_update = kgmTime::getTicks();
    m_transform   = v.m_transform;
  }

  virtual ~kgmVisual()
  {
    clear();
  }

  void clear()
  {
    m_type = TypeNone;

    m_transform.identity();

    if(m_tm_joints)
      delete [] m_tm_joints;

    if(m_material)
      m_material->release();

    if(m_visual)
      m_visual->release();

    if(m_skeleton)
      m_skeleton->release();

    if(m_animation)
      m_animation->release();
  }

  void enable()
  {
    m_valid = false;
  }

  void disable()
  {
    m_valid = false;
  }

  bool valid()
  {
    return m_valid;
  }

  void remove()
  {
    m_remove = true;
  }

  bool removed()
  {
    return m_remove;
  }

  void show()
  {
    m_visible = true;
  }

  void hide()
  {
    m_visible = false;
  }

  bool visible()
  {
    return m_visible;
  }

  u32 type()
  {
    return m_type;
  }

  void set(kgmMaterial* m)
  {
    if(m == null)
      return;

    if(m_material)
      m_material->release();

    m_material = m;
    m->increment();
  }

  kgmMaterial* getMaterial()
  {
    return m_material;
  }

  void set(mtx4 *mtx)
  {
    if(!mtx)
      return;

    m_transform = *mtx;
  }

  void set(mtx4 mtx)
  {
    m_transform = mtx;
  }

  mtx4& getTransform()
  {
    return m_transform;
  }

  void set(kgmParticles* par)
  {
    if(!par)
      return;

    clear();

    par->increment();
    m_visual = par;
    m_type = TypeParticles;
  }

  kgmParticles* getParticles()
  {
    return (kgmParticles*)m_visual;
  }

  void set(kgmText* text)
  {
    if(!text)
      return;

    clear();

    text->increment();
    m_visual = text;
    m_type = TypeText;
  }

  kgmText* getText()
  {
    return (kgmText*)m_visual;
  }

  void set(kgmSprite* sprite)
  {
    if(!sprite)
      return;

    clear();

    sprite->increment();
    m_visual = sprite;
    m_type = TypeSprite;
  }

  kgmSprite* getSprite()
  {
    return (kgmSprite*)m_visual;
  }

  void setAnimation(kgmAnimation* a, u32 start = 0, u32 end = 0, bool loop = false)
  {
    if(a == m_animation)
      return;

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

  bool set(kgmMesh* msh)
  {
    if(!msh)
      return false;

    clear();

    m_visual = new Mesh(msh);
    m_type = TypeMesh;
    m_bound = msh->bound();
  }

  Mesh* getMesh()
  {
    return (Mesh*)m_visual;
  }

  box3 getBound()
  {
    return m_bound;
  }

  virtual kgmObject* clone()
  {
    return new kgmVisual(*this);
  }

  void update()
  {
    u32 dtick = kgmTime::getTicks() - m_last_update;

    if(dtick < 50)
      return;

    switch(m_type)
    {
    case TypeMesh:
      animate();
    break;
    case TypeParticles:
      if(m_visual)
      {
        getParticles()->update(dtick);
      }
      break;
    case TypeSprite:
      if(m_visual)
      {
      }
    }

    m_last_update = kgmTime::getTicks();
  }

private:
  void animate()
  {
    if(!m_valid)
      return;

    Mesh* m_mesh = getMesh();

    if(!m_animation || !m_skeleton || !m_mesh || !m_mesh->skin)
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

    if(m_mesh)
    {
      kgmMesh::Vertex_P_N_C_T_BW_BI* vbase = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m_mesh->mesh->vertices();
      kgmMesh::Vertex_P_N_C_T_BW_BI* verts = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m_mesh->vertices;

      for(int i = 0; i < m_mesh->mesh->vcount(); i++)
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
