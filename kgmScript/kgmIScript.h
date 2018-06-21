#pragma once
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"

class kgmIScript: public kgmObject
{
 public:
  virtual bool  load(kgmString id) = 0;
  virtual bool  set(kgmString name, void (*fn)(void*)) = 0;
  virtual bool  args(kgmString fmt, ...) = 0;
  virtual bool  resl(kgmString fmt, ...) = 0;
  virtual void* call(kgmString name, kgmString fmt, ...) = 0;

  virtual void  setX(void*) = 0;
  virtual void* getX()      = 0;
};
