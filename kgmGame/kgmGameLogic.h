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

public:
  //kgmList<kgmActor*>       m_actors;
  //kgmList<kgmSensor*>      m_sensors;
  //kgmList<kgmTrigger*>     m_triggers;
  kgmList<kgmGameObject*>  m_objects;

  kgmActor*               m_gameplayer;
  //kgmActor*               m_player;

public:
    kgmGameLogic();
    virtual ~kgmGameLogic();

    void clear();
    bool add(kgmActor*);
    bool add(kgmGameObject*);
    bool add(kgmSensor*);
    bool add(kgmTrigger*);

    virtual void action(ACTION, void* src, int arg){}
    virtual void input(int, int);
    virtual void update(u32 milliseconds);
    virtual void collide(kgmGameObject*, kgmGameObject*);

    virtual kgmGameObject* createGameObject(kgmString t);

    kgmActor* gameplayer(u8 i){
        return m_gameplayer;
    }
};

#endif // KGMGAMELOGIC_H
