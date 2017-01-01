#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"
#include "kgmGameNode.h"

class kgmIGame;
class kgmSound;

class kgmUnit : public kgmGameNode
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

private:
  Action m_action;

  static kgmTab<kgmString, ActionCallback> g_list_action;

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

  kgmList<kgmVariable> m_variables;

#ifdef EDITOR
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
  kgmUnit(kgmIGame* g = null);
  ~kgmUnit();

  void update(u32);

  kgmString action() const
  {
    return m_action.id;
  }

  void action(kgmString a)
  {
    m_action.id = a;
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

  static bool getAvailableActions(kgmList<kgmString>& actions)
  {
    if(g_list_action.length() < 1)
      return false;

    for(int i = 0; i < g_list_action.length(); i++)
    {
      kgmString id;
      ActionCallback call;

      g_list_action.get(i, id, call);

      actions.add(id);
    }

    return true;
  }

  static void setActionCallback(kgmString action, kgmUnit::ActionCallback callback)
  {
    if(g_list_action.hasKey(action))
    {
      g_list_action[action] = callback;

      return;
    }

    g_list_action.add(action, callback);
  }

  static kgmUnit::ActionCallback getActionCallback(kgmString action)
  {
    if(!g_list_action.hasKey(action))
      return null;

    return g_list_action[action];
  }
};

#endif // KGMGAMEOBJECT_H
