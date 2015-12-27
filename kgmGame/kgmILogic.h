#ifndef KGMILOGIC_H
#define KGMILOGIC_H

#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmPointer.h"

template <class T> class kgmList;

class kgmUnit;
class kgmActor;
class kgmEffect;
class kgmSensor;
class kgmTrigger;

class kgmILogic
{
public:
  virtual bool add(kgm_ptr<kgmUnit>)    = 0;
  virtual bool add(kgm_ptr<kgmActor>)   = 0;
  virtual bool add(kgm_ptr<kgmEffect>)  = 0;
  virtual bool add(kgm_ptr<kgmSensor>)  = 0;
  virtual bool add(kgm_ptr<kgmTrigger>) = 0;

  virtual bool remove(kgm_ptr<kgmUnit>) = 0;

  virtual void clear()                     = 0;
  virtual void build()                     = 0;
  virtual void update(u32)                 = 0;
  virtual void collide(kgmUnit*, kgmUnit*) = 0;

  virtual kgmUnit*  getObjectById(kgmString&)             = 0;
  virtual u32       getObjects(kgmList<kgmUnit*>&)        = 0;
  virtual u32       getObjectsByType(kgmRuntime&,
                                     kgmList<kgmUnit*>&)  = 0;
  virtual u32       getObjectsByClass(kgmRuntime&,
                                      kgmList<kgmUnit*>&) = 0;

  virtual void      setPlayer(kgm_ptr<kgmActor>)          = 0;
  virtual kgm_ptr<kgmActor> getPlayer()                   = 0;
};

#endif // KGMILOGIC_H
