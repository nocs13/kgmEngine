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
  //kgmList<kgmActor*>       m_actors;
  //kgmList<kgmSensor*>      m_sensors;
  //kgmList<kgmTrigger*>     m_triggers;
  kgmList<kgmGameObject*>  m_objects;

  kgmActor*                m_gameplayer;
  //kgmActor*               m_player;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmActor*);
  bool add(kgmGameObject*);
  bool add(kgmSensor*);
  bool add(kgmTrigger*);
  bool chooseLogic(kgmString);
  bool isvalid(kgmGameObject*);

  virtual void prepare();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void action(ACTION, kgmObject*, kgmString);
  virtual void collide(kgmGameObject*, kgmGameObject*);
  virtual kgmGameObject* getObjectById(kgmString);
  virtual u32            getObjectsByType(kgmRuntime&,
                                          kgmList<kgmGameObject*>&);
  virtual u32            getObjectsByClass(kgmRuntime&,
                                           kgmList<kgmGameObject*>&);

  kgmActor* gameplayer(u8 i)
  {
    return m_gameplayer;
  }
};

#endif // KGMGAMELOGIC_H
