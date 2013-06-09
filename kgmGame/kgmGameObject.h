#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmGameObject : public kgmObject
{
  KGM_OBJECT(kgmGameObject);

private:
  kgmString m_id;
  kgmString m_class;

  bool      m_valid;
  bool      m_remove;
  bool      m_culled;
  bool      m_visible;

  u32       m_birth;
  u32       m_timeout;
protected:
  vec3        m_position;
  vec3        m_rotation;
  quat        m_quaternion;

  kgmBody*    m_body;
  kgmVisual*  m_visual;

  kgmGameObject* 	         m_parent;
  kgmList<kgmGameObject*>  m_childs;


public:
  kgmGameObject();
  virtual ~kgmGameObject();

  virtual void         exit(){}
  virtual void         init(){}
  virtual void         update(u32 mls);
  virtual kgmBody*     getBody(){ return m_body; }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  bool valid()      { return m_valid;    }
  bool removed()    { return m_remove;   }
  bool culled()     { return m_culled;   }
  bool visible()    { return m_visible;  }

  void remove()     { m_remove = true;   }
  void enable()     { m_valid  = true;   }
  void disable()    { m_valid  = false;  }
  void show()       { m_visible = true;  }
  void hide()       { m_visible = false; }

  void timeout(u32 t){ m_timeout = t;    }

  void setId(kgmString s)
  {
    m_id = s;
  }

  kgmString getId()
  {
    return m_id;
  }

  void setPosition(vec3& v)
  {
    m_position = v;

    if(m_body)
      m_body->translate(v.x, v.y, v.z);
  }

  void setRotation(vec3& r)
  {
    m_rotation = r;

    if(m_body)
      m_body->rotate(r.x, r.y, r.z);
  }

  void setQuaternion(quat& q)
  {
    m_quaternion = q;

    if(m_body)
      m_body->rotate(q);
  }

  void setParent(kgmGameObject* a)
  {
    if(!a)
      return;

    if(m_parent)
    {
      m_parent->removeChild(this);
    }

    m_parent = a;
  }

  void addChild(kgmGameObject* a)
  {
    if(!a)
      return;

    m_childs.add(a);
    a->increment();
  }

  void removeChild(kgmGameObject* a)
  {
    if(!a)
      return;

    for(int i = 0; i < m_childs.length(); i++)
    {
      if(a == m_childs[i])
      {
        a->release();
        m_childs.erase(i);

        return;
      }
    }
  }
};

#endif // KGMGAMEOBJECT_H
