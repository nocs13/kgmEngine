#ifndef KGMGAMENODE_H
#define KGMGAMENODE_H

#include "../kgmBase/kgmObject.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmIGame;

class kgmGameNode: public kgmObject//, private kgmBody,  private kgmVisual
{
  KGM_OBJECT(kgmGameNode);

  kgmIGame* m_game = null;

  kgmString m_id;
  kgmString m_class;

  bool m_lock;
  bool m_valid;
  bool m_remove;
  bool m_culled;
  bool m_visible;

  u32 m_group;
  u32 m_birth;
  u32 m_living;

  vec3 m_position;
  vec3 m_rotation;
  quat m_quaternion;


protected:
  kgmBody*    m_body   = null;
  kgmVisual*  m_visual = null;

public:
  kgmGameNode(kgmIGame* g);
  ~kgmGameNode();

  virtual void         init(){}
  virtual void         exit(){}
  virtual void         update(u32){ }
  virtual void         event(kgmObject*, kgmString){ }

  void remove();
  u32 timeout();

private:
  virtual void clear(){}

public:
  kgmIGame* game() const
  {
    return m_game;
  }

  kgmBody* body() const
  {
    return m_body;
  }

  kgmVisual* visual() const
  {
    return m_visual;
  }

  bool valid()   const
  {
    return m_valid;
  }

  bool culled()  const
  {
    return m_culled;
  }

  bool visible() const
  {
    return m_visible;
  }

  bool removed() const
  {
    return m_remove;
  }

  void enable()
  {
    m_valid  = true;
  }

  void disable()
  {
    m_valid  = false;
  }

  void show()
  {
    m_visible = true;
  }

  void hide()
  {
    m_visible = false;
  }

  bool lock() const
  {
    return m_lock;
  }

  void lock(bool l)
  {
    m_lock = l;
  }

  void setId(kgmString s)
  {
    m_id = s;
  }

  kgmString getId()
  {
    return m_id;
  }

  void setClass(kgmString c)
  {
    m_class = c;
  }

  kgmString getClass()
  {
    return m_class;
  }

  void setGroup(u32 g)
  {
    m_group = g;
  }

  u32 getGroup()
  {
    return m_group;
  }

  void setLiving(u32 l)
  {
    m_living = l;
  }

  u32 getLiving() const
  {
    return m_living;
  }

  vec3 position()
  {
    if(m_body)
      return m_body->position();

    return m_position;
  }

  void position(vec3& v)
  {
    if(m_body)
      m_body->translate(v.x, v.y, v.z);
    else
      m_position = v;
  }

  vec3 rotation()
  {
    if(m_body)
      return m_body->rotation();

    return m_rotation;
  }

  void rotation(vec3& r)
  {
    if(m_body)
      m_body->rotate(r.x, r.y, r.z);
    else
      m_rotation = r;
  }

  quat quaternion()
  {
    if(m_body)
      return m_body->quaternion();

    return quat();
  }

  void quaternion(quat& q)
  {
    if(m_body)
      m_body->rotate(q);
  }

  u32 birth()
  {
    return m_birth;
  }
};

#endif // KGMGAMENODE_H
