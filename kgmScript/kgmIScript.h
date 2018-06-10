#pragma once
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"

class kgmIScript: public kgmObject
{
 public:
  virtual bool  load(kgmString id) = 0;
  virtual bool  set(kgmString name, void* fn) = 0;
  virtual void* call(kgmString name, kgmString fmt, ...) = 0;
};
