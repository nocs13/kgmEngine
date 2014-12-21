#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmActor.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

//#include <vector>

class kgmGameLogic : public kgmILogic, public kgmObject
{
public:
  kgmList<kgmGameObject*>    m_objects;
  kgmList<kgmGameObject*>    m_inputs;
  kgmList<kgmGameObject*>    m_trush;

  kgmActor*                  m_gameplayer;

  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmActor*);
  bool add(kgmSensor*);
  bool add(kgmTrigger*);
  bool chooseLogic(kgmString);
  bool isValid(kgmGameObject*);
  bool addGameplayer(kgmActor*);
  bool delGameplayer(kgmActor*);


  virtual void build();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void action(ACTION, kgmObject*, kgmString);
  virtual void collide(kgmGameObject*, kgmGameObject*);
  virtual kgmGameObject* getObjectById(kgmString);
  virtual u32            getObjects(kgmList<kgmGameObject*>&);
  virtual u32            getObjectsByType(kgmRuntime&,
                                          kgmList<kgmGameObject*>&);
  virtual u32            getObjectsByClass(kgmRuntime&,
                                           kgmList<kgmGameObject*>&);

  kgmActor* gameplayer(u8 i)
  {
    return m_gameplayer;
  }

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
