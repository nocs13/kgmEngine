#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmActor.h"

class kgmGameLogic : public kgmObject
{
public:
  enum ACTION
  {
      ACTION_INPUT,
      ACTION_OBJECT
  };

  class Sensor
  {
    kgmActor* actor;

  public:
    Sensor(kgmActor* a)
    {
      actor = a;
    }

    virtual void sense(){}
  };

  class Trigger
  {

  };

public:
  kgmList<kgmActor*>      m_actors;
  kgmList<Sensor*>        m_sensors;
  kgmList<Trigger*>       m_triggers;

  kgmActor*               m_gameplayer;

  kgmActor*               m_player;

public:
    kgmGameLogic();
    virtual ~kgmGameLogic();

    void clear();
    bool add(kgmActor* a);
    bool add(Sensor*);
    bool add(Trigger*);

    virtual void action(ACTION, void* src, int arg){}
    virtual void input(int, int);
    virtual void update(u32 milliseconds);

    virtual kgmActor*  createActor(kgmString t){
      return null;
    }

    virtual kgmObject* createGameObject(kgmString t){
      return null;
    }

    kgmActor* gameplayer(u8 i){
        return m_gameplayer;
    }
};

#endif // KGMGAMELOGIC_H
