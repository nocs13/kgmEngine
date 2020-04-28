#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmIApp
{
public:
  virtual ~kgmIApp() {}

  virtual s32  exec(s32, s8**) = 0;
  virtual void abort()         = 0;
};
