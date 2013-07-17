#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmActor.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

class kgmGameLogic : public kgmILogic, public kgmObject
{
public:
  enum ACTION
  {
    ACTION_INPUT,
    ACTION_OBJECT
  };

  class LogicOfLevel
  {
    kgmGameLogic* m_logic;

  public:
    LogicOfLevel(kgmGameLogic* l)
      :m_logic(null)
    {
      m_logic = l;
    }

    virtual void init()   = 0;
    virtual void exit()   = 0;
    virtual void update() = 0;
  };

  //kgmList<kgmActor*>       m_actors;
  //kgmList<kgmSensor*>      m_sensors;
  //kgmList<kgmTrigger*>     m_triggers;
  kgmList<kgmGameObject*>  m_objects;

  kgmActor*                m_gameplayer;
  //kgmActor*               m_player;
  LogicOfLevel*            m_levlogic;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmActor*);
  bool add(kgmGameObject*);
  bool add(kgmSensor*);
  bool add(kgmTrigger*);
  bool chooseLogic(kgmString);

  virtual void init();
  virtual void action(ACTION, void* src, int arg){}
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void collide(kgmGameObject*, kgmGameObject*);
  virtual kgmGameObject* getObjectById(kgmString);
  virtual LogicOfLevel*  getLogic(kgmString);

  kgmActor* gameplayer(u8 i)
  {
    return m_gameplayer;
  }
};

#endif // KGMGAMELOGIC_H
