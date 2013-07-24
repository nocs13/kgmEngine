#ifndef KGMILOGIC_H
#define KGMILOGIC_H

#include "../kgmBase/kgmString.h"

template <class T> class kgmList;

class kgmActor;
class kgmSensor;
class kgmTrigger;
class kgmGameObject;

class kgmILogic
{
public:
  virtual bool add(kgmActor*)                          = 0;
  virtual bool add(kgmSensor*)                         = 0;
  virtual bool add(kgmTrigger*)                        = 0;
  virtual bool add(kgmGameObject*)                     = 0;
  virtual bool isvalid(kgmGameObject*)                 = 0;

  virtual void update(u32)                             = 0;
  virtual void collide(kgmGameObject*, kgmGameObject*) = 0;

  virtual kgmGameObject* getObjectById(kgmString)                    = 0;
  virtual u32            getObjectsByType(kgmRuntime&,
                                          kgmList<kgmGameObject*>&)  = 0;
  virtual u32            getObjectsByClass(kgmRuntime&,
                                           kgmList<kgmGameObject*>&) = 0;
};

#endif // KGMILOGIC_H
