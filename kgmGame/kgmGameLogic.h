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

  class AI
  {
  public:
    virtual void update(kgmActor* a, u32 milliseconds) = 0;
    virtual void input(kgmActor* a, u32 btn, int state) = 0;
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
  kgmTab<kgmString, AI*>  m_ais;
  kgmTab<kgmActor*, AI*>  m_actors;
  kgmList<Sensor*>        m_sensors;
  kgmList<Trigger*>       m_triggers;

  kgmActor*               m_gameplayer;

  kgmActor*               m_player;

public:
    kgmGameLogic();
    virtual ~kgmGameLogic();

    void clear();
    bool add(kgmString, AI*);
    bool add(kgmActor* a);
    bool add(Sensor*);
    bool add(Trigger*);

    virtual void action(ACTION, void* src, int arg){}
    virtual void input(int, int);
    virtual void update(u32 milliseconds);

    kgmActor* gameplayer(u8 i){
        return m_gameplayer;
    }
};

#endif // KGMGAMELOGIC_H
