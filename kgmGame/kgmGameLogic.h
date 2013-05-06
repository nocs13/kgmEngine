#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmActor.h"

class kgmGameLogic : public kgmObject
{
public:

  class AI
  {
  public:
    virtual void ai(kgmActor* a){}
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
  kgmTab<kgmString, AI*> m_ais;
  kgmList<kgmActor*>     m_actors;
  kgmList<Sensor*>       m_sensors;
  kgmList<Trigger*>      m_triggers;

public:
    kgmGameLogic();

    void clear();
    void addAI(kgmString, AI*);
    void addActor(kgmActor*);
    void addSensor(Sensor*);
    void addTrigger(Trigger*);

    void onInput(int, int);

    virtual void update(u32 milliseconds);
};

#endif // KGMGAMELOGIC_H
