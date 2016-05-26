#ifndef KGMILOGIC_H
#define KGMILOGIC_H

#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmPointer.h"

template <class T> class kgmList;

class kgmIGame;

class kgmUnit;
class kgmActor;
class kgmEffect;
class kgmSensor;
class kgmTrigger;

class kgmILogic
{
public:
  virtual bool add(kgmUnit*)    = 0;
  virtual bool remove(kgmUnit*) = 0;

  virtual void clear()                     = 0;
  virtual void build()                     = 0;
  virtual void update(u32)                 = 0;
  virtual void collide(kgmUnit*, kgmUnit*) = 0;

  virtual kgmUnit*  getObjectById(kgmString&)             = 0;
  virtual u32       getObjects(kgmList<kgmUnit*>&)        = 0;
  virtual u32       getObjectsByClass(const char*,
                                      kgmList<kgmUnit*>&) = 0;

  virtual void      setPlayer(kgmActor*)          = 0;
  virtual kgmActor* getPlayer()                   = 0;
};

#endif // KGMILOGIC_H
