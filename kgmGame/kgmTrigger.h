#ifndef KGMTRIGGER_H
#define KGMTRIGGER_H

#include "kgmUnit.h"

class kgmTrigger : public kgmUnit
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
  kgmString      target;

public:
  kgmTrigger(kgmIGame* g = null);

  void init() {}
  void exit() {}

  bool setCount(u32 c)
  {
    if(c > 32)
      return false;

    chanels.alloc(c);

    for(u32 i = 0; i < c; i++)
      chanels[i] = false;

    return true;
  }

  u32 getCount()
  {
    return chanels.length();
  }

  void set(u32 c, bool p)
  {
    if(c >= (u32) chanels.length() || chanels[c] == p)
      return;

    bool prev = state();

    chanels[c] = p;

    bool cur = state();

    if(prev != cur)
    {
      if(cur)
        sendToTarget("on");
      else
        sendToTarget("off");
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

  void setTarget(kgmString o)
  {
    target = o;
  }

  kgmString getTarget()
  {
    return target;
  }

  virtual void triggering(){}

private:
  void sendToTarget(kgmString);
};

#endif // KGMTRIGGER_H
