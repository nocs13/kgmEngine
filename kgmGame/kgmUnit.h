#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmPhysics/kgmObstacle.h"
#include "../kgmGraphics/kgmVisual.h"

#include "../kgmGraphics/kgmIGraphics.h"
#include "../kgmPhysics/kgmIPhysics.h"

#include "kgmPBody.h"
#include "kgmGNode.h"

#define KGM_UNIT(o_class)                                             \
  public:                                                             \
  static kgmUnit* gen_unit(kgmIGame* g) { return new o_class(g); }    \
  private:


class kgmIGame;
class kgmSound;
class kgmTerrain;

class kgmUnit : public kgmObject
{
  KGM_OBJECT(kgmUnit);

public:
  struct Action;

  typedef void (*ActionCallback)(kgmIGame*, kgmUnit*, Action*);

  struct Action
  {
    kgmString id;
    u32       time;
    u32       state;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };

  kgmList<kgmVariable> m_variables;

private:
  kgmIGame* m_game = null;

  u32       m_id;
  kgmString m_name;
  kgmString m_class;

  bool m_lock;
  bool m_valid;
  bool m_remove;
  bool m_culled;
  bool m_visible;

  u32 m_group;
  u32 m_birth;
  u32 m_living;

  mtx4 m_transform;

  box m_bound;

protected:

  kgmPBody* m_body = null;
  kgmGNode* m_node = null;

public:
  kgmUnit(kgmIGame* g = null);
  ~kgmUnit();

  virtual void start();

  virtual void finish()
  { }

  virtual void event(kgmUnit*, kgmString)
  { }

  virtual void update(u32);

  void remove();
  u32  timeout();

  kgmIGraphics::INode* getNode() const { return m_node; }
  kgmBody*             getBody() const { return m_body; }

  void setNode(kgmGNode* n);
  void setBody(kgmPBody* b);

private:
  virtual void clear();

public:
  kgmIGame* game() const
  {
    return m_game;
  }

  box   getBound()
  {
    return m_bound;
  }

  void  setBound(box b)
  {
    m_bound = b;
  }

  bool valid() const
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

  void setId(u32 c)
  {
    m_id = c;
  }

  u32 getId()
  {
    return m_id;
  }

  void setName(kgmString s)
  {
    m_name = s;
  }

  kgmString getName()
  {
    return m_name;
  }

  void setClass(kgmString s)
  {
    m_class = s;
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
    return m_transform.position();
  }

  void position(vec3& v)
  {
    m_transform.translate(v);
  }

  vec3 rotation()
  {
    return m_transform.rotation();
  }

  void rotation(vec3& r)
  {
    m_transform.from_euler(r.x, r.y, r.z);
  }

  mtx4 transform()
  {
    return m_transform;
  }

  u32 birth()
  {
    return m_birth;
  }

};

#endif // KGMGAMEOBJECT_H
