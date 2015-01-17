#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmTime.h"

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

#include "kgmGameObject.h"
#include "kgmGamePhysics.h"

class kgmActor: public kgmGameObject
{
  KGM_GO_OBJECT(kgmActor);

public:
  struct Action
  {
    kgmString name;

    virtual void action()
    {

    }
  };

  struct Input
  {
    u32        input;
    u32        input1;
    u32        input2;
    u32        status;
    kgmString  state;
  };

  class State
  {
  public:
    kgmString id;
    kgmString type;
    kgmString action;
    kgmString switchto;
    kgmString switchfrom;

    u32       priopity;
    u32       timeout;
    u32       stime;

    kgmSound*     sound;
    kgmAnimation* animation;
    u32           fstart, fend;
  };

  struct Animation
  {
    kgmAnimation* animation;
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
  virtual ~kgmActor();

  virtual void init();
  virtual void exit();
  virtual void update(u32);
  virtual void input(u32, int);
  virtual void action(kgmString& s);

  void add(kgmDummy* m)
  {
    if(m)
    {
      m_dummies.add(m);
      m->increment();
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
      m_visual->setAnimation(a);
    }
  }

  bool      setState(kgmString s, bool force = false);
  kgmString getState() const { if(m_state) return m_state->id; }

#ifdef EDITOR
  u32       getStatesCount() { return m_states.length(); }
  kgmString getStateName(u32 i) { if(i < m_states.length()) return m_states[i]->id; }
#endif

  // options
  void setOption(kgmString key, kgmString value)
  {
    kgmString opt;

    if(m_options.get(key, opt))
    {
      m_options[key] = value;
    }
    else
    {
      m_options.add(key, value);
    }
  }

  kgmString getOption(kgmString key)
  {
    kgmString opt;

    m_options.get(key, opt);

    return opt;
  }
};

typedef kgmList<kgmActor*> kgmActors;
typedef kgmList<kgmActor*> kgmActorList;
