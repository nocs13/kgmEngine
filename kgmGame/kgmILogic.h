#ifndef KGMILOGIC_H
#define KGMILOGIC_H

#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmPointer.h"

template <class T> class kgmList;

class kgmIGame;

class kgmGameNode;
class kgmActor;

class kgmILogic
{
public:
  virtual bool add(kgmGameNode*)    = 0;
  virtual bool remove(kgmGameNode*) = 0;

  virtual void clear()                     = 0;
  virtual void build()                     = 0;
  virtual void update(u32)                 = 0;

  virtual void collide(kgmGameNode*, kgmGameNode*) = 0;

  virtual kgmGameNode*  getObjectById(kgmString&)             = 0;
  virtual u32           getObjects(kgmList<kgmGameNode*>&)    = 0;
  virtual u32           getObjectsByClass(kgmString&,
                                          kgmList<kgmGameNode*>&) = 0;

  virtual void      setPlayer(kgmActor*)          = 0;
  virtual kgmActor* getPlayer()                   = 0;
};

#endif // KGMILOGIC_H
