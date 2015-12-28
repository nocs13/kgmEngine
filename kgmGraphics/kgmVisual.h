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

    kgm_ptr<kgmMesh>      mesh;

    bool skin;

    kgm_ptr<kgmMesh::Vertex> vertices;
  public:

    Mesh()
    {
      mesh      = null;

      skin      = false;
      vertices  = null;
    }

    Mesh(kgm_ptr<kgmMesh> msh)
    {
      mesh = msh;

      skin      = false;
      vertices  = null;

      if(mesh)
      {
        if(mesh->fvf() & gcv_bn0)
        {
          vertices = kgm_ptr<kgmMesh::Vertex>(new kgmMesh::Vertex_P_N_C_T_BW_BI[mesh->vcount()]);
          memcpy(vertices, mesh->vertices(),
                 sizeof(kgmMesh::Vertex_P_N_C_T_BW_BI) * mesh->vcount());
          skin = true;
        }
      }
    }

    virtual ~Mesh()
    {
      if(vertices)  delete [] vertices;
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

  kgm_ptr<kgmMaterial>  m_material;

  kgm_ptr<kgmObject>    m_visual;

  kgm_ptr<kgmSkeleton>  m_skeleton;
  kgm_ptr<kgmAnimation> m_animation;

  bool                   m_floop;
  u32                    m_fstart;
  u32                    m_fend;
  u32                    m_fset;

//private:
  box3                   m_bound;
  kgm_ptr<mtx4>          m_tm_joints;
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

    if(m_skeleton)
    {
      m_tm_joints = kgm_ptr<mtx4>(new mtx4[m_skeleton->m_joints.size()]);
    }

    if(v.m_visual)
    {
      if(m_type == TypeMesh)
      {
        Mesh* mesh = (Mesh*) ((kgmObject*)v.m_visual);
        m_visual = kgm_ptr<kgmObject>((kgmObject*)(new Mesh(mesh->mesh)));
      }
      else
      {
        m_visual = v.m_visual;
      }
    }

    m_fstart = v.m_fstart;
    m_fend   = v.m_fend;
    m_floop  = v.m_floop;

    m_bound       = v.m_bound;
    m_last_update = kgmTime::getTicks();
    m_transform   = v.m_transform;
  }

  ~kgmVisual()
  {
    clear();
  }

  void clear()
  {
    m_type = TypeNone;

    m_transform.identity();
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

    m_material = m;
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

  void set(kgm_ptr<kgmParticles> par)
  {
    if(!par)
      return;

    m_visual.reset();

    m_visual = (kgm_ptr<kgmObject>) par;

    m_type = TypeParticles;
  }

  kgm_ptr<kgmParticles> getParticles()
  {
    return kgm_ptr_cast<kgmParticles, kgmObject>(m_visual);
  }

  void set(kgm_ptr<kgmText> text)
  {
    if(!text)
      return;

    m_visual.reset();

    m_visual = (kgm_ptr<kgmObject>) text;

    m_type = TypeText;
  }

  kgm_ptr<kgmText> getText()
  {
    return kgm_ptr_cast<kgmText, kgmObject>(m_visual);
  }

  void set(kgm_ptr<kgmSprite> sprite)
  {
    if(!sprite)
      return;

    m_visual.reset();

    m_visual = (kgm_ptr<kgmObject>) sprite;

    m_type = TypeSprite;
  }

  kgm_ptr<kgmSprite> getSprite()
  {
    return kgm_ptr_cast<kgmSprite, kgmObject>(m_visual);
  }

  void setAnimation(kgm_ptr<kgmAnimation> a, u32 start = 0, u32 end = 0, bool loop = false)
  {
    if(a == null)
      return;

    if(a != m_animation)
    {
      m_animation.reset();

      m_animation = a;
    }

    m_fstart    = start;
    m_fset      = start;
    m_fend      = end;
    m_floop     = loop;
  }

  void setSkeleton(kgm_ptr<kgmSkeleton> skel)
  {
    if(skel)
    {
      if(m_skeleton)
      {
        m_skeleton.reset();

        if(m_tm_joints)
        {
          m_tm_joints.reset();
        }
      }

      m_skeleton = skel;
      m_tm_joints = kgm_ptr<mtx4>(new mtx4[skel->m_joints.size()]);
    }
  }

  bool set(kgm_ptr<kgmMesh> msh)
  {
    if(!msh)
      return false;

    m_visual.reset();

    kgm_ptr<Mesh> mesh;

    mesh = kgm_ptr<Mesh>(new Mesh(msh));

    m_visual = kgm_ptr_cast<kgmObject, Mesh>(mesh);

    m_type = TypeMesh;

    m_bound = msh->bound();
  }

  Mesh* getMesh()
  {
    kgm_ptr<Mesh> mesh;

    mesh = kgm_ptr_cast<Mesh, kgmObject>(m_visual);

    Mesh *m = (Mesh*) mesh;

    return m;
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
      kgmMesh::Vertex_P_N_C_T_BW_BI* verts = (kgmMesh::Vertex_P_N_C_T_BW_BI*)((kgmMesh::Vertex*)m_mesh->vertices);

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
