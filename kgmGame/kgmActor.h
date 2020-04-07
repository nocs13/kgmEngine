#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmString.h"

#include "../kgmGraphics/kgmVisual.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmDummy.h"
#include "../kgmGraphics/kgmCamera.h"

#include "../kgmMedia/kgmSound.h"

#include "../kgmPhysics/kgmBody.h"

#include "kgmGamePhysics.h"
#include "kgmUnit.h"

class kgmActor: public kgmUnit
{
  KGM_OBJECT(kgmActor);

public:
  struct Action;

  //typedef kgmCallback<void, kgmIGame*, kgmActor*, Action*> ActionCallback;
  typedef void (*ActionCallback)(kgmIGame*, kgmActor*, Action*);

  static kgmTab<kgmString, ActionCallback> g_actions;

  struct Action
  {
    kgmString id;
    u32       time;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };

  struct Input
  {
    u32        input;
    u32        input1;
    u32        input2;
    u32        status;
    kgmString  state;
  };

  struct State
  {
    kgmString id;
    kgmString type;
    kgmString switchto;
    kgmString switchfrom;

    u32       priopity;
    u32       timeout;
    u32       stime;

    kgmSound*      sound     = null;
    kgmAnimation*  animation = null;
    u32           fstart, fend;

    Action action;
  };

  struct Animation
  {
    kgmAnimation* animation = null;
    u32           start;
    u32           end;
  };

public:
  u32     m_uid;
  u32     m_type;
  u32     m_bearing;

  s32     m_group;
  s32     m_health;
  s32     m_attack;
  s32     m_defence;
  s32     m_evasion;
  s32     m_accuracy;

  mtx4    m_transform;
  mtx4    m_dvisual;      //visual transform relative to object transform

  kgmList<State*>     m_states;
  kgmList<Input>      m_inputs;
  kgmList<Input>      m_ainputs;
  kgmList<kgmDummy*>  m_dummies;


  kgmAnimation*       m_animation;
  kgmSkeleton*        m_skeleton;
  State*              m_state;
  bool                m_gameplayer;

  kgmTab<kgmString, kgmString> m_options;

public:
  kgmActor(kgmIGame* g = null);
  ~kgmActor();

  virtual void init();
  virtual void exit();
  virtual void update(u32);
  virtual void input(u32, int);

  void action(Action* a);

  void add(kgmDummy* m)
  {
    if(m)
    {
      m_dummies.add(m);
    }
  }

  kgmDummy* dummy(kgmString id)
  {
    for(int i = 0; i < m_dummies.length(); i++)
    {
      if(m_dummies[i]->m_id == id)
        return m_dummies[i];
    }

    return null;
  }

  void add(u32 btn, u32 stat, kgmString state, u32 btn1 = 0, u32 btn2 = 0, bool active = false)
  {
    Input inp;
    inp.input  = btn;
    inp.input1 = btn1;
    inp.input2 = btn2;
    inp.state  = state;
    inp.status = stat;

    if(active)
      m_ainputs.add(inp);
    else
      m_inputs.add(inp);
  }

  void setAnimation(kgmAnimation* a)
  {
    if(a)
    {
      m_animation = a;
    }
  }

  bool      setState(kgmString s, bool force = false);
  kgmString getState() const { if(m_state) return m_state->id; return ""; }

#ifdef EDITOR
  u32       getStatesCount() { return m_states.length(); }
  kgmString getStateName(u32 i) { if(i < m_states.length()) return m_states[i]->id; return ""; }
#endif

  // options
  void setOption(kgmString key, kgmString value)
  {
    m_options.set(key, value);
  }

  kgmString getOption(kgmString key)
  {
    kgmTab<kgmString, kgmString>::iterator i = m_options.get(key);

    if (i.isEnd())
      return kgmString();

    return i.data();
  }
};

typedef kgmList<kgmActor*> kgmActors;
typedef kgmList<kgmActor*> kgmActorList;
