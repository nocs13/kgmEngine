#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmUnit.h"
#include "kgmEffect.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

class kgmGameLogic : public kgmILogic, public kgmObject
{

public:
  kgmTab<kgmString, kgmUnit*> m_objects;
  //kgmMap<kgmString, kgmUnit*> m_maps;
  kgmUnit* m_gameplayer;
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

  virtual kgmUnit* getObject(kgmString&);

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
