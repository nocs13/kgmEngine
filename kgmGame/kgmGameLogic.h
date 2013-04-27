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
    virtual void logic(kgmActor* a){}
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

public:
  kgmTab<kgmString, AI*> m_ais;
  kgmList<kgmActor*>     m_actors;
  kgmList<Sensor*>       m_sensors;

public:
    kgmGameLogic();

    void clear();

    virtual void update(u32 milliseconds);
};

#endif // KGMGAMELOGIC_H
