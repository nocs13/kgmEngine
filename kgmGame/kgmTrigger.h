#ifndef KGMTRIGGER_H
#define KGMTRIGGER_H

#include "kgmGameObject.h"

class kgmTrigger : public kgmGameObject
{
  KGM_OBJECT(kgmTrigger);

  enum Type
  {
    TypeNone,
    TypeAnd,
    TypeXor,
    TypeOr
  };

private:
  kgmArray<bool> chanels;
  kgmGameObject* destination;

public:
  kgmTrigger();

  bool setCount(u32 c)
  {
    if(c > 32)
      return false;

    chanels.alloc(c);

    for(int i = 0; i < c; i++)
      chanels[i] = false;

    return true;
  }

  void set(u32 c, bool p)
  {
    if(chanels[c] == p)
      return;

    chanels[c] = p;

    for(int i = 0; i < c; i++)
    {
      if(chanels[i] == false)
        return;
    }

    if(destination)
      destination->event(this, "on");
  }

  bool get(u32 c)
  {
    return chanels[c];
  }

  void setDestination(kgmGameObject* o)
  {

  }

  virtual void triggering(){}
};

#endif // KGMTRIGGER_H
