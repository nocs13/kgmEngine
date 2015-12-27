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

  kgm_ptr<kgmActor> m_gameplayer;

public:
  kgmList< kgm_ptr<kgmUnit> > m_objects;

  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgm_ptr<kgmUnit>);
  bool add(kgm_ptr<kgmActor>);
  bool add(kgm_ptr<kgmEffect>);
  bool add(kgm_ptr<kgmSensor>);
  bool add(kgm_ptr<kgmTrigger>);
  bool remove(kgm_ptr<kgmUnit>);
  bool chooseLogic(kgmString);
  bool isValid(kgmUnit*);

  virtual void build();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void collide(kgmUnit*, kgmUnit*);

  virtual kgmUnit* getObjectById(kgmString&);

  virtual u32 getObjects(kgmList<kgmUnit*>&);
  virtual u32 getObjectsByType(kgmRuntime&, kgmList<kgmUnit*>&);
  virtual u32 getObjectsByClass(kgmRuntime&, kgmList<kgmUnit*>&);

  void  setPlayer(kgm_ptr<kgmActor> gp)
  {
    m_gameplayer.reset();

    m_gameplayer = gp;
  }

  kgm_ptr<kgmActor> getPlayer()
  {
    return m_gameplayer;
  }

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
