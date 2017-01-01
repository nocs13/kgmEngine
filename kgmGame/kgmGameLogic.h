#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmUnit.h"
#include "kgmActor.h"
#include "kgmEffect.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

class kgmGameLogic : public kgmILogic, public kgmObject
{

  kgmActor* m_gameplayer = null;

public:
  kgmList<kgmGameNode*> m_objects;

  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmGameNode*);
  bool remove(kgmGameNode*);
  bool chooseLogic(kgmString);
  bool isValid(kgmGameNode*);

  virtual void build();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void collide(kgmGameNode*, kgmGameNode*);

  virtual kgmGameNode* getObjectById(kgmString&);

  virtual u32 getObjects(kgmList<kgmGameNode*>&);
  virtual u32 getObjectsByClass(kgmString&, kgmList<kgmGameNode*>&);

  void  setPlayer(kgmActor* gp)
  {
    m_gameplayer = gp;
  }

  kgmActor* getPlayer() const
  {
    return m_gameplayer;
  }

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
