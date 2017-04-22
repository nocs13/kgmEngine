#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmTab.h"
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
  kgmTab<kgmString, kgmUnit*> m_objects;
  kgmMap<kgmString, kgmUnit*> m_maps;


  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmUnit*);
  bool remove(kgmUnit*);
  bool chooseLogic(kgmString);
  bool isValid(kgmUnit*);

  virtual void build();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void collide(kgmUnit*, kgmUnit*);

  virtual kgmUnit* getObjectById(kgmString&);

  virtual u32 getObjects(kgmList<kgmUnit*>&);
  virtual u32 getObjectsByClass(kgmString&, kgmList<kgmUnit*>&);

  void  setPlayer(kgmActor* gp)
  {
    m_gameplayer = gp;
  }

  kgmActor* getPlayer()
  {
    return m_gameplayer;
  }

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
