#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmIGame;

#define KGM_GO_OBJECT(o_class)                                           \
  KGM_OBJECT(o_class)                                                    \
  public:                                                                \
  static  o_class*     New(kgmIGame* g){ return new o_class(g); }        \
  private:


class kgmGameObject : public kgmObject
{
  KGM_GO_OBJECT(kgmGameObject);

private:
  kgmIGame* m_game;

  kgmString m_id;
  kgmString m_class;

  bool      m_valid;
  bool      m_remove;
  bool      m_culled;
  bool      m_visible;

  u32       m_group;
  u32       m_birth;
  u32       m_timeout;

protected:
  vec3           m_position;
  vec3           m_rotation;
  quat           m_quaternion;

  kgmBody*       m_body;
  kgmVisual*     m_visual;

  kgmGameObject*      m_parent;

public:
  enum GoType
  {
    GoActor,
    GoSensor,
    GoObject
  };

  typedef kgmGameObject* (*GenGo)(kgmIGame*);

  static kgmTab<kgmString, GenGo> g_typ_objects;

  kgmList<kgmVariable> m_variables; //update variables for control from outside

#ifdef EDITOR
  static kgmList<kgmString> g_list_objects;
  static kgmList<kgmString> g_list_sensors;
  static kgmList<kgmString> g_list_actors;

  virtual void eupdate()
  {

  }

  virtual void resetToVariables()
  {
    for(int i = 0; i < m_variables.length(); i++)
      m_variables[i].sync(kgmVariable::SyncToLink);
  }
#endif

public:
  kgmGameObject(kgmIGame* g = null);
  virtual ~kgmGameObject();

  virtual void         init(){}
  virtual void         update(u32 mls);
  virtual void         event(kgmObject*, kgmString){ }
  virtual kgmBody*     getBody(){   return m_body;   }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  kgmIGame* game()  { return m_game;     }

  bool valid()      { return m_valid;    }
  bool culled()     { return m_culled;   }
  bool visible()    { return m_visible;  }
  bool removed()    { return m_remove;   }

  void enable()     { m_valid  = true;   }
  void disable()    { m_valid  = false;  }
  void show()       { m_visible = true;  }
  void hide()       { m_visible = false; }

  void remove();

  void timeout(u32 t){ m_timeout = t;    }

  void setId(kgmString s)
  {
    m_id = s;
  }

  kgmString getId()
  {
    return m_id;
  }

  void setGroup(u32 g)
  {
    m_group = g;
  }

  u32 getGroup()
  {
    return m_group;
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
    m_parent = a;
  }

  kgmGameObject* getParent()
  {
    return m_parent;
  }

  u32 birth()
  {
    return m_birth;
  }

  void setParameter(kgmString name, kgmString value)
  {
    for(int i = 0; i < m_variables.length(); ++i)
    {
      kgmVariable& var = m_variables[i];

      if(var.getName() == name)
        var.fromString(value);
    }
  }

  static void goRegister(kgmString id, GoType type, kgmGameObject*(*create)(kgmIGame*))
  {
    g_typ_objects.add(id, create);

#ifdef EDITOR
    switch((int)type)
    {
    case GoActor:
      g_list_actors.add(id);
      break;
    case GoSensor:
      g_list_sensors.add(id);
      break;
    case GoObject:
      g_list_objects.add(id);
      break;
    default:
      break;
    }
#endif
  }

private:
  virtual void         clear(){}
};

#endif // KGMGAMEOBJECT_H
