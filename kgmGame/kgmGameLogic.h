#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmUnit.h"
#include "kgmActor.h"
#include "kgmEffect.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

//#include <vector>

class kgmGameLogic : public kgmILogic, public kgmObject
{

  kgmActor*                  m_gameplayer;

public:
  kgmList<kgmGameObject*>    m_objects;

  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmUnit*);
  bool add(kgmActor*);
  bool add(kgmEffect*);
  bool add(kgmSensor*);
  bool add(kgmTrigger*);
  bool remove(kgmGameObject*);
  bool chooseLogic(kgmString);
  bool isValid(kgmGameObject*);

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

  void      setPlayer(kgmActor* gp) { m_gameplayer = gp; }
  kgmActor* getPlayer() { return m_gameplayer; }


private:
  void trush();
};

#endif // KGMGAMELOGIC_H
