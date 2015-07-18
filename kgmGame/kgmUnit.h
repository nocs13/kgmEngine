#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

/*TODO: Must switch to kgmGameNode. */

class kgmIGame;

#define KGM_UNIT(o_class)                                           \
  KGM_OBJECT(o_class)                                                    \
  public:                                                                \
  static  __stdcall o_class* New(kgmIGame* g){ return new o_class(g); }  \
  private:


class kgmUnit : public kgmObject
{
  KGM_OBJECT(kgmUnit);

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

  kgmUnit*          m_parent;
  kgmList<kgmUnit*> m_childs;

protected:
  kgmBody*       m_body;
  kgmVisual*     m_visual;

public:
  enum Type
  {
    Unit,
    Mesh,
    Actor,
    Camera,
    Effect,
    Sensor,
    Trigger
  };

  typedef kgmUnit* (*Generate)(kgmIGame*);

  static kgmTab<kgmString, Generate> g_typ_objects;

  //update variables for control from outside

  kgmList<kgmVariable> m_variables;

#ifdef EDITOR
  static kgmList<kgmString> g_list_sensors;
  static kgmList<kgmString> g_list_effects;
  static kgmList<kgmString> g_list_units;

  virtual void eupdate()
  {

  }

  virtual void resetToVariables()
  {
    for(int i = 0; i < m_variables.length(); i++)
      m_variables[i].sync(kgmVariable::SyncToLink);
  }
#endif

protected:
  virtual ~kgmUnit();

public:
  kgmUnit(kgmIGame* g = null);

  virtual void         init() = 0;
  virtual void         update(u32 mls);
  virtual void         event(kgmObject*, kgmString){ }
  virtual kgmBody*     getBody(){   return m_body;   }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  kgmIGame* game() const { return m_game;   }

  bool valid()   const { return m_valid;    }
  bool culled()  const { return m_culled;   }
  bool visible() const { return m_visible;  }
  bool removed() const { return m_remove;   }

  void enable()  { m_valid  = true;   }
  void disable() { m_valid  = false;  }
  void show()    { m_visible = true;  }
  void hide()    { m_visible = false; }

  void remove();

  void timeout(u32 t){ m_timeout = t; }

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
    if(m_body)
      m_body->translate(v.x, v.y, v.z);
  }

  void setRotation(vec3& r)
  {
    if(m_body)
      m_body->rotate(r.x, r.y, r.z);
  }

  void setQuaternion(quat& q)
  {
    if(m_body)
      m_body->rotate(q);
  }

  void setParent(kgmUnit* a)
  {
    m_parent = a;
  }

  kgmUnit* getParent()
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

  static void unitRegister(kgmString id, Type type, kgmUnit*(*create)(kgmIGame*))
  {
    g_typ_objects.add(id, create);

#ifdef EDITOR
    switch((int)type)
    {
    case Unit:
      g_list_units.add(id);
      break;
    case Effect:
      g_list_effects.add(id);
      break;
    case Sensor:
      g_list_sensors.add(id);
      break;
    default:
      break;
    }
#endif
  }

  static void unitUnregister()
  {
    g_typ_objects.clear();

#ifdef EDITOR
    g_list_units.clear();
    g_list_effects.clear();
    g_list_sensors.clear();
#endif
  }

private:
  virtual void clear(){}
};

#endif // KGMGAMEOBJECT_H
