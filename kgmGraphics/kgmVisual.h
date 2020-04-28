#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmIGC.h"

#include "kgmMesh.h"
#include "kgmText.h"
#include "kgmLight.h"
#include "kgmShape.h"
#include "kgmSprite.h"
#include "kgmSkeleton.h"
#include "kgmParticles.h"
#include "kgmAnimation.h"

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
    TypeShape,
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

  bool  m_valid;
  bool  m_remove;
  bool  m_visible;

private:
  mtx4  m_transform;

  bool  m_lighting;

public:
  Type        m_type;
  TypeShadow  m_typeshadow;

  kgmMaterial*  m_material;

  union
  {
    kgmMesh*       m_mesh;
    kgmText*       m_text;
    kgmShape*      m_shape;
    kgmSprite*     m_sprite;
    kgmParticles*  m_particles;

    kgmObject*  m_visual = null;
  };

  kgmSkeleton*   m_skeleton  = null;
  kgmAnimation*  m_animation = null;

  bool m_floop;

  u32  m_fstart;
  u32  m_fend;
  u32  m_fset;

//private:
  box3   m_bound;
  u32    m_last_update;

  kgmArray<mtx4>  m_tm_joints;

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

    m_fstart = m_fend = 0;
    m_floop  = false;

    m_bound       = box3(vec3(-1, -1, -1), vec3(1, 1, 1));
    m_last_update = kgmTime::getTicks();

    m_transform.identity();

    m_lighting = true;
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
      m_tm_joints.alloc(m_skeleton->m_joints.size());
    }

    if(v.m_visual)
    {
      if(m_type == TypeMesh)
      {
        kgmMesh* mesh = (kgmMesh*) ((kgmObject*)v.m_visual);

        m_visual = new kgmMesh(*mesh);
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

    m_lighting = true;
  }

  ~kgmVisual()
  {
    clear();
  }

  void clear()
  {
    switch((u32) m_type)
    {
    case TypeMesh:
      if(m_mesh)
        m_mesh->release();
      m_mesh = null;
      break;
    case TypeText:
      break;
    case TypeShape:
      if(m_shape)
        delete m_shape;
      m_shape = null;
      break;
    case TypeSprite:
      break;
    case TypeParticles:
      break;
    }

    m_visual    = null;
    m_skeleton  = null;
    m_animation = null;

    m_tm_joints.clear();

    m_transform.identity();

    m_type = TypeNone;
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

  bool set(kgmParticles* p)
  {
    if(!p || m_visual)
      return false;

    m_particles = p;

    m_type = TypeParticles;

    return true;
  }

  kgmParticles* getParticles()
  {
    if(m_type != TypeParticles)
      return null;

    return m_particles;
  }

  bool set(kgmText* t)
  {
    if(!t || m_visual)
      return false;

    m_text = t;

    m_type = TypeText;

    return true;
  }

  kgmText* getText()
  {
    if(m_type != TypeText)
      return null;

    return m_text;
  }

  bool set(kgmSprite* s)
  {
    if(!s || m_visual)
      return false;

    m_sprite = s;

    m_type = TypeSprite;

    return true;
  }

  kgmSprite* getSprite()
  {
    if(m_type != TypeSprite)
      return null;

    return m_sprite;
  }

  bool set(kgmMesh* m)
  {
    if(!m || m_visual)
      return false;

    m_mesh = m;

    m_type = TypeMesh;

    m_bound = m->bound();

    return true;
  }

  kgmMesh* getMesh()
  {
    if(m_type != TypeMesh)
      return null;

    return m_mesh;
  }

  bool set(kgmShape* s)
  {
    if(!s || m_visual)
      return false;

    m_shape = s;

    m_type = TypeShape;

    return true;
  }

  kgmShape* getShape()
  {
    if(m_type != TypeShape)
      return null;

    return m_shape;
  }

  void setAnimation(kgmAnimation* a, u32 start = 0, u32 end = 0, bool loop = false)
  {
    if(a == null)
      return;

    if(a != m_animation)
    {
      m_animation = a;
    }

    m_fstart    = start;
    m_fset      = start;
    m_fend      = end;
    m_floop     = loop;
  }

  void setSkeleton(kgmSkeleton* s)
  {
    if(s)
    {
      if(m_skeleton)
      {
        if(m_tm_joints)
        {
          m_tm_joints.clear();
        }
      }

      m_skeleton = s;
      m_tm_joints.alloc(s->m_joints.size());
    }
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

    m_last_update = kgmTime::getTicks();
  }

  bool lighting() const
  {
    return m_lighting;
  }

  void lighting(bool l)
  {
    m_lighting = l;
  }
};
