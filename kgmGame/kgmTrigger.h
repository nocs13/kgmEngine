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
    if(c >= chanels.length() || chanels[c] == p)
      return;

    bool prev = state();

    chanels[c] = p;

    bool cur = state();

    if(prev != cur)
    {
      if(destination)
      {
        if(cur)
          destination->event(this, "on");
        else
          destination->event(this, "off");
      }
    }
  }

  bool get(u32 c)
  {
    return chanels[c];
  }

  bool state()
  {
    for(int i = 0; i < chanels.length(); i++)
    {
      if(chanels[i] == false)
        return false;
    }

    return true;
  }

  void setDestination(kgmGameObject* o)
  {
    destination = o;
  }

  virtual void triggering(){}
};

#endif // KGMTRIGGER_H
